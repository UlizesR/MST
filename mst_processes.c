


#include <libproc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void MSTUserProcessesNum(void) {
    // Get the current user's ID
    uid_t uid = getuid();

    // Get the number of processes
    int numberOfProcesses = proc_listallpids(NULL, 0);
    if (numberOfProcesses <= 0) {
        perror("proc_listallpids");
        return;
    }

    // Allocate memory for the process IDs
    pid_t *pids = malloc(numberOfProcesses * sizeof(pid_t));
    if (pids == NULL) {
        perror("malloc");
        return;
    }

    // Get the process IDs
    numberOfProcesses = proc_listallpids(pids, numberOfProcesses);
    if (numberOfProcesses <= 0) {
        perror("proc_listallpids");
        free(pids);
        return;
    }

    // Count the processes owned by the current user
    int numberOfUserProcesses = 0;
    for (int i = 0; i < numberOfProcesses; i++) {
        struct proc_bsdinfo info;
        if (proc_pidinfo(pids[i], PROC_PIDTBSDINFO, 0, &info, sizeof(info)) > 0) {
            if (info.pbi_uid == uid) {
                numberOfUserProcesses++;
            }
        }
    }

    free(pids);

    printf("Number of user processes: %d\n", numberOfUserProcesses);
}

void MSTActiveProcessesNum(void) {
    int numberOfProcesses = proc_listallpids(NULL, 0);
    if (numberOfProcesses <= 0) {
        perror("proc_listallpids");
        return;
    }

    printf("Number of active processes: %d\n", numberOfProcesses);
}

void MSTPrintUserProcesses(void) {
    // Get the current user's ID
    uid_t uid = getuid();

    // Get the number of processes
    int numberOfProcesses = proc_listallpids(NULL, 0);
    if (numberOfProcesses <= 0) {
        perror("proc_listallpids");
        return;
    }

    // Allocate memory for the process IDs
    pid_t *pids = malloc(numberOfProcesses * sizeof(pid_t));
    if (pids == NULL) {
        perror("malloc");
        return;
    }

    // Get the process IDs
    numberOfProcesses = proc_listallpids(pids, numberOfProcesses);
    if (numberOfProcesses <= 0) {
        perror("proc_listallpids");
        free(pids);
        return;
    }

    // Print the paths of the processes owned by the current user
    for (int i = 0; i < numberOfProcesses; i++) {
        struct proc_bsdinfo info;
        if (proc_pidinfo(pids[i], PROC_PIDTBSDINFO, 0, &info, sizeof(info)) > 0) {
            if (info.pbi_uid == uid) {
                char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
                proc_pidpath(pids[i], pathbuf, sizeof(pathbuf));
                printf("Process ID: %d, Path: %s\n", pids[i], pathbuf);
            }
        }
    }

    free(pids);
}