#include <string>
//#include <GL/glew.h>
//#include <GL/GLU.h>
#include<glad.h>
#include <GLFW/glfw3.h>
#include "Macros.h"

using namespace std;

static GLFWwindow* window;

static int StartGL()
{
	if (!glfwInit())
		CLOSEWIN();
	   
	window = glfwCreateWindow(640, 640, "", NULL, NULL);
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSetWindowTitle(window, "Celestial Engine V0.0 - Client");



	if (!window)
		CLOSEWIN();

	glfwMakeContextCurrent(window);

	//if (glewInit() != GLEW_OK)
	//	LOG("Glew Error");
	//LOG(glGetString(GL_VERSION));

	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

	return 0;
}

static int UpdateGL()
{
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		HELLOTRIANGLE();
		glfwSwapBuffers(window);
	}
	return 0;
}

int main(void)
{
	StartGL();
	UpdateGL();
	CLOSEWIN();
}