#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NOME 50
#define HASH_TAM 10

// ===============================================================
// 🌱 NÍVEL NOVATO — ÁRVORE BINÁRIA (MAPA DA MANSÃO)
// ===============================================================

typedef struct Sala {
    char nome[MAX_NOME];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

Sala* criarSala(char* nome) {
    Sala* s = (Sala*)malloc(sizeof(Sala));
    strcpy(s->nome, nome);
    s->esquerda = s->direita = NULL;
    return s;
}

void explorarSalas(Sala* atual) {
    char escolha;

    while (1) {
        printf("\nVocê está na sala: %s\n", atual->nome);
        printf("Ir para esquerda (e) | direita (d) | sair (s): ");
        scanf(" %c", &escolha);

        if (escolha == 's') {
            printf("\nExploração encerrada.\n");
            return;
        }
        else if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        }
        else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        }
        else {
            printf("Movimento inválido!\n");
        }
    }
}

// ===============================================================
// 🔍 NÍVEL AVENTUREIRO — ÁRVORE DE BUSCA (PISTAS)
// ===============================================================

typedef struct Pista {
    char texto[100];
    struct Pista* esquerda;
    struct Pista* direita;
} Pista;

Pista* criarPista(char* texto) {
    Pista* p = (Pista*)malloc(sizeof(Pista));
    strcpy(p->texto, texto);
    p->esquerda = p->direita = NULL;
    return p;
}

Pista* inserirBST(Pista* raiz, char* texto) {
    if (raiz == NULL) return criarPista(texto);

    if (strcmp(texto, raiz->texto) < 0)
        raiz->esquerda = inserirBST(raiz->esquerda, texto);
    else
        raiz->direita = inserirBST(raiz->direita, texto);

    return raiz;
}

void emOrdem(Pista* raiz) {
    if (raiz == NULL) return;
    emOrdem(raiz->esquerda);
    printf("- %s\n", raiz->texto);
    emOrdem(raiz->direita);
}

// ===============================================================
// 🧠 NÍVEL MESTRE — TABELA HASH (SUSPEITOS)
// ===============================================================

typedef struct NodoSuspeito {
    char nome[MAX_NOME];
    char pista[100];
    int contador;                
    struct NodoSuspeito* prox;
} NodoSuspeito;

NodoSuspeito* hashTabela[HASH_TAM];

// Hash simples pela primeira letra
int hash(char* nome) {
    return (nome[0] % HASH_TAM);
}

void inicializarHash() {
    for (int i = 0; i < HASH_TAM; i++)
        hashTabela[i] = NULL;
}

// Insere associação pista ↔ suspeito
void inserirHash(char* suspeito, char* pista) {
    int indice = hash(suspeito);

    NodoSuspeito* atual = hashTabela[indice];

    // verifica se suspeito já existe
    while (atual != NULL) {
        if (strcmp(atual->nome, suspeito) == 0) {
            atual->contador++;
            strcpy(atual->pista, pista);
            return;
        }
        atual = atual->prox;
    }

    // cria novo suspeito
    NodoSuspeito* novo = (NodoSuspeito*)malloc(sizeof(NodoSuspeito));
    strcpy(novo->nome, suspeito);
    strcpy(novo->pista, pista);
    novo->contador = 1;
    novo->prox = hashTabela[indice];
    hashTabela[indice] = novo;
}

void listarAssociacoes() {
    printf("\n=== Relação de Suspeitos e Pistas ===\n");
    for (int i = 0; i < HASH_TAM; i++) {
        NodoSuspeito* atual = hashTabela[i];
        while (atual != NULL) {
            printf("Suspeito: %s | Última pista: %s | Total de citações: %d\n",
                   atual->nome, atual->pista, atual->contador);
            atual = atual->prox;
        }
    }
}

void suspeitoMaisProvavel() {
    NodoSuspeito* mais = NULL;

    for (int i = 0; i < HASH_TAM; i++) {
        NodoSuspeito* atual = hashTabela[i];
        while (atual != NULL) {
            if (mais == NULL || atual->contador > mais->contador)
                mais = atual;
            atual = atual->prox;
        }
    }

    if (mais != NULL) {
        printf("\n💡 Suspeito mais provável: %s (mencionado %d vezes)\n",
               mais->nome, mais->contador);
    } else {
        printf("\nNenhum suspeito registrado.\n");
    }
}

// ===============================================================
// 🕵️ FUNÇÃO PRINCIPAL
// ===============================================================
int main() {
    // ----------------- Criando o mapa da mansão -----------------
    Sala* hall = criarSala("Hall de Entrada");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* cozinha = criarSala("Cozinha");
    Sala* sotao = criarSala("Sótão");

    hall->esquerda = biblioteca;
    hall->direita = cozinha;
    cozinha->esquerda = sotao;

    // árvore de pistas
    Pista* pistas = NULL;

    // inicializa hash
    inicializarHash();

    int opcao;

    do {
        printf("\n===== DETECTIVE QUEST =====\n");
        printf("1 - Explorar mansão\n");
        printf("2 - Adicionar pista manualmente\n");
        printf("3 - Listar pistas (em ordem)\n");
        printf("4 - Relacionar pista com suspeito\n");
        printf("5 - Listar suspeitos e pistas\n");
        printf("6 - Mostrar suspeito mais provável\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        if (opcao == 1) {
            explorarSalas(hall);
        }
        else if (opcao == 2) {
            char texto[100];
            printf("Digite a pista: ");
            fgets(texto, 100, stdin);
            texto[strcspn(texto, "\n")] = 0;
            pistas = inserirBST(pistas, texto);
        }
        else if (opcao == 3) {
            printf("\nPistas coletadas:\n");
            emOrdem(pistas);
        }
        else if (opcao == 4) {
            char suspeito[50], pista[100];
            printf("Suspeito: ");
            fgets(suspeito, 50, stdin);
            suspeito[strcspn(suspeito, "\n")] = 0;

            printf("Pista associada: ");
            fgets(pista, 100, stdin);
            pista[strcspn(pista, "\n")] = 0;

            inserirHash(suspeito, pista);
        }
        else if (opcao == 5) {
            listarAssociacoes();
        }
        else if (opcao == 6) {
            suspeitoMaisProvavel();
        }

    } while (opcao != 0);

    printf("\nEncerrando sistema...\n");

    return 0;
}
