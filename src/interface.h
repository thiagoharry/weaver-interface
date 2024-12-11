/*17:*/
#line 350 "weaver-interface.cweb"

#ifndef __WEAVER_INTERFACE
#define __WEAVER_INTERFACE
#ifdef __cplusplus
extern"C"{
#endif
#line 356 "weaver-interface.cweb"
#include <stdbool.h>  
#if !defined(_WIN32)
#include <sys/param.h>  
#endif
#line 360 "weaver-interface.cweb"
/*20:*/
#line 444 "weaver-interface.cweb"

#if defined(__linux__) || defined(BSD)
#include <pthread.h> 
#elif defined(_WIN32)
#line 448 "weaver-interface.cweb"
#include <windows.h> 
#endif
#line 450 "weaver-interface.cweb"
/*:20*//*26:*/
#line 563 "weaver-interface.cweb"

#if defined(__linux__) || defined(BSD) || defined(__EMSCRIPTEN__)
#include <EGL/egl.h> 
#include <GLES3/gl3.h> 
#endif
#line 568 "weaver-interface.cweb"
#if defined(_WIN32)
#pragma comment(lib, "Opengl32.lib")
#include <windows.h> 
#include <GL/gl.h> 
#endif
#line 573 "weaver-interface.cweb"
/*:26*/
#line 360 "weaver-interface.cweb"

/*19:*/
#line 425 "weaver-interface.cweb"

#if defined(__linux__) || defined(BSD)
#define _MUTEX_DECLARATION(mutex) pthread_mutex_t mutex
#define _STATIC_MUTEX_DECLARATION(mutex) static pthread_mutex_t mutex
#elif defined(_WIN32)
#line 430 "weaver-interface.cweb"
#define _MUTEX_DECLARATION(mutex) CRITICAL_SECTION mutex
#define _STATIC_MUTEX_DECLARATION(mutex) static CRITICAL_SECTION mutex
#elif defined(__EMSCRIPTEN__)
#line 433 "weaver-interface.cweb"
#define _MUTEX_DECLARATION(mutex)
#define _STATIC_MUTEX_DECLARATION(mutex)
#endif
#line 436 "weaver-interface.cweb"
/*:19*/
#line 361 "weaver-interface.cweb"

/*28:*/
#line 612 "weaver-interface.cweb"

struct user_interface{
int type;
void*next;
float x,y,_x,_y,z;
float rotation,_rotation;
float mouse_x,mouse_y;
GLfloat _transform_matrix[16];
float height,width;
float background_color[4],foreground_color[4];
int integer;
bool visible;
struct shader*shader_program;
_MUTEX_DECLARATION(mutex);

bool _mouse_over;
void(*on_mouse_over)(struct user_interface*);
void(*on_mouse_out)(struct user_interface*);
void(*on_mouse_left_down)(struct user_interface*);
void(*on_mouse_left_up)(struct user_interface*);
void(*on_mouse_middle_down)(struct user_interface*);
void(*on_mouse_middle_up)(struct user_interface*);
void(*on_mouse_right_down)(struct user_interface*);
void(*on_mouse_right_up)(struct user_interface*);

GLuint*_texture1;
bool _loaded_texture;
bool animate;
unsigned number_of_frames;
unsigned current_frame;
unsigned*frame_duration;
unsigned long _t;
int max_repetition;

void*_internal_data;
void(*_free_internal_data)(void*);
void(*_reload_texture)(struct user_interface*);
};
/*:28*/
#line 362 "weaver-interface.cweb"

/*1:*/
#line 101 "weaver-interface.cweb"

struct user_interface*_Wnew_interface(char*filename,char*shader_filename,
float x,float y,float z,float width,
float height);
/*:1*//*2:*/
#line 121 "weaver-interface.cweb"

void _Wset_interface_shader_library(char*source);
/*:2*//*3:*/
#line 127 "weaver-interface.cweb"

void _Wmove_interface(struct user_interface*i,float x,float y,float z);
/*:3*//*4:*/
#line 138 "weaver-interface.cweb"

void _Wrotate_interface(struct user_interface*i,float rotation);
/*:4*//*5:*/
#line 147 "weaver-interface.cweb"

void _Wresize_interface(struct user_interface*i,
float new_width,float new_height);
/*:5*//*6:*/
#line 155 "weaver-interface.cweb"

void _Wrender_interface(unsigned long long time);
/*:6*//*7:*/
#line 171 "weaver-interface.cweb"

void _Wmark_history_interface(void);
/*:7*//*8:*/
#line 181 "weaver-interface.cweb"

struct user_interface*_Wlink_interface(struct user_interface*i);
/*:8*//*9:*/
#line 192 "weaver-interface.cweb"

void _Winteract_interface(int mouse_x,int mouse_y,bool left_click,
bool middle_click,bool right_click);
/*:9*//*10:*/
#line 208 "weaver-interface.cweb"

void _Wrestore_history_interface(void);
/*:10*//*13:*/
#line 247 "weaver-interface.cweb"

#include <stdlib.h>  
void _Winit_interface(int*window_width,int*window_height,
void*(*permanent_alloc)(size_t),
void(*permanent_free)(void*),
void*(*temporary_alloc)(size_t),
void(*temporary_free)(void*),
void(*before_loading_interface)(void),
void(*after_loading_interface)(void),
...);
/*:13*//*16:*/
#line 305 "weaver-interface.cweb"

void _Wfinish_interface(void);
/*:16*/
#line 363 "weaver-interface.cweb"

#ifdef __cplusplus
}
#endif
#line 367 "weaver-interface.cweb"
#endif
#line 368 "weaver-interface.cweb"
/*:17*/
