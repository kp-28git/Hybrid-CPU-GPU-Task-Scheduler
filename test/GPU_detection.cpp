#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl.h>                  
#include <iostream>
#include <vector>

// Function to list all OpenCL GPU devices on the system
void listOpenCLGPUDevices() {
    cl_uint numPlatforms = 0;

    // Step 1: Get number of available OpenCL platforms (vendors)
    clGetPlatformIDs(0, nullptr, &numPlatforms);

    // Step 2: Retrieve all platform IDs
    std::vector<cl_platform_id> platforms(numPlatforms);
    clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);

    for (auto &platform : platforms) {
        cl_uint numDevices = 0; // Will store number of devices for this platform

        // Step 3: Query number of devices for the platform
        // CL_DEVICE_TYPE_ALL → list CPUs, GPUs, accelerators, etc.
        clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, nullptr, &numDevices);

        // Step 4: Retrieve all GPU device IDs
        std::vector<cl_device_id> devices(numDevices);
        clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, numDevices, devices.data(), nullptr);

        // Loop through each device
        for (auto &device : devices) {
            char name[128];      
            cl_uint computeUnits;
            cl_ulong memSize;    

            // Step 5: Get device name
            clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(name), name, nullptr);

            // Step 6: Get number of compute units (cores for GPU/CPU in OpenCL terms)
            clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(computeUnits), &computeUnits, nullptr);

            // Step 7: Get total global memory size
            clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(memSize), &memSize, nullptr);

            // Step 8: Print device info
            std::cout << "GPU: " << name << "\n";
            std::cout << "Compute Units: " << computeUnits << "\n";
            std::cout << "Global Memory: " << (memSize / (1024 * 1024)) << " MB\n\n";
        }
    }
}

int main() {
    listOpenCLGPUDevices();
    return 0;
}
