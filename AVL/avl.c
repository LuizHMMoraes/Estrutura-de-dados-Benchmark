#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

struct TNodoA{
int id;
char senha[50];
int FB;
struct TNodoA *esq;
struct TNodoA *dir;
};

int Altura (pNodoA *a)
{
    int Alt_Esq, Alt_Dir;
    if (a == NULL)
      return 0;
    else
    {
       Alt_Esq = Altura (a->esq);
       Alt_Dir = Altura (a->dir);
       if (Alt_Esq > Alt_Dir)
         return (1 + Alt_Esq);
       else
         return (1 + Alt_Dir);
     }
}

int Calcula_FB(pNodoA *a)
{
    return (Altura(a->esq) - Altura(a->dir));
}

void Desenha(pNodoA *a, int nivel) {
    if (a != NULL) {
        for (int x = 1; x <= nivel; x++) printf("=");
        printf("ID: %d | Senha: %s | FB: %d\n", a->id, a->senha, a->FB);
        if (a->esq != NULL) Desenha(a->esq, nivel + 1);
        if (a->dir != NULL) Desenha(a->dir, nivel + 1);
    }
}

int is_avl(pNodoA *a)
{
  int alt_esq, alt_dir;

  if (a!=NULL)
  {
     alt_esq = Altura(a->esq);
     alt_dir = Altura(a->dir);
     return ( (alt_esq - alt_dir <2) && (alt_dir - alt_esq <2) && (is_avl(a->esq)) && (is_avl(a->dir)) );
  }
  else
  return 1;
}

pNodoA* rotacao_direita(pNodoA *pt){
   pNodoA* ptu;

   ptu = pt->esq;
   pt->esq = ptu->dir;
   ptu->dir = pt;
   pt->FB = 0;
   pt = ptu;
   return pt;
}

pNodoA* rotacao_esquerda(pNodoA *pt){
   pNodoA* ptu;

   ptu = pt->dir;
   pt->dir = ptu->esq;
   ptu->esq = pt;
   pt->FB = 0;
   pt = ptu;
   return pt;
}

pNodoA* rotacao_dupla_direita (pNodoA* pt){
   pNodoA* ptu, *ptv;

   ptu = pt->esq;
   ptv = ptu->dir;
   ptu->dir = ptv->esq;
   ptv->esq = ptu;
   pt->esq = ptv->dir;
   ptv->dir = pt;
   if (ptv->FB == 1)   pt->FB = -1;
      else pt->FB = 0;
   if (ptv->FB == -1)  ptu->FB = 1;
      else ptu->FB = 0;
   pt = ptv;
   return pt;
}

pNodoA* rotacao_dupla_esquerda (pNodoA* pt){
   pNodoA *ptu, *ptv;

   ptu = pt->dir;
   ptv = ptu->esq;
   ptu->esq = ptv->dir;
   ptv->dir = ptu;
   pt->dir = ptv->esq;
   ptv->esq = pt;
   if (ptv->FB == -1) pt->FB = 1;
     else pt->FB = 0;
   if (ptv->FB == 1) ptu->FB = -1;
      else ptu->FB = 0;
   pt = ptv;
   return pt;
}


pNodoA* Caso1 (pNodoA* a , int *ok)
{
   pNodoA *ptu;

	ptu = a->esq;
	if (ptu->FB == 1)
    {
        printf("fazendo rotacao direita em %d\n",a->id);
        a = rotacao_direita(a);
     }
    else
    {
        printf("fazendo rotacao dupla direita em %d\n",a->id);
        a = rotacao_dupla_direita(a);
    }

    a->FB = 0;
	*ok = 0;
	return a;
}

pNodoA* Caso2 (pNodoA *a , int *ok)
{
    pNodoA *ptu;

	ptu = a->dir;
	if (ptu->FB == -1)
    {
       Desenha(a,1);
       printf("fazendo rotacao esquerda em %d\n",a->id);
       a=rotacao_esquerda(a);
    }
    else
    {
       Desenha(a,1);
       printf("fazendo rotacao dupla esquerda em %d\n",a->id);
       a=rotacao_dupla_esquerda(a);
    }
	a->FB = 0;
	*ok = 0;
	return a;
}

pNodoA* InsereAVL(pNodoA *a, int id, const char* senha, int *ok) {
    if (a == NULL) {
        // Criação de um novo nodo com os campos user_id e senha
        a = (pNodoA*) malloc(sizeof(pNodoA));
        if (a == NULL) {
            printf("Erro ao alocar memória para o nodo.\n");
            exit(1);
        }
        a->id = id;
        strncpy(a->senha, senha, 50);  // Copia a senha de forma segura
        a->senha[49] = '\0';          // Garante a terminação nula
        a->esq = NULL;
        a->dir = NULL;
        a->FB = 0;
        *ok = 1;
    } else if (id < a->id) {
        a->esq = InsereAVL(a->esq, id, senha, ok);
        if (*ok) {
            switch (a->FB) {
                case -1: a->FB = 0; *ok = 0; break;
                case  0: a->FB = 1; break;
                case  1: a = Caso1(a, ok); break;
            }
        }
    } else if (id > a->id) {
        a->dir = InsereAVL(a->dir, id, senha, ok);
        if (*ok) {
            switch (a->FB) {
                case  1: a->FB = 0; *ok = 0; break;
                case  0: a->FB = -1; break;
                case -1: a = Caso2(a, ok); break;
            }
        }
    } else {
        // user_id já existe na árvore
        printf("ID %d já existe. Nenhuma alteração foi feita.\n", id);
        *ok = 0;
    }
    return a;
}

// Exibe a árvore AVL (em ordem)
void display_avl(pNodoA *a) {
    if (a) {
        display_avl(a->esq);
        printf("User ID: %d, Password: %s\n", a->id, a->senha);
        display_avl(a->dir);
    }
}

// Função para buscar uma senha na AVL por user_id
pNodoA* search_avl(pNodoA* a, int id) {
    // Caso base: Nó nulo ou nó com o ID correspondente
    if (a == NULL || a->id == id) {
        return a;
    }

    // Se o user_id buscado for menor, busca na subárvore esquerda
    if (id < a->id) {
        return search_avl(a->esq, id);
    }

    // Se o user_id buscado for maior, busca na subárvore direita
    return search_avl(a->dir, id);
}

// Função para carregar dados de um arquivo para a AVL
void load_data(pNodoA** a, const char* filename, int* ok) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", filename);
        return;
    }
    int id;
    char senha[50];
    while (fscanf(file, "%d %s", &id, senha) != EOF) {
        //printf("Carregando: %d %s\n", user_id, password);  // Linha de depuração
        *a = InsereAVL(*a, id, senha, ok);
    }
    fclose(file);
}

// Função para testar senhas a partir de um arquivo
/*void test_passwords(pNodoA* a, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", filename);
        return;
    }

    int id;
    char senha[50];
    int correct_count = 0;
    int incorrect_count = 0;

    // Lê o arquivo linha por linha até o final
    while (fscanf(file, "%d %s", &id, senha) != EOF) {
        // Busca a senha associada ao user_id na AVL
        char* avl_senha = search_avl(a, id);

        // Verifica se a senha da árvore AVL corresponde à senha fornecida
        if (avl_senha && strcmp(avl_senha, senha) == 0) {
            correct_count++;
        } else {
            incorrect_count++;
        }
    }
    fclose(file);
    printf("Correct Passwords: %d\n", correct_count);
    printf("Incorrect Passwords: %d\n", incorrect_count);
}*/

// Função para exibir o menu
void show_menu() {
    printf("=== Benchmark Menu ===\n\n");
    printf("1. Lista 1k ordenados com 20%% erros\n\n");
    printf("2. Lista 1k ordenados com 50%% erros\n\n");
    printf("3. Lista 1k não ordenados com 20%% erros\n\n");
    printf("4. Lista 1k não ordenados com 50%% erros\n\n");
    printf("5. Lista 10k ordenados com 20%% erros\n\n");
    printf("6. Lista 10k ordenados com 50%% erros\n\n");
    printf("7. Lista 10k não ordenados com 20%% erros\n\n");
    printf("8. Lista 10k não ordenados com 50%% erros\n\n");
    printf("9. Lista 100k ordenados com 20%% erros\n\n");
    printf("10. Lista 100k ordenados com 50%% erros\n\n");
    printf("11. Lista 100k não ordenados com 20%% erros\n\n");
    printf("12. Lista 100k não ordenados com 50%% erros\n\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

// Função para mapear a escolha do menu para os arquivos de dados e teste
void get_file_names(int choice, char* data_file, char* test_file) {
    switch (choice) {
        case 1: strcpy(data_file, "1k_sorted.txt"); strcpy(test_file, "500_test_20.txt"); break;
        case 2: strcpy(data_file, "1k_sorted.txt"); strcpy(test_file, "500_test_50.txt"); break;
        case 3: strcpy(data_file, "1k_unsorted.txt"); strcpy(test_file, "500_test_20.txt"); break;
        case 4: strcpy(data_file, "1k_unsorted.txt"); strcpy(test_file, "500_test_50.txt"); break;
        case 5: strcpy(data_file, "10k_sorted.txt"); strcpy(test_file, "1k_test_20.txt"); break;
        case 6: strcpy(data_file, "10k_sorted.txt"); strcpy(test_file, "1k_test_50.txt"); break;
        case 7: strcpy(data_file, "10k_unsorted.txt"); strcpy(test_file, "1k_test_20.txt"); break;
        case 8: strcpy(data_file, "10k_unsorted.txt"); strcpy(test_file, "1k_test_50.txt"); break;
        case 9: strcpy(data_file, "100k_sorted.txt"); strcpy(test_file, "10k_test_20.txt"); break;
        case 10: strcpy(data_file, "100k_sorted.txt"); strcpy(test_file, "10k_test_50.txt"); break;
        case 11: strcpy(data_file, "100k_unsorted.txt"); strcpy(test_file, "10k_test_20.txt"); break;
        case 12: strcpy(data_file, "100k_unsorted.txt"); strcpy(test_file, "10k_test_50.txt"); break;
        default: data_file[0] = test_file[0] = '\0'; break; // Caso inválido
    }
}
