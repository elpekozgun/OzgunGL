#define STB_IMAGE_IMPLEMENTATION
#include <string>
#include<glad\glad.h>
#include <GLFW\glfw3.h>
#include "Macros.h"
#include "Shader.h"
#include "src/Vendor/stb_Image/stb_image.h"
#include "GLM/glm\glm.hpp"
#include "GLM/glm\gtc\matrix_transform.hpp"
#include "GLM/glm\gtc\type_ptr.hpp"
#include "Camera.h"	


void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallBack(GLFWwindow * window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void ProcessInput(GLFWwindow* window);
void BindImage(const char* image, unsigned int* texture, GLenum format);

GLFWwindow* StartWindow();

const unsigned int SCR_HEIGHT = 600;
const unsigned int SCR_WIDTH = 800;

Camera camera(glm::vec3(0.0f, 3.0f,8.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main(void)
{
	GLFWwindow* window = StartWindow();

	float cubeVertices[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	float verticesWithNormals[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	float verticesWithNormalsTexCoords[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	Shader lightShader("res/Shaders/ColorIntroVertexShader.glsl", "res/Shaders/FragmentShaderTextureMultiLight.glsl");
	Shader lampShader("res/Shaders/ColorIntroVertexShader.glsl","res/Shaders/constantLightShader.glsl" );

#pragma region Binding vertex array using a buffer and unbinding

	// Order: VAO -> VBO -> EBO -> Attribute(shader location) -> enable attribute

	unsigned int VBO,cubeVAO;

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesWithNormalsTexCoords), &verticesWithNormalsTexCoords[0], GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	//remember that vertex arraj object gets any bind buffer data after its assigned.
	unsigned int lightVAO;	
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	// even though we dont need to use layout 1 for lamp in VBO, which seems like a waste
	// its much more efficient to use this way than allocating new memory for Lamp only.
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	unsigned int diffuseTextureID = -1;
	unsigned int specularTextureID = -1;
	unsigned int emissionTextureID = -1;
	BindImage("res/Textures/crate.png", &diffuseTextureID, GL_RGBA);
	BindImage("res/Textures/container2_specular.png", &specularTextureID, GL_RGBA);
	//BindImage("res/Textures/matrix.jpg", &emissionTextureID, GL_RGB);


#pragma endregion

	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);

		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//render
		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Lets make a sun
		lightPos.x = (float)sin(glfwGetTime()) * 5.0f;
		lightPos.z = (float)cos(glfwGetTime()) * 5.0f;
		lightPos.y = (float)sin(glfwGetTime()) * 0.0f;

		lightShader.Use();
		lightShader.setVec3("cameraPosition", camera.Position);
		
		// set "light struct" properties 
		//lightShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		//lightShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		//lightShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		//lightShader.setVec3("light.lightPos", lightPos);						// Basic Light

		//lightShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);			// Directional Light

		//lightShader.setVec3("light.lightPos", lightPos);						// Point Light
		//lightShader.SetFloat("light.Kconstant", 1.0f);
		//lightShader.SetFloat("light.Klinear", 0.09f);
		//lightShader.SetFloat("light.Kquad", 0.0032f);

		//lightShader.setVec3("light.lightPos", camera.Position);					// Spot Light
		//lightShader.setVec3("light.direction", camera.Front);
		//lightShader.SetFloat("light.cutoff", glm::cos(glm::radians(12.0f)));
		//lightShader.SetFloat("light.outerCutoff", glm::cos(glm::radians(17.5f)));
		//lightShader.SetFloat("light.Kconstant", 1.0f);
		//lightShader.SetFloat("light.Klinear", 0.0045f);
		//lightShader.SetFloat("light.Kquad", 0.0075f);


		// MultiLights:
		lightShader.setVec3("directLight.direction", -0.2f, -1.0f,-0.3f);
		lightShader.setVec3("directLight.ambient", 0.05f, 0.05f, 0.05f);
		lightShader.setVec3("directLight.diffuse", 0.5f, 0.5f, 0.5f);
		lightShader.setVec3("directLight.specular", 1.0f, 1.0f, 1.0f);

		lightShader.setVec3("spotlight.position", camera.Position);
		lightShader.setVec3("spotlight.direction", camera.Front);
		lightShader.setVec3("spotlight.ambient", 0.2f, 0.2f, 0.2f);
		lightShader.setVec3("spotlight.diffuse", 0.5f, 0.5f, 0.5f);
		lightShader.setVec3("spotlight.specular", 1.0f, 1.0f, 1.0f);
		lightShader.SetFloat("spotlight.cutOff", cos(glm::radians(12.5f)));
		lightShader.SetFloat("spotlight.outerCutOff", cos(glm::radians(18.0f)));
		lightShader.SetFloat("spotlight.Kconstant", 1.0f);
		lightShader.SetFloat("spotlight.Klinear", 0.027);
		lightShader.SetFloat("spotlight.Kquad", 0.0028);
		
		lightShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		lightShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		lightShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		lightShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		lightShader.SetFloat("pointLights[0].Kconstant", 1.0f);
		lightShader.SetFloat("pointLights[0].Klinear", 0.09);
		lightShader.SetFloat("pointLights[0].Kquad", 0.032);
		
		lightShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		lightShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		lightShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		lightShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		lightShader.SetFloat("pointLights[1].Kconstant", 1.0f);
		lightShader.SetFloat("pointLights[1].Klinear", 0.09);
		lightShader.SetFloat("pointLights[1].Kquad", 0.032);
		
		lightShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		lightShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		lightShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		lightShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		lightShader.SetFloat("pointLights[2].Kconstant", 1.0f);
		lightShader.SetFloat("pointLights[2].Klinear", 0.09);
		lightShader.SetFloat("pointLights[2].Kquad", 0.032);
		
		lightShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		lightShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		lightShader.setVec3("pointLights[3].diffuse", 0.8f, 0.0f, 0.0f);
		lightShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 0.0f);
		lightShader.SetFloat("pointLights[3].Kconstant", 1.0f);
		lightShader.SetFloat("pointLights[3].Klinear", 0.09);
		lightShader.SetFloat("pointLights[3].Kquad", 0.032);

		// set "material struct" properties
		//lightShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);			// Pure Color Properties
		//lightShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		//lightShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		
		lightShader.SetInt("material.diffuse", 0);
		lightShader.SetInt("material.specular", 1);
		//lightShader.SetInt("material.emission", 2);
		lightShader.SetFloat("material.shineness", 32.0f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTextureID);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularTextureID);

		glm::mat4 Projection = glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
		glm::mat4 View = camera.GetViewMatrix();

		lightShader.setMat4("View", View);
		lightShader.setMat4("Projection", Projection);
		
		glBindVertexArray(cubeVAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 Model = glm::mat4(1.0f);
			Model =  glm::translate(Model, cubePositions[i]);
			float angle = 20.0f * i;
			Model = glm::rotate(Model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightShader.setMat4("Model", Model);
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, emissionTextureID);

		lampShader.Use();
		lampShader.setMat4("View", View);
		lampShader.setMat4("Projection", Projection);


		//pointLightPositions[1].x = (float)sin(glfwGetTime()) * 5.0f;
		//pointLightPositions[1].z = (float)cos(glfwGetTime()) * 5.0f;
		//pointLightPositions[1].y = (float)sin(glfwGetTime()) * 0.0f;
		//pointLightPositions[2].x = (float)sin(glfwGetTime()) * 5.0f;
		//pointLightPositions[2].z = (float)cos(glfwGetTime()) * 5.0f;
		//pointLightPositions[2].y = (float)sin(glfwGetTime()) * 0.0f;
		//pointLightPositions[3].x = (float)sin(glfwGetTime()) * 5.0f;
		//pointLightPositions[3].z = (float)cos(glfwGetTime()) * 5.0f;
		//pointLightPositions[3].y = (float)sin(glfwGetTime()) * 0.0f;

		glBindVertexArray(lightVAO);
		for (int i = 0; i < 4; i++)
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

		//swap buffer and poll IO
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &VBO);

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
		(float)lastX = xPos;
		(float)lastY = yPos;
		firstMouse = false;
	}

	float xoffset = xPos - lastX;
	float yoffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, image); //border parameter must always be 0 (legacy stuff)
		glGenerateMipmap(GL_TEXTURE_2D);	//this must be called last, because it uses the actual bound image on the Gl_TEXTURE_2D target
	}
	else
		LOG("Failed To load image: " << imageName);
	stbi_image_free(image);
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

