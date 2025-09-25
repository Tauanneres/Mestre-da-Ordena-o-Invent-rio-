#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 20

typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
} Componente;

// Protótipos
void menu();
void adicionar(Componente v[], int *n);
void listar(const Componente v[], int n);
void limparEntrada();

long long bubbleNome(Componente v[], int n);
long long insertionTipo(Componente v[], int n);
long long selectionPrioridade(Componente v[], int n);

int buscaBinaria(const Componente v[], int n, const char* chave, long long *comp);

int main() {
    Componente inventario[MAX];
    int total = 0, opcao, ordenadoNome = 0;

    do {
        menu();
        scanf("%d", &opcao);
        limparEntrada();
        system("clear");

        switch(opcao) {
            case 1:
                adicionar(inventario, &total);
                ordenadoNome = 0;
                break;
            case 2: case 3: case 4: {
                if (total == 0) {
                    printf("Inventario vazio.\n");
                    break;
                }
                long long comp = 0;
                clock_t ini = clock();

                if (opcao == 2) {
                    comp = bubbleNome(inventario, total);
                    ordenadoNome = 1;
                } else if (opcao == 3) {
                    comp = insertionTipo(inventario, total);
                    ordenadoNome = 0;
                } else {
                    comp = selectionPrioridade(inventario, total);
                    ordenadoNome = 0;
                }

                clock_t fim = clock();
                double tempo = (double)(fim - ini) / CLOCKS_PER_SEC;

                printf("Comparacoes: %lld | Tempo: %.6f s\n", comp, tempo);
                listar(inventario, total);
                break;
            }
            case 5: {
                if (!ordenadoNome) {
                    printf("Ordene por nome antes de buscar (opcao 2).\n");
                    break;
                }
                char chave[30];
                printf("Nome: ");
                fgets(chave, sizeof(chave), stdin);
                chave[strcspn(chave, "\n")] = 0;

                long long compBusca = 0;
                int pos = buscaBinaria(inventario, total, chave, &compBusca);

                if (pos != -1)
                    printf("Encontrado no indice %d.\n", pos);
                else
                    printf("Nao encontrado.\n");

                printf("Comparacoes: %lld\n", compBusca);
                break;
            }
            case 6:
                listar(inventario, total);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}

void menu() {
    printf("\n1. Adicionar\n");
    printf("2. Ordenar por nome (Bubble)\n");
    printf("3. Ordenar por tipo (Insertion)\n");
    printf("4. Ordenar por prioridade (Selection)\n");
    printf("5. Buscar por nome (binaria)\n");
    printf("6. Listar\n");
    printf("0. Sair\n");
    printf("Opcao: ");
}

void adicionar(Componente v[], int *n) {
    if (*n >= MAX) {
        printf("Inventario cheio.\n");
        return;
    }
    printf("Nome: ");
    fgets(v[*n].nome, sizeof(v[*n].nome), stdin);
    v[*n].nome[strcspn(v[*n].nome, "\n")] = 0;

    printf("Tipo: ");
    fgets(v[*n].tipo, sizeof(v[*n].tipo), stdin);
    v[*n].tipo[strcspn(v[*n].tipo, "\n")] = 0;

    printf("Prioridade: ");
    scanf("%d", &v[*n].prioridade);
    limparEntrada();

    (*n)++;
}

void listar(const Componente v[], int n) {
    for (int i = 0; i < n; i++)
        printf("%s | %s | %d\n", v[i].nome, v[i].tipo, v[i].prioridade);
}

void limparEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void troca(Componente *a, Componente *b) {
    Componente tmp = *a;
    *a = *b;
    *b = tmp;
}

long long bubbleNome(Componente v[], int n) {
    long long comp = 0;
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++) {
            comp++;
            if (strcmp(v[j].nome, v[j+1].nome) > 0)
                troca(&v[j], &v[j+1]);
        }
    return comp;
}

long long insertionTipo(Componente v[], int n) {
    long long comp = 0;
    for (int i = 1; i < n; i++) {
        Componente chave = v[i];
        int j = i - 1;
        while (j >= 0) {
            comp++;
            if (strcmp(v[j].tipo, chave.tipo) > 0) {
                v[j+1] = v[j];
                j--;
            } else break;
        }
        v[j+1] = chave;
    }
    return comp;
}

long long selectionPrioridade(Componente v[], int n) {
    long long comp = 0;
    for (int i = 0; i < n-1; i++) {
        int max = i;
        for (int j = i+1; j < n; j++) {
            comp++;
            if (v[j].prioridade > v[max].prioridade)
                max = j;
        }
        if (max != i) troca(&v[i], &v[max]);
    }
    return comp;
}

int buscaBinaria(const Componente v[], int n, const char* chave, long long *comp) {
    int esq = 0, dir = n - 1;
    *comp = 0;
    while (esq <= dir) {
        (*comp)++;
        int meio = (esq + dir) / 2;
        int r = strcmp(chave, v[meio].nome);
        if (r == 0) return meio;
        else if (r > 0) esq = meio + 1;
        else dir = meio - 1;
    }
    return -1;
}
