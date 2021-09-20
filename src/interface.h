/*17:*/
#line 329 "weaver-interface_en.tex"

#ifndef __WEAVER_INTERFACE
#define __WEAVER_INTERFACE
#ifdef __cplusplus
extern"C"{
#endif
#include <stdbool.h>  
/*20:*/
#line 415 "weaver-interface_en.tex"

#if defined(__linux__) || defined(BSD)
#include <pthread.h> 
#elif defined(_WIN32)
#include <windows.h> 
#endif
/*:20*/
#line 336 "weaver-interface_en.tex"

/*19:*/
#line 399 "weaver-interface_en.tex"

#if defined(__linux__) || defined(BSD)
#define _MUTEX_DECLARATION(mutex) pthread_mutex_t mutex
#elif defined(_WIN32)
#define _MUTEX_DECLARATION(mutex) CRITICAL_SECTION mutex
#elif defined(__EMSCRIPTEN__)
#define _MUTEX_DECLARATION(mutex)
#endif
/*:19*/
#line 337 "weaver-interface_en.tex"

/*1:*/
#line 89 "weaver-interface_en.tex"

struct interface*_Wnew_interface(char*filename,char*shader_filename,
float x,float y,float width,float height);
/*:1*//*2:*/
#line 106 "weaver-interface_en.tex"

void _Wmove_interface(struct interface*i,float x,float y,float z);
/*:2*//*3:*/
#line 119 "weaver-interface_en.tex"

void _Wrotate_interface(struct interface*i,float rotation);
/*:3*//*4:*/
#line 130 "weaver-interface_en.tex"

void _Wresize_interface(struct interface*i,float new_width,float new_height);
/*:4*//*5:*/
#line 140 "weaver-interface_en.tex"

void _Wset_max_size_interface(struct interface*i,float max_width,
float max_height);
/*:5*//*6:*/
#line 150 "weaver-interface_en.tex"

void _Wset_min_size_interface(struct interface*i,float min_width,
float min_height);
/*:6*//*7:*/
#line 164 "weaver-interface_en.tex"

void _Wupdate_size_interface(unsigned old_width,unsigned old_height,
unsigned new_width,unsigned new_height);
/*:7*//*8:*/
#line 176 "weaver-interface_en.tex"

void _Wrender_interface(void);
/*:8*//*9:*/
#line 189 "weaver-interface_en.tex"

void _Wmark_history_interface(void);
/*:9*//*10:*/
#line 200 "weaver-interface_en.tex"

struct interface*_Wlink_interface(struct interface*i);
/*:10*//*11:*/
#line 211 "weaver-interface_en.tex"

void _Winteract_interface(int mouse_x,int mouse_y,bool moving,bool left_click,
bool middle_click,bool right_click);
/*:11*//*12:*/
#line 226 "weaver-interface_en.tex"

void _Wrestore_history_interface(void);
/*:12*//*15:*/
#line 266 "weaver-interface_en.tex"

#include <stdlib.h>  
void _Winit_interface(void*(*permanent_alloc)(size_t),
void(*permanent_free)(void*),
void*(*temporary_alloc)(size_t),
void(*temporary_free)(void*),
void*(*before_loading_interface)(void),
void*(*after_loading_interface)(void),
...);
/*:15*//*16:*/
#line 301 "weaver-interface_en.tex"

void _Wfinish_interface(void);
/*:16*/
#line 338 "weaver-interface_en.tex"

#ifdef __cplusplus
}
#endif
#endif
/*:17*/
