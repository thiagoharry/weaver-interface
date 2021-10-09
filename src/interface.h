/*19:*/
#line 353 "weaver-interface_en.tex"

#ifndef __WEAVER_INTERFACE
#define __WEAVER_INTERFACE
#ifdef __cplusplus
extern"C"{
#endif
#include <stdbool.h>  
#if !defined(_WIN32)
#include <sys/param.h>  
#endif
/*22:*/
#line 447 "weaver-interface_en.tex"

#if defined(__linux__) || defined(BSD)
#include <pthread.h> 
#elif defined(_WIN32)
#include <windows.h> 
#endif
/*:22*//*28:*/
#line 560 "weaver-interface_en.tex"

#if defined(__linux__) || defined(BSD)
#include <EGL/egl.h> 
#include <GLES2/gl2.h> 
#endif
#if defined(_WIN32)
#pragma comment(lib, "Opengl32.lib")
#include <windows.h> 
#include <GL/gl.h> 
#endif
/*:28*/
#line 363 "weaver-interface_en.tex"

/*21:*/
#line 428 "weaver-interface_en.tex"

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
/*:21*/
#line 364 "weaver-interface_en.tex"

/*27:*/
#line 529 "weaver-interface_en.tex"

struct interface{
int type;
void*next;
float x,y,z;
float rotation;
float height,width;
float background_color[4],foreground_color[4];
int integer;
bool visible;
GLfloat _transform_matrix[16];
GLuint _shader;
_MUTEX_DECLARATION(interface_mutex);

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
#line 365 "weaver-interface_en.tex"

/*1:*/
#line 89 "weaver-interface_en.tex"

struct interface*_Wnew_interface(char*filename,char*shader_filename,
float x,float y,float z,float width,
float height);
/*:1*//*2:*/
#line 110 "weaver-interface_en.tex"

void _Wset_interface_shader_library(char*source);
/*:2*//*3:*/
#line 119 "weaver-interface_en.tex"

void _Wmove_interface(struct interface*i,float x,float y,float z);
/*:3*//*4:*/
#line 132 "weaver-interface_en.tex"

void _Wrotate_interface(struct interface*i,float rotation);
/*:4*//*5:*/
#line 143 "weaver-interface_en.tex"

void _Wresize_interface(struct interface*i,float new_width,float new_height);
/*:5*//*6:*/
#line 153 "weaver-interface_en.tex"

void _Wset_max_size_interface(struct interface*i,float max_width,
float max_height);
/*:6*//*7:*/
#line 163 "weaver-interface_en.tex"

void _Wset_min_size_interface(struct interface*i,float min_width,
float min_height);
/*:7*//*8:*/
#line 177 "weaver-interface_en.tex"

void _Wupdate_size_interface(unsigned old_width,unsigned old_height,
unsigned new_width,unsigned new_height);
/*:8*//*9:*/
#line 189 "weaver-interface_en.tex"

void _Wrender_interface(void);
/*:9*//*10:*/
#line 202 "weaver-interface_en.tex"

void _Wmark_history_interface(void);
/*:10*//*11:*/
#line 213 "weaver-interface_en.tex"

struct interface*_Wlink_interface(struct interface*i);
/*:11*//*12:*/
#line 224 "weaver-interface_en.tex"

void _Winteract_interface(int mouse_x,int mouse_y,bool moving,bool left_click,
bool middle_click,bool right_click);
/*:12*//*13:*/
#line 239 "weaver-interface_en.tex"

void _Wrestore_history_interface(void);
/*:13*//*16:*/
#line 279 "weaver-interface_en.tex"

#include <stdlib.h>  
void _Winit_interface(int*window_width,int*window_height,
void*(*permanent_alloc)(size_t),
void(*permanent_free)(void*),
void*(*temporary_alloc)(size_t),
void(*temporary_free)(void*),
void*(*before_loading_interface)(void),
void*(*after_loading_interface)(void),
...);
/*:16*//*18:*/
#line 325 "weaver-interface_en.tex"

void _Wfinish_interface(void);
/*:18*/
#line 366 "weaver-interface_en.tex"

#ifdef __cplusplus
}
#endif
#endif
/*:19*/
