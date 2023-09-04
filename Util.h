#pragma once
#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <string>
std::string read_file(std::string& filepath);
struct Camera
{
	glm::vec3 position;
	glm::quat lookAt;
};