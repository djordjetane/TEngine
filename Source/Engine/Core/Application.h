#pragma once

namespace Engine{

	class Window;
	class Graphics;

	class Application
	{
	public:
		Application() = default;
		~Application();

		bool Init();
		bool Run();
		bool Shutdown();

	private:
		Window* m_Window;
		Graphics* m_Graphics;
	};

	// To be defined in the Game
	Application* CreateApplication();
} // Engine

