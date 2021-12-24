#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
std::string read_file(std::string& filepath);
struct Camera
{
	glm::vec3 position;
	glm::quat lookAt;
};