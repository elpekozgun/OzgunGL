#define STB_IMAGE_IMPLEMENTATION
#include<glad\glad.h>
#include <GLFW\glfw3.h>

#include "GLM/glm\glm.hpp"
#include "GLM/glm\gtc\matrix_transform.hpp"
#include "GLM/glm\gtc\type_ptr.hpp"

#include "Macros.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"	

#include <iostream>


void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallBack(GLFWwindow * window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void ProcessInput(GLFWwindow* window);

GLFWwindow* StartWindow();

const unsigned int SCR_HEIGHT = 900;
const unsigned int SCR_WIDTH = 1200;

Camera camera(glm::vec3(0.0f, 3.0f, 8.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


float deltaTime = 0.0f;
float lastFrame = 0.0f;

float cube[] = {
	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f,0.0f,
	0.5f,  0.5f, -0.5f, 1.0f, 0.0f,0.0f,
	0.5f,  0.5f, -0.5f, 1.0f, 0.0f,0.0f,
	-0.5f,  0.5f, -0.5f, 1.0f, 0.0f,0.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,0.0f,

	-0.5f, -0.5f,  0.5f, 0.0f,1.0f,0.0f,
	0.5f, -0.5f,  0.5f, 0.0f,1.0f,0.0f,
	0.5f,  0.5f,  0.5f, 0.0f,1.0f,0.0f,
	0.5f,  0.5f,  0.5f, 0.0f,1.0f,0.0f,
	-0.5f,  0.5f,  0.5f, 0.0f,1.0f,0.0f,
	-0.5f, -0.5f,  0.5f, 0.0f,1.0f,0.0f,

	-0.5f,  0.5f,  0.5f, 0.0f,0.0f,1.0f,
	-0.5f,  0.5f, -0.5f, 0.0f,0.0f,1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f,0.0f,1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f,0.0f,1.0f,
	-0.5f, -0.5f,  0.5f, 0.0f,0.0f,1.0f,
	-0.5f,  0.5f,  0.5f, 0.0f,0.0f,1.0f,

	0.5f,  0.5f,  0.5f, 1.0f, 0.0f,0.0f,
	0.5f,  0.5f, -0.5f,1.0f, 0.0f,0.0f,
	0.5f, -0.5f, -0.5f,1.0f, 0.0f,0.0f,
	0.5f, -0.5f, -0.5f,1.0f, 0.0f,0.0f,
	0.5f, -0.5f,  0.5f, 1.0f, 0.0f,0.0f,
	0.5f,  0.5f,  0.5f, 1.0f, 0.0f,0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f,1.0f,0.0f,
	0.5f, -0.5f, -0.5f,0.0f,1.0f,0.0f,
	0.5f, -0.5f,  0.5f,0.0f,1.0f,0.0f,
	0.5f, -0.5f,  0.5f,0.0f,1.0f,0.0f,
	-0.5f, -0.5f,  0.5f, 0.0f,1.0f,0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f,1.0f,0.0f,

	-0.5f,  0.5f, -0.5f, 0.0f,0.0f,1.0f,
	0.5f,  0.5f, -0.5f, 0.0f,0.0f,1.0f,
	0.5f,  0.5f,  0.5f, 0.0f,0.0f,1.0f,
	0.5f,  0.5f,  0.5f, 0.0f,0.0f,1.0f,
	-0.5f,  0.5f,  0.5f, 0.0f,0.0f,1.0f,
	-0.5f,  0.5f, -0.5f, 0.0f,0.0f,1.0f,
};

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,  2.2f,  2.0f),
	glm::vec3(-2.0f, 4.0f, -3.0f),
	glm::vec3(2.0f,  6.0f, -3.0f),
};


int main(void)
{

	GLFWwindow* window = StartWindow();

	Shader nanoShader("res/Shaders/ModelLoadingVertexShaderWithLight.glsl", "res/Shaders/ModelLoadingFragmentShaderLighted.glsl");
	Shader lampShader("res/Shaders/LightVertexShader.glsl", "res/Shaders/constantLightShader.glsl");
	

	const char* path1 = "res/Models/nanosuit/nanosuit.obj";

	Model nanosuit(path1);
	

	//Bind vertex array for lamps
	unsigned int lightVAO, VBO;
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), &cube[0], GL_STATIC_DRAW);

	glBindVertexArray(lightVAO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);

		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		nanoShader.Use();

		nanoShader.setVec3("cameraPosition", camera.Position);
		nanoShader.setVec3("directLight.direction", -0.2f, -1.0f, -0.3f);
		nanoShader.setVec3("directLight.ambient", 0.05f, 0.05f, 0.05f);
		nanoShader.setVec3("directLight.diffuse", 0.5f, 0.5f, 0.5f);
		nanoShader.setVec3("directLight.specular", 1.0f, 1.0f, 1.0f);

		nanoShader.setVec3("spotlight.position", camera.Position);
		nanoShader.setVec3("spotlight.direction", camera.Front);
		nanoShader.setVec3("spotlight.ambient", 0.2f, 0.2f, 0.2f);
		nanoShader.setVec3("spotlight.diffuse", 0.5f, 0.5f, 0.5f);
		nanoShader.setVec3("spotlight.specular", 1.0f, 1.0f, 1.0f);
		nanoShader.SetFloat("spotlight.cutOff", cos(glm::radians(12.5f)));
		nanoShader.SetFloat("spotlight.outerCutOff", cos(glm::radians(18.0f)));
		nanoShader.SetFloat("spotlight.Kconstant", 1.0f);
		nanoShader.SetFloat("spotlight.Klinear", 0.027f);
		nanoShader.SetFloat("spotlight.Kquad", 0.0028f);

		nanoShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		nanoShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		nanoShader.setVec3("pointLights[0].diffuse", 0.8f, 0.0f, 0.0f);
		nanoShader.setVec3("pointLights[0].specular", 1.0f, 0.0f, 0.0f);
		nanoShader.SetFloat("pointLights[0].Kconstant", 1.0f);
		nanoShader.SetFloat("pointLights[0].Klinear", 0.09f);
		nanoShader.SetFloat("pointLights[0].Kquad", 0.032f);

		nanoShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		nanoShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		nanoShader.setVec3("pointLights[1].diffuse", 0.0f, 0.8f, 0.0f);
		nanoShader.setVec3("pointLights[1].specular", 0.0f, 1.0f, 0.0f);
		nanoShader.SetFloat("pointLights[1].Kconstant", 1.0f);
		nanoShader.SetFloat("pointLights[1].Klinear", 0.09f);
		nanoShader.SetFloat("pointLights[1].Kquad", 0.032f);

		nanoShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		nanoShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		nanoShader.setVec3("pointLights[2].diffuse", 0.0f, 0.0f, 0.8f);
		nanoShader.setVec3("pointLights[2].specular", 0.0f, 0.0f, 1.0f);
		nanoShader.SetFloat("pointLights[2].Kconstant", 1.0f);
		nanoShader.SetFloat("pointLights[2].Klinear", 0.09f);
		nanoShader.SetFloat("pointLights[2].Kquad", 0.032f);

		nanoShader.SetFloat("material.shineness", 32.0f);


		glm::mat4 Projection = glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
		glm::mat4 View = camera.GetViewMatrix();
		nanoShader.setMat4("Projection", Projection);
		nanoShader.setMat4("View", View);

		glm::mat4 Model = glm::mat4(1.0f);
		Model = glm::scale(Model, glm::vec3(0.2f, 0.2f,0.2f));
		nanoShader.setMat4("Model", Model);

		nanosuit.Draw(nanoShader);


		lampShader.Use();
		lampShader.setMat4("View", View);
		lampShader.setMat4("Projection", Projection);
		
		glBindVertexArray(lightVAO);
		for (int i = 0; i < pointLightPositions->length(); i++)
		{
			pointLightPositions[i].x += (float)sin(glfwGetTime()) * 0.02f;
			pointLightPositions[i].z += (float)cos(glfwGetTime()) * 0.02f;
			pointLightPositions[i].y += (float)sin(glfwGetTime()) * 0.02f;

			glm::mat4 lampModel = glm::mat4(1.0f);
			lampModel = glm::translate(lampModel, pointLightPositions[i]);
			lampModel = glm::scale(lampModel, glm::vec3(0.2f));
			
			lampShader.setMat4("Model", lampModel);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//swap buffer and poll IO,
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	CLOSEWIN();
}

#pragma region Callback Functions

void FrameBufferSizeCallback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void MouseCallBack(GLFWwindow * window, double xPos, double yPos)
{
	if (firstMouse)
	{
		(float)lastX = (float)xPos;
		(float)lastY = (float)yPos;
		firstMouse = false;
	}

	float xoffset = (float)xPos - lastX;
	float yoffset = lastY - (float)yPos;
	lastX = (float)xPos;
	lastY = (float)yPos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}

#pragma endregion

void ProcessInput(GLFWwindow * window)
{
	float cameraSpeed = 20 * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, TRUE);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
	glfwSetCursorPosCallback(window, MouseCallBack);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Need a wrapper like GLAD, GLEW etc to easily wrap OpenGL functions like glViewport();
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))	//platform specific
	{
		LOG("Failed to Initialize GLAD");
		return nullptr;
	}

	return window;
}

