# MegaManX Clone
A clone for the original Mega Man X. This project is made using only structured programming principles and is intended to serve as a university's group project.

**DISCLAIMER: This project was developed as part of a university's course group assignment. Certain design decisions, including the methodology and file structure, were determined by course requirements and constraints rather than by our preferred practices.** 

## Quick start

### Command line

1. Install [Git](https://git-scm.com/downloads) and [CMake](https://cmake.org/download/). Use your system's package manager if available.
2. Follow [GitHub's instructions](https://docs.github.com/en/repositories/creating-and-managing-repositories/creating-a-repository-from-a-template) for how to use their project template feature to create your own project. If you don't want to use GitHub, see the section below.
3. Clone your new GitHub repo and open the repo in your text editor of choice.
4. Open [CMakeLists.txt](CMakeLists.txt). Rename the project and the target name of the executable to whatever name you want. Make sure to change all occurrences.
5. If you want to add or remove any .cpp files, change the source files listed in the `add_executable` call in CMakeLists.txt to match the source files your project requires. If you plan on keeping the default main.cpp file then no changes are required.
6. If your code uses the Audio or Network modules then add `SFML::Audio` or `SFML::Network` to the `target_link_libraries` call alongside the existing `SFML::Graphics` library that is being linked.
7. If you use Linux, install SFML's dependencies using your system package manager. On Ubuntu and other Debian-based distributions you can use the following commands:
   ```
   sudo apt update
   sudo apt install \
       libxrandr-dev \
       libxcursor-dev \
       libxi-dev \
       libudev-dev \
       libfreetype-dev \
       libflac-dev \
       libvorbis-dev \
       libgl1-mesa-dev \
       libegl1-mesa-dev \
       libfreetype-dev
   ```
8. Configure and build your project. Most popular IDEs support CMake projects with very little effort on your part.

   - [VS Code](https://code.visualstudio.com) via the [CMake extension](https://code.visualstudio.com/docs/cpp/cmake-linux)
   - [Visual Studio](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170)
   - [CLion](https://www.jetbrains.com/clion/features/cmake-support.html)
   - [Qt Creator](https://doc.qt.io/qtcreator/creator-project-cmake.html)

   Using CMake from the command line is straightforward as well.
   Be sure to run these commands in the root directory of the project you just created.

   ```
   cmake -B build
   cmake --build build
   ```

## Portability checklist

1. Build in an out-of-source directory:
   ```
   mkdir -p build
   cmake -B build -S .
   cmake --build build --config Release
   ```
2. Keep generated files out of Git (already in `.gitignore`: `build/`, `.vs/`, `.vscode/`).
3.  Avoid absolute paths in code/config; use relative paths for assets (e.g. `fonts/`, `assets/`).
4.  SFML dependency is fetched via CMake `FetchContent`, so other machines get it automatically.
5.  On Windows use the build config in your IDE; on Linux/macOS use `-DCMAKE_BUILD_TYPE=Debug/Release`.

9. Enjoy!

### Visual Studio

Using a Visual Studio workspace is the simplest way to get started on windows.

1. Ensure you have the [required components installed](https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio#installation).
2. Follow [GitHub's instructions](https://docs.github.com/en/repositories/creating-and-managing-repositories/creating-a-repository-from-a-template) for how to use their project template feature to create your own project.
3. If you have already cloned this repo, you can [open the folder](https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio0#ide-integration).
4. If not, you can [clone it directly in Visual Studio](https://learn.microsoft.com/en-us/visualstudio/get-started/tutorial-open-project-from-repo).

Visual Studio should automatically configure the CMake project, then you can build and run as normal through Visual Studio. See the links above for more details.

## Upgrading SFML

SFML is found via CMake's [FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html) module.
FetchContent automatically downloads SFML from GitHub and builds it alongside your own code.
Beyond the convenience of not having to install SFML yourself, this ensures ABI compatibility and simplifies things like specifying static versus shared libraries.

Modifying what version of SFML you want is as easy as changing the `GIT_TAG` argument.
Currently it uses SFML 3 via the `3.0.2` tag.

## License

The source code is dual licensed under Public Domain and MIT -- choose whichever you prefer.
