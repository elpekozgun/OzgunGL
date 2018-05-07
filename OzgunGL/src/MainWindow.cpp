#define STB_IMAGE_IMPLEMENTATION

#include <string>
//#include <GL/glew.h>
//#include <GL/GLU.h>
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
void BindImage(const char* image,unsigned int* texture, GLenum format);
GLFWwindow* StartWindow();

const unsigned int SCR_HEIGHT = 600;
const unsigned int SCR_WIDTH = 800;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;


int main(void)
{
	GLFWwindow* window = StartWindow();

	float vertices[] = {
		// positions        
		0.5f, -0.5f, 0.0f,  
		-0.5f, -0.5f, 0.0f, 
		0.0f,  0.5f, 0.0f,  
	};

	float verticesWithMore[] = {
		// positions         // colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};

	float verticesWithTexture[] = {
	// positions          // colors           // texture coords
	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	float cubeVertices[] = {
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

	unsigned int indices[] = { 0, 1, 3,
							   1, 2, 3};

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

	Shader shader("res/Shaders/BasicVertexShader.glsl", "res/Shaders/BasicFragmentShader.glsl");

#pragma region Binding vertex array using a buffer and unbinding

	// Order: VAO -> VBO -> EBO -> Attribute(shader location) -> enable attribute

	//buffer generation
	//unsigned int VAO, VBO, EBO;
	//glGenVertexArrays(1, &VAO);		
	//glGenBuffers(1, &VBO);			
	//glGenBuffers(1, &EBO);


	//glBindVertexArray(VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);													
	//glBufferData(GL_ARRAY_BUFFER, sizeof(verticesWithTexture), &verticesWithTexture[0], GL_STATIC_DRAW);
	//																					
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);	// or &indices[0]
	//
	//glEnableVertexAttribArray(0);	//layout in vertex shader  layout(location = 0)		
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);		
	//																					
	//glEnableVertexAttribArray(1);	//layout in vertex shader  layout(location = 1)
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	//glBindBuffer(GL_ARRAY_BUFFER, 0);	
	//glBindVertexArray(0);

	//CUBE GENERATION WITHOUT INDEXING
	unsigned int VAO,VBOcube;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBOcube);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBOcube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));


#pragma endregion

#pragma region Texture stuff

	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };

	unsigned int texture1 = -1;
	unsigned int texture2 = -1;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &borderColor[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	//minify , use linear neighbor for texture filtering, and linearly interpolates between mipmaps to match pixel size.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);				//magnify

	const char* imageWood = "res/Textures/container.jpg";
	const char* imageFace = "res/Textures/awesomeface.png";

	BindImage(imageWood, &texture1, GL_RGB);
	BindImage(imageFace, &texture2, GL_RGBA);

	shader.Use();
	glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);	//can do both ways. moreover, even though you dont set the first, it assignes Gl_TEXTURE0 to it.
	shader.SetInt("texture2", 1);

#pragma endregion

	//glm::mat4 trans = glm::mat4(1.0);
	//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

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
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		//bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// render container
		shader.Use();

		float timeValue = (float)glfwGetTime();
		float value = sin(timeValue) / 2.0f + 0.5f;
		shader.SetFloat("param", value );
		glBindVertexArray(VAO);		

		glm::mat4 View = glm::mat4(1.0f);
		float radius = 20.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;

		View = camera.GetViewMatrix();
		unsigned int viewLoc = glGetUniformLocation(shader.ID, "View");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);

		glm::mat4 Projection = glm::perspective(glm::radians(camera.Zoom),float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
		unsigned int projectionLoc = glGetUniformLocation(shader.ID, "Projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &Projection[0][0]);

		for (int i = 0; i < 10; i++)
		{
			glm::mat4 Model = glm::mat4(1.0f);
			Model = glm::translate(Model, cubePositions[i]);
			float angle = 20.0f * (10-i) * glfwGetTime();
			Model = glm::rotate(Model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			unsigned int transformLoc = glGetUniformLocation(shader.ID, "Model");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &Model[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}
		
		//swap buffer and poll IO
		glfwSwapBuffers(window);		
		glfwPollEvents();				
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBOcube);
/*
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);*/

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

	if (glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, TRUE);
}

void BindImage(const char * imageName,unsigned int* texture, GLenum format)
{
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &borderColor[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR );	//minify , use linear neighbor for texture filtering, and linearly interpolates between mipmaps to match pixel size.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

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

