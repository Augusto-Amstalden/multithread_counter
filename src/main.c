#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>


//==========================================================================================//

  int numeros_primos = 0;
  pthread_mutex_t trava;

//==========================================================================================//

// Funcao para verificar se um numero eh primo
  int eh_primo(unsigned int p) {

    int cont = 0;
    unsigned int primos[80] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
    283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409};
    if(p < 410) {
      for(int i = 0; i < 80; i++) {
        if(p == primos[i]) {
          cont = 1;
          break;
        }
      }
      return cont;
    }
    else {
      for(unsigned int i = 2; i <= p/2; i++) {
        if(p%i == 0) {
          cont = 1;
          break;
        }
      }
      if(cont == 1) {
        return 0;
      }
      else if(cont == 0) {
        return 1;
      }
    }
  }

//==========================================================================================//

// Estrutura para armazenar todos os dados que serao passados para a thread 
    typedef struct { 
    	int quantidade;
    	int* marcador;
    	unsigned int* numeros;
    } arg_struct;

//==========================================================================================//

// Funçao thread, que verifica se os numeros sao primos
    void* funcao_thread(void* arg) {
      int mark;
      arg_struct* N = arg; // Estrutura passada com todos os dados necessaros
      while(1) {
        mark = 0;
        for(int i = 0; i < N->quantidade; i++) {
          pthread_mutex_lock(&(trava));
          if(N->marcador[i] == 0) {
            N->marcador[i] = 1;
            pthread_mutex_unlock(&(trava));
            if(eh_primo(N->numeros[i]))
            (numeros_primos)++;
          }
          else {
            pthread_mutex_unlock(&(trava));
            mark++;
          }
        }
        if(mark == N->quantidade) // Caso todos os numeros da entrada ja tenham sido analisados
          return NULL;
      }
    }

//==========================================================================================//

  int main() {

    int quantidade = 0;
    int threads = 0;

//==========================================================================================//

// Colocando a entrada em um vetor de numeros a serem analisados
    unsigned int numeros[512];
    char nums[1024]; // Espaço para os numeros todos como um texto
    scanf(" %[^\n]", nums); // Le todos os números como um texto
    char *token = strtok(nums," "); // Le ate ao primeiro espaço para a variavel token
    int pos = 0; // Posição para guardar os numeros no vetor começa em 0

    while (token != NULL) {
      numeros[pos++] = atoi(token); // Guarda o valor convertido em numero no vetor e avança
      quantidade++;
      token = strtok(NULL, " "); // Le ate ao próximo espaço a partir de onde terminou
    }

//==========================================================================================//

// Inicializando a lista de marcadores de numeros ja analisadios
    int* marcador = malloc(quantidade*sizeof(int));

    for(int i = 0; i < quantidade; i++) {
      marcador[i] = 0;
    }
    
    pthread_t thread[quantidade];
    for(int a = 0; a < quantidade && a <= 4; a++) {
      thread[a] = a + 1;
    }

//==========================================================================================//

    arg_struct* N = malloc(sizeof(arg_struct)); // Condensa todos os dados em uma estrutura
    N->quantidade = quantidade;
    N->marcador = marcador;
    N->numeros = numeros;


//==========================================================================================//

// Criando o numero de threads necessarios para varrer a lista de numeros (max de 4 threads)
    if(quantidade > 0)
      pthread_create(&(thread[0]), NULL, funcao_thread, N);
      threads++;
    if(quantidade > 1)
      pthread_create(&(thread[1]), NULL, funcao_thread, N);
      threads++;
    if(quantidade > 2)
      pthread_create(&(thread[2]), NULL, funcao_thread, N);
      threads++;
    if(quantidade > 3)
      pthread_create(&(thread[3]), NULL, funcao_thread, N);
      threads++;
      
//==========================================================================================//

// Esperando todas as threads terminarem e liberando a memoria alocada
	if(quantidade > 0)
	  pthread_join(thread[0], NULL);
	if(quantidade > 1)  
	  pthread_join(thread[1], NULL);
	if(quantidade > 2)
	  pthread_join(thread[2], NULL);
	if(quantidade > 3)
	  pthread_join(thread[3], NULL);
	free(N);
	free(marcador);
	printf("%d\n", numeros_primos); // Quantidade de numeros primos encontrados
    }

//==========================================================================================//

