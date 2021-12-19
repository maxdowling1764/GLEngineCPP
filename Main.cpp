#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "CoreEngine.h"

int main()
{
	CoreEngine engine = CoreEngine();
	engine.Start();
}