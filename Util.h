#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct Camera
{
	glm::vec3 position;
	glm::quat lookAt;
};