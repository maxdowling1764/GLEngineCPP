#include "Util.h"
std::string read_file(std::string& filepath)
{
	std::ifstream ifs(filepath);
	std::string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	std::cout << content << std::endl;
	return content;
	
}

