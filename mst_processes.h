#ifndef _MST_PROCESSES_H_
#define _MST_PROCESSES_H_

// Get the number of active processes owned by the user
void MSTUserProcessesNum(void);

// Get the number of active processes in the system
void MSTActiveProcessesNum(void);

// Print the active processes owned by the user
void MSTPrintUserProcesses(void);

#endif // _MST_PROCESSES_H_