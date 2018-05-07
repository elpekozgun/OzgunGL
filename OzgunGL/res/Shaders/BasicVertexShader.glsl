#version 440

layout(location = 0) in vec3 Pos;
//layout(location = 1) in vec3 inColor;
layout(location = 1) in vec2 inTex;

out vec3 outColor;
out vec2 texCoord;

uniform float offset;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
	gl_Position = Projection * View * Model * vec4(Pos, 1.0f);
	//outColor = inColor;
	texCoord = inTex;
}
