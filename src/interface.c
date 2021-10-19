/*20:*/
#line 393 "weaver-interface_en.tex"

#include "interface.h"
/*35:*/
#line 837 "weaver-interface_en.tex"

#include <string.h> 
/*:35*//*37:*/
#line 918 "weaver-interface_en.tex"

#include <stdarg.h> 
/*:37*//*45:*/
#line 1073 "weaver-interface_en.tex"

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
/*:45*//*63:*/
#line 1601 "weaver-interface_en.tex"

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
#line 586 "weaver-interface_en.tex"

#define TYPE_INTERFACE 1 
#define TYPE_LINK      2 
#define TYPE_MARKING   3 
#define TYPE_SHADER    4 
/*:29*//*39:*/
#line 982 "weaver-interface_en.tex"

#if !defined(W_GLSL_VERSION)
#define W_GLSL_VERSION "#version 100\n"
#endif
/*:39*/
#line 396 "weaver-interface_en.tex"

/*27:*/
#line 541 "weaver-interface_en.tex"

struct shader{
int type;
void*next;
GLuint program;

GLint attribute_vertex_position;
GLint uniform_foreground_color,uniform_background_color;
GLint uniform_model_view_matrix;
GLint uniform_interface_size;
GLint uniform_time;
GLint uniform_integer;
GLint uniform_texture1;
};
/*:27*//*31:*/
#line 720 "weaver-interface_en.tex"

struct marking{
int type;
void*next;
void*prev;
struct marking*previous_marking;
unsigned number_of_interfaces;
};
/*:31*//*32:*/
#line 750 "weaver-interface_en.tex"

struct link{
int type;
void*next;
struct interface*linked_interface;
};
/*:32*//*33:*/
#line 782 "weaver-interface_en.tex"

struct file_function{
char*extension;
void(*load_texture)(void*(*permanent_alloc)(size_t),
void(*permanent_free)(void*),
void*(*temporary_alloc)(size_t),
void(*temporary_free)(void*),
void(*before_loading_interface)(void),
void(*after_loading_interface)(void),
char*source_filename,struct interface*target);
};
static unsigned number_of_file_functions_in_the_list= 0;
static struct file_function*list_of_file_functions= NULL;
/*:33*/
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

static void(*before_loading_interface)(void)= NULL;
static void(*after_loading_interface)(void)= NULL;
/*:15*//*17:*/
#line 302 "weaver-interface_en.tex"

static int*window_width= NULL,*window_height= NULL;
/*:17*//*40:*/
#line 1006 "weaver-interface_en.tex"

static const char vertex_shader_macro[]= "#define VERTEX_SHADER\n";
static const char fragment_shader_macro[]= "#define FRAGMENT_SHADER\n";
/*:40*//*41:*/
#line 1025 "weaver-interface_en.tex"

static const char precision_qualifier[]= "precision highp float;\n"
"precision highp int;\n";
/*:41*//*42:*/
#line 1037 "weaver-interface_en.tex"

static char*shader_library= "";
/*:42*//*49:*/
#line 1210 "weaver-interface_en.tex"

static const char default_shader_source[]= ""
/*55:*/
#line 1300 "weaver-interface_en.tex"

"#if defined(VERTEX_SHADER)\n"
"attribute vec3 vertex_position;\n"
"#endif\n"
/*:55*//*56:*/
#line 1315 "weaver-interface_en.tex"

"uniform vec4 foreground_color, background_color;\n"
"uniform mat4 model_view_matrix;\n"
"uniform vec2 interface_size;\n"
"uniform float time;\n"
"uniform int integer;\n"
"uniform sampler2D texture1;\n"
/*:56*//*57:*/
#line 1331 "weaver-interface_en.tex"

"varying mediump vec2 texture_coordinate;\n"
/*:57*/
#line 1212 "weaver-interface_en.tex"

"#if defined(VERTEX_SHADER)\n"
/*53:*/
#line 1271 "weaver-interface_en.tex"

"void main(){\n"
"  gl_Position = model_view_matrix * vec4(vertex_position, 1.0);\n"
"  texture_coordinate = vec2(vertex_position.x, vertex_position.y);\n"
"}\n"
/*:53*/
#line 1214 "weaver-interface_en.tex"

"#else\n"
/*54:*/
#line 1284 "weaver-interface_en.tex"

"void main(){\n"
"  vec4 texture = texture2D(texture1, texture_coordinate);\n"
"  gl_FragData[0] = texture;\n"
"}\n"
/*:54*/
#line 1216 "weaver-interface_en.tex"

"#endif\n"
"";
/*:49*//*50:*/
#line 1233 "weaver-interface_en.tex"

static const float interface_vertices[12]= {0.0,0.0,0.0,
1.0,0.0,0.0,1.0,1.0,0.0,0.0,1.0,0.0};
static GLuint interface_vbo;
/*:50*//*59:*/
#line 1380 "weaver-interface_en.tex"

static GLuint default_texture;
/*:59*//*64:*/
#line 1620 "weaver-interface_en.tex"

static void*last_structure= NULL;
static struct marking*last_marking= NULL;
_STATIC_MUTEX_DECLARATION(linked_list_mutex);
/*:64*//*67:*/
#line 1664 "weaver-interface_en.tex"

struct shader*default_shader;
/*:67*/
#line 398 "weaver-interface_en.tex"

/*34:*/
#line 809 "weaver-interface_en.tex"

static inline void(*get_loading_function(char*ext))
(void*(*permanent_alloc)(size_t),
void(*permanent_free)(void*),
void*(*temporary_alloc)(size_t),
void(*temporary_free)(void*),
void(*before_loading_interface)(void),
void(*after_loading_interface)(void),
char*source_filename,struct interface*target){
unsigned i;
for(i= 0;i<number_of_file_functions_in_the_list;i++){
if(!strcmp(list_of_file_functions[i].extension,ext))
return list_of_file_functions[i].load_texture;
}
return NULL;
}
/*:34*//*46:*/
#line 1094 "weaver-interface_en.tex"

/*47:*/
#line 1137 "weaver-interface_en.tex"

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
/*:47*//*48:*/
#line 1166 "weaver-interface_en.tex"

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
/*:48*/
#line 1095 "weaver-interface_en.tex"

static GLuint compile_shader(const char*source_code){
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
/*:46*//*58:*/
#line 1349 "weaver-interface_en.tex"

static GLuint compile_shader_from_file(const char*filename){
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
shader_program= compile_shader(buffer);
if(temporary_free!=NULL)temporary_free(buffer);
return shader_program;
}
/*:58*//*62:*/
#line 1565 "weaver-interface_en.tex"

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
/*:62*//*68:*/
#line 1679 "weaver-interface_en.tex"

static struct shader*new_shader(char*shader_source){
struct shader*new= (struct shader*)permanent_alloc(sizeof(struct shader));
if(new!=NULL){
new->type= TYPE_SHADER;
new->next= NULL;
if(shader_source==NULL)
new->program= compile_shader(default_shader_source);
else
new->program= compile_shader_from_file(shader_source);
new->attribute_vertex_position= glGetAttribLocation(new->program,
"vertex_position");
new->uniform_foreground_color= glGetUniformLocation(new->program,
"foreground_color");
new->uniform_background_color= glGetUniformLocation(new->program,
"background_color");
new->uniform_model_view_matrix= glGetUniformLocation(new->program,
"model_view_matrix");
new->uniform_interface_size= glGetUniformLocation(new->program,
"interface_size");
new->uniform_time= glGetUniformLocation(new->program,"time");
new->uniform_integer= glGetUniformLocation(new->program,"integer");
new->uniform_texture1= glGetUniformLocation(new->program,"texture1");
if(shader_source!=NULL){
MUTEX_WAIT(&linked_list_mutex);
if(last_structure!=NULL)
((struct interface*)last_structure)->next= (void*)new;
else
last_structure= (void*)new;
MUTEX_SIGNAL(&linked_list_mutex);
}
}
return new;
}
/*:68*//*70:*/
#line 1735 "weaver-interface_en.tex"

static void destroy_shader(struct shader*shader_struct){
glDeleteProgram(shader_struct->program);
if(permanent_free!=NULL)
permanent_free(shader_struct);
}
/*:70*//*73:*/
#line 1845 "weaver-interface_en.tex"

static void destroy_interface(struct interface*interface_struct){
if(interface_struct->_texture1!=NULL){
glDeleteTextures(interface_struct->number_of_frames,
interface_struct->_texture1);
if(permanent_free!=NULL)
permanent_free(interface_struct->_texture1);
}
if(interface_struct->frame_duration!=NULL&&permanent_free!=NULL)
permanent_free(interface_struct->frame_duration);
MUTEX_DESTROY(&(interface_struct->interface_mutex));
if(permanent_free!=NULL)
permanent_free(interface_struct);
}
/*:73*/
#line 399 "weaver-interface_en.tex"

/*36:*/
#line 850 "weaver-interface_en.tex"

void _Winit_interface(int*window_width_p,int*window_height_p,
void*(*new_permanent_alloc)(size_t),
void(*new_permanent_free)(void*),
void*(*new_temporary_alloc)(size_t),
void(*new_temporary_free)(void*),
void(*new_before_loading_interface)(void),
void(*new_after_loading_interface)(void),...){
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
void(*before_loading_interface)(void),
void(*after_loading_interface)(void),
char*source_filename,struct interface*target);
va_start(args,new_after_loading_interface);
do{
count++;
ext= va_arg(args,char*);
func= va_arg(args,void(*)(void*(*)(size_t),void(*)(void*),
void*(*)(size_t),void(*)(void*),
void(*)(void),void(*)(void),
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
void(*)(void),void(*)(void),
char*,struct interface*));
}
}
/*51:*/
#line 1246 "weaver-interface_en.tex"

glGenBuffers(1,&interface_vbo);
glBindBuffer(GL_ARRAY_BUFFER,interface_vbo);

glBufferData(GL_ARRAY_BUFFER,sizeof(interface_vertices),interface_vertices,
GL_STATIC_DRAW);
/*:51*//*60:*/
#line 1395 "weaver-interface_en.tex"

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
#line 1631 "weaver-interface_en.tex"

MUTEX_INIT(&linked_list_mutex);
/*:65*//*69:*/
#line 1725 "weaver-interface_en.tex"

default_shader= new_shader(NULL);
/*:69*//*76:*/
#line 1932 "weaver-interface_en.tex"

_Wmark_history_interface();
/*:76*/
#line 901 "weaver-interface_en.tex"

}
/*:36*//*38:*/
#line 933 "weaver-interface_en.tex"

void _Wfinish_interface(void){
/*44:*/
#line 1061 "weaver-interface_en.tex"

shader_library= "";
/*:44*//*52:*/
#line 1258 "weaver-interface_en.tex"

glDeleteBuffers(1,&interface_vbo);
/*:52*//*61:*/
#line 1411 "weaver-interface_en.tex"

glDeleteTextures(1,&default_texture);
/*:61*//*66:*/
#line 1639 "weaver-interface_en.tex"

MUTEX_DESTROY(&linked_list_mutex);
/*:66*//*71:*/
#line 1747 "weaver-interface_en.tex"

destroy_shader(default_shader);
/*:71*//*77:*/
#line 1938 "weaver-interface_en.tex"


while(last_marking->previous_marking!=NULL){
_Wrestore_history_interface();
}

_Wrestore_history_interface();

if(permanent_free!=NULL)
permanent_free(last_marking);
last_marking= NULL;
last_structure= NULL;
/*:77*/
#line 935 "weaver-interface_en.tex"

if(permanent_free!=NULL)
permanent_free(list_of_file_functions);
number_of_file_functions_in_the_list= 0;
permanent_alloc= malloc;
temporary_alloc= malloc;
permanent_free= free;
temporary_free= free;
before_loading_interface= NULL;
after_loading_interface= NULL;
}
/*:38*//*43:*/
#line 1048 "weaver-interface_en.tex"

void _Wset_interface_shader_library(char*source){
shader_library= source;
}
/*:43*//*72:*/
#line 1761 "weaver-interface_en.tex"

struct interface*_Wnew_interface(char*filename,char*shader_filename,
float x,float y,float z,float width,
float height){
struct interface*new_interface;
void(*loading_function)(void*(*permanent_alloc)(size_t),
void(*permanent_free)(void*),
void*(*temporary_alloc)(size_t),
void(*temporary_free)(void*),
void(*before_loading_interface)(void),
void(*after_loading_interface)(void),
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
new_interface->shader_program= new_shader(shader_filename);
else
new_interface->shader_program= default_shader;
new_interface->_texture1= NULL;
new_interface->_loaded_texture= false;
new_interface->animate= false;
new_interface->number_of_frames= 0;
new_interface->current_frame= 0;
new_interface->frame_duration= NULL;
new_interface->_t= 0;
new_interface->max_repetition= 0;
MUTEX_INIT(&(new_interface->interface_mutex));
MUTEX_WAIT(&linked_list_mutex);
if(last_structure!=NULL)
((struct interface*)last_structure)->next= (void*)new_interface;
else
last_structure= (void*)new_interface;
last_marking->number_of_interfaces++;
MUTEX_SIGNAL(&linked_list_mutex);
if(filename!=NULL){
char*ext;
for(ext= filename;*ext!='\0';ext++);
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
/*:72*//*74:*/
#line 1872 "weaver-interface_en.tex"

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
last_marking->number_of_interfaces++;
MUTEX_SIGNAL(&linked_list_mutex);
return i;
}
/*:74*//*75:*/
#line 1904 "weaver-interface_en.tex"

void _Wmark_history_interface(void){
struct marking*new_marking= permanent_alloc(sizeof(struct marking));
if(new_marking!=NULL){
new_marking->type= TYPE_MARKING;
new_marking->next= NULL;
new_marking->previous_marking= last_marking;
new_marking->number_of_interfaces= 0;
MUTEX_WAIT(&linked_list_mutex);
new_marking->prev= last_structure;
if(last_structure!=NULL)
((struct interface*)last_structure)->next= (void*)new_marking;
else
last_structure= (void*)new_marking;
last_marking= new_marking;
MUTEX_SIGNAL(&linked_list_mutex);
}
}
/*:75*//*78:*/
#line 1977 "weaver-interface_en.tex"

void _Wrestore_history_interface(void){
struct marking*to_be_removed;
struct interface*current,*next;
MUTEX_WAIT(&linked_list_mutex);
last_structure= last_marking->prev;
to_be_removed= last_marking;
current= (struct interface*)to_be_removed->next;

while(current!=NULL){
next= (struct interface*)(current->next);
if(current->type==TYPE_INTERFACE)
destroy_interface(current);
else if(current->type==TYPE_SHADER)
destroy_shader((struct shader*)current);
else if(permanent_free!=NULL)
permanent_free(current);
current= next;
}

if(to_be_removed->previous_marking!=NULL){
last_marking= to_be_removed->previous_marking;
if(permanent_free!=NULL)
permanent_free(to_be_removed);
}
else
to_be_removed->number_of_interfaces= 0;
MUTEX_SIGNAL(&linked_list_mutex);
}
/*:78*/
#line 400 "weaver-interface_en.tex"

/*:20*/
