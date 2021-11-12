/*16:*/
#line 339 "weaver-interface.tex"

#ifndef __WEAVER_INTERFACE
#define __WEAVER_INTERFACE
#ifdef __cplusplus
extern"C"{
#endif
#include <stdbool.h>  
#if !defined(_WIN32)
#include <sys/param.h>  
#endif
/*19:*/
#line 433 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#include <pthread.h> 
#elif defined(_WIN32)
#include <windows.h> 
#endif
/*:19*//*25:*/
#line 551 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#include <EGL/egl.h> 
#include <GLES2/gl2.h> 
#endif
#if defined(_WIN32)
#pragma comment(lib, "Opengl32.lib")
#include <windows.h> 
#include <GL/gl.h> 
#endif
/*:25*/
#line 349 "weaver-interface.tex"

/*18:*/
#line 414 "weaver-interface.tex"

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
/*:18*/
#line 350 "weaver-interface.tex"

/*27:*/
#line 600 "weaver-interface.tex"

struct interface{
int type;
void*next;
float x,y,z;
float rotation;
GLfloat _transform_matrix[16];
float height,width;
float background_color[4],foreground_color[4];
int integer;
bool visible;
struct shader*shader_program;
_MUTEX_DECLARATION(interface_mutex);

bool _mouse_over;
void(*on_mouse_over)(struct interface*);
void(*on_mouse_out)(struct interface*);
void(*on_mouse_left_down)(struct interface*);
void(*on_mouse_left_up)(struct interface*);
void(*on_mouse_middle_down)(struct interface*);
void(*on_mouse_middle_up)(struct interface*);
void(*on_mouse_right_down)(struct interface*);
void(*on_mouse_right_up)(struct interface*);

GLuint*_texture1;
bool _loaded_texture;
bool animate;
unsigned number_of_frames;
unsigned current_frame;
unsigned*frame_duration;
unsigned long _t;
int max_repetition;
};
/*:27*/
#line 351 "weaver-interface.tex"

/*1:*/
#line 104 "weaver-interface.tex"

struct interface*_Wnew_interface(char*filename,char*shader_filename,
float x,float y,float z,float width,
float height);
/*:1*//*2:*/
#line 126 "weaver-interface.tex"

void _Wset_interface_shader_library(char*source);
/*:2*//*3:*/
#line 134 "weaver-interface.tex"

void _Wmove_interface(struct interface*i,float x,float y,float z);
/*:3*//*4:*/
#line 147 "weaver-interface.tex"

void _Wrotate_interface(struct interface*i,float rotation);
/*:4*//*5:*/
#line 158 "weaver-interface.tex"

void _Wresize_interface(struct interface*i,float new_width,float new_height);
/*:5*//*6:*/
#line 167 "weaver-interface.tex"

void _Wrender_interface(unsigned long long time);
/*:6*//*7:*/
#line 184 "weaver-interface.tex"

void _Wmark_history_interface(void);
/*:7*//*8:*/
#line 194 "weaver-interface.tex"

struct interface*_Wlink_interface(struct interface*i);
/*:8*//*9:*/
#line 205 "weaver-interface.tex"

void _Winteract_interface(int mouse_x,int mouse_y,bool left_click,
bool middle_click,bool right_click);
/*:9*//*10:*/
#line 221 "weaver-interface.tex"

void _Wrestore_history_interface(void);
/*:10*//*13:*/
#line 260 "weaver-interface.tex"

#include <stdlib.h>  
void _Winit_interface(int*window_width,int*window_height,
void*(*permanent_alloc)(size_t),
void(*permanent_free)(void*),
void*(*temporary_alloc)(size_t),
void(*temporary_free)(void*),
void(*before_loading_interface)(void),
void(*after_loading_interface)(void),
...);
/*:13*//*15:*/
#line 308 "weaver-interface.tex"

void _Wfinish_interface(void);
/*:15*/
#line 352 "weaver-interface.tex"

#ifdef __cplusplus
}
#endif
#endif
/*:16*/
