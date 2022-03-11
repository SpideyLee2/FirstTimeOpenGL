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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Objects/VBO.h"
#include "Objects/VAO.h"
#include "Objects/EBO.h"
#include "ShaderClass.h"
#include "Texture.h"
#include "Camera.h"

const unsigned int width = 800;
const unsigned int height = 800;

// Vertices of plane
GLfloat vertices[] = {
//		  COORDS		//		 COLORS		  // TEX COORDS  //		NORMALS
	-1.0f, 0.0f,  1.0f,     0.0f, 0.0f, 0.0f, 	 0.0f, 0.0f,	0.0f, 1.0f, 0.0f,	// Top side
	-1.0f, 0.0f, -1.0f,     0.0f, 0.0f, 0.0f,	 0.0f, 1.0f,	0.0f, 1.0f, 0.0f,	// Top side
	 1.0f, 0.0f, -1.0f,     0.0f, 0.0f, 0.0f,	 1.0f, 1.0f,	0.0f, 1.0f, 0.0f,	// Top side
	 1.0f, 0.0f,  1.0f,     0.0f, 0.0f, 0.0f,	 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	// Top side
};

// Indices of square
GLuint indices[] = {
	0, 1, 2,	// Bottom side
	0, 2, 3,	// Bottom side
	4, 6, 5,	// Left side
	7, 9, 8,	// Non-facing side
	10, 12, 11, // RIght side
	13, 15, 14	// Facing side
};

GLfloat lightVertices[] = {
	//		COORDINATES
		-0.1f, -0.1f,  0.1f,
		-0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f,  0.1f,
		-0.1f,  0.1f,  0.1f,
		-0.1f,  0.1f, -0.1f,
		 0.1f,  0.1f, -0.1f,
		 0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] = {
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
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
	GLFWwindow* window = glfwCreateWindow(width, height, "FirstTimeOpenGL", NULL, NULL);
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
	glViewport(0, 0, width, height);

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
	vao1.LinkAttrib(vbo1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	// Specifies location of color in vertices
	vao1.LinkAttrib(vbo1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	// Specifies location of texture coordinates in vertices
	vao1.LinkAttrib(vbo1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	// Specifies location of texture coordinates in vertices
	vao1.LinkAttrib(vbo1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));

	// Unbind all to prevent modifying these objects later on.
	vao1.Unbind();
	vbo1.Unbind();
	ebo1.Unbind();

	Shader lightShader("Shaders/light.vert", "Shaders/light.frag");

	VAO lightVAO;
	lightVAO.Bind();

	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndices, sizeof(lightIndices));

	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// Creates the planks texture and assigns it to the 0th texture unit.
	Texture planksTex("planks.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	planksTex.TexUnit(shaderProgram, "tex0", 0);
	// Creates the planks specular texture and assigns it to the 1st texture unit.
	Texture planksSpec("planksSpec.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
	planksSpec.TexUnit(shaderProgram, "tex1", 1);

	// Enables the depth buffer. Otherwise, openGL doesn't know which faces to render on top.
	glEnable(GL_DEPTH_TEST);

	// Initializes a camera that is 2.0 away from the world origin
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Keeps the window open until it should close. The closing condition can be the close button 
	// or another function.
	while (!glfwWindowShouldClose(window)) {
		// Clears the color of the buffer and gives it another color.
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // Navy Blue
		// Specifies to openGL to use the previous command on the color buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Provides inputs for moving the camera around the world
		camera.Inputs(window);
		// Updates the camera matrix
		camera.UpdateMatrix(45.0f, 0.1f, 100.0f);
		// Activates shader program
		shaderProgram.Activate();

		// Exports the camera position to the fragment shader
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.pos.x, camera.pos.y, camera.pos.z);

		// Exports the camera matrix to the vertex shader
		camera.Matrix(shaderProgram, "camMatrix");

		planksTex.Bind();
		planksSpec.Bind();
		vao1.Bind();

		// glDrawElements(primitive, num indices, indices data type, start index of indices)
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

		lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

		// The back buffer contains the color we want. This swaps the front and back buffer.
		glfwSwapBuffers(window);

		// Tells GLFW to process all polled events (resize, close, minimize, etc.)
		glfwPollEvents();
	}

	// Memory cleanup
	vao1.Delete();
	vbo1.Delete();
	ebo1.Delete();
	planksTex.Delete();
	planksSpec.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}