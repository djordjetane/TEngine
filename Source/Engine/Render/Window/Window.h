#pragma once
#include "WindowData.h"

namespace Engine{

	class Window
	{
	public:
		Window() = default;
		~Window();
		// disabling instances	
		Window(const Window& other) = delete;
		void operator=(const Window& other) = delete;

		bool Init();
		void Update();
		bool IsRunning();	
		HWND GetHWND(); // FIXIT: Change to GetNative

		// Events
		void onCreate();
		void onUpdate();
		void onDestroy();

	private:
		HWND m_hwnd;		
		bool m_running;		

		constexpr std::pair<int,int> m_GetDimensions();

		static LRESULT CALLBACK ms_WndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );
			
	};

} // Engine