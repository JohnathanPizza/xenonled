#ifndef GRAPHICS_UTILS_H
#define GRAPHICS_UTILS_H

#define GLEW_STATIC

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

enum class shaderType {VERTEX, FRAGMENT};

struct Shader{
	std::string source;
	GLuint id = 0;
	operator int() const;
	operator GLuint() const;
	Shader();
	~Shader();
	Shader(std::string);
	void set(const char* s);
	void set(std::ifstream file);
	void compile(shaderType t);
};

struct ShaderProgram{
	GLuint id = 0;
	ShaderProgram();
	template<typename... T>
	void makeShaderProgram(T&... t){
		GLuint p = glCreateProgram();
		std::vector<int> v = {t...};
		for(int s : v){
			glAttachShader(p, s);
		}
		glLinkProgram(p);
		id = p;
		int length = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		if(length){
			char* log = new char[length];
			glGetProgramInfoLog(id, length, NULL, log);
			std::cout << "SHADER LINKING FAILED\n";
			std::cout << log << std::endl;
			delete[] log;
		}
	}
	void use();
};

struct Point{
	float x = 0.f, y = 0.f;
	float r = 0.f, g = 0.f, b = 0.f;
	Point() = default;
	Point(float x, float y, int _r = 0, int _g = 0, int _b = 0);
};

#endif