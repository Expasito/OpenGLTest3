#include "Render.h"

int Render::windowHeight = 600;
int Render::windowWidth = 800;
float* Render::vertices = (float*)malloc(10 * sizeof(float));
size_t Render::verticesSize = 10 * sizeof(float);
std::vector<Render::uniform> Render::uniforms = {};
Entity* Render::skybox = nullptr;
//std::vector<Entity> Render::entities = {};
unsigned int Render::VAO = 0, Render::VBO = 0, Render::EBO = 0;
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
	glBindBuffer(GL_ARRAY_BUFFER, Render::VBO);
	glBufferData(GL_ARRAY_BUFFER, Render::verticesSize, Render::vertices, GL_STATIC_DRAW);
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
	if (e->hasComponent<ColorComponent>()) {
		glUniform3fv(Render::getUniformLoc("color"), 1, glm::value_ptr(e->getComponent<ColorComponent>()->color));
	}
	else {
		glUniform3fv(Render::getUniformLoc("color"), 1, glm::value_ptr(glm::vec3(1,1,1)));

	}
	if (e->hasComponent<TransformComponent>()) {
		//get position data to shader
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//get texture data to shader
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		
		glUniform3fv(Render::getUniformLoc("translateData"), 1, glm::value_ptr(e->getComponent<TransformComponent>()->translate));
		glUniform3fv(Render::getUniformLoc("rotateData"), 1, glm::value_ptr(e->getComponent<TransformComponent>()->rotate));
		glUniform3fv(Render::getUniformLoc("scaleData"), 1, glm::value_ptr(e->getComponent<TransformComponent>()->scale));

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void Render::activateSkybox() {
	if (Render::skybox == nullptr) {
		Render::skybox = new Entity();
		Render::skybox->addComponent<TransformComponent>()->scale = glm::vec3(50, 50, 50);
		Render::skybox->addComponent<TextureComponent>()->texture = 5;
	}
}