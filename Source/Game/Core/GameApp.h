#pragma once



namespace Game{

	class GameApp final : public Engine::Application
	{	
		
	};	

} // Game


Engine::Application* Engine::CreateApplication()
{
	return new Game::GameApp();
}