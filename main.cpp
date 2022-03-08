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
*/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// OpenGL doesn't provide defaults for vertex and fragment shaders.
// LOOK INTO THESE!!!
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"	FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

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

	// VERTEX SHADER
	// Reference to store vertex shader in.
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// glShaderSource(reference value, number of strings for shader, address of shader source code, ?)
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// GPU can't understand the source code, so this compiles it into machine code.
	glCompileShader(vertexShader);

	// FRAGMENT SHADER
	// Reference to store fragment shader in.
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// To use both the shaders, they need to be wrapped into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Wraps up the shader program
	glLinkProgram(shaderProgram);

	// Shaders can be deleted because they are now in the program.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// VBO (Vertex Buffer Object) is actually an array of references, but there's only one object, 
	// so we only need one.
	// VAO (Vertex Array Object) stores pointers to one or more VBOs and tells openGL how to 
	// interpret them.
	// EBO (Entity Buffer Object) stores the indices.
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO); // The VAO MUST be generated before the VBO.
	// Generates the buffer object containing 1 objects.
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Binds VAO
	glBindVertexArray(VAO);
	// Binds VBO to GL_ARRAY_BUFFER, making VBO the binded object.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// GL_STREAM_...	- vertices will be modified once and used a few times.
	// GL_STATIC_...	- vertices will be modified once and used many times.
	// GL_DYNAMIC_...	- vertices will be modified many times and used many times.
	// GL_..._DRAW		- vertices will be used to draw an image on the screen.
	// GL_..._READ 		- vertices will read the buffer object into it.
	// GL_..._COPY 		- vertices will read the buffer object into it and be used to draw an image 
	//					  on the screen.
	// Stores vertices in the VBO.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Stores indices in the EBO.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Configures the VAO so openGL knows how to use the VBO
	// glVertexAttribPointer(pos of vertex attrib, num values per vertex, type of values, 
	//						 coords are integers?, size of vertices, 
	//						 pointer to vertices begin in array)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbinds VBO, VBA, and EBO to prevent accidentally changing them.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Must be unbinded after VAO since it is stored in 
											  // the VAO.


	// Clears the color of the buffer and gives it another color.
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // Navy Blue
	// Specifies to openGL to use the previous command on the color buffer.
	glClear(GL_COLOR_BUFFER_BIT);
	// The back buffer contains the color we want. This swaps the front and back buffer.
	glfwSwapBuffers(window);

	// Swapping the buffers twice more shows that buffers are never deleted; the front and back 
	// buffers just replace one another.
	glfwSwapBuffers(window);
	glfwSwapBuffers(window);

	// Keeps the window open until it should close. The closing condition can be the close button 
	// or another function.
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// Activates shader program
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		// glDrawArrays(primitive, start index of vertices, num vertices to draw)
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		// glDrawElements(primitive, num indices, indices data type, start index of indices)
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		// Tells GLFW to process all polled events (resize, close, minimize, etc.)
		glfwPollEvents();
	}

	// Memory cleanup
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}