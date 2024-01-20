#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
std::string get_file_contents(const char* filename);
class ShaderClass
{
public:
	GLuint ID;
	ShaderClass(const char* vertexFile,const  char* fragmentFile);
	ShaderClass(const char* vertexFile, const char* geometryShader, const  char* fragmentFile);
	
	void Activate();
	void Delete();
	void compileErrors(unsigned int shader, const char* type);

};



