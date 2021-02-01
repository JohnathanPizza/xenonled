#include <xenonled/graphicsUtils.hh>

Shader::operator int() const{
	return static_cast<int>(id);
}

Shader::operator GLuint() const{
	return id;
}

Shader::Shader(){}

Shader::~Shader(){
	glDeleteShader(id);
}

Shader::Shader(std::string s){
	std::ifstream file;
	file.open(s, std::ios::in);
	if(file.is_open()){
		std::string data;
		while(std::getline(file, data)){
			source += data + "\n";
		}
		file.close();
	}else{
		source = s;
	}
}

void Shader::set(const char* s){
	std::ifstream file;
	file.open(s, std::ios::in);
	if(file.is_open()){
		std::string data;
		while(std::getline(file, data)){
			source += data + "\n";
		}
		file.close();
	}else{
		source = s;
	}
}

void Shader::set(std::ifstream file){
	std::string data;
	while(std::getline(file, data)){
		source += data + "\n";
	}
	file.close();
}

void Shader::compile(shaderType t){
	switch(t){
	case shaderType::VERTEX:
		id = glCreateShader(GL_VERTEX_SHADER);
		break;
	case shaderType::FRAGMENT:
		id = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	}
	const char* s = source.c_str();
	glShaderSource(id, 1, &s, NULL);
	glCompileShader(id);
	int length;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
	if(length){
		char* log = new char[length];
		glGetShaderInfoLog(id, length, NULL, log);
		std::cout << "SHADER COMPILATION FAILED\n";
		std::cout << log << std::endl;
		delete[] log;
	}
}

ShaderProgram::ShaderProgram(){}

void ShaderProgram::use(){ glUseProgram(id); }

Point::Point(float x, float y, int _r, int _g, int _b) : x(x), y(y), r(_r / 255.f), g(_g / 255.f), b(_b / 255.f){}