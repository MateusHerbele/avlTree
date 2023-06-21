#include "avl.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// a resolver - função enfileirando dando merda pra krl

//substitua por seus dados
struct aluno* getAluno1(){
    struct aluno* retorno = malloc(sizeof(struct aluno));
    if(!retorno)
        exit(1);
    retorno->nome = malloc(sizeof("Mateus dos Santos Herbele"));//sizeof conta o \0
	if(!retorno->nome)
		exit(1);
    retorno->nomeDinf = malloc(sizeof("paulo"));
    if(!retorno->nomeDinf)
        exit(1);
    
	strcpy(retorno->nome, "Mateus dos Santos Herbele");
    strcpy(retorno->nomeDinf, "msh22");
    retorno->grr = 20221254;

	return retorno;
}

struct aluno* getAluno2(){
    return NULL;
}

void imprimirDadosAlunos(){
    struct aluno* aluno = getAluno1();
    
    printf("Trabalho de %s\n", aluno->nome);
    printf("Login Dinf %s\n", aluno->nomeDinf);
    printf("GRR %u\n\n", aluno->grr);

    free(aluno->nome);
    free(aluno->nomeDinf);
    free(aluno);

    aluno = getAluno2();
    if(!aluno)
        return;
    
    printf("...E... \n\n");
    printf("Trabalho de %s\n", aluno->nome);
    printf("Login Dinf %s\n", aluno->nomeDinf);
    printf("GRR %u\n\n", aluno->grr);

    free(aluno->nome);
    free(aluno->nomeDinf);
    free(aluno);
}

// funções de balanceamento
static int altura(struct nodo* nodo){
    if(!nodo)
        return 0;
    return nodo->altura;
}

static int maior(int a, int b){
    if(a > b)
        return a;
    return b;
}

static void rotacaoEsquerda(struct nodo** raiz) {
    printf("rotacao esquerda\n, com base em %d\n", (*raiz)->chave);
    struct nodo* aux = (*raiz)->fdir;
    (*raiz)->fdir = aux->fesq; // nada
    aux->fesq = *raiz;
    (*raiz)->altura = maior(altura((*raiz)->fesq), altura((*raiz)->fdir)) + 1;
    aux->altura = maior(altura(aux->fesq), altura(aux->fdir)) + 1;
    
    if((*raiz)->pai != NULL){
        if((*raiz)->pai->fdir == *raiz)
            (*raiz)->pai->fdir = aux;
        else
            (*raiz)->pai->fesq = aux;
    }
    aux->pai = (*raiz)->pai;
    (*raiz)->pai = aux;
    *raiz = aux; // para que a raiz da subarvore seja atualizada
}

static void rotacaoDireita(struct nodo** raiz){
    printf("rotacao direita\n, com base em %d\n", (*raiz)->chave);
    struct nodo* aux = (*raiz)->fesq;
    (*raiz)->fesq = aux->fdir;
    aux->fdir = *raiz;
    (*raiz)->altura = maior(altura((*raiz)->fesq), altura((*raiz)->fdir)) + 1;
    aux->altura = maior(altura(aux->fesq), altura(aux->fdir)) + 1;
    
    if((*raiz)->pai != NULL){
        if((*raiz)->pai->fesq == *raiz)
            (*raiz)->pai->fesq = aux;
        else
            (*raiz)->pai->fdir = aux;
        aux->pai = (*raiz)->pai;
        (*raiz)->pai = aux;
        *raiz = aux;
    }else{
        aux->pai = NULL;
        
        *raiz = aux;
    }
}

static void duplaRotacaoEsquerda(struct nodo** raiz){
    printf("dupla rotacao esquerda\n, com base em %d\n", (*raiz)->chave);
    rotacaoDireita(&(*raiz)->fdir);
    rotacaoEsquerda(raiz);
}

static void duplaRotacaoDireita(struct nodo** raiz){
    printf("dupla rotacao direita com base em %d\n", (*raiz)->chave);
    rotacaoEsquerda(&(*raiz)->fesq);
    rotacaoDireita(raiz);
}
static void rebalancear(struct nodo** raiz){
    struct nodo* nodo = *raiz;
    switch(nodo->balanceamento){
        case 2:
            printf("teste1\n");
            if(nodo->fdir->balanceamento == 1)
                rotacaoEsquerda(raiz);
            else    
                duplaRotacaoEsquerda(raiz);
            break;
        case -2:
            printf("teste2\n");
            if(nodo->fesq->balanceamento == -1)
                rotacaoDireita(raiz);
            else
                duplaRotacaoDireita(raiz);
            break;
    }
}

static void atualizaAltura(struct nodo* nodo){
    if(!nodo)
        return;
    nodo->altura = maior(altura(nodo->fesq), altura(nodo->fdir)) + 1;
    // balancear enquanto atualiza a altura
    nodo->balanceamento = altura(nodo->fdir) - altura(nodo->fesq);
    if(nodo->balanceamento > 1 || nodo->balanceamento < -1)
        rebalancear(&nodo);
    atualizaAltura(nodo->pai);
}

struct nodo* inserir(struct nodo** raiz, int chave){
    if(!*raiz){
        *raiz = malloc(sizeof(struct nodo));
        if(!*raiz)
            exit(1);
        (*raiz)->chave = chave;
        (*raiz)->fesq = NULL;
        (*raiz)->fdir = NULL;
        (*raiz)->pai = NULL;
        (*raiz)->altura = 1;
        (*raiz)->balanceamento = 0;
        return *raiz;
    }
    if(buscar(*raiz, chave)){
        printf("Falha ao inserir.\n");
        return NULL;
    }     
    struct nodo* atual = malloc(sizeof(struct nodo)); // para percorrer a árvore 
    if(!atual)
        exit(1);
    atual = *raiz;
    struct nodo* pai = NULL;
    struct nodo* novo = malloc(sizeof(struct nodo));
    if(!novo)
        exit(1);
    novo->chave = chave;
    novo->fesq = NULL;
    novo->fdir = NULL;
    novo->balanceamento = 0;

    while(atual){
        pai = atual;
        if(novo->chave < atual->chave)
            atual = atual->fesq;
        else
            atual = atual->fdir;
    }
    novo->pai = pai;
    if(novo->chave < pai->chave)
        pai->fesq = novo;
    else
        pai->fdir = novo;
    
    // atualizar a altura de todos os nós
    atual = novo;
    while(atual){
        int alturaEsquerda; 
        int alturaDireita;
        /* se faz necessário conferir
        se os filhos não estão apontando para NULL*/
        if(atual->fesq)
            alturaEsquerda = atual->fesq->altura;
        else
            alturaEsquerda = 0;
        if(atual->fdir)
            alturaDireita = atual->fdir->altura;
        else
            alturaDireita = 0;
        atual->altura = maior(alturaEsquerda, alturaDireita) + 1;
        atual->balanceamento = alturaDireita - alturaEsquerda;
        //balanceia subindo a árvore
        if(atual->balanceamento == 2 || atual->balanceamento == -2)
            rebalancear(&atual);
        atual = atual->pai;
    }
    return novo;
}

// minha função
static struct nodo* antecessor(struct nodo* nodo){
    // Se existe um filho à esquerda, o antecessor é o nó mais à direita desse filho
    if(nodo->fesq != NULL){
        nodo = nodo->fesq;
        while(nodo->fdir != NULL)
            nodo = nodo->fdir;
        return nodo;
    }
    // Se não há filho à esquerda, o antecessor é o primeiro ancestral à direita
    struct nodo* pai = nodo->pai;
    while(pai != NULL && nodo == pai->fesq){
        nodo = pai;
        pai = pai->pai;
    }
    return pai;
}


int excluir(struct nodo** raiz, int chave){
    int alturaEsquerda;
    int alturaDireita;
    if(!*raiz){
        printf("Árvore vazia\n");
        return -99;
    }
    struct nodo* nodo = buscar(*raiz, chave);
    if(!nodo){
        printf("Chave não encontrada\n");
        return -99;
    }
    // caso o nodo seja uma folha
    if(!nodo->fesq && !nodo->fdir){
        if(nodo->pai){
            if(nodo->pai->fesq == nodo){
                nodo->pai->fesq = NULL;

                //nodo->pai->altura = maior(altura(nodo->pai->fesq), altura(nodo->pai->fdir)) + 1;
            }
            else{
                nodo->pai->fdir = NULL;

                //nodo->pai->altura = maior(altura(nodo->pai->fesq), altura(nodo->pai->fdir)) + 1;
            }   
            atualizaAltura(nodo->pai); // atualiza altura da árvore
        }
        else
            *raiz = NULL;
        free(nodo);
        return 0; // caso a árvore tenha apenas um elemento
    }
    // caso o nodo tenha apenas um filho
    if((nodo->fesq && !nodo->fdir) || (!nodo->fesq && nodo->fdir)){
        struct nodo* filho = NULL;
        if(nodo->fesq)
            filho = nodo->fesq;
        else
            filho = nodo->fdir;
        if(nodo->pai){
            if(nodo->pai->fesq == nodo){
                nodo->pai->fesq = filho;
                //filho->altura = filho->altura - 1;
                //nodo->pai->altura = maior(altura(nodo->pai->fesq), altura(nodo->pai->fdir)) + 1;
            }
            else{
                nodo->pai->fdir = filho;
                //filho->altura = filho->altura - 1;
                //nodo->pai->altura = maior(altura(nodo->pai->fesq), altura(nodo->pai->fdir)) + 1;
            }   
            atualizaAltura(nodo->pai); // atualiza altura da árvore
        }
        else{ // caso o nodo seja a raiz
           // filho->altura = filho->altura - 1;
            *raiz = filho;
        }
        free(nodo);
        return 0;
    }
    // caso o nodo tenha dois filhos
    struct nodo* nodoAntecessor = malloc(sizeof(struct nodo));
    printf("antecessor: %d\n", antecessor(nodo)->chave);
    if(!nodoAntecessor)
        exit(1);
    nodoAntecessor = antecessor(nodo);
    //pela innserção a árvore vai estar balanceada, então não é necessário verificar se foi encontrado o antecessor
    if(nodoAntecessor->pai->fesq == nodoAntecessor){
        //nodo->altura = nodo->a1ltura - 1;
        // if(nodoAntecessor->fesq)
        //     alturaEsquerda = altura(nodoAntecessor->fesq);
        // else
        //     alturaEsquerda = 0;
        // if(nodoAntecessor->fdir)
        //     alturaDireita = altura(nodoAntecessor->fdir);
        // else
        //     alturaDireita = 0;
        // nodoAntecessor->balanceamento = alturaDireita - alturaEsquerda;
        nodoAntecessor->pai->fesq = NULL;
    }   
    else{
        // if(nodoAntecessor->fesq)
        //     alturaEsquerda = altura(nodoAntecessor->fesq);
        // else
        //     alturaEsquerda = 0;
        // if(nodoAntecessor->fdir)
        //     alturaDireita = altura(nodoAntecessor->fdir);
        // else
        //     alturaDireita = 0;
        // nodoAntecessor->balanceamento = alturaDireita - alturaEsquerda;
        nodoAntecessor->pai->fdir = NULL;
    }
    nodo->chave = nodoAntecessor->chave;
    nodo->fesq = nodoAntecessor->fesq;
    atualizaAltura(nodo);
    free(nodoAntecessor);
    return 0;
}
// minha função - buscas
static struct nodo* buscaPorProfundidade(struct nodo* nodo, int chave){
    if(!nodo)
        return NULL;
    if(nodo->chave == chave)
        return nodo;
    if(chave < nodo->chave)
        return buscaPorProfundidade(nodo->fesq, chave);
    return buscaPorProfundidade(nodo->fdir, chave);
}

struct nodo* buscar(struct nodo* nodo, int chave){
    if(!nodo){
        printf("Árvore vazia\n");
        return NULL;
    }
    return buscaPorProfundidade(nodo, chave);
}

// minha função
static void imprimirEmOrdemAVL(struct nodo* raiz){
    if(!raiz)
        return;
    imprimirEmOrdemAVL(raiz->fesq);
    printf("%d ", raiz->chave);
    imprimirEmOrdemAVL(raiz->fdir);
}

void imprimirEmOrdem(struct nodo* raiz){
	printf("Imprimindo em ordem\n");
    imprimirEmOrdemAVL(raiz);
    printf("\n");
}

// minha função
static struct nodo* removeCabeca(struct fila** fila){
        // if(!(*fila)->nodo)
        //     printf("Fila vazia\n");
        struct nodo* retorno = (*fila)->nodo;                                                       
        printf("Removendo %d\n", retorno->chave);
        struct fila* aux = (*fila)->prox;
        printf("Removendo2 %d\n", retorno->chave);
        free(*fila);
        printf("Removendo3 %d\n", retorno->chave);
        *fila = aux;
        return retorno;
}

// minha função
static void enfileirar(struct nodo* nodo, struct fila* fila){
    if(!fila->nodo){ // fila vazia
        fila->nodo = malloc(sizeof(struct nodo));
        if(!fila->nodo)
            exit(1);
        printf("Enfileirando1 %d\n", nodo->chave);
        fila->nodo = nodo;
        printf("Enfileirando2 %d\n", fila->nodo->chave);
        fila->prox = NULL;
        printf("Enfileirando3 %d\n", fila->nodo->chave);
        return;
    }
    printf("Enfileirando4 %d\n", fila->nodo->chave);
    struct fila* aux = fila; // aux para não perder o ponteiro para o início da fila
    printf("Enfileirando5 %d\n", fila->nodo->chave);
    while(aux->prox)
        aux = aux->prox;
    aux->prox = malloc(sizeof(struct fila));
        printf("Enfileirando6 %d\n", fila->nodo->chave);
    if(!aux->prox)
        exit(1);
    aux->prox->nodo = malloc(sizeof(int));
    if(!aux->prox->nodo)
        exit(1);
    aux->prox->nodo->chave = nodo->chave;
            printf("Enfileirando7 %d\n", fila->nodo->chave);
    aux->prox->prox = NULL;
            printf("Enfileirando8 %d\n", fila->nodo->chave);
}


void imprimirEmLargura(struct nodo* raiz){
    int altura = 0;
    int nosProxNivel = 0;
    int nosNesteNivel = 1;//começa na raíz
    if(!raiz){
        printf("Árvore vazia\n");
        return;
    }    
    struct nodo* atual = raiz; // para não perder o ponteiro para a raiz
	printf("Imprimindo em largura\n");

    struct fila* fila = malloc(sizeof(struct fila));
    if(!fila)
        exit(1);
    fila->prox = NULL;
    
    printf("[%d]: ", altura);//imprime a altura da raiz 
    altura++;
    enfileirar(atual, fila);
    while(fila->nodo){
        atual = removeCabeca(&fila);
        printf("conteudo %d ", atual->chave); // teste - remover
        printf("primeiro elemento da fila2 %d\n", fila->nodo->chave); // teste - remover
        if(atual->fesq){
            printf("fesq %d ", atual->fesq->chave); // teste - remover
            enfileirar(atual->fesq, fila);
            nosProxNivel++;
        }
        if(atual->fdir){
            printf("fdir %d ", atual->fdir->chave); // teste - remover
            enfileirar(atual->fdir, fila);
            nosProxNivel++;
        }
        nosNesteNivel--;
        if(nosNesteNivel == 0){ // se todos os nós deste nível já foram impressos
            nosNesteNivel = nosProxNivel; // atualiza o número de nós que serão impressos no próximo nível
            nosProxNivel = 0; // zera para que guarde o número de nodos seguintes
            if(fila->nodo) // garante que se a fila estiver vazia não imprime nada
                printf("\n[%d]: ", altura++);
        }

    }
}

void liberaMemoria(struct nodo* raiz){
    if(!raiz)
        return;
    liberaMemoria(raiz->fesq);
    liberaMemoria(raiz->fdir);
    free(raiz);
    raiz = NULL; // para não ficar com um ponteiro apontando para um endereço inválido
}