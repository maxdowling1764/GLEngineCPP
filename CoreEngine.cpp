#include "CoreEngine.h"
#include <iostream>
#include "Camera.h"
void CoreEngine::processInput(GLFWwindow* window)
{
	glm::vec3 cam_pos = m_renderer.GetActiveCamera()->GetPosition();
	glm::vec3 cam_forward = m_renderer.GetActiveCamera()->GetForward();
	glm::vec3 cam_up = m_renderer.GetActiveCamera()->GetUp();
	glm::vec3 cam_left = glm::cross(cam_up, cam_forward);
	glm::vec3 deltaPos = glm::vec3(0.0f);
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		deltaPos += move_speed * cam_forward;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		deltaPos -= move_speed * cam_forward;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		deltaPos += move_speed * cam_left;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		deltaPos -= move_speed * cam_left;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		deltaPos += move_speed * cam_up;
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		deltaPos -= move_speed * cam_up;
	}
	
	// heading and azimuth
	double d_theta = 0.0f;
	double d_phi = 0.0f;

	// performing updates
	cam_pos += deltaPos;
	glfwGetCursorPos(m_window.GetWindowHandle(), &d_theta, &d_phi);
	d_theta *= 0.005;
	d_phi *= 0.005;

	// enforcing invariants
	cam_forward = glm::normalize(cam_forward - float(d_phi)*cam_up - float(d_theta)*cam_left);
	cam_left = glm::normalize(cam_left - float(d_theta) * cam_forward);
	cam_up = glm::vec3(0, 1, 0);

	glfwSetCursorPos(m_window.GetWindowHandle(), 0, 0);

	// Finalizing to global camera state
	m_renderer.GetActiveCamera()->SetPosition(cam_pos);
	m_renderer.GetActiveCamera()->SetForward(cam_forward);
	m_renderer.GetActiveCamera()->SetUp(cam_up);
	
}

void CoreEngine::loop()
{
	m_renderer.Init();
	while (m_isRunning && !glfwWindowShouldClose(m_window.GetWindowHandle()))
	{
		// Process Inputs
		processInput(m_window.GetWindowHandle());
		m_time += DT;
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
	glfwSetInputMode(m_window.GetWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	loop();
}
