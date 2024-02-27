#pragma once
#include <string>
#include "Mesh.h"
#include <iostream>
#include <fstream>

namespace ModelParser
{
	// It is assumed here that UV_VERT will be identical for all possible
	// UV texture lookups (i.e. normals, specularity, displaecment, parallax, etc)
	enum Token
	{
		VERT, FACE, UV_VERT, NORM_VERT, NONE
	};

	bool parse_vec3(std::istringstream& s, glm::vec3& dest);
	bool parse_vec2(std::istringstream& s, glm::vec2& dest);
	bool parse_index(std::istringstream& s, Index& dest);
	bool parse_face(std::istringstream& s,
					const std::vector<glm::vec3>& vp,
					const std::vector<glm::vec3>& vn,
					const std::vector<glm::vec2>& vt,
					std::vector<Vertex>& vertices,
					std::vector<Index>& indices);
	bool parse_obj(const std::string& objPath, 
					std::vector<glm::vec3>& vp,
					std::vector<glm::vec3>& vn,
					std::vector<glm::vec2>& vt,
					std::vector<Vertex>& vertices,
					std::vector<Index>& indices);
	bool parse_obj(const std::string& objPath, Mesh& m);
}

