/*17:*/
#line 349 "weaver-interface.tex"

#ifndef __WEAVER_INTERFACE
#define __WEAVER_INTERFACE
#ifdef __cplusplus
extern"C"{
#endif
#include <stdbool.h>  
/*20:*/
#line 435 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#include <pthread.h> 
#elif defined(_WIN32)
#include <windows.h> 
#endif
/*:20*/
#line 356 "weaver-interface.tex"

/*19:*/
#line 419 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#define _MUTEX_DECLARATION(mutex) pthread_mutex_t mutex
#elif defined(_WIN32)
#define _MUTEX_DECLARATION(mutex) CRITICAL_SECTION mutex
#elif defined(__EMSCRIPTEN__)
#define _MUTEX_DECLARATION(mutex)
#endif
/*:19*/
#line 357 "weaver-interface.tex"

/*1:*/
#line 104 "weaver-interface.tex"

struct interface*_Wnew_interface(char*filename,char*shader_filename,
float x,float y,float width,float height);
/*:1*//*2:*/
#line 122 "weaver-interface.tex"

void _Wmove_interface(struct interface*i,float x,float y,float z);
/*:2*//*3:*/
#line 135 "weaver-interface.tex"

void _Wrotate_interface(struct interface*i,float rotation);
/*:3*//*4:*/
#line 146 "weaver-interface.tex"

void _Wresize_interface(struct interface*i,float new_width,float new_height);
/*:4*//*5:*/
#line 156 "weaver-interface.tex"

void _Wset_max_size_interface(struct interface*i,float max_width,
float max_height);
/*:5*//*6:*/
#line 166 "weaver-interface.tex"

void _Wset_min_size_interface(struct interface*i,float min_width,
float min_height);
/*:6*//*7:*/
#line 181 "weaver-interface.tex"

void _Wupdate_size_interface(unsigned old_width,unsigned old_height,
unsigned new_width,unsigned new_height);
/*:7*//*8:*/
#line 193 "weaver-interface.tex"

void _Wrender_interface(void);
/*:8*//*9:*/
#line 206 "weaver-interface.tex"

void _Wmark_history_interface(void);
/*:9*//*10:*/
#line 216 "weaver-interface.tex"

struct interface*_Wlink_interface(struct interface*i);
/*:10*//*11:*/
#line 227 "weaver-interface.tex"

void _Winteract_interface(int mouse_x,int mouse_y,bool moving,bool left_click,
bool middle_click,bool right_click);
/*:11*//*12:*/
#line 243 "weaver-interface.tex"

void _Wrestore_history_interface(void);
/*:12*//*15:*/
#line 282 "weaver-interface.tex"

#include <stdlib.h>  
void _Winit_interface(void*(*permanent_alloc)(size_t),
void(*permanent_free)(void*),
void*(*temporary_alloc)(size_t),
void(*temporary_free)(void*),
void*(*before_loading_interface)(void),
void*(*after_loading_interface)(void),
...);
/*:15*//*16:*/
#line 318 "weaver-interface.tex"

void _Wfinish_interface(void);
/*:16*/
#line 358 "weaver-interface.tex"

#ifdef __cplusplus
}
#endif
#endif
/*:17*/
