#include "mst_cpu.h"

#include <mach/mach_host.h>
#include <mach/processor_info.h>
#include <mach/vm_map.h>

#include <sys/resource.h>
#include <sys/sysctl.h>

#include <stdio.h>
#include <stdlib.h>

double MSTCPUUsage(void)
{
    processor_cpu_load_info_t cpuLoad;
    mach_msg_type_number_t processorMsgCount;
    natural_t processorCount;

    kern_return_t kr = host_processor_info(mach_host_self(), PROCESSOR_CPU_LOAD_INFO, &processorCount, (processor_info_array_t *)&cpuLoad, &processorMsgCount);
    if (kr != KERN_SUCCESS) {
        return -1.0;
    }

    double totalUser = 0.0, totalSystem = 0.0, totalIdle = 0.0;

    for (natural_t i = 0; i < processorCount; i++) {
        totalUser += cpuLoad[i].cpu_ticks[CPU_STATE_USER];
        totalSystem += cpuLoad[i].cpu_ticks[CPU_STATE_SYSTEM];
        totalIdle += cpuLoad[i].cpu_ticks[CPU_STATE_IDLE];
    }

    vm_deallocate(mach_task_self(), (vm_address_t)cpuLoad, processorMsgCount);

    double total = totalUser + totalSystem + totalIdle;
    return (totalUser + totalSystem) / total;
}

double MSTProcessCPUUsage(void)
{
    struct rusage usage;
    int ret;

    // Get resource usage statistics for your process
    ret = getrusage(RUSAGE_SELF, &usage);
    if (ret != 0) {
        perror("getrusage");
        return -1.0;
    }

    // Calculate the program's CPU usage as a percentage
    double user_time = (double)usage.ru_utime.tv_sec + (double)usage.ru_utime.tv_usec / 1000000.0;
    double system_time = (double)usage.ru_stime.tv_sec + (double)usage.ru_stime.tv_usec / 1000000.0;
    double total_cpu_time = user_time + system_time;
    double total_time_elapsed = (double)usage.ru_utime.tv_sec + (double)usage.ru_utime.tv_usec / 1000000.0 +
                                (double)usage.ru_stime.tv_sec + (double)usage.ru_stime.tv_usec / 1000000.0;
    double program_cpu_usage_percentage = (total_cpu_time / total_time_elapsed);

    return program_cpu_usage_percentage;
}

void MSTGetCpuInfo(void) {
    cpu_type_t type;
    cpu_subtype_t subtype;

    size_t len = sizeof(type);
    sysctlbyname("hw.cputype", &type, &len, NULL, 0);
    len = sizeof(subtype);
    sysctlbyname("hw.cpusubtype", &subtype, &len, NULL, 0);

    char *typeStr, *subtypeStr;
    switch (type) {
        case CPU_TYPE_X86_64:
            typeStr = "x86_64";
            break;
        // add more cases as needed
        case CPU_TYPE_ARM64:
            typeStr = "arm64";
            break;
        case CPU_TYPE_ARM64_32:
            typeStr = "arm64_32";
            break;
        case CPU_TYPE_ARM:
            typeStr = "arm";
            break;
        default:
            typeStr = "unknown";
    }
    switch (subtype) {
        case CPU_SUBTYPE_X86_64_ALL:
            subtypeStr = "all";
            break;
        // add more cases as needed
        case CPU_SUBTYPE_ARM64E:
            subtypeStr = "arm64e";
            break;
        case CPU_SUBTYPE_ARM64_V8:
            subtypeStr = "arm64v8";
            break;
        case CPU_SUBTYPE_ARM64_32_ALL:
            subtypeStr = "arm64_32_all";
            break;
        default:
            subtypeStr = "unknown";
    }
    
    char cpuName[128];
    len = sizeof(cpuName);
    
    if (sysctlbyname("machdep.cpu.brand_string", &cpuName, &len, NULL, 0) == 0) {
        printf("CPU Name: %s\n", cpuName);
    } else {
        perror("sysctlbyname");
    }
    printf("CPU Type: %s\n", typeStr);
    printf("CPU Subtype: %s\n", subtypeStr);
}