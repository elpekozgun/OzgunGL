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

#include "Vendor\ImGui\imgui.h"
#include "Vendor\ImGui\imgui_impl_glfw_gl3.h"

#include <iostream>


void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallBack(GLFWwindow * window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void ProcessInput(GLFWwindow* window);
void BindImage(const char * imageName, unsigned int* texture, GLenum format);
glm::vec3 GlmVec3(float x, float y, float z);


GLFWwindow* StartWindow();

const unsigned int SCR_HEIGHT = 900;
const unsigned int SCR_WIDTH = 1200;

Camera camera(glm::vec3(0.0f, 3.0f, 8.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

bool show_demo_window = true;
bool show_another_window = false;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float cubeTexture[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};


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

float plane[] = {
	-0.5f, -0.5f,0.0f,0.0f,0.0f,-1.0f,   // Bottom-left
	0.5f, -0.5f, 0.0f,0.0f,0.0f,-1.0f, // Bottom-right
	0.5f,  0.5f, 0.0f,0.0f,0.0f,-1.0f, // Top-right
	-0.5f,  0.5f,0.0f,0.0f,0.0f,-1.0f // Top-left
};

unsigned int indices[] = { 0,1,2,
						   2,3,0 };

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,  2.2f,  2.0f),
	glm::vec3(-2.0f, 4.0f, -3.0f),
	glm::vec3(2.0f,  6.0f, -3.0f),
};

float grassVertices[] = {
	// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
	0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.0f, -0.5f,  0.0f,  0.0f,  0.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  0.0f,

	0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  0.0f,
	1.0f,  0.5f,  0.0f,  1.0f,  1.0f
};


int main(void)
{

	GLFWwindow* window = StartWindow();

	//Shader nanoShader("res/Shaders/ModelLoadingVertexShaderWithLight.glsl", "res/Shaders/FragmentShaderDepthTest.glsl");
	Shader nanoShader("res/Shaders/ModelLoadingVertexShaderWithLight.glsl", "res/Shaders/ModelLoadingFragmentShaderLighted.glsl");
	Shader lampShader("res/Shaders/LightVertexShader.glsl", "res/Shaders/constantLightShader.glsl");
	Shader planeShader("res/Shaders/ColorIntroVertexShader.glsl", "res/Shaders/FragmentShaderMaterial.glsl");
	Shader vegetationShader("res/Shaders/VertexShaderTextureAlpha.glsl", "res/Shaders/FragmentShaderTextureAlpha.glsl");

	const char* path1 = "res/Models/nanosuit/nanosuit.obj";
	//	const char* path1 = "res/Models/sponza/sponza.obj";

	Model nanosuit(path1);


	std::vector<glm::vec3> vegetation;
	vegetation.push_back(GlmVec3(-1.5f, 0.0f, -0.48f));
	vegetation.push_back(GlmVec3(1.5f, 0.0f, 0.51f));
	vegetation.push_back(GlmVec3(0.0f, 0.0f, 0.7f));
	vegetation.push_back(GlmVec3(-0.3f, 0.0f, -2.3f));
	vegetation.push_back(GlmVec3(0.5f, 0.0f, -0.6f));
	vegetation.push_back(GlmVec3(2.0f, 0.0f, -1.6f));
	vegetation.push_back(GlmVec3(-1.6f, 0.0f, -1.6f));
	vegetation.push_back(GlmVec3(2.1f, 0.0f, 1.7f));
	vegetation.push_back(GlmVec3(-2.8f, 0.0f, 1.9f));
	vegetation.push_back(GlmVec3(2.2f, 0.0f, 1.6f));
	vegetation.push_back(GlmVec3(2.1f, 0.0f, 2.0f));
	vegetation.push_back(GlmVec3(1.5f, 0.0f, -2.1f));
	vegetation.push_back(GlmVec3(2.2f, 0.0f, -1.8f));
	vegetation.push_back(GlmVec3(-1.5f, 0.0f, -0.1f));
	vegetation.push_back(GlmVec3(-2.3f, 0.0f, -0.4f));


	//Bind vertex array for lamps
	unsigned int lightVAO, VBO, planeVAO;
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), &cube[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));


	//plane
	unsigned int EBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane), &plane[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// comparison Cube
	unsigned int windowTextureID = -1;
	BindImage("res/Textures/window.png", &windowTextureID, GL_RGBA);
	unsigned int cubeVAO, cubeTexVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeTexVBO);

	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeTexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTexture), &cubeTexture[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	

	//grass
	unsigned int grassTextureID = -1;
	BindImage("res/Textures/grass.png", &grassTextureID, GL_RGBA);
	unsigned int grassVAO, grassVBO;

	glGenVertexArrays(1, &grassVAO);
	glGenBuffers(1, &grassVBO);

	glBindVertexArray(grassVAO);
	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices), grassVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);	//face to cull: default back
	//glFrontFace(GL_CW);		//which face is front? default: ccw

	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	std::map<float, glm::vec3> sortedTextures;
	for (unsigned int i = 0; i < vegetation.size(); i++)
	{
		float distance = glm::length(camera.Position - vegetation[i]);
		sortedTextures[distance] = vegetation[i];
	}


	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);

		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		std::string fps = "fps: " + std::to_string(1.0f / deltaTime);
		ImGui_ImplGlfwGL3_NewFrame();
		ImGui::Text(fps.c_str());

		//render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


		// nanosuit
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
		Model = glm::scale(Model, glm::vec3(0.3f, 0.3f, 0.3f));
		//Model = glm::scale(Model, glm::vec3(0.01f, 0.01f,0.01f));
		nanoShader.setMat4("Model", Model);

		nanosuit.Draw(nanoShader);


		//lights
		lampShader.Use();
		lampShader.setMat4("View", View);
		lampShader.setMat4("Projection", Projection);
		lampShader.SetInt("isReflection", 0);
		glBindVertexArray(lightVAO);
		for (int i = 0; i < pointLightPositions->length(); i++)
		{
			pointLightPositions[i].x += (float)sin(glfwGetTime()) * 0.02f;
			pointLightPositions[i].z += (float)cos(glfwGetTime()) * 0.02f;
			//pointLightPositions[i].y += (float)sin(glfwGetTime()) * 0.02f;

			glm::mat4 lampModel = glm::mat4(1.0f);
			lampModel = glm::translate(lampModel, pointLightPositions[i]);
			lampModel = glm::scale(lampModel, glm::vec3(0.2f));

			lampShader.setMat4("Model", lampModel);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}




		//Plane
		planeShader.Use();
		planeShader.setMat4("View", View);
		planeShader.setMat4("Projection", Projection);

		planeShader.setVec3("cameraPosition", camera.Position);
		planeShader.setVec3("light.lightPos", -0.2f, 1.0f, -0.3f);
		planeShader.setVec3("light.ambient", 0.05f, 0.05f, 0.05f);
		planeShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		planeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		planeShader.setVec3("material.ambient", 0.05375f, 0.05f, 0.06625f);
		planeShader.setVec3("material.diffuse", 0.18275f, 0.17f, 0.22525f);
		planeShader.setVec3("material.specular", 0.332741f, 0.328634f, 0.346435f);
		planeShader.SetFloat("material.shineness", 76.8f);

		glBindVertexArray(planeVAO);
		glm::mat4 planeModel = glm::mat4(1.0f);
		planeModel = glm::rotate(planeModel, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		planeModel = glm::scale(planeModel, glm::vec3(6.0f, -6.0f, 6.0f));
		planeShader.setMat4("Model", planeModel);




		glEnable(GL_STENCIL_TEST);

		//stencil fail, depth fail, both pass
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);  // set value of ref as 0xFF. 
		glDepthMask(GL_FALSE);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		// Reflection
		glStencilMask(0xFF);
		glStencilFunc(GL_LEQUAL, 1, 0xFF);
		glDepthMask(GL_TRUE);


		//Model = glm::rotate(Model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		nanoShader.Use();
		Model = glm::scale(Model, glm::vec3(1.0f, -1.0f, 1.0f));
		nanoShader.setMat4("Model", Model);

		nanoShader.setVec3("directLight.direction", -0.2f, -1.0f, -0.3f);
		nanoShader.setVec3("directLight.ambient", 0.00f, 0.00f, 0.00f);
		nanoShader.setVec3("directLight.diffuse", 0.1f, 0.1f, 0.2f);
		nanoShader.setVec3("directLight.specular", 0.5f, 0.5f, 0.5f);
		nanoShader.setVec3("spotlight.ambient", 0.0f, 0.0f, 0.0f);
		nanoShader.setVec3("spotlight.diffuse", 0.0f, 0.0f, 0.0f);
		nanoShader.setVec3("spotlight.specular", 0.0f, 0.0f, 0.0f);
		nanoShader.setVec3("pointLights[0].ambient", 0.02f, 0.02f, 0.02f);
		nanoShader.setVec3("pointLights[0].diffuse", 0.8f, 0.0f, 0.0f);
		nanoShader.setVec3("pointLights[0].specular", 1.0f, 0.0f, 0.0f);
		nanoShader.setVec3("pointLights[1].ambient", 0.02f, 0.02f, 0.02f);
		nanoShader.setVec3("pointLights[1].diffuse", 0.0f, 0.8f, 0.0f);
		nanoShader.setVec3("pointLights[1].specular", 1.0f, 0.0f, 0.0f);
		nanoShader.setVec3("pointLights[2].ambient", 0.02f, 0.02f, 0.02f);
		nanoShader.setVec3("pointLights[2].diffuse", 0.0f, 0.0f, 0.8f);
		nanoShader.setVec3("pointLights[2].specular", 1.0f, 0.0f, 0.0f);

		//draw reflections
		nanosuit.Draw(nanoShader);

		lampShader.Use();
		lampShader.SetInt("isReflection", 1);
		lampShader.setVec3("overrideColor", GlmVec3(0.1f, 0.1f, 0.1f));
		glBindVertexArray(lightVAO);
		for (int i = 0; i < pointLightPositions->length(); i++)
		{
			pointLightPositions[i].x += (float)sin(glfwGetTime()) * 0.02f;
			pointLightPositions[i].z += (float)cos(glfwGetTime()) * 0.02f;


			glm::mat4 lampModel = glm::mat4(1.0f);
			lampModel = glm::translate(lampModel, GlmVec3(pointLightPositions[i].x, -1 * pointLightPositions[i].y, pointLightPositions[i].z));//pointLightPositions[i]
			lampModel = glm::scale(lampModel, glm::vec3(0.2f));
			lampShader.setMat4("Model", lampModel);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glDisable(GL_STENCIL_TEST);

		//sort and render blending textures last since z-buffer doesnt care about it.
		// Draw opaques -> sort transparent objects -> draw transparent objects

		//enable color blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Grasses
		vegetationShader.Use();
		vegetationShader.setMat4("View", View);
		vegetationShader.setMat4("Projection", Projection);
		vegetationShader.SetInt("texture1", 0);
		glBindTexture(GL_TEXTURE_2D, grassTextureID);
		glBindVertexArray(grassVAO);
		for (std::map<float, glm::vec3>::reverse_iterator it = sortedTextures.rbegin(); it != sortedTextures.rend(); ++it)
		{
			glm::mat4 vegatationModel = glm::mat4(1.0f);
			vegatationModel = glm::translate(vegatationModel, it->second); // it -> first = key, it -> second value
			vegatationModel = glm::translate(vegatationModel, GlmVec3(0.0f, 0.5f, 0.0f));
			vegetationShader.setMat4("Model", vegatationModel);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		glBindTexture(GL_TEXTURE_2D, windowTextureID);
		glBindVertexArray(cubeVAO);

		glm::mat4 cubeModel = glm::mat4(1.0f);
		cubeModel = glm::translate(cubeModel, GlmVec3(0.0f,0.51f,2.0f));
		vegetationShader.setMat4("Model", cubeModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		//swap buffer and poll IO,
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
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
	glfwSwapInterval(1);	//enables v-sync
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

	//imgui stuff
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	return window;
}

// Visual studio crashes when typing glm::vec3(x,y, 0. ---> crash);
glm::vec3 GlmVec3(float x, float y, float z)
{
	return glm::vec3(x, y, z);
}

void BindImage(const char * imageName, unsigned int* texture, GLenum format)
{
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &borderColor[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);	//minify , use linear neighbor for texture filtering, and linearly interpolates between mipmaps to match pixel size.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, NrOfColorChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image = stbi_load(imageName, &width, &height, &NrOfColorChannels, 0);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image); //border parameter must always be 0 (legacy stuff)
		glGenerateMipmap(GL_TEXTURE_2D);	//this must be called last, because it uses the actual bound image on the Gl_TEXTURE_2D target
	}
	else
		LOG("Failed To load image: " << imageName);
	stbi_image_free(image);
}
