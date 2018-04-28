#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

void main()
{
	gl_position = position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
	color = u_color;
}
