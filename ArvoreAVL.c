#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Usuario {
    char nome[100];   
    int id;
    char email[100];
} Usuario;

typedef struct NO {
    Usuario usuario;
    int altura;
    struct NO *esq;
    struct NO *dir;
} NO;

int altura_NO(NO *no) {
    return (no == NULL) ? -1 : no->altura;
}

int maior(int a, int b) {
    return (a > b) ? a : b;
}

NO *novoNO(Usuario user) {
    NO *no = (NO *)malloc(sizeof(NO));
    no->usuario = user;
    no->altura = 0;
    no->esq = NULL;
    no->dir = NULL;
    return no;
}

int fatorBalanceamento(NO *no) {
    if (no == NULL)
        return 0;
    return altura_NO(no->esq) - altura_NO(no->dir);
}

// Rotações
NO *rotacaoLL(NO *raiz) {
    NO *no = raiz->esq;
    raiz->esq = no->dir;
    no->dir = raiz;
    raiz->altura = maior(altura_NO(raiz->esq), altura_NO(raiz->dir)) + 1;
    no->altura = maior(altura_NO(no->esq), altura_NO(no->dir)) + 1;
    return no;
}

NO *rotacaoRR(NO *raiz) {
    NO *no = raiz->dir;
    raiz->dir = no->esq;
    no->esq = raiz;
    raiz->altura = maior(altura_NO(raiz->esq), altura_NO(raiz->dir)) + 1;
    no->altura = maior(altura_NO(no->esq), altura_NO(no->dir)) + 1;
    return no;
}

NO *rotacaoLR(NO *raiz) {
    raiz->esq = rotacaoRR(raiz->esq);
    return rotacaoLL(raiz);
}

NO *rotacaoRL(NO *raiz) {
    raiz->dir = rotacaoLL(raiz->dir);
    return rotacaoRR(raiz);
}

// Inserção
NO *inserir(NO *raiz, Usuario user) {
    if (raiz == NULL)
        return novoNO(user);

    int cmp = strcmp(user.nome, raiz->usuario.nome);
    if (cmp < 0)
        raiz->esq = inserir(raiz->esq, user);
    else if (cmp > 0)
        raiz->dir = inserir(raiz->dir, user);
    else {
        printf("Usuário com nome '%s' já existe.\n", user.nome);
        return raiz;
    }

    raiz->altura = maior(altura_NO(raiz->esq), altura_NO(raiz->dir)) + 1;
    int fb = fatorBalanceamento(raiz);

    // Rebalanceamento
    if (fb > 1 && strcmp(user.nome, raiz->esq->usuario.nome) < 0)
        return rotacaoLL(raiz);
    if (fb < -1 && strcmp(user.nome, raiz->dir->usuario.nome) > 0)
        return rotacaoRR(raiz);
    if (fb > 1 && strcmp(user.nome, raiz->esq->usuario.nome) > 0)
        return rotacaoLR(raiz);
    if (fb < -1 && strcmp(user.nome, raiz->dir->usuario.nome) < 0)
        return rotacaoRL(raiz);

    return raiz;
}

NO *menorValor(NO *no) {
    NO *atual = no;
    while (atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

// Remoção
NO *remover(NO *raiz, const char *nome) {
    if (raiz == NULL)
        return NULL;

    int cmp = strcmp(nome, raiz->usuario.nome);
    if (cmp < 0)
        raiz->esq = remover(raiz->esq, nome);
    else if (cmp > 0)
        raiz->dir = remover(raiz->dir, nome);
    else {
        if (raiz->esq == NULL || raiz->dir == NULL) {
            NO *temp = (raiz->esq != NULL) ? raiz->esq : raiz->dir;
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else {
                *raiz = *temp;
            }
            free(temp);
        } else {
            NO *temp = menorValor(raiz->dir);
            raiz->usuario = temp->usuario;
            raiz->dir = remover(raiz->dir, temp->usuario.nome);
        }
    }

    if (raiz == NULL)
        return NULL;

    raiz->altura = maior(altura_NO(raiz->esq), altura_NO(raiz->dir)) + 1;
    int fb = fatorBalanceamento(raiz);

    if (fb > 1 && fatorBalanceamento(raiz->esq) >= 0)
        return rotacaoLL(raiz);
    if (fb > 1 && fatorBalanceamento(raiz->esq) < 0)
        return rotacaoLR(raiz);
    if (fb < -1 && fatorBalanceamento(raiz->dir) <= 0)
        return rotacaoRR(raiz);
    if (fb < -1 && fatorBalanceamento(raiz->dir) > 0)
        return rotacaoRL(raiz);

    return raiz;
}

// Busca
NO *buscar(NO *raiz, const char *nome) {
    if (raiz == NULL)
        return NULL;

    int cmp = strcmp(nome, raiz->usuario.nome);
    if (cmp == 0)
        return raiz;
    else if (cmp < 0)
        return buscar(raiz->esq, nome);
    else
        return buscar(raiz->dir, nome);
}

// Listagem
void listarEmOrdem(NO *raiz) {
    if (raiz != NULL) {
        listarEmOrdem(raiz->esq);
        printf("Nome: %s | ID: %d | Email: %s\n",
               raiz->usuario.nome, raiz->usuario.id, raiz->usuario.email);
        listarEmOrdem(raiz->dir);
    }
}

int main() {
    NO *raiz = NULL;
    int opcao;
    Usuario user;
    char nomeBusca[100];

    do {
        printf("\n==========================");
        printf("\n1 - Cadastrar usuário");
        printf("\n2 - Remover usuário");
        printf("\n3 - Buscar usuário");
        printf("\n4 - Listar usuários");
        printf("\n0 - Sair");
        printf("\n==========================");
        printf("\nOpção: ");

        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                printf("Nome: ");
                fgets(user.nome, sizeof(user.nome), stdin);
                user.nome[strcspn(user.nome, "\n")] = '\0';

                printf("ID: ");
                scanf("%d", &user.id);
                getchar();

                printf("Email: ");
                fgets(user.email, sizeof(user.email), stdin);
                user.email[strcspn(user.email, "\n")] = '\0';

                raiz = inserir(raiz, user);
                break;

            case 2:
                printf("Nome do usuário a remover: ");
                fgets(nomeBusca, sizeof(nomeBusca), stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
                raiz = remover(raiz, nomeBusca);
                break;

            case 3: {
                printf("Nome do usuário a buscar: ");
                fgets(nomeBusca, sizeof(nomeBusca), stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

                NO *resultado = buscar(raiz, nomeBusca);
                if (resultado)
                    printf("Encontrado: Nome: %s | ID: %d | Email: %s\n",
                           resultado->usuario.nome, resultado->usuario.id, resultado->usuario.email);
                else
                    printf("Usuário não encontrado.\n");
                break;
            }

            case 4:
                printf("Usuários cadastrados (ordem alfabética):\n");
                listarEmOrdem(raiz);
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
