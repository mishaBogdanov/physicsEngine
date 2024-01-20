#include "ExperimentingWithShaders.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "ShaderClass.h"
#include <stb/stb_image.h>
#include "TextureClass.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <windows.h>
#include "Camera.h"
#include <vector>
#include "Mesh.h"
#include "Model.h"
#include "floor.h"
//#include "World.cpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
//const unsigned int SCR_WIDTH = 1000;
//const unsigned int SCR_HEIGHT = 800;
//const unsigned int xScale = 1;
//bool isConsole = false;

const unsigned int SCR_WIDTH = 200;
const unsigned int SCR_HEIGHT = 100;
const double xScale = 1.8;
bool isConsole = true;


ExperimentingWithShaders::ExperimentingWithShaders() {

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	//GLFW.glfwWindowHint(GLFW.GLFW_VISIBLE, GLFW.GLFW_FALSE);
	//if (isConsole) {
	//	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	//}

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	//std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	//TextureClass tex1("triangleGreggor.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	//ShaderClass shaderProgram("default.vert", "default.geom", "default.frag");
	//ShaderClass shaderProgram2("default.vert", "outline.geom", "outline.frag");

	//VAO VAO1;
	//VAO1.Bind();


	//VBO VBO1(vertices, sizeof(vertices));
	//EBO EBO1(ind);

	//VAO1.LinkAtrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	//VAO1.LinkAtrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//VAO1.LinkAtrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//VAO1.Unbind();
	//VBO1.Unbind();
	//EBO1.Unbind();
	// render loop
	// -----------
	//GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");


	//tex1.texUnit(shaderProgram, "tex0", 0);


	glEnable(GL_DEPTH_TEST);

	Model cyber = Model("TeslaTruck.object", 10);
	Model cyber2 = Model("TeslaTruck.object");

	glm::vec3 velocity = glm::vec3(0, 0, -10);
	glm::vec3 velocity2 = glm::vec3(0, 0, 0);

	cyber.setVelocity(velocity);
	cyber2.setVelocity(velocity2);

	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
	wchar_t* screen = new wchar_t[SCR_WIDTH*SCR_HEIGHT+1];
	float pixels[SCR_WIDTH * SCR_HEIGHT * 3] = { 0 };

	
	//Mesh mesh(vertices2, ind);




	Camera camera(SCR_WIDTH, SCR_HEIGHT * xScale, 1, glm::vec3(0.0f, 0.0f, 2.0f));

	double t = glfwGetTime();
	Floor floor = Floor(10, 100, 0, 0, 0);


	while (!glfwWindowShouldClose(window))
	{
		double deltaT = glfwGetTime() - t;
		t = glfwGetTime();
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// be sure to activate the shader before any calls to glUniform
		//glBindTexture(GL_TEXTURE_2D, texture);


		camera.setMatrix(90.0f, 0.1f, 300.0f);
		




		// update shader uniform
		//VAO1.Bind();

		// render the triangle
		//glDrawElements(GL_TRIANGLES, ind.size(), GL_UNSIGNED_INT, 0);
		//shaderProgram2.Activate();
		//tex1.Bind();
		//camera.setMatrix(90.0f, 0.1f, 100.0f, shaderProgram2);
		//VAO1.Bind();

		//glDrawElements(GL_TRIANGLES, ind.size(), GL_UNSIGNED_INT, 0);

		//std::cout << "r: " << (pixels[0]) << '\n';

		//mesh.Draw(shaderProgram, camera);
		//mesh.Draw(shaderProgram2, camera);
		cyber.update(deltaT);
		cyber.Draw(camera);
		cyber2.update(deltaT);
		cyber2.Draw(camera);
		floor.Draw(camera);



		if (isConsole) {
			glReadPixels(0, 0, SCR_WIDTH, SCR_HEIGHT, GL_RGB, GL_FLOAT, pixels);
			for (int i = 0; i < SCR_WIDTH * SCR_HEIGHT*3; i+=3) {
				int pos=0;
				if (i / 3 / SCR_WIDTH == 0) {
					pos = (SCR_HEIGHT-1) * SCR_WIDTH + i / 3;
					//std::cout << i / 3;
				}
				else {
					pos = (SCR_HEIGHT - (i / 3 / SCR_WIDTH)-1)*SCR_WIDTH + ((i / 3) % SCR_WIDTH);
					//std::cout << pos;
				}
				float colorValue = pixels[i] + pixels[i + 1] + pixels[i + 2];
				if (colorValue < 0.1) {
					screen[pos] = ' ';
				}
				else if (colorValue < 0.6) {
					screen[pos] = ',';
				}
				else if (colorValue < 1.2) {
					screen[pos] = ';';
				}
				else if (colorValue < 1.8) {
					screen[pos] = 'i';
				}
				else if (colorValue < 2.2) {
					screen[pos] = '0';
				}
				else if (colorValue < 2.6) {
					screen[pos] = 0x2592;
				}
				else if (colorValue < 8) {
					screen[pos] = 0x2593;
				}
				else {
					screen[pos] = 0x2588;
				}
			}
			screen[SCR_WIDTH * SCR_HEIGHT] = '\0';
			WriteConsoleOutputCharacter(hConsole, screen, SCR_WIDTH * SCR_HEIGHT, { 0,0 }, &dwBytesWritten);
		}




		camera.Inputs(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------

	//glDeleteTextures(1, &texture);
	//tex1.Delete();
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//	// make sure the viewport matches the new window dimensions; note that width and 
//	// height will be significantly larger than specified on retina displays.
//	glViewport(0, 0, width, height);
//}
