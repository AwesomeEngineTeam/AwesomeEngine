#pragma once

#ifndef AWESOME_EDITOR_CORE_MATERIAL_GUARD
#define AWESOME_EDITOR_CORE_MATERIAL_GUARD

#include <Awesome/Renderer/Resource/Material.hpp>
#include <Editor/Widgets/DropWidget.hpp>

namespace ed
{
	enum class MaterialType : aw::uint8
	{
		Lit,
		Unlit,
		PBRLit
	};

	class Material
	{
		aw::uint32 m_index;
		MaterialType m_type;

	public:
		aw::Material& GetMaterial() const;

		Material() = default;
		inline explicit Material(aw::uint32 index, MaterialType type);

		inline MaterialType GetType() const;

		DropWidget* NewDiffuseDropWidget();
		DropWidget* NewNormalDropWidget();
		DropWidget* NewSpecularDropWidget();
		DropWidget* NewHeightDropWidget();
		DropWidget* NewMetalicRoughnessDropWidget();
		DropWidget* NewOcclusionDropWidget();
		DropWidget* NewEmissiveDropWidget();
	};
}

#include <Editor/Core/Material.inl>

#endif // GUARD