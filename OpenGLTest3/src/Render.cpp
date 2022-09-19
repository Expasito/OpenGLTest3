#include "Render.h"

int Render::windowHeight = 600;
int Render::windowWidth = 800;
float* Render::vertices = (float*)malloc(10 * sizeof(float));
size_t Render::verticesSize = 10 * sizeof(float);
std::vector<Render::uniform> Render::uniforms = {};
Entity* Render::skybox = nullptr;
//std::vector<Entity> Render::entities = {};
unsigned int Render::VAO = 0, Render::VBO = 0, Render::EBO = 0;
//Framebuffer stuff
unsigned int Render::fbo = 0, Render::colorTexture = 0, Render::depthTexture = 0;
unsigned int Render::instanceVBO = 0, Render::instanceVBO2=0, Render::instanceVBO3 = 0, Render::instanceVBO4 = 0;
std::vector<glm::vec3> Render::translations = {};
std::vector<glm::vec3> Render::rotations = {};
std::vector<glm::vec3> Render::scalations = {};
std::vector<glm::vec3> Render::color = {};

GLFWwindow* Render::init() {
	//Basic glfw init methods
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_MAXIMIZED,GLFW_TRUE);

	//create window pointer
	GLFWwindow* wind = glfwCreateWindow(windowWidth, windowHeight, "Opengl window", nullptr, nullptr);

	//check to make sure glfw and window are working
	if (!glfwInit() || wind==NULL) {
		std::cout << "Failed to create Window. Failed under Render::init()\n";
		exit(EXIT_FAILURE);
	}
	return wind;
}

void Render::callBacks(GLFWwindow* wind, GLFWkeyfun keyCallBack, GLFWframebuffersizefun buffersizefun, GLFWmousebuttonfun mouseButtonCallback, GLFWscrollfun scrollCallBack, GLFWcursorposfun mouseCallBack) {
	//other functions needed
	glfwMakeContextCurrent(wind);
	gladLoadGL(glfwGetProcAddress);
	glfwSetKeyCallback(wind, keyCallBack);
	glfwSetFramebufferSizeCallback(wind, buffersizefun);
	glfwSetMouseButtonCallback(wind, mouseButtonCallback);
	glfwSetScrollCallback(wind, scrollCallBack);
	glfwSetCursorPosCallback(wind, mouseCallBack);
	glfwSwapInterval(0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glfwSetInputMode(wind, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(wind, 400, 300);
}

void Render::genCubeVert() {
	Render::vertices = (float*)malloc(sizeof(float) * 5 * 36);
	Render::verticesSize = 5*36 * sizeof(float);
	float vertices[] = {
	-1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
	 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
	-1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,  0.0f, 0.0f,

	-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
	-1.0f,  1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,

	-1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  1.0f,  1.0f, 0.0f,

	 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
	 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,

	-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f, -1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
	 1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
	-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,

	-1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
	 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
	 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f, -1.0f,  0.0f, 1.0f
	};
	const float textureDx = .01;
	//modify texture cords by moving +- .01
	for (int i = 3; i <5*36; i += 5) {
		if (vertices[i] == 1.0f)
			vertices[i] -= textureDx;
		if (vertices[i + 1] == 1.0f)
			vertices[i + 1] -= textureDx;
		if (vertices[i] == 0.0f)
			vertices[i] += textureDx;
		if (vertices[i + 1] == 0.0f)
			vertices[i + 1] += textureDx;
	}
	for (int i = 0; i < sizeof(vertices)/sizeof(float); i++) {
		Render::vertices[i] = vertices[i];
	}

}

void Render::initEntities() {
	Render::genCubeVert();
	//Cube::adjustTextCords

	//shader stuff begins here
	glGenVertexArrays(1, &Render::VAO);
	glBindVertexArray(Render::VAO);

	glGenBuffers(1, &Render::VBO);
	
	glGenBuffers(1, &Render::instanceVBO);
	glGenBuffers(1, &Render::instanceVBO2);
	glGenBuffers(1, &Render::instanceVBO3);
	glGenBuffers(1, &Render::instanceVBO4);
	
}

unsigned int Render::getUniformLoc(std::string name) {
	for (Render::uniform uni : Render::uniforms) {
		if (uni.name == name) {
			return uni.loc;
		}
	}
	return -1;
}

void Render::draw(Entity* e) {
	//Tell GPU how to read Cube data
	
	if (e->hasComponent<TextureComponent>()) {
		glBindTexture(GL_TEXTURE_2D, e->getComponent<TextureComponent>()->texture);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, 1);
	}
	
	if (e->hasComponent<TransformComponent>()) {
		if (e->hasComponent<ColorComponent>()) {
			Render::color.push_back(e->getComponent<ColorComponent>()->color);
			//glUniform3fv(Render::getUniformLoc("color"), 1, glm::value_ptr(e->getComponent<ColorComponent>()->color));
		}
		else {
			Render::color.push_back(glm::vec3(1, 1, 1));
			//glUniform3fv(Render::getUniformLoc("color"), 1, glm::value_ptr(glm::vec3(1,1,1)));

		}
		//get position data to shader

		
		//glUniform3fv(Render::getUniformLoc("translateData"), 1, glm::value_ptr(e->getComponent<TransformComponent>()->translate));
		//glUniform3fv(Render::getUniformLoc("rotateData"), 1, glm::value_ptr(e->getComponent<TransformComponent>()->rotate));
		//glUniform3fv(Render::getUniformLoc("scaleData"), 1, glm::value_ptr(e->getComponent<TransformComponent>()->scale));
		Render::translations.push_back(e->getComponent<TransformComponent>()->translate);
		Render::rotations.push_back(e->getComponent<TransformComponent>()->rotate);
		Render::scalations.push_back(e->getComponent<TransformComponent>()->scale);

		//glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void Render::rend() {

	

	glBindTexture(GL_TEXTURE_2D, 6);
	glBindBuffer(GL_ARRAY_BUFFER, Render::VBO);
	glBufferData(GL_ARRAY_BUFFER, Render::verticesSize, Render::vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//get texture data to shader
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//get instancing uniorm array data to shader
	//for (int i = 0; i < Render::translations.size(); i++) {
	//	//std::cout << Render::translations.at(i).x << Render::translations.at(i).y << Render::translations.at(i).z << "\n";
	//	//std::cout << Render::rotations.at(i).x << Render::rotations.at(i).y << Render::rotations.at(i).z << "\n";
	//	//std::cout << Render::scalations.at(i).x << Render::scalations.at(i).y << Render::scalations.at(i).z << "\n";
	//	//std::cout << Render::color.at(i).x << Render::color.at(i).y << Render::color.at(i).z << "\n";
	//	
	//	t = glGetUniformLocation(1, ("translations[" + std::to_string(i) + "]").c_str());
	//	glUniform3fv(t, 1, glm::value_ptr(Render::translations.at(i)));
	//	t = glGetUniformLocation(1, ("rotations[" + std::to_string(i) + "]").c_str());
	//	glUniform3fv(t, 1, glm::value_ptr(Render::rotations.at(i)));
	//	t = glGetUniformLocation(1, ("scalations[" + std::to_string(i) + "]").c_str());
	//	glUniform3fv(t, 1, glm::value_ptr(Render::scalations.at(i)));
	//	t = glGetUniformLocation(1, ("color[" + std::to_string(i) + "]").c_str());
	//	glUniform3fv(t, 1, glm::value_ptr(Render::color.at(i)));
	//}
	//glm::vec2 tr[100000];
	//for (int i = 0; i < Render::translations.size(); i++) {
	//	tr[i] = Render::translations[i];
	//}
	glBindBuffer(GL_ARRAY_BUFFER, Render::instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Render::translations.size(), &Render::translations[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glVertexAttribDivisor(3, 1);

	glBindBuffer(GL_ARRAY_BUFFER, Render::instanceVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Render::translations.size(), &Render::rotations[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glVertexAttribDivisor(4, 1);

	glBindBuffer(GL_ARRAY_BUFFER, Render::instanceVBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Render::translations.size(), &Render::scalations[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glVertexAttribDivisor(5, 1);

	glBindBuffer(GL_ARRAY_BUFFER, Render::instanceVBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Render::translations.size(), &Render::color[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glVertexAttribDivisor(6, 1);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, Render::translations.size());
}

void Render::activateSkybox() {
	if (Render::skybox == nullptr) {
		Render::skybox = new Entity();
		Render::skybox->addComponent<TransformComponent>()->scale = glm::vec3(50, 50, 50);
		Render::skybox->addComponent<TextureComponent>()->texture = 5;
	}
}

void Render::prepareFramebuffer() {
	glGenFramebuffers(1, &Render::fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, Render::fbo);

	glGenTextures(1, &Render::colorTexture);
	glBindTexture(GL_TEXTURE_2D, Render::colorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Render::windowWidth, Render::windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Render::colorTexture, 0);

	glGenTextures(1, &Render::depthTexture);
	glBindTexture(GL_TEXTURE_2D, Render::depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, Render::windowWidth, Render::windowHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, Render::depthTexture, 0);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
}