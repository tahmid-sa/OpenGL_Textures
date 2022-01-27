#ifndef  SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#pragma once
class Shader
{
public:
	// The shader program ID
	unsigned int ID;

	// Constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);

	// Use/activate the shader program
	void use();

	// Utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	// Delete the shader program
	void deleteProgram();
};

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	// Retrieve the vertex/fragment source code from the file paths
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// Ensure ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// Open the files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		// Read the file's buffer contents into streams
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// Close the files handlers
		vShaderFile.close();
		fShaderFile.close();

		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n" << std::endl;
	}

	// Convert the shader codes to C style strings
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// Compile the shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[1024];

	// Generate vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	// Print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Generate fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	// Checking if compilation of the fragment shader was a success
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Creating a shader program object
	ID = glCreateProgram();

	// Attach and link the shaders to the program object
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	// Checking if linking of the shaders to program object was a success
	glGetProgramiv(ID, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(ID, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Delete the shader objects
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

// Use the shader program
void Shader::use() {
	glUseProgram(ID);
}

// Utility uniform functions
void Shader::setBool(const std::string& name, bool value) const {
	int locationOfName = glGetUniformLocation(ID, name.c_str());
	glUniform1i(locationOfName, (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
	int locationOfName = glGetUniformLocation(ID, name.c_str());
	glUniform1i(locationOfName, value);
}

void Shader::setFloat(const std::string& name, float value) const {
	int locationOfName = glGetUniformLocation(ID, name.c_str());
	glUniform1i(locationOfName, value);
}

void Shader::deleteProgram() {
	glDeleteProgram(ID);
}

#endif // ! SHADER_H
