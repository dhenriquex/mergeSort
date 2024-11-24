#include <stdio.h>      // Biblioteca padrão de entrada e saída
#include <stdlib.h>     // Biblioteca para alocação dinâmica e funções gerais
#include <string.h>     // Para manipulação de strings e memória
#include <limits.h>     // Para valores extremos como INT_MAX
#include <time.h>       // Para geração de números aleatórios
#include <sys/time.h>   // Para medir o tempo de execução
#include <sys/resource.h> // Para obter informações sobre o uso de recursos

//====================================================
// Trabalho de EDII                         (24/11/24)
// Código executado no VSCode
// Compilados no Fedora 41 e Ubuntu 24.4
// Código baseado em artigos
// Feito por: Davi, Rafael e Robson 
//====================================================

// contantes--------------------------------------------
// Define o valor máximo para os números gerados aleatoriamente
#define MAX_VALOR 100
// Define o valor padrão de K
#define K 5

// funções auxiliares-----------------------------------
// Função para calcular e registrar o tempo de execução
void tempoExecucao(FILE *saida, struct rusage inicio, struct rusage fim) {
    // Calcula o tempo de CPU no modo usuário antes e depois
    double tempo_usuario_inicio = inicio.ru_utime.tv_sec + inicio.ru_utime.tv_usec / 1e6;
    double tempo_usuario_fim = fim.ru_utime.tv_sec + fim.ru_utime.tv_usec / 1e6;
    // Calcula o tempo de CPU no modo sistema antes e depois
    double tempo_sistema_inicio = inicio.ru_stime.tv_sec + inicio.ru_stime.tv_usec / 1e6;
    double tempo_sistema_fim = fim.ru_stime.tv_sec + fim.ru_stime.tv_usec / 1e6;
    // Tempo total em segundos
    double tempo_total = (tempo_usuario_fim - tempo_usuario_inicio) + (tempo_sistema_fim - tempo_sistema_inicio);
    // Escreve os dados no arquivo
    fprintf(saida, "%.6fs\n", tempo_total);
}

// Gera números aleatórios para preencher o vetor
void gerarVetorAleatorio(int vetor[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = rand() % (MAX_VALOR + 1); // Gera números de 0 a MAX_VALOR
    }
}
// Exibe os elementos de um vetor
void exibirVetor(int vetor[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
}

// Mescla os vetores-----------------------------
void mesclar(int *vetor, int inicio, int meio, int fim, int *temp) {
    int i = inicio, j = meio, k = inicio;
    // Mescla os subvetores enquanto houver elementos em ambos
    while (i < meio && j < fim) {
        if (vetor[i] <= vetor[j]) {
            temp[k++] = vetor[i++];
        } else {
            temp[k++] = vetor[j++];
        }
    }
    // Copia os elementos restantes do primeiro subvetor
    while (i < meio) {
        temp[k++] = vetor[i++];
    }
    // Copia os elementos restantes do segundo subvetor
    while (j < fim) {
        temp[k++] = vetor[j++];
    }
    // Copia o resultado de volta para o vetor original
    memcpy(vetor + inicio, temp + inicio, (fim - inicio) * sizeof(int));
}

// Função recursiva para dividir e ordenar usando K partições
void ordenarKParticoes(int *vetor, int inicio, int fim, int k, int *temp) {
    // Calcula o tamanho do intervalo a ser ordenado
    int tamanho = fim - inicio;
    if (tamanho <= 1) return;
    // Divide o vetor em K partições
    int tamanhos[k + 1];
    tamanhos[0] = inicio;
    // Loop para calcular os índices de início e fim de cada partição
    for (int i = 1; i <= k; i++) {
        // Define o início da partição i com base no tamanho dividido igualmente 
        // Adiciona 1 ao tamanho se ainda houver restos ao dividir igualmente (balanceamento)
        tamanhos[i] = tamanhos[i - 1] + tamanho / k + (i <= (tamanho % k));
    }
    // Ordena recursivamente cada partição
    for (int i = 0; i < k; i++) {
        // Chama a função recursivamente para ordenar a partição de tamanhos[i] a tamanhos[i + 1
        ordenarKParticoes(vetor, tamanhos[i], tamanhos[i + 1], k, temp);
    }
    // Mescla todas as partições
    for (int i = 1; i < k; i++) {
        // Chama a função mesclar para combinar as partições em uma única sequência ordenada
        mesclar(vetor, tamanhos[0], tamanhos[i], tamanhos[i + 1], temp);
    }
}

// função principal--------------------------------
// reecebe um numero e um arquivo como destinatario das saidas
int main(int argc, char *argv[]) {
    // Verifica os argumentos da linha de comando
    if (argc != 3) {
        printf("Uso: %s <tamanho do vetor> <arquivo de saída>\n", argv[0]);
        return 1;
    }
    int tamanho = atoi(argv[1]);  // Converte o primeiro argumento para inteiro
    const char *arquivoSaida = argv[2];  // Nome do arquivo de saída
    if (tamanho <= 0) {
        printf("O tamanho do vetor deve ser positivo.\n");
        return 1;
    }
    FILE *saida = fopen(arquivoSaida, "a");
    if (!saida) {
        perror("Erro ao abrir arquivo de saída");
        return 1;
    }
    int *vetor = (int *)malloc(tamanho * sizeof(int));
    int *temp = (int *)malloc(tamanho * sizeof(int));
    if (!vetor || !temp) {
        perror("Erro ao alocar memória");
        fclose(saida);
        return 1;
    }
    srand(time(NULL));  // Inicializa o gerador de números aleatórios
    struct rusage usoInicio, usoFim;
    gerarVetorAleatorio(vetor, tamanho);
    if (getrusage(RUSAGE_SELF, &usoInicio) != 0) {
        perror("Erro ao obter informações iniciais de uso");
        free(vetor);
        fclose(saida);
        return 1;
    }
    ordenarKParticoes(vetor, 0 ,tamanho, K, temp);
    if (getrusage(RUSAGE_SELF, &usoFim) != 0) {
        perror("Erro ao obter informações finais de uso");
        free(vetor);
        fclose(saida);
        return 1;
    }
    tempoExecucao(saida, usoInicio, usoFim);
    // Libera recursos
    //exibirVetor(vetor, tamanho);
    free(vetor);
    free(temp);
    fclose(saida);
    return 0;
}
