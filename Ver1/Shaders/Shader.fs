#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;
in vec2 TexCoords;

struct PointLight
{
	vec3 position;
	vec3 color;
	float intensity	;
};

struct DirectionLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
  
uniform PointLight light;

uniform vec3 objectColor = vec3(1,1,1);
uniform vec3 viewPos;

vec3 subsurfaceColor = vec3(1,1,1);
float subsurfaceIntensity = 0.25;
float subsurfaceStaticColor = 0.2;

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;

void main()
{
    // ќкружающа€ составл€юща€
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light.color * vec3(texture(diffuse_texture, TexCoords));
  	
    // ƒиффузна€ составл€юща€ 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color * light.intensity * vec3(texture(diffuse_texture, TexCoords));
	
	//отражени€
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float specular_float = pow(max(dot(viewDir,reflectDir),0.0),32);
	vec3 specular = specularStrength * specular_float * light.color * vec3(texture(specular_texture, TexCoords));\
            
	//результат
    vec3 result = (ambient + diffuse + specular) * objectColor;
	
	//подповерхностное рассеивание
	float subsurfaceRange = length(light.position - FragPos);
	subsurfaceRange = 0.7 - abs(subsurfaceRange / 5) / subsurfaceIntensity;
	
	vec3 sss = mix(result, subsurfaceColor, subsurfaceRange);
	//result = sss + vec3(0.5,0.5,0.5);
	
    FragColor = vec4(result, 1.0);
} 