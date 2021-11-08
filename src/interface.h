/*18:*/
#line 349 "weaver-interface_en.tex"

#ifndef __WEAVER_INTERFACE
#define __WEAVER_INTERFACE
#ifdef __cplusplus
extern"C"{
#endif
#include <stdbool.h>  
#if !defined(_WIN32)
#include <sys/param.h>  
#endif
/*21:*/
#line 443 "weaver-interface_en.tex"

#if defined(__linux__) || defined(BSD)
#include <pthread.h> 
#elif defined(_WIN32)
#include <windows.h> 
#endif
/*:21*//*27:*/
#line 558 "weaver-interface_en.tex"

#if defined(__linux__) || defined(BSD)
#include <EGL/egl.h> 
#include <GLES2/gl2.h> 
#endif
#if defined(_WIN32)
#pragma comment(lib, "Opengl32.lib")
#include <windows.h> 
#include <GL/gl.h> 
#endif
/*:27*/
#line 359 "weaver-interface_en.tex"

/*20:*/
#line 424 "weaver-interface_en.tex"

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
/*:20*/
#line 360 "weaver-interface_en.tex"

/*29:*/
#line 605 "weaver-interface_en.tex"

struct interface{
int type;
void*next;
float x,y,z;
float rotation;
float height,width,min_height,max_height,min_width,max_width;
float background_color[4],foreground_color[4];
int integer;
bool visible;
GLfloat _transform_matrix[16];
struct shader*shader_program;
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
/*:29*/
#line 361 "weaver-interface_en.tex"

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

void _Wset_size_limit_interface(struct interface*i,float min_width,
float min_height,float max_width,
float max_height);
/*:6*//*7:*/
#line 168 "weaver-interface_en.tex"

void _Wupdate_size_interface(unsigned old_width,unsigned old_height,
unsigned new_width,unsigned new_height);
/*:7*//*8:*/
#line 180 "weaver-interface_en.tex"

void _Wrender_interface(unsigned long long time);
/*:8*//*9:*/
#line 198 "weaver-interface_en.tex"

void _Wmark_history_interface(void);
/*:9*//*10:*/
#line 209 "weaver-interface_en.tex"

struct interface*_Wlink_interface(struct interface*i);
/*:10*//*11:*/
#line 220 "weaver-interface_en.tex"

void _Winteract_interface(int mouse_x,int mouse_y,bool moving,bool left_click,
bool middle_click,bool right_click);
/*:11*//*12:*/
#line 235 "weaver-interface_en.tex"

void _Wrestore_history_interface(void);
/*:12*//*15:*/
#line 275 "weaver-interface_en.tex"

#include <stdlib.h>  
void _Winit_interface(int*window_width,int*window_height,
void*(*permanent_alloc)(size_t),
void(*permanent_free)(void*),
void*(*temporary_alloc)(size_t),
void(*temporary_free)(void*),
void(*before_loading_interface)(void),
void(*after_loading_interface)(void),
...);
/*:15*//*17:*/
#line 321 "weaver-interface_en.tex"

void _Wfinish_interface(void);
/*:17*/
#line 362 "weaver-interface_en.tex"

#ifdef __cplusplus
}
#endif
#endif
/*:18*/
