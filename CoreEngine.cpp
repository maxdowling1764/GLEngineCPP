#include "CoreEngine.h"
#include <iostream>
void CoreEngine::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void CoreEngine::UpdateJuliaPos()
{
	int h;
	int w;
	double xpos, ypos;
	glfwGetCursorPos(m_window.GetWindowHandle(), &xpos, &ypos);
	glfwGetFramebufferSize(m_window.GetWindowHandle(), &w, &h);
	m_renderer.SetJuliaPos(glm::vec2(2.0f*(xpos / w) - 1.0f, 2.0f*(ypos / h) - 1.0f));
	//std::cout << "Mouse Position: " << xpos/w << ", " << ypos/h << std::endl;
}

void CoreEngine::loop()
{
	m_renderer.Init();
	while (m_isRunning && !glfwWindowShouldClose(m_window.GetWindowHandle()))
	{
		// Process Inputs
		processInput(m_window.GetWindowHandle());
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		UpdateJuliaPos();
		m_renderer.Update(m_time, DT);
		// Render Here 
		m_renderer.Render();

		// Check events
		glfwSwapBuffers(m_window.GetWindowHandle());
		glfwPollEvents();
		m_time += DT;
		if (m_window.ShouldClose())
		{
			m_isRunning = false;
		}
	}
}

void CoreEngine::Start()
{
	std::cout << "Starting Main Loop" << std::endl;
	m_isRunning = true;
	loop();
}
