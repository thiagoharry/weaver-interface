/*20:*/
#line 393 "weaver-interface_en.tex"

#include "interface.h"
/*34:*/
#line 786 "weaver-interface_en.tex"

#include <string.h> 
/*:34*//*36:*/
#line 867 "weaver-interface_en.tex"

#include <stdarg.h> 
/*:36*//*44:*/
#line 1021 "weaver-interface_en.tex"

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
/*:44*//*63:*/
#line 1549 "weaver-interface_en.tex"

#include <math.h> 
/*:63*/
#line 395 "weaver-interface_en.tex"

/*23:*/
#line 460 "weaver-interface_en.tex"

#if defined(__linux__) || defined(BSD)
#define MUTEX_INIT(mutex) pthread_mutex_init(mutex, NULL);
#elif defined(_WIN32)
#define MUTEX_INIT(mutex) InitializeCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#define MUTEX_INIT(mutex)
#endif
/*:23*//*24:*/
#line 474 "weaver-interface_en.tex"

#if defined(__linux__) || defined(BSD)
#define MUTEX_DESTROY(mutex) pthread_mutex_destroy(mutex);
#elif defined(_WIN32)
#define MUTEX_DESTROY(mutex) DeleteCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#define MUTEX_DESTROY(mutex)
#endif
/*:24*//*25:*/
#line 490 "weaver-interface_en.tex"

#if defined(__linux__) || defined(BSD)
#define MUTEX_WAIT(mutex) pthread_mutex_lock(mutex);
#elif defined(_WIN32)
#define MUTEX_WAIT(mutex) EnterCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#define MUTEX_WAIT(mutex)
#endif
/*:25*//*26:*/
#line 505 "weaver-interface_en.tex"

#if defined(__linux__) || defined(BSD)
#define MUTEX_SIGNAL(mutex) pthread_mutex_unlock(mutex);
#elif defined(_WIN32)
#define MUTEX_SIGNAL(mutex) LeaveCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#define MUTEX_SIGNAL(mutex)
#endif
/*:26*//*29:*/
#line 585 "weaver-interface_en.tex"

#define TYPE_INTERFACE 1 
#define TYPE_LINK      2 
#define TYPE_MARKING   3 
/*:29*//*38:*/
#line 930 "weaver-interface_en.tex"

#if !defined(W_GLSL_VERSION)
#define W_GLSL_VERSION "#version 100\n"
#endif
/*:38*/
#line 396 "weaver-interface_en.tex"

/*30:*/
#line 671 "weaver-interface_en.tex"

struct marking{
int type;
void*next;
struct marking*previous_marking;
};
/*:30*//*31:*/
#line 699 "weaver-interface_en.tex"

struct link{
int type;
void*next;
struct interface*linked_interface;
};
/*:31*//*32:*/
#line 731 "weaver-interface_en.tex"

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
/*:32*/
#line 397 "weaver-interface_en.tex"

/*14:*/
#line 252 "weaver-interface_en.tex"

#include <stdlib.h> 
static void*(*permanent_alloc)(size_t)= malloc;
static void*(*temporary_alloc)(size_t)= malloc;
static void(*permanent_free)(void*)= free;
static void(*temporary_free)(void*)= free;
/*:14*//*15:*/
#line 269 "weaver-interface_en.tex"

static void*(*before_loading_interface)(void)= NULL;
static void*(*after_loading_interface)(void)= NULL;
/*:15*//*17:*/
#line 302 "weaver-interface_en.tex"

static int*window_width= NULL,*window_height= NULL;
/*:17*//*39:*/
#line 954 "weaver-interface_en.tex"

static const char vertex_shader_macro[]= "#define VERTEX_SHADER\n";
static const char fragment_shader_macro[]= "#define FRAGMENT_SHADER\n";
/*:39*//*40:*/
#line 973 "weaver-interface_en.tex"

static const char precision_qualifier[]= "precision highp float;\n"
"precision highp int;\n";
/*:40*//*41:*/
#line 985 "weaver-interface_en.tex"

static char*shader_library= "";
/*:41*//*48:*/
#line 1158 "weaver-interface_en.tex"

static const char default_shader_source[]= ""
/*52:*/
#line 1223 "weaver-interface_en.tex"

"#if defined(VERTEX_SHADER)\n"
"attribute vec3 vertex_position;\n"
"#endif\n"
/*:52*//*53:*/
#line 1238 "weaver-interface_en.tex"

"uniform vec4 foreground_color, background_color;\n"
"uniform mat4 model_view_matrix;\n"
"uniform vec2 interface_size;\n"
"uniform float time;\n"
"uniform int integer;\n"
"uniform sampler2D texture1;\n"
/*:53*//*54:*/
#line 1254 "weaver-interface_en.tex"

"varying mediump vec2 texture_coordinate;\n"
/*:54*/
#line 1160 "weaver-interface_en.tex"

"#if defined(VERTEX_SHADER)\n"
/*50:*/
#line 1194 "weaver-interface_en.tex"

"void main(){\n"
"  gl_Position = model_view_matrix * vec4(vertex_position, 1.0);\n"
"  texture_coordinate = vec2(vertex_position.x, vertex_position.y);\n"
"}\n"
/*:50*/
#line 1162 "weaver-interface_en.tex"

"#else\n"
/*51:*/
#line 1207 "weaver-interface_en.tex"

"void main(){\n"
"  vec4 texture = texture2D(texture1, texture_coordinate);\n"
"  gl_FragData[0] = texture;\n"
"}\n"
/*:51*/
#line 1164 "weaver-interface_en.tex"

"#endif\n"
"";
/*:48*//*49:*/
#line 1181 "weaver-interface_en.tex"

static const float interface_vertices[12]= {0.0,0.0,0.0,
1.0,0.0,0.0,1.0,1.0,0.0,0.0,1.0,0.0};
/*:49*//*55:*/
#line 1263 "weaver-interface_en.tex"

static GLuint default_shader_program;
/*:55*//*59:*/
#line 1328 "weaver-interface_en.tex"

static GLuint default_texture;
/*:59*//*64:*/
#line 1568 "weaver-interface_en.tex"

static void*last_structure= NULL;
static struct marking*last_marking= NULL;
_STATIC_MUTEX_DECLARATION(linked_list_mutex);
/*:64*/
#line 398 "weaver-interface_en.tex"

/*33:*/
#line 758 "weaver-interface_en.tex"

static inline void(*get_loading_function(char*ext))
(void*(*permanent_alloc)(size_t),
void(*permanent_free)(void*),
void*(*temporary_alloc)(size_t),
void(*temporary_free)(void*),
void*(*before_loading_interface)(void),
void*(*after_loading_interface)(void),
char*source_filename,struct interface*target){
unsigned i;
for(i= 0;i<number_of_file_functions_in_the_list;i++){
if(!strcmp(list_of_file_functions[i].extension,ext))
return list_of_file_functions[i].load_texture;
}
return NULL;
}
/*:33*//*45:*/
#line 1042 "weaver-interface_en.tex"

/*46:*/
#line 1085 "weaver-interface_en.tex"

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
/*:46*//*47:*/
#line 1114 "weaver-interface_en.tex"

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
/*:47*/
#line 1043 "weaver-interface_en.tex"

static GLuint new_shader(const char*source_code){
GLuint vertex_shader,fragment_shader,program;
const char*list_of_source_code[5];
vertex_shader= glCreateShader(GL_VERTEX_SHADER);
fragment_shader= glCreateShader(GL_FRAGMENT_SHADER);
list_of_source_code[0]= W_GLSL_VERSION;
list_of_source_code[1]= vertex_shader_macro;
list_of_source_code[2]= precision_qualifier;
list_of_source_code[3]= shader_library;
list_of_source_code[4]= source_code;
glShaderSource(vertex_shader,5,list_of_source_code,NULL);
list_of_source_code[1]= fragment_shader_macro;
glShaderSource(fragment_shader,5,list_of_source_code,NULL);
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
/*:45*//*58:*/
#line 1297 "weaver-interface_en.tex"

static GLuint new_shader_from_file(const char*filename){
char*buffer;
size_t source_size;
FILE*fp;
GLuint shader_program;
fp= fopen(filename,"r");
if(fp==NULL)return 0;

fseek(fp,0,SEEK_END);
source_size= ftell(fp);
rewind(fp);

buffer= (char*)temporary_alloc(sizeof(char)*(source_size+1));
if(buffer==NULL)return 0;
fread(buffer,sizeof(char),source_size,fp);
buffer[source_size]= '\0';
shader_program= new_shader(buffer);
if(temporary_free!=NULL)temporary_free(buffer);
return shader_program;
}
/*:58*//*62:*/
#line 1513 "weaver-interface_en.tex"

static void initialize_model_view_matrix(struct interface*in){
GLfloat width= ((GLfloat)in->width)/((GLfloat)*window_width);
GLfloat height= ((GLfloat)in->height)/((GLfloat)*window_height);
GLfloat x= ((GLfloat)in->x)/((GLfloat)*window_width);
GLfloat y= ((GLfloat)in->y)/((GLfloat)*window_height);
GLfloat cos_theta= cos(in->rotation);
GLfloat sin_theta= sin(in->rotation);

in->_transform_matrix[0]= width*cos_theta;
in->_transform_matrix[1]= width*sin_theta;
in->_transform_matrix[2]= 0.0;
in->_transform_matrix[3]= 0.0;

in->_transform_matrix[4]= -height*sin_theta;
in->_transform_matrix[5]= height*cos_theta;
in->_transform_matrix[6]= 0.0;
in->_transform_matrix[7]= 0.0;

in->_transform_matrix[8]= 0.0;
in->_transform_matrix[9]= 0.0;
in->_transform_matrix[10]= 1.0;
in->_transform_matrix[11]= 0.0;

in->_transform_matrix[12]= -width/2*cos_theta+height/2*sin_theta+x;
in->_transform_matrix[13]= -width/2*sin_theta-height/2*cos_theta+y;
in->_transform_matrix[14]= 0.0;
in->_transform_matrix[15]= 1.0;
}
/*:62*/
#line 399 "weaver-interface_en.tex"

/*35:*/
#line 799 "weaver-interface_en.tex"

void _Winit_interface(int*window_width_p,int*window_height_p,
void*(*new_permanent_alloc)(size_t),
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
window_width= window_width_p;
window_height= window_height_p;
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
/*56:*/
#line 1271 "weaver-interface_en.tex"

default_shader_program= new_shader(default_shader_source);
/*:56*//*60:*/
#line 1343 "weaver-interface_en.tex"

{
GLubyte pixels[3]= {255,255,255};
glGenTextures(1,&default_texture);
glBindTexture(GL_TEXTURE_2D,default_texture);
glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,1,1,0,GL_RGB,GL_UNSIGNED_BYTE,
pixels);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}
/*:60*//*65:*/
#line 1579 "weaver-interface_en.tex"

MUTEX_INIT(&linked_list_mutex);
/*:65*/
#line 850 "weaver-interface_en.tex"

}
/*:35*//*37:*/
#line 882 "weaver-interface_en.tex"

void _Wfinish_interface(void){
/*43:*/
#line 1009 "weaver-interface_en.tex"

shader_library= "";
/*:43*//*57:*/
#line 1279 "weaver-interface_en.tex"

glDeleteProgram(default_shader_program);
/*:57*//*61:*/
#line 1359 "weaver-interface_en.tex"

glDeleteTextures(1,&default_texture);
/*:61*//*66:*/
#line 1587 "weaver-interface_en.tex"

MUTEX_DESTROY(&linked_list_mutex);
/*:66*/
#line 884 "weaver-interface_en.tex"

permanent_free(list_of_file_functions);
number_of_file_functions_in_the_list= 0;
permanent_alloc= malloc;
temporary_alloc= malloc;
permanent_free= free;
temporary_free= free;
before_loading_interface= NULL;
after_loading_interface= NULL;
}
/*:37*//*42:*/
#line 996 "weaver-interface_en.tex"

void _Wset_interface_shader_library(char*source){
shader_library= source;
}
/*:42*//*67:*/
#line 1601 "weaver-interface_en.tex"

struct interface*_Wnew_interface(char*filename,char*shader_filename,
float x,float y,float z,float width,
float height){
struct interface*new_interface;
void(*loading_function)(void*(*permanent_alloc)(size_t),
void(*permanent_free)(void*),
void*(*temporary_alloc)(size_t),
void(*temporary_free)(void*),
void*(*before_loading_interface)(void),
void*(*after_loading_interface)(void),
char*source_filename,struct interface*target);
int i;
new_interface= permanent_alloc(sizeof(struct interface));
if(new_interface!=NULL){
new_interface->type= TYPE_INTERFACE;
new_interface->next= NULL;
new_interface->x= x;
new_interface->y= y;
new_interface->z= z;
new_interface->rotation= 0;
new_interface->width= width;
new_interface->height= height;
for(i= 0;i<4;i++){
new_interface->background_color[i]= 0.0;
new_interface->foreground_color[i]= 0.0;
}
new_interface->integer= 0;
new_interface->visible= true;
initialize_model_view_matrix(new_interface);
if(shader_filename!=NULL)
new_interface->_shader= new_shader_from_file(shader_filename);
else
new_interface->_shader= default_shader_program;
new_interface->_texture1= NULL;
new_interface->_loaded_texture= false;
new_interface->animate= false;
new_interface->number_of_frames= 0;
new_interface->current_frame= 0;
new_interface->frame_duration= NULL;
new_interface->_t= 0;
new_interface->max_repetition= 0;
MUTEX_WAIT(&linked_list_mutex);
if(last_structure!=NULL)
((struct interface*)last_structure)->next= (void*)new_interface;
else
last_structure= (void*)new_interface;
MUTEX_SIGNAL(&linked_list_mutex);
if(filename!=NULL){
char*ext;
for(ext= filename;ext!='\0';ext++);
for(;*ext!='.'&&ext!=filename;ext--);
if(*ext=='.'){
ext++;
loading_function= get_loading_function(ext);
if(loading_function!=NULL)
loading_function(permanent_alloc,permanent_free,temporary_alloc,
temporary_free,before_loading_interface,
after_loading_interface,filename,new_interface);
}
}
}
return new_interface;
}
/*:67*//*68:*/
#line 1684 "weaver-interface_en.tex"

struct interface*_Wlink_interface(struct interface*i){
struct link*new_link= permanent_alloc(sizeof(struct link));
if(new_link==NULL)
return NULL;
new_link->type= TYPE_LINK;
new_link->next= NULL;
new_link->linked_interface= i;
MUTEX_WAIT(&linked_list_mutex);
if(last_structure!=NULL)
((struct interface*)last_structure)->next= (void*)new_link;
else
last_structure= (void*)new_link;
MUTEX_SIGNAL(&linked_list_mutex);
return i;
}
/*:68*//*69:*/
#line 1711 "weaver-interface_en.tex"

void _Wmark_history_interface(void){
struct marking*new_marking= permanent_alloc(sizeof(struct marking));
if(new_marking!=NULL){
new_marking->type= TYPE_MARKING;
new_marking->next= NULL;
new_marking->previous_marking= last_marking;
MUTEX_WAIT(&linked_list_mutex);
if(last_structure!=NULL)
((struct interface*)last_structure)->next= (void*)new_marking;
else
last_structure= (void*)new_marking;
MUTEX_SIGNAL(&linked_list_mutex);
}
}
/*:69*/
#line 400 "weaver-interface_en.tex"

/*:20*/
