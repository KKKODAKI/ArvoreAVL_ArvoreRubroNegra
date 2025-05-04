#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { RED, BLACK } Color;

typedef struct Node {
    int codigo;
    char nome[100];
    int quantidade;
    float preco;
    Color cor;
    struct Node *esq, *dir, *pai;
} Node;

Node* raiz = NULL;

Node* criarNo(int codigo, char* nome, int quantidade, float preco) {
    Node* novo = (Node*)malloc(sizeof(Node));
    novo->codigo = codigo;
    strcpy(novo->nome, nome);
    novo->quantidade = quantidade;
    novo->preco = preco;
    novo->cor = RED;
    novo->esq = novo->dir = novo->pai = NULL;
    return novo;
}

void rotacaoEsquerda(Node** raiz, Node* x) {
    Node* y = x->dir;
    x->dir = y->esq;
    if (y->esq) y->esq->pai = x;
    y->pai = x->pai;
    if (!x->pai) *raiz = y;
    else if (x == x->pai->esq) x->pai->esq = y;
    else x->pai->dir = y;
    y->esq = x;
    x->pai = y;
}

void rotacaoDireita(Node** raiz, Node* y) {
    Node* x = y->esq;
    y->esq = x->dir;
    if (x->dir) x->dir->pai = y;
    x->pai = y->pai;
    if (!y->pai) *raiz = x;
    else if (y == y->pai->esq) y->pai->esq = x;
    else y->pai->dir = x;
    x->dir = y;
    y->pai = x;
}

void ajustarInsercao(Node** raiz, Node* z) {
    while (z != *raiz && z->pai->cor == RED) {
        if (z->pai == z->pai->pai->esq) {
            Node* y = z->pai->pai->dir;
            if (y && y->cor == RED) {
                z->pai->cor = BLACK;
                y->cor = BLACK;
                z->pai->pai->cor = RED;
                z = z->pai->pai;
            } else {
                if (z == z->pai->dir) {
                    z = z->pai;
                    rotacaoEsquerda(raiz, z);
                }
                z->pai->cor = BLACK;
                z->pai->pai->cor = RED;
                rotacaoDireita(raiz, z->pai->pai);
            }
        } else {
            Node* y = z->pai->pai->esq;
            if (y && y->cor == RED) {
                z->pai->cor = BLACK;
                y->cor = BLACK;
                z->pai->pai->cor = RED;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esq) {
                    z = z->pai;
                    rotacaoDireita(raiz, z);
                }
                z->pai->cor = BLACK;
                z->pai->pai->cor = RED;
                rotacaoEsquerda(raiz, z->pai->pai);
            }
        }
    }
    (*raiz)->cor = BLACK;
}

Node* inserirBST(Node* raiz, Node* z) {
    if (!raiz) return z;
    if (z->codigo < raiz->codigo) {
        raiz->esq = inserirBST(raiz->esq, z);
        raiz->esq->pai = raiz;
    } else if (z->codigo > raiz->codigo) {
        raiz->dir = inserirBST(raiz->dir, z);
        raiz->dir->pai = raiz;
    }
    return raiz;
}

Node* inserir(Node* raiz, int codigo, char* nome, int quantidade, float preco) {
    Node* novo = criarNo(codigo, nome, quantidade, preco);
    raiz = inserirBST(raiz, novo);
    ajustarInsercao(&raiz, novo);
    return raiz;
}

Node* buscarProduto(Node* raiz, int codigo) {
    if (!raiz || raiz->codigo == codigo) return raiz;
    if (codigo < raiz->codigo)
        return buscarProduto(raiz->esq, codigo);
    else
        return buscarProduto(raiz->dir, codigo);
}

void emOrdem(Node* raiz) {
    if (!raiz) return;
    emOrdem(raiz->esq);
    printf("Código: %d, Nome: %s, Quantidade: %d, Preço: %.2f (%s)\n",
           raiz->codigo, raiz->nome, raiz->quantidade, raiz->preco,
           raiz->cor == RED ? "R" : "B");
    emOrdem(raiz->dir);
}

int main() {
    int opcao;
    while (1) {
        printf("\n==========================");
        printf("\n1 - Cadastrar Produto");
        printf("\n2 - Buscar Produto");
        printf("\n3 - Listar Produtos");
        printf("\n0 - Sair");
        printf("\n==========================");
        printf("\nOpção: ");

        scanf("%d", &opcao);
        if (opcao == 0) break;

        if (opcao == 1) {
            int codigo, quantidade;
            float preco;
            char nome[100];
            printf("Código: "); 
            scanf("%d", &codigo);

            printf("Nome: "); 
            scanf("%s", nome);

            printf("Quantidade: "); 
            scanf("%d", &quantidade);

            printf("Preço: "); 
            scanf("%f", &preco);

            raiz = inserir(raiz, codigo, nome, quantidade, preco);
        } else if (opcao == 2) {
            int codigo;
            printf("Código do produto a buscar: ");
            scanf("%d", &codigo);
            Node* resultado = buscarProduto(raiz, codigo);
            if (resultado) {
                printf("Encontrado: Código: %d, Nome: %s, Quantidade: %d, Preço: %.2f\n",
                       resultado->codigo, resultado->nome, resultado->quantidade, resultado->preco);
            } else {
                printf("Produto não encontrado.\n");
            }
        } else if (opcao == 3) {
            emOrdem(raiz);
        } else {
            printf("Opção inválida.\n");
        }
    }
    return 0;
}
