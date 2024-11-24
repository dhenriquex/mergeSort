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

//funçoes auxiliares------------------------------
//usado apra calcualr o tempo de execução
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
void vetorAleatorio(int vetor[], int n) {
    for (int i = 0; i < n; i++) {
        vetor[i] = rand() % 101;  // Gera um número aleatório entre 0 e 100
    }
}

// exibe o vetor
void exibirVetor(int vetor[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
}

//mergesort interativo-----------------------------------
void merge(int vetor[], int inicio, int meio, int fim) {
    // calcula o tamanho das duas metades do vetor a ser mesclado
    int valor1 = meio - inicio + 1;
    int valor2 = fim - (meio + 1) + 1;
    // cria vetores temporarios
    int* vault1 = (int*)malloc(valor1 * sizeof(int));
    int* vault2 = (int*)malloc(valor2 * sizeof(int));
    // verifica se a alocação funcionou
    if (vault1 == NULL || vault2 == NULL) {
        fprintf(stderr, "Erro ao alocar memória para os vetores temporários.\n");
        exit(1);
    }
    // copia a primeira metadeda dovetor
    memcpy(vault1, &vetor[inicio], valor1 * sizeof(int));
    // copaia a segunda metade do vetor
    memcpy(vault2, &vetor[meio + 1], valor2 * sizeof(int));
    // inicializa os indices
    int i = 0, j = 0, k = inicio;
    //mesclas o vetores e salva no vetor original seguindo a ordem de crescente
    while (i < valor1 && j < valor2) {
        //se vt1 for menor q vt2 entao é salva no vetor original
        if (vault1[i] <= vault2[j]) {
            vetor[k] = vault1[i];
            i++;
        } 
        else {
            vetor[k] = vault2[j];
            j++;
        }
        k++;
    }
    // Se ainda houver elementos em vt2 coloca noo vetor original
    while (j < valor2) {
        vetor[k] = vault2[j];
        j++;
        k++;
    }
    // Se ainda houver elementos em vt1 coloca no vetor original
    while (i < valor1) {
        vetor[k] = vault1[i];
        i++;
        k++;
    }
    // lebra a memria alocada para os vetores temporários
    free(vault1);
    free(vault2);
}

// merge sort, mescla os valores
void mergeSortIterativo(int vetor[], int tamanho) {
    // Loop para controlar o tamanho do subvetor, começando de 1 e dobrando a cada iteração
    for (int tamanhoSubvetor = 1; tamanhoSubvetor < tamanho; tamanhoSubvetor *= 2) {
         // Loop para percorrer o vetor em blocos de 2*tamanhoSubvetor
        for (int inicio = 0; inicio < tamanho; inicio += 2 * tamanhoSubvetor) {
            // Calcula o meio do subvetor
            int meio = inicio + tamanhoSubvetor - 1;
            // Calcula o fim do subvetor
            int fim = inicio + 2 * tamanhoSubvetor - 1;
            // Ajusta o fim se ele ultrapassar o tamanho do vetor
            if (fim >= tamanho) {
                fim = tamanho - 1;
            }
            // Verifica se ainda há elementos para mesclar
            if (meio < fim) {
                // Chama a função de mesclagem para combinar os subvetores
                merge(vetor, inicio, meio, fim);
            }
        }
    }
}

// função princiapl--------------------------
//  recebe um numero para o tamanho e um arquivo como destinatário da saídas
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
    mergeSortIterativo(vetor, tamanho);
    if (getrusage(RUSAGE_SELF, &usoFim) != 0) {
        perror("Erro ao obter informações finais de uso");
        free(vetor);
        fclose(saida);
        return 1;
    }
    tempoExecucao(saida, usoInicio, usoFim);
    // Libera a memória alocada para o vetor
    // exibirVetor(vetor, tamanho);
    free(vetor);
    // Fecha o arquivo de saída
    fclose(saida);
    return 0;
}
