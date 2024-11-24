#include <stdio.h>      // Biblioteca padrão de entrada e saída
#include <stdlib.h>     // Biblioteca para alocação dinâmica e funções gerais
#include <string.h>     // Para manipulação de strings e memória
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

//funções auxiliares-----------------------------------------
// Usado para calcular o tempo de execução
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

// gera valores aleatorios de 1-100
void gerar_vetor_aleatorio(int vetor[], int n) {
    for (int i = 0; i < n; i++) {
        vetor[i] = rand() % 101;  // Gera um número aleatório entre 0 e 100
    }
}

// Exibe os elementos de um vetor
void exibirVetor(int vetor[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
}
//função padrão do merge------------------------------------
void merge(int vetor[], int inicio, int meio, int fim) {
    int tamanho = fim - inicio + 1;
    // Cria um vetor temporário
    int *temp = (int*) malloc(tamanho * sizeof(int));
    if (temp == NULL) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        exit(1);
    }
    // Inicializa os ponteiros para os subvetores e as variáveis de fim de subvetor
    int p1 = inicio, p2 = meio + 1;
    int fim1 = 0, fim2 = 0;
    // Loop para mesclar os subvetores no vetor temporário
    for (int i = 0; i < tamanho; i++) {
        // Verifica se nenhum dos subvetores chegou ao fim
        if (!fim1 && !fim2) {
            // Compara os elementos dos dois subvetores e adiciona o menor ao vetor temporário
            if (vetor[p1] < vetor[p2]) {
                temp[i] = vetor[p1++];
            } else {
                temp[i] = vetor[p2++];
            }
            // Atualiza as variáveis de fim de subvetor se um dos ponteiros atingir o meio ou o fim do vetor
            if (p1 > meio) fim1 = 1;
            if (p2 > fim) fim2 = 1;
        } else {
             // Adiciona os elementos restantes do subvetor que ainda não chegou ao fim
            if (!fim1) {
                temp[i] = vetor[p1++];
            } else {
                temp[i] = vetor[p2++];
            }
        }
    }
    //  Copia de elementos do vetor temporario
    for (int i = 0; i < tamanho; i++) {
        vetor[inicio + i] = temp[i];
    }
    // Libera o vetor temporário
    free(temp);
}

//mergesort, particiona o vetor
void mergeSort(int vetor[], int inicio, int fim) {
    // Verifica se o início do intervalo é menor que o fim
    if (inicio < fim) {
        int meio = (inicio + fim) / 2;
        // Chama recursivamente mergeSort para ordenar a primeira metade
        mergeSort(vetor, inicio, meio);
        // Chama recursivamente mergeSort para ordenar a segunda metade
        mergeSort(vetor, meio + 1, fim);
        // Mescla as duas metades ordenadas
        merge(vetor, inicio, meio, fim);
    }
}

// Função principal,-----------------------------------------
// recebe um numero para o tamanho e um arquivo como destinatário da saídas
int main(int argc, char *argv[]) {
    // Verifica se o número de argumentos passados na linha de comando é diferente de 3 (programa, tamanho e arquivo)
    if (argc != 3) {
        printf("Uso: %s <tamanho do vetor> <arquivo de saída>\n", argv[0]);
        return 1;
    }
     // Converte o primeiro argumento para um inteiro (tamanho do vetor)
    int tamanho = atoi(argv[1]);
    // O segundo argumento é o nome do arquivo de saída
    const char *arquivoSaida = argv[2];
      // Verifica se o tamanho do vetor é válido
    if (tamanho <= 0) {
        printf("O tamanho do vetor deve ser positivo.\n");
        return 1;
    }
    // Abre o arquivo de saída no modo append 
    FILE *saida = fopen(arquivoSaida, "a");
    // Verifica se abriu o arquivo corretamente
    if (!saida) {
        perror("Erro ao abrir arquivo de saída");
        return 1;
    }
    // Aloca memória para o vetor de inteiros com o tamanho especificado
    int *vetor = (int *)malloc(tamanho * sizeof(int));
    // Verifica se a locação deu certo
    if (!vetor) {
        perror("Erro ao alocar memória para o vetor");
        fclose(saida);
        return 1;
    }
    // Semente da função que serve para garantir que os valores serao aleatorios
    srand(time(NULL));
    // Chama a função para preencher o vetor com números aleatórios
    gerar_vetor_aleatorio(vetor, tamanho);
    // Obtém as informações de uso de CPU antes de executar a ordenação
    struct rusage usoInicio, usoFim;
    if (getrusage(RUSAGE_SELF, &usoInicio) != 0) {
        perror("Erro ao obter informações iniciais de uso");
        free(vetor);
        fclose(saida);
        return 1;
    }
    mergeSort(vetor, 0 ,tamanho -1);
    if (getrusage(RUSAGE_SELF, &usoFim) != 0) {
        perror("Erro ao obter informações finais de uso");
        free(vetor);
        fclose(saida);
        return 1;
    }
    tempoExecucao(saida, usoInicio, usoFim);
    // Libera a memória alocada para o vetor
    //exibirVetor(vetor, tamanho);
    free(vetor);
    // Fecha o arquivo de saída
    fclose(saida);
    return 0;
}
