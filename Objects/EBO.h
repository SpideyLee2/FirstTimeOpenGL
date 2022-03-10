#ifndef EBO_CLASS_H
	#define EBO_CLASS_H

	#include<glad/glad.h>


	// EBO (Entity Buffer Object) stores the indices.
	class EBO {
	public:
		// Reference ID of the EBO.
		GLuint ID;

		// Constructor that generates a EBO and links it to indices.
		EBO(GLuint* indices, GLsizeiptr size);

		void Bind();
		void Unbind();
		void Delete();
	};
#endif