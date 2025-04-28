#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int get_tens_and_hundreds(int pid) {
    int hundreds = (pid / 100) % 10;
    int tens     = (pid /  10) % 10;
    return hundreds * 10 + tens;
}

void collatz_sequence(unsigned int x){
    while (x != 1) {
        printf("%u ", x);
        fflush(stdout);
        if (x % 2 == 0) {
            x = x / 2;
        } else {
            x = 3 * x + 1;
        }
    }
    printf("1\n");
}

int process_quantity(void) {
    int quantity;
    while (1) {
        printf("Enter the number of processes: ");
        if (scanf("%d", &quantity) == 1 && quantity > 0) {
            while (getchar() != '\n'); 
            return quantity;
        }
        printf("Invalid quantity. Try again.\n");
        while (getchar() != '\n');
    }
}

int main(){
    int i, n, status;
    pid_t x, child;
    n = process_quantity();
    for(i = 0; i < n; i++) {
        child = fork();

        if (child != 0) {
            printf("\nParent process (pid %d) running. Child process (pid %d) created.\n", getpid(), child);
            printf("State: current = %d, parent = %d, child = %d\n", getpid(), getppid(), child);
            printf("Parent (pid %d) waiting for child (pid %d)\n", getpid(), child);
            x = waitpid(child, &status, 0);
            sleep(2);
            printf("Child (pid %d) finished\n", x);
            printf("\n[Context switch]: Control returned to parent (pid %d)\n", getpid());
            printf("State: current = %d, parent = %d, child = %d (finished)\n", getpid(), getppid(), 0);
        } 
        else {
            printf("\n[Context switch]: Child process (pid %d) started.\n", getpid());
            printf("State: current = %d, parent = %d, child = %d\n", getpid(), getppid(), 0);
            printf("Child (pid %d) separating PID number into tens and hundreds...\n", getpid());
            sleep(2);

            unsigned int num_id = get_tens_and_hundreds(getpid());
            printf("PID %d separated into tens and hundreds: %u\n", getpid(), num_id);
            printf("Child (pid %d) generating Collatz sequence for number %u...\n", getpid(), num_id);
            sleep(2);
            printf("Collatz sequence for number %u: ", num_id);
            collatz_sequence(num_id);
            exit(0);
        }
    }

    return 0;
}
