#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h> // Library to generate random numbers and measure execution time

// Structure to store thread data
typedef struct {
    int id;               // Thread identifier
    int start;            // Start index for the thread to process
    int end;              // End index for the thread to process
    int rows;             // Number of rows in the matrix
    int cols;             // Number of columns in the matrix
    int **matrix_A;       // Matrix A
    int **matrix_B;       // Matrix B
    int **matrix_C;       // Matrix C (result of the sum)
} thread_data;

// Function to allocate a matrix with the given number of rows and columns
int **allocate_matrix(int rows, int cols) {
    int **matrix = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *)malloc(cols * sizeof(int));
    }
    return matrix;
}

// Function to fill the matrix with random values
void fill_random_matrix(int rows, int cols, int **matrix) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 100;
        }
    }
}

// Function executed by the threads to sum elements of matrices A and B
void *sum_matrices(void *arg) {
    thread_data *data = (thread_data *)arg;

    // Each thread sums the elements within its assigned interval
    for (int idx = data->start; idx < data->end; idx++) {
        int i = idx / data->cols;
        int j = idx % data->cols;
        int a = data->matrix_A[i][j];
        int b = data->matrix_B[i][j];
        data->matrix_C[i][j] = a + b;
        printf("Thread %d sums element [%d,%d]: %d + %d = %d\n", data->id, i + 1, j + 1, a, b, a + b);
    }
    return NULL;
}

// Function to print the matrix to the console
void print_matrix(int rows, int cols, int **matrix) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Function to get matrix dimensions from user input
void get_matrix_size(int *rows, int *cols) {
    do {
        printf("Enter the number of rows: ");
        if (scanf("%d", rows) != 1 || *rows <= 0) {
            printf("Invalid input!\n");
            while (getchar() != '\n');
            *rows = 0;
        }
    } while (*rows <= 0);

    do {
        printf("Enter the number of columns: ");
        if (scanf("%d", cols) != 1 || *cols <= 0) {
            printf("Invalid input!\n");
            while (getchar() != '\n');
            *cols = 0;
        }
    } while (*cols <= 0);
}

// Function to get the number of threads from the user
int get_threads_count(int max_threads) {
    int threads_count;
    do {
        printf("Enter the number of threads (up to %d): ", max_threads);
        if (scanf("%d", &threads_count) != 1 || threads_count <= 0 || threads_count > max_threads) {
            printf("Invalid input!\n");
            while (getchar() != '\n');
            threads_count = 0;
        }
    } while (threads_count <= 0);
    return threads_count;
}

int main() {
    srand(time(NULL));  // Seed for random number generation

    int rows, cols;
    get_matrix_size(&rows, &cols);  // Get matrix size from user

    // Allocate memory for matrices A, B, and C
    int **matrix_A = allocate_matrix(rows, cols);
    int **matrix_B = allocate_matrix(rows, cols);
    int **matrix_C = allocate_matrix(rows, cols);

    // Fill matrices A and B with random values
    fill_random_matrix(rows, cols, matrix_A);
    fill_random_matrix(rows, cols, matrix_B);

    printf("\nMatrix A:\n");
    print_matrix(rows, cols, matrix_A);

    printf("\nMatrix B:\n");
    print_matrix(rows, cols, matrix_B);

    int num_elements = rows * cols;
    int threads_count = get_threads_count(num_elements);

    pthread_t threads[threads_count];
    thread_data data[threads_count];

    int base_load = num_elements / threads_count;
    int remainder = num_elements % threads_count;

    clock_t start_time = clock();

    for (int i = 0; i < threads_count; i++) {
        data[i].id = i;
        if (i < remainder) {
            data[i].start = i * base_load + i;
        } else {
            data[i].start = i * base_load + remainder;
        }
        data[i].end = data[i].start + base_load + (i < remainder ? 1 : 0);
        data[i].rows = rows;
        data[i].cols = cols;
        data[i].matrix_A = matrix_A;
        data[i].matrix_B = matrix_B;
        data[i].matrix_C = matrix_C;

        pthread_create(&threads[i], NULL, sum_matrices, (void *)&data[i]);
        printf("Thread %d -> range [%d, %d)\n", data[i].id, data[i].start + 1, data[i].end + 1);
    }

    for (int j = 0; j < threads_count; j++) {
        pthread_join(threads[j], NULL);
    }

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("\nMatrix C (A + B):\n");
    print_matrix(rows, cols, matrix_C);

    printf("Total time: %f seconds\n", elapsed_time);

    return 0;
}
