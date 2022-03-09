/*
* READ THE VENDORS' COMMENTS ON THE INDIVIDUAL METHODS FOR MORE DETAILS! GLFW's are quite descriptive.
* Notes:
*	Frames are generated from the top-left of the screen to the bottom-right.
*	Buffers are a space in memory for storing pixels.
*	The front buffer contains the frame that is currently being displayed on screen while the back
	buffer is creating the frame the that will be displayed next. 
*	Frame-to-frame, the back buffer is overwriting the front buffer.
 
*	Graphics Pipeline
*		Series of functions that take data at the beginning and output a frame at the end.
*		Input: Vertex data (array of vertices which also contain things like color, texture coords, 
		etc.)
	
*	All openGL objects are accessed by reference.
*	Transporting data between CPU and GPU is slow, buffers are used to send batches of data.
*	Binding in openGL means making a certain object the current (binded) object. The binded object 
	catches function calls to objects of that type.
*	OpenGL doesn't provide defaults for vertex and fragment shaders.
*/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "ShaderClass.h"


int main() {
	// =============================== USING GLFW TO MAKE A WINDOW ===============================
	// Initialize glfw library
	glfwInit();

	// GLFW doesn't know what version of openGL to use.
	// glfwWindowHint allows you to specify a hint and a value (version type, version number)
	// Using GLFW 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// An openGL profile is a package of functions. There are only two (CORE and COMPATIBILITY).
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creates a GLFW window of size 1280x720 with a title and in windowed mode.
	GLFWwindow* window = glfwCreateWindow(1280, 720, "FirstTimeOpenGL", NULL, NULL);
	// Error checking in case window doesn't get created for whatever reason.
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Tells GLFW to make the window part of the current context.
	glfwMakeContextCurrent(window);
	// ===========================================================================================

	//// Vertices for an equilateral triangle (x,y,z)
	//GLfloat vertices[] = {
	//	-0.5f * ((float)720 / 1280), -0.5f * float(sqrt(3)) / 3, 0.0f,	// Lower left corner
	//	0.5f * ((float)720 / 1280), -0.5f * float(sqrt(3)) / 3, 0.0f,	// Lower right corner
	//	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f						// Upper corner
	//};

	// Vertices for an equilateral triangle with triange cutout in center
	GLfloat vertices[] = {
		-0.5f * ((float)720 / 1280), -0.5f * float(sqrt(3)) / 3, 0.0f,		// Lower left corner
		0.5f * ((float)720 / 1280), -0.5f * float(sqrt(3)) / 3, 0.0f,		// Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,							// Upper corner
		-0.5f * ((float)720 / 1280) / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,	// Inner left
		0.5f * ((float)720 / 1280) / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,	// Inner right
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f								// Inner down
	};

	// Indices for referencing vertices when generating index buffer
	GLuint indices[] = {
		0, 3, 5,	// Lower left triangle
		3, 2, 4,	// Upper triangle
		5, 4, 1		// Lower right triangle
	};

	// Glad loads the immediate configurations for openGL.
	gladLoadGL();

	// Area of the window openGL should be rendered (bottom-left to top-right).
	glViewport(0, 0, 1280, 720);

	// Creates the shader program from the vertex and fragement shader files.
	Shader shaderProgram("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it.
	VAO vao1;
	vao1.Bind();

	// Generates Vertex Buffer Object and links it to the vertices.
	VBO vbo1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to the indices.
	EBO ebo1(indices, sizeof(indices));

	// Links VBO to VAO
	vao1.LinkVBO(vbo1, 0);

	// Prevents modifying these objects by accident.
	vao1.Unbind();
	vbo1.Unbind();
	ebo1.Unbind();

	// Keeps the window open until it should close. The closing condition can be the close button 
	// or another function.
	while (!glfwWindowShouldClose(window)) {
		// Clears the color of the buffer and gives it another color.
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // Navy Blue
		// Specifies to openGL to use the previous command on the color buffer.
		glClear(GL_COLOR_BUFFER_BIT);
		// Activates shader program
		shaderProgram.Activate();

		vao1.Bind();

		// glDrawArrays(primitive, start index of vertices, num vertices to draw)
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		// glDrawElements(primitive, num indices, indices data type, start index of indices)
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		// The back buffer contains the color we want. This swaps the front and back buffer.
		glfwSwapBuffers(window);

		// Tells GLFW to process all polled events (resize, close, minimize, etc.)
		glfwPollEvents();
	}

	// Memory cleanup
	vao1.Delete();
	vbo1.Delete();
	ebo1.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}