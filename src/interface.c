/*18:*/
#line 392 "weaver-interface.cweb"

#include "interface.h"
/*33:*/
#line 910 "weaver-interface.cweb"

#include <string.h> 
/*:33*//*35:*/
#line 979 "weaver-interface.cweb"

#include <stdarg.h> 
/*:35*//*44:*/
#line 1148 "weaver-interface.cweb"

#include <stdio.h> 
/*:44*//*63:*/
#line 1747 "weaver-interface.cweb"

#include <math.h> 
/*:63*/
#line 394 "weaver-interface.cweb"

/*15:*/
#line 285 "weaver-interface.cweb"

typedef void pointer_to_interface_function(void*(*)(size_t),void(*)(void*),
void*(*)(size_t),void(*)(void*),
void(*)(void),void(*)(void),
char*,struct user_interface*);
/*:15*//*21:*/
#line 457 "weaver-interface.cweb"

#if defined(__linux__) || defined(BSD)
#define MUTEX_INIT(mutex) pthread_mutex_init(mutex, NULL);
#elif defined(_WIN32)
#line 461 "weaver-interface.cweb"
#define MUTEX_INIT(mutex) InitializeCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#line 463 "weaver-interface.cweb"
#define MUTEX_INIT(mutex)
#endif
#line 465 "weaver-interface.cweb"
/*:21*//*22:*/
#line 472 "weaver-interface.cweb"

#if defined(__linux__) || defined(BSD)
#define MUTEX_DESTROY(mutex) pthread_mutex_destroy(mutex);
#elif defined(_WIN32)
#line 476 "weaver-interface.cweb"
#define MUTEX_DESTROY(mutex) DeleteCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#line 478 "weaver-interface.cweb"
#define MUTEX_DESTROY(mutex)
#endif
#line 480 "weaver-interface.cweb"
/*:22*//*23:*/
#line 489 "weaver-interface.cweb"

#if defined(__linux__) || defined(BSD)
#define MUTEX_WAIT(mutex) pthread_mutex_lock(mutex);
#elif defined(_WIN32)
#line 493 "weaver-interface.cweb"
#define MUTEX_WAIT(mutex) EnterCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#line 495 "weaver-interface.cweb"
#define MUTEX_WAIT(mutex)
#endif
#line 497 "weaver-interface.cweb"
/*:23*//*24:*/
#line 504 "weaver-interface.cweb"

#if defined(__linux__) || defined(BSD)
#define MUTEX_SIGNAL(mutex) pthread_mutex_unlock(mutex);
#elif defined(_WIN32)
#line 508 "weaver-interface.cweb"
#define MUTEX_SIGNAL(mutex) LeaveCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#line 510 "weaver-interface.cweb"
#define MUTEX_SIGNAL(mutex)
#endif
#line 512 "weaver-interface.cweb"
/*:24*//*27:*/
#line 589 "weaver-interface.cweb"

#define TYPE_INTERFACE 1 
#define TYPE_LINK      2 
#define TYPE_MARKING   3 
#define TYPE_SHADER    4 
/*:27*//*37:*/
#line 1044 "weaver-interface.cweb"

#if !defined(W_GLSL_VERSION)
#define W_GLSL_VERSION "#version 100\n"
#endif
#line 1048 "weaver-interface.cweb"
/*:37*/
#line 395 "weaver-interface.cweb"

/*25:*/
#line 541 "weaver-interface.cweb"

struct shader{
int type;
void*next;
GLuint program;

GLint attribute_vertex_position;
GLint uniform_foreground_color,uniform_background_color;
GLint uniform_model_view_matrix;
GLint uniform_interface_size;
GLint uniform_mouse_coordinate;
GLint uniform_time;
GLint uniform_integer;
GLint uniform_texture1;
};
/*:25*//*29:*/
#line 792 "weaver-interface.cweb"

struct marking{
int type;
void*next;
void*prev;
struct marking*previous_marking;
unsigned number_of_interfaces;
};
/*:29*//*30:*/
#line 821 "weaver-interface.cweb"

struct link{
int type;
void*next;
struct user_interface*linked_interface;
};
/*:30*//*31:*/
#line 853 "weaver-interface.cweb"

struct file_function{
char*extension;
void(*load_texture)(void*(*permanent_alloc)(size_t),
void(*permanent_free)(void*),
void*(*temporary_alloc)(size_t),
void(*temporary_free)(void*),
void(*before_loading_interface)(void),
void(*after_loading_interface)(void),
char*source_filename,struct user_interface*target);
};
static unsigned number_of_file_functions_in_the_list= 0;
static struct file_function*list_of_file_functions= NULL;
/*:31*/
#line 396 "weaver-interface.cweb"

/*11:*/
#line 221 "weaver-interface.cweb"

static void*(*permanent_alloc)(size_t)= malloc;
static void*(*temporary_alloc)(size_t)= malloc;
static void(*permanent_free)(void*)= free;
static void(*temporary_free)(void*)= free;
/*:11*//*12:*/
#line 237 "weaver-interface.cweb"

static void(*before_loading_interface)(void)= NULL;
static void(*after_loading_interface)(void)= NULL;
/*:12*//*14:*/
#line 273 "weaver-interface.cweb"

static int*window_width= NULL,*window_height= NULL;
/*:14*//*38:*/
#line 1066 "weaver-interface.cweb"

static const char vertex_shader_macro[]= "#define VERTEX_SHADER\n";
static const char fragment_shader_macro[]= "#define FRAGMENT_SHADER\n";
/*:38*//*39:*/
#line 1088 "weaver-interface.cweb"

static const char precision_qualifier[]= "precision highp float;\n"
"precision highp int;\n";
/*:39*//*40:*/
#line 1100 "weaver-interface.cweb"

static char*shader_library= "";
/*:40*//*43:*/
#line 1134 "weaver-interface.cweb"

static const char shader_variables[]= ""
/*54:*/
#line 1391 "weaver-interface.cweb"

"#if defined(VERTEX_SHADER)\n"
"attribute vec3 vertex_position;\n"
"attribute vec2 vertex_texture_coordinate;\n"
"#endif\n"
/*:54*//*55:*/
#line 1407 "weaver-interface.cweb"

"uniform vec4 foreground_color, background_color;\n"
"uniform mat4 model_view_matrix;\n"
"uniform vec2 interface_size;\n"
"uniform vec2 mouse_coordinate;\n"
"uniform float time;\n"
"uniform int integer;\n"
"uniform sampler2D texture1;\n"
/*:55*//*56:*/
#line 1424 "weaver-interface.cweb"

"varying mediump vec2 texture_coordinate;\n"
/*:56*/
#line 1136 "weaver-interface.cweb"

"";
/*:43*//*48:*/
#line 1277 "weaver-interface.cweb"

static const char default_shader_source[]= ""
"#if defined(VERTEX_SHADER)\n"
/*52:*/
#line 1362 "weaver-interface.cweb"

"void main(){\n"
"  gl_Position = model_view_matrix * vec4(vertex_position, 1.0);\n"
"  texture_coordinate = vertex_texture_coordinate;\n"
"}\n"
/*:52*/
#line 1280 "weaver-interface.cweb"

"#else\n"
/*53:*/
#line 1374 "weaver-interface.cweb"

"void main(){\n"
"  vec4 texture = texture2D(texture1, texture_coordinate);\n"
"  gl_FragData[0] = texture;\n"
"}\n"
/*:53*/
#line 1282 "weaver-interface.cweb"

"#endif\n"
"";
/*:48*//*49:*/
#line 1301 "weaver-interface.cweb"

static const float interface_vertices[20]= {
0.0,0.0,0.0,
0.0,0.0,
1.0,0.0,0.0,
1.0,0.0,
1.0,1.0,0.0,
1.0,1.0,
0.0,1.0,0.0,
0.0,1.0};
static GLuint interface_vbo;
/*:49*//*58:*/
#line 1474 "weaver-interface.cweb"

static GLuint default_texture;
/*:58*//*64:*/
#line 1767 "weaver-interface.cweb"

static void*last_structure= NULL;
static struct marking*last_marking= NULL;
_STATIC_MUTEX_DECLARATION(linked_list_mutex);
/*:64*//*66:*/
#line 1807 "weaver-interface.cweb"

struct shader*default_shader;
/*:66*//*78:*/
#line 2224 "weaver-interface.cweb"

static struct user_interface**z_list= NULL;
static unsigned z_list_size= 0;
_STATIC_MUTEX_DECLARATION(z_list_mutex);
/*:78*//*83:*/
#line 2343 "weaver-interface.cweb"

static unsigned long long previous_time= 0;
/*:83*//*89:*/
#line 2632 "weaver-interface.cweb"

static bool mouse_last_left_click= false,mouse_last_middle_click= false,
mouse_last_right_click= false;
/*:89*/
#line 397 "weaver-interface.cweb"

/*32:*/
#line 881 "weaver-interface.cweb"

static inline void(*get_loading_function(char*ext))
(void*(*permanent_alloc)(size_t),
void(*permanent_free)(void*),
void*(*temporary_alloc)(size_t),
void(*temporary_free)(void*),
void(*before_loading_interface)(void),
void(*after_loading_interface)(void),
char*source_filename,struct user_interface*target){
unsigned i;
for(i= 0;i<number_of_file_functions_in_the_list;i++){
if(!strcmp(list_of_file_functions[i].extension,ext)){
return list_of_file_functions[i].load_texture;
}
}
return NULL;
}
/*:32*//*45:*/
#line 1160 "weaver-interface.cweb"

/*46:*/
#line 1205 "weaver-interface.cweb"

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
#line 1233 "weaver-interface.cweb"

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
#line 1261 "weaver-interface.cweb"
 return false;
}
/*:47*/
#line 1161 "weaver-interface.cweb"

static GLuint compile_shader(const char*source_code){
GLuint vertex_shader,fragment_shader,program;
const char*list_of_source_code[6];
vertex_shader= glCreateShader(GL_VERTEX_SHADER);
fragment_shader= glCreateShader(GL_FRAGMENT_SHADER);
list_of_source_code[0]= W_GLSL_VERSION;
list_of_source_code[1]= vertex_shader_macro;
list_of_source_code[2]= precision_qualifier;
list_of_source_code[3]= shader_library;
list_of_source_code[4]= shader_variables;
list_of_source_code[5]= source_code;
glShaderSource(vertex_shader,6,list_of_source_code,NULL);
list_of_source_code[1]= fragment_shader_macro;
glShaderSource(fragment_shader,6,list_of_source_code,NULL);
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
/*:45*//*57:*/
#line 1442 "weaver-interface.cweb"

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
/*:57*//*62:*/
#line 1705 "weaver-interface.cweb"

static void initialize_model_view_matrix(struct user_interface*i){
GLfloat x,y;
/*61:*/
#line 1695 "weaver-interface.cweb"

x= 2.0*(i->_x)/(*window_width)-1.0;
y= 2.0*(i->_y)/(*window_height)-1.0;
/*:61*/
#line 1708 "weaver-interface.cweb"

GLfloat cos_theta= cos(i->_rotation);
GLfloat sin_theta= sin(i->_rotation);

i->_transform_matrix[0]= (2*i->width/(*window_width))*
cos_theta;
i->_transform_matrix[1]= (2*i->width/(*window_height))*
sin_theta;
i->_transform_matrix[2]= 0.0;
i->_transform_matrix[3]= 0.0;

i->_transform_matrix[4]= -(2*i->height/(*window_width))*
sin_theta;
i->_transform_matrix[5]= (2*i->height/(*window_height))*
cos_theta;
i->_transform_matrix[6]= 0.0;
i->_transform_matrix[7]= 0.0;

i->_transform_matrix[8]= 0.0;
i->_transform_matrix[9]= 0.0;
i->_transform_matrix[10]= 1.0;
i->_transform_matrix[11]= 0.0;

i->_transform_matrix[12]= x+
(i->height/(*window_width))*sin_theta-
(i->width/(*window_width))*cos_theta;
i->_transform_matrix[13]= y-
(i->width/(*window_height))*sin_theta-
(i->height/(*window_height))*cos_theta;
i->_transform_matrix[14]= 0.0;
i->_transform_matrix[15]= 1.0;
}
/*:62*//*67:*/
#line 1822 "weaver-interface.cweb"

static struct shader*new_shader(char*shader_source){
struct shader*new= (struct shader*)permanent_alloc(sizeof(struct shader));
if(new!=NULL){
new->type= TYPE_SHADER;
new->next= NULL;
if(shader_source==NULL)
new->program= compile_shader(default_shader_source);
else
new->program= compile_shader_from_file(shader_source);

glBindAttribLocation(new->program,0,"vertex_position");
glBindAttribLocation(new->program,1,"vertex_texture_coordinate");

new->uniform_foreground_color= glGetUniformLocation(new->program,
"foreground_color");
new->uniform_background_color= glGetUniformLocation(new->program,
"background_color");
new->uniform_model_view_matrix= glGetUniformLocation(new->program,
"model_view_matrix");
new->uniform_interface_size= glGetUniformLocation(new->program,
"interface_size");
new->uniform_mouse_coordinate= glGetUniformLocation(new->program,
"mouse_coordinate");
new->uniform_time= glGetUniformLocation(new->program,"time");
new->uniform_integer= glGetUniformLocation(new->program,"integer");
new->uniform_texture1= glGetUniformLocation(new->program,"texture1");
if(shader_source!=NULL){
MUTEX_WAIT(&linked_list_mutex);
if(last_structure!=NULL)
((struct user_interface*)last_structure)->next= (void*)new;
last_structure= (void*)new;
MUTEX_SIGNAL(&linked_list_mutex);
}
}
return new;
}
/*:67*//*69:*/
#line 1882 "weaver-interface.cweb"

static void destroy_shader(struct shader*shader_struct){
glDeleteProgram(shader_struct->program);
if(permanent_free!=NULL)
permanent_free(shader_struct);
}
/*:69*//*72:*/
#line 2014 "weaver-interface.cweb"

static void destroy_interface(struct user_interface*interface_struct){
if(interface_struct->_texture1!=NULL){
glDeleteTextures(interface_struct->number_of_frames,
interface_struct->_texture1);
if(permanent_free!=NULL)
permanent_free(interface_struct->_texture1);
}
if(interface_struct->frame_duration!=NULL&&permanent_free!=NULL)
permanent_free(interface_struct->frame_duration);
MUTEX_DESTROY(&(interface_struct->mutex));
if(interface_struct->_free_internal_data!=NULL&&
interface_struct->_internal_data!=NULL){
interface_struct->_free_internal_data(interface_struct->_internal_data);
}
if(permanent_free!=NULL)
permanent_free(interface_struct);
}
/*:72*/
#line 398 "weaver-interface.cweb"

/*34:*/
#line 922 "weaver-interface.cweb"

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
va_arg(args,pointer_to_interface_function*);
}while(ext!=NULL);
number_of_file_functions_in_the_list= count;
list_of_file_functions= (struct file_function*)
permanent_alloc(sizeof(struct file_function)*
count);
va_start(args,new_after_loading_interface);
for(i= 0;i<count;i++){
list_of_file_functions[i].extension= va_arg(args,char*);
list_of_file_functions[i].load_texture= 
va_arg(args,pointer_to_interface_function*);
}
}
/*50:*/
#line 1336 "weaver-interface.cweb"

glGenBuffers(1,&interface_vbo);
glBindBuffer(GL_ARRAY_BUFFER,interface_vbo);

glBufferData(GL_ARRAY_BUFFER,sizeof(interface_vertices),interface_vertices,
GL_STATIC_DRAW);
/*:50*//*59:*/
#line 1484 "weaver-interface.cweb"

{
GLubyte pixels[3]= {255,255,255};
glGenTextures(1,&default_texture);
glBindTexture(GL_TEXTURE_2D,default_texture);
glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,1,1,0,GL_RGB,GL_UNSIGNED_BYTE,
pixels);
}
/*:59*//*65:*/
#line 1778 "weaver-interface.cweb"

MUTEX_INIT(&linked_list_mutex);
/*:65*//*68:*/
#line 1871 "weaver-interface.cweb"

default_shader= new_shader(NULL);
/*:68*//*75:*/
#line 2103 "weaver-interface.cweb"

_Wmark_history_interface();
/*:75*//*79:*/
#line 2235 "weaver-interface.cweb"

MUTEX_INIT(&z_list_mutex);
z_list_size= 0;
z_list= NULL;
/*:79*//*84:*/
#line 2353 "weaver-interface.cweb"

previous_time= 0;
/*:84*//*90:*/
#line 2642 "weaver-interface.cweb"

mouse_last_left_click= false;
mouse_last_middle_click= false;
mouse_last_right_click= false;
/*:90*/
#line 961 "weaver-interface.cweb"

}
/*:34*//*36:*/
#line 995 "weaver-interface.cweb"

void _Wfinish_interface(void){
/*42:*/
#line 1124 "weaver-interface.cweb"

shader_library= "";
/*:42*//*51:*/
#line 1348 "weaver-interface.cweb"

glDeleteBuffers(1,&interface_vbo);
/*:51*//*60:*/
#line 1498 "weaver-interface.cweb"

glDeleteTextures(1,&default_texture);
/*:60*//*70:*/
#line 1894 "weaver-interface.cweb"

destroy_shader(default_shader);
/*:70*//*76:*/
#line 2109 "weaver-interface.cweb"


while(last_marking->previous_marking!=NULL){
_Wrestore_history_interface();
}

_Wrestore_history_interface();

if(permanent_free!=NULL)
permanent_free(last_marking);
last_marking= NULL;
last_structure= NULL;
MUTEX_DESTROY(&linked_list_mutex);
/*:76*//*80:*/
#line 2247 "weaver-interface.cweb"

MUTEX_DESTROY(&z_list_mutex);
if(z_list!=NULL&&permanent_free!=NULL)
permanent_free(z_list);
z_list= NULL;
z_list_size= 0;
/*:80*/
#line 997 "weaver-interface.cweb"

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
/*:36*//*41:*/
#line 1110 "weaver-interface.cweb"

void _Wset_interface_shader_library(char*source){
shader_library= source;
}
/*:41*//*71:*/
#line 1908 "weaver-interface.cweb"

struct user_interface*_Wnew_interface(char*filename,char*shader_filename,
float x,float y,float z,float width,
float height){
struct user_interface*new_interface;
void(*loading_function)(void*(*permanent_alloc)(size_t),
void(*permanent_free)(void*),
void*(*temporary_alloc)(size_t),
void(*temporary_free)(void*),
void(*before_loading_interface)(void),
void(*after_loading_interface)(void),
char*source_filename,struct user_interface*target);
int i;
new_interface= permanent_alloc(sizeof(struct user_interface));
if(new_interface!=NULL){
new_interface->type= TYPE_INTERFACE;
new_interface->next= NULL;
new_interface->x= new_interface->_x= x;
new_interface->y= new_interface->_y= y;
new_interface->rotation= new_interface->_rotation= 0;
#if defined(W_FORCE_LANDSCAPE)
if(*window_height> *window_width){
new_interface->_x= *window_width-y;
new_interface->_y= x;
new_interface->_rotation+= M_PI_2;
}
#endif
#line 1935 "weaver-interface.cweb"
 new_interface->z= z;
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
new_interface->number_of_frames= 1;
new_interface->current_frame= 0;
new_interface->frame_duration= NULL;
new_interface->_t= 0;
new_interface->max_repetition= 0;
MUTEX_INIT(&(new_interface->mutex));
new_interface->_mouse_over= false;
new_interface->on_mouse_over= NULL;
new_interface->on_mouse_out= NULL;
new_interface->on_mouse_left_down= NULL;
new_interface->on_mouse_left_up= NULL;
new_interface->on_mouse_middle_down= NULL;
new_interface->on_mouse_middle_up= NULL;
new_interface->on_mouse_right_down= NULL;
new_interface->on_mouse_right_up= NULL;
new_interface->_internal_data= NULL;
new_interface->_free_internal_data= NULL;
new_interface->_reload_texture= NULL;
MUTEX_WAIT(&linked_list_mutex);
if(last_structure!=NULL)
((struct user_interface*)last_structure)->next= (void*)new_interface;
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
/*:71*//*73:*/
#line 2045 "weaver-interface.cweb"

struct user_interface*_Wlink_interface(struct user_interface*i){
struct link*new_link= permanent_alloc(sizeof(struct link));
if(new_link==NULL)
return NULL;
new_link->type= TYPE_LINK;
new_link->next= NULL;
new_link->linked_interface= i;
MUTEX_WAIT(&linked_list_mutex);
if(last_structure!=NULL)
((struct user_interface*)last_structure)->next= (void*)new_link;
last_structure= (void*)new_link;
last_marking->number_of_interfaces++;
MUTEX_SIGNAL(&linked_list_mutex);
return i;
}
/*:73*//*74:*/
#line 2076 "weaver-interface.cweb"

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
((struct user_interface*)last_structure)->next= (void*)new_marking;
last_structure= (void*)new_marking;
last_marking= new_marking;
MUTEX_SIGNAL(&linked_list_mutex);
}
}
/*:74*//*77:*/
#line 2151 "weaver-interface.cweb"

void _Wrestore_history_interface(void){
struct marking*to_be_removed;
struct user_interface*current,*next;
MUTEX_WAIT(&linked_list_mutex);
last_structure= last_marking->prev;
if(last_structure!=NULL)
((struct user_interface*)last_structure)->next= NULL;
to_be_removed= last_marking;
current= (struct user_interface*)to_be_removed->next;

while(current!=NULL){
next= (struct user_interface*)(current->next);
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
/*81:*/
#line 2261 "weaver-interface.cweb"

MUTEX_WAIT(&z_list_mutex);
if(z_list!=NULL&&permanent_free!=NULL)
permanent_free(z_list);
z_list= NULL;
z_list_size= 0;
MUTEX_SIGNAL(&z_list_mutex);
/*:81*/
#line 2180 "weaver-interface.cweb"

MUTEX_SIGNAL(&linked_list_mutex);
}
/*:77*//*85:*/
#line 2379 "weaver-interface.cweb"

void _Wrender_interface(unsigned long long time){
/*82:*/
#line 2281 "weaver-interface.cweb"

if(z_list_size!=last_marking->number_of_interfaces){
void*p;
unsigned i,j;
MUTEX_WAIT(&z_list_mutex);

if(z_list!=NULL&&permanent_free!=NULL)
permanent_free(z_list);
z_list_size= last_marking->number_of_interfaces;
z_list= (struct user_interface**)
permanent_alloc(sizeof(struct user_interface*)*z_list_size);

p= last_marking->next;
for(i= 0;i<z_list_size;i++){
if(((struct user_interface*)p)->type==TYPE_INTERFACE)
z_list[i]= (struct user_interface*)p;
else if(((struct user_interface*)p)->type==TYPE_LINK)
z_list[i]= ((struct link*)p)->linked_interface;
else if(((struct user_interface*)p)->type==TYPE_SHADER)
i--;
p= ((struct user_interface*)p)->next;
}

for(i= 1;i<z_list_size;i++){
j= i;
while(j> 0&&z_list[j-1]->z> z_list[j]->z){
p= z_list[j];
z_list[j]= z_list[j-1];
z_list[j-1]= (struct user_interface*)p;
j= j-1;
}
}
MUTEX_SIGNAL(&z_list_mutex);
}
/*:82*/
#line 2381 "weaver-interface.cweb"

{
unsigned i,elapsed_time;
if(previous_time!=0)
elapsed_time= (int)(time-previous_time);
else
elapsed_time= 0;

glBindBuffer(GL_ARRAY_BUFFER,interface_vbo);

glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),
(void*)0);
glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),
(void*)(3*sizeof(float)));
glEnableVertexAttribArray(0);
glEnableVertexAttribArray(1);
MUTEX_WAIT(&z_list_mutex);
for(i= 0;i<z_list_size;i++){
if(!(z_list[i]->_loaded_texture)||!(z_list[i]->visible))
continue;

if(z_list[i]->animate&&
z_list[i]->max_repetition!=0){
z_list[i]->_t+= elapsed_time;
z_list[i]->current_frame%= z_list[i]->number_of_frames;
while(z_list[i]->_t> 
z_list[i]->frame_duration[z_list[i]->current_frame]&&
z_list[i]->max_repetition!=0){
z_list[i]->_t-= 
z_list[i]->frame_duration[z_list[i]->current_frame];
z_list[i]->current_frame++;
z_list[i]->current_frame%= z_list[i]->number_of_frames;
if(z_list[i]->number_of_frames==1&&
z_list[i]->_reload_texture!=NULL){
z_list[i]->_reload_texture(z_list[i]);


glBindBuffer(GL_ARRAY_BUFFER,interface_vbo);
glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),
(void*)0);
glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),
(void*)(3*sizeof(float)));
glEnableVertexAttribArray(0);
glEnableVertexAttribArray(1);



if(z_list[i]->_t> z_list[i]->frame_duration[0])
z_list[i]->_t= z_list[i]->frame_duration[0];
}
else if(z_list[i]->current_frame==0&&z_list[i]->max_repetition> 0)
z_list[i]->max_repetition--;
if(z_list[i]->frame_duration[z_list[i]->current_frame]==0)
break;
}
}

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
glUniform2f(z_list[i]->shader_program->uniform_mouse_coordinate,
z_list[i]->mouse_x,z_list[i]->mouse_y);

glUniform1f(z_list[i]->shader_program->uniform_time,
((double)(time%3600000000ull))/(double)(1000000.0));
glUniform1i(z_list[i]->shader_program->uniform_integer,
z_list[i]->integer);

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
/*:85*//*86:*/
#line 2489 "weaver-interface.cweb"

void _Wmove_interface(struct user_interface*i,
float new_x,float new_y,float new_z){
GLfloat x,y;
GLfloat cos_theta= cos(i->_rotation);
GLfloat sin_theta= sin(i->_rotation);
MUTEX_WAIT(&(i->mutex));
i->x= i->_x= new_x;
i->y= i->_y= new_y;
#if defined(W_FORCE_LANDSCAPE)
if(*window_height> *window_width){
i->_x= *window_width-new_y;
i->_y= new_x;
}
#endif
#line 2504 "weaver-interface.cweb"
/*61:*/
#line 1695 "weaver-interface.cweb"

x= 2.0*(i->_x)/(*window_width)-1.0;
y= 2.0*(i->_y)/(*window_height)-1.0;
/*:61*/
#line 2504 "weaver-interface.cweb"

i->_transform_matrix[12]= x+
(i->height/(*window_width))*sin_theta-
(i->width/(*window_width))*cos_theta;
i->_transform_matrix[13]= y-
(i->width/(*window_height))*sin_theta-
(i->height/(*window_height))*cos_theta;
if(new_z!=i->z){
unsigned j;
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
MUTEX_SIGNAL(&(i->mutex));
}
/*:86*//*87:*/
#line 2543 "weaver-interface.cweb"

void _Wrotate_interface(struct user_interface*i,float rotation){
GLfloat x,y;
GLfloat cos_theta= cos(rotation);
GLfloat sin_theta= sin(rotation);
MUTEX_WAIT(&(i->mutex));
i->rotation= i->_rotation= rotation;
#if defined(W_FORCE_LANDSCAPE)
if(*window_height> *window_width)
i->_rotation+= M_PI_2;
#endif
#line 2554 "weaver-interface.cweb"
/*61:*/
#line 1695 "weaver-interface.cweb"

x= 2.0*(i->_x)/(*window_width)-1.0;
y= 2.0*(i->_y)/(*window_height)-1.0;
/*:61*/
#line 2554 "weaver-interface.cweb"

i->_transform_matrix[0]= (2*i->width/(*window_width))*
cos_theta;
i->_transform_matrix[1]= (2*i->width/(*window_height))*
sin_theta;
i->_transform_matrix[4]= -(2*i->height/(*window_width))*
sin_theta;
i->_transform_matrix[5]= (2*i->height/(*window_height))*
cos_theta;
i->_transform_matrix[12]= x+
(i->height/(*window_width))*sin_theta-
(i->width/(*window_width))*cos_theta;
i->_transform_matrix[13]= y-
(i->width/(*window_height))*sin_theta-
(i->height/(*window_height))*cos_theta;
MUTEX_SIGNAL(&(i->mutex));
}
/*:87*//*88:*/
#line 2586 "weaver-interface.cweb"

void _Wresize_interface(struct user_interface*i,
float new_width,float new_height){
GLfloat x,y;
GLfloat cos_theta= cos(i->_rotation);
GLfloat sin_theta= sin(i->_rotation);
MUTEX_WAIT(&(i->mutex));
i->width= new_width;
i->height= new_height;
if(i->_reload_texture!=NULL)
i->_reload_texture(i);
/*61:*/
#line 1695 "weaver-interface.cweb"

x= 2.0*(i->_x)/(*window_width)-1.0;
y= 2.0*(i->_y)/(*window_height)-1.0;
/*:61*/
#line 2597 "weaver-interface.cweb"

i->_transform_matrix[0]= (2*i->width/(*window_width))*
cos_theta;
i->_transform_matrix[1]= (2*i->width/(*window_height))*
sin_theta;
i->_transform_matrix[4]= -(2*i->height/(*window_width))*
sin_theta;
i->_transform_matrix[5]= (2*i->height/(*window_height))*
cos_theta;
i->_transform_matrix[12]= x+
(i->height/(*window_width))*sin_theta-
(i->width/(*window_width))*cos_theta;
i->_transform_matrix[13]= y-
(i->width/(*window_height))*sin_theta-
(i->height/(*window_height))*cos_theta;
MUTEX_SIGNAL(&(i->mutex));
}
/*:88*//*91:*/
#line 2676 "weaver-interface.cweb"

void _Winteract_interface(int mouse_x,int mouse_y,bool left_click,
bool middle_click,bool right_click){
int i;
struct user_interface*previous= NULL,*current= NULL;
MUTEX_WAIT(&z_list_mutex);
for(i= z_list_size-1;i>=0;i--){
float x,y;
/*92:*/
#line 2755 "weaver-interface.cweb"

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
/*:92*/
#line 2684 "weaver-interface.cweb"

z_list[i]->mouse_x= x-z_list[i]->x+(z_list[i]->width/2);
z_list[i]->mouse_y= y-z_list[i]->y+(z_list[i]->height/2);
if(current==NULL&&
z_list[i]->mouse_x> 0&&z_list[i]->mouse_x<z_list[i]->width&&
z_list[i]->mouse_y> 0&&z_list[i]->mouse_y<z_list[i]->height)
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
/*:91*/
#line 399 "weaver-interface.cweb"

/*:18*/
