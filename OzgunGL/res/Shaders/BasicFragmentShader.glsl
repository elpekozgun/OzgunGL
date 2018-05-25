#version 330 core


out vec4 fragColor;

//uniform sampler2D texture1;	// =  whatever the hell is on Gl_TEXTURE0 by default even if we dont assign
//uniform sampler2D texture2;

uniform vec3 Color;

uniform vec3 diffuse;
uniform vec3 specular;
uniform vec3 ambient;


void main()
{
//	fragColor = texture( inTexture,texCoord); //* vec4(outColor,1.0f);
	fragColor = vec4(Color,1.0f);
}