#version 450
layout(location = 0) in vec3 Pos;
layout(location = 1) in vec2 inTexCoord;


out vec2 TexCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
	TexCoord = inTexCoord;
	gl_Position = Projection * View * Model * vec4(Pos, 1.0f);
}

