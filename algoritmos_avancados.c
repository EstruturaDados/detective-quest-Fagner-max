#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Estrutura para o Mapa da Mansão (Árvore Binária Comum) ---
/**
 * @brief Representa um cômodo (Sala) no mapa.
 * Contém o nome e a pista encontrada no local.
 */
typedef struct Sala {
    char nome[30];
    char pista[100];        // Pista associada a este cômodo (pode ser vazia)
    struct Sala *esquerda;  // Caminho à esquerda
    struct Sala *direita;   // Caminho à direita
} Sala;

// --- Estrutura para a Árvore de Pistas (Árvore Binária de Busca - BST) ---
/**
 * @brief Representa um nó na BST de Pistas.
 * Armazena o conteúdo da pista e ponteiros para a organização.
 */
typedef struct PistaNode {
    char conteudo[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Ponteiro global para a raiz da BST de Pistas
PistaNode *raizPistas = NULL;

/**
 * @brief Cria dinamicamente um novo nó de pista (BST).
 * @param conteudoPista O texto da pista.
 * @return Um ponteiro para o novo PistaNode.
 */
PistaNode* criarPistaNode(const char *conteudoPista) {
    PistaNode *novo = (PistaNode*)malloc(sizeof(PistaNode));
    if (novo == NULL) {
        perror("Erro de alocação de memória para PistaNode");
        exit(1);
    }
    strncpy(novo->conteudo, conteudoPista, 99);
    novo->conteudo[99] = '\0';
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

/**
 * @brief Insere recursivamente uma nova pista na BST, mantendo a ordem alfabética.
 * @note Requisito: inserirPista()
 * @param raiz O ponteiro para a raiz (ou sub-raiz) atual da BST.
 * @param conteudoPista O texto da pista a ser inserida.
 * @return O novo ponteiro para a raiz após a inserção.
 */
PistaNode* inserirPista(PistaNode *raiz, const char *conteudoPista) {
    // 1. Caso base: Se a sub-árvore for nula, cria o nó e retorna
    if (raiz == NULL) {
        return criarPistaNode(conteudoPista);
    }

    // Compara alfabeticamente a nova pista com a pista do nó atual
    int comparacao = strcmp(conteudoPista, raiz->conteudo);

    if (comparacao < 0) {
        // Se a nova pista for 'menor' (alfabeticamente), vai para a esquerda
        raiz->esquerda = inserirPista(raiz->esquerda, conteudoPista);
    } else if (comparacao > 0) {
        // Se a nova pista for 'maior', vai para a direita
        raiz->direita = inserirPista(raiz->direita, conteudoPista);
    } else {
        // Se for igual, a pista já foi coletada.
        printf(" [Pista Duplicada Ignorada]\n");
    }

    // Retorna o ponteiro (raiz) sem alterações
    return raiz;
}

/**
 * @brief Exibe todas as pistas da BST em ordem alfabética (percurso In-ordem).
 * @note Requisito: exibirPistas()
 * @param raiz O ponteiro para a raiz (ou sub-raiz) atual da BST.
 */
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        // 1. Visita o filho esquerdo (Recursão)
        exibirPistas(raiz->esquerda);

        // 2. Visita o nó atual (Imprime a pista)
        printf("- %s\n", raiz->conteudo);

        // 3. Visita o filho direito (Recursão)
        exibirPistas(raiz->direita);
    }
}

/**
 * @brief Cria dinamicamente um novo cômodo (Sala) para o mapa.
 * @note Requisito: criarSala()
 * @param nomeSala O nome do cômodo.
 * @param conteudoPista A pista associada à sala (use "" se não houver).
 * @return Um ponteiro para a nova Sala alocada.
 */
Sala* criarSala(const char *nomeSala, const char *conteudoPista) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        perror("Erro de alocação de memória para Sala");
        exit(1);
    }

    // Inicializa a sala
    strncpy(novaSala->nome, nomeSala, 29);
    novaSala->nome[29] = '\0';
    strncpy(novaSala->pista, conteudoPista, 99);
    novaSala->pista[99] = '\0';
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

/**
 * @brief Permite a navegação e coleta de pistas pelo jogador.
 * @note Requisito: explorarSalasComPistas()
 * @param atual O ponteiro para a Sala atual.
 */
void explorarSalasComPistas(Sala *atual) {
    char escolha;
    
    printf("\n\n--- 🧭 Exploração da Mansão ---\n");
    
    // Loop de exploração contínua
    while (1) {
        printf("\nVocê está no cômodo: **%s**\n", atual->nome);

        // Verifica e coleta a pista
        if (strlen(atual->pista) > 0) {
            printf("✨ Pista Encontrada! Conteúdo: \"%s\"\n", atual->pista);
            // Adiciona a pista à BST
            raizPistas = inserirPista(raizPistas, atual->pista);
            // Limpa a pista na sala para não ser coletada novamente
            atual->pista[0] = '\0';
        } else {
            printf("⚠️ Nenhuma pista restante neste cômodo.\n");
        }

        // --- Opções de Navegação ---
        printf("\nOnde você gostaria de ir?\n");
        int temCaminho = 0;
        
        if (atual->esquerda != NULL) {
            printf("  [e] Esquerda (-> %s)\n", atual->esquerda->nome);
            temCaminho = 1;
        }
        if (atual->direita != NULL) {
            printf("  [d] Direita (-> %s)\n", atual->direita->nome);
            temCaminho = 1;
        }
        printf("  [s] Sair e Ver Pistas Coletadas\n");

        if (!temCaminho) {
            printf("🚫 Sem caminhos adicionais nesta sala. Sua única opção é sair [s].\n");
        }

        printf("Escolha (e/d/s): ");
        if (scanf(" %c", &escolha) != 1) {
            printf("⚠️ Entrada inválida.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n'); // Limpa buffer

        // Lógica de navegação
        if (escolha == 'e' || escolha == 'E') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
                printf("✅ Você seguiu para a Esquerda.\n");
            } else {
                printf("❌ Caminho indisponível. Escolha outra opção.\n");
            }
        } else if (escolha == 'd' || escolha == 'D') {
            if (atual->direita != NULL) {
                atual = atual->direita;
                printf("✅ Você seguiu para a Direita.\n");
            } else {
                printf("❌ Caminho indisponível. Escolha outra opção.\n");
            }
        } else if (escolha == 's' || escolha == 'S') {
            printf("\n🚪 **SAINDO**. Preparando relatório de pistas...\n");
            break;
        } else {
            printf("⚠️ Escolha inválida. Tente novamente.\n");
        }
    }
}

/**
 * @brief Libera a memória alocada para a árvore da mansão.
 */
void liberarMansao(Sala *raiz) {
    if (raiz != NULL) {
        liberarMansao(raiz->esquerda);
        liberarMansao(raiz->direita);
        free(raiz);
    }
}

/**
 * @brief Libera a memória alocada para a BST de pistas.
 */
void liberarPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

int main() {
    // 1. Montagem da Árvore Binária (Mapa da Mansão)
    // Raiz: Hall de Entrada
    Sala *hall = criarSala("Hall de Entrada", "A carta estava rasgada."); 

    // Nível 1
    Sala *salaEstar = criarSala("Sala de Estar", "O relógio parou às 3:15.");
    Sala *cozinha = criarSala("Cozinha", ""); // Sem pista aqui
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    // Nível 2
    Sala *biblioteca = criarSala("Biblioteca", "O livro chave tem capa verde.");
    Sala *dispensa = criarSala("Dispensa", "Havia poeira no piso.");
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = dispensa;

    Sala *jardim = criarSala("Jardim", "As marcas de pneu são novas.");
    Sala *porao = criarSala("Porão", "Um anel de prata foi encontrado.");
    cozinha->esquerda = jardim;
    cozinha->direita = porao;

    // Nível 3 (Nós-Folha ou caminhos sem pistas)
    Sala *lab = criarSala("Laboratório Secreto", "O frasco continha veneno cianídrico.");
    biblioteca->esquerda = lab; 
    
    Sala *salaDeJogos = criarSala("Sala de Jogos", "O bilhete fala sobre um barco.");
    jardim->direita = salaDeJogos;
    
    // Dispensa, Laboratório e Porão são nós-folha ou têm caminhos NULL.

    printf("=== 🕵️ DETECTIVE QUEST: Sistema de Coleta de Pistas ===\n");
    printf("Explore a mansão e colete as pistas. Elas serão organizadas automaticamente.\n");
    
    // 2. Início da Exploração e Coleta
    explorarSalasComPistas(hall);

    // 3. Exibição Final das Pistas Coletadas (BST In-ordem)
    printf("\n======================================================\n");
    printf("               RELATÓRIO FINAL DE PISTAS               \n");
    printf("         (Organizadas em Ordem Alfabética)            \n");
    printf("======================================================\n");

    if (raizPistas == NULL) {
        printf("Nenhuma pista foi coletada durante a exploração.\n");
    } else {
        exibirPistas(raizPistas);
    }

    printf("======================================================\n");

    // 4. Limpeza de Memória
    liberarMansao(hall);
    liberarPistas(raizPistas);
    printf("\nMemória das estruturas liberada. Fim do programa.\n");

    return 0;
}