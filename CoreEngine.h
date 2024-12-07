#pragma once
#include "Window.h"
#include "Renderer.h"
#include <chrono>
class CoreEngine
{
	const float DT = 0.01f;
	const float move_speed = 0.125f;
	
private:
	Window m_window;
	Renderer m_renderer;
	float m_time = 0;

	std::chrono::time_point<std::chrono::high_resolution_clock> m_lastFrameTime;

	bool m_isRunning = false;
	void processInput(GLFWwindow* window);
	void loop();
public:

	void Start();
	CoreEngine() :m_window(Window(1920, 1080, "Test")), m_renderer(Renderer())
	{
		m_window.Init();
	}
};

