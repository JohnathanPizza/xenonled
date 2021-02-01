#ifndef SHADERS_H
#define SHADERS_H

#include <string>

namespace xenonShaders{
	std::string vertexShader(
		"#version 420 core\n"
		"layout (location = 0) in vec2 pos;\n"
		"layout (location = 1) in vec3 col;\n"
		"out vec4 c;\n"
		"uniform mat4 projTransform;\n"
		"void main(){\n"
			"gl_Position = projTransform * vec4(pos.x, pos.y, -1.0, 1.0);\n"
			"c = vec4(col, 1.0);\n"
		"}");

	std::string fragmentShader(
		"#version 420 core\n"
		"in vec4 c;\n"
		"out vec4 FragColor;\n"
		"void main(){\n"
			"FragColor = c;\n"
		"}"
	);
}

#endif