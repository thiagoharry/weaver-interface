/*18:*/
#line 360 "weaver-interface.tex"

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
#line 450 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#include <pthread.h> 
#elif defined(_WIN32)
#include <windows.h> 
#endif
/*:21*/
#line 370 "weaver-interface.tex"

/*20:*/
#line 434 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#define _MUTEX_DECLARATION(mutex) pthread_mutex_t mutex
#elif defined(_WIN32)
#define _MUTEX_DECLARATION(mutex) CRITICAL_SECTION mutex
#elif defined(__EMSCRIPTEN__)
#define _MUTEX_DECLARATION(mutex)
#endif
/*:20*/
#line 371 "weaver-interface.tex"

/*1:*/
#line 104 "weaver-interface.tex"

struct interface*_Wnew_interface(char*filename,char*shader_filename,
float x,float y,float width,float height);
/*:1*//*2:*/
#line 125 "weaver-interface.tex"

void _Wset_interface_shader_library(char*source);
/*:2*//*3:*/
#line 133 "weaver-interface.tex"

void _Wmove_interface(struct interface*i,float x,float y,float z);
/*:3*//*4:*/
#line 146 "weaver-interface.tex"

void _Wrotate_interface(struct interface*i,float rotation);
/*:4*//*5:*/
#line 157 "weaver-interface.tex"

void _Wresize_interface(struct interface*i,float new_width,float new_height);
/*:5*//*6:*/
#line 167 "weaver-interface.tex"

void _Wset_max_size_interface(struct interface*i,float max_width,
float max_height);
/*:6*//*7:*/
#line 177 "weaver-interface.tex"

void _Wset_min_size_interface(struct interface*i,float min_width,
float min_height);
/*:7*//*8:*/
#line 192 "weaver-interface.tex"

void _Wupdate_size_interface(unsigned old_width,unsigned old_height,
unsigned new_width,unsigned new_height);
/*:8*//*9:*/
#line 204 "weaver-interface.tex"

void _Wrender_interface(void);
/*:9*//*10:*/
#line 217 "weaver-interface.tex"

void _Wmark_history_interface(void);
/*:10*//*11:*/
#line 227 "weaver-interface.tex"

struct interface*_Wlink_interface(struct interface*i);
/*:11*//*12:*/
#line 238 "weaver-interface.tex"

void _Winteract_interface(int mouse_x,int mouse_y,bool moving,bool left_click,
bool middle_click,bool right_click);
/*:12*//*13:*/
#line 254 "weaver-interface.tex"

void _Wrestore_history_interface(void);
/*:13*//*16:*/
#line 293 "weaver-interface.tex"

#include <stdlib.h>  
void _Winit_interface(void*(*permanent_alloc)(size_t),
void(*permanent_free)(void*),
void*(*temporary_alloc)(size_t),
void(*temporary_free)(void*),
void*(*before_loading_interface)(void),
void*(*after_loading_interface)(void),
...);
/*:16*//*17:*/
#line 329 "weaver-interface.tex"

void _Wfinish_interface(void);
/*:17*/
#line 372 "weaver-interface.tex"

#ifdef __cplusplus
}
#endif
#endif
/*:18*/
