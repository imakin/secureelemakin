cmake_minimum_required(VERSION 3.0)
project(SecureelemakinLinux)

# Set C++ standard to C++11 (or higher if required)
set(CMAKE_CXX_STANDARD 11)

# Specify the folder containing the source files
set(SOURCE_DIR ".")

# Retrieve the list of source files in the specified folder
file(GLOB SOURCE_FILES "${SOURCE_DIR}/*.cpp")

# Exclude the files that will be compiled independently later
list(FILTER SOURCE_FILES EXCLUDE REGEX "${SOURCE_DIR}/main.cpp$")
list(FILTER SOURCE_FILES EXCLUDE REGEX "${SOURCE_DIR}/main_api.cpp$")

# Compile all the source files into object files
add_library(AllLib OBJECT ${SOURCE_FILES})

# Execute the shell command to get pybind11 includes
execute_process(
    COMMAND python3 -m pybind11 --includes
    OUTPUT_VARIABLE PYBIND11_INCLUDES
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Remove the "-I" from the paths
string(REPLACE "-I" "" PYBIND11_INCLUDES "${PYBIND11_INCLUDES}")

# Convert the string to a list
separate_arguments(PYBIND11_INCLUDES)

# AES128.cpp can only compiled to shared object with -fPIC
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# Create the executable
add_executable(main_exe $<TARGET_OBJECTS:AllLib>)
target_sources(main_exe PRIVATE main.cpp)

# Create the shared object library
add_library(securelemakintool SHARED $<TARGET_OBJECTS:AllLib>)
target_sources(securelemakintool PRIVATE main_api.cpp)

# Add the Python include directory as an include directory for main_api
target_include_directories(securelemakintool PRIVATE /usr/include/python3.10)
# Add the pybind11 includes as a compile definition for main_api
target_include_directories(securelemakintool PRIVATE ${PYBIND11_INCLUDES})
