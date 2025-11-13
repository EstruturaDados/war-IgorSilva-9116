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

#include <stdio.h>    // Biblioteca para entrada e saída (printf, scanf)
#include <stdlib.h>   // Biblioteca para alocação dinâmica (malloc, calloc, free)
#include <string.h>   // Biblioteca para manipulação de strings (strcmp, strcpy)
#include <time.h>     // Biblioteca para gerar números aleatórios (rand, srand)

// Estrutura que representa um território no jogo
typedef struct {
    char nome[30];    // Nome do território
    char cor[10];     // Cor do exército que controla o território
    int tropas;       // Quantidade de tropas no território
} Territorio;

/* 
 * Função: cadastrarTerritorios
 * Objetivo: Permite ao usuário cadastrar os territórios, preenchendo nome, cor e tropas.
 * Parâmetros:
 *   - mapa: ponteiro para o vetor de territórios
 *   - qtd: quantidade de territórios
 */
void cadastrarTerritorios(Territorio *mapa, int qtd) {
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

/* 
 * Função: exibirTerritorios
 * Objetivo: Exibe todos os territórios cadastrados com seus dados atuais.
 * Parâmetros:
 *   - mapa: ponteiro para o vetor de territórios
 *   - qtd: quantidade de territórios
 */
void exibirTerritorios(Territorio *mapa, int qtd) {
    printf("\n=== Cenário Atual ===\n");
    for (int i = 0; i < qtd; i++) {
        printf("%d - Nome: %s | Cor: %s | Tropas: %d\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

/* 
 * Função: atacar
 * Objetivo: Simula uma batalha entre dois territórios usando rolagem de dados.
 * Parâmetros:
 *   - atacante: ponteiro para o território que está atacando
 *   - defensor: ponteiro para o território que está sendo atacado
 * Lógica:
 *   - Valida se ataque é permitido (não pode atacar aliado ou a si mesmo)
 *   - Gera dois números aleatórios (dados) para decidir o vencedor
 *   - Atualiza cor e tropas do defensor se atacante vencer
 *   - Se perder, atacante perde uma tropa
 */
void atacar(Territorio *atacante, Territorio *defensor) {
    // Validações
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

    // Resultado da batalha
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

    printf("\n>>> Cenário após a batalha:\n");
}

/* 
 * Função: liberarMemoria
 * Objetivo: Libera a memória alocada dinamicamente para os territórios.
 * Parâmetros:
 *   - mapa: ponteiro para o vetor de territórios
 */
void liberarMemoria(Territorio *mapa) {
    free(mapa);
}

/* 
 * Função principal (main)
 * Objetivo: Controla o fluxo do programa:
 *   - Solicita quantidade de territórios
 *   - Aloca memória dinamicamente
 *   - Cadastra territórios
 *   - Permite ataques até o usuário decidir parar
 *   - Exibe cenário atualizado após cada ataque
 *   - Libera memória antes de encerrar
 */
int main() {
    srand(time(NULL)); // Inicializa aleatoriedade

    int qtd;
    printf("Informe a quantidade de territórios: ");
    scanf("%d", &qtd);

    // Alocação dinâmica do vetor de territórios
    Territorio *mapa = (Territorio *)calloc(qtd, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    // Cadastro inicial
    cadastrarTerritorios(mapa, qtd);
    exibirTerritorios(mapa, qtd);

    int opcao;
    do {
        int idxAtacante, idxDefensor;
        printf("\nEscolha um território atacante (1-%d): ", qtd);
        scanf("%d", &idxAtacante);
        printf("Escolha um território defensor (1-%d): ", qtd);
        scanf("%d", &idxDefensor);

        if (idxAtacante >= 1 && idxAtacante <= qtd && idxDefensor >= 1 && idxDefensor <= qtd) {
            atacar(&mapa[idxAtacante - 1], &mapa[idxDefensor - 1]);
            exibirTerritorios(mapa, qtd);
        } else {
            printf("Índices inválidos!\n");
        }

        printf("\nDeseja realizar outro ataque? (1 - Sim / 0 - Não): ");
        scanf("%d", &opcao);
    } while (opcao == 1);

    liberarMemoria(mapa);
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
