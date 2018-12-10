#include <Awesome/SDK.hpp>

#include <Editor/Core/Window.hpp>

namespace ed
{
	// Use this function because static Icon must be create after Application create.
	const QIcon& TextIcon()
	{
		static QIcon sTextIcon = QIcon("assetsEditor/icons/PictureIcon.png");
		return sTextIcon;
	}

	const QIcon& MeshIcon()
	{
		static QIcon sMeshIcon = QIcon("assetsEditor/icons/MeshIcon.png");
		return sMeshIcon;
	}

	const QIcon& MaterialIcon()
	{
		static QIcon sMaterialIcon = QIcon("assetsEditor/icons/MaterialIcon.png");
		return sMaterialIcon;
	}

	const QIcon& FontIcon()
	{
		static QIcon sFontIcon = QIcon("assetsEditor/icons/FontIcon.png");
		return sFontIcon;
	}

	AssetsView::QAssetsList::QAssetsList(AssetsView& assetsView, const FolderView& folderView, QWidget* parent) :
		QListWidget(parent),
		m_folderView{ folderView },
		m_assetsView{ assetsView }
	{
		setViewMode(QListView::ListMode);
		setDragEnabled(true);
		setAcceptDrops(true);
		setDropIndicatorShown(true);

		connect(this, &QAssetsList::itemChanged, this, &QAssetsList::ForceAssetsRename);
	}

	uint32_t AssetsView::QAssetsList::AddItem(const QString& objName)
	{
		QAssetsItem* item = new QAssetsItem(objName, this);

		item->type = AssetsType::Texture;
		item->setIcon(TextIcon());

		aw::TexelBuffer texelBuffer = aw::sdk::TextureLoader::Load(("assets/" + objName).toStdString());

		aw::SampledTexture textureUnique;
		textureUnique.Create(
			vk::ImageType::e2D,
			vk::Format::eR8G8B8A8Unorm,
			std::move(texelBuffer),
			vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
			vk::ImageAspectFlagBits::eColor
		);
		aw::SampledTextureHandle texture = aw::sdk::RenderSystem::GetResourceManager().Register(objName.toStdString(), std::move(textureUnique));

		item->index = aw::sdk::RenderSystem::RegisterTexture(texture);

		return item->index;
	}

	void AssetsView::QAssetsList::ForceAssetsRename(QListWidgetItem* item)
	{
		if (dynamic_cast<QAssetsItem*>(item)->type != AssetsType::Material)
			return;

		if(!item->text().size())
			item->setText("DefaultMat.mat");

		if (!item->text().endsWith(".mat"))
			item->setText(item->text() + ".mat");
	}

	void AssetsView::QAssetsList::dragEnterEvent(QDragEnterEvent* event)
	{
		if (event->source() == m_folderView.GetTreeSource())
		{
			QString objName = event->mimeData()->text();

			if(objName.endsWith(".png", Qt::CaseInsensitive) || objName.endsWith(".jpg", Qt::CaseInsensitive) ||
				objName.endsWith(".jpeg", Qt::CaseInsensitive) || objName.endsWith(".ttf", Qt::CaseInsensitive))
				event->accept();
		}
		else
			event->ignore();
	}

	void AssetsView::QAssetsList::dragMoveEvent(QDragMoveEvent* event)
	{
		event->accept();
	}

	void AssetsView::QAssetsList::dropEvent(QDropEvent* event)
	{
		QString objName = event->mimeData()->text();
		objName = objName.remove(0, objName.indexOf("assets/") + 7); // 7 == "assets/" size

		if (!findItems(objName, Qt::MatchFlag::MatchCaseSensitive).size()) // Already load ?
			AddItem(objName);

		event->accept();
	}

	QMimeData* AssetsView::QAssetsList::mimeData(const QList<QListWidgetItem *> items) const
	{
		if (items.size() > 1)
		{
			LOG_WARNING("AssetsView::QAssetsList::mimeData item list > 1!");
			return nullptr;
		}

		QMimeData* mime = new QMimeData;
		mime->setText(items.front()->text());

		return mime;
	}

	void AssetsView::QAssetsList::contextMenuEvent(QContextMenuEvent* event)
	{
		QMenu* menu = new QMenu;
		QMenu* creMenu = menu->addMenu("&Create");

		QMenu* matMenu = creMenu->addMenu("&Material");
		creMenu->addMenu(matMenu);

		QAction* litMatAct = new QAction("&Lit Assets", this);
		litMatAct->setStatusTip("Create a new Lit Assets");
		connect(litMatAct, &QAction::triggered, &m_assetsView, &AssetsView::AddLitMaterial);
		matMenu->addAction(litMatAct);

		QAction* unlitMatAct = new QAction("&Unlit Assets", this);
		unlitMatAct->setStatusTip("Create a new Unlit Assets");
		connect(unlitMatAct, &QAction::triggered, &m_assetsView, &AssetsView::AddUnlitMaterial);
		matMenu->addAction(unlitMatAct);

		QAction* PBRLitMatAct = new QAction("&PBR Lit Assets", this);
		PBRLitMatAct->setStatusTip("Create a new PBR Lit Assets");
		connect(PBRLitMatAct, &QAction::triggered, &m_assetsView, &AssetsView::AddPBRLitMaterial);
		matMenu->addAction(PBRLitMatAct);

		QAction* desAct = new QAction("&Destroy", this);
		desAct->setStatusTip("Delete an Asset");
		connect(desAct, &QAction::triggered, &m_assetsView, &AssetsView::Remove);
		menu->addAction(desAct);

		menu->exec(event->globalPos());
		delete menu;
	}

	AssetsView::QAssetsItem::QAssetsItem(const QString& name, QAssetsList* view) : QListWidgetItem(name, view)
	{
	}

	AssetsView::AssetsView(Window& win) :
		QDockWidget("Assets", &win),
		m_inspec{ win.inspec }
	{
		setObjectName("Assets View");

		QAssetsList* assetsList = new QAssetsList(*this, win.folderView);

		Init(assetsList);
		setWidget(assetsList);

		QListWidget::connect(assetsList, &QListWidget::clicked, this, &AssetsView::LeftClick);
	}

	void AssetsView::Init(QAssetsList* assetsList)
	{
		m_assetsList = assetsList;

		QAssetsItem* item = new QAssetsItem("Quad.awmesh", assetsList);
		item->type = AssetsType::Mesh;
		item->setIcon(MeshIcon());
		item->index = static_cast<aw::uint32>(aw::sdk::PrimitivesIndex::Quad);

		item = new QAssetsItem("Cube.awmesh", assetsList);
		item->type = AssetsType::Mesh;
		item->setIcon(MeshIcon());
		item->index = static_cast<aw::uint32>(aw::sdk::PrimitivesIndex::Cube);

		item = new QAssetsItem("Sphere.awmesh", assetsList);
		item->type = AssetsType::Mesh;
		item->setIcon(MeshIcon());
		item->index = static_cast<aw::uint32>(aw::sdk::PrimitivesIndex::Sphere);

		item = new QAssetsItem("Capsule.awmesh", assetsList);
		item->type = AssetsType::Mesh;
		item->setIcon(MeshIcon());
		item->index = static_cast<aw::uint32>(aw::sdk::PrimitivesIndex::Capsule);

		item = new QAssetsItem("Cylinder.awmesh", assetsList);
		item->type = AssetsType::Mesh;
		item->setIcon(MeshIcon());
		item->index = static_cast<aw::uint32>(aw::sdk::PrimitivesIndex::Cylinder);

		item = new QAssetsItem("Cone.awmesh", assetsList);
		item->type = AssetsType::Mesh;
		item->setIcon(MeshIcon());
		item->index = static_cast<aw::uint32>(aw::sdk::PrimitivesIndex::Cone);

		item = new QAssetsItem("Hemiphere.awmesh", assetsList);
		item->type = AssetsType::Mesh;
		item->setIcon(MeshIcon());
		item->index = static_cast<aw::uint32>(aw::sdk::PrimitivesIndex::Hemiphere);

		item = new QAssetsItem("NoTexture.awtext", assetsList);
		item->type = AssetsType::Texture;
		item->setIcon(TextIcon());
		item->index = static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::NoTexture);

		item = new QAssetsItem("Black.awtext", assetsList);
		item->type = AssetsType::Texture;
		item->setIcon(TextIcon());
		item->index = static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::Black);

		item = new QAssetsItem("White.awtext", assetsList);
		item->type = AssetsType::Texture;
		item->setIcon(TextIcon());
		item->index = static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::White);

		item = new QAssetsItem("Red.awtext", assetsList);
		item->type = AssetsType::Texture;
		item->setIcon(TextIcon());
		item->index = static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::Red);

		item = new QAssetsItem("Green.awtext", assetsList);
		item->type = AssetsType::Texture;
		item->setIcon(TextIcon());
		item->index = static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::Green);

		item = new QAssetsItem("Blue.awtext", assetsList);
		item->type = AssetsType::Texture;
		item->setIcon(TextIcon());
		item->index = static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::Blue);

		//item = new QAssetsItem("Black Transparent.awtext", assetsList);
		//item->type = AssetsType::Texture;
		//item->setIcon(TextIcon());
		//item->index = static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::BlackTransparent);

		//item = new QAssetsItem("White Transparent.awtext", assetsList);
		//item->type = AssetsType::Texture;
		//item->setIcon(TextIcon());
		//item->index = static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::WhiteTransparent);
	}

	void AssetsView::LeftClick(const QModelIndex &index)
	{
		QAssetsItem* item = reinterpret_cast<QAssetsItem*>(index.internalPointer());

		if (!item || item->type != AssetsType::Material)
			return;

		m_inspec.Clear();

		ed::Material material(item->index, item->materialType);

		MaterialWidget* widget = m_inspec.Inspect(material);

		auto layout = dynamic_cast<MaterialLayout*>(widget->layout());
		layout->name->Update(item->text().toStdString().erase(item->text().size() - 4, 4)); // trick remove .mat

		layout->name->onValueChange = [item](const std::string& name) {
				QString qname = name.c_str();

				if (!qname.endsWith(".mat"))
					qname += ".mat";

				item->setText(qname);
		};
	}

	AssetsView::QAssetsItem* AssetsView::CreateMaterialItem()
	{
		QAssetsList* list = reinterpret_cast<QAssetsList*>(widget());

		QInputDialog dialog;
		dialog.setLabelText("Enter a name:");

		if (!dialog.exec())
			return nullptr;

		QAssetsItem* item = new QAssetsItem(dialog.textValue() + ".mat", list);

		item->type = AssetsType::Material;
		item->setIcon(MaterialIcon());

		return item;
	}

	AssetsView::QAssetsItem* AssetsView::CreateMaterialItem(const std::string& itemName)
	{
		QAssetsList* list = reinterpret_cast<QAssetsList*>(widget());

		QAssetsItem* item = new QAssetsItem(QString(itemName.c_str()) + ".mat", list);

		item->type = AssetsType::Material;
		item->setIcon(MaterialIcon());

		return item;
	}

	uint32_t AssetsView::AddLitMaterial()
	{
		QAssetsItem* item = CreateMaterialItem();

		if (!item)
			return ~0;

		{
			aw::Material material;

			material.SetDiffuse(static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::NoTexture));
			material.SetNormal(static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::BlackTransparent));
			material.SetSpecular(static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::White));
			material.SetHeight(static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::Black));
			material.materialInfo.receiveLights = true;

			aw::sdk::RenderSystem::GetResourceManager().materialManager.CreateMaterial(material, aw::sb::ShaderBuilder::MakeShaderProgram);
			item->index = aw::sdk::RenderSystem::RegisterMaterial(std::move(material));

			item->materialType = MaterialType::Lit;
		}

		item->type = AssetsType::Material;
		item->setIcon(MaterialIcon());

		return item->index;
	}

	uint32_t AssetsView::AddUnlitMaterial()
	{
		QAssetsItem* item = CreateMaterialItem();

		if (!item)
			return ~0;

		{
			aw::Material material;

			material.SetDiffuse(static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::NoTexture));
			material.materialInfo.receiveLights = false;

			aw::sdk::RenderSystem::GetResourceManager().materialManager.CreateMaterial(material, aw::sb::ShaderBuilder::MakeShaderProgram);
			item->index = aw::sdk::RenderSystem::RegisterMaterial(std::move(material));

			item->materialType = MaterialType::Unlit;
		}

		item->type = AssetsType::Material;
		item->setIcon(MaterialIcon());

		return item->index;
	}

	uint32_t AssetsView::AddPBRLitMaterial()
	{
		QAssetsItem* item = CreateMaterialItem();

		if (!item)
			return ~0;

		{
			aw::Material material;

			material.SetDiffuse(static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::NoTexture));
			material.SetNormal(static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::BlackTransparent));
			material.SetMetalicRoughness(static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::BlackTransparent));
			material.SetOcclusion(static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::White));
			material.SetEmissive(static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::Black));
			material.materialInfo.receiveLights = true;

			aw::sdk::RenderSystem::GetResourceManager().materialManager.CreateMaterial(material, aw::sb::ShaderBuilder::MakeShaderProgram);
			item->index = aw::sdk::RenderSystem::RegisterMaterial(std::move(material));

			item->materialType = MaterialType::PBRLit;
		}

		item->type = AssetsType::Material;
		item->setIcon(MaterialIcon());

		return item->index;
	}

	uint32_t AssetsView::AddNamedLitMaterial(const std::string& itemName)
	{
		QAssetsItem* item = CreateMaterialItem(itemName);

		if (!item)
			return ~0;

		{
			aw::Material material;

			material.SetDiffuse(static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::NoTexture));
			material.SetNormal(static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::BlackTransparent));
			material.SetSpecular(static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::White));
			material.SetHeight(static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::Black));
			material.materialInfo.receiveLights = true;

			aw::sdk::RenderSystem::GetResourceManager().materialManager.CreateMaterial(material, aw::sb::ShaderBuilder::MakeShaderProgram);
			item->index = aw::sdk::RenderSystem::RegisterMaterial(std::move(material));

			item->materialType = MaterialType::Lit;
		}

		item->type = AssetsType::Material;
		item->setIcon(MaterialIcon());

		return item->index;
	}

	uint32_t AssetsView::AddNamedUnlitMaterial(const std::string& itemName)
	{
		QAssetsItem* item = CreateMaterialItem(itemName);

		if (!item)
			return ~0;

		{
			aw::Material material;

			material.SetDiffuse(static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::NoTexture));
			material.materialInfo.receiveLights = false;

			aw::sdk::RenderSystem::GetResourceManager().materialManager.CreateMaterial(material, aw::sb::ShaderBuilder::MakeShaderProgram);
			item->index = aw::sdk::RenderSystem::RegisterMaterial(std::move(material));

			item->materialType = MaterialType::Unlit;
		}

		item->type = AssetsType::Material;
		item->setIcon(MaterialIcon());

		return item->index;
	}

	uint32_t AssetsView::AddNamedPBRLitMaterial(const std::string& itemName)
	{
		QAssetsItem* item = CreateMaterialItem(itemName);

		if (!item)
			return ~0;

		{
			aw::Material material;

			material.SetDiffuse(static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::NoTexture));
			material.SetNormal(static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::BlackTransparent));
			material.SetMetalicRoughness(static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::BlackTransparent));
			material.SetOcclusion(static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::White));
			material.SetEmissive(static_cast<aw::uint32>(aw::sdk::DefaultTextureIndex::Black));
			material.materialInfo.receiveLights = true;

			aw::sdk::RenderSystem::GetResourceManager().materialManager.CreateMaterial(material, aw::sb::ShaderBuilder::MakeShaderProgram);
			item->index = aw::sdk::RenderSystem::RegisterMaterial(std::move(material));

			item->materialType = MaterialType::PBRLit;
		}

		item->type = AssetsType::Material;
		item->setIcon(MaterialIcon());

		return item->index;
	}

	void AssetsView::Remove()
	{
		QAssetsList* list = reinterpret_cast<QAssetsList*>(widget());

		if (list->selectedItems().size() != 0)
		{
			QAssetsItem* item = dynamic_cast<QAssetsItem*>(list->currentItem());

			if (item->text().endsWith(".awmesh") || item->text().endsWith(".awtext"))
				return;

			if (item->type == AssetsType::Texture)
				aw::sdk::RenderSystem::UnRegisterTexture(item->index);
			// TODO: ResourceMgr.Delete(item->type, item->index)

			delete list->takeItem(list->row(item));
		}
	}

	QString AssetsView::GetAssetName(aw::uint32 index, AssetsType type) const
	{
		QAssetsList* list = reinterpret_cast<QAssetsList*>(widget());

		for (int i = 0; i < list->count(); i++)
		{
			auto item = reinterpret_cast<QAssetsItem*>(list->item(i));

			if (item->index == index && item->type == type)
			{
				return item->text();
			}
		}

		return "";
	}

	aw::uint32 AssetsView::GetAssetIndex(const QString& str) const
	{
		QAssetsList* list = reinterpret_cast<QAssetsList*>(widget());

		for (int i = 0; i < list->count(); i++)
		{
			auto item = reinterpret_cast<QAssetsItem*>(list->item(i));

			if (item->text() == str)
			{
				return item->index;
			}
		}

		return ~0;
	}

	uint32_t AssetsView::GetItemCount() const
	{
		QAssetsList* list = reinterpret_cast<QAssetsList*>(widget());

		return static_cast<uint32_t>(list->count());
	}

	AssetsView::QAssetsItem& AssetsView::GetItemAt(uint32_t index)
	{
		QAssetsList* list = reinterpret_cast<QAssetsList*>(widget());

		// QListWidgetItem* item = list->item(index);
		auto item = reinterpret_cast<QAssetsItem*>(list->item(index));

		ASSERT(item == nullptr, "item == nullptr");

		return *item;
	}
}