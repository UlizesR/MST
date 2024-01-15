#include "MST.h"

#include <stdio.h>
#include <string.h>

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void printCpuUsage()
{
    // program cpu usage
    // Calculate and print the program's CPU usage
    double program_cpu_usage = MSTProcessCPUUsage();
    double system_cpu_usage = MSTCPUUsage();
    if (program_cpu_usage >= 0) {
        printf("Program CPU Usage: %.2f%% | System CPU Usage: %.2f%%\n", program_cpu_usage * 100.0, system_cpu_usage * 100.0);
    } else {
        printf("Error calculating program CPU usage.\n");
    }
}

int main(int argc, const char * argv[])
{
    MSTGetCpuInfo();
    MSTActiveProcessesNum();
    MSTUserProcessesNum();
    MSTPrintUserProcesses();
    
    return 0;
}