#include "ShaderClass.h"

// Reads the contents from shader files into a string.
std::string get_file_contents(const char* filename) {
	std::ifstream in(filename, std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

// Constructs the vertex shader and fragment shader.
Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	// Read vertexFile and fragmentFile and store the strings.
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Convert the shader source strings into character array.
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// VERTEX SHADER
	// Reference to store vertex shader in.
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// glShaderSource(reference value, number of strings for shader, address of shader source code, ?)
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// GPU can't understand the source code, so this compiles it into machine code.
	glCompileShader(vertexShader);
	CompileErrors(vertexShader, "VERTEX");

	// FRAGMENT SHADER
	// Reference to store fragment shader in.
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	CompileErrors(fragmentShader, "FRAGMENT");

	// To use both the shaders, they need to be wrapped into a shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// Wraps up the shader program
	glLinkProgram(ID);
	CompileErrors(ID, "PROGRAM");

	// Shaders can be deleted because they are now in the program.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// Activates the shader program
void Shader::Activate() {
	glUseProgram(ID);
}

// Deletes the shader program
void Shader::Delete() {
	glDeleteProgram(ID);
}

// Error logging for if an error occurs while compiling a shader.
void Shader::CompileErrors(unsigned int shader, const char* type) {
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for: " << type << "\n" << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for: " << type << "\n" << std::endl;
		}
	}
}