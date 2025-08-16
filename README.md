# Nazi Mod PC
Cool ass Pixel Gun 3D pasteware! 🔥🔥 <br />
Just in keep in mind the codebase is a literal shithole. I don't really know c++ at the time I'm creating this.

Hopefully, this will be useful for others who wants to learn Unity Modding.

## Working with the project
### Building:
- Get [Visual Studio](https://visualstudio.microsoft.com/downloads/) 
- Select the following components when setting up Visual Studio installation:
    - CMake
    - MSVC v143 or newer
    - Windows 10 SDK (10.x.x) or newer
- Clone and open this project.
- Select the build Configuration to **x64-Release**
- To build the project expand **Build** Tab and click Build All, or simply press **CTRL + SHIFT + B**. <br />

Build binaries will be generated in **out/build/{Build Configuration}**

### Debugging:
- Select **nazimod-loader.exe** as startup item and Build Configuration to **x64-Debug**.
- Start the project without debugging or hit up **CTRL + F5**.
- Once Just-in-time debugger window appears, select the current Visual Studio workspace or whatever you wanna call it.


## Updating pattern to keep up with the game update
*TODO Guide*

## Licensing
This project is licensed under the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.en.html). 

If you use **ANY** code from the source:
- You must disclose the source code of your modified work and the source code you took from this project. This means you are not allowed to use code from this project (even partially) in a closed-source and/or obfuscated application.
- You must state clearly and obviously to all end users that you are using code from this project.
- Your application must also be licensed under the same license.