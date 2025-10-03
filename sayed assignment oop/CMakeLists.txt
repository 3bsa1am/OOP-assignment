cmake_minimum_required(VERSION 3.10)
project(assignment)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# If you installed OpenCV via a package manager (e.g., vcpkg) or installer
# you can optionally set OpenCV_DIR to the folder that contains OpenCVConfig.cmake.
# Example:
# set(OpenCV_DIR "C:/libs/opencv")

find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME}
    assign.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})

# Convenience target: cmake --build . --target run
add_custom_target(run
    COMMAND ${CMAKE_COMMAND} -E env "PATH=C:/msys64/mingw64/bin;$ENV{PATH}" $<TARGET_FILE:assignment>
    DEPENDS assignment
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
