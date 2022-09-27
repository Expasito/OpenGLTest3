#include "Render.h"

int Render::windowHeight = 600;
int Render::windowWidth = 800;
float* Render::vertices = (float*)malloc(10 * sizeof(float));
unsigned int* Render::indices = (unsigned int*)(malloc(6 * sizeof(unsigned int)));
size_t Render::verticesSize = 10 * sizeof(float);
size_t Render::indicesSize = 6 * sizeof(unsigned int);
std::vector<Render::uniform> Render::uniforms = {};
Entity* Render::skybox = nullptr;
//std::vector<Entity> Render::entities = {};
unsigned int Render::VAO = 0, Render::VBO = 0, Render::EBO = 0, Render::IBO=0;
//Framebuffer stuff
unsigned int Render::fbo = 0, Render::colorTexture = 0, Render::depthTexture = 0;
unsigned int Render::instanceVBO = 0, Render::instanceVBO2=0, Render::instanceVBO3 = 0, Render::instanceVBO4 = 0, Render::instanceVBO5=0;
std::vector<glm::vec3> Render::translations = {};
std::vector<glm::vec3> Render::rotations = {};
std::vector<glm::vec3> Render::scalations = {};
std::vector<glm::vec3> Render::color = {};
std::vector<float> Render::texture = {};

GLFWwindow* Render::init() {
	//Basic glfw init methods
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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
	static const int faces = 6;
	Render::vertices = (float*)malloc(sizeof(float) * 5 * 4*faces);
	Render::verticesSize = 5*4*faces* sizeof(float);
	Render::indices = (unsigned int*)(malloc(6*faces * sizeof(unsigned int)));
	Render::indicesSize = sizeof(unsigned int) * 6*faces;

	//float vertices[] = {
	//-1.0f, -1.0f, -1.0f,  0.0f, 0.0f, //bottom left
	// 1.0f, -1.0f, -1.0f,  1.0f, 0.0f, // bottom right
	// 1.0f,  1.0f, -1.0f,  1.0f, 1.0f, // top right
	//-1.0f,  1.0f, -1.0f,  0.0f, 1.0f, //top left
	//
	//-1.0f, -1.0f, 1.0f,   0.0f, 0.0f, //bottom left
	// 1.0f, -1.0f, 1.0f,   1.0f, 0.0f, // bottom right
	// 1.0f,  1.0f, 1.0f,   1.0f, 1.0f, // top right
	//-1.0f,  1.0f, 1.0f,   0.0f, 1.0f, //top left
	//
	//1.0f, -1.0f, -1.0f,   0.0f, 0.0f, //bottom left
	// 1.0f, -1.0f, 1.0f,   1.0f, 0.0f, // bottom right
	// 1.0f,  1.0f, 1.0f,   1.0f, 1.0f, // top right
	//1.0f,  1.0f, -1.0f,   0.0f, 1.0f, //top left
	//
	//-1.0f, -1.0f, -1.0f,  0.0f, 0.0f, //bottom left
	//-1.0f, -1.0f, 1.0f,   1.0f, 0.0f, // bottom right
	// -1.0f,  1.0f, 1.0f,  1.0f, 1.0f, // top right
	//-1.0f,  1.0f, -1.0f,  0.0f, 1.0f, //top left
	//
	//-1.0f, -1.0f, -1.0f,  0.0f, 0.0f, //bottom left
	//1.0f, -1.0f, -1.0f,   1.0f, 0.0f, // bottom right
	// 1.0f, -1.0f, 1.0f,   1.0f, 1.0f, // top right
	//-1.0f,  -1.0f, 1.0f,  0.0f, 1.0f, //top left
	//
	//-1.0f, 1.0f, -1.0f,   0.0f, 0.0f, //bottom left
	//1.0f, 1.0f, -1.0f,    1.0f, 0.0f, // bottom right
	// 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, // top right
	//-1.0f,  1.0f, 1.0f,   0.0f, 1.0f, //top left
	//};

	float vertices[] = {
	// back
	1.0f, -1.0f, -1.0f,  0.0f, 1.0/3.0, //bottom left
	 -1.0f, -1.0f, -1.0f,  .25f, 01.0/3.0, // bottom right
	 -1.0f,  1.0f, -1.0f,  .25f, 2.0/3.0, // top right
	1.0f,  1.0f, -1.0f,  0.0f, 2.0/3.0, //top left
	
	//front
	-1.0f, -1.0f, 1.0f,   .5f, 1.0/3.0, //bottom left
	 1.0f, -1.0f, 1.0f,   .75f, 1.0/3.0, // bottom right
	 1.0f,  1.0f, 1.0f,   .75f, 2.0/3.0, // top right
	-1.0f,  1.0f, 1.0f,   .5f, 2.0/3.0, //top left
	//right
	1.0f, -1.0f, 1.0f,   .75f, 1.0/3.0, //bottom left
	 1.0f, -1.0f, -1.0f,   1.0f, 1.0/3.0, // bottom right
	 1.0f,  1.0f, -1.0f,   1.0f, 2.0/3.0, // top right
	1.0f,  1.0f, 1.0f,   .75f, 2.0/3.0, //top left
	//left
	-1.0f, -1.0f, -1.0f,  0.25f, 1.0/3.0, //bottom left
	-1.0f, -1.0f, 1.0f,   .5f, 1.0/3.0, // bottom right
	 -1.0f,  1.0f, 1.0f,  .5f, 2.0/3.0, // top right
	-1.0f,  1.0f, -1.0f,  .25f, 2.0/3.0, //top left
	//bottom
	-1.0f, 1.0f, -1.0f,  .25f, 2.0 / 3.0, //bottom left.25f, 0.0f,
	-1.0f, 1.0f, 1.0f,   .5f, 2.0 / 3.0, // bottom righ.5f, 0.0f, t
	1.0f, 1.0f, 1.0f,   .5f, 1.0f, // top right  .5f, 1.0/3.0, 
	1.0f,  1.0f, -1.0f,  .25f, 1.0f, //top left   .25f, 1.0/3.0,
	//top
	-1.0f, -1.0f, -1.0f,   .25f, 1.0/3.0, //bottom left
	1.0f, -1.0f, -1.0f,    .25f, 0.0f, // bottom right
	 1.0f, -1.0f, 1.0f,    .5f, 0.0f, // top right
	-1.0f,  -1.0f, 1.0f,   .5f, 1.0/3.0, //top left
	};

	unsigned int indices[] = {
		0,1,2,2,3,0
	};

	for (int i = 0; i < faces; i++) {
		for (int j = 0; j < 6; j++) {
			Render::indices[j+(6*i)] = indices[j] + (i * 4);
		}
	}


	//const float textureDx = .01;
	////modify texture cords by moving +- .01
	//for (int i = 3; i <5*4*faces; i += 5) {
	//	if (vertices[i] == 1.0f)
	//		vertices[i] -= textureDx;
	//	if (vertices[i + 1] == 1.0f)
	//		vertices[i + 1] -= textureDx;
	//	if (vertices[i] == 0.0f)
	//		vertices[i] += textureDx;
	//	if (vertices[i + 1] == 0.0f)
	//		vertices[i + 1] += textureDx;
	//}
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
	glGenBuffers(1, &Render::instanceVBO5);
	glGenBuffers(1, &Render::IBO);
	
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
	if (e->hasComponent<TransformComponent>()) {
		if (e->hasComponent<ColorComponent>()) {
			Render::color.push_back(e->getComponent<ColorComponent>()->color);
		}
		else {
			Render::color.push_back(glm::vec3(1, 1, 1));
		}
		if (e->hasComponent<TextureComponent>()) {
			Render::texture.push_back(e->getComponent<TextureComponent>()->texture);
		}
		else {
			Render::texture.push_back(0);
		}
		//Render::texture.push_back(e->getComponent<TextureComponent>()->texture);
		Render::translations.push_back(e->getComponent<TransformComponent>()->translate);
		Render::rotations.push_back(e->getComponent<TransformComponent>()->rotate);
		Render::scalations.push_back(e->getComponent<TransformComponent>()->scale);
	}
}

void Render::rend() {

	
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, Render::VBO);
	glBufferData(GL_ARRAY_BUFFER, Render::verticesSize, Render::vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Render::IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*6*6, Render::indices, GL_STATIC_DRAW);

	//get texture data to shader
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//get instancing uniorm array data to shader

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

	glBindBuffer(GL_ARRAY_BUFFER, Render::instanceVBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Render::translations.size(), &Render::texture[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0);
	glVertexAttribDivisor(7, 1);
	
	glBindTextureUnit(0, 0);
	glBindTextureUnit(1, 1);
	glBindTextureUnit(2, 2);
	glBindTextureUnit(3, 3);
	glBindTextureUnit(4, 4);
	glBindTextureUnit(5, 5);



	glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0,Render::translations.size());

	//clear data for each draw
	Render::translations = {};
	Render::rotations = {};
	Render::scalations = {};
	Render::color = {};
	Render::texture = {};
}

void Render::activateSkybox() {
	if (Render::skybox == nullptr) {
		Render::skybox = new Entity();
		Render::skybox->addComponent<TransformComponent>()->scale = glm::vec3(500, 500, 500);
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