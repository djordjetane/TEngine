#pragma once
#include "precomp.h"

int APIENTRY WinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR	  lpCmdLine,
		int		  nCmdShow )
{

	#ifdef _DEBUG
	//*********************************
	//** Console Debug Redirection
	
		AllocConsole();

		HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
		int hCrt = _open_osfhandle((long long) handle_out, _O_TEXT);
		FILE* hf_out = _fdopen(hCrt, "w");
		setvbuf(hf_out, NULL, _IONBF, 1);
		*stdout = *hf_out;

		HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
		hCrt = _open_osfhandle((long long)handle_in , _O_TEXT);
		FILE* hf_in = _fdopen(hCrt, "r");
		setvbuf(hf_in, NULL, _IONBF, 128);
		*stdin = *hf_in;	
	//*********************************
	#endif

	Engine::Log::Init();	
	LOG_CORE_INFO("Logging from Core macro");
	LOG_INFO("Logging from Client macro");

	Engine::Application* app = Engine::CreateApplication();
	if( !app->Init() )
	{
		LOG_CORE_FATAL("Failed to initialize App");
		return 1;
	}

	while( app->Run() )
	{
		
	}	

	app->Shutdown();

	delete app;

	return 0;
}