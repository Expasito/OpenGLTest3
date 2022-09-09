#include "Headers.h"
#include "Shaders.h"
#include "Render.h"
#include "Object.h"
#include <random>

#define clearMemory delete[] objs; glfwDestroyWindow(window); glfwTerminate();


static bool mousePressed=false;
static float fov = 45;
static bool left = false, right = false, up1 = false, down = false;
static float lastX = 400, lastY = 300;
static float xoffset, yoffset;
const float sensitivity = .3f;
static float yaw=-90.0f, pitch=0, roll=0;
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 direction;


void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		left = true;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
		left = false;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		right = true;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
		right = false;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		up1 = true;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
		up1 = false;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		down = true;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
		down = false;
}

void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		if (mousePressed) {
			mousePressed = false;
		}
		else if (!mousePressed) {
			mousePressed = true;
		}
	}
}

void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset) {
	fov -= yoffset*3;
	if (fov < 1.0f)
		fov = 1;
	if (fov > 120.0f)
		fov = 120;

}

void mouseCallBack(GLFWwindow* window, double xpos, double ypos) {
	xoffset = xpos - lastX;
	yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.9)
		pitch = 89.9;
	if (pitch < -89.9)
		pitch = -89.9;
	
	direction.x = glm::cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


//Define static class variables to be reused
//float Object::vertices[] =  {
//	// positions          // colors           // texture coords
//	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
//	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
//};

float Object::vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

unsigned int Object::indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
};

float randomInRange() {
	return (float)((float)std::rand() / (float)RAND_MAX) * 2 - 1;
}



int main() {
	
	//Create window and set key callback functions
	GLFWwindow* window = Render::init();
	Render::callBacks(window, keyCallBack, framebuffer_size_callback, mouseButtonCallBack,scrollCallBack, mouseCallBack);

	//Set random generator
	std::srand(std::time(0));
	
	
	//Create an array of objects to be drawn
	const int objsCount = 100;
	Object* objs = new Object[objsCount];
	for (int i = 0; i < objsCount; i++) {
		objs[i] = Object();
	}


	const float textureDx = .01;
	//modify texture cords by moving +- .01
	for (int i = 3; i < 36*5; i += 5) {
		if (Object::vertices[i] == 1.0f)
			Object::vertices[i] -= textureDx;
		if (Object::vertices[i+1] == 1.0f)
			Object::vertices[i+1] -= textureDx;
		if (Object::vertices[i] == 0.0f)
			Object::vertices[i] += textureDx;
		if (Object::vertices[i + 1] == 0.0f)
			Object::vertices[i + 1] += textureDx;
	}


	//camera data
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

	//target
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	//right axis
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	//up axis
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	//model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 0.0f));

	//view matrix
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));

	//projection matrix
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);


	//camera directions
	

	//modify view matrix
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	const float radius = 10.0f;
	

	//define how the shaders read texture inputs
	Shaders::defineTextureInputs();


	//load textures begins here
	unsigned int texture1 = Shaders::loadTexture("../OpenGLTest3/res/textures/Texture1.png");
	unsigned int texture2 = Shaders::loadTexture("../OpenGLTest3/res/textures/Texture2.png");
	unsigned int texture3 = Shaders::loadTexture("../OpenGLTest3/res/textures/Deor.png");


	//shader stuff begins here
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Object::vertices), Object::vertices, GL_STATIC_DRAW);

	/*glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Object::indices), Object::indices, GL_STATIC_DRAW);*/

	
	


	Shaders::ShaderProgramSource source = Shaders::ParseShader("../OpenGLTest3/res/shaders/Shader.shader");
	Shaders::CompileShaderStatus shaderRet = Shaders::CreateShader(source.VertexSource, source.FragmentSource);
	//Exit program if shader error
	if (shaderRet.error) {
		clearMemory
		exit(1);
	}
	
	unsigned int shader = shaderRet.id;
	glUseProgram(shader);

	//get position data to shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	////get color data to shader
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//get texture data to shader
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//define which texture to use
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);



	//Get uniform locations
	unsigned int translateDataLoc = glGetUniformLocation(shader, "translateData");
	unsigned int rotateDataLoc = glGetUniformLocation(shader, "rotateData");
	unsigned int scaleDataLoc = glGetUniformLocation(shader, "scaleData");


	//Get uniforms for camera data
	unsigned int modelLoc = glGetUniformLocation(shader, "model");
	unsigned int viewLoc = glGetUniformLocation(shader, "view");
	unsigned int projectionLoc = glGetUniformLocation(shader, "projection");
	


	//create random values for transform data
	for (int i = 0; i < objsCount; i++) {
		objs[i].translate.x = randomInRange()*4;
		objs[i].translate.y = randomInRange() * 4;
		objs[i].translate.z = randomInRange()*4;
		objs[i].rotate.x = randomInRange();
		objs[i].rotate.y = randomInRange();
		objs[i].rotate.z = randomInRange();
		objs[i].scale.x = randomInRange();
		objs[i].scale.y = randomInRange();
		objs[i].scale.z = randomInRange();
	}

	float timer = 0;
	
	//main run loop
	while (!glfwWindowShouldClose(window)) {
		auto t1 =std::chrono::high_resolution_clock::now();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		if (!mousePressed) {
			timer += .001;
			model = glm::mat4(1.0f);
			model = glm::rotate(model, (float)timer * glm::radians(5.0f), glm::vec3(0.5f, 1.0f, 0.0f));

		}
		//move view matrix
		/*float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));*/



		//move left/right
		const float camSpeed = .001f;
		if (up1)
			cameraPos += camSpeed*cameraFront;
		if (down)
			cameraPos -= camSpeed * cameraFront;
		if (left)
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * camSpeed;
		if (right)
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * camSpeed;

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


		projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
		//Send camera data to shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		
		
		for (int i = 0; i < objsCount; i++) {
			glUniform3fv(translateDataLoc, 1, glm::value_ptr(objs[i].translate));
			glUniform3fv(rotateDataLoc, 1, glm::value_ptr(objs[i].rotate*(float)glfwGetTime()));
			glUniform3fv(scaleDataLoc, 1, glm::value_ptr(objs[i].scale));
			//glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		
		glfwPollEvents();
		auto t2 = std::chrono::high_resolution_clock::now();
		auto ms_init = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
		//std::cout << ms_init.count() << "ms\n";
		
	}

	//Macro for making program memory safe
	clearMemory

}