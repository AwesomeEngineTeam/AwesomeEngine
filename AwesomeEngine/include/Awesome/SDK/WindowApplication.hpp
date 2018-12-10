#pragma once

#ifndef AWESOME_SDK_WINDOW_APPLICATION_GUARD
#define AWESOME_SDK_WINDOW_APPLICATION_GUARD

#include <Awesome/Renderer/Control/RenderWindow.hpp>
#include <Awesome/Windowing.hpp>

#include <Awesome/SDK/BaseApplication.hpp>

namespace aw
{
	namespace sdk
	{
		class WindowApplication : public BaseApplication
		{
		protected:
			std::unique_ptr<RenderWindow> m_mainWindow = nullptr;

		public:
			inline WindowApplication(uint32 width = 1280u, uint32 height = 800u, const char* title = "Window Application", uint16 posX = 50, uint16 posY = 50, uint8 monitorId = 1, WindowStyleBit style = WindowStyleBit::Default);
			~WindowApplication() override = default;

			inline void Start() override;
			inline void Update() override;
			inline void End() override;
		};
	}
}

#include <Awesome/SDK/WindowApplication.inl>

#endif // GUARD