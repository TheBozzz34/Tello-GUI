#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


int main() 
{
	std::cout << "Initilizing GLFW!" << std::endl;

	if (!glfwInit())
	{
		std::cout << "Unable to init glfw!" << std::endl;
		return -1;
	}
	else
	{
		std::cout << "GLFW initilizied!" << std::endl;
	}

	glfwSetErrorCallback([](int error, const char* description) {
		std::cerr << "GLFW Error " << error << ": " << description << std::endl;
	});


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello World!", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window)) 
	{
		glfwPollEvents();
	}



	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;

}