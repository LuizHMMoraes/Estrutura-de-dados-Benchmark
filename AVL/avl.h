
typedef struct TNodoA pNodoA;

int Altura (pNodoA * a);
int Calcula_FB(pNodoA * a);
void Desenha(pNodoA * a , int nivel);
int is_avl(pNodoA * a);
pNodoA * rotacao_direita(pNodoA * pt);
pNodoA * rotacao_esquerda(pNodoA * pt);
pNodoA * rotacao_dupla_direita (pNodoA * pt);
pNodoA * rotacao_dupla_esquerda (pNodoA * pt);
pNodoA * Caso1 (pNodoA * a , int *ok);
pNodoA * Caso2 (pNodoA * a , int *ok);
pNodoA * InsereAVL (pNodoA *a, int id, const char* senha, int *ok);
//void test_passwords(pNodoA* a, const char* filename);
void load_data(pNodoA** a, const char* filename, int* ok);
void get_file_names(int choice, char* data_file, char* test_file);
void show_menu();
