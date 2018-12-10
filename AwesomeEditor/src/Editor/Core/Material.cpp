#include <Editor/Core/Material.hpp>
#include <Editor/Core/EditorApplication.hpp>
#include <Awesome/SDK/System/RenderSystem.hpp>

namespace ed
{
	aw::Material& Material::GetMaterial() const
	{
		return aw::sdk::RenderSystem::GetMaterial(m_index);
	};

	DropWidget* Material::NewDiffuseDropWidget()
	{
		DropWidget* drop = new DropWidget;
		drop->AddExtension(".awtext"); drop->AddExtension(".png"); drop->AddExtension(".jpeg"); drop->AddExtension(".jpg");

		EditorApplication* editor = dynamic_cast<EditorApplication*>(QApplication::instance());
		drop->AddSources(editor->GetWindow().assetsView.widget());

		aw::uint32 textureIndex = GetMaterial().GetDiffuse();

		drop->setText(editor->GetWindow().GetAssetName(textureIndex, AssetsView::AssetsType::Texture));

		Material copyMat(*this);

		drop->onValueChange = [copyMat, editor](const std::string& str)
		{
			aw::uint32 textureIndex = editor->GetWindow().GetAssetIndex(QString(str.c_str()));

			if (textureIndex == ~0)
				return;

			aw::Context::GetDevice().WaitIdle();

			aw::Material& material = copyMat.GetMaterial();

			material.SetDiffuse(textureIndex);
			material.SetSampledTexture(
				aw::sdk::RenderSystem::GetTexture(textureIndex),
				material.GetDiffuseBinding()
			);
		};

		return drop;
	}

	DropWidget* Material::NewNormalDropWidget()
	{
		DropWidget* drop = new DropWidget;
		drop->AddExtension(".awtext"); drop->AddExtension(".png"); drop->AddExtension(".jpeg"); drop->AddExtension(".jpg");

		EditorApplication* editor = dynamic_cast<EditorApplication*>(QApplication::instance());
		drop->AddSources(editor->GetWindow().assetsView.widget());

		aw::uint32 textureIndex = GetMaterial().GetNormal();

		drop->setText(editor->GetWindow().GetAssetName(textureIndex, AssetsView::AssetsType::Texture));

		Material copyMat(*this);

		drop->onValueChange = [copyMat, editor](const std::string& str)
		{
			aw::uint32 textureIndex = editor->GetWindow().GetAssetIndex(QString(str.c_str()));

			if (textureIndex == ~0)
				return;

			aw::Context::GetDevice().WaitIdle();

			aw::Material& material = copyMat.GetMaterial();

			material.SetNormal(textureIndex);
			material.SetSampledTexture(
				aw::sdk::RenderSystem::GetTexture(textureIndex),
				material.GetNormalBinding()
			);
		};

		return drop;
	}

	DropWidget* Material::NewSpecularDropWidget()
	{
		DropWidget* drop = new DropWidget;
		drop->AddExtension(".awtext"); drop->AddExtension(".png"); drop->AddExtension(".jpeg"); drop->AddExtension(".jpg");

		EditorApplication* editor = dynamic_cast<EditorApplication*>(QApplication::instance());
		drop->AddSources(editor->GetWindow().assetsView.widget());

		aw::uint32 textureIndex = GetMaterial().GetSpecular();

		drop->setText(editor->GetWindow().GetAssetName(textureIndex, AssetsView::AssetsType::Texture));

		Material copyMat(*this);

		drop->onValueChange = [copyMat, editor](const std::string& str)
		{
			aw::uint32 textureIndex = editor->GetWindow().GetAssetIndex(QString(str.c_str()));

			if (textureIndex == ~0)
				return;

			aw::Context::GetDevice().WaitIdle();

			aw::Material& material = copyMat.GetMaterial();

			material.SetSpecular(textureIndex);
			material.SetSampledTexture(
				aw::sdk::RenderSystem::GetTexture(textureIndex),
				material.GetSpecularBinding()
			);
		};

		return drop;
	}

	DropWidget* Material::NewHeightDropWidget()
	{
		DropWidget* drop = new DropWidget;
		drop->AddExtension(".awtext"); drop->AddExtension(".png"); drop->AddExtension(".jpeg"); drop->AddExtension(".jpg");

		EditorApplication* editor = dynamic_cast<EditorApplication*>(QApplication::instance());
		drop->AddSources(editor->GetWindow().assetsView.widget());

		aw::uint32 textureIndex = GetMaterial().GetHeight();

		drop->setText(editor->GetWindow().GetAssetName(textureIndex, AssetsView::AssetsType::Texture));

		Material copyMat(*this);

		drop->onValueChange = [copyMat, editor](const std::string& str)
		{
			aw::uint32 textureIndex = editor->GetWindow().GetAssetIndex(QString(str.c_str()));

			if (textureIndex == ~0)
				return;

			aw::Context::GetDevice().WaitIdle();

			aw::Material& material = copyMat.GetMaterial();

			material.SetHeight(textureIndex);
			material.SetSampledTexture(
				aw::sdk::RenderSystem::GetTexture(textureIndex),
				material.GetHeightBinding()
			);
		};

		return drop;
	}

	DropWidget* Material::NewMetalicRoughnessDropWidget()
	{
		DropWidget* drop = new DropWidget;
		drop->AddExtension(".awtext"); drop->AddExtension(".png"); drop->AddExtension(".jpeg"); drop->AddExtension(".jpg");

		EditorApplication* editor = dynamic_cast<EditorApplication*>(QApplication::instance());
		drop->AddSources(editor->GetWindow().assetsView.widget());

		aw::uint32 textureIndex = GetMaterial().GetMetalicRoughness();

		drop->setText(editor->GetWindow().GetAssetName(textureIndex, AssetsView::AssetsType::Texture));

		Material copyMat(*this);

		drop->onValueChange = [copyMat, editor](const std::string& str)
		{
			aw::uint32 textureIndex = editor->GetWindow().GetAssetIndex(QString(str.c_str()));

			if (textureIndex == ~0)
				return;

			aw::Context::GetDevice().WaitIdle();

			aw::Material& material = copyMat.GetMaterial();

			material.SetMetalicRoughness(textureIndex);
			material.SetSampledTexture(
				aw::sdk::RenderSystem::GetTexture(textureIndex),
				material.GetMetalicRoughnessBinding()
			);
		};

		return drop;
	}

	DropWidget* Material::NewOcclusionDropWidget()
	{
		DropWidget* drop = new DropWidget;
		drop->AddExtension(".awtext"); drop->AddExtension(".png"); drop->AddExtension(".jpeg"); drop->AddExtension(".jpg");

		EditorApplication* editor = dynamic_cast<EditorApplication*>(QApplication::instance());
		drop->AddSources(editor->GetWindow().assetsView.widget());

		aw::uint32 textureIndex = GetMaterial().GetOcclusion();

		drop->setText(editor->GetWindow().GetAssetName(textureIndex, AssetsView::AssetsType::Texture));

		Material copyMat(*this);

		drop->onValueChange = [copyMat, editor](const std::string& str)
		{
			aw::uint32 textureIndex = editor->GetWindow().GetAssetIndex(QString(str.c_str()));

			if (textureIndex == ~0)
				return;

			aw::Context::GetDevice().WaitIdle();

			aw::Material& material = copyMat.GetMaterial();

			material.SetOcclusion(textureIndex);
			material.SetSampledTexture(
				aw::sdk::RenderSystem::GetTexture(textureIndex),
				material.GetOcclusionBinding()
			);
		};

		return drop;
	}

	DropWidget* Material::NewEmissiveDropWidget()
	{
		DropWidget* drop = new DropWidget;
		drop->AddExtension(".awtext"); drop->AddExtension(".png"); drop->AddExtension(".jpeg"); drop->AddExtension(".jpg");

		EditorApplication* editor = dynamic_cast<EditorApplication*>(QApplication::instance());
		drop->AddSources(editor->GetWindow().assetsView.widget());

		aw::uint32 textureIndex = GetMaterial().GetEmissive();

		drop->setText(editor->GetWindow().GetAssetName(textureIndex, AssetsView::AssetsType::Texture));

		Material copyMat(*this);

		drop->onValueChange = [copyMat, editor](const std::string& str)
		{
			aw::uint32 textureIndex = editor->GetWindow().GetAssetIndex(QString(str.c_str()));

			if (textureIndex == ~0)
				return;

			aw::Context::GetDevice().WaitIdle();

			aw::Material& material = copyMat.GetMaterial();

			material.SetEmissive(textureIndex);
			material.SetSampledTexture(
				aw::sdk::RenderSystem::GetTexture(textureIndex),
				material.GetEmissiveBinding()
			);
		};

		return drop;
	}
}
