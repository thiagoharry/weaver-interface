#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#if defined(_WIN32)
#include <Windows.h>
#else
#include <sys/time.h>
#endif

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#endif

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
		      void (*before_loading_interface)(void),
		      void (*after_loading_interface)(void),
		      char *source_filename, struct user_interface *target){
  if(before_loading_interface != NULL)
    before_loading_interface();
  char *p = (char *) temporary_alloc(1);
  if(temporary_free != NULL)
    temporary_free(p);
  if(after_loading_interface != NULL)
    after_loading_interface();
}

void animated_loading_function(void *(*permanent_alloc)(size_t),
		      void (*permanent_free)(void *),
		      void *(*temporary_alloc)(size_t),
		      void (*temporary_free)(void *),
		      void *(*before_loading_interface)(void),
		      void *(*after_loading_interface)(void),
		      char *source_filename, struct user_interface *target){
  GLubyte frame1[16] = {255, 0, 0, 255, 255, 0, 0, 255,
			0, 0, 255, 255, 255, 0, 0, 255};
  GLubyte frame2[16] = {0, 255, 0, 255, 0, 255, 0, 255,
			0, 255, 0, 255, 0, 255, 0, 255};
  if(before_loading_interface != NULL)
    before_loading_interface();
  target -> _texture1 = (GLuint *) permanent_alloc(sizeof(GLuint) * 2);
  glGenTextures(2, target -> _texture1);
  glBindTexture(GL_TEXTURE_2D, target -> _texture1[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA,
	       GL_UNSIGNED_BYTE, frame1);
  glBindTexture(GL_TEXTURE_2D, target -> _texture1[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA,
	       GL_UNSIGNED_BYTE, frame2);
  glBindTexture(GL_TEXTURE_2D, 0);
  target -> animate = true;
  target -> number_of_frames = 2;
  target -> max_repetition = -1;
  target -> frame_duration = (unsigned *) permanent_alloc(sizeof(unsigned) * 2);
  target -> frame_duration[0] = 1000000;
  target -> frame_duration[1] = 1000000;
  target -> _loaded_texture = true;
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
  // TODO: Wasm is failing here:
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
  struct user_interface *i0, *i1, *tmp;
  struct marking *mark;
  i0 = _Wnew_interface(NULL, NULL, 0.0, 0.0, 0.0, 100.0, 100.0);
  i1 = _Wnew_interface(NULL, NULL, 0.0, 0.0, 0.0, 100.0, 100.0);
  tmp = (struct user_interface *) i0 -> next;
  assert("List of structures linking new elements",
	 tmp != NULL && tmp == i1 && i1 -> next == NULL);
  _Wmark_history_interface();
  mark = (struct marking *) i1 -> next;
  assert("Creating markings in the structure history",
	 mark != NULL && mark -> prev == (void *) i1 &&
	 i1 -> next == (void *) mark);
  _Wnew_interface(NULL, NULL, 0.0, 0.0, 0.0, 100.0, 100.0);
  _Wlink_interface(i0);
  assert("History markings counting elements correctly",
	 mark -> next != NULL && mark -> number_of_interfaces == 2);
  _Wrestore_history_interface();
  assert("Linked list updated after restoring history",
	 i1 -> next == NULL);
  _Wfinish_interface(); 
}

void set_integer(struct user_interface *i){
  i -> integer = 1;
}

void unset_integer(struct user_interface *i){
  i -> integer = 0;
}

void width_up(struct user_interface *i){
  _Wresize_interface(i, i -> width + 50, i -> height);
}

void width_down(struct user_interface *i){
  _Wresize_interface(i, i -> width - 50, i -> height);
}

void height_up(struct user_interface *i){
  _Wresize_interface(i, i -> width, i -> height + 50);
}

void height_down(struct user_interface *i){
  _Wresize_interface(i, i -> width, i -> height - 50);
}


void rendering_test(void){
  unsigned long elapsed;
  bool testing = true;
  struct user_interface *i, *j;
  _Winit_interface(&window_width, &window_height,
		   malloc, free, malloc, free, NULL, NULL,
		   "animate", animated_loading_function,
		   NULL);
  int interface_width = window_width / 2,
    interface_height = window_height / 2,
    interface_x = window_width / 4,
    interface_y = window_height / 4;
#if defined(W_FORCE_LANDSCAPE)
  if(window_height > window_width){
    //interface_width = window_height / 2;
    //interface_height = window_width / 2;
    //interface_x = window_height / 3;
    //interface_y = window_width / 4;
  } 
#endif
  i = _Wnew_interface(".animate", NULL, interface_x, interface_y, 0.0,
		      interface_width, interface_height);
  j = _Wnew_interface(NULL, NULL, interface_x, interface_y, 1.0,
		      100, 100);
  //_Wrotate_interface(i, M_PI_2);
  //_Wrotate_interface(j, M_PI_2);
  printf("i: (%f x %f)\n", i -> width, i -> height);
  printf("window: (%d x %d)\n", window_width, window_height);
  //printf("i: (%f, %f | %f, %f) width: %d, height: %d\n", i -> x, i -> y,
  //	 i -> _x, i -> _y, window_width, window_height);
  //printf("j: (%f, %f | %f, %f)\n", j -> x, j -> y, j -> _x, j -> _y);
  j -> on_mouse_over = set_integer;
  j -> on_mouse_out = unset_integer;
  j -> on_mouse_left_down = width_up;
  j -> on_mouse_left_up = width_down;
  j -> on_mouse_right_down = height_up;
  j -> on_mouse_right_up = height_down;
  //_Wtoggle_fullscreen();
  //_Wresize_window(800,600);
  //_Wget_window_size(&window_width, &window_height);
  {
#if defined(_WIN32)
    LARGE_INTEGER initial_time, current_time, frequency;
    QueryPerformanceCounter(&initial_time);
    QueryPerformanceCounter(&current_time);
    QueryPerformanceFrequency(&frequency);
#else
    struct timeval initial_time, current_time;
    gettimeofday(&current_time, NULL);
    gettimeofday(&initial_time, NULL);
#endif
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    elapsed = 0;
    while(elapsed < 3000000){
      _Wget_window_input(elapsed);
      _Winteract_interface(_Wmouse.x, _Wmouse.y, _Wmouse.button[W_MOUSE_LEFT],
			   _Wmouse.button[W_MOUSE_MIDDLE],
			   _Wmouse.button[W_MOUSE_RIGHT]);
      glClear(GL_COLOR_BUFFER_BIT);
      if((elapsed / 1000000) % 2){
	_Wmove_interface(j, j -> x + 1.0, j -> y + 1.0, j -> z);
	_Wresize_interface(j, j -> width + 1.0, j -> height + 1.0);
      }
      else{
	_Wmove_interface(j, j -> x - 1.0, j -> y - 1.0, j -> z);
	_Wresize_interface(j, j -> width - 1.0, j -> height - 1.0);
      }
      if(j -> integer == 0)
	_Wrotate_interface(j, j -> rotation + 0.1);
      _Wrotate_interface(i, i -> rotation + 0.01);
      _Wrender_interface((unsigned long long) elapsed);
      if(i -> current_frame != (elapsed / 1000000) % 2)
	testing = false;
#if defined(_WIN32)
      QueryPerformanceCounter(&current_time);
      elapsed = ((current_time.QuadPart - initial_time.QuadPart) * 1000000) / frequency.QuadPart;
#else
      gettimeofday(&current_time, NULL);
      elapsed = (current_time.tv_sec - initial_time.tv_sec) * 1000000;
      elapsed += (current_time.tv_usec - initial_time.tv_usec);
#endif
      _Wrender_window();
#if defined(__EMSCRIPTEN__)
      emscripten_sleep(10);
#endif
    }
  }
  assert("Testing rendering of animated interface", testing);
  _Wfinish_interface(); 
}

int main(int argc, char **argv){
  _Wcreate_window();
  _Wget_window_size(&window_width, &window_height);
  //printf("%d x %d\n", window_width, window_height);
  test_custom_functions();
  test_structure_history();
  rendering_test();
  imprime_resultado();
  _Wdestroy_window();
  return 0;
}
