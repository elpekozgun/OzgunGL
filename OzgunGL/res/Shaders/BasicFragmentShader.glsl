#version 330 core

in vec3 outColor;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D texture1;	// =  whatever the hell is on Gl_TEXTURE0 by default even if we dont assign
uniform sampler2D texture2;
 

void main()
{
//	fragColor = texture( inTexture,texCoord) * vec4(outColor,1.0f);
	fragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);
}