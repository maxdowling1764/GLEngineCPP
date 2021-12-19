#pragma once
#include "Window.h"
#include "Renderer.h"
class CoreEngine
{
private:
	Window m_window;
	Renderer m_renderer;

	bool m_isRunning = false;
	void loop();
public:
	void Start();
	CoreEngine() :m_window(Window(1920, 1080, "Test")), m_renderer(Renderer())
	{
		m_renderer.Init();
	}
};

