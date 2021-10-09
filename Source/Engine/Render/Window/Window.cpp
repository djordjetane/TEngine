#include <precomp.h>
#include "Window.h"

Engine::Window::~Window()
{
	DestroyWindow( m_hwnd );
}

LRESULT CALLBACK Engine::Window::ms_WndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	switch( msg )
	{
		case WM_CREATE:
		{
			// Window created event
			auto* const params = reinterpret_cast< LPCREATESTRUCT >( lparam );
			auto* const window = reinterpret_cast< Engine::Window* const >( params->lpCreateParams );
			
			SetWindowLongPtr( hwnd, GWLP_USERDATA, reinterpret_cast< LONG_PTR >( window ) );
			window->onCreate();
			break;
		}

		case WM_DESTROY:
		{
			// Window destoryed event
			auto* const window = reinterpret_cast< Engine::Window* const >( GetWindowLongPtr( hwnd, GWLP_USERDATA ) );
			window->onDestroy();
			PostQuitMessage( 0 );
			break;
		}

		default:
			return DefWindowProc( hwnd, msg, wparam, lparam );
	}

	return 0;
}

bool Engine::Window::Init()
{	

	// Disabling fullscreen in debug
#ifdef _DEBUG
	WindowData::Fullscreen = false;	
#endif


	//Setting up WNDCLASSEX
	WNDCLASSEX wc{};
	wc.cbSize = sizeof( WNDCLASSEX );
	wc.hbrBackground = ( HBRUSH )COLOR_WINDOW;
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
	wc.hInstance = GetModuleHandle( nullptr );
	wc.lpszClassName = WindowData::ClassName;
	wc.lpfnWndProc = ms_WndProc;

	// Check if registration failed
	if( !RegisterClassEx( &wc ) )
	{
		LOG_CORE_ERROR("Failed to register CLASSEX");
		return false;
	}		

	auto [width, height] = m_GetDimensions();
	

	//Creation of the window handler
	m_hwnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		WindowData::ClassName,
		WindowData::TitleName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		nullptr,
		nullptr,
		nullptr,
		this );


	// If window handle creation fail
	if( !m_hwnd )
	{
		LOG_CORE_ERROR("Failed to create window");
		return false;
	}		


	if (WindowData::Fullscreen)
    {
        SetWindowLong(m_hwnd, GWL_STYLE, 0);
    }

	m_running = true;

	ShowWindow( m_hwnd, SW_SHOW );

	return true;	
}

void Engine::Window::Update()
{
	MSG msg{};
	while( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) > 0 )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	onUpdate();

	Sleep( 1 );
}

bool Engine::Window::IsRunning()
{
	return m_running;
}

HWND Engine::Window::GetHWND()
{
	return m_hwnd;
}

void Engine::Window::onCreate()
{

}

void Engine::Window::onUpdate()
{
}

void Engine::Window::onDestroy()
{
	m_running = false;
}


constexpr std::pair<int,int> Engine::Window::m_GetDimensions() 
{

	if (WindowData::Fullscreen)
    {
        HMONITOR hmon = MonitorFromWindow(m_hwnd,
            MONITOR_DEFAULTTONEAREST);
        MONITORINFO moni = { sizeof(moni) };
        GetMonitorInfo(hmon, &moni);

        const auto width  = moni.rcMonitor.right - moni.rcMonitor.left;
        const auto height = moni.rcMonitor.bottom - moni.rcMonitor.top;

		return {width, height};
    }

	

	// Non-client area dimensions
	// AdjustWindowRectEx API for adjusting
	RECT rect = { 0, 0, WindowData::Height, WindowData::Width };
	AdjustWindowRectEx( &rect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW );

	const auto width = rect.right - rect.left;
	const auto height = rect.bottom - rect.top;
	
	return {width, height};
}