#include <stdio.h>
#include <stdlib.h>

#include "../src/interface.h"
#include "window.h"

static int window_width, window_height;

int numero_de_testes = 0, acertos = 0, falhas = 0;
void imprime_resultado(void){
  printf("\n%d tests: %d sucess, %d fails\n\n",
	 numero_de_testes, acertos, falhas);
}

void assert(char *descricao, bool valor){
  char pontos[72], *s = descricao;
  size_t tamanho_string = 0;
  int i;
  while(*s)
    tamanho_string += (*s++ & 0xC0) != 0x80;
  pontos[0] = ' ';
  for(i = 1; i < 71 - (int) tamanho_string; i ++)
    pontos[i] = '.';
  pontos[i] = '\0';
  numero_de_testes ++;
  printf("%s%s", descricao, pontos);
  if(valor){
#if defined(__unix__) && !defined(__EMSCRIPTEN__)
    printf("\e[32m[OK]\033[0m\n");
#else
    printf("[OK]\n");
#endif
    acertos ++;
  }
  else{
#if defined(__unix__) && !defined(__EMSCRIPTEN__)
    printf("\033[0;31m[FAIL]\033[0m\n");
#else
    printf("[FAIL]\n");
#endif
    falhas ++;
  }
}

static int counter_permanent_alloc = 0;
void *custom_permanent_alloc(size_t n){
  counter_permanent_alloc ++;
  return malloc(n);
}
static int counter_permanent_free = 0;
void custom_permanent_free(void *p){
  counter_permanent_free ++;
  free(p);
  return;
}


void test_custom_functions(void){
  _Winit_interface(&window_width, &window_height,
		   custom_permanent_alloc, custom_permanent_free,
		   NULL, NULL, NULL, NULL, NULL);
  assert("Using custom permanent allocation", counter_permanent_alloc == 1);
  _Wfinish_interface();
  assert("Using custom permanent free function", counter_permanent_free == 1);
}

int main(int argc, char **argv){
  _Wcreate_window();
  _Wget_window_size(&window_width, &window_height);
  test_custom_functions();
  imprime_resultado();
  _Wdestroy_window();
  return 0;
}
