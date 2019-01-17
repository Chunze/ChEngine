#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_specular1;
	sampler2D texture_specular2;
	sampler2D texture_height1;
	float shininess;
}; 

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
  
uniform Material material;
uniform Light light;

void main()
{
	// ambient lighting
	float ambientStrength = 0.1;
	vec3 ambient = vec3(texture(material.texture_diffuse1, TexCoords) +
						texture(material.texture_diffuse2, TexCoords)) * light.ambient;

	// difuse lighting
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = vec3(texture(material.texture_diffuse1, TexCoords) +
						texture(material.texture_diffuse2, TexCoords)) * diff * light.diffuse;

	// specular lighting
	float specularStrength = 0.5;
	float specularPower = 32;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = vec3(texture(material.texture_specular1, TexCoords) + 
						 texture(material.texture_specular2, TexCoords)) * spec * light.specular;

	vec3 result = ambient + diffuse + specular;

	//vec3 result = diffuse;

	FragColor = vec4(result, 1.0);
}