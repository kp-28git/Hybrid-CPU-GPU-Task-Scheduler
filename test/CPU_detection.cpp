#include <iostream>
#include <windows.h>
#include <thread>
#include <intrin.h>

// Function to query CPU vendor, family, model, and stepping
void cpuInfo_cpuid() {
    int cpuInfo[4] = {0};
    

    __cpuid(cpuInfo, 0);
    char vendor[13];
    memcpy(vendor, &cpuInfo[1], 4); 
    memcpy(vendor + 4, &cpuInfo[3], 4); 
    memcpy(vendor + 8, &cpuInfo[2], 4); 
    vendor[12] = '\0';
    std::cout << "CPU Vendor: " << vendor << "\n";

    // Processor Info and Features
    __cpuid(cpuInfo, 1);
    int family = ((cpuInfo[0] >> 8) & 0xF) + ((cpuInfo[0] >> 20) & 0xFF);
    int model  = ((cpuInfo[0] >> 4) & 0xF) + ((cpuInfo[0] >> 16) & 0xF) << 4;
    int stepping = cpuInfo[0] & 0xF;

    std::cout << "CPU Family: " << family
              << ", Model: " << model
              << ", Stepping: " << stepping << "\n";
}


int main() {
    cpuInfo_cpuid();
    unsigned int cores = std::thread::hardware_concurrency();
    std::cout << "CPU Cores: " << cores << "\n";
    return 0;
}
