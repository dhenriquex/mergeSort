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

//define as constantes-----------------------
#define MAX_VALOR 100
// Defini os valores de k
#define K 10

// funçoes auxiliares--------------------------------
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

// Gera valores aleatórios de 1-100
void vetorAleatorio(int vetor[], int n) {
    for (int i = 0; i < n; i++) {
        vetor[i] = rand() % 101;  // Gera um número aleatório entre 0 e 100
    }
}

// Função de Insertion Sort
void insertionSort(int vetor[], int tamanho) {
    for (int i = 1; i < tamanho; i++) {
        int chave = vetor[i];
        int j = i - 1;
        while (j >= 0 && vetor[j] > chave) {
            vetor[j + 1] = vetor[j];
            j = j - 1;
        }
        vetor[j + 1] = chave;
    }
}

// Função para imprimir o vetor
void exibirVetor(int vetor[], int tamanho) {
    for (int i = 0; i < tamanho; i++)
        printf("%d ", vetor[i]);
    printf("\n");
}

// Função Merge -----------------------------------------
void merge(int vetor[], int esq[], int dir[], int meio, int fim) {
    //inicia os indicies
    int i = 0, j = 0, k = 0;
    // calcula o tamanho dos sub arrays
    int inicio = meio;
    int final = fim - meio;
  // Enquanto houver elementos em ambas as sublistas (esq[] e dir[]), o loop compara e ordena
    while (i < inicio && j < final) {
        if (esq[i] <= dir[j]) {
            vetor[k] = esq[i];
            i++;
        } else {
            vetor[k] = dir[j];
            j++;
        }
        k++;
    }
// Se sobrar algum elemento na sublista da esquerda (esq[]), ele é colocado no vetor[].
    while (i < inicio) {
        vetor[k] = esq[i];
        i++;
        k++;
    }
// Se sobrar algum elemento na sublista da direita (dir[]), ele é colocado no vetor[].
    while (j < final) {
        vetor[k] = dir[j];
        j++;
        k++;
    }
}

// Função Merge Sort com Insertion Sort para partições menores ou iguais a m
void mergeSort(int vetor[], int tamanho, int m) {
    // Se o tamanho do vetor for menor ou igual a 'm', utiliza InsertionSort para ordenar a sublista.
    if (tamanho <= m) {
        insertionSort(vetor, tamanho);
        return;
    }
    // Calcula o índice do meio do vetor para dividir o vetor em duas sublistas.
    int meio = tamanho / 2;
    // Aloca memória para os vetores da esquerda (esq[]) e da direita (dir[]), com tamanhos baseados na divisão do vetor original.
    int* esq = (int*)malloc(meio * sizeof(int));
    int* dir = (int*)malloc((tamanho - meio) * sizeof(int));
    // Verifica se a alocação de memória foi bem-sucedida
    if (!esq || !dir) {
        fprintf(stderr, "Erro ao alocar memória para os subvetores.\n");
        exit(1);
    }
    //  copia os elementos do vetor para a sublista esquerda
    for (int i = 0; i < meio; i++) {
        esq[i] = vetor[i];
    }
    // copia os elementos do vetor para a sublista direita
    for (int i = meio; i < tamanho; i++) {
        dir[i - meio] = vetor[i];
    }
    // Chama recursivamente o mergeSort para a sublista esquerda (esq[])
    mergeSort(esq, meio, m);
    // Chama recursivamente o mergeSort para a sublista direita (dir[])
    mergeSort(dir, tamanho - meio, m);
    // Chama a função merge para mesclar as duas sublistas ordenadas de volta ao vetor original
    merge(vetor, esq, dir, meio, tamanho);
    // Libera memória alocada
    free(esq);
    free(dir);
}

// função principal-----------------------------------
// Função principal, recebe um numero para o tamanho e um arquivo como destinatário da saídas
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
    vetorAleatorio(vetor, tamanho);
    // Obtém as informações de uso de CPU antes de executar a ordenação
    struct rusage usoInicio, usoFim;
    if (getrusage(RUSAGE_SELF, &usoInicio) != 0) {
        perror("Erro ao obter informações iniciais de uso");
        free(vetor);
        fclose(saida);
        return 1;
    }
    mergeSort(vetor,tamanho, K);
    if (getrusage(RUSAGE_SELF, &usoFim) != 0) {
        perror("Erro ao obter informações finais de uso");
        free(vetor);
        fclose(saida);
        return 1;
    }
    tempoExecucao(saida, usoInicio, usoFim);
    // Libera a memória alocada para o vetor
    // exibirVetor(vetor, tamanho);
    //printArray(vetor, tamanho);
    free(vetor);
    // Fecha o arquivo de saída
    fclose(saida);
    return 0;
}
