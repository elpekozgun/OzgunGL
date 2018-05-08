#version 330 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shineness;
};

struct Light
{
	vec3 lightPos;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 FragmentNormal;
in vec3 FragmentPosition;

out vec4 FragColor;

uniform vec3 cameraPosition;
uniform Material material;
uniform Light light;

void main()
{
	// Lights work as an addition of light parameters which later multiplied by color of object.
	
	// Ambient
	vec3 ambient = light.ambient * material.ambient;
																	
	// Diffuse														
	vec3 norm = normalize(FragmentNormal);							
	vec3 lightDir = normalize(light.lightPos - FragmentPosition);	
	float diff = max(dot(norm, lightDir), 0.0f);			
	vec3 diffuse = light.diffuse * (diff * material.diffuse);						

	// Specular														
	float specularStrength = 0.5f;									
	vec3 viewDir = normalize(cameraPosition - FragmentPosition);	
	vec3 reflectDir = reflect(-lightDir,norm);						
	float spec = pow(max(dot(viewDir,reflectDir),0.0f), material.shineness);			
	vec3 specular = light.specular * (spec * material.specular);			

	vec3 result = (ambient + diffuse + specular);

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
