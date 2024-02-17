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
}

Mesh parse_obj(std::string& objPath);
