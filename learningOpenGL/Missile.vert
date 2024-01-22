#version 330 core
layout (location = 0) in vec3 aPos;


uniform mat4 camMatrix;
uniform mat4 translationMatrix;
uniform mat4 positionMatrix;




void main()
{
	gl_Position = camMatrix * translationMatrix * positionMatrix *vec4(aPos, 1.0);
}