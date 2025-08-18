# Visualization-Framework

This project is an OpenGL-based visualization framework developed in C++. It uses FreeGLUT for windowing and input handling, and GLEW 1.13.0 for managing OpenGL extensions.
## Result
<img width="1920" height="1025" alt="image" src="https://github.com/user-attachments/assets/6a748dd2-7094-442b-8027-531a287741f5" />

## Table of Contents
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Building the Project](#building-the-project)
- [Usage](#usage)
- [License](#license)

## Prerequisites

Before building and running the project, ensure you have the following dependencies installed:

- **C++ Compiler** (with C++17 or higher standard support)
- **FreeGLUT** (for window management and input handling)
- **GLEW 1.13.0** (for OpenGL extension loading)
- **CMake** (optional but recommended for building)

### Windows
You can install the dependencies via `vcpkg`:

```bash
vcpkg install freeglut glew:x64-windows
