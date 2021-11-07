#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
	mat3 TBN;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform bool reverse_normals;
uniform bool normals;

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoords = aTexCoords;
	
	mat3 modelVector = transpose(inverse(mat3(model)));
	
	vec3 T = normalize(modelVector * aTangent);
	vec3 B = normalize(modelVector * aBitangent);
	vec3 N = normalize(modelVector * aNormal);
	vs_out.TBN = mat3(T,B,N);
	
	if(normals){
    if(reverse_normals) // небольшой трюк, чтобы быть уверенными, что внешний куб-комната отображает освещение "изнутри", а не "снаружи"  
        vs_out.Normal = normalize(modelVector * -1.0 * aNormal);
    else
        vs_out.Normal = normalize(modelVector * aNormal);
		
		}
		else{
		vs_out.Normal = normalize(aNormal);
		}
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
