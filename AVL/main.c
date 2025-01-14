#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

int main()
{

   pNodoA *arv = NULL;
   int control, ok = 0;
   char dados[50], teste[50];

   do {
        show_menu();
        scanf("%d", &control);
        // Obter os nomes dos arquivos de dados e teste baseados na escolha do menu
        get_file_names(control, dados, teste);
        if (control == 0) {
            printf("Encerrado.\n");
            break;
        }
        // Carregar dados na árvore AVL
        printf("Carregando dados do arquivo: %s\n", dados);
        load_data(&arv, dados, &ok);
        // Testar senhas a partir do arquivo de teste
        //printf("Testando senhas do arquivo: %s\n", teste);
        //test_passwords(arv, teste);
    } while (control != 0);

    // Exibe a árvore
    printf("\nÁrvore AVL:\n");
    Desenha(arv, 1);

    // Verifica se é AVL
    if (is_avl(arv)) puts("É AVL");
    else puts("Não é AVL");

    //display_avl(arv);

  return 0;
}
