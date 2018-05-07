#define STB_IMAGE_IMPLEMENTATION

#include <string>
#include<glad\glad.h>
#include <GLFW\glfw3.h>
#include "Macros.h"
#include "Shader.h"
#include "stb_image.h"
#include "GLM\glm\glm.hpp"
#include "GLM\glm\gtc\matrix_transform.hpp"
#include "GLM\glm\gtc\type_ptr.hpp"
#include "Camera.h"	

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallBack(GLFWwindow * window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void ProcessInput(GLFWwindow* window);
void BindImage(const char* image, unsigned int* texture, GLenum format);

GLFWwindow* StartWindow();

const unsigned int SCR_HEIGHT = 600;
const unsigned int SCR_WIDTH = 800;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
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

	Shader lightShader("res/Shaders/ColorIntroVertexShader.glsl", "res/Shaders/ColorIntroFragmentShader.glsl");
	Shader lampShader("res/Shaders/ColorIntroVertexShader.glsl","res/Shaders/constantLightShader.glsl" );

#pragma region Binding vertex array using a buffer and unbinding

	// Order: VAO -> VBO -> EBO -> Attribute(shader location) -> enable attribute

	unsigned int VBO,cubeVAO;

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices[0], GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//remember that vertex arraj object gets any bind buffer data after its assigned.
	unsigned int lightVAO;	
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		
#pragma endregion

	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//render
		//glClearColor(1.0f, 0.5f, 0.31f, 1.0f);
		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightShader.Use();
		lightShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		lightShader.setVec3("lightColor", sin(glfwGetTime()), cos(glfwGetTime()), 0.0f);

		glm::mat4 View = camera.GetViewMatrix();
		glm::mat4 Projection = glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
		glm::mat4 Model = glm::mat4(1.0f);

		lightShader.setMat4("View", View);
		lightShader.setMat4("Projection", Projection);
		lightShader.setMat4("Model", Model);
		
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lampShader.Use();

		lampShader.setMat4("View", View);
		lampShader.setMat4("Projection", Projection);
		glm::mat4 lampModel = glm::mat4(1.0f);
		lampModel = glm::translate(lampModel, lightPos);
		lampModel = glm::scale(lampModel, glm::vec3(0.2f));
		lampShader.setMat4("Model", lampModel);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

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
		lastX = xPos;
		lastY = yPos;
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
	float cameraSpeed = 5 * deltaTime;

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

