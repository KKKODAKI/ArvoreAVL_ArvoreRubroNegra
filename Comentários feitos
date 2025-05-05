#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura de um usuário
typedef struct Usuario {
    char nome[100];
    int id;
    char email[100];
} Usuario;

// Estrutura de um nó da árvore AVL
typedef struct NO {
    Usuario usuario;
    int altura;
    struct NO *esq;
    struct NO *dir;
} NO;

// Funções auxiliares para manipulação da árvore
int altura_NO(NO *no) {
    return (no == NULL) ? -1 : no->altura;
}

int maior(int a, int b) {
    return (a > b) ? a : b;
}

// Cria um novo nó
NO *novoNO(Usuario user) {
    NO *no = (NO *)malloc(sizeof(NO));
    no->usuario = user;
    no->altura = 0;
    no->esq = NULL;
    no->dir = NULL;
    return no;
}

// Função para calcular o fator de balanceamento
int fatorBalanceamento(NO *no) {
    if (no == NULL)
        return 0;
    return altura_NO(no->esq) - altura_NO(no->dir);
}

// Funções de rotação para balancear a árvore
// Rotação simples à esquerda
NO *rotacaoLL(NO *raiz) { 
    NO *no = raiz->esq;
    raiz->esq = no->dir;
    no->dir = raiz;
    raiz->altura = maior(altura_NO(raiz->esq), altura_NO(raiz->dir)) + 1;
    no->altura = maior(altura_NO(no->esq), altura_NO(no->dir)) + 1;
    return no;
}

// Rotação simples à direita
NO *rotacaoRR(NO *raiz) { 
    NO *no = raiz->dir;
    raiz->dir = no->esq;
    no->esq = raiz;
    raiz->altura = maior(altura_NO(raiz->esq), altura_NO(raiz->dir)) + 1;
    no->altura = maior(altura_NO(no->esq), altura_NO(no->dir)) + 1;
    return no;
}

// Rotação dupla (esquerda-direita)
NO *rotacaoLR(NO *raiz) { 
    raiz->esq = rotacaoRR(raiz->esq);
    return rotacaoLL(raiz);
}

// Rotação dupla (direita-esquerda)
NO *rotacaoRL(NO *raiz) { 
    raiz->dir = rotacaoLL(raiz->dir);
    return rotacaoRR(raiz);
}

// Função de inserção na árvore balanceada
NO *inserir(NO *raiz, Usuario user) {
    if (raiz == NULL) return novoNO(user);  // Caso base: árvore vazia

    // Insere o usuário na árvore de forma recursiva
    int cmp = strcmp(user.nome, raiz->usuario.nome);
    if (cmp < 0) raiz->esq = inserir(raiz->esq, user);
    else if (cmp > 0) raiz->dir = inserir(raiz->dir, user);
    else { 
        printf("Usuário já existe\n");
        return raiz; 
    }

    // Atualiza a altura e realiza balanceamento
    raiz->altura = maior(altura_NO(raiz->esq), altura_NO(raiz->dir)) + 1;
    int fb = fatorBalanceamento(raiz);
    
    // Rebalanceamento dependendo do fator de balanceamento
    if (fb > 1 && strcmp(user.nome, raiz->esq->usuario.nome) < 0) return rotacaoLL(raiz);
    if (fb < -1 && strcmp(user.nome, raiz->dir->usuario.nome) > 0) return rotacaoRR(raiz);
    if (fb > 1 && strcmp(user.nome, raiz->esq->usuario.nome) > 0) return rotacaoLR(raiz);
    if (fb < -1 && strcmp(user.nome, raiz->dir->usuario.nome) < 0) return rotacaoRL(raiz);

    return raiz;
}

// Função de busca na árvore
NO *buscar(NO *raiz, const char *nome) {
    if (raiz == NULL) return NULL;
    int cmp = strcmp(nome, raiz->usuario.nome);
    if (cmp == 0) return raiz;
    else if (cmp < 0) return buscar(raiz->esq, nome);
    else return buscar(raiz->dir, nome);
}

// Função para listar todos os usuários em ordem alfabética
void listarEmOrdem(NO *raiz) {
    if (raiz != NULL) {
        listarEmOrdem(raiz->esq);
        printf("Nome: %s | ID: %d | Email: %s\n", raiz->usuario.nome, raiz->usuario.id, raiz->usuario.email);
        listarEmOrdem(raiz->dir);
    }
}

// Função principal que gerencia o menu de opções
int main() {
    NO *raiz = NULL;
    int opcao;
    Usuario user;
    char nomeBusca[100];

    do {
        printf("\n1 - Cadastrar usuário\n2 - Remover usuário\n3 - Buscar usuário\n4 - Listar usuários\n0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        getchar();  // Limpar buffer do teclado

        switch (opcao) {
            case 1:
                printf("Nome: ");
                fgets(user.nome, sizeof(user.nome), stdin);
                user.nome[strcspn(user.nome, "\n")] = '\0';
                printf("ID: ");
                scanf("%d", &user.id);
                getchar();  // Limpar buffer
                printf("Email: ");
                fgets(user.email, sizeof(user.email), stdin);
                user.email[strcspn(user.email, "\n")] = '\0';
                raiz = inserir(raiz, user);  // Inserir usuário na árvore
                break;

            case 2:
                printf("Nome do usuário a remover: ");
                fgets(nomeBusca, sizeof(nomeBusca), stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
                raiz = remover(raiz, nomeBusca);  // Remover usuário da árvore
                break;

            case 3:
                printf("Nome do usuário a buscar: ");
                fgets(nomeBusca, sizeof(nomeBusca), stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
                NO *resultado = buscar(raiz, nomeBusca);  // Buscar usuário
                if (resultado)
                    printf("Encontrado: Nome: %s | ID: %d | Email: %s\n", resultado->usuario.nome, resultado->usuario.id, resultado->usuario.email);
                else
                    printf("Usuário não encontrado.\n");
                break;

            case 4:
                printf("Usuários cadastrados (ordem alfabética):\n");
                listarEmOrdem(raiz);  // Listar usuários
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);

    return 0;
}
