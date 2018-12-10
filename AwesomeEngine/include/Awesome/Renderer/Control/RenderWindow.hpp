#pragma once

#ifndef AWESOME_RENDERER_RENDER_WINDOW_GUARD
#define AWESOME_RENDERER_RENDER_WINDOW_GUARD

#include <Awesome/Renderer/Prerequisites.hpp>
#include <Awesome/Renderer/Control/RenderSurface.hpp>
#include <Awesome/Renderer/Config.hpp>

#include <Awesome/Windowing/Window.hpp>

namespace aw
{
	class RenderWindow
	{
		RenderSurface m_renderSurface{};

		Window m_window;

		std::vector<vk::UniqueCommandBuffer> m_commandBuffers;
		vk::UniqueCommandBuffer m_blitCommandBuffer;

		vk::CommandBuffer m_currentCmdBuffer{};
		uint32 m_currentImageIndex{ ~0u };

		vk::UniqueSemaphore m_imageAvailableSemaphore{};
		vk::UniqueSemaphore m_frameFinishedSemaphore{};
		vk::UniqueSemaphore m_blitFrameSemaphore{};

		vk::UniqueFence m_acquireImageFence{};
		vk::UniqueFence m_presentFence{};

		std::function<void()> m_onResize{};
		std::function<void(uint32, uint32)> m_onOutOfDate;

		bool m_shouldQuit{ false };

	public:
		RenderWindow() = default;
		RenderWindow(const RenderWindow&) = delete;
		RenderWindow(RenderWindow&&) = default;
		AWESOME_RENDERER_API ~RenderWindow();

		inline void Create(uint32 w, uint32 h, const char* title = "Awesome Engine", uint16 posX = 0, uint16 posY = 0, uint8 monitorId = 0, WindowStyleBit style = WindowStyleBit::Default);
		inline void Create(HWND);
		AWESOME_RENDERER_API void Init();

		AWESOME_RENDERER_API bool Draw(vk::Image) const;

		inline const RenderSurface& GetSurface() const;
		AWESOME_RENDERER_API RenderingResources GetNewRenderingResources();

	private:
		inline void OnSurfaceOutOfDate();

	public:
		inline void SetOnOutOfDateCallback(const std::function<void(uint32, uint32)>&);

		AWESOME_RENDERER_API void Resize(uint32 w, uint32 h);

		inline bool ShouldQuit();
		inline void Quit();

		inline const Window* operator->() const;
		inline Window* operator->();

		inline const Window& operator*() const;
		inline Window& operator*();

		RenderWindow& operator=(const RenderWindow&) = delete;
		RenderWindow& operator=(RenderWindow&&) = default;
	};
}

#include <Awesome/Renderer/Control/RenderWindow.inl>

#endif //GUARD