#include <Awesome/Renderer/Descriptor/DescriptorPool.hpp>
#include <Awesome/Renderer/Descriptor/DescriptorSetLayoutInfo.hpp>
#include <Awesome/Renderer/Control/Context.hpp>

namespace aw
{
	void DescriptorPool::Create(const vk::DescriptorPoolCreateInfo& createInfo)
	{
		m_handle = Context::GetDevice().Get().createDescriptorPoolUnique(createInfo);
	}

	void DescriptorPool::Create(const std::vector<DescriptorSetLayoutInfo>& createInfos, uint32 maxSets)
	{
		std::vector<vk::DescriptorPoolSize> poolSizes;
		for (const auto& createInfo : createInfos)
		{
			const std::vector<vk::DescriptorPoolSize>& poolSize = createInfo.GetDescriptorPoolSizes();
			poolSizes.insert(poolSizes.end(), poolSize.begin(), poolSize.end());
		}

		uint32 count = uint32(poolSizes.size());

		vk::DescriptorPoolCreateInfo poolInfo(
			vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
			(maxSets == 0 ? count : maxSets),
			count,
			poolSizes.data()
		);

		Create(poolInfo);
	}

	void DescriptorPool::Destroy()
	{
		m_handle.reset();
	}
}