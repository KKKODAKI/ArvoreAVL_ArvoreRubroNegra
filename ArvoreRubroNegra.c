#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enum para representar as cores dos nós
typedef enum { RED, BLACK } Color;

// Estrutura do nó da árvore
typedef struct Node {
    int codigo;
    char nome[100];
    int quantidade;
    float preco;
    Color cor;
    struct Node *esq, *dir, *pai;
} Node;

// Ponteiro global para a raiz da árvore
Node* raiz = NULL;

// Cria um novo nó com os dados do produto
Node* criarNo(int codigo, char* nome, int quantidade, float preco) {
    Node* novo = (Node*)malloc(sizeof(Node));
    novo->codigo = codigo;
    strcpy(novo->nome, nome);
    novo->quantidade = quantidade;
    novo->preco = preco;
    novo->cor = RED; // Novos nós começam como vermelhos
    novo->esq = novo->dir = novo->pai = NULL;
    return novo;
}

// Rotação à esquerda (usada para rebalancear a árvore)
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

// Rotação à direita (espelho da rotação à esquerda)
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

// Ajusta a árvore após uma inserção para manter as propriedades da árvore rubro-negra
void ajustarInsercao(Node** raiz, Node* z) {
    while (z != *raiz && z->pai && z->pai->cor == RED) {
        if (!z->pai->pai) break; // Garante que o avô exista
        if (z->pai == z->pai->pai->esq) {
            Node* y = z->pai->pai->dir; // Tio
            if (y && y->cor == RED) {
                // Caso 1: Tio vermelho
                z->pai->cor = BLACK;
                y->cor = BLACK;
                z->pai->pai->cor = RED;
                z = z->pai->pai;
            } else {
                if (z == z->pai->dir) {
                    // Caso 2: Triângulo
                    z = z->pai;
                    rotacaoEsquerda(raiz, z);
                }
                // Caso 3: Linha
                if (z->pai) z->pai->cor = BLACK;
                if (z->pai && z->pai->pai) {
                    z->pai->pai->cor = RED;
                    rotacaoDireita(raiz, z->pai->pai);
                }
            }
        } else {
            // Espelhamento dos casos acima
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
                if (z->pai) z->pai->cor = BLACK;
                if (z->pai && z->pai->pai) {
                    z->pai->pai->cor = RED;
                    rotacaoEsquerda(raiz, z->pai->pai);
                }
            }
        }
    }
    if (*raiz) (*raiz)->cor = BLACK; // A raiz deve sempre ser preta
}

// Inserção em árvore binária de busca (sem considerar as cores)
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

// Insere um novo produto na árvore
void inserir(Node** raiz, int codigo, char* nome, int quantidade, float preco) {
    Node* novo = criarNo(codigo, nome, quantidade, preco);
    *raiz = inserirBST(*raiz, novo); // Inserção padrão
    ajustarInsercao(raiz, novo);     // Ajuste rubro-negro
    if (*raiz) (*raiz)->cor = BLACK; // Garante raiz preta
}

// Busca um produto pelo código
Node* buscarProduto(Node* raiz, int codigo) {
    if (!raiz || raiz->codigo == codigo) return raiz;
    if (codigo < raiz->codigo)
        return buscarProduto(raiz->esq, codigo);
    else
        return buscarProduto(raiz->dir, codigo);
}

// Percorre e exibe a árvore em ordem
void emOrdem(Node* raiz) {
    if (!raiz) return;
    emOrdem(raiz->esq);
    printf("Código: %d, Nome: %s, Quantidade: %d, Preço: %.2f (%s)\n",
           raiz->codigo, raiz->nome, raiz->quantidade, raiz->preco,
           raiz->cor == RED ? "R" : "B");
    emOrdem(raiz->dir);
}

// Retorna o nó com menor valor (mais à esquerda)
Node* minimo(Node* no) {
    while (no->esq) no = no->esq;
    return no;
}

// Substitui um nó da árvore por outro
void substituir(Node** raiz, Node* u, Node* v) {
    if (!u->pai)
        *raiz = v;
    else if (u == u->pai->esq)
        u->pai->esq = v;
    else
        u->pai->dir = v;
    if (v) v->pai = u->pai;
}

// Remove um produto da árvore pelo código
void remover(Node** raiz, int codigo) {
    Node* z = buscarProduto(*raiz, codigo);
    if (!z) return;

    Node *y = z, *x = NULL;
    Color corOriginal = y->cor;

    if (!z->esq) {
        x = z->dir;
        substituir(raiz, z, z->dir);
    } else if (!z->dir) {
        x = z->esq;
        substituir(raiz, z, z->esq);
    } else {
        y = minimo(z->dir); // Sucessor
        corOriginal = y->cor;
        x = y->dir;
        if (y->pai == z) {
            if (x) x->pai = y;
        } else {
            substituir(raiz, y, y->dir);
            y->dir = z->dir;
            if (y->dir) y->dir->pai = y;
        }
        substituir(raiz, z, y);
        y->esq = z->esq;
        if (y->esq) y->esq->pai = y;
        y->cor = z->cor;
    }

    free(z);

    if (corOriginal == BLACK && x != NULL) {
        ajustarInsercao(raiz, x); // Reutilizando ajuste de inserção (não ideal)
    }

    if (*raiz) (*raiz)->cor = BLACK;
}

// Função principal com menu
int main() {
    int opcao;
    while (1) {
        printf("\n==========================");
        printf("\n1 - Cadastrar Produto");
        printf("\n2 - Buscar Produto");
        printf("\n3 - Listar Produtos");
        printf("\n4 - Remover Produto");
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
            scanf(" %[^\n]", nome); // Permite nomes com espaços

            printf("Quantidade: "); 
            scanf("%d", &quantidade);

            printf("Preço: "); 
            scanf("%f", &preco);

            inserir(&raiz, codigo, nome, quantidade, preco);
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
        } else if (opcao == 4) {
            int codigo;
            printf("Código do produto a remover: ");
            scanf("%d", &codigo);
            remover(&raiz, codigo);
        } else {
            printf("Opção inválida.\n");
        }
    }

    return 0;
}
