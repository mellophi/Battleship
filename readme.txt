========================================
INSTRUCTIONS TO BUILD AND RUN THE SOURCE 
========================================
1. Make sure you have the updated version if build tools for vs2022.
2. Open the Visual Studio Console for vs2022 and cd to root path of the folder.
3. Create a build directory.
    $mkdir build
4. Change the directory to build.
    $cd build
5. Run CMake to generate Ninja file using the following command.
    $cmake -G Ninja ..
6. Run Ninja to compile and link the libraries.
    $ninja
7. Change the directory to run.
    $cd run
8. Run run.exe to get play the game.

N.B - 
1. Make sure there is no " " (blank space) in the path where the project is saved.
2. If you change the directory of the project: To run the project, delete the build folder, and repeat steps 3 to 8.
