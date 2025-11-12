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