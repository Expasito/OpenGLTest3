#include "Headers.h"
#include "Shaders.h"
#include "Render.h"
#include "Object.h"
#include <random>
#include "Camera.h"
#include "../Depend/ImGui/imgui_impl_opengl3.h"


#define clearMemory glfwDestroyWindow(window); glfwTerminate();

//Create Camera
Camera camera(0, 0, -10);

//Create model, view, projection matrices
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection;

static bool mousePressed=false;

static bool left = false, right = false, forward = false, backward = false, up=false, down=false;


void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	left = glfwGetKey(window, GLFW_KEY_A);
	right = glfwGetKey(window, GLFW_KEY_D);
	forward = glfwGetKey(window, GLFW_KEY_W);
	backward = glfwGetKey(window, GLFW_KEY_S);
	up = glfwGetKey(window, GLFW_KEY_SPACE);
	down = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
	

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
	camera.zoom(xoffset, yoffset);

}

void mouseCallBack(GLFWwindow* window, double xpos, double ypos) {
	camera.rotate(xpos, ypos);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


float randomInRange() {
	return (float)((float)std::rand() / (float)RAND_MAX) * 2 - 1;
}



int main() {

	//Create window and set key callback functions
	GLFWwindow* window = Render::init();
	Render::callBacks(window, keyCallBack, framebuffer_size_callback, mouseButtonCallBack, scrollCallBack, mouseCallBack);
	
	//Set random generator
	std::srand(std::time(0));



	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);


	//define how the shaders read texture inputs
	Shaders::defineTextureInputs();


	//load textures begins here
	unsigned int texture0 = Shaders::loadTexture("../OpenGLTest3/res/textures/Default.png");
	unsigned int texture1 = Shaders::loadTexture("../OpenGLTest3/res/textures/Texture1.png");
	unsigned int texture2 = Shaders::loadTexture("../OpenGLTest3/res/textures/Texture2.png");
	unsigned int texture3 = Shaders::loadTexture("../OpenGLTest3/res/textures/Deor.png");
	unsigned int texture4 = Shaders::loadTexture("../OpenGLTest3/res/textures/skybox2.png");

	


	Shaders::ShaderProgramSource source = Shaders::ParseShader("../OpenGLTest3/res/shaders/Shader.shader");
	Shaders::CompileShaderStatus shaderRet = Shaders::CreateShader(source.VertexSource, source.FragmentSource);
	//Exit program if shader error
	if (shaderRet.error) {
		clearMemory
			exit(1);
	}

	unsigned int shader = shaderRet.id;
	glUseProgram(shader);


	Render::initEntities();


	//Get uniform locations
	unsigned int translateDataLoc = glGetUniformLocation(shader, "translateData");
	unsigned int rotateDataLoc = glGetUniformLocation(shader, "rotateData");
	unsigned int scaleDataLoc = glGetUniformLocation(shader, "scaleData");

	Render::uniforms.push_back({translateDataLoc,"translateData"});
	Render::uniforms.push_back({rotateDataLoc,"rotateData"});
	Render::uniforms.push_back({scaleDataLoc,"scaleData"});

	//Get uniforms for camera data
	unsigned int modelLoc = glGetUniformLocation(shader, "model");
	unsigned int viewLoc = glGetUniformLocation(shader, "view");
	unsigned int projectionLoc = glGetUniformLocation(shader, "projection");

	Render::uniforms.push_back({modelLoc, "model"});
	Render::uniforms.push_back({viewLoc, "view"});
	Render::uniforms.push_back({projectionLoc, "projection"});



	float timer = 0;
	
	Entity e;
	e.addComponent<TransformComponent>(glm::vec3(1,1,1));
	e.addComponent<TextureComponent>(texture0);

	Entity e2;
	e2.addComponent<TransformComponent>()->translate = glm::vec3(4, 4, 4);
	e2.addComponent<TextureComponent>()->texture = texture2;

	Render::activateSkybox();

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	//main run loop
	while (!glfwWindowShouldClose(window)) {
		auto t1 =std::chrono::high_resolution_clock::now();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Render::draw(Render::skybox);

		//if (!mousePressed) {
		//	timer += .001;
		//	model = glm::mat4(1.0f);
		//	model = glm::rotate(model, (float)timer * glm::radians(5.0f), glm::vec3(0.5f, 1.0f, 0.0f));

		//}
		//ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		//Adjust camera position
		camera.translate(left, right, up, down,forward,backward);

		view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);


		projection = glm::perspective(glm::radians(camera.fov), 800.0f / 600.0f, 0.01f, 1000.0f);


		//Send camera data to shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		e.getComponent<TransformComponent>()->rotate = glm::vec3(glfwGetTime(), glfwGetTime(), glfwGetTime());
		Render::draw(&e);
		Render::draw(&e2);


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		
		glfwPollEvents();
		auto t2 = std::chrono::high_resolution_clock::now();
		auto ms_init = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
		std::cout << ms_init.count() << "ms\n";
		
	}

	//Macro for making program memory safe
	clearMemory;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

}