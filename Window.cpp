#include "Window.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLFWwindow* Window::GetWindowHandle()
{
	return p_window;
}

int Window::Init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	p_window = glfwCreateWindow(m_width, m_height, "Test", NULL, NULL);
	if (p_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(p_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, m_width, m_height);
	glfwSetFramebufferSizeCallback(p_window, framebuffer_size_callback);
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(p_window);;
}
