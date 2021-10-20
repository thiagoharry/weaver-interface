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

struct marking {
  int type;
  void *next; // Ponteiro para próximo na lista encadeada
  void *prev; // Ponteiro para anterior na lista encadeada
  struct marking *previous_marking;
  unsigned number_of_interfaces;
};

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
static int counter_temporary_alloc = 0;
void *custom_temporary_alloc(size_t n){
  counter_temporary_alloc ++;
  return malloc(n);
}
static int counter_temporary_free = 0;
void custom_temporary_free(void *p){
  counter_temporary_free ++;
  free(p);
  return;
}

static int counter_before = 0;
void before_loading_interface(void){
  counter_before ++;
}

static int counter_after = 0;
void after_loading_interface(void){
  counter_after ++;
}


void empty_loading_function(void *(*permanent_alloc)(size_t),
		      void (*permanent_free)(void *),
		      void *(*temporary_alloc)(size_t),
		      void (*temporary_free)(void *),
		      void *(*before_loading_interface)(void),
		      void *(*after_loading_interface)(void),
		      char *source_filename, struct interface *target){
  if(before_loading_interface != NULL)
    before_loading_interface();
  char *p = (char *) temporary_alloc(1);
  if(temporary_free != NULL)
    temporary_free(p);
  if(after_loading_interface != NULL)
    after_loading_interface();
}


void test_custom_functions(void){
  _Winit_interface(&window_width, &window_height,
		   custom_permanent_alloc, custom_permanent_free,
		   custom_temporary_alloc, custom_temporary_free,
		   before_loading_interface, after_loading_interface,
		   "test", empty_loading_function,
		   NULL);
  // Permanent alloc: interface + run empty_loading_function + default shader
  _Wnew_interface("filename.test", NULL, 0.0, 0.0, 0.0, 100.0, 100.0);
  _Wfinish_interface();
  assert("Using loading initialization function", counter_before == 1);
  assert("Using loading finalization function", counter_after == 1);
  // Permanent allocation: list of loading functions + history marking + interface
  assert("Using custom permanent allocation", counter_permanent_alloc == 4);
  assert("Using custom temporary allocation function",
	 counter_temporary_alloc == 1);
  assert("Using custom permanent free function", counter_permanent_free == 4);
  assert("Using custom temporary free function", counter_temporary_free == 1);
  counter_permanent_free = 0;
  counter_temporary_free = 0;
  _Winit_interface(&window_width, &window_height,
		   custom_permanent_alloc, NULL,
		   custom_temporary_alloc, NULL,
		   before_loading_interface, after_loading_interface,
		   "test", empty_loading_function,
		   NULL);
  _Wnew_interface("filename.test", NULL, 0.0, 0.0, 0.0, 100.0, 100.0);
  _Wfinish_interface();
  assert("If NULL, do not use free functions", counter_permanent_free == 0 &&
	 counter_temporary_free == 0);
}

void test_structure_history(void){
  _Winit_interface(&window_width, &window_height,
		   malloc, free, malloc, free, NULL, NULL, NULL);
  struct interface *i0, *i1, *i2, *tmp;
  struct marking *mark;
  i0 = _Wnew_interface(NULL, NULL, 0.0, 0.0, 0.0, 100.0, 100.0);
  i1 = _Wnew_interface(NULL, NULL, 0.0, 0.0, 0.0, 100.0, 100.0);
  tmp = (struct interface *) i0 -> next;
  assert("List of structures linking new elements",
	 tmp != NULL && tmp == i1 && i1 -> next == NULL);
  _Wmark_history_interface();
  mark = (struct marking *) i1 -> next;
  assert("Creating markings in the structure history",
	 mark != NULL && mark -> prev == (void *) i1 &&
	 i1 -> next == (void *) mark);
  i2 = _Wnew_interface(NULL, NULL, 0.0, 0.0, 0.0, 100.0, 100.0);
  _Wlink_interface(i0);
  assert("History markings counting elements correctly",
	 mark -> next != NULL && mark -> number_of_interfaces == 2);
  _Wrestore_history_interface();
  assert("Linked list updated after restoring history",
	 i1 -> next == NULL);
  _Wfinish_interface(); 
}

int main(int argc, char **argv){
  _Wcreate_window();
  _Wget_window_size(&window_width, &window_height);
  test_custom_functions();
  test_structure_history();
  imprime_resultado();
  _Wdestroy_window();
  return 0;
}
