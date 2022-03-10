#ifndef VAO_CLASS_H
	#define VAO_CLASS_H

	#include<glad/glad.h>
	#include "VBO.h"

	// VAO (Vertex Array Object) stores pointers to one or more VBOs and tells openGL how to interpret 
	// them.
	class VAO {
	public:
		// Reference ID of the VAO.
		GLuint ID;
		// Constructor that generates a VAO.
		VAO();

		// Links VBO to VAO
		void LinkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);

		void Bind();
		void Unbind();
		void Delete();
	};
#endif