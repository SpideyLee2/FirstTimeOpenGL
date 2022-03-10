#include "VAO.h"

// Constructor that generates a VAO.
VAO::VAO() {
	glGenVertexArrays(1, &ID); // The VAO MUST be generated before the VBO.
}

// Links VBO to VAO
void VAO::LinkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	vbo.Bind();

	// Configures the VAO so openGL knows how to use the VBO
	// glVertexAttribPointer(pos of vertex attrib, num values per vertex, type of values, 
	//						 are coords integers?, size of vertices, 
	//						 pointer to vertices begin in array)
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);

	vbo.Unbind();
}

void VAO::Bind() {
	glBindVertexArray(ID);
}

void VAO::Unbind() {
	glBindVertexArray(0);
}

void VAO::Delete() {
	glDeleteVertexArrays(1, &ID);
}