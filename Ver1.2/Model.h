#pragma once
#define MODEL_H


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include <vector>
#include <iostream>


class Model {
public:
	Model(std::string path) {
		loadModel(path);
	}
	Model(){}
	void Draw() {
		for (unsigned int i = 0;i < meshes.size();i++) {
			meshes[i].Draw();
		}
	}
private:
	std::vector<Mesh> meshes;
	std::string directory;

public:
	void loadModel(std::string path) {
		// Чтение файла с помощью Assimp
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cout << "Error in assimp import!\n" << importer.GetErrorString() << "\n";
		}

		//directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}
private:
	void processNode(aiNode* node, const aiScene* scene) {
		for (unsigned int i = 0;i < node->mNumMeshes;i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}
	Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		for (unsigned int i = 0;i < mesh->mNumVertices;i++) {
			Vertex vertex;
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			if (mesh->mTextureCoords[0]) {
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else {
				vertex.TexCoords = glm::vec2(0, 0);
			}
			// Касательный вектор
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;

			// Вектор бинормали
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
			vertices.push_back(vertex);
		}

		/*
		for (unsigned int i = 0;i < mesh->mNumBones;i++) {
			unsigned int BoneIndex = 0;
			std::string BoneName(mesh->mBones[i]->mName.data);

			if(m_bone)
		}
		*/
		for (unsigned int i = 0;i < mesh->mNumFaces;i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0;j < face.mNumIndices;j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		return Mesh(vertices, indices);
	}
};