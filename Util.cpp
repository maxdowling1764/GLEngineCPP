#include "Util.h"
#include "glm.hpp"
#include "Util.h"

std::string read_file(std::string& filepath)
{
	std::ifstream ifs(filepath);
	std::string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	std::cout << content << std::endl;
	return content;
}

glm::vec4 piecewise_lerp(std::vector<std::pair<float, glm::vec4>>& points, float a, size_t start)
{
	for (size_t i = start; i < points.size() - 1; i++)
	{
		if (a < points[i + 1].first && a > points[i].first)
		{
			return glm::mix(points[i].second, points[i + 1].second, a);
		}
	}
	return glm::vec4(0);
}

// It is assumed that T is a linear order and control points are sorted in ascending order
glm::vec4 piecewise_lerp(std::vector<std::pair<float, glm::vec4>>& points, float a)
{
	return piecewise_lerp(points, a, 0);
}