#pragma once

#include <string>

#include "Objects/VAO.h"
#include "Objects/EBO.h"
#include "Camera.h"
#include "Texture.h"
#include <vector>

class Mesh {
public:
	// Store the elements of a mesh
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	VAO vao;

	// Constructs the mesh and links attributes
	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);

	// Draws the mesh
	void Draw(Shader& shader, Camera& camera);
};