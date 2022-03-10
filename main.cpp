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
*	If a primitive has different colors at its vertices, openGL will create a gradient (interpolation).
* 
*	Shaders
*		Don't declare uniforms unless you're going to use them because openGL will delete them.
* 
*	Textures
*		Square textures in powers of 2 are better optimized than a random width/height value.
*		Texture units are assigned to textures. A texture unit contains slots to bundle textures.
*			Holds about 16 textures.
*			Allows the fragment shader to work with all the textures at the same time.
*		Configuring Textures:
*			GL_NEAREST - keeps pixels from being distorted/blurred (pixelated).
*			GL_LINEAR - creates new pixels according to nearby pixels to fill in the blanks (blurry).
*			GL_REPEAT - repeats the texture.
*			GL_MIRRORED_REPEAT - repeats the texture by mirroring it horizontally and vertically..
*			GL_CLAMP_TO_EDGE - stretches the borders.
*			GL_CLAMP_TO_BORDER - puts a flat color outside the image.
*		openGL reads textures from bottom-left to top-right, whereas stb reads from top-left to 
		bottom-right, so images will be reversed by default.
*/

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Objects/VBO.h"
#include "Objects/VAO.h"
#include "Objects/EBO.h"
#include "ShaderClass.h"
#include "Texture.h"

// Vertices for a square
//		   COORDS		//		 COLORS			//	TEX COORDS
GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,		0.8f, 0.3f, 0.02f,		0.0f, 0.0f,	// Lower left corner
	-0.5f,  0.5f, 0.0f,		0.8f, 0.3f, 0.02f,		0.0f, 1.0f, // Upper left corner
	 0.5f,  0.5f, 0.0f,		1.0f, 0.6f, 0.32f,		1.0f, 1.0f,	// Upper right corner
	 0.5f, -0.5f, 0.0f,		0.9f, 0.45f, 0.17f,		1.0f, 0.0f // Lower right corner
};

// Indices of square
GLuint indices[] = {
	0, 2, 1,	// Upper triangle
	0, 3, 2		// Lower triangle
};

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
	GLFWwindow* window = glfwCreateWindow(800, 800, "FirstTimeOpenGL", NULL, NULL);
	// Error checking in case window doesn't get created for whatever reason.
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Tells GLFW to make the window part of the current context.
	glfwMakeContextCurrent(window);
	// ===========================================================================================

	// Glad loads the immediate configurations for openGL.
	gladLoadGL();

	// Area of the window that openGL should be rendered in (bottom-left to top-right).
	glViewport(0, 0, 800, 800);

	// Creates the shader program from the vertex and fragement shader files.
	Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag");

	// Generates Vertex Array Object and binds it.
	VAO vao1;
	vao1.Bind();

	// Generates Vertex Buffer Object and links it to the vertices.
	VBO vbo1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to the indices.
	EBO ebo1(indices, sizeof(indices));

	// Links VBO to VAO
	// Specifies location of coordinates in vertices
	vao1.LinkAttrib(vbo1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	// Specifies location of color in vertices
	vao1.LinkAttrib(vbo1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	// Specifies location of texture coordinates in vertices
	vao1.LinkAttrib(vbo1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// Unbind all to prevent modifying these objects later on.
	vao1.Unbind();
	vbo1.Unbind();
	ebo1.Unbind();

	// Gets the reference value of the scale uniform in the shader program (in default.vert).
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Creates the popCat texture and assigns it to the 0th texture unit.
	Texture popCat("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	popCat.TexUnit(shaderProgram, "tex0", 0);

	// Keeps the window open until it should close. The closing condition can be the close button 
	// or another function.
	while (!glfwWindowShouldClose(window)) {
		// Clears the color of the buffer and gives it another color.
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // Navy Blue
		// Specifies to openGL to use the previous command on the color buffer.
		glClear(GL_COLOR_BUFFER_BIT);
		// Activates shader program
		shaderProgram.Activate();

		// Scales the items drawn to the screen to 1.5x size.
		glUniform1f(uniID, 0.5f); // Can ONLY be done after the shader program is activated.

		popCat.Bind();

		vao1.Bind();

		// glDrawArrays(primitive, start index of vertices, num vertices to draw)
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		// glDrawElements(primitive, num indices, indices data type, start index of indices)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// The back buffer contains the color we want. This swaps the front and back buffer.
		glfwSwapBuffers(window);

		// Tells GLFW to process all polled events (resize, close, minimize, etc.)
		glfwPollEvents();
	}

	// Memory cleanup
	vao1.Delete();
	vbo1.Delete();
	ebo1.Delete();
	popCat.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}