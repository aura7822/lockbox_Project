cmake_minimum_required(VERSION 3.10)
project(lockbox LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Put binary into build/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include dirs (project include/ and src/ are common)
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)

# Sources
set(SRC_FILES
    src/main.cpp
    src/encryptor.cpp
    src/obfuscator.cpp
    src/imgutils.cpp
    src/platform/pwd_unix.cpp
)

# Find OpenCV (dynamic)
find_package(OpenCV REQUIRED)

# Find libsodium (dynamic)
find_library(SODIUM_LIB sodium)

if(NOT SODIUM_LIB)
    message(FATAL_ERROR "libsodium not found. Install libsodium (libsodium-devel) or set SODIUM_LIB manually.")
endif()

# Create executable
add_executable(lockbox ${SRC_FILES})

# Include OpenCV headers for the target
target_include_directories(lockbox PRIVATE ${OpenCV_INCLUDE_DIRS})

# Link libraries (dynamic)
target_link_libraries(lockbox PRIVATE ${OpenCV_LIBS} ${SODIUM_LIB})

# Helpful config-time messages
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "OpenCV include dirs: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libs: ${OpenCV_LIBS}")
message(STATUS "Using libsodium: ${SODIUM_LIB}")
message(STATUS "Output binary: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lockbox")


