/*17:*/
#line 364 "weaver-interface.tex"

#ifndef __WEAVER_INTERFACE
#define __WEAVER_INTERFACE
#ifdef __cplusplus
extern"C"{
#endif
#include <stdbool.h>  
#if !defined(_WIN32)
#include <sys/param.h>  
#endif
/*20:*/
#line 458 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#include <pthread.h> 
#elif defined(_WIN32)
#include <windows.h> 
#endif
/*:20*//*26:*/
#line 576 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD) || defined(__EMSCRIPTEN__)
#include <EGL/egl.h> 
#include <GLES2/gl2.h> 
#endif
#if defined(_WIN32)
#pragma comment(lib, "Opengl32.lib")
#include <windows.h> 
#include <GL/gl.h> 
#endif
/*:26*/
#line 374 "weaver-interface.tex"

/*19:*/
#line 439 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#define _MUTEX_DECLARATION(mutex) pthread_mutex_t mutex
#define _STATIC_MUTEX_DECLARATION(mutex) static pthread_mutex_t mutex
#elif defined(_WIN32)
#define _MUTEX_DECLARATION(mutex) CRITICAL_SECTION mutex
#define _STATIC_MUTEX_DECLARATION(mutex) static CRITICAL_SECTION mutex
#elif defined(__EMSCRIPTEN__)
#define _MUTEX_DECLARATION(mutex)
#define _STATIC_MUTEX_DECLARATION(mutex)
#endif
/*:19*/
#line 375 "weaver-interface.tex"

/*28:*/
#line 625 "weaver-interface.tex"

struct user_interface{
int type;
void*next;
float x,y,_x,_y,z;
float rotation,_rotation;
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
};
/*:28*/
#line 376 "weaver-interface.tex"

/*1:*/
#line 104 "weaver-interface.tex"

struct user_interface*_Wnew_interface(char*filename,char*shader_filename,
float x,float y,float z,float width,
float height);
/*:1*//*2:*/
#line 126 "weaver-interface.tex"

void _Wset_interface_shader_library(char*source);
/*:2*//*3:*/
#line 134 "weaver-interface.tex"

void _Wmove_interface(struct user_interface*i,float x,float y,float z);
/*:3*//*4:*/
#line 147 "weaver-interface.tex"

void _Wrotate_interface(struct user_interface*i,float rotation);
/*:4*//*5:*/
#line 158 "weaver-interface.tex"

void _Wresize_interface(struct user_interface*i,
float new_width,float new_height);
/*:5*//*6:*/
#line 168 "weaver-interface.tex"

void _Wrender_interface(unsigned long long time);
/*:6*//*7:*/
#line 185 "weaver-interface.tex"

void _Wmark_history_interface(void);
/*:7*//*8:*/
#line 195 "weaver-interface.tex"

struct user_interface*_Wlink_interface(struct user_interface*i);
/*:8*//*9:*/
#line 206 "weaver-interface.tex"

void _Winteract_interface(int mouse_x,int mouse_y,bool left_click,
bool middle_click,bool right_click);
/*:9*//*10:*/
#line 222 "weaver-interface.tex"

void _Wrestore_history_interface(void);
/*:10*//*13:*/
#line 261 "weaver-interface.tex"

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
#line 319 "weaver-interface.tex"

void _Wfinish_interface(void);
/*:16*/
#line 377 "weaver-interface.tex"

#ifdef __cplusplus
}
#endif
#endif
/*:17*/
