#ifndef UNICODE
#define UNICODE
#endif

#ifndef NDEBUG
// Production builds should set NDEBUG=1
#define NDEBUG false
#else
#define NDEBUG true
#endif

#ifndef DEBUG
#define DEBUG !NDEBUG
#endif


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

#include <iostream>
#include <map>
#include <cassert>
#include <string>
#include <thread>
#include <fstream> 
#include <codecvt>
#include <ctime>

#include <boost/asio.hpp>

#include <Windows.h>
#include <shlobj.h>
#include <wbemidl.h>
#include <comdef.h>
#include <shellapi.h>

#include "util.h"
#include "tello.h"

#include <sentry.h>


#pragma comment(lib, "wbemuuid.lib")


void ExportBufferToFile(const char* buffer, const char* path)
{
	std::ofstream file;
	file.open(path);
	file << buffer;
	file.close();

	spdlog::info("Exported buffer to {}", path);
}


static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main()
{
    util utilFunction;
	tello drone("192.168.10.1", 8889);

	if (DEBUG)
	{
		spdlog::debug("Debug mode enabled!");
		sentry_options_t* options = sentry_options_new();
		sentry_options_set_dsn(options, "https://d08305e3156f47698859d549119368ce@o561860.ingest.sentry.io/4505859179937792");
		sentry_options_set_database_path(options, ".sentry-native");
		sentry_options_set_release(options, "tello-gui@0.0.2.3");
		sentry_options_set_debug(options, 1);
		sentry_init(options);
		spdlog::apply_all([&](std::shared_ptr<spdlog::logger> l) {l->set_level(spdlog::level::debug); });

	} 
	else
	{
		spdlog::info("Debug mode disabled!");
		sentry_options_t* options = sentry_options_new();
		sentry_options_set_dsn(options, "https://d08305e3156f47698859d549119368ce@o561860.ingest.sentry.io/4505859179937792");
		sentry_options_set_database_path(options, ".sentry-native");
		sentry_options_set_release(options, "tello-gui@0.0.2.3");
		sentry_options_set_debug(options, 0);
		sentry_init(options);
		spdlog::apply_all([&](std::shared_ptr<spdlog::logger> l) {l->set_level(spdlog::level::info); });
	}

	const char* app_version = "0.0.3.0";

	spdlog::info("Initilizing GLFW!");

	if (!glfwInit())
	{
		spdlog::critical("Unable to initialize GLFW!");
		return -1;
	}
	else
	{
		spdlog::info("GLFW initilizied!");
	}

	glfwSetErrorCallback(glfw_error_callback);

	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Tello GUI!", nullptr, nullptr);
	if (window == nullptr)
	{
		spdlog::critical("Unable to create GLFW window!");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		spdlog::critical("Unable to initialize GLAD!");
		return -1;
	}
	else
	{
		spdlog::info("GLAD initilizied!");
	}

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigWindowsMoveFromTitleBarOnly = true;


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

	static char inputBuffer[256] = "";


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
				spdlog::info("Refreshing drone connection!");

				utilFunction.AppendToBuffer("Refreshing drone connection!", inputBuffer);

				drone.testConnection();

				isConnected = drone.isConnected();
			}

			ImGui::Separator();

			{
				ImGui::Text("Commands");

				if (ImGui::Button("Takeoff"))
				{
					spdlog::info("Taking off!");
					utilFunction.AppendToBuffer("Taking off!", inputBuffer);
					drone.takeoff([&](int response) {
						if (response == 0)
						{
							utilFunction.AppendToBuffer("Error taking off!", inputBuffer);
						}
					});
				}

				if (ImGui::Button("Land"))
				{
					spdlog::info("Landing!");
					utilFunction.AppendToBuffer("Landing!", inputBuffer);
					drone.land([&](int response) {
						if (response == 0)
						{
							utilFunction.AppendToBuffer("Error landing!", inputBuffer);
						}
						});
				}

				if (ImGui::Button("Emergency"))
				{
					spdlog::info("Emergency!");
					utilFunction.AppendToBuffer("Emergency!", inputBuffer);
					drone.emergency([&](int response) {
						if (response == 0)
						{
							utilFunction.AppendToBuffer("Error sending Emergency!", inputBuffer);
						}
						});
				}

				if (ImGui::Button("Stop"))
				{
					spdlog::info("Stopping!");
					utilFunction.AppendToBuffer("Stopping!", inputBuffer);
					drone.stop([&](int response) {
						if (response == 0)
						{
							utilFunction.AppendToBuffer("Error stopping!", inputBuffer);
						}
						});
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
						if (ImGui::ArrowButton("##Up", ImGuiDir_Up)) 
						{
							utilFunction.AppendToBuffer("Moving up!", inputBuffer);
							drone.up(20, [&](int response) {
								if (response == 0)
								{
									utilFunction.AppendToBuffer("Error moving up!", inputBuffer);
								}
								});
						}
						break;
					case 3:
						if (ImGui::ArrowButton("##Left", ImGuiDir_Left)) 
						{
							utilFunction.AppendToBuffer("Moving left!", inputBuffer);
							drone.left(20, [&](int response) {
								if (response == 0)
								{
									utilFunction.AppendToBuffer("Error moving left!", inputBuffer);
								}
								});
						}
						break;
					case 4:
						if (ImGui::Button("C")) {}
						break;
					case 5:
						if (ImGui::ArrowButton("##Right", ImGuiDir_Right)) 
						{
							utilFunction.AppendToBuffer("Moving right!", inputBuffer);
							drone.right(20, [&](int response) {
								if (response == 0)
								{
									utilFunction.AppendToBuffer("Error moving right!", inputBuffer);
								}
								});
						}
						break;
					case 7:
						if (ImGui::ArrowButton("##Down", ImGuiDir_Down)) 
						{
							utilFunction.AppendToBuffer("Moving down!", inputBuffer);
							drone.down(20, [&](int response) {
								if (response == 0)
								{
									utilFunction.AppendToBuffer("Error moving down!", inputBuffer);
								}
								});
						}
						break;
					}
					ImGui::NextColumn();
				}
				ImGui::PopButtonRepeat();
				ImGui::EndChild();

				ImGui::BeginChild("RotateButtons", { 210, 70 }); // Increase the width to accommodate three buttons with spacing
				ImGui::Columns(3, nullptr, false);
				ImGui::PushButtonRepeat(true);

				for (int i = 0; i < 9; i++)
				{
					switch (i)
					{
					case 1:
						if (ImGui::ArrowButton("##CW", ImGuiDir_Right))
						{
							utilFunction.AppendToBuffer("Moving cw!", inputBuffer);
							drone.cw(20, [&](int response) {
								if (response == 0)
								{
									utilFunction.AppendToBuffer("Error Moving cw!", inputBuffer);
								}
								});
						}
						break;
					case 3:
						if (ImGui::ArrowButton("##CCW", ImGuiDir_Left))
						{
							utilFunction.AppendToBuffer("Moving ccw!", inputBuffer);
							drone.ccw(20, [&](int response) {
								if (response == 0)
								{
									utilFunction.AppendToBuffer("Error moving ccw!", inputBuffer);
								}
								});
						}
						break;
					}

					if (i < 8)
					{
						ImGui::SameLine(); // Add this to keep the buttons on the same line with spacing
					}

					ImGui::NextColumn();
				}

				ImGui::PopButtonRepeat();
				ImGui::EndChild();
			}



			ImGui::End();

		}

		{
			ImGui::Begin("Console");


			ImGui::SetWindowCollapsed(true, ImGuiCond_FirstUseEver);
			ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);


			ImGui::Text("Console");

			ImGui::SameLine();
			if (ImGui::Button("Clear"))
			{
				inputBuffer[0] = '\0';
			}
			ImGui::SameLine();
			if (ImGui::Button("Export"))
			{
				
				BROWSEINFO bi = { 0 };
				char selectedPath[MAX_PATH] = { 0 };

				bi.hwndOwner = NULL; 
				bi.pidlRoot = NULL;  
				bi.pszDisplayName = (LPWSTR)selectedPath;
				bi.lpszTitle = (LPWSTR)"Select a directory";  // Dialog title
				bi.ulFlags = BIF_RETURNONLYFSDIRS; 

				LPITEMIDLIST pidl = SHBrowseForFolderA((LPBROWSEINFOA) & bi);

				if (pidl != NULL) {
					// Convert the selected PIDL to a file system path
					if (SHGetPathFromIDListA(pidl, selectedPath)) {
						std::string path = selectedPath;
						path += "\\console.txt";
						ExportBufferToFile(inputBuffer, path.c_str());
						std::string message = "Exported buffer to " + path;
						utilFunction.AppendToBuffer(message.c_str(), inputBuffer);
					}
					// Free the PIDL
					IMalloc* pMalloc;
					if (SUCCEEDED(SHGetMalloc(&pMalloc))) {
						pMalloc->Free(pidl);
						pMalloc->Release();
					}
				}
				//ExportBufferToFile(inputBuffer, "console.txt");
			}

			ImGui::Separator();

			ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
			ImGui::TextUnformatted(inputBuffer);
			ImGui::PopStyleVar();
			ImGui::EndChild();



			ImGui::End();
		}

		{
			ImGui::Begin("Settings");

			ImGui::SetWindowPos(
				ImVec2(0, 0),
				true);


			ImGui::SetWindowCollapsed(true, ImGuiCond_FirstUseEver);
			ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);

			ImGui::Text("Settings");
			ImGui::Separator();

			ImGui::Text("Drone IP");
			// ImGui::InputText("##DroneIP", &drone.ip); // TODO: Fix this

			ImGui::Text("Drone Port");
			// ImGui::InputInt("##DronePort", &drone.port); // TODO: Fix this

			ImGui::Text("Show Demo Window");
			ImGui::Checkbox("##ShowDemoWindow", &show_demo_window);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

			if (DEBUG)
			{
				ImGui::Text("Debug mode enabled at compile time");
				if (ImGui::Button("Send test Sentry message"))
				{
					sentry_capture_event(sentry_value_new_message_event(
						/*   level */ SENTRY_LEVEL_INFO,
						/*  logger */ "custom",
						/* message */ "It works!"
					));
				}
			} 
			else
			{
				ImGui::Text("Debug mode disabled at compile time");
			}


			ImGui::End();
		}

		{
			ImGui::Begin("About");
			ImGui::SetWindowCollapsed(true, ImGuiCond_FirstUseEver);
			ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);

			ImGui::Text("Tello Control");
			ImGui::Text("Version: %s", app_version);
			ImGui::Text("By: Necrozma");
			if (ImGui::Button("GitHub"))
			{
				ShellExecute(0, 0, L"http://github.com/thebozzz34/Tello-GUI", 0, 0, SW_SHOW);
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

	// Cleanup ImGui resources
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Cleanup GLFW resources
	glfwDestroyWindow(window);
	glfwTerminate();

	sentry_close();

	return 0;

}

