#pragma once

#ifndef AWESOME_EDITOR_CORE_ASSETS_VIEW_GUARD
#define AWESOME_EDITOR_CORE_ASSETS_VIEW_GUARD

#include <Awesome/Core/Types.hpp>

#include <QtWidgets/qmenu.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qdockwidget.h>

namespace ed
{
	class Window;
	class Inspector;
	class FolderView;

	class AssetsView : public QDockWidget
	{
	public:
		enum class AssetsType : aw::uint8
		{
			Mesh = 1,
			Font = 2,
			Texture = 3,
			Material = 4,
		};

		class QAssetsList;

		QAssetsList* m_assetsList{ nullptr };

		class QAssetsList : public QListWidget
		{
			const FolderView& m_folderView;
			AssetsView& m_assetsView;

		public:
			uint32_t AddItem(const QString& objName);
			void ForceAssetsRename(QListWidgetItem* item);

			QAssetsList(AssetsView& assetsView, const FolderView& folderView, QWidget* parent = nullptr);
			~QAssetsList() = default;

		protected:
			void dragEnterEvent(QDragEnterEvent* event) override;
			void dragMoveEvent(QDragMoveEvent* event) override;
			void dropEvent(QDropEvent* event) override;

			QMimeData* mimeData(const QList<QListWidgetItem *> items) const override;
			void contextMenuEvent(QContextMenuEvent* event) override;
		};

		struct QAssetsItem : QListWidgetItem
		{
			AssetsType type;
			aw::uint32 index = 0u;
			MaterialType materialType;

			QAssetsItem(const QString& name, QAssetsList* view = nullptr);
			~QAssetsItem() = default;
		};

	private:
		Inspector& m_inspec;

		void Init(QAssetsList* assetsList);

		void LeftClick(const QModelIndex &index);
		QAssetsItem* CreateMaterialItem();
		QAssetsItem* CreateMaterialItem(const std::string& itemName);

	public:
		AssetsView(Window& win);
		~AssetsView() = default;

		uint32_t AddLitMaterial();
		uint32_t AddUnlitMaterial();
		uint32_t AddPBRLitMaterial();
		uint32_t AddNamedLitMaterial(const std::string& itemName);
		uint32_t AddNamedUnlitMaterial(const std::string& itemName);
		uint32_t AddNamedPBRLitMaterial(const std::string& itemName);
		void Remove();

		QString GetAssetName(aw::uint32, AssetsType) const;
		aw::uint32 GetAssetIndex(const QString& str) const;

		uint32_t GetItemCount() const;
		QAssetsItem& GetItemAt(uint32_t index);
	};
}

#endif // GUARD