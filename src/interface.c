/*17:*/
#line 345 "weaver-interface_en.tex"

#include "interface.h"

/*13:*/
#line 237 "weaver-interface_en.tex"

#include <stdlib.h> 
static void*(*permanent_alloc)(size_t)= malloc;
static void*(*temporary_alloc)(size_t)= malloc;
static void(*permanent_free)(void*)= free;
static void(*temporary_free)(void*)= free;
/*:13*/
#line 348 "weaver-interface_en.tex"

/*:17*/
