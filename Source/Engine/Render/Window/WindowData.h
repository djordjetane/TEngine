#pragma once

struct WindowData
{
	// inline static const wchar_t* ClassName = "WindowClassName";
	// inline static const wchar_t* ClassName = "WindowClassName";
	inline static const char* ClassName = "WindowClassName";
	inline static const char* TitleName = "WindowTitleName";
	inline static const int Width = 720;	
	inline static const int Height = 1280;
	inline static constexpr float AspectRatio = Height / Width;
	inline static bool Fullscreen = false; // will change width and height if true
	inline static const UINT BufferNum = 3;
};
