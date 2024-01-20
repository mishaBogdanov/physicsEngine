#version 330 core


layout (triangles) in;
layout (triangle_strip, max_vertices =3) out;



out vec4 givenColor;


in DATA
{
	mat4 cameraMatrix;
	mat4 given_translation_matrix;
	mat4 posMatrix;

} data_in[];

void main()
{

	vec4 position0 = data_in[0].given_translation_matrix * data_in[0].posMatrix * gl_in[0].gl_Position;
	vec4 position1 = data_in[1].given_translation_matrix * data_in[1].posMatrix * gl_in[1].gl_Position;
	vec4 position2 = data_in[2].given_translation_matrix * data_in[2].posMatrix * gl_in[2].gl_Position;







	vec3 vector0 = vec3(position0.x-position1.x, position0.y-position1.y, position0.z-position1.z );
	vec3 vector1 = vec3(position2.x-position1.x, position2.y-position1.y, position2.z-position1.z);
	vec3 normal = vec3(normalize(cross(vector0, vector1)));
	float color = - dot(normal, vec3(0,0,1));


	gl_Position = data_in[0].cameraMatrix * position0;
	givenColor = vec4(color, color, color, 1.0);
	EmitVertex();

	gl_Position = data_in[1].cameraMatrix * position1;
	givenColor = vec4(color, color, color, 1.0);
	EmitVertex();

	gl_Position = data_in[2].cameraMatrix * position2;
	givenColor = vec4(color, color, color, 1.0);
	EmitVertex();

	EndPrimitive();
	
	
	
	
}

