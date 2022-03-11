#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 pos) : width(width), height(height), pos(pos) {}

void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane) {
	// Used to convert world coords into view coords
	glm::mat4 view = glm::mat4(1.0f);
	// Used to convert view coords into clip coords
	glm::mat4 proj = glm::mat4(1.0f);

	// Specifies a look at view matrix
	view = glm::lookAt(pos, pos + orientation, up);
	// Adds perspective to the scene
	// glm::perspective(field of view, aspect ratio, closest clip plane, furthest clip plane)
	proj = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);

	// Assigns the camera matrix
	cameraMatrix = proj * view;
}

void Camera::Matrix(Shader& shader, const char* uniform) {
	// Exports the camera matrix to the vertex shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window) {
	// Keyboard inputs for moving the camera around in the world
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		pos += speed * orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		pos += speed * -orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		pos += speed * -glm::normalize(glm::cross(orientation, up));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		pos += speed * glm::normalize(glm::cross(orientation, up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		pos += speed * up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		pos += speed * -up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		speed = 0.2f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
		speed = 0.05f;
	}

	// Mouse inputs for rotating the camera when left click is being held
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		// Hides the cursor while rotating the screen
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents the camera from jumping around when first clicking left click
		if (firstClick) {
			glfwSetCursorPos(window, width / 2, height / 2);
			firstClick = false;
		}

		// Stores the x and y positions of the cursor
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle
		// of the screen and then transforms them into degrees.
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (height / 2)) / height;

		// Precalculates the new position of the camera so to prevent barrel-rolling
		glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), 
											   glm::normalize(glm::cross(orientation, up)));
		if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
			orientation = newOrientation;
		}

		// Rotates the orientation left and right
		orientation = glm::rotate(orientation, glm::radians(-rotY), up);

		// Resets the cursor position to the middle of the window
		glfwSetCursorPos(window, width / 2, height / 2);
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		// Unhides the cursor since the camera is not longer being rotated
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
}
