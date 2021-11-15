/*17:*/
#line 381 "weaver-interface.tex"

#include "interface.h"
/*32:*/
#line 859 "weaver-interface.tex"

#include <string.h> 
/*:32*//*34:*/
#line 934 "weaver-interface.tex"

#include <stdarg.h> 
/*:34*//*42:*/
#line 1090 "weaver-interface.tex"

#include <stdio.h> 
/*:42*//*61:*/
#line 1632 "weaver-interface.tex"

#include <math.h> 
/*:61*/
#line 383 "weaver-interface.tex"

/*20:*/
#line 446 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#define MUTEX_INIT(mutex) pthread_mutex_init(mutex, NULL);
#elif defined(_WIN32)
#define MUTEX_INIT(mutex) InitializeCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#define MUTEX_INIT(mutex)
#endif
/*:20*//*21:*/
#line 461 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#define MUTEX_DESTROY(mutex) pthread_mutex_destroy(mutex);
#elif defined(_WIN32)
#define MUTEX_DESTROY(mutex) DeleteCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#define MUTEX_DESTROY(mutex)
#endif
/*:21*//*22:*/
#line 478 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#define MUTEX_WAIT(mutex) pthread_mutex_lock(mutex);
#elif defined(_WIN32)
#define MUTEX_WAIT(mutex) EnterCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#define MUTEX_WAIT(mutex)
#endif
/*:22*//*23:*/
#line 493 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#define MUTEX_SIGNAL(mutex) pthread_mutex_unlock(mutex);
#elif defined(_WIN32)
#define MUTEX_SIGNAL(mutex) LeaveCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#define MUTEX_SIGNAL(mutex)
#endif
/*:23*//*26:*/
#line 577 "weaver-interface.tex"

#define TYPE_INTERFACE 1 
#define TYPE_LINK      2 
#define TYPE_MARKING   3 
#define TYPE_SHADER    4 
/*:26*//*36:*/
#line 999 "weaver-interface.tex"

#if !defined(W_GLSL_VERSION)
#define W_GLSL_VERSION "#version 100\n"
#endif
/*:36*/
#line 384 "weaver-interface.tex"

/*24:*/
#line 530 "weaver-interface.tex"

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
/*:24*//*28:*/
#line 741 "weaver-interface.tex"

struct marking{
int type;
void*next;
void*prev;
struct marking*previous_marking;
unsigned number_of_interfaces;
};
/*:28*//*29:*/
#line 770 "weaver-interface.tex"

struct link{
int type;
void*next;
struct interface*linked_interface;
};
/*:29*//*30:*/
#line 802 "weaver-interface.tex"

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
/*:30*/
#line 385 "weaver-interface.tex"

/*11:*/
#line 234 "weaver-interface.tex"

static void*(*permanent_alloc)(size_t)= malloc;
static void*(*temporary_alloc)(size_t)= malloc;
static void(*permanent_free)(void*)= free;
static void(*temporary_free)(void*)= free;
/*:11*//*12:*/
#line 250 "weaver-interface.tex"

static void(*before_loading_interface)(void)= NULL;
static void(*after_loading_interface)(void)= NULL;
/*:12*//*14:*/
#line 286 "weaver-interface.tex"

static int*window_width= NULL,*window_height= NULL;
/*:14*//*37:*/
#line 1021 "weaver-interface.tex"

static const char vertex_shader_macro[]= "#define VERTEX_SHADER\n";
static const char fragment_shader_macro[]= "#define FRAGMENT_SHADER\n";
/*:37*//*38:*/
#line 1043 "weaver-interface.tex"

static const char precision_qualifier[]= "precision highp float;\n"
"precision highp int;\n";
/*:38*//*39:*/
#line 1055 "weaver-interface.tex"

static char*shader_library= "";
/*:39*//*46:*/
#line 1218 "weaver-interface.tex"

static const char default_shader_source[]= ""
/*52:*/
#line 1320 "weaver-interface.tex"

"#if defined(VERTEX_SHADER)\n"
"attribute vec3 vertex_position;\n"
"#endif\n"
/*:52*//*53:*/
#line 1335 "weaver-interface.tex"

"uniform vec4 foreground_color, background_color;\n"
"uniform mat4 model_view_matrix;\n"
"uniform vec2 interface_size;\n"
"uniform float time;\n"
"uniform int integer;\n"
"uniform sampler2D texture1;\n"
/*:53*//*54:*/
#line 1351 "weaver-interface.tex"

"varying mediump vec2 texture_coordinate;\n"
/*:54*/
#line 1220 "weaver-interface.tex"

"#if defined(VERTEX_SHADER)\n"
/*50:*/
#line 1292 "weaver-interface.tex"

"void main(){\n"
"  gl_Position = model_view_matrix * vec4(vertex_position, 1.0);\n"
"  texture_coordinate = vec2(vertex_position.x, vertex_position.y);\n"
"}\n"
/*:50*/
#line 1222 "weaver-interface.tex"

"#else\n"
/*51:*/
#line 1304 "weaver-interface.tex"

"void main(){\n"
"  vec4 texture = texture2D(texture1, texture_coordinate);\n"
"  gl_FragData[0] = texture;\n"
"}\n"
/*:51*/
#line 1224 "weaver-interface.tex"

"#endif\n"
"";
/*:46*//*47:*/
#line 1242 "weaver-interface.tex"

static const float interface_vertices[12]= {0.0,0.0,0.0,
1.0,0.0,0.0,
1.0,1.0,0.0,
0.0,1.0,0.0};
static GLuint interface_vbo;
/*:47*//*56:*/
#line 1401 "weaver-interface.tex"

static GLuint default_texture;
/*:56*//*62:*/
#line 1652 "weaver-interface.tex"

static void*last_structure= NULL;
static struct marking*last_marking= NULL;
_STATIC_MUTEX_DECLARATION(linked_list_mutex);
/*:62*//*65:*/
#line 1697 "weaver-interface.tex"

struct shader*default_shader;
/*:65*//*77:*/
#line 2095 "weaver-interface.tex"

static struct interface**z_list= NULL;
static unsigned z_list_size= 0;
_STATIC_MUTEX_DECLARATION(z_list_mutex);
/*:77*//*82:*/
#line 2212 "weaver-interface.tex"

static unsigned long long previous_time= 0;
/*:82*//*88:*/
#line 2439 "weaver-interface.tex"

static bool mouse_last_left_click= false,mouse_last_middle_click= false,
mouse_last_right_click= false;
/*:88*/
#line 386 "weaver-interface.tex"

/*31:*/
#line 830 "weaver-interface.tex"

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
if(!strcmp(list_of_file_functions[i].extension,ext)){
return list_of_file_functions[i].load_texture;
}
}
return NULL;
}
/*:31*//*43:*/
#line 1102 "weaver-interface.tex"

/*44:*/
#line 1146 "weaver-interface.tex"

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
/*:44*//*45:*/
#line 1174 "weaver-interface.tex"

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
/*:45*/
#line 1103 "weaver-interface.tex"

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
/*:43*//*55:*/
#line 1369 "weaver-interface.tex"

static GLuint compile_shader_from_file(const char*filename){
char*buffer;
size_t source_size,ret;
FILE*fp;
GLuint shader_program;
fp= fopen(filename,"r");
if(fp==NULL)return 0;

fseek(fp,0,SEEK_END);
source_size= ftell(fp);

buffer= (char*)temporary_alloc(sizeof(char)*(source_size+1));
if(buffer==NULL)return 0;
do{
rewind(fp);
ret= fread(buffer,sizeof(char),source_size,fp);
}while(feof(fp)&&!ferror(fp)&&ret/sizeof(char)==source_size);
buffer[source_size]= '\0';
shader_program= compile_shader(buffer);
if(temporary_free!=NULL)temporary_free(buffer);
return shader_program;
}
/*:55*//*60:*/
#line 1598 "weaver-interface.tex"

static void initialize_model_view_matrix(struct interface*i){
GLfloat x,y,w,h;
/*59:*/
#line 1586 "weaver-interface.tex"

x= 2.0*(i->x)/(*window_width)-1.0;
y= 2.0*(i->y)/(*window_height)-1.0;
w= 2.0*(i->width)/(*window_width);
h= 2.0*(i->height)/(*window_height);
/*:59*/
#line 1601 "weaver-interface.tex"

GLfloat cos_theta= cos(i->rotation);
GLfloat sin_theta= sin(i->rotation);

i->_transform_matrix[0]= w*cos_theta;
i->_transform_matrix[1]= h*sin_theta;
i->_transform_matrix[2]= 0.0;
i->_transform_matrix[3]= 0.0;

i->_transform_matrix[4]= -w*sin_theta;
i->_transform_matrix[5]= h*cos_theta;
i->_transform_matrix[6]= 0.0;
i->_transform_matrix[7]= 0.0;

i->_transform_matrix[8]= 0.0;
i->_transform_matrix[9]= 0.0;
i->_transform_matrix[10]= 1.0;
i->_transform_matrix[11]= 0.0;

i->_transform_matrix[12]= -w/2*cos_theta+w/2*sin_theta+x;
i->_transform_matrix[13]= -h/2*sin_theta-h/2*cos_theta+y;
i->_transform_matrix[14]= 0.0;
i->_transform_matrix[15]= 1.0;
}
/*:60*//*66:*/
#line 1712 "weaver-interface.tex"

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
last_structure= (void*)new;
MUTEX_SIGNAL(&linked_list_mutex);
}
}
return new;
}
/*:66*//*68:*/
#line 1768 "weaver-interface.tex"

static void destroy_shader(struct shader*shader_struct){
glDeleteProgram(shader_struct->program);
if(permanent_free!=NULL)
permanent_free(shader_struct);
}
/*:68*//*71:*/
#line 1890 "weaver-interface.tex"

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
/*:71*/
#line 387 "weaver-interface.tex"

/*33:*/
#line 871 "weaver-interface.tex"

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
va_start(args,new_after_loading_interface);
do{
count++;
ext= va_arg(args,char*);
va_arg(args,void(*)(void*(*)(size_t),void(*)(void*),
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
/*48:*/
#line 1266 "weaver-interface.tex"

glGenBuffers(1,&interface_vbo);
glBindBuffer(GL_ARRAY_BUFFER,interface_vbo);

glBufferData(GL_ARRAY_BUFFER,sizeof(interface_vertices),interface_vertices,
GL_STATIC_DRAW);
/*:48*//*57:*/
#line 1411 "weaver-interface.tex"

{
GLubyte pixels[3]= {255,255,255};
glGenTextures(1,&default_texture);
glBindTexture(GL_TEXTURE_2D,default_texture);
glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,1,1,0,GL_RGB,GL_UNSIGNED_BYTE,
pixels);
}
/*:57*//*63:*/
#line 1663 "weaver-interface.tex"

MUTEX_INIT(&linked_list_mutex);
/*:63*//*67:*/
#line 1757 "weaver-interface.tex"

default_shader= new_shader(NULL);
/*:67*//*74:*/
#line 1975 "weaver-interface.tex"

_Wmark_history_interface();
/*:74*//*78:*/
#line 2106 "weaver-interface.tex"

MUTEX_INIT(&z_list_mutex);
z_list_size= 0;
z_list= NULL;
/*:78*//*83:*/
#line 2222 "weaver-interface.tex"

previous_time= 0;
/*:83*//*89:*/
#line 2449 "weaver-interface.tex"

mouse_last_left_click= false;
mouse_last_middle_click= false;
mouse_last_right_click= false;
/*:89*/
#line 916 "weaver-interface.tex"

}
/*:33*//*35:*/
#line 950 "weaver-interface.tex"

void _Wfinish_interface(void){
/*41:*/
#line 1079 "weaver-interface.tex"

shader_library= "";
/*:41*//*49:*/
#line 1278 "weaver-interface.tex"

glDeleteBuffers(1,&interface_vbo);
/*:49*//*58:*/
#line 1425 "weaver-interface.tex"

glDeleteTextures(1,&default_texture);
/*:58*//*64:*/
#line 1671 "weaver-interface.tex"

MUTEX_DESTROY(&linked_list_mutex);
/*:64*//*69:*/
#line 1780 "weaver-interface.tex"

destroy_shader(default_shader);
/*:69*//*75:*/
#line 1981 "weaver-interface.tex"


while(last_marking->previous_marking!=NULL){
_Wrestore_history_interface();
}

_Wrestore_history_interface();

if(permanent_free!=NULL)
permanent_free(last_marking);
last_marking= NULL;
last_structure= NULL;
/*:75*//*79:*/
#line 2118 "weaver-interface.tex"

MUTEX_DESTROY(&z_list_mutex);
if(z_list!=NULL&&permanent_free!=NULL)
permanent_free(z_list);
z_list= NULL;
z_list_size= 0;
/*:79*/
#line 952 "weaver-interface.tex"

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
/*:35*//*40:*/
#line 1065 "weaver-interface.tex"

void _Wset_interface_shader_library(char*source){
shader_library= source;
}
/*:40*//*70:*/
#line 1794 "weaver-interface.tex"

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
if(filename!=NULL)
new_interface->_loaded_texture= false;
else
new_interface->_loaded_texture= true;
new_interface->animate= false;
new_interface->number_of_frames= 0;
new_interface->current_frame= 0;
new_interface->frame_duration= NULL;
new_interface->_t= 0;
new_interface->max_repetition= 0;
MUTEX_INIT(&(new_interface->interface_mutex));
new_interface->_mouse_over= false;
new_interface->on_mouse_over= NULL;
new_interface->on_mouse_out= NULL;
new_interface->on_mouse_left_down= NULL;
new_interface->on_mouse_left_up= NULL;
new_interface->on_mouse_middle_down= NULL;
new_interface->on_mouse_middle_up= NULL;
new_interface->on_mouse_right_down= NULL;
new_interface->on_mouse_right_up= NULL;
MUTEX_WAIT(&linked_list_mutex);
if(last_structure!=NULL)
((struct interface*)last_structure)->next= (void*)new_interface;
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
/*:70*//*72:*/
#line 1917 "weaver-interface.tex"

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
last_structure= (void*)new_link;
last_marking->number_of_interfaces++;
MUTEX_SIGNAL(&linked_list_mutex);
return i;
}
/*:72*//*73:*/
#line 1948 "weaver-interface.tex"

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
last_structure= (void*)new_marking;
last_marking= new_marking;
MUTEX_SIGNAL(&linked_list_mutex);
}
}
/*:73*//*76:*/
#line 2022 "weaver-interface.tex"

void _Wrestore_history_interface(void){
struct marking*to_be_removed;
struct interface*current,*next;
MUTEX_WAIT(&linked_list_mutex);
last_structure= last_marking->prev;
if(last_structure!=NULL)
((struct interface*)last_structure)->next= NULL;
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
/*80:*/
#line 2132 "weaver-interface.tex"

MUTEX_WAIT(&z_list_mutex);
if(z_list!=NULL&&permanent_free!=NULL)
permanent_free(z_list);
z_list= NULL;
z_list_size= 0;
MUTEX_SIGNAL(&z_list_mutex);
/*:80*/
#line 2051 "weaver-interface.tex"

MUTEX_SIGNAL(&linked_list_mutex);
}
/*:76*//*84:*/
#line 2248 "weaver-interface.tex"

void _Wrender_interface(unsigned long long time){
/*81:*/
#line 2152 "weaver-interface.tex"

if(z_list_size!=last_marking->number_of_interfaces){
void*p;
int i,j;
MUTEX_WAIT(&z_list_mutex);

if(z_list!=NULL&&permanent_free!=NULL)
permanent_free(z_list);
z_list_size= last_marking->number_of_interfaces;
z_list= (struct interface**)permanent_alloc(sizeof(struct interface*)*
z_list_size);

p= last_marking->next;
for(i= 0;i<z_list_size;i++){
if(((struct interface*)p)->type==TYPE_INTERFACE)
z_list[i]= (struct interface*)p;
else
z_list[i]= ((struct link*)p)->linked_interface;
p= ((struct interface*)p)->next;
}

for(i= 1;i<z_list_size;i++){
j= i;
while(j> 0&&z_list[j-1]->z> z_list[j]->z){
p= z_list[j];
z_list[j]= z_list[j-1];
z_list[j-1]= (struct interface*)p;
j= j-1;
}
}
MUTEX_SIGNAL(&z_list_mutex);
}
/*:81*/
#line 2250 "weaver-interface.tex"

{
int i,elapsed_time;
if(previous_time!=0)
elapsed_time= (int)(time-previous_time);
else
elapsed_time= 0;

glBindBuffer(GL_ARRAY_BUFFER,interface_vbo);

glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,
(void*)0);
glEnableVertexAttribArray(0);
MUTEX_WAIT(&z_list_mutex);
for(i= 0;i<z_list_size;i++){
if(!(z_list[i]->_loaded_texture)||!(z_list[i]->visible))
continue;

glUseProgram(z_list[i]->shader_program->program);

glUniform4fv(z_list[i]->shader_program->uniform_foreground_color,4,
z_list[i]->foreground_color);
glUniform4fv(z_list[i]->shader_program->uniform_background_color,4,
z_list[i]->background_color);
glUniformMatrix4fv(z_list[i]->shader_program->
uniform_model_view_matrix,1,false,
z_list[i]->_transform_matrix);
glUniform2f(z_list[i]->shader_program->uniform_interface_size,
z_list[i]->width,z_list[i]->height);

glUniform1f(z_list[i]->shader_program->uniform_time,
((double)(time%3600000000ull))/(double)(1000000.0));
glUniform1i(z_list[i]->shader_program->uniform_integer,
z_list[i]->integer);

if(z_list[i]->animate&&z_list[i]->number_of_frames> 1&&
z_list[i]->max_repetition!=0){
z_list[i]->_t+= elapsed_time;
z_list[i]->current_frame%= z_list[i]->number_of_frames;
while(z_list[i]->_t>=
z_list[i]->frame_duration[z_list[i]->current_frame]){
z_list[i]->_t-= 
z_list[i]->frame_duration[z_list[i]->current_frame];
z_list[i]->current_frame++;
z_list[i]->current_frame%= z_list[i]->number_of_frames;
}
}

if(z_list[i]->_texture1!=NULL)
glBindTexture(GL_TEXTURE_2D,
z_list[i]->_texture1[z_list[i]->current_frame]);
else
glBindTexture(GL_TEXTURE_2D,default_texture);
glDrawArrays(GL_TRIANGLE_FAN,0,4);
}
MUTEX_SIGNAL(&z_list_mutex);
glBindTexture(GL_TEXTURE_2D,0);
}
previous_time= time;
}
/*:84*//*85:*/
#line 2330 "weaver-interface.tex"

void _Wmove_interface(struct interface*i,float new_x,float new_y,float new_z){
GLfloat x,y,w,h;
GLfloat cos_theta= cos(i->rotation);
GLfloat sin_theta= sin(i->rotation);
MUTEX_WAIT(&(i->interface_mutex));
i->x= new_x;
i->y= new_y;
/*59:*/
#line 1586 "weaver-interface.tex"

x= 2.0*(i->x)/(*window_width)-1.0;
y= 2.0*(i->y)/(*window_height)-1.0;
w= 2.0*(i->width)/(*window_width);
h= 2.0*(i->height)/(*window_height);
/*:59*/
#line 2338 "weaver-interface.tex"

i->_transform_matrix[12]= -w/2*cos_theta+w/2*sin_theta+x;
i->_transform_matrix[13]= -h/2*sin_theta-h/2*cos_theta+y;
if(new_z!=i->z){
int j;
i->z= new_z;
MUTEX_WAIT(&z_list_mutex);
for(j= 0;j<z_list_size;j++){
if(z_list[j]==i){
while(j> 0&&i->z<z_list[j-1]->z){
z_list[j]= z_list[j-1];
z_list[j-1]= i;
j--;
}
while(j<z_list_size-1&&i->z> z_list[j+1]->z){
z_list[j]= z_list[j+1];
z_list[j+1]= i;
j++;
}
}
}
MUTEX_SIGNAL(&z_list_mutex);
}
MUTEX_SIGNAL(&(i->interface_mutex));
}
/*:85*//*86:*/
#line 2373 "weaver-interface.tex"

void _Wrotate_interface(struct interface*i,float rotation){
GLfloat x,y,w,h;
GLfloat cos_theta= cos(rotation);
GLfloat sin_theta= sin(rotation);
MUTEX_WAIT(&(i->interface_mutex));
i->rotation= rotation;
/*59:*/
#line 1586 "weaver-interface.tex"

x= 2.0*(i->x)/(*window_width)-1.0;
y= 2.0*(i->y)/(*window_height)-1.0;
w= 2.0*(i->width)/(*window_width);
h= 2.0*(i->height)/(*window_height);
/*:59*/
#line 2380 "weaver-interface.tex"

i->_transform_matrix[0]= w*cos_theta;
i->_transform_matrix[1]= h*sin_theta;
i->_transform_matrix[4]= -w*sin_theta;
i->_transform_matrix[5]= h*cos_theta;
i->_transform_matrix[12]= -w/2*cos_theta+w/2*sin_theta+x;
i->_transform_matrix[13]= -h/2*sin_theta-h/2*cos_theta+y;
MUTEX_SIGNAL(&(i->interface_mutex));
}
/*:86*//*87:*/
#line 2404 "weaver-interface.tex"

void _Wresize_interface(struct interface*i,float new_width,float new_height){
GLfloat x,y,w,h;
GLfloat cos_theta= cos(i->rotation);
GLfloat sin_theta= sin(i->rotation);
MUTEX_WAIT(&(i->interface_mutex));
i->width= new_width;
i->height= new_height;
/*59:*/
#line 1586 "weaver-interface.tex"

x= 2.0*(i->x)/(*window_width)-1.0;
y= 2.0*(i->y)/(*window_height)-1.0;
w= 2.0*(i->width)/(*window_width);
h= 2.0*(i->height)/(*window_height);
/*:59*/
#line 2412 "weaver-interface.tex"

i->_transform_matrix[0]= w*cos_theta;
i->_transform_matrix[1]= h*sin_theta;
i->_transform_matrix[4]= -w*sin_theta;
i->_transform_matrix[5]= h*cos_theta;
i->_transform_matrix[12]= -w/2*cos_theta+w/2*sin_theta+x;
i->_transform_matrix[13]= -h/2*sin_theta-h/2*cos_theta+y;
MUTEX_SIGNAL(&(i->interface_mutex));
}
/*:87*//*90:*/
#line 2483 "weaver-interface.tex"

void _Winteract_interface(int mouse_x,int mouse_y,bool left_click,
bool middle_click,bool right_click){
int i;
struct interface*previous= NULL,*current= NULL;
MUTEX_WAIT(&z_list_mutex);
for(i= z_list_size-1;i>=0;i--){
float x,y;
/*91:*/
#line 2561 "weaver-interface.tex"

if(z_list[i]->rotation==0.0){
x= mouse_x;
y= mouse_y;
}
else{
float cos_theta= cos(-(z_list[i]->rotation));
float sin_theta= sin(-(z_list[i]->rotation));
x= (mouse_x-z_list[i]->x)*cos_theta-
(mouse_y-z_list[i]->y)*sin_theta;
y= (mouse_x-z_list[i]->x)*sin_theta+
(mouse_y-z_list[i]->y)*cos_theta;
x+= z_list[i]->x;
y+= z_list[i]->y;
}
/*:91*/
#line 2491 "weaver-interface.tex"

if(current==NULL&&z_list[i]->x-(z_list[i]->width/2)<x&&
z_list[i]->x+(z_list[i]->width/2)> x&&
z_list[i]->y-(z_list[i]->height/2)<y&&
z_list[i]->y+(z_list[i]->height/2)> y)
current= z_list[i];
else{
if(z_list[i]->_mouse_over){
z_list[i]->_mouse_over= false;
previous= z_list[i];
}
}
}
MUTEX_SIGNAL(&z_list_mutex);
if(previous!=NULL&&previous->on_mouse_out!=NULL){
previous->on_mouse_out(previous);
}
if(current!=NULL){
if(current->_mouse_over==false){
current->_mouse_over= true;
if(current->on_mouse_over!=NULL)
current->on_mouse_over(current);
}
if(left_click&&!mouse_last_left_click&&current->on_mouse_left_down)
current->on_mouse_left_down(current);
else if(!left_click&&mouse_last_left_click&&current->on_mouse_left_up)
current->on_mouse_left_up(current);
if(middle_click&&!mouse_last_middle_click&&
current->on_mouse_middle_down)
current->on_mouse_middle_down(current);
else if(!middle_click&&mouse_last_middle_click&&
current->on_mouse_middle_up)
current->on_mouse_middle_up(current);
if(right_click&&!mouse_last_right_click&&current->on_mouse_right_down)
current->on_mouse_right_down(current);
else if(!right_click&&mouse_last_right_click&&
current->on_mouse_right_up)
current->on_mouse_right_up(current);
}
mouse_last_left_click= left_click;
mouse_last_middle_click= middle_click;
mouse_last_right_click= right_click;
}
/*:90*/
#line 388 "weaver-interface.tex"

/*:17*/
