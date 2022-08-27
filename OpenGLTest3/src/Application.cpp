#include "Headers.h"
#include "Shaders.h"
#include "Render.h"


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

	
	float verticies[] = {
		-0.5f,-.5f, 0.0f, 1.0f,0.0f,0.0f,
		0.5f,-.5f,0.0f, 0.0f,1.0f,0.0f,
		0.0f,.5f,0.0f, 0.0f,0.0f,1.0f,
		1.0f,.75f,-.25f,1.0f,1.0f,1.0f
	};
	

	float verticies2[] = {
		-0.75f,-.75f, 0.0f, 1.0f,0.0f,0.0f,
		0.75f,-.75f,0.0f, 0.0f,1.0f,0.0f,
		0.0f,.75f,0.0f, 0.0f,0.0f,1.0f,
		1.0f,.75f,-.75f,1.0f,1.0f,1.0f
	};

	unsigned int indicies[] = {
		0,1,3,2,3,1
	};

	unsigned int indicies2[] = {
		0,3,2
	};

	//shader stuff begins here
	unsigned int VBO,VBO2;
	unsigned int VAO;
	unsigned int EBO,EBO2;
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &EBO2);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1,&VBO);
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies2), verticies2, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies2), indicies2, GL_STATIC_DRAW);

	
	


	Shaders::ShaderProgramSource source = Shaders::ParseShader("../OpenGLTest3/res/shaders/Shader.shader");
	unsigned int shader = Shaders::CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	//get position data to shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//get color data to shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	//uniforms here
	int vertexColorLocation = glGetUniformLocation(shader, "color");
	//glUniform4f(vertexColorLocation, 0.0f, 1.0f, 0.0f, 1.0f);



	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		//glUniform4f(vertexColorLocation, 0.0f, glfwGetTime()/2, 0.0f, 1.0f);

		//get position data to shader
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//get color data to shader
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glDrawElements(GL_TRIANGLES, 3,GL_UNSIGNED_INT,0);


		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//get color data to shader
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	//delete window;

}