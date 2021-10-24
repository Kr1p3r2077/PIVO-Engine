#pragma once
#define MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Material.h"

struct VertexBoneData {
    unsigned int IDs[4];
    float weights[4];
};

struct Vertex {

    // Позиция
    glm::vec3 Position;

    // Нормаль
    glm::vec3 Normal;

    // Текстурные координаты
    glm::vec2 TexCoords;

    // Касательный вектор
    glm::vec3 Tangent;

    // Вектор бинормали (вектор, перпендикулярный касательному вектору и вектору нормали)
    glm::vec3 Bitangent;

    //Веса и индексы костей
    //VertexBoneData boneData;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
		this->vertices = vertices;
		this->indices = indices;

		setupMesh();
	}
    void Draw() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
	// Данные для рендеринга
	unsigned int VAO, VBO, EBO;

	void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),&indices[0], GL_STATIC_DRAW);

        // Координаты вершин
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // Нормали вершин
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        // Текстурные координаты вершин
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        
        // Тангенты
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

        // Битангенты
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

        /*
        // Индексы и веса
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 8, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, boneData));
        */
        glBindVertexArray(0);
	}
};