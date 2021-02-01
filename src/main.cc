#define GLEW_STATIC

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <xenonled/graphicsUtils.hh>
#include <xenonled/command.hh>
#include <xenonled/CommandManager.hh>
#include <xenonled/essential.hh>
#include <xenonled/shaders.hh>
#include <xenonled/inogen.hh>
#include <xenonled/xenonex.hh>

int width = 1000, height = 1000;
glm::mat4 projection;

ShaderProgram shaderp;

void windowReshapeCallback(GLFWwindow* win, int w, int h){
	width = w;
	height = h;
	glViewport(0, 0, width, height);
	projection = glm::perspective(90.f, (float)width / (float)height, 0.1f, 10.0f);
	glProgramUniformMatrix4fv(shaderp.id, glGetUniformLocation(shaderp.id, "projTransform"), 1, GL_FALSE, glm::value_ptr(projection));
}

template<typename T>
bool ask(T& out, const std::string s = ""){
	std::cout << s;
	std::cin >> out;
	bool r = std::cin.good();
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return r;
}

template<typename T>
T pick(const std::string question, const std::vector<T>& opt, const std::string bad = "try again"){
	T out;
	std::string redo;
	do{
		std::cout << redo << question << std::endl;
		for(const T& var : opt){
			std::cout << var << std::endl;
		}
		ask(out);
		redo = bad + '\n';
	}while(std::find(opt.begin(), opt.end(), out) == opt.end());
	return out;
}

int main(int argc, char** argv){
	xenon::CommandManager cm;
	xenon::mainCM = &cm;
	cm.commands.emplace_back("i", xenonCommands::i, CHECKTYPE::CTvoid, CHECKTYPE::CTnum);
	cm.commands.emplace_back("setpix", xenonCommands::setpix, CHECKTYPE::CTnum, CHECKTYPE::CTnum);
	cm.commands.emplace_back("setpix", xenonCommands::setpix, CHECKTYPE::CTnum, CHECKTYPE::CTnum, CHECKTYPE::CTnum, CHECKTYPE::CTnum);
	cm.commands.emplace_back("d", xenonCommands::d, CHECKTYPE::CTvoid, CHECKTYPE::CTnum);
	cm.commands.emplace_back("set", xenonCommands::set, CHECKTYPE::CTvar, CHECKTYPE::CTnum);
	cm.commands.emplace_back("if", xenonCommands::_if, CHECKTYPE::CTnum);
	cm.commands.emplace_back("else", xenonCommands::_else);
	cm.commands.emplace_back("elseif", xenonCommands::elseif, CHECKTYPE::CTnum);
	cm.commands.emplace_back("loop", xenonCommands::loop, CHECKTYPE::CTnum);
	cm.commands.emplace_back("continue", xenonCommands::end);
	cm.commands.emplace_back("end", xenonCommands::end);
	cm.commands.emplace_back("swapd", xenonCommands::swapd, CHECKTYPE::CTnum, CHECKTYPE::CTnum);
	cm.commands.emplace_back("setall", xenonCommands::setall, CHECKTYPE::CTnum);
	cm.commands.emplace_back("setall", xenonCommands::setall, CHECKTYPE::CTnum, CHECKTYPE::CTnum, CHECKTYPE::CTnum);
	cm.commands.emplace_back("break", xenonCommands::_break);
	cm.commands.emplace_back("tag", xenonCommands::tag, CHECKTYPE::CTvoid);
	cm.commands.emplace_back("tag", xenonCommands::tag, CHECKTYPE::CTvoid, CHECKTYPE::CTvoid);
   	cm.commands.emplace_back("newstrip", xenonCommands::newstrip, CHECKTYPE::CTvoid, CHECKTYPE::CTnum, CHECKTYPE::CTnum);
	cm.commands.emplace_back("setstrip", xenonCommands::setstrip, CHECKTYPE::CTvoid);
	cm.commands.emplace_back("show", xenonCommands::show);
	//cm.commands.emplace_back("checker", xenonCommands::checker, CHECKTYPE::CTnum, CHECKTYPE::CTnum, CHECKTYPE::CTnum, CHECKTYPE::CTnum);
	std::string fname;
	std::string text;
	if(argc == 2){
		fname = argv[1];
	}else{
		while(1){
			ask(fname, "where is the file to process (e to quit)\n");
			if(fname == "e"){
				return 0;
			}
			std::ifstream file(fname, std::ios::in);
			if(!file.is_open()){
				std::cout << "couldn't open file\n";
			}else{
				text = xenonTools::readf(file);
				file.close();
				break;
			}
		}
	}
	try{
		std::cout << "processing selected file...\n";
		xenonTools::process(text, cm, xenon::rawList);
		std::cout << "processing done\n";
	}catch(xenon::XenonEx& e){
		std::cout << "xenon processing failed\n";
		std::cout << e.what() << std::endl;
		return 1;
	}
	while(true){
		std::string in;
		ask(in, "what do you want to do?\n=======================\ng - generate .ino file\np - preview led\ne - exit\n");
		if(in == "e"){
			break;
		}else if(in == "p"){
			if(!glfwInit()){
				return EXIT_FAILURE;
			}
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
			GLFWwindow* window = glfwCreateWindow(width, height, "xenonled", NULL, NULL);
			glfwMakeContextCurrent(window);
			glfwSetWindowSizeCallback(window, windowReshapeCallback);
			glfwSwapInterval(1);
			if(glewInit() != GLEW_OK){
				return EXIT_FAILURE;
			}
			Shader vs(xenonShaders::vertexShader);
			vs.compile(shaderType::VERTEX);
			Shader fs(xenonShaders::fragmentShader);
			fs.compile(shaderType::FRAGMENT);
			shaderp.makeShaderProgram(vs, fs);
			shaderp.use();
			GLuint vao = 0;
			GLuint vbo = 0;
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glClearColor(0.f, 0.f, 0.f, 1.0f);
			while(!glfwWindowShouldClose(window)){
				if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
					glfwSetWindowShouldClose(window, true);
				}
				float ar = (float)width / height;
				glUniformMatrix4fv(glGetUniformLocation(shaderp.id, "projTransform"), 1, GL_FALSE, glm::value_ptr(glm::ortho(-0.5f * ar, 0.5f * ar, -0.5f, 0.5f, 0.1f, 10.f)));
				float time = (float)glfwGetTime();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				if(!xenonTools::execute(xenon::rawList, cm)){
					std::cout << "xenon execution failed\n";
					return 2;
				}
				std::vector<Point> vertices;
				int largest = 0;
				for(const auto& a : xenon::stripList){
					if(a->getSize() > largest){
						largest = a->getSize();
					}
				}
				glPointSize(width * 0.9f / largest);
				int sizev = xenon::stripList.size();
				for(int a = 0; a < sizev; ++a){
					int sizeh = xenon::stripList.at(a)->getSize();
					for(int b = 0; b < sizeh; ++b){
						vertices.emplace_back(0.9f / (sizeh + 1) * (b + 1) - 0.45f, (-1.f / (sizev + 1)) * (a + 1) + 0.5f, xenon::stripList.at(a)->get(b)->r(), xenon::stripList.at(a)->get(b)->g(), xenon::stripList.at(a)->get(b)->b());
					}
				}
				glBindVertexArray(vao);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * vertices.size(), vertices.data(), GL_STATIC_DRAW);
				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point), 0);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Point), reinterpret_cast<const void*>(2 * sizeof(float)));
				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);
				glDrawArrays(GL_POINTS, 0, vertices.size());
				glfwSwapBuffers(window);
				glfwPollEvents();
			}
			glfwDestroyWindow(window);
			glfwTerminate();
			xenon::reset();
		}else if(in == "g"){
			std::string name;
			ask(name, "what should the output file be named?");
			xenon::inogen(name);
		}else{
			std::cout << "not a valid command\n";
		}
		std::cout << std::endl;
	}
	return 0;
}