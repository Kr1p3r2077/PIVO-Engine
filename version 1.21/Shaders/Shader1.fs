#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
 
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 lightPos;
uniform vec3 lightColor;
 
void main()
{
    // ќкружающа€ составл€юща€
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // ƒиффузна€ составл€юща€ 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = vec4(diff * lightColor,1.0);
	
	vec4 objectColor = texture(texture1, TexCoord);
	
	
    vec4 result = (ambient,1.0 + diffuse,1.0) * objectColor;
    FragColor = result;
}