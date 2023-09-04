#include "CoreEngine.h"
#include <iostream>
#include "Camera.h"
void CoreEngine::processInput(GLFWwindow* window)
{
	glm::vec3 camPos = m_renderer.GetActiveCamera()->GetPosition();
	glm::vec3 deltaPos = glm::vec3(0.0f);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		deltaPos = move_speed * glm::vec3(0.0f, 0.0f, 1.0f);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		deltaPos = move_speed * glm::vec3(0.0f, 0.0f, -1.0f);
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		deltaPos = move_speed * glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		deltaPos = move_speed * glm::vec3(-1.0f, 0.0f, 0.0f);
	}
	else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		deltaPos = move_speed * glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		deltaPos = move_speed * glm::vec3(0.0f, -1.0f, 0.0f);
	}
	camPos = camPos + deltaPos;
	m_renderer.GetActiveCamera()->SetPosition(camPos);
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
