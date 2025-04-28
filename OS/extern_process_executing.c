#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void get_path(char *path) {
    printf("Enter the path of the program the child process should execute:\n");
    scanf("%255s", path);
}

int main() {
    char path[256];
    get_path(path);

    pid_t pid = fork();

    if (pid < 0) {
        perror("Error creating child process");
        return 1;
    }

    if (pid == 0) {
        printf("Child (PID %d): executing '%s'\n", getpid(), path);
        sleep(2);
        execl(path, path, (char *)NULL);
        perror("execl failed");
        exit(1);
    } else {
        int status;
        printf("Parent (PID %d): waiting for child (PID %d) to finish...\n", getpid(), pid);
        sleep(2);
        waitpid(pid, &status, 0);
        printf("Parent (PID %d): child process finished\n", getpid());
    }
    return 0;
}
