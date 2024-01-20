#version 330 core


out vec4 FragColor;

in vec4 givenColor;

uniform sampler2D tex0;


void main()
{
   FragColor = givenColor;
}