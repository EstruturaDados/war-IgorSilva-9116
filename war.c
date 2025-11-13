// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

#include <stdio.h>    // Funções de entrada e saída (printf, scanf)
#include <stdlib.h>   // Funções para alocação dinâmica (malloc, calloc, free)
#include <string.h>   // Funções para manipulação de strings (strcmp, strcpy)
#include <time.h>     // Funções para gerar números aleatórios (rand, srand)

// Estrutura que representa um território no jogo
typedef struct {
    char nome[30];    // Nome do território
    char cor[10];     // Cor do exército que controla o território
    int tropas;       // Quantidade de tropas no território
} Territorio;

// ---------------- Funções Utilitárias ----------------

// Limpa o buffer de entrada para evitar problemas com scanf
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Descarta caracteres até encontrar '\n'
}

// ---------------- Funções Principais ----------------

// Aloca dinamicamente memória para o vetor de territórios
Territorio* alocarMapa(int qtd) {
    Territorio *mapa = (Territorio *)calloc(qtd, sizeof(Territorio)); // calloc inicializa com zeros
    if (mapa == NULL) { // Verifica se a alocação falhou
        printf("Erro ao alocar memória!\n");
        exit(1); // Encerra o programa em caso de erro
    }
    return mapa; // Retorna ponteiro para o vetor alocado
}

// Inicializa os territórios com dados fornecidos pelo usuário
void inicializarTerritorios(Territorio *mapa, int qtd) {
    for (int i = 0; i < qtd; i++) {
        printf("\nCadastro do território %d:\n", i + 1);
        printf("Digite o nome do território: ");
        scanf(" %[^\n]", mapa[i].nome); // Lê nome com espaços
        printf("Digite a cor do exército: ");
        scanf(" %[^\n]", mapa[i].cor);  // Lê cor com espaços
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
        if (mapa[i].tropas < 1) mapa[i].tropas = 1; // Garante pelo menos 1 tropa
    }
}

// Libera memória alocada para mapa e missão
void liberarMemoria(Territorio *mapa, char *missaoJogador) {
    free(mapa);          // Libera vetor de territórios
    free(missaoJogador); // Libera string da missão
}

// Exibe o menu principal com as opções do jogo
void exibirMenuPrincipal() {
    printf("\n=== MENU PRINCIPAL ===\n");
    printf("1 - Exibir mapa\n");
    printf("2 - Atacar\n");
    printf("3 - Mostrar missão\n");
    printf("0 - Sair\n");
}

// Exibe o estado atual do mapa (somente leitura)
void exibirMapa(const Territorio *mapa, int qtd) {
    printf("\n=== Cenário Atual ===\n");
    for (int i = 0; i < qtd; i++) {
        printf("%d - Nome: %s | Cor: %s | Tropas: %d\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Exibe a missão atual do jogador
void exibirMissao(const char *missao) {
    printf("\nSua missão: %s\n", missao);
}

// Sorteia uma missão aleatória e copia para a variável do jogador
void sortearMissao(char *destino, char *missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes; // Gera índice aleatório
    strcpy(destino, missoes[sorteio]);   // Copia missão para destino
}

// Simula um ataque entre dois territórios
void simularAtaque(Territorio *atacante, Territorio *defensor) {
    // Validações para impedir ataques inválidos
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\nNão é permitido atacar um território da mesma cor!\n");
        return;
    }
    if (atacante == defensor) {
        printf("\nNão é permitido atacar o próprio território!\n");
        return;
    }
    if (atacante->tropas < 2) {
        printf("\nO atacante precisa ter pelo menos 2 tropas para atacar!\n");
        return;
    }

    // Exibe informações da batalha
    printf("\n--- Batalha ---\n");
    printf("%s (Cor: %s, Tropas: %d) ATACA %s (Cor: %s, Tropas: %d)\n",
           atacante->nome, atacante->cor, atacante->tropas,
           defensor->nome, defensor->cor, defensor->tropas);

    // Rolagem de dados (1 a 6)
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("Dado atacante: %d | Dado defensor: %d\n", dadoAtacante, dadoDefensor);

    // Atualiza territórios conforme resultado
    if (dadoAtacante > dadoDefensor) {
        printf("Resultado: O atacante venceu!\n");
        strcpy(defensor->cor, atacante->cor); // Muda dono do território
        defensor->tropas = atacante->tropas / 2; // Transfere metade das tropas
        atacante->tropas /= 2; // Atacante perde metade ao enviar tropas
    } else {
        printf("Resultado: O defensor resistiu! O atacante perde uma tropa.\n");
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0; // Evita valor negativo
    }
}

// Gerencia a fase de ataque (interface com jogador)
void faseDeAtaque(Territorio *mapa, int qtd) {
    int idxAtacante, idxDefensor;
    printf("\nEscolha um território atacante (1-%d): ", qtd);
    scanf("%d", &idxAtacante);
    printf("Escolha um território defensor (1-%d): ", qtd);
    scanf("%d", &idxDefensor);

    if (idxAtacante >= 1 && idxAtacante <= qtd && idxDefensor >= 1 && idxDefensor <= qtd) {
        simularAtaque(&mapa[idxAtacante - 1], &mapa[idxDefensor - 1]);
    } else {
        printf("Índices inválidos!\n");
    }
}

// Verifica se a missão do jogador foi cumprida
int verificarVitoria(const char *missao, const Territorio *mapa, int qtd, const char *corJogador) {
    // Missão: Conquistar 3 territórios
    if (strstr(missao, "Conquistar 3 territórios")) {
        int contador = 0;
        for (int i = 0; i < qtd; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) contador++;
        }
        return (contador >= 3);
    }
    // Missão: Eliminar cor vermelha
    if (strstr(missao, "Eliminar cor vermelha")) {
        for (int i = 0; i < qtd; i++) {
            if (strcmp(mapa[i].cor, "vermelha") == 0) return 0;
        }
        return 1;
    }
    return 0; // Caso não cumpra nenhuma condição
}

// ---------------- Função Principal ----------------
int main() {
    srand(time(NULL)); // Inicializa gerador de números aleatórios

    // Vetor de missões disponíveis
    char *missoes[] = {
        "Conquistar 3 territórios",
        "Eliminar cor vermelha",
        "Controlar todos os territórios azuis",
        "Manter pelo menos 10 tropas em um território",
        "Conquistar dois territórios consecutivos"
    };
    int totalMissoes = 5;

    // Solicita quantidade de territórios
    int qtd;
    printf("Informe a quantidade de territórios: ");
    scanf("%d", &qtd);

    // Aloca mapa dinamicamente
    Territorio *mapa = alocarMapa(qtd);

    // Inicializa territórios
    inicializarTerritorios(mapa, qtd);

    // Sorteia missão para jogador
    char *missaoJogador = (char *)malloc(100 * sizeof(char));
    sortearMissao(missaoJogador, missoes, totalMissoes);

    // Solicita cor do jogador
    char corJogador[10];
    printf("\nInforme sua cor de exército: ");
    scanf(" %[^\n]", corJogador);

    // Exibe missão secreta
    printf("\nSua missão secreta: %s\n", missaoJogador);

    // Loop principal do jogo
    int opcao;
    do {
        exibirMenuPrincipal();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                exibirMapa(mapa, qtd);
                break;
            case 2:
                faseDeAtaque(mapa, qtd);
                exibirMapa(mapa, qtd);
                if (verificarVitoria(missaoJogador, mapa, qtd, corJogador)) {
                    printf("\n>>> Missão cumprida! Você venceu!\n");
                    opcao = 0; // Encerra jogo
                }
                break;
            case 3:
                exibirMissao(missaoJogador);
                break;
            case 0:
                printf("\nEncerrando o jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    // Libera memória antes de sair
    liberarMemoria(mapa, missaoJogador);
    return 0;
}


// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
