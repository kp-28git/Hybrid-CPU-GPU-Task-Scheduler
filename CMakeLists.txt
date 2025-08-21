cmake_minimum_required(VERSION 3.10)
project(HybridSchedulerCPU LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OpenCV_DIR "C:/OpenCV/build_mingw/install")

find_package(OpenCL REQUIRED)
find_package(OpenCV REQUIRED)

# Include directories
include_directories(include)                
include_directories(${OpenCL_INCLUDE_DIRS}) 
include_directories(${OpenCV_INCLUDE_DIRS}) 

# Source files
set(SOURCES
    src/main.cpp
    src/cpu_tasks.cpp
    src/gpu_tasks.cpp
    src/metrics.cpp
    src/scheduler.cpp
    src/task.cpp
)

# Create executable
add_executable(hybrid_scheduler_cpu ${SOURCES})

# Link libraries
target_link_libraries(hybrid_scheduler_cpu
    ${OpenCL_LIBRARIES}
    ${OpenCV_LIBS}   
)

# Copy kernel file(s) to build directory
file(COPY ${CMAKE_SOURCE_DIR}/kernels DESTINATION ${CMAKE_BINARY_DIR})

# Define input/output image directories
set(INPUT_DIR  ${CMAKE_SOURCE_DIR}/images/input_img)
set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/images/output_img)

# Make sure output directory exists
file(MAKE_DIRECTORY ${OUTPUT_DIR})

#  copy input images to build directory for easy testing
add_custom_command(TARGET hybrid_scheduler_cpu POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${INPUT_DIR} $<TARGET_FILE_DIR:hybrid_scheduler_cpu>/input_img
)
