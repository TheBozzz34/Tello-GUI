#ifndef UNICODE
#define UNICODE
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>
#include <iostream>
#include <boost/asio.hpp>
#include <map>
#include <cassert>
#include <string>
#include <windows.h>
#include <wbemidl.h>
#include <comdef.h>
#include <codecvt>
#include "util.h"
#include <fstream> 

#pragma comment(lib, "wbemuuid.lib")




static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main()
{
    util utilFunction;
	/*
    std::wstring ssid = GetConnectedWifiSSID(utilFunction);
    if (!ssid.empty()) {
        wprintf(L"Connected Wi-Fi SSID: %s\n", ssid.c_str());
    }
    else {
        wprintf(L"Unable to retrieve Wi-Fi SSID information.\n");
        ssid = L"Unable to retrieve Wi-Fi SSID information.";
    }
	


    const char* ssidcstr = utilFunction.WStringToConstChar(ssid);

    std::cout << ssidcstr << std::endl;
	*/



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

	glfwSetErrorCallback(glfw_error_callback);

	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello World!", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	else
	{
		std::cout << "GLAD initilizied!" << std::endl;
	}

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls


	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);



	boost::asio::io_service io_service;
	boost::asio::ip::udp::socket socket(io_service);
	boost::asio::ip::udp::endpoint drone_endpoint;

	socket.open(boost::asio::ip::udp::v4());

	drone_endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string("192.168.10.1"), 8889);


	bool show_demo_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		{
			ImGui::Begin("Setup");

            
			ImGui::Text("Network");
			std::string ssidText;
			if (ImGui::Button("Test SSID"))
			{
				utilFunction.exec("netsh wlan show interfaces > ssid.txt");
				std::ifstream ReadFile("ssid.txt");
				while (getline(ReadFile, ssidText)) {
					// Output the text from the file
					std::cout << ssidText << std::endl;
				}
				ReadFile.close();

			}

			ImGui::Separator();

			ImGui::Text("Drone");

			if (ImGui::Button("Refresh"))
			{
				std::cout << "Refresh" << std::endl;
			}


            ImGui::End();

		}

		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();


	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;

}