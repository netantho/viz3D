# Set project name
project (viz3D)

# Set minimum version
cmake_minimum_required (VERSION 2.8.6)
if (COMMAND cmake_policy)
cmake_policy (SET CMP0003 NEW)
endif ()

# Set CMake modules search path
set (CMAKE_MODULE_PATH $ENV{URHO3D_HOME}/Source/CMake/Modules CACHE PATH "Path to Urho3D-specific CMake modules")

# Include Urho3D Cmake common module
include (Urho3D-CMake-common)

# Find Urho3D library
find_package (Urho3D REQUIRED)
include_directories (${URHO3D_INCLUDE_DIRS})

# Include Samples folder for Sample.h
include_directories ($ENV{URHO3D_HOME}/Source/Samples)

# Define target name
set (TARGET_NAME Viz3D)

# Define source files
define_source_files ()

# Setup target with resource copying
setup_main_executable ()
