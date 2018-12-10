#pragma once

#ifndef AWESOME_RENDERER_GUARD
#define AWESOME_RENDERER_GUARD

#include <Awesome/Renderer/Control/Context.hpp>
#include <Awesome/Renderer/Control/Device.hpp>
#include <Awesome/Renderer/Control/RenderSurface.hpp>
#include <Awesome/Renderer/Control/RenderWindow.hpp>
#include <Awesome/Renderer/Control/Vulkan.hpp>

#include <Awesome/Renderer/Descriptor/DescriptorPool.hpp>
#include <Awesome/Renderer/Descriptor/DescriptorSet.hpp>
#include <Awesome/Renderer/Descriptor/DescriptorSetLayoutInfo.hpp>

#include <Awesome/Renderer/FramebufferInfo.hpp>

#include <Awesome/Renderer/Framegraph/Framegraph.hpp>
#include <Awesome/Renderer/Framegraph/Pass.hpp>
#include <Awesome/Renderer/Framegraph/RenderArea.hpp>

#include <Awesome/Renderer/GUI/GUIManager.hpp>
#include <Awesome/Renderer/GUI/GUIBase.hpp>
#include <Awesome/Renderer/GUI/GUIBenchmark.hpp>

#include <Awesome/Renderer/Memory/AlignedSubBuffer.hpp>
#include <Awesome/Renderer/Memory/Allocator.hpp>
#include <Awesome/Renderer/Memory/AllocatorMemoryPool.hpp>
#include <Awesome/Renderer/Memory/Buffer.hpp>
#include <Awesome/Renderer/Memory/BufferManager.hpp>
#include <Awesome/Renderer/Memory/BufferMemoryPool.hpp>
#include <Awesome/Renderer/Memory/DeviceMemory.hpp>
#include <Awesome/Renderer/Memory/MemInfo.hpp>
#include <Awesome/Renderer/Memory/SubBuffer.hpp>
#include <Awesome/Renderer/Memory/UniformBuffer.hpp>
#include <Awesome/Renderer/Memory/UniformBufferLayout.hpp>

#include <Awesome/Renderer/Pipeline/PipelineInfo.hpp>
#include <Awesome/Renderer/Pipeline/Program.hpp>
#include <Awesome/Renderer/Pipeline/StaticVertex.hpp>
#include <Awesome/Renderer/Pipeline/StaticVertexAttribute.hpp>
#include <Awesome/Renderer/Pipeline/VertexAttribute.hpp>
#include <Awesome/Renderer/Pipeline/VertexLayout.hpp>

#include <Awesome/Renderer/RenderPassInfo.hpp>
#include <Awesome/Renderer/RenderQueue.hpp>

#include <Awesome/Renderer/Resource/FontAtlas.hpp>
#include <Awesome/Renderer/Resource/Image.hpp>
#include <Awesome/Renderer/Resource/Material.hpp>
#include <Awesome/Renderer/Resource/MaterialManager.hpp>
#include <Awesome/Renderer/Resource/Mesh.hpp>
#include <Awesome/Renderer/Resource/Model.hpp>
#include <Awesome/Renderer/Resource/SampledTexture.hpp>
#include <Awesome/Renderer/Resource/TexelBuffer.hpp>
#include <Awesome/Renderer/Resource/Texture.hpp>

#include <Awesome/Renderer/ShaderBuilder/Shader.hpp>
#include <Awesome/Renderer/ShaderBuilder/ShaderBuilder.hpp>

#endif //GUARD