#version 330 core

struct Material
{
	sampler2D diffuse;	//ambient and diffuse colors are almost the same for textures.
	sampler2D specular;
	sampler2D emission;
	float shineness;
};

struct Light
{
	vec3 lightPos;	//when dealing with directional light, position is no longer needed
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};

in vec3 FragmentNormal;
in vec3 FragmentPosition;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 cameraPosition;
uniform Material material;
uniform Light light;

void main()
{
	// Lights work as an addition of light parameters which later multiplied by color of object.
	


	// Ambient
	vec3 ambient =  light.ambient * texture(material.diffuse,TexCoord).rgb;
								
	// Diffuse														
	vec3 norm = normalize(FragmentNormal);							
	vec3 lightDir = normalize(light.lightPos - FragmentPosition);	
	//vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0f);			
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoord));						


	// Specular														
	vec3 viewDir = normalize(cameraPosition - FragmentPosition);	
	vec3 reflectDir = reflect(-lightDir,norm);						
	float spec = pow(max(dot(viewDir,reflectDir),0.0f), material.shineness);			
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));			

	// Emmision
	vec3 emission = vec3(texture(material.emission, TexCoord));

	vec3 result = (ambient + diffuse + specular /*+ emission*/);

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
