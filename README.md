# DJI Tello Drone Control GUI

### This project is in the early stages of development, so expect bugs and missing features.

## At this time, only Windows is supported.

![Tello Drone](https://m.media-amazon.com/images/I/61mZiDc5wPL.jpg)

This C++ GUI project is designed to control a DJI Tello drone using [OpenGL](https://www.opengl.org/) for graphics, [Dear ImGui](https://github.com/ocornut/imgui) for the user interface, and [Boost.Asio](https://www.boost.org/doc/libs/1_78_0/doc/html/boost_asio.html) for UDP communication with the drone. [spdlog](https://github.com/gabime/spdlog) is used for logging. Additionally, [Sentry.io](https://sentry.io/welcome/) is used to capture exceptions and crashes. With this application, you can easily control your Tello drone and monitor its status through a user-friendly interface.

Communication based off the [Tello SDK](https://dl-cdn.ryzerobotics.com/downloads/Tello/Tello%20SDK%202.0%20User%20Guide.pdf). The app is tested with this janky drone "emulator" [tello_emu.py](https://necrozma.xyz/tello_emu.py)

---

## Working commands

| Command   | Description                          | 
|-----------|--------------------------------------|
| takeoff   | Auto takeoff.                        |  
| land      | Auto landing.                        |   
| emergency | Stop motors immediately.             |   
| up x      | Ascend to “x” cm.                    |   
| down x    | down “x” Descend to “x” cm.          |   
| left x    | Fly left for “x” cm                  |  
| right x   | Fly right for “x” cm                 |   
| forward x | Fly forward for “x” cm               |  
| back x    | Fly back for “x” cm                  |  
| cw x      | Rotate “x” degrees clockwise.        |  
| ccw x     | Rotate “x” degrees counterclockwise. |   

## Other features

- UDP Communication: Utilizes Boost.Asio for low-level UDP communication with the drone.
- Crash management: Sentry.io used to log crashes and exceptions with the help of Google Crashpad

### Signing

All my code in the form of commits, releases, etc. etc. is digitally signed with my personal GPG key [F2FE38E55D832A82](https://keys.openpgp.org/vks/v1/by-fingerprint/0951795ECB2C6DFD86DF2F7BF2FE38E55D832A82). 

## Usage:

- Download the latest [release](https://github.com/TheBozzz34/Tello-GUI/releases)
- Connect to your drone's wifi network
- Run `Tello GUI.exe`
- ### IMPORTANT! In order to use this you must first click the `refresh` button after connecting to the wifi network, this puts the drone in command mode.




## Prerequisites for building

Before you begin, ensure you have met the following requirements:

- C++ development environment (C++11 or higher)
- Boost C++ Libraries (Boost.Asio)
- OpenGL library
- Dear ImGui library
- spdlog library
- Sentry.io Native SDK
- Google Crashpad, in the form of `crashpad_handler.exe`
    - [Build Instructions](https://chromium.googlesource.com/crashpad/crashpad/+/HEAD/doc/developing.md)
    - [My prebuilt binaries](https://necrozma.xyz/pub/Default.zip) 

---

[![demo1](https://r2.e-z.host/66429241-79bf-4da7-b4b6-33cb201c59b4/z692sv2x.png)](https://botnet.monster/z692sv2x.png)

[![demo2](https://r2.e-z.host/66429241-79bf-4da7-b4b6-33cb201c59b4/14srricn.png)](https://botnet.monster/14srricn.png)
---

## Getting Started for development

Follow these steps to get started with the project:

1. **Clone the Repository:**

   ```bash
    $ git clone https://github.com/TheBozzz34/Tello-GUI.git
    $ cd Tello-GUI
   ```

2. **Build the Project:**

    Use your preferred C++ build system or IDE to compile the project, ensuring that you link the required libraries (Boost.Asio, OpenGL, Dear ImGui and spdlog).

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

Contributing

If you would like to contribute to this project, please fork the repository and submit pull requests with your improvements.

## License

This project is licensed under the Apache License 2.0. See the LICENSE file for details.

## Acknowledgments

    DJI Tello for providing an affordable and accessible drone platform.
    The developers of Boost.Asio, OpenGL, Dear ImGui, Google, Sentry.io, and spdlog for their excellent libraries.

Contact

If you have any questions or suggestions, please feel free to contact the project maintainer:

    Necrozma
    necrozma@catgirlsaresexy.org
