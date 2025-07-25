# Development Guide

This document describes how to build and run the project on both Windows and Linux.

---

## Prerequisites

- **CMake** (version 3.10 or higher)
- **C++ Compiler**
  - Windows: MSVC or MinGW
  - Linux: GCC or Clang
- **Git** (optional, for cloning the repository)

---

## Building and Running on Windows

1. **Open a terminal (Command Prompt or PowerShell).**
2. **Navigate to the project directory:**
   ```sh
   cd path\to\cpp-code-quest
   ```
3. **Ensure the `build` directory exists and is configured with CMake.**  
   If not, run:
   ```sh
   cmake -S . -B build
   ```
4. **Build the project using the batch script:**
   ```sh
   build.bat
   ```
   - This will build all targets.
   - To build and run a specific example (e.g., `level3_smart_pointers`):
     ```sh
     build.bat level3_smart_pointers
     ```
   - For help:
     ```sh
     build.bat -h
     ```

---

## Building and Running on Linux

1. **Open a terminal.**
2. **Navigate to the project directory:**
   ```sh
   cd path/to/cpp-code-quest
   ```
3. **Ensure the `build` directory exists and is configured with CMake.**  
   If not, run:
   ```sh
   cmake -S . -B build
   ```
4. **Make the build script executable (first time only):**
   ```sh
   chmod +x build.sh
   ```
5. **Build the project using the shell script:**
   ```sh
   ./build.sh
   ```
   - This will build all targets.
   - To build and run a specific example (e.g., `level3_smart_pointers`):
     ```sh
     ./build.sh level3_smart_pointers
     ```
   - For help:
     ```sh
     ./build.sh -h
     ```

---

## Output

- Executables are generated in the `build/src/` or `build/examples/` directories.
- The scripts will attempt to run the built executable if a specific target is provided.

---

## Troubleshooting

- Ensure all dependencies are installed and available in your system's PATH.
- If you encounter build errors, check that your compiler supports C++14/17 features.