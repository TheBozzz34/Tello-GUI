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
#include "tello.h"
#include <thread>

#pragma comment(lib, "wbemuuid.lib")



static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main()
{
    util utilFunction;
	tello drone("192.168.10.1", 8889);
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



	bool show_demo_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	const char* connectedDefaultText = "false";
	bool isConnected = false;


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

			ImGui::Text("Connected: %s", isConnected ? "true" : "false");

	

			if (ImGui::Button("Refresh"))
			{
				std::cout << "Refresh" << std::endl;
				drone.testConnection();

				isConnected = drone.isConnected();
			}

			ImGui::Separator();

			{
				ImGui::Text("Commands");

				if (ImGui::Button("Takeoff"))
				{
					std::cout << "Takeoff" << std::endl;
					drone.takeoff();
				}
			}

			ImGui::Separator();

			{
				ImGui::Text("Speed");
				// ImGui::SliderInt("Speed", &drone.speed, 10, 100); Not implemented yet
			}

			ImGui::Separator();

			{
				ImGui::Text("Battery");
				ImGui::ProgressBar(0.5f, ImVec2(0.0f, 0.0f));
			}

			ImGui::Separator();

			{
				ImGui::Text("Flight Data");
				ImGui::Columns(2, nullptr, false);
				ImGui::Text("SN: %s", drone.getSN().c_str());
				ImGui::Text("Speed: %s", drone.getSpeed().c_str());
				ImGui::Text("Battery: %s", drone.getBattery().c_str());
				ImGui::Text("Time: %s", drone.getTime().c_str());
				ImGui::Text("Wifi: %s", drone.getWifi().c_str());
				ImGui::NextColumn();
				/* Not implemented yet
				ImGui::Text("Height: %s", drone.getHeight().c_str());
				ImGui::Text("Temp Low: %s", drone.getTempLow().c_str());
				ImGui::Text("Temp High: %s", drone.getTempHigh().c_str());
				ImGui::Text("Attitude: %s", drone.getAttitude().c_str());
				ImGui::Text("Baro: %s", drone.getBaro().c_str());
				ImGui::Text("Acceleration: %s", drone.getAcceleration().c_str());
				ImGui::Text("TOF: %s", drone.getTOF().c_str());
				ImGui::Text("Flight Time: %s", drone.getFlightTime().c_str());
				ImGui::Text("WIFI Disturb: %s", drone.getWIFIDisturb().c_str());
				ImGui::Text("Drone Fly Time Left: %s", drone.getDroneFlyTimeLeft().c_str());
				ImGui::Text("Drone Battery Left: %s", drone.getDroneBatteryLeft().c_str());
				ImGui::Text("Drone Control: %s", drone.getDroneControl().c_str());
				ImGui::Text("Drone State: %s", drone.getDroneState().c_str());
				ImGui::Text("Drone Version: %s", drone.getDroneVersion().c_str());
				ImGui::Text("Drone Serial Number: %s", drone.getDroneSerialNumber().c_str());
				ImGui::Text("Drone Wifi Strength: %s", drone.getDroneWifiStrength().c_str());
				ImGui::Text("Drone SDK Version: %s", drone.getDroneSDKVersion().c_str());
				ImGui::Text("Drone Serial Number: %s", drone.getDroneSerialNumber().c_str());
				ImGui::Text("Drone Serial")
				*/
			}

			{
				ImGui::BeginChild("ArrowButtons", { 70, 70 });
				ImGui::Columns(3, nullptr, false);
				ImGui::PushButtonRepeat(true);
				for (int i = 0; i < 9; i++)
				{
					switch (i)
					{
					case 1:
						if (ImGui::ArrowButton("##Up", ImGuiDir_Up)) {}
						break;
					case 3:
						if (ImGui::ArrowButton("##Left", ImGuiDir_Left)) {}
						break;
					case 4:
						if (ImGui::Button("C")) {}
						break;
					case 5:
						if (ImGui::ArrowButton("##Right", ImGuiDir_Right)) {}
						break;
					case 7:
						if (ImGui::ArrowButton("##Down", ImGuiDir_Down)) {}
						break;
					}
					ImGui::NextColumn();
				}
				ImGui::PopButtonRepeat();
				ImGui::EndChild();
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