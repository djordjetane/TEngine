#include "precomp.h"
#include "Render/Graphics/Graphics.h"
#include "Application.h"

Engine::Application::~Application()
{
	delete m_Window;
	delete m_Graphics;
}

bool Engine::Application::Init()
{
	m_Window = new Window();

	if( !m_Window->Init() )
		return false;	
	
	m_Graphics = new Graphics();
	if( !m_Graphics->Init( m_Window->GetHWND()) )
		return false;
	

	return true;
}

bool Engine::Application::Run()
{
	m_Window->Update();
	m_Graphics->Render();

	bool running = m_Window->IsRunning() && m_Graphics->IsRunning();

	return running;
}

bool Engine::Application::Shutdown()
{
	m_Window->onDestroy();
	return true;
}
