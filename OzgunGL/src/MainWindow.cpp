#define STB_IMAGE_IMPLEMENTATION

#include <string>
//#include <GL/glew.h>
//#include <GL/GLU.h>
#include<glad\glad.h>
#include <GLFW\glfw3.h>
#include "Macros.h"
#include "Shader.h"
#include "stb_image.h"

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void BindImage(const char* image,unsigned int* texture, GLenum format);
GLFWwindow* StartWindow();

const unsigned int SCR_HEIGHT = 600;
const unsigned int SCR_WIDTH = 800;

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

	unsigned int indices[] = { 0, 1, 3,
							   1, 2, 3};


	Shader shader("res/Shaders/BasicVertexShader.glsl", "res/Shaders/BasicFragmentShader.glsl");


#pragma region Binding vertex array using a buffer and unbinding

	// Order: VAO -> VBO -> EBO -> Attribute(shader location) -> enable attribute

	//buffer generation
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);		
	glGenBuffers(1, &VBO);			
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);													
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesWithTexture), &verticesWithTexture[0], GL_STATIC_DRAW);
																						
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);	// or &indices[0]
	
	glEnableVertexAttribArray(0);	//layout in vertex shader  layout(location = 0)		
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);		
																						
	glEnableVertexAttribArray(1);	//layout in vertex shader  layout(location = 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);	
	glBindVertexArray(0);


#pragma endregion

#pragma region Texture stuff

	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };

	unsigned int texture;
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

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		//input
		ProcessInput(window);

		//render
		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// render container
		shader.Use();
		glBindVertexArray(VAO);		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//swap buffer and poll IO
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

void BindImage(const char * imageName,unsigned int* texture, GLenum format)
{
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &borderColor[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	//minify , use linear neighbor for texture filtering, and linearly interpolates between mipmaps to match pixel size.
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

	//Need a wrapper like GLAD, GLEW etc to easily wrap OpenGL functions like glViewport();
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))	//platform specific
	{
		LOG("Failed to Initialize GLAD");
		return nullptr;
	}

	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	return window;
}

