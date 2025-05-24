#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

int main(){
    int size, i, status;
    pid_t child, me, parent, x;

    while (1) {
        printf("Enter the size of the array: ");
        if (scanf("%d", &size) == 1 && size > 0) {
            break;
        } else {
            printf("Invalid size. Try again.\n");
            while (getchar() != '\n');
        }
    }

    int *array1 = (int*)malloc(size * sizeof(int));
    int *array2 = (int*)malloc(size * sizeof(int));
    int *array_result = (int*)malloc(size * sizeof(int));

    if (array1 == NULL || array2 == NULL || array_result == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        return 1;
    }

    child = fork();
    me = getpid();
    parent = getppid();
    status = 0;

    if (child != 0) {
        printf("\nParent process (pid %d) executing. Child process (pid %d) created.\n", getpid(), child);
        printf("State: current = %d, parent = %d, child = %d\n", getpid(), getppid(), child);

        printf("Parent (pid %d) waiting for child (pid %d)\n", getpid(), child);
        x = waitpid(child, &status, 0);
        sleep(2);
        printf("Child (pid %d) finished\n", x);

        printf("\n[Context switch]: Control returned to Parent (pid %d)\n", getpid());
        printf("State: current = %d, parent = %d, child = %d (finished)\n", getpid(), getppid(), 0);

        sleep(2);
        printf("Parent (pid %d) exiting.\n", getpid());

        printf("Program finished.\n");

        exit(0);
    } else {

        printf("\n[Context switch]: Child process (pid %d) started.\n", getpid());
        printf("State: current = %d, parent = %d, child = %d\n", getpid(), getppid(), 0);

        srand(time(NULL) ^ getpid());
        printf("Child (pid %d) generating random arrays...\n", getpid());
        sleep(2);
        for (i = 0; i < size; i++) {
            array1[i] = rand() % 100;
            array2[i] = rand() % 100;
            printf("array1[%d] = %d, array2[%d] = %d\n", i, array1[i], i, array2[i]);
            sleep(2);
        }

        printf("Child (pid %d) multiplying arrays...\n", getpid());
        for (i = 0; i < size; i++) {
            array_result[i] = array1[i] * array2[i];
            printf("array1[%d] * array2[%d] = %d * %d = %d\n", i, i, array1[i], array2[i], array_result[i]);
            sleep(2);
        }
    }
    return 0;
}
