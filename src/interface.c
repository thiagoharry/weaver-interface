/*19:*/
#line 401 "weaver-interface.tex"

#include "interface.h"
/*32:*/
#line 830 "weaver-interface.tex"

#include <stdarg.h> 
/*:32*//*39:*/
#line 974 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#include <EGL/egl.h> 
#include <GLES2/gl2.h> 
#endif
#if defined(_WIN32)
#pragma comment(lib, "Opengl32.lib")
#include <windows.h> 
#include <GL/gl.h> 
#endif
#include <stdio.h> 
/*:39*/
#line 403 "weaver-interface.tex"

/*22:*/
#line 463 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#define MUTEX_INIT(mutex) pthread_mutex_init(mutex, NULL);
#elif defined(_WIN32)
#define MUTEX_INIT(mutex) InitializeCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#define MUTEX_INIT(mutex)
#endif
/*:22*//*23:*/
#line 478 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#define MUTEX_DESTROY(mutex) pthread_mutex_destroy(mutex);
#elif defined(_WIN32)
#define MUTEX_DESTROY(mutex) DeleteCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#define MUTEX_DESTROY(mutex)
#endif
/*:23*//*24:*/
#line 495 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#define MUTEX_WAIT(mutex) pthread_mutex_lock(mutex);
#elif defined(_WIN32)
#define MUTEX_WAIT(mutex) EnterCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#define MUTEX_WAIT(mutex)
#endif
/*:24*//*25:*/
#line 510 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#define MUTEX_SIGNAL(mutex) pthread_mutex_unlock(mutex);
#elif defined(_WIN32)
#define MUTEX_SIGNAL(mutex) LeaveCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#define MUTEX_SIGNAL(mutex)
#endif
/*:25*//*27:*/
#line 572 "weaver-interface.tex"

#define TYPE_INTERFACE 1 
#define TYPE_LINK      2 
#define TYPE_MARKING   3 
/*:27*//*34:*/
#line 894 "weaver-interface.tex"

#if !defined(W_GLSL_VERSION)
#define W_GLSL_VERSION "#version 100\n"
#endif
/*:34*/
#line 404 "weaver-interface.tex"

/*28:*/
#line 675 "weaver-interface.tex"

struct marking{
int type;
void*next;
struct marking*previous_marking;
};
/*:28*//*29:*/
#line 702 "weaver-interface.tex"

struct link{
int type;
void*next;
struct interface*linked_interface;
};
/*:29*//*30:*/
#line 734 "weaver-interface.tex"

struct file_function{
char*extension;
void(*load_texture)(void*(*permanent_alloc)(size_t),
void(*permanent_free)(void*),
void*(*temporary_alloc)(size_t),
void(*temporary_free)(void*),
void*(*before_loading_interface)(void),
void*(*after_loading_interface)(void),
char*source_filename,struct interface*target);
};
static unsigned number_of_file_functions_in_the_list= 0;
static struct file_function*list_of_file_functions= NULL;
/*:30*/
#line 405 "weaver-interface.tex"

/*14:*/
#line 267 "weaver-interface.tex"

static void*(*permanent_alloc)(size_t)= malloc;
static void*(*temporary_alloc)(size_t)= malloc;
static void(*permanent_free)(void*)= free;
static void(*temporary_free)(void*)= free;
/*:14*//*15:*/
#line 283 "weaver-interface.tex"

static void*(*before_loading_interface)(void)= NULL;
static void*(*after_loading_interface)(void)= NULL;
/*:15*//*35:*/
#line 916 "weaver-interface.tex"

static const char vertex_shader_macro[]= "#define VERTEX_SHADER\n";
static const char fragment_shader_macro[]= "#define FRAGMENT_SHADER\n";
/*:35*//*36:*/
#line 934 "weaver-interface.tex"

static char*shader_library= "";
/*:36*//*43:*/
#line 1110 "weaver-interface.tex"

static const char default_shader_source[]= ""
/*47:*/
#line 1178 "weaver-interface.tex"

"attribute vec3 vertex_position;\n"
/*:47*//*48:*/
#line 1191 "weaver-interface.tex"

"uniform vec4 foreground_color, background_color;\n"
"uniform mat4 model_view_matrix;\n"
"uniform vec2 interface_size;\n"
"uniform float time;\n"
"uniform int integer;\n"
/*:48*//*49:*/
#line 1206 "weaver-interface.tex"

"varying mediump vec2 texture_coordinate;\n"
/*:49*/
#line 1112 "weaver-interface.tex"

"#if defined(VERTEX_SHADER)\n"
/*45:*/
#line 1148 "weaver-interface.tex"

"void main(){\n"
"  gl_Position = model_view_matrix * vec4(vertex_position, 1.0);\n"
"  texture_coordinate = vec2(vertex_position.x, vertex_position.y);\n"
"}\n"
/*:45*/
#line 1114 "weaver-interface.tex"

"#else\n"
/*46:*/
#line 1160 "weaver-interface.tex"

"void main(){\n"
"  vec4 texture = texture2D(texture1, texture_coordinate);\n"
"  gl_FragData[0] = texture;\n"
"}\n"
/*:46*/
#line 1116 "weaver-interface.tex"

"#endif\n"
"";
/*:43*//*44:*/
#line 1134 "weaver-interface.tex"

static const float interface_vertices[12]= {0.0,0.0,0.0,
1.0,0.0,0.0,1.0,1.0,0.0,0.0,1.0,0.0};
/*:44*//*50:*/
#line 1215 "weaver-interface.tex"

static GLuint default_shader_program;
/*:50*/
#line 406 "weaver-interface.tex"

/*40:*/
#line 995 "weaver-interface.tex"

/*41:*/
#line 1038 "weaver-interface.tex"

static bool check_compiling_error(GLuint shader){
GLint status;
glGetShaderiv(shader,GL_COMPILE_STATUS,&status);
if(status==GL_FALSE){
int info_log_length;
char*error_msg;
glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&info_log_length);
error_msg= (char*)temporary_alloc(info_log_length);
glGetShaderInfoLog(shader,info_log_length,&info_log_length,error_msg);
fprintf(stderr,"Shader Error: %s\n",error_msg);
if(temporary_free!=NULL)
temporary_free(error_msg);
return true;
}
return false;
}
/*:41*//*42:*/
#line 1066 "weaver-interface.tex"

static bool check_linking_error(GLuint program){
GLint status;
GLsizei info_log_length;
char*error_msg;
glGetProgramiv(program,GL_LINK_STATUS,&status);
if(status==GL_FALSE){
glGetProgramiv(program,GL_INFO_LOG_LENGTH,&info_log_length);
error_msg= (char*)temporary_alloc(info_log_length);
glGetProgramInfoLog(program,info_log_length,&info_log_length,error_msg);
fprintf(stderr,"Shader Error: %s\n",error_msg);
if(temporary_free!=NULL)
temporary_free(error_msg);
return true;
}
#if defined(W_DEBUG_INTERFACE)
glValidateProgram(program);
glGetProgramiv(program,GL_VALIDATE_STATUS,&status);
if(status==GL_FALSE){
glGetProgramiv(program,GL_INFO_LOG_LENGTH,&info_log_length);
error_msg= (char*)temporary_alloc(info_log_length);
glGetProgramInfoLog(program,info_log_length,&info_log_length,error_msg);
fprintf(stderr,"Shader Error: %s\n",error_msg);
if(temporary_free!=NULL)
temporary_free(error_msg);
return true;
}
#endif
return false;
}
/*:42*/
#line 996 "weaver-interface.tex"

static GLuint new_shader(const char*source_code){
GLuint vertex_shader,fragment_shader,program;
const char*list_of_source_code[4];
vertex_shader= glCreateShader(GL_VERTEX_SHADER);
fragment_shader= glCreateShader(GL_FRAGMENT_SHADER);
list_of_source_code[0]= W_GLSL_VERSION;
list_of_source_code[1]= vertex_shader_macro;
list_of_source_code[2]= shader_library;
list_of_source_code[3]= source_code;
glShaderSource(vertex_shader,4,list_of_source_code,NULL);
list_of_source_code[1]= fragment_shader_macro;
glShaderSource(fragment_shader,4,list_of_source_code,NULL);
glCompileShader(vertex_shader);
if(check_compiling_error(vertex_shader))
return 0;
glCompileShader(fragment_shader);
if(check_compiling_error(fragment_shader))
return 0;
program= glCreateProgram();
glAttachShader(program,vertex_shader);
glAttachShader(program,fragment_shader);
glLinkProgram(program);
if(check_linking_error(program))
return 0;
glDeleteShader(vertex_shader);
glDeleteShader(fragment_shader);
return program;
}
/*:40*/
#line 407 "weaver-interface.tex"

/*31:*/
#line 764 "weaver-interface.tex"

void _Winit_interface(void*(*new_permanent_alloc)(size_t),
void(*new_permanent_free)(void*),
void*(*new_temporary_alloc)(size_t),
void(*new_temporary_free)(void*),
void*(*new_before_loading_interface)(void),
void*(*new_after_loading_interface)(void),...){
if(new_permanent_alloc!=NULL)
permanent_alloc= new_permanent_alloc;
if(new_temporary_alloc!=NULL)
temporary_alloc= new_temporary_alloc;
permanent_free= new_permanent_free;
temporary_free= new_temporary_free;
before_loading_interface= new_before_loading_interface;
after_loading_interface= new_after_loading_interface;
{
int count= -1,i;
va_list args;
char*ext;
void(*func)(void*(*permanent_alloc)(size_t),
void(*permanent_free)(void*),void*(*temporary_alloc)(size_t),
void(*temporary_free)(void*),
void*(*before_loading_interface)(void),
void*(*after_loading_interface)(void),
char*source_filename,struct interface*target);
va_start(args,new_after_loading_interface);
do{
count++;
ext= va_arg(args,char*);
func= va_arg(args,void(*)(void*(*)(size_t),void(*)(void*),
void*(*)(size_t),void(*)(void*),
void*(*)(void),void*(*)(void),
char*,struct interface*));
}while(ext!=NULL);
number_of_file_functions_in_the_list= count;
list_of_file_functions= (struct file_function*)
permanent_alloc(sizeof(struct file_function)*
count);
va_start(args,new_after_loading_interface);
for(i= 0;i<count;i++){
list_of_file_functions[i].extension= va_arg(args,char*);
list_of_file_functions[i].load_texture= 
va_arg(args,void(*)(void*(*)(size_t),void(*)(void*),
void*(*)(size_t),void(*)(void*),
void*(*)(void),void*(*)(void),
char*,struct interface*));
}
}
/*51:*/
#line 1223 "weaver-interface.tex"

default_shader_program= new_shader(default_shader_source);
/*:51*/
#line 812 "weaver-interface.tex"

}
/*:31*//*33:*/
#line 846 "weaver-interface.tex"

void _Wfinish_interface(void){
/*38:*/
#line 958 "weaver-interface.tex"

shader_library= "";
/*:38*//*52:*/
#line 1231 "weaver-interface.tex"

glDeleteProgram(default_shader_program);
/*:52*/
#line 848 "weaver-interface.tex"

permanent_free(list_of_file_functions);
number_of_file_functions_in_the_list= 0;
permanent_alloc= malloc;
temporary_alloc= malloc;
permanent_free= free;
temporary_free= free;
before_loading_interface= NULL;
after_loading_interface= NULL;
}
/*:33*//*37:*/
#line 944 "weaver-interface.tex"

void _Wset_interface_shader_library(char*source){
shader_library= source;
}
/*:37*/
#line 408 "weaver-interface.tex"

/*:19*/
