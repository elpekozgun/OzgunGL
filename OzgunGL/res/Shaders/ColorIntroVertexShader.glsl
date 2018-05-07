#version 410

layout(location = 0) in vec3 Pos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
	gl_Position = Projection * View * Model * vec4(Pos,1.0f);
}