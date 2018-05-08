#version 410

layout(location = 0) in vec3 Pos;
layout(location = 1) in vec3 inputNormal;
layout(location = 2) in vec2 inputTexCoord;

out vec3 FragmentNormal;
out vec3 FragmentPosition;
out vec2 TexCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;



void main()
{
	gl_Position = Projection * View * Model * vec4(Pos,1.0f);
	
	FragmentPosition = vec3(Model * vec4(Pos, 1.0f));

	//this is done to calculate normals properly.(to avoid problems in non-uniform scaling)
	// matrix inversion is extremely costly, better to calculate them beforehand and send to shader
	FragmentNormal = mat3(transpose(inverse(Model))) * inputNormal;		
	TexCoord = inputTexCoord;
}