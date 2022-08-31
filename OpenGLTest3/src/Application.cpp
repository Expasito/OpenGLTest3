#include "Headers.h"
#include "Shaders.h"
#include "Render.h"
#include <random>


void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}



int main() {

	GLFWwindow* window = Render::init();
	Render::callBacks(window, keyCallBack, framebuffer_size_callback);

	
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	

	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	vec = trans * vec;
	trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));


	unsigned int indicies[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	float texCoords[] = {
		0.0f,0.0f,
		1.0f, 0.0f,
		.5f,1.0f
	};

	//define how opengl reads texture data
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Mipmaps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	//load textures begins here
	//stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char* data = SOIL_load_image("../OpenGLTest3/res/textures/Texture1.png", &width, &height, &nrChannels, 0);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	//load texture ends here




	//shader stuff begins here
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	
	


	Shaders::ShaderProgramSource source = Shaders::ParseShader("../OpenGLTest3/res/shaders/Shader.shader");
	unsigned int shader = Shaders::CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	//get position data to shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//get color data to shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//get texture data to shader
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);


	unsigned int translate = glGetUniformLocation(shader, "translateData");
	unsigned int rotate = glGetUniformLocation(shader, "rotate");
	unsigned int rotateValue = glGetUniformLocation(shader, "rotateValue");
	unsigned int scale = glGetUniformLocation(shader, "scaleData");
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));



	std::srand(std::time(0));

	const int items = 200000;
	const int dataPoints = 6;
	float* startValues = new float[items * dataPoints];

	for (int i = 0; i < items*dataPoints; i++) {
		startValues[i]= (float)(std::rand() % 100) / 50;
		//std::cout << startValues[i] << "\n";
		startValues[i] -= 1;
	}
	


	while (!glfwWindowShouldClose(window)) {
		std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
		glClear(GL_COLOR_BUFFER_BIT);

		//glUniform4f(vertexColorLocation, 0.0f, glfwGetTime()/2, 0.0f, 1.0f);

		
		for (int i = 0; i < items*dataPoints; i+=dataPoints) {
			trans = glm::mat4(1.0f);
			//trans = glm::translate(trans, glm::vec3(startValues[i], startValues[i+1],0));
			trans = glm::rotate(trans, 2*startValues[i+2]*(float)glfwGetTime(), glm::vec3(1.0f, 1.0, 1.0));
			//trans = glm::scale(trans, glm::vec3(startValues[i+3], startValues[i+4], startValues[i+5]));
			glUniform3f(translate, startValues[i], startValues[i+1], 0);
			glUniformMatrix4fv(rotate, 1, GL_FALSE,glm::value_ptr(trans));
			//glUniform1f(rotateValue, glfwGetTime()*2*startValues[i+2]);
			glUniform3f(scale, startValues[i + 3], startValues[i + 4], startValues[i + 5]);
			//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		

		glfwSwapBuffers(window);
		glfwPollEvents();
		std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
		std::cout << (end - start).count() << "\n";
	}
	delete startValues;
	glfwDestroyWindow(window);
	glfwTerminate();
	//delete window;

}