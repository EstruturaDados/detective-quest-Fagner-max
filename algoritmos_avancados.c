#include <stdio.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura para um cômodo (nó da árvore binária)
typedef struct Sala {
    char nome[50];         // Nome do cômodo
    struct Sala *esquerda; // Caminho para a sala à esquerda (filho esquerdo)
    struct Sala *direita;  // Caminho para a sala à direita (filho direito)
} Sala;

/**
 * @brief Cria uma nova sala com alocação dinâmica.
 * * @param nome O nome do cômodo a ser criado.
 * @return Um ponteiro para a nova Sala criada.
 */
Sala* criarSala(const char *nome) {
    // Aloca dinamicamente memória para a nova estrutura Sala
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    
    // Verifica se a alocação foi bem-sucedida
    if (novaSala == NULL) {
        printf("Erro na alocação de memória.\n");
        exit(1); // Sai do programa em caso de falha crítica
    }
    
    // Copia o nome para o campo 'nome' da estrutura
    strcpy(novaSala->nome, nome);
    
    // Inicializa os ponteiros dos filhos como NULL (sem caminhos)
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/**
 * @brief Permite ao jogador navegar pela mansão (árvore binária).
 * * A exploração continua até que o jogador chegue a um nó-folha (sem caminhos).
 * * @param hall O ponteiro para o nó inicial (Hall de entrada/raiz).
 */
void explorarSalas(Sala *hall) {
    Sala *salaAtual = hall;
    char escolha;

    printf("╔═════════════════════════════════╗\n");
    printf("║  BEM-VINDO(A) AO DETECTIVE QUEST  ║\n");
    printf("╚═════════════════════════════════╝\n");
    printf("Explore a mansão a partir do Hall de Entrada.\n");
    printf("\n");

    // Loop de exploração: continua enquanto houver caminhos
    while (salaAtual != NULL) {
        printf("Você está em: **%s**\n", salaAtual->nome);
        
        // Verifica se é um nó-folha (fim da linha de exploração)
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("\n");
            printf("----------------------------------------------------\n");
            printf("Você chegou a um beco sem saída. A exploração terminou.\n");
            printf("----------------------------------------------------\n");
            return; // Encerra a função, pois não há mais caminhos
        }

        printf("Escolha o próximo caminho:\n");
        
        // Exibe opções de acordo com os caminhos disponíveis
        if (salaAtual->esquerda != NULL) {
            printf("  [e] Esquerda (-> %s)\n", salaAtual->esquerda->nome);
        }
        if (salaAtual->direita != NULL) {
            printf("  [d] Direita (-> %s)\n", salaAtual->direita->nome);
        }
        printf("  [s] Sair da mansão\n");
        
        printf("> Sua escolha: ");
        scanf(" %c", &escolha); // O espaço antes de %c é importante para ignorar espaços/quebras de linha

        switch (escolha) {
            case 'e':
            case 'E':
                if (salaAtual->esquerda != NULL) {
                    salaAtual = salaAtual->esquerda;
                } else {
                    printf("❌ Não há caminho para a esquerda a partir daqui.\n");
                }
                break;
            case 'd':
            case 'D':
                if (salaAtual->direita != NULL) {
                    salaAtual = salaAtual->direita;
                } else {
                    printf("❌ Não há caminho para a direita a partir daqui.\n");
                }
                break;
            case 's':
            case 'S':
                printf("\n");
                printf("👋 Você optou por sair da mansão. Até a próxima!\n");
                return; // Encerra o jogo
            default:
                printf("⁉️ Opção inválida. Tente novamente.\n");
                break;
        }
        printf("\n"); // Adiciona uma quebra de linha para clareza
    }
}

/**
 * @brief Função principal: monta o mapa da mansão e inicia a exploração.
 */
int main() {
    // 1. MONTANDO O MAPA (A Árvore Binária)

    // Nível 0: Raiz
    Sala *hall = criarSala("Hall de Entrada");

    // Nível 1
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *jantar = criarSala("Sala de Jantar");
    hall->esquerda = salaEstar;
    hall->direita = jantar;

    // Nível 2
    Sala *cozinha = criarSala("Cozinha");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *patio = criarSala("Patio Central");
    
    salaEstar->esquerda = cozinha; // Cozinha tem caminhos para os fundos
    salaEstar->direita = NULL;     // Sem caminho à direita da Sala de Estar
    
    jantar->esquerda = biblioteca; // Biblioteca tem caminhos
    jantar->direita = patio;       // Pátio é um beco sem saída
    
    // Nível 3 - Beco sem saída (Folhas)
    Sala *jardim = criarSala("Jardim dos Fundos");
    
    cozinha->esquerda = jardim;    // Jardim dos Fundos é um beco sem saída
    cozinha->direita = NULL;       // Sem caminho à direita da Cozinha
    
    // A Biblioteca leva de volta para a Sala de Estar ou para um Quarto (simplificação da estrutura)
    Sala *quarto1 = criarSala("Quarto de Hospedes"); // Beco sem saída
    
    biblioteca->esquerda = quarto1;
    biblioteca->direita = NULL;

    // A partir daqui, todas as salas seguintes são becos sem saída (nós-folha).

    // 2. INICIANDO A EXPLORAÇÃO
    explorarSalas(hall);
    
    // 3. LIBERAÇÃO DE MEMÓRIA (Boa prática, embora a alocação dinâmica não seja o foco)
    // Para simplificar para o nível novato, a liberação de memória pode ser omitida,
    // mas em código real, ela seria feita via uma função recursiva (e.g., freeTree(hall)).
    
    // Como simplificação, confiaremos que o SO irá liberar a memória ao fim do programa.
    
    return 0;
}