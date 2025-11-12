#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Definição da Struct (Nó) ---
/**
 * @brief Estrutura que representa um cômodo (Sala) no mapa da mansão.
 *
 * É o nó da árvore binária, contendo o nome da sala e ponteiros para os caminhos.
 */
typedef struct Sala {
    char nome[30];          // Nome do cômodo
    struct Sala *esquerda;  // Caminho à esquerda (filho esquerdo)
    struct Sala *direita;   // Caminho à direita (filho direito)
} Sala;


// --- Funções de Criação e Manipulação ---

/**
 * @brief Cria dinamicamente uma nova Sala (nó da árvore) com o nome fornecido.
 *
 * @param nomeSala O nome a ser atribuído à nova sala.
 * @return Um ponteiro para a nova Sala alocada ou NULL se falhar.
 */
Sala* criarSala(const char *nomeSala) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala)); // Alocação dinâmica

    if (novaSala == NULL) {
        printf("⚠️ Erro de alocação de memória para a sala: %s\n", nomeSala);
        exit(1);
    }

    // Inicializa a sala
    strncpy(novaSala->nome, nomeSala, 29); // Copia o nome
    novaSala->nome[29] = '\0';             // Garante o terminador nulo
    novaSala->esquerda = NULL;             // Inicialmente, não há caminhos
    novaSala->direita = NULL;

    return novaSala;
}

/**
 * @brief Permite a exploração interativa da mansão pelo jogador.
 *
 * O jogador navega pela árvore binária (mapa) até atingir um nó-folha ou sair.
 *
 * @param atual O ponteiro para a Sala atual (nó da árvore).
 */
void explorarSalas(Sala *atual) {
    char escolha;

    // A exploração começa no Hall de entrada
    printf("\n\n--- 🧭 Explorando a Mansão ---\n");
    printf("Você está no cômodo: **%s**\n", atual->nome);

    while (1) {
        // Verifica as opções de saída (nó-folha)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("\n🎉 Você chegou a um ponto final (nó-folha)! Este cômodo não tem mais caminhos.\n");
            printf("Fim da exploração.\n");
            break;
        }

        printf("\nOnde você gostaria de ir a partir de **%s**?\n", atual->nome);

        // Guia de opções
        if (atual->esquerda != NULL) {
            printf("  [e] Esquerda (-> %s)\n", atual->esquerda->nome);
        }
        if (atual->direita != NULL) {
            printf("  [d] Direita (-> %s)\n", atual->direita->nome);
        }
        printf("  [s] Sair da exploração\n");
        printf("Escolha (e/d/s): ");

        // Limpa o buffer antes de ler a escolha
        while (scanf(" %c", &escolha) != 1) {
            printf("⚠️ Entrada inválida. Tente novamente: ");
            while (getchar() != '\n');
        }
        while (getchar() != '\n'); // Limpa o restante da linha

        // Lógica de navegação
        if (escolha == 'e' || escolha == 'E') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
                printf("✅ Você seguiu para a Esquerda. Novo cômodo: **%s**\n", atual->nome);
            } else {
                printf("❌ Caminho à esquerda não existe a partir de **%s**.\n", atual->nome);
            }
        } else if (escolha == 'd' || escolha == 'D') {
            if (atual->direita != NULL) {
                atual = atual->direita;
                printf("✅ Você seguiu para a Direita. Novo cômodo: **%s**\n", atual->nome);
            } else {
                printf("❌ Caminho à direita não existe a partir de **%s**.\n", atual->nome);
            }
        } else if (escolha == 's' || escolha == 'S') {
            printf("\n🚪 Você decidiu sair da exploração.\n");
            break;
        } else {
            printf("⚠️ Escolha inválida. Use 'e', 'd' ou 's'.\n");
        }
    }
}

/**
 * @brief Libera a memória alocada para a árvore (boa prática).
 *
 * @param raiz O nó raiz (ou sub-raiz) a ser liberado.
 */
void liberarArvore(Sala *raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}


// --- Função Principal: Montagem do Mapa e Início ---

int main() {
    // 1. Montagem da Árvore Binária (Mapa da Mansão)
    // O(a Raiz) é o Hall de Entrada
    Sala *hall = criarSala("Hall de Entrada"); 

    // Nível 1
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    // Nível 2
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *dispensa = criarSala("Dispensa");
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = dispensa;

    Sala *jardim = criarSala("Jardim");
    Sala *porao = criarSala("Porão");
    cozinha->esquerda = jardim;
    cozinha->direita = porao;

    // Nível 3 (Nós-Folha para testar o fim do caminho)
    // Biblioteca->esquerda e ->direita são NULL (nó-folha)
    // Dispensa->esquerda e ->direita são NULL (nó-folha)
    Sala *lab = criarSala("Laboratório Secreto");
    jardim->esquerda = lab; 
    // jardim->direita é NULL (nó-folha)

    // porao->esquerda e ->direita são NULL (nó-folha)

    printf("=== 🕵️ DETECTIVE QUEST: Mapa da Mansão (Árvore Binária) ===\n");
    printf("O mapa da mansão foi criado automaticamente. Prepare-se para a exploração!\n");
    
    // 2. Início da Exploração
    explorarSalas(hall);

    // 3. Limpeza de Memória
    liberarArvore(hall);
    printf("\nMemória da mansão liberada. Fim do programa.\n");

    return 0;
}