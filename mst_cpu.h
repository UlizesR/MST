#ifndef _MST_CPU_H_
#define _MST_CPU_H_

// CPU Processing information

// Returns the percentage of CPU used by the system
double MSTCPUUsage(void);

// Returns the percentage of CPU used by the current process
double MSTProcessCPUUsage(void);

// Get the CPU information
void MSTGetCpuInfo(void);

#endif // _MST_CPU_H_