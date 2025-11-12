#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Para tolower

// --- Estrutura para a Árvore do Mapa ---
typedef struct Sala {
    char nome[30];
    char pista[100];        // Pista específica da sala
    struct Sala *esquerda;  // Caminho à esquerda
    struct Sala *direita;   // Caminho à direita
} Sala;

// --- Estrutura para a BST de Pistas Coletadas ---
typedef struct PistaNode {
    char conteudo[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// --- Estrutura para a Tabela Hash (Pista -> Suspeito) ---
#define TAMANHO_HASH 13 // Número primo para melhor distribuição

typedef struct HashEntry {
    char pista[100];       // Chave (Pista)
    char suspeito[30];     // Valor (Suspeito)
    struct HashEntry *proximo; // Tratamento de colisão por encadeamento
} HashEntry;

HashEntry *tabelaHash[TAMANHO_HASH]; // A Tabela Hash principal

// Variável Global para a BST de Pistas Coletadas
PistaNode *raizPistas = NULL;
// Lista global para armazenar todas as pistas coletadas para a fase final
char pistasColetadas[30][100];
int numPistasColetadas = 0;

// Função hash simples: soma dos valores ASCII (ou 1º char) mod TAMANHO_HASH
unsigned int hashFunction(const char *key) {
    if (key == NULL || *key == '\0') return 0;
    return (unsigned int)tolower(key[0]) % TAMANHO_HASH; 
}

/**
 * @brief Insere uma associação Pista-Suspeito na Tabela Hash.
 * @note Requisito: inserirNaHash()
 */
void inserirNaHash(const char *pista, const char *suspeito) {
    unsigned int indice = hashFunction(pista);

    // 1. Cria nova entrada
    HashEntry *novaEntrada = (HashEntry*)malloc(sizeof(HashEntry));
    if (novaEntrada == NULL) {
        perror("Erro de alocação HashEntry");
        exit(1);
    }
    strncpy(novaEntrada->pista, pista, 99);
    novaEntrada->pista[99] = '\0';
    strncpy(novaEntrada->suspeito, suspeito, 29);
    novaEntrada->suspeito[29] = '\0';
    novaEntrada->proximo = NULL;

    // 2. Insere na lista encadeada (encadeamento)
    novaEntrada->proximo = tabelaHash[indice];
    tabelaHash[indice] = novaEntrada;
}

/**
 * @brief Encontra o suspeito associado a uma pista na Tabela Hash.
 * @note Requisito: encontrarSuspeito()
 * @return O nome do suspeito ou NULL se não for encontrado.
 */
const char* encontrarSuspeito(const char *pista) {
    unsigned int indice = hashFunction(pista);
    HashEntry *atual = tabelaHash[indice];

    // Busca na lista encadeada
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito; // Encontrado
        }
        atual = atual->proximo;
    }
    return NULL; // Não encontrado
}

PistaNode* criarPistaNode(const char *conteudoPista) {
    PistaNode *novo = (PistaNode*)malloc(sizeof(PistaNode));
    if (novo == NULL) {
        perror("Erro de alocação PistaNode");
        exit(1);
    }
    strncpy(novo->conteudo, conteudoPista, 99);
    novo->conteudo[99] = '\0';
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

/**
 * @brief Insere recursivamente uma nova pista na BST.
 * @note Requisito: inserirPista() / adicionarPista()
 */
PistaNode* inserirPistaBST(PistaNode *raiz, const char *conteudoPista) {
    if (raiz == NULL) {
        return criarPistaNode(conteudoPista);
    }

    int comparacao = strcmp(conteudoPista, raiz->conteudo);

    if (comparacao < 0) {
        raiz->esquerda = inserirPistaBST(raiz->esquerda, conteudoPista);
    } else if (comparacao > 0) {
        raiz->direita = inserirPistaBST(raiz->direita, conteudoPista);
    } 
    // Ignora pistas duplicadas

    return raiz;
}

/**
 * @brief Exibe as pistas coletadas em ordem alfabética (In-ordem).
 */
void exibirPistasBST(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistasBST(raiz->esquerda);
        printf("- %s\n", raiz->conteudo);
        exibirPistasBST(raiz->direita);
    }
}

/**
 * @brief Cria dinamicamente uma nova Sala (nó da árvore) com nome e pista.
 * @note Requisito: criarSala()
 */
Sala* criarSala(const char *nomeSala, const char *pistaConteudo) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        perror("Erro de alocação Sala");
        exit(1);
    }
    strncpy(novaSala->nome, nomeSala, 29);
    novaSala->nome[29] = '\0';
    strncpy(novaSala->pista, pistaConteudo, 99);
    novaSala->pista[99] = '\0';
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/**
 * @brief Controla a navegação e a coleta de pistas pelo jogador.
 * @note Requisito: explorarSalas()
 */
void explorarSalas(Sala *atual) {
    char escolha;
    
    printf("\n\n--- 🧭 Exploração da Mansão ---\n");
    
    while (1) {
        printf("\nVocê está no cômodo: **%s**\n", atual->nome);

        // Ação: Coleta de Pista
        if (strlen(atual->pista) > 0) {
            printf("✨ Pista Encontrada! Conteúdo: \"%s\"\n", atual->pista);
            
            // 1. Armazena na BST (Ordenação)
            raizPistas = inserirPistaBST(raizPistas, atual->pista);

            // 2. Armazena na lista global para a fase de julgamento
            if (numPistasColetadas < 30) {
                strncpy(pistasColetadas[numPistasColetadas], atual->pista, 99);
                pistasColetadas[numPistasColetadas][99] = '\0';
                numPistasColetadas++;
            }

            // 3. Verifica o suspeito associado
            const char* suspeito = encontrarSuspeito(atual->pista);
            if (suspeito != NULL) {
                printf("💡 Pista Relacionada ao Suspeito: %s\n", suspeito);
            } else {
                printf("❓ Esta pista ainda não foi relacionada a um suspeito na tabela hash.\n");
            }
            
            // Limpa a pista na sala para não ser coletada novamente
            atual->pista[0] = '\0';
        } else {
            printf("⚠️ Nenhuma pista restante neste cômodo.\n");
        }

        // --- Opções de Navegação ---
        printf("\nOnde ir?\n");
        if (atual->esquerda != NULL) printf("  [e] Esquerda (-> %s)\n", atual->esquerda->nome);
        if (atual->direita != NULL) printf("  [d] Direita (-> %s)\n", atual->direita->nome);
        printf("  [s] Sair e Iniciar Julgamento Final\n");
        printf("Escolha (e/d/s): ");

        if (scanf(" %c", &escolha) != 1) {
            printf("⚠️ Entrada inválida.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n'); // Limpa buffer

        // Lógica de navegação
        if (tolower(escolha) == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
                printf("✅ Caminho Esquerdo.\n");
            } else {
                printf("❌ Caminho indisponível.\n");
            }
        } else if (tolower(escolha) == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita;
                printf("✅ Caminho Direito.\n");
            } else {
                printf("❌ Caminho indisponível.\n");
            }
        } else if (tolower(escolha) == 's') {
            printf("\n🚪 **SAINDO**. Preparando o Julgamento...\n");
            break;
        } else {
            printf("⚠️ Escolha inválida.\n");
        }
    }
}

/**
 * @brief Conduz à fase final, avaliando a acusação do jogador.
 * @note Requisito: verificarSuspeitoFinal()
 */
void verificarSuspeitoFinal() {
    char acusado[30];
    int contagemPistas = 0;

    printf("\n======================================================\n");
    printf("               JULGAMENTO FINAL DO DETETIVE             \n");
    printf("======================================================\n");
    
    printf("\n--- Pistas Coletadas (Ordem Alfabética) ---\n");
    if (raizPistas == NULL) {
        printf("Nenhuma pista foi coletada. Não é possível fazer uma acusação.\n");
        return;
    }
    exibirPistasBST(raizPistas);
    printf("-------------------------------------------\n");

    // Solicita a acusação
    printf("\nAcusação: Quem é o culpado? (Sr. Smith, Sra. Brown, Mordomo James): ");
    if (scanf(" %29[^\n]", acusado) != 1) return;
    while (getchar() != '\n'); // Limpa buffer

    // Verifica quantas pistas apontam para o acusado
    for (int i = 0; i < numPistasColetadas; i++) {
        const char* suspeito = encontrarSuspeito(pistasColetadas[i]);
        if (suspeito != NULL && strcasecmp(suspeito, acusado) == 0) {
            contagemPistas++;
        }
    }

    printf("\n>>> Verificação das Evidências <<<\n");
    printf("Suspeito Acusado: %s\n", acusado);
    printf("Pistas que apoiam a acusação: %d\n", contagemPistas);

    // Requisito: Verificar se, pelo menos, duas pistas sustentam a acusação
    if (contagemPistas >= 2) {
        printf("\n🎉 SUCESSO! A acusação de %s é sustentada por %d pistas.\n", acusado, contagemPistas);
        printf("O mistério foi resolvido com base em evidências sólidas.\n");
    } else {
        printf("\n❌ FRACASSO! A acusação contra %s é fraca.\n", acusado);
        printf("Você precisa de pelo menos 2 pistas. O culpado escapou!\n");
    }
}

// Funções de Liberação de Memória (Boa Prática)
void liberarMansao(Sala *raiz) {
    if (raiz != NULL) {
        liberarMansao(raiz->esquerda);
        liberarMansao(raiz->direita);
        free(raiz);
    }
}
void liberarPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}
void liberarHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        HashEntry *atual = tabelaHash[i];
        HashEntry *temp;
        while (atual != NULL) {
            temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
}

int main() {
    // --- 1. Montagem da Tabela Hash (Pista -> Suspeito) ---
    printf("🛠️ Inicializando Sistema de Pistas e Suspeitos...\n");
    // Pistas Falsas (para Sr. Smith)
    inserirNaHash("O relógio parou às 3:15", "Sr. Smith"); 
    inserirNaHash("Havia poeira no piso", "Sr. Smith");
    
    // Pistas Verdadeiras (para Sra. Brown)
    inserirNaHash("O bilhete fala sobre um barco", "Sra. Brown"); 
    inserirNaHash("A carta estava rasgada", "Sra. Brown"); 
    inserirNaHash("O frasco continha veneno cianídrico", "Sra. Brown"); 
    
    // Pistas Neutras/Distração (para Mordomo James)
    inserirNaHash("As marcas de pneu são novas", "Mordomo James"); 
    inserirNaHash("Um anel de prata foi encontrado", "Mordomo James"); 
    
    printf("✅ Tabela Hash de Evidências Pronta.\n");

    // --- 2. Montagem da Árvore Binária (Mapa da Mansão) ---
    Sala *hall = criarSala("Hall de Entrada", "A carta estava rasgada"); 
    Sala *salaEstar = criarSala("Sala de Estar", "O relógio parou às 3:15");
    Sala *cozinha = criarSala("Cozinha", "Nenhuma pista relevante"); 
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    Sala *biblioteca = criarSala("Biblioteca", "Um anel de prata foi encontrado");
    Sala *dispensa = criarSala("Dispensa", "Havia poeira no piso");
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = dispensa;

    Sala *jardim = criarSala("Jardim", "As marcas de pneu são novas");
    Sala *porao = criarSala("Porão", "O frasco continha veneno cianídrico");
    cozinha->esquerda = jardim;
    cozinha->direita = porao;

    Sala *lab = criarSala("Laboratório Secreto", "O bilhete fala sobre um barco");
    biblioteca->esquerda = lab; 

    printf("=== 🕵️ DETECTIVE QUEST: Nível Mestre ===\n");
    
    // --- 3. Execução do Jogo ---
    explorarSalas(hall);

    // --- 4. Fase Final de Julgamento ---
    verificarSuspeitoFinal();

    // --- 5. Limpeza de Memória ---
    liberarMansao(hall);
    liberarPistas(raizPistas);
    liberarHash();
    printf("\nMemória do sistema liberada. Fim do programa.\n");

    return 0;
}