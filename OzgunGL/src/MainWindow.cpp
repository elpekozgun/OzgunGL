#include <string>
//#include <GL/glew.h>
//#include <GL/GLU.h>
#include<glad\glad.h>
#include <GLFW/glfw3.h>
#include "Macros.h"

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
int Render(GLFWwindow* window);

static int StartGL(GLFWwindow* window)
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

	//if (GL_COMPILE_STATUS)
		//LOG("Glew Error");
	//LOG(glGetString(GL_VERSION));

	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

	return 0;
}
static int UpdateGL(GLFWwindow* window)
{
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		HELLOTRIANGLE();
		glfwSwapBuffers(window);
	}
	return 0;
}

const unsigned int SCR_HEIGHT = 600;
const unsigned int SCR_WIDTH = 800;


int main(void)
{
	//StartGL();
	//UpdateGL();
	//CLOSEWIN();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);						//needed for MacOS
	#endif

	
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ozgun GL", NULL, NULL);
	if (!window)
	{
		LOG("Failed to Create GLFW Window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Need a wrapper like GLAD, GLEW etc to easily wrap OpenGL functions like glViewport();
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))	//platform specific
	{
		LOG("Failed to Initialize GLAD");
		return -1;
	}
	
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

	Render(window);
}

void FrameBufferSizeCallback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow * window)
{
	if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, TRUE);
	}
}

int Render(GLFWwindow* window)
{
	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);

		glfwSwapBuffers(window);		//swaps buffer for rendering
		glfwPollEvents();				//for responsiveness
	}
	glfwTerminate();
	return 0;
}

