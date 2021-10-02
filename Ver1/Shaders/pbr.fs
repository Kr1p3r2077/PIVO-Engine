#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    mat3 TBN;
} fs_in;

uniform sampler2D albedoMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D emissionMap;
uniform sampler2D normalMap;
uniform sampler2D aoMap;

uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
const float lightIntensity = 2;

uniform float far_plane;
uniform bool shadows;

const float PI = 3.14159265359;

float distributionGGX(float NdotH, float roughness){
	float a = roughness * roughness;
	float a2 = a*a;
	float denom = NdotH * NdotH * (a2 - 1.0)+1.0;
	denom = PI * denom * denom;
	return a2/max(denom,0.0000001);
}

float geometrySmith(float NdotV, float NdotL, float roughness){
	float r = roughness + 1.0;
	float k = (r * r) / 9.0;
	float ggx1 = NdotV / (NdotV * (1.0 - k) + k);
	float ggx2 = NdotL / (NdotL * (1.0 - k) + k);
	return ggx1 + ggx2;
}

vec3 fresnelSchlick(float HdotV, vec3 baseReflectivity){
	return baseReflectivity + (1.0 + baseReflectivity) * pow(1.0 - HdotV, 5.0);
}

// Массив направлений смещения для сэмплирования
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

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

vec3 Shlick(float h, float v, vec3 F0){
	return F0 + (1 - F0) * (pow((1-(h * v)),5));
}

float CalculateAttenuation(vec3 fragpos, vec3 lightpos){
	float distance = length(lightPos - fragpos);
	float attenuation = 1.0 / (distance * distance);
	return attenuation;
}

void main()
{          
	vec4 bright = vec4(0);
	
	vec3 albedo = texture(albedoMap, fs_in.TexCoords).rgb;
	vec3 emission = texture(emissionMap,fs_in.TexCoords).rgb;
	float metallic = texture(metallicMap, fs_in.TexCoords).r;
	float roughness = texture(roughnessMap, fs_in.TexCoords).r;
	float ao = texture(aoMap,fs_in.TexCoords).r;
	vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
	
	//vec3 normal = fs_in.Normal;
	//normal = normal * 2.0 - 1.0;
	//normal = normalize(fs_in.TBN * normal);
	normal = normalize(normal);
	
    vec3 lightColor = vec3(1);
	vec3 wi = normalize(lightPos - fs_in.FragPos);
	float cosTheta = max(dot(normal, wi),0.0);
	float attenuation = CalculateAttenuation(fs_in.FragPos, lightPos);
	vec3 radiance = lightColor * attenuation * cosTheta;
	
	// Диффузная составляющая
    //vec3 lightDir = fs_in.TBN * normalize(lightPos - fs_in.FragPos);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
	
	vec3 V = normalize(viewPos - fs_in.FragPos);
	vec3 H = normalize(V + lightDir);
	
	vec3 baseReflectivity = mix(vec3(0.04),albedo,metallic);
	
	vec3 Lo = vec3(0.0);
	
	float NdotV = max(dot(normal,V),0.0000001);
	float NdotL = max(dot(normal,lightDir),0.0000001);
	float HdotV = max(dot(H,V),0.0);
	float NdotH = max(dot(normal,H),0.0);
	
	float D = distributionGGX(NdotH,roughness);
	float G = geometrySmith(NdotV, NdotL, roughness);
	vec3 F = fresnelSchlick(HdotV,baseReflectivity);
	
	vec3 specular = D * G * F;
	specular /= 4.0 * NdotV * NdotL;
	
	vec3 kD = vec3(1.0)-F;
	kD *= 10 - metallic;
	Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	
	vec3 ambient = vec3(0.09) * albedo * ao;
	
    vec3 color = ambient + Lo;
	
    // Отраженная составляющая
    //vec3 viewDir = fs_in.TBN * normalize(viewPos - fs_in.FragPos);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos) * 2;
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    specular = spec * lightColor;    
	
    // Вычисляем тень
    float shadow = ShadowCalculation(fs_in.FragPos);                   
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;  
	
	//lighting *= lightIntensity;
    lighting = lighting / (lighting + vec3(0.8));
	lighting = pow(lighting, vec3(1.0/1.5));
	
	lighting += emission;
	
	//if(lighting.r > 0.8 && lighting.g > 0.8 && lighting.b > 0.8) discard;
    FragColor = vec4(lighting,1.0);//vec4(lighting, 1.0);
    //FragColor = texture(normalMap,fs_in.TexCoords);
}
