#include <string>
//#include <GL/glew.h>
//#include <GL/GLU.h>
#include<glad\glad.h>
#include <GLFW\glfw3.h>
#include "Macros.h"
#include "Shader.h"

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
GLFWwindow* StartWindow();

const unsigned int SCR_HEIGHT = 600;
const unsigned int SCR_WIDTH = 800;

int main(void)
{
	GLFWwindow* window = StartWindow();


	float verticesWithMore[] = {
		// positions         // colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};

	unsigned int indices[] = { 0, 1, 3,
							   1, 2, 3};

	unsigned int VAO,VBO,EBO;

#pragma region Binding vertex array using a buffer and unbinding

	// Order: VAO -> VBO -> EBO -> Attribute(shader location) -> enable attribute

	//buffer generation
	glGenVertexArrays(1, &VAO);		
	glGenBuffers(1, &VBO);			
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);													
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesWithMore), &verticesWithMore[0], GL_STATIC_DRAW);		
																						
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);	// or &indices[0]
	
	glEnableVertexAttribArray(0);	//layout in vertex shader  layout(location = 0)		
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);		
																						
	glEnableVertexAttribArray(1);	//layout in vertex shader  layout(location = 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
																						
	glBindBuffer(GL_ARRAY_BUFFER, 0);	
	glBindVertexArray(0);


#pragma endregion

	Shader shader("res/Shaders/BasicVertexShader.glsl", "res/Shaders/BasicFragmentShader.glsl");
	shader.Use();

	shader.SetFloat("offset", 0.5f);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);

		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);		

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);		
		glfwPollEvents();				
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	CLOSEWIN();
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

GLFWwindow* StartWindow()
{
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
		return nullptr;
	}
	glfwMakeContextCurrent(window);

	//Need a wrapper like GLAD, GLEW etc to easily wrap OpenGL functions like glViewport();
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))	//platform specific
	{
		LOG("Failed to Initialize GLAD");
		return nullptr;
	}

	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	return window;
}

