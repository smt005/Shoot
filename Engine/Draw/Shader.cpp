
#include "Shader.h"
#include "../App/File.h"

#include <GL/glew.h>

#include<fstream>
#include<iostream>
#include<string.h>

bool Shader::getShaderProgram(unsigned int& program, const string& vertexLink, const string& fragmentLink)
{
    const char* shaderSource = File::loadText(fragmentLink);
	if (!shaderSource)
		return false;

	GLuint _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_fragmentShader, 1, &shaderSource, 0);
	delete[] shaderSource;
	glCompileShader(_fragmentShader);

	GLint isShaderCompiled;
	glGetShaderiv(_fragmentShader, GL_COMPILE_STATUS, &isShaderCompiled);
	if (!isShaderCompiled)
	{
		// If an error happened, first retrieve the length of the log message
		int infoLogLength, charactersWritten;
		glGetShaderiv(_fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(_fragmentShader, infoLogLength, &charactersWritten, infoLog);
        
        #ifdef BUILD_OSX
                printf("Shader compiled fragment ERROR: %s", infoLog);
		#elif BUILD_WIN_GLES
			_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "Shader compiled fragment ERROR: %s\n", infoLog);
        #endif
        
		delete[] infoLog;
		return false;
	}

    shaderSource = File::loadText(vertexLink);
	if (!shaderSource)
		return false;

	GLuint _vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(_vertexShader, 1, &shaderSource, 0);
	delete[] shaderSource;
	glCompileShader(_vertexShader);

	glGetShaderiv(_vertexShader, GL_COMPILE_STATUS, &isShaderCompiled);
	if (!isShaderCompiled)
	{
		// If an error happened, first retrieve the length of the log message
		int infoLogLength, charactersWritten;
		glGetShaderiv(_vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* infoLog = new char[infoLogLength];
		glGetShaderInfoLog(_vertexShader, infoLogLength, &charactersWritten, infoLog);

        #ifdef BUILD_OSX
               printf("Shader compiled vertex ERROR: %s", infoLog);
		#elif BUILD_WIN_GLES
			_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "Shader compiled vertex ERROR: %s\n", infoLog);
        #endif

		delete[] infoLog;
		return false;
	}

	GLuint _shaderProgram = glCreateProgram();
	glAttachShader(_shaderProgram, _fragmentShader);
	glAttachShader(_shaderProgram, _vertexShader);

	glLinkProgram(_shaderProgram);

	GLint isLinked;
	glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &isLinked);
	if (!isLinked)
	{
		// If an error happened, first retrieve the length of the log message
		int infoLogLength, charactersWritten;
		glGetProgramiv(_shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);

		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(_shaderProgram, infoLogLength, &charactersWritten, infoLog);

        #ifdef BUILD_OSX
                printf("Shader linked ERROR: %s", infoLog);
		#elif BUILD_WIN_GLES
			_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "Shader linked ERROR: %s \n", infoLog);
        #endif

		delete[] infoLog;
		return false;
	}

	program = _shaderProgram;
	return true;
}

void deInitializeGLState(GLuint fragmentShader, GLuint vertexShader, GLuint shaderProgram, GLuint vertexBuffer)
{
	// Frees the OpenGL handles for the program and the 2 shaders
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteProgram(shaderProgram);

	// Delete the VBO as it is no longer needed
	glDeleteBuffers(1, &vertexBuffer);
}
