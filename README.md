# DJI Tello Drone Control GUI

![Tello Drone](https://m.media-amazon.com/images/I/61mZiDc5wPL.jpg)

This C++ GUI project is designed to control a DJI Tello drone using OpenGL for the backend rendering, Dear ImGui for the graphical user interface (GUI), and Boost.Asio for UDP communication with the drone. With this application, you can easily control your Tello drone and monitor its status through a user-friendly interface.

## Prerequisites

Before you begin, ensure you have met the following requirements:

- C++ development environment (C++11 or higher)
- Boost C++ Libraries (Boost.Asio)
- OpenGL library
- Dear ImGui library

## Getting Started

Follow these steps to get started with the project:

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/your-username/tello-drone-control.git
   cd tello-drone-control
   ```

2. **Build the Project:**

    Use your preferred C++ build system or IDE to compile the project, ensuring that you link the required libraries (Boost.Asio, OpenGL, and Dear ImGui).

3. **Connect to the Tello Drone:**
        Power on your DJI Tello drone and connect your computer to its Wi-Fi network.
        Ensure that your computer can communicate with the drone via UDP. The default Tello IP address is 192.168.10.1 on port 8889.

4. **Run the Application:**

    Execute the compiled binary to launch the control GUI.

5. **Use the GUI:**
        The GUI provides various controls for taking off, landing, maneuvering the drone, adjusting camera settings, and monitoring battery and status information.
        Follow the on-screen instructions to control the drone.

Planned Features:

    Real-time Video Streaming: View live video feed from the Tello drone camera within the GUI.
    Easy Control: Use intuitive controls to take off, land, move the drone, and adjust camera settings.
    Status Monitoring: Monitor the drone's battery level, flight time, and other essential parameters.
    UDP Communication: Utilizes Boost.Asio for low-level UDP communication with the drone.

Contributing

If you would like to contribute to this project, please fork the repository and submit pull requests with your improvements.

## License

This project is licensed under the Apache License 2.0. See the LICENSE file for details.

## Acknowledgments

    DJI Tello for providing an affordable and accessible drone platform.
    The developers of Boost.Asio, OpenGL, and Dear ImGui for their excellent libraries.

Contact

If you have any questions or suggestions, please feel free to contact the project maintainer:

    Necrozma
    necrozma@catgirlsaresexy.org
