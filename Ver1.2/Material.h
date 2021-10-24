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

	bool useTextureForAlbedo;
	bool useTextureForMetallic;
	bool useTextureForRoughness;
	bool useTextureForNormal;
	bool useTextureForEmission;
	bool useTextureForAO;

public:

	void SetAlbedoColor(glm::vec3 clr = glm::vec3(1), std::string path = "Textures/1px.png") {
		albedoColor = clr;
		albedo.Import(path);
	}

	void SetJustAlbedoColor(glm::vec3 clr = glm::vec3(1)) {
		albedoColor = clr;
	}

	void SetEmissionColor(glm::vec3 clr = glm::vec3(1), std::string path = "Textures/1px.png") {
		emissionColor = clr;
		emission.Import(path);
	}

	void SetMetallicValue(float val = 1, std::string path = "Textures/1px.png") {
		metallicValue = val;
		metallic.Import(path);
	}

	void SetRoughnessValue(float val = 1, std::string path = "Textures/1px.png") {
		roughnessValue = val;
		roughness.Import(path);
	}

	void SetAOValue(std::string path = "Textures/1px.png") {
		ao.Import(path);
	}

	Material(Shader& _shader) {
		shader = _shader;

		useTextureForAlbedo = false;
		useTextureForMetallic = false;
		useTextureForRoughness = false;
		useTextureForNormal = false;
		useTextureForEmission = false;
		useTextureForAO = false;

		albedoColor = glm::vec3(1);
		emissionColor = glm::vec3(1);
		metallicValue = 1;
		roughnessValue = 1;
	}

	Material(){}

	Material& operator= (const Material& a) {
		shader = a.shader;
		albedo = a.albedo;
		metallic = a.metallic;
		roughness = a.roughness;
		normal = a.normal;
		emission = a.emission;
		ao = a.ao;

		albedoColor = a.albedoColor;
		emissionColor = a.emissionColor;

		metallicValue = a.metallicValue;
		roughnessValue = a.roughnessValue;

		useTextureForAlbedo = a.useTextureForAlbedo;
		useTextureForAO = a.useTextureForAO;
		useTextureForEmission = a.useTextureForEmission;
		useTextureForMetallic = a.useTextureForMetallic;
		useTextureForNormal = a.useTextureForNormal;
		useTextureForRoughness = a.useTextureForRoughness;

		return *this;
	}

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

		shader.setUniformVec3("albedoColor", albedoColor);
		shader.setUniformVec3("emissionColor", emissionColor);
		shader.setUniformFloat("metallicValue", metallicValue);
		shader.setUniformFloat("roughnessValue", roughnessValue);
	}
};