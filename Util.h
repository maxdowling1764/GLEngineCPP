#pragma once
#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <string>
#include <iostream>
#include <vector>
std::string read_file(std::string& filepath);

template <typename T>
std::vector<T> read_file_raw(std::string& filepath)
{
	std::ifstream file(filepath, std::ios::binary);
	std::vector<T> res;
	if (!file.is_open())
	{
		std::cerr << "Unable to open file: " << filepath << std::endl;
		return res;
	}

	file.seekg(0, std::ios::end);
	std::streampos fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	std::size_t n = fileSize / sizeof(T);
	res = std::vector<T>(n);
	file.read(reinterpret_cast<char*>(res.data()), fileSize);
	file.close();
	return res;
}
