#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Estrutura para armazenar os dados da thread
typedef struct {
    int id;         // ID da thread
    int start;      // Índice inicial de processamento
    int end;        // Índice final de processamento
    int *vetor_a;   // Vetor A
    int *vetor_b;   // Vetor B
    int *vetor_c;   // Resultado A * B
} thread_data;

// Função para preencher os vetores com números aleatórios
void fill_random_vector(int *vetor, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = rand() % 100;
    }
}

// Função para imprimir um vetor
void print_vector(const char *nome, int *vetor, int tamanho) {
    printf("%s: ", nome);
    for (int i = 0; i < tamanho; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
}

// Função executada pelas threads para multiplicar os elementos dos vetores
void *multiply_vectors(void *arg) {
    thread_data *data = (thread_data *)arg;

    for (int i = data->start; i < data->end; i++) {
        int a = data->vetor_a[i];
        int b = data->vetor_b[i];
        data->vetor_c[i] = a * b;
        printf("Thread %d multiplica elemento [%d]: %d * %d = %d\n", data->id, i, a, b, data->vetor_c[i]);
    }

    return NULL;
}

// Função para obter o tamanho do vetor
int get_vector_size() {
    int tamanho;
    do {
        printf("Digite o tamanho do vetor: ");
        if (scanf("%d", &tamanho) != 1 || tamanho <= 0) {
            printf("Entrada inválida!\n");
            while (getchar() != '\n');
            tamanho = 0;
        }
    } while (tamanho <= 0);
    return tamanho;
}

// Função para obter a quantidade de threads
int get_threads_count(int max) {
    int threads;
    do {
        printf("Digite o número de threads (até %d): ", max);
        if (scanf("%d", &threads) != 1 || threads <= 0 || threads > max) {
            printf("Entrada inválida!\n");
            while (getchar() != '\n');
            threads = 0;
        }
    } while (threads <= 0);
    return threads;
}

int main() {
    srand(time(NULL));
    int tamanho = get_vector_size();

    int *vetor_a = (int *)malloc(tamanho * sizeof(int));
    int *vetor_b = (int *)malloc(tamanho * sizeof(int));
    int *vetor_c = (int *)malloc(tamanho * sizeof(int));

    fill_random_vector(vetor_a, tamanho);
    fill_random_vector(vetor_b, tamanho);

    printf("\nVetor A:\n");
    print_vector("A", vetor_a, tamanho);

    printf("\nVetor B:\n");
    print_vector("B", vetor_b, tamanho);

    int num_threads = get_threads_count(tamanho);
    pthread_t threads[num_threads];
    thread_data data[num_threads];

    int base_load = tamanho / num_threads;
    int remainder = tamanho % num_threads;

    clock_t start_time = clock();

    for (int i = 0; i < num_threads; i++) {
        data[i].id = i;
        data[i].start = (i < remainder) ? i * (base_load + 1) : i * base_load + remainder;
        data[i].end = data[i].start + ((i < remainder) ? base_load + 1 : base_load);
        data[i].vetor_a = vetor_a;
        data[i].vetor_b = vetor_b;
        data[i].vetor_c = vetor_c;

        pthread_create(&threads[i], NULL, multiply_vectors, &data[i]);

        printf("Thread %d -> intervalo [%d, %d)\n", i, data[i].start, data[i].end);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("\nVetor C (A * B):\n");
    print_vector("C", vetor_c, tamanho);
    printf("Tempo total: %f segundos\n", elapsed_time);

    free(vetor_a);
    free(vetor_b);
    free(vetor_c);
    return 0;
}
