#version 440

layout(location = 0) in vec3 Pos;
//layout(location = 2) in vec2 inTex;

//out vec2 texCoord;

//Uniforms
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
	gl_Position = Projection * View * Model * vec4(Pos, 1.0f);
//	texCoord = inTex;
}

