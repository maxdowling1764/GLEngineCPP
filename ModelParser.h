#pragma once
#include <string>
#include "Mesh.h"
#include <iostream>
#include <fstream>

namespace ModelParser
{
	enum TOKEN
	{
		VERT, FACE, NONE
	};
	bool parse_vertex(std::istringstream& s, glm::vec3& dest);
	bool parse_face(std::istringstream& s, Mesh& dest);
	bool parse_obj(std::string& objPath, Mesh& m);
}

