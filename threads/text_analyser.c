#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <dirent.h>

#define MAX_WORD_LEN 1000
#define HASH_SIZE 1000
#define MAX_THREADS 1000
#define MAX_PATH_LEN 512

typedef struct NodePalavra {
    char palavra[MAX_WORD_LEN];
    int count;
    struct NodePalavra *next;
} NodePalavra;

typedef struct {
    char diretorio[MAX_PATH_LEN];
    char nome_arquivo[MAX_PATH_LEN];
} ParamThread;

NodePalavra *tab_palavras[HASH_SIZE] = {NULL};

unsigned int hash(const char *str) {
    unsigned int hash = 0;
    while (*str) hash = (hash << 5) + *str++;
    return hash % HASH_SIZE;
}

void ad_palavra(const char *palavra) {
    unsigned int indice = hash(palavra);
    NodePalavra *node = tab_palavras[indice];
    while (node) {
        if (strcmp(node->palavra, palavra) == 0) {
            node->count++;
            return;
        }
        node = node->next;
    }
    NodePalavra *new_node = malloc(sizeof(NodePalavra));
    strcpy(new_node->palavra, palavra);
    new_node->count = 1;
    new_node->next = tab_palavras[indice];
    tab_palavras[indice] = new_node;
}

void libera_tab_palavra() {
    for (int i = 0; i < HASH_SIZE; i++) {
        NodePalavra *node = tab_palavras[i];
        while (node) {
            NodePalavra *tmp = node;
            node = node->next;
            free(tmp);
        }
        tab_palavras[i] = NULL;
    }
}

int verifica_vogal(char c) {
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

int verifica_consoante(char c) {
    return isalpha(c) && !verifica_vogal(c);
}

void *processa_arquivo_thread(void *arg) {
    ParamThread *param = (ParamThread *)arg;
    char file_path[3024], output_path[3024];
    snprintf(file_path, sizeof(file_path), "%s/%s", param->diretorio, param->nome_arquivo);
    snprintf(output_path, sizeof(output_path), "%s/%s.upper.txt", param->diretorio, param->nome_arquivo);

    FILE *input = fopen(file_path, "r");
    if (!input) {
        perror("Erro ao abrir arquivo");
        free(param);
        return NULL;
    }

    FILE *output = fopen(output_path, "w");
    if (!output) {
        perror("Erro ao criar arquivo de saída");
        fclose(input);
        free(param);
        return NULL;
    }

    int count_palavras = 0, count_vogal = 0, count_consoante = 0;
    int freq_vogal[26] = {0}, freq_consoante[26] = {0};
    char ch, palavra[MAX_WORD_LEN];
    int idx = 0;

    while ((ch = fgetc(input)) != EOF) {
        fputc(toupper(ch), output);
        if (isalpha(ch)) {
            char lower = tolower(ch);
            if (verifica_vogal(lower))
                freq_vogal[lower - 'a']++;
            else
                freq_consoante[lower - 'a']++;

            if (idx < MAX_WORD_LEN - 1)
                palavra[idx++] = lower;
        } else {
            if (idx > 0) {
                palavra[idx] = '\0';
                ad_palavra(palavra);
                count_palavras++;
                idx = 0;
            }
        }
    }

    if (idx > 0) {
        palavra[idx] = '\0';
        ad_palavra(palavra);
        count_palavras++;
    }

    for (int i = 0; i < 26; i++) {
        count_vogal += freq_vogal[i];
        count_consoante += freq_consoante[i];
    }

    char palavra_mais_freq[MAX_WORD_LEN] = "";
    int max_pcount = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        NodePalavra *node = tab_palavras[i];
        while (node) {
            if (node->count > max_pcount) {
                max_pcount = node->count;
                strcpy(palavra_mais_freq, node->palavra);
            }
            node = node->next;
        }
    }

    char vogal_mais_freq = '\0', consoante_mais_freq = '\0';
    int max_vogal = 0, max_consoante = 0;
    for (int i = 0; i < 26; i++) {
        if (freq_vogal[i] > max_vogal) {
            max_vogal = freq_vogal[i];
            vogal_mais_freq = 'a' + i;
        }
        if (freq_consoante[i] > max_consoante) {
            max_consoante = freq_consoante[i];
            consoante_mais_freq = 'a' + i;
        }
    }

    printf("Arquivo: %s\n", param->nome_arquivo);
    printf("Número de palavras: %d\n", count_palavras);
    printf("Número de vogais: %d\n", count_vogal);
    printf("Número de consoantes: %d\n", count_consoante);
    printf("Palavra mais frequente: %s (%d vezes)\n", palavra_mais_freq, max_pcount);
    printf("Vogal mais frequente: %c (%d vezes)\n", vogal_mais_freq, max_vogal);
    printf("Consoante mais frequente: %c (%d vezes)\n\n", consoante_mais_freq, max_consoante);

    fclose(input);
    fclose(output);
    libera_tab_palavra();
    free(param);
    return NULL;
}

int main() {
    char path[MAX_PATH_LEN];
    printf("Digite o caminho do diretório dos arquivos .txt: ");
    fgets(path, sizeof(path), stdin);
    path[strcspn(path, "\n")] = '\0';

    DIR *dir = opendir(path);
    if (!dir) {
        perror("Erro ao abrir diretório");
        return 1;
    }

    struct dirent *entry;
    pthread_t threads[MAX_THREADS];
    int thread_count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".txt") != NULL &&
            strstr(entry->d_name, ".upper.txt") == NULL) {

            if (thread_count >= MAX_THREADS) {
                fprintf(stderr, "Limite de threads excedido!\n");
                break;
            }

            ParamThread *param = malloc(sizeof(ParamThread));
            strcpy(param->diretorio, path);
            strcpy(param->nome_arquivo, entry->d_name);
            pthread_create(&threads[thread_count], NULL, processa_arquivo_thread, param);
            thread_count++;
        }
    }

    for (int i = 0; i < thread_count; i++)
        pthread_join(threads[i], NULL);

    closedir(dir);
    return 0;
}
