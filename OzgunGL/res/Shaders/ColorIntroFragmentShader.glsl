#version 330 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shineness;
};

in vec3 FragmentNormal;
in vec3 FragmentPosition;

out vec4 FragColor;


uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform Material material;

void main()
{
	// Lights work as an addition of light parameters which later multiplied by color of object.
	
	// Ambient
	float ambientStrength = 0.1f;	//ambient factor
	vec3 ambient = ambientStrength * lightColor;
																	
	// Diffuse														
	vec3 norm = normalize(FragmentNormal);							
	vec3 lightDir = normalize(lightPosition - FragmentPosition);	
	float diffuseValue = max(dot(norm, lightDir), 0.0f);			
	vec3 diffuse = diffuseValue * lightColor;						

	// Specular														
	float specularStrength = 0.5f;									
	vec3 viewDir = normalize(cameraPosition - FragmentPosition);	
	vec3 reflectDir = reflect(-lightDir,norm);						
	float spec = pow(max(dot(viewDir,reflectDir),0.0f),64);			
	vec3 specular = specularStrength * spec * lightColor;			

	vec3 result = (ambient + diffuse + specular) * objectColor;

	FragColor = vec4(result,1.0f);
}

// -------------- Diffuse -----------------
//
//					 n  lightsource
//					 | /
//				 ____|/____
//				|		   |
//				|__________|
//

// -------------- Specular -----------------
//
//		  reflection n  lightsources
//		view	   \ | /
//				 ___\|/____		dot(view,reflect)	
//				|		   |
//				|__________|
//
