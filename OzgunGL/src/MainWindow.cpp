#include <string>
//#include <GL/glew.h>
//#include <GL/GLU.h>
#include<glad\glad.h>
#include <GLFW/glfw3.h>
#include "Macros.h"

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
int Render(GLFWwindow* window);
void ShaderLogger(unsigned int Shader, const char* shaderType);
void ProgramLinkLogger(unsigned int Program);

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

const char* vertexShaderSource = 
"#version 330 core\n"
"layout(location = 0) in vec3 Pos;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(Pos.x, Pos.y, Pos.z, 1.0);\n"
"}\n";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{ FragColor = vec4(1.0f,0.5f,0.2f,1.0f); }";

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

	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

	float vertices[] =
		{-0.5f,-0.5f,0.0f,
		  0.5f,-0.5f,0.0f,
		  0.0f, 0.5f,0.0f};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),&vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1,  &vertexShaderSource , NULL);
	glCompileShader(vertexShader);
	ShaderLogger(vertexShader,"VERTEX" );

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	ShaderLogger(fragmentShader, "FRAGMENT");

	//Shader Program
	unsigned int ShaderProgram;
	ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, vertexShader);
	glAttachShader(ShaderProgram, fragmentShader);
	glLinkProgram(ShaderProgram);
	ProgramLinkLogger(ShaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glUseProgram(ShaderProgram);
	

	glBindVertexArray(VAO);


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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		ProcessInput(window);

		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window);		//swaps buffer for rendering
		glfwPollEvents();				//for responsiveness
	}

	glfwTerminate();
	return 0;
}

void ShaderLogger(unsigned int Shader,const char* shaderType)
{
	int success;
	char infoLog[512];
	glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(Shader, 512, NULL, infoLog);
		LOG("ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" << infoLog);
	}
	else
		LOG("Shader Compilation Successfull");
}

void ProgramLinkLogger(unsigned int Program)
{
	int success;
	char infoLog[512];
	glGetProgramiv(Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(Program, 512, NULL, infoLog);
		LOG(infoLog);
	}
	else
		LOG("Shader linking Successfull");
}
