#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Estrutura de dados para as threads
typedef struct {
    int id;
    int *A, *B, *C;
    int m, n, p;
    int start_row, end_row;
} ThreadData;

// Função executada por cada thread
void *multiply_matrix(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    for (int i = data->start_row; i < data->end_row; ++i) {
        for (int j = 0; j < data->p; ++j) {
            int sum = 0;
            for (int k = 0; k < data->n; ++k) {
                sum += data->A[i * data->n + k] * data->B[k * data->p + j];
            }
            data->C[i * data->p + j] = sum;
            printf("Thread %d calcula C[%d][%d] = %d\n", data->id, i, j, sum);
        }
    }

    return NULL;
}

// Aloca dinamicamente uma matriz linearizada
int *allocate_matrix(int rows, int cols) {
    return (int *)malloc(rows * cols * sizeof(int));
}

// Preenche a matriz com valores aleatórios entre 0 e 9
void fill_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows * cols; ++i)
        matrix[i] = rand() % 10;
}

// Imprime a matriz
void print_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j)
            printf("%4d ", matrix[i * cols + j]);
        printf("\n");
    }
}

// Solicita entrada válida de inteiro positivo
int get_positive_int(const char *prompt) {
    int value;
    do {
        printf("%s", prompt);
        if (scanf("%d", &value) != 1 || value <= 0) {
            printf("Entrada inválida!\n");
            while (getchar() != '\n');
            value = 0;
        }
    } while (value <= 0);
    return value;
}

int main() {
    srand(time(NULL));

    // Entrada robusta das dimensões
    int m = get_positive_int("Digite o número de linhas da matriz A (M): ");
    int n = get_positive_int("Digite o número de colunas da matriz A e linhas da B (N): ");
    int p = get_positive_int("Digite o número de colunas da matriz B (P): ");
    int total_threads = get_positive_int("Digite o número de threads: ");

    // Alocação de memória
    int *A = allocate_matrix(m, n);
    int *B = allocate_matrix(n, p);
    int *C = allocate_matrix(m, p);

    fill_matrix(A, m, n);
    fill_matrix(B, n, p);

    printf("\nMatriz A:\n");
    print_matrix(A, m, n);

    printf("\nMatriz B:\n");
    print_matrix(B, n, p);

    pthread_t threads[total_threads];
    ThreadData thread_data[total_threads];

    int base_rows = m / total_threads;
    int remainder = m % total_threads;
    int current_row = 0;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Criação das threads
    for (int i = 0; i < total_threads; ++i) {
        int rows = base_rows + (i < remainder ? 1 : 0);

        thread_data[i] = (ThreadData){
            .id = i,
            .A = A, .B = B, .C = C,
            .m = m, .n = n, .p = p,
            .start_row = current_row,
            .end_row = current_row + rows
        };

        pthread_create(&threads[i], NULL, multiply_matrix, &thread_data[i]);

        printf("Thread %d -> linhas [%d, %d)\n", i, current_row, current_row + rows);
        current_row += rows;
    }

    // Espera das threads
    for (int i = 0; i < total_threads; ++i)
        pthread_join(threads[i], NULL);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) +
                     (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("\nMatriz Resultado (A x B):\n");
    print_matrix(C, m, p);

    printf("\nTempo de execução: %.6f segundos\n", elapsed);

    // Liberação de memória
    free(A);
    free(B);
    free(C);

    return 0;
}
