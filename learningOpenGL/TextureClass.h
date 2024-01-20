#pragma once
#include <stb/stb_image.h>
#include <glad/glad.h>
#include "ShaderClass.h"

class TextureClass
{
public:
	GLuint ID;
	GLenum type;

	TextureClass(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

	void texUnit(ShaderClass & shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();
};

