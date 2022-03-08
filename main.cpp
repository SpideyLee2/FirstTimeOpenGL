/*
* READ THE VENDORS' COMMENTS ON THE INDIVIDUAL METHODS FOR MORE DETAILS! They're quite descriptive.
* Notes:
*	Frames are generated from the top-left of the screen to the bottom-right.
*	Buffers are a space in memory for storing pixels.
*	The front buffer contains the frame that is currently being displayed on screen while the back
*	buffer is creating the frame the that will be displayed next. 
*	Frame-to-frame, the back buffer is overwriting the front buffer.
*/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
	// ============================= USING GLFW TO MAKE A WINDOW =============================
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

	// Glad loads the immediate configurations for openGL.
	gladLoadGL();

	// Area of the window openGL should be rendered (bottom-left to top-right).
	glViewport(0, 0, 1280, 720);

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
		// Tells GLFW to process all polled events (resize, close, minimize, etc.)
		glfwPollEvents();
	}

	// Destroys the window and terminates glfw library
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}