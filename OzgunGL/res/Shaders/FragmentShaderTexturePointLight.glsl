#version 330 core

//-------------------Distance - light strength------------------
//
//			Distance 	Constant 	Linear 	Quadratic
//			7 	1.0 	0.7 	1.8
//			13 	1.0 	0.35 	0.44
//			20 	1.0 	0.22 	0.20
//			32 	1.0 	0.14 	0.07
//			50 	1.0 	0.09 	0.032
//			65 	1.0 	0.07 	0.017
//			100 	1.0 	0.045 	0.0075
//			160 	1.0 	0.027 	0.0028
//			200 	1.0 	0.022 	0.0019
//			325 	1.0 	0.014 	0.0007
//			600 	1.0 	0.007 	0.0002
//			3250 	1.0 	0.0014 	0.000007

struct Material
{
	sampler2D diffuse;	//ambient and diffuse colors are almost the same for textures.
	sampler2D specular;
	sampler2D emission;
	float shineness;
};

struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//point light params
	vec3 lightPos;
	float Kconstant;
	float Klinear;
	float Kquad;
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

	//point light parameters;
	float distance = length(light.lightPos - FragmentPosition);
	float attenuation = 1.0f/ (light.Kconstant + distance * light.Klinear + distance * distance * light.Kquad);

	ambient		*=	attenuation;
	diffuse		*=	attenuation;
	specular	*=	attenuation;

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
