#include <stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include<sys/wait.h>
#include <stdbool.h>

pid_t ppid;

void child();
//void parent();

int get_child_exit_status()
{
    int status;
    if (wait(&status) < 0)
        return EXIT_FAILURE;

    else if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return EXIT_FAILURE;
}

bool isChild(pid_t process){
    return process == 0;
}

bool isInvalid(pid_t process){
    return process < 0;
}

int main(void)
{
    pid_t child_pid;
    ppid = getpid();

    child_pid = fork();

    if (isChild(child_pid))
        child();

    else if (isInvalid(child_pid))
        return EXIT_FAILURE;

    printf("Child exited with status=%d",get_child_exit_status());

}

void child()
{
    pid_t c_pid = getpid();
    if(c_pid == ppid)
    {
        printf("This is not a child\n");
        return;
    }
    printf("This is a child\n");
    int status;
    scanf("%d",&status);
    exit(status);
}
