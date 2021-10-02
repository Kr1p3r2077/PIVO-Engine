#version 330 core
out vec4 FragColor;

struct TextureAtlas {
    sampler2D diffuse_texture;
    sampler2D specular_texture;
	sampler2D emissive_texture;
    float shininess;
}; 

struct Light {
    vec3 position;  
    vec3 direction;
    float cutOff;
    float outerCutOff;
	float intensity;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform TextureAtlas material;
uniform Light light[2];

vec3 ambient = vec3(0.1,0.1,0.1);

vec3 emissive_color = vec3(1,1,1);

vec3 CalculateLight(int ind);
vec3 ShadowCalculation(int ind);

void main()
{
    // ќкружающа€ составл€юща€
    vec3 ambient = ambient * texture(material.diffuse_texture, TexCoords).rgb;
   
	vec3 result = ambient;
	for(int i =0;i<1;i++){
		result += CalculateLight(i);
	}
	
	//vec3 emissive = texture(material.emissive_texture, TexCoords).xyz * emissive_color;
	
	//result += emissive.xyz;
	
	//result = pow(result, vec3(1.0/2.2));
    FragColor = vec4(result,1.0);
}

/*
float ShadowCalculation(vec3 fragPos)
{
    // Получаем вектор между положением фрагмента и положением источника света
    vec3 fragToLight = fragPos - lightPos;
	
    // Используем полученный вектор для выборки из карты глубины    
    // float closestDepth = texture(depthMap, fragToLight).r;
	
    // В данный момент значения лежат в диапазоне [0,1]. Преобразуем их обратно к исходным значениям
    // closestDepth *= far_plane;
	
    // Теперь получим текущую линейную глубину как длину между фрагментом и положением источника света
    float currentDepth = length(fragToLight);
	
    // Теперь проводим проверку на нахождение в тени
    // float bias = 0.05; // мы используем гораздо большее теневое смещение, так как значение глубины теперь находится в диапазоне [near_plane, far_plane]
    // float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
	
    // PCF
    // float shadow = 0.0;
    // float bias = 0.05; 
    // float samples = 4.0;
    // float offset = 0.1;
    // for(float x = -offset; x < offset; x += offset / (samples * 0.5))
    // {
        // for(float y = -offset; y < offset; y += offset / (samples * 0.5))
        // {
            // for(float z = -offset; z < offset; z += offset / (samples * 0.5))
            // {
                // float closestDepth = texture(depthMap, fragToLight + vec3(x, y, z)).r; // используем lightdir для осмотра кубической карты
                // closestDepth *= far_plane;   // Undo mapping [0;1]
                // if(currentDepth - bias > closestDepth)
                    // shadow += 1.0;
            // }
        // }
    // }
    // shadow /= (samples * samples * samples);
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane; 
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
        
    // Отладка - отображение значений переменной  closestDepth (для визуализации кубической карты глубины)
    // FragColor = vec4(vec3(closestDepth / far_plane), 1.0);    
        
    return shadow;
}
*/

vec3 CalculateLight(int ind){
	vec3 res = vec3(0,0,0);
	
	    // ƒиффузна€ составл€юща€ 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light[ind].position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light[ind].diffuse * light[ind].intensity * diff * texture(material.diffuse_texture, TexCoords).rgb;  
    
    // ќтраженна€ составл€юща€
    vec3 viewDir = normalize(viewPos - FragPos);
	
	//Фонг
    //vec3 reflectDir = reflect(-lightDir, norm);  
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//
	
	// Блин Фонг
	vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 16.0);
	//
	
    vec3 specular = light[ind].specular * spec * texture(material.specular_texture, TexCoords).rgb;  
    
    // ѕрожектор
    //float theta = dot(lightDir, normalize(-light[ind].direction)); 
    //float epsilon = (light.cutOff - light[ind].outerCutOff);
    //float intensity = clamp((theta - light[ind].outerCutOff) / epsilon, 0.0, 1.0);
    //diffuse  *= intensity;
    //specular *= intensity;
    
    // Затухание
	float max_distance=1.5;
    float distance = length(light[ind].position - FragPos);
    //float attenuation = 1.0 / (light[ind].constant + light[ind].linear * distance + light[ind].quadratic * (distance * distance));
    float attenuation = 1.0 / (distance * distance);
    ambient *= attenuation; 
    diffuse *= attenuation;
    specular *= attenuation;   
	
	res += specular + ambient + diffuse;
	return res;
}