#pragma once
#include "Window.h"
#include "Renderer.h"
class CoreEngine
{
	const float DT = 0.01f;
private:
	Window m_window;
	Renderer m_renderer;
	glm::vec2 m_PrevMousePos;
	glm::vec2 m_MouseVel;
	float m_time = 0;

	bool m_isRunning = false;
	void loop();
public:
	void Start();
	CoreEngine() :m_window(Window(1080, 1080, "Test")), m_renderer(Renderer())
	{
		//m_renderer.Init();
		m_window.Init();
	}
};

