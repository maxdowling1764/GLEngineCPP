#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Window
{
private:
	std::string m_name;
	int m_width;
	int m_height;
	GLFWwindow* p_window;
	
	int Init();

public:
	GLFWwindow* GetWindowHandle();
	Window(unsigned int w, unsigned int h, std::string name) : m_width(w), m_height(h), m_name(name) 
	{
		Init();
	};
	bool ShouldClose();

};

