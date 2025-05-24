#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Estrutura para armazenar os dados de cada thread
typedef struct {
    int id;
    int start;
    int end;
    int rows;
    int cols;
    int **matrix_A;
    int **matrix_B; // matriz transposta
} thread_data;

// Função para alocar uma matriz
int **allocate_matrix(int rows, int cols) {
    int **matrix = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *)malloc(cols * sizeof(int));
    }
    return matrix;
}

// Função para preencher a matriz com valores aleatórios
void fill_random_matrix(int rows, int cols, int **matrix) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            matrix[i][j] = rand() % 100;
}

// Função para imprimir a matriz
void print_matrix(int rows, int cols, int **matrix) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            printf("%3d ", matrix[i][j]);
        printf("\n");
    }
}

// Função executada pelas threads para transpor a matriz
void *transpose_matrix(void *arg) {
    thread_data *data = (thread_data *)arg;

    for (int idx = data->start; idx < data->end; idx++) {
        int i = idx / data->cols;
        int j = idx % data->cols;
        data->matrix_B[j][i] = data->matrix_A[i][j];
        printf("Thread %d transposiciona [%d][%d] = %d → [%d][%d]\n",
               data->id, i, j, data->matrix_A[i][j], j, i);
    }

    return NULL;
}

// Função para obter tamanho da matriz
void get_matrix_size(int *rows, int *cols) {
    do {
        printf("Digite o número de linhas: ");
        if (scanf("%d", rows) != 1 || *rows <= 0) {
            printf("Entrada inválida!\n");
            while (getchar() != '\n');
            *rows = 0;
        }
    } while (*rows <= 0);

    do {
        printf("Digite o número de colunas: ");
        if (scanf("%d", cols) != 1 || *cols <= 0) {
            printf("Entrada inválida!\n");
            while (getchar() != '\n');
            *cols = 0;
        }
    } while (*cols <= 0);
}

// Função para obter número de threads
int get_threads_count(int max_threads) {
    int count;
    do {
        printf("Digite o número de threads (até %d): ", max_threads);
        if (scanf("%d", &count) != 1 || count <= 0 || count > max_threads) {
            printf("Entrada inválida!\n");
            while (getchar() != '\n');
            count = 0;
        }
    } while (count <= 0);
    return count;
}

int main() {
    srand(time(NULL));

    int rows, cols;
    get_matrix_size(&rows, &cols);

    int **matrix_A = allocate_matrix(rows, cols);
    int **matrix_B = allocate_matrix(cols, rows); // transposta

    fill_random_matrix(rows, cols, matrix_A);

    printf("\nMatriz A:\n");
    print_matrix(rows, cols, matrix_A);

    int total_elements = rows * cols;
    int threads_count = get_threads_count(total_elements);

    pthread_t threads[threads_count];
    thread_data data[threads_count];

    int base_load = total_elements / threads_count;
    int remainder = total_elements % threads_count;

    clock_t start_time = clock();

    for (int i = 0; i < threads_count; i++) {
        data[i].id = i;

        if (i < remainder) {
            data[i].start = i * base_load + i;
            data[i].end = data[i].start + base_load + 1;
        } else {
            data[i].start = i * base_load + remainder;
            data[i].end = data[i].start + base_load;
        }

        data[i].rows = rows;
        data[i].cols = cols;
        data[i].matrix_A = matrix_A;
        data[i].matrix_B = matrix_B;

        pthread_create(&threads[i], NULL, transpose_matrix, &data[i]);
        printf("Thread %d -> intervalo [%d, %d)\n", i, data[i].start + 1, data[i].end);
    }

    for (int i = 0; i < threads_count; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("\nMatriz B (Transposta):\n");
    print_matrix(cols, rows, matrix_B);

    printf("Tempo total: %f segundos\n", elapsed_time);

    return 0;
}
