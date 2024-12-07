#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include <glm.hpp>
std::string read_file(std::string& filepath);

template <typename T>
std::vector<T> read_file_raw(std::string filepath)
{
	std::ifstream file = std::ifstream(filepath, std::ios::binary);
	std::vector<T> res;
	if (!file.is_open())
	{
		std::cerr << "Unable to open file: " << filepath << std::endl;
		return res;
	}

	file.seekg(0, std::ios::end);
	std::streampos fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	assert(fileSize % sizeof(T) == 0);
	std::size_t n = fileSize / sizeof(T);
	res = std::vector<T>(n);
	file.read(reinterpret_cast<char*>(res.data()), fileSize);
	file.close();
	return res;
}

glm::vec4 piecewise_lerp(std::vector<std::pair<float, glm::vec4>>& points, float a, size_t start);

// It is assumed that T is a linear order and control points are sorted in ascending order
glm::vec4 piecewise_lerp(std::vector<std::pair<float, glm::vec4>>& points, float a);

