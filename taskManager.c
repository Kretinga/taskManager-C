#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

// ANSI CODE COLORS
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

// CONSTANTS
#define MAX_COMMAND_LENGTH 100
#define MAX_BUFFER 1024

// Principal Functions

int listProc()
{
    DIR *dir;
    struct dirent *entry;
    FILE *fp;
    char path[MAX_BUFFER];
    char cmdline[MAX_BUFFER];

    dir = opendir("/proc");
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR && atoi(entry->d_name) != 0 )
        {
            snprintf(path, sizeof(path), "/proc/%s/cmdline", entry->d_name);
            fp = fopen(path, "r");
            if (fp != NULL)
            {
                fgets(cmdline, sizeof(cmdline), fp);
                printf(BLUE "[<+>] Process ID: %s || %s\n", entry->d_name, cmdline, RESET);
            }
            else
            {
                printf(RED "ERROR" RESET);
            }
        }
    }

    closedir(dir);
    return 0;
}

int killProc(int pid)
{
    // Enviar señal para cerrar el PID
    int result = kill(pid, SIGKILL);

    if (result == 0)
    {
        printf(GREEN "[<[+]>] Process %d terminated succesfully\n\n", pid);
    }
    else 
    {
        printf(RED "[<!>] An error ocurred...\n\n" RESET);
    }

    return 0;
}

int infoProc(int pid)
{
    char path[MAX_BUFFER];
    char buffer[MAX_BUFFER];

    snprintf(path, sizeof(path), "/proc/%d/status", pid);

    FILE *fp = fopen(path, "r");

    printf("\n[<+>] PID info: %d\n\n", pid);
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        printf("%s", buffer);
    }
}

// Main Function

int main()
{
    char command[MAX_COMMAND_LENGTH];
    int pid;

    while(1)
    {
        printf(GREEN "[<>] Task Manager >> " RESET);
        fflush(stdout);
        
        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            printf(RED "\n[*] An error ocurred" RESET);
            break;
        }

        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "list") == 0)
        {
            listProc();
        }

        else if (strncmp(command, "kill", 4) == 0)
        {
            char *pid_str = command + 4;
            int pid = atoi(pid_str);

            if (pid != 0)
            {
                killProc(pid);
            }
            else {
                printf(RED "[<!>] Incorrect format, use: kill [PID]\n" RESET);
            }
        }

        else if (strncmp(command, "info", 4) == 0)
        {
            char *pid_str = command + 4;
            int pid = atoi(pid_str);

            if (pid != 0)
            {
                infoProc(pid);
            }
            else {
                printf(RED "[<!>] Incorrect format, use: info [PID]\n" RESET);
            }
        }

        else if (strcmp(command, "help") == 0)
        {
            printf(MAGENTA "\n----COMMANDS----\n[1] - list (List process)\n[2] - kill <pid> (Kill a process)\n[3] - info <pid> (More info about a process)\n[4] - exit (Exit the taskmanager)\n\n" RESET);
        }

        else if (strcmp(command, "exit") == 0)
        {
            break;
        }

        else {
            printf(RED "[<!>] Command not found, type help for more information\n" RESET);
        }
    }

    return 0;
}
