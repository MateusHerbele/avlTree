#include <stdio.h>
#include <stdlib.h>

#include "avl.h"

int main(){
	char* op; //operação a executar
	char* erro; //controla o que entra 
	size_t tamanho = 0; //tamanho para o buffer
	int chave; //chave a ser inserida, removida ou buscada
	imprimirDadosAlunos();
	struct nodo* raiz = NULL;

	while(getline(&op, &tamanho, stdin)){
		switch(*op){
			case 'i':
				chave = strtol(op+1, &erro, 10); //converte a string para int
				if(*erro == '\n') /* se ocorrer alguma anormalidade 
									dentro do que se quer colocar 
									não entra na função inserir*/
					inserir(&raiz, chave);
				else
					printf("Valor de inserção inválido\n");			
				break;
			case 'r':
				chave = strtol(op+1, &erro, 10); //converte a string para int
				if(*erro == '\n') /* se ocorrer alguma anormalidade 
									dentro do que se quer colocar 
									não entra na função remover*/
					excluir(&raiz, chave);
				else
					printf("Valor de remoção inválido\n");			
				break;
			case 'b':
				chave = strtol(op+1, &erro, 10); //converte a string para int
				if(*erro == '\n'){
					struct nodo* nodoEncontrado = NULL;
					nodoEncontrado = buscar(raiz, chave);
					if(nodoEncontrado)
						printf("Encontrado %d\n Altura do nodo: %d\n", chave, nodoEncontrado->altura);
						// remover esse altura depois, só para debug
					else
						printf("Não encontrado %d.\n", chave);
				}else
					printf("Valor de busca inválido\n");			
				break;
				//buscar
				//leia a chave
				//busque a chave
				break;
			case 'e':
				imprimirEmOrdem(raiz);
				//imprimir em ordem
				//imprima a árvore em ordem
				break;
			case 'l':
				imprimirEmLargura(raiz);
				//imprimir em largura
				//imprima a árvore em largura
				break;
			case 'f':
				chave = strtol(op+1, &erro, 10); //converte a string para int
				if(*erro == '\n'){ /* se ocorrer alguma anormalidade 
									dentro do que se quer colocar 
									não entra na função de saída*/
					liberaMemoria(raiz);
					free(op); //libera a memória alocada para op
					free(erro); //libera a memória alocada para erro
					exit(0); // sair do programa, 0 é sucesso
				}else
					printf("Valor de fechamento inválido\n");			
				break;
			default:
				printf("Operação inválida\n");
		}
	}




	return 0;
}
