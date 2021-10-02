#pragma once
#include "Texture.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Material {
private:
	Shader shader;
	Texture albedo, metallic, roughness, normal, emission, ao;
	glm::vec3 albedoColor, emissionColor;
	float metallicValue, roughnessValue;
public:
	bool useTextureForAlbedo;
	bool useTextureForMetallic;
	bool useTextureForRoughness;
	bool useTextureForNormal;
	bool useTextureForEmission;
	bool useTextureForAO;

	Material(Shader& _shader) {
		shader = _shader;

		useTextureForAlbedo = false;
		useTextureForMetallic = false;
		useTextureForRoughness = false;
		useTextureForNormal = false;
		useTextureForEmission = false;
		useTextureForAO = false;
	}

	Material(){}

	/*
	void SetMatrices(glm::mat4 model, glm::mat4 proj, glm::mat4 view) {
		shader.
	}
	*/

	Shader GetShader() {
		return shader;
	}

	void SetShader(Shader& _shader) {
		shader = _shader;
	}

	void SetAlbedoTexture(std::string path) {
		albedo.Import(path);
	}

	void SetMetallicTexture(std::string path) {
		metallic.Import(path);
	}

	void SetRoughnessTexture(std::string path) {
		roughness.Import(path);
	}

	void SetNormalTexture(std::string path) {
		normal.Import(path);
	}

	void SetEmissionTexture(std::string path) {
		emission.Import(path);
	}

	void SetAOTexture(std::string path) {
		ao.Import(path);
	}

	void Activate() {
		shader.use();

		albedo.Activate(0);
		metallic.Activate(2);
		roughness.Activate(3);
		emission.Activate(4);
		ao.Activate(5);
		normal.Activate(6);
	}
};