#version 330 core

layout(location = 0) in vec3 Pos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTex;

out vec3 outColor;
out vec2 texCoord;

uniform float offset;

void main()
{
	gl_Position = vec4(Pos.x + offset , Pos.y, Pos.z, 1.0);
	outColor = inColor;
	texCoord = inTex;
}
