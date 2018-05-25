#version 330 core

in vec2 TexCoord;
out vec4 fragColor;
uniform sampler2D texture1;

void main()
{
	vec4 texColor = texture(texture1,TexCoord);
//	if(texColor.a < 0.1f)	//if colors alpha value is smaller than 0.1, discard that pixel
//		discard;

	fragColor = texColor;
}

