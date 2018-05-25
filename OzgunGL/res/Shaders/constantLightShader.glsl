#version 330 core

in vec3 fragColor;
out vec4 FragColor;

uniform vec3 overrideColor;
uniform int	isReflection;

void main()
{
	if(isReflection == 1)
	{
		FragColor = vec4(overrideColor *fragColor,1.0f);
	}
	else
	{
		FragColor = vec4(fragColor,1.0f);
	} 
}

