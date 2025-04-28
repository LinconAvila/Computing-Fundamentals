#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

typedef enum {
    BOTH_EVEN,
    BOTH_ODD,
    PARENT_EVEN_CHILD_ODD,
    PARENT_ODD_CHILD_EVEN
} ParityPattern;

int is_even(pid_t n) {
    return n % 2 == 0;
}

ParityPattern get_parity_pattern(pid_t parent, pid_t child) {
    if (is_even(parent) && is_even(child)) {
        return BOTH_EVEN;
    } else if (!is_even(parent) && !is_even(child)) {
        return BOTH_ODD;
    } else if (is_even(parent) && !is_even(child)) {
        return PARENT_EVEN_CHILD_ODD;
    } else {
        return PARENT_ODD_CHILD_EVEN;
    }
}

int get_array_size() {
    int size;
    do {
        printf("Enter the size of the array: ");
        if (scanf("%d", &size) != 1 || size <= 0) {
            printf("Invalid input! Please enter a positive integer.\n");
            while (getchar() != '\n') { }
            size = 0;
        }
    } while (size <= 0);
    return size;
}

int *allocate_memory(int size) {
    int *array = malloc(size * sizeof(int));
    if (!array) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    return array;
}

void generate_random_array(int *a, int size, const char *name) {
    for (int i = 0; i < size; i++) {
        a[i] = rand() % 100;
        printf("%s[%d] = %d\n", name, i, a[i]);
        sleep(2);
    }
}

void multiply_arrays(int *a, int *b, int *r, int n) {
    printf("Child (pid %d) multiplying arrays...\n", getpid());
    fflush(stdout);
    sleep(2);
    for (int i = 0; i < n; i++) {
        r[i] = a[i] * b[i];
        printf("%s[%d] * %s[%d] = %d * %d = %d\n",
               "array1", i, "array2", i, a[i], b[i], r[i]);
        sleep(2);
    }
}

void subtract_arrays(int *a, int *b, int *r, int n) {
    printf("Child (pid %d) subtracting arrays...\n", getpid());
    fflush(stdout);
    sleep(2);
    for (int i = 0; i < n; i++) {
        r[i] = a[i] - b[i];
        printf("%s[%d] - %s[%d] = %d - %d = %d\n",
               "array1", i, "array2", i, a[i], b[i], r[i]);
        sleep(2);
    }
}

void sum_arrays(int *a, int *b, int *r, int n) {
    printf("Child (pid %d) summing arrays...\n", getpid());
    fflush(stdout);
    sleep(2);
    for (int i = 0; i < n; i++) {
        r[i] = a[i] + b[i];
        printf("%s[%d] + %s[%d] = %d + %d = %d\n",
               "array1", i, "array2", i, a[i], b[i], r[i]);
        sleep(2);
    }
}

int main() {
    int size = get_array_size();
    srand(time(NULL) ^ getpid());

    pid_t child = fork();

    if (child != 0) {
        int status;
        printf("\nParent process (pid %d) running. Child (pid %d) created.\n",
               getpid(), child);
        printf("State: current = %d, parent = %d, child = %d\n",
               getpid(), getppid(), child);
        fflush(stdout);

        if (is_even(child)) {
            printf("Parent (pid %d) waiting for Child (pid %d)\n",
                   getpid(), child);
            fflush(stdout);
            waitpid(child, &status, 0);
            printf("Child (pid %d) finished\n", child);
            fflush(stdout);
            sleep(2);
            printf("\n[Context switch]: Control returned to Parent (pid %d)\n",
                   getpid());
            printf("State: current = %d, parent = %d, child = %d (finished)\n",
                   getpid(), getppid(), 0);
            sleep(2);
            printf("Parent (pid %d) exiting.\n", getpid());
            printf("Program finished.\n");
            fflush(stdout);
        } else {
            printf("Parent (pid %d) will not wait for Child (pid %d)\n",
                   getpid(), child);
            printf("Parent (pid %d) exiting.\n", getpid());
            fflush(stdout);
        }
        exit(0);
    } else {
        pid_t parent_before = getppid();
        sleep(2);

        printf("\n[Context switch]: Child process (pid %d) started.\n",
               getpid());
        printf("State: current = %d, parent = %d, child = %d\n",
               getpid(), getppid(), 0);
        fflush(stdout);

        printf("Child (pid %d) generating random arrays...\n", getpid());
        fflush(stdout);
        sleep(2);

        int *array1 = allocate_memory(size);
        int *array2 = allocate_memory(size);
        int *result = allocate_memory(size);

        generate_random_array(array1, size, "array1");
        generate_random_array(array2, size, "array2");

        switch (get_parity_pattern(parent_before, getpid())) {
            case BOTH_EVEN:
                multiply_arrays(array1, array2, result, size);
                break;
            case BOTH_ODD:
                subtract_arrays(array1, array2, result, size);
                break;
            case PARENT_ODD_CHILD_EVEN:
                sum_arrays(array1, array2, result, size);
                break;
            case PARENT_EVEN_CHILD_ODD:
                multiply_arrays(array1, array2, result, size);
                subtract_arrays(array1, array2, result, size);
                sum_arrays(array1, array2, result, size);
                break;
        }

        if (!is_even(getpid())) {
            printf("Child (pid %d) exiting.\n", getpid());
            printf("Program finished.\n");
        }

        free(array1);
        free(array2);
        free(result);
        exit(0);
    }

    return 0;
}
