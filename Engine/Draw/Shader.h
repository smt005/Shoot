#pragma once

#include <string>

class Shader
{
public:
	static bool getShaderProgram(unsigned int& program, const std::string& vertexLink, const std::string& fragmentLink);

};
