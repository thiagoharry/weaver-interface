/*20:*/
#line 419 "weaver-interface.tex"

#include "interface.h"
/*35:*/
#line 881 "weaver-interface.tex"

#include <string.h> 
/*:35*//*37:*/
#line 962 "weaver-interface.tex"

#include <stdarg.h> 
/*:37*//*45:*/
#line 1118 "weaver-interface.tex"

#include <stdio.h> 
/*:45*//*63:*/
#line 1657 "weaver-interface.tex"

#include <math.h> 
/*:63*/
#line 421 "weaver-interface.tex"

/*23:*/
#line 484 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#define MUTEX_INIT(mutex) pthread_mutex_init(mutex, NULL);
#elif defined(_WIN32)
#define MUTEX_INIT(mutex) InitializeCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#define MUTEX_INIT(mutex)
#endif
/*:23*//*24:*/
#line 499 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#define MUTEX_DESTROY(mutex) pthread_mutex_destroy(mutex);
#elif defined(_WIN32)
#define MUTEX_DESTROY(mutex) DeleteCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#define MUTEX_DESTROY(mutex)
#endif
/*:24*//*25:*/
#line 516 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#define MUTEX_WAIT(mutex) pthread_mutex_lock(mutex);
#elif defined(_WIN32)
#define MUTEX_WAIT(mutex) EnterCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#define MUTEX_WAIT(mutex)
#endif
/*:25*//*26:*/
#line 531 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#define MUTEX_SIGNAL(mutex) pthread_mutex_unlock(mutex);
#elif defined(_WIN32)
#define MUTEX_SIGNAL(mutex) LeaveCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#define MUTEX_SIGNAL(mutex)
#endif
/*:26*//*29:*/
#line 615 "weaver-interface.tex"

#define TYPE_INTERFACE 1 
#define TYPE_LINK      2 
#define TYPE_MARKING   3 
#define TYPE_SHADER    4 
/*:29*//*39:*/
#line 1027 "weaver-interface.tex"

#if !defined(W_GLSL_VERSION)
#define W_GLSL_VERSION "#version 100\n"
#endif
/*:39*/
#line 422 "weaver-interface.tex"

/*27:*/
#line 568 "weaver-interface.tex"

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
#line 763 "weaver-interface.tex"

struct marking{
int type;
void*next;
void*prev;
struct marking*previous_marking;
unsigned number_of_interfaces;
};
/*:31*//*32:*/
#line 792 "weaver-interface.tex"

struct link{
int type;
void*next;
struct interface*linked_interface;
};
/*:32*//*33:*/
#line 824 "weaver-interface.tex"

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
#line 423 "weaver-interface.tex"

/*14:*/
#line 272 "weaver-interface.tex"

static void*(*permanent_alloc)(size_t)= malloc;
static void*(*temporary_alloc)(size_t)= malloc;
static void(*permanent_free)(void*)= free;
static void(*temporary_free)(void*)= free;
/*:14*//*15:*/
#line 288 "weaver-interface.tex"

static void(*before_loading_interface)(void)= NULL;
static void(*after_loading_interface)(void)= NULL;
/*:15*//*17:*/
#line 324 "weaver-interface.tex"

static int*window_width= NULL,*window_height= NULL;
/*:17*//*40:*/
#line 1049 "weaver-interface.tex"

static const char vertex_shader_macro[]= "#define VERTEX_SHADER\n";
static const char fragment_shader_macro[]= "#define FRAGMENT_SHADER\n";
/*:40*//*41:*/
#line 1071 "weaver-interface.tex"

static const char precision_qualifier[]= "precision highp float;\n"
"precision highp int;\n";
/*:41*//*42:*/
#line 1083 "weaver-interface.tex"

static char*shader_library= "";
/*:42*//*49:*/
#line 1246 "weaver-interface.tex"

static const char default_shader_source[]= ""
/*55:*/
#line 1348 "weaver-interface.tex"

"#if defined(VERTEX_SHADER)\n"
"attribute vec3 vertex_position;\n"
"#endif\n"
/*:55*//*56:*/
#line 1363 "weaver-interface.tex"

"uniform vec4 foreground_color, background_color;\n"
"uniform mat4 model_view_matrix;\n"
"uniform vec2 interface_size;\n"
"uniform float time;\n"
"uniform int integer;\n"
"uniform sampler2D texture1;\n"
/*:56*//*57:*/
#line 1379 "weaver-interface.tex"

"varying mediump vec2 texture_coordinate;\n"
/*:57*/
#line 1248 "weaver-interface.tex"

"#if defined(VERTEX_SHADER)\n"
/*53:*/
#line 1320 "weaver-interface.tex"

"void main(){\n"
"  gl_Position = model_view_matrix * vec4(vertex_position, 1.0);\n"
"  texture_coordinate = vec2(vertex_position.x, vertex_position.y);\n"
"}\n"
/*:53*/
#line 1250 "weaver-interface.tex"

"#else\n"
/*54:*/
#line 1332 "weaver-interface.tex"

"void main(){\n"
"  vec4 texture = texture2D(texture1, texture_coordinate);\n"
"  gl_FragData[0] = texture;\n"
"}\n"
/*:54*/
#line 1252 "weaver-interface.tex"

"#endif\n"
"";
/*:49*//*50:*/
#line 1270 "weaver-interface.tex"

static const float interface_vertices[12]= {0.0,0.0,0.0,
1.0,0.0,0.0,
1.0,1.0,0.0,
0.0,1.0,0.0};
static GLuint interface_vbo;
/*:50*//*59:*/
#line 1427 "weaver-interface.tex"

static GLuint default_texture;
/*:59*//*64:*/
#line 1677 "weaver-interface.tex"

static void*last_structure= NULL;
static struct marking*last_marking= NULL;
_STATIC_MUTEX_DECLARATION(linked_list_mutex);
/*:64*//*67:*/
#line 1722 "weaver-interface.tex"

struct shader*default_shader;
/*:67*//*79:*/
#line 2108 "weaver-interface.tex"

static struct interface**z_list= NULL;
static unsigned list_z_size= 0;
/*:79*//*83:*/
#line 2208 "weaver-interface.tex"

static unsigned long long previous_time= 0;
/*:83*/
#line 424 "weaver-interface.tex"

/*34:*/
#line 852 "weaver-interface.tex"

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
/*:34*//*46:*/
#line 1130 "weaver-interface.tex"

/*47:*/
#line 1174 "weaver-interface.tex"

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
#line 1202 "weaver-interface.tex"

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
#line 1131 "weaver-interface.tex"

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
#line 1397 "weaver-interface.tex"

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
#line 1615 "weaver-interface.tex"

static void initialize_model_view_matrix(struct interface*in){
GLfloat width= 2.0*((GLfloat)in->width)/((GLfloat)*window_width);
GLfloat height= 2.0*((GLfloat)in->height)/((GLfloat)*window_height);
GLfloat x= 2.0*((GLfloat)in->x)/((GLfloat)*window_width);
GLfloat y= 2.0*((GLfloat)in->y)/((GLfloat)*window_height);
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

in->_transform_matrix[12]= -3*width/2*cos_theta+
3*height/2*sin_theta+x;
in->_transform_matrix[13]= -3*width/2*sin_theta-
3*height/2*cos_theta+y;
in->_transform_matrix[14]= 0.0;
in->_transform_matrix[15]= 1.0;
}
/*:62*//*68:*/
#line 1737 "weaver-interface.tex"

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
/*:68*//*70:*/
#line 1793 "weaver-interface.tex"

static void destroy_shader(struct shader*shader_struct){
glDeleteProgram(shader_struct->program);
if(permanent_free!=NULL)
permanent_free(shader_struct);
}
/*:70*//*73:*/
#line 1903 "weaver-interface.tex"

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
#line 425 "weaver-interface.tex"

/*36:*/
#line 893 "weaver-interface.tex"

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
#line 1294 "weaver-interface.tex"

glGenBuffers(1,&interface_vbo);
glBindBuffer(GL_ARRAY_BUFFER,interface_vbo);

glBufferData(GL_ARRAY_BUFFER,sizeof(interface_vertices),interface_vertices,
GL_STATIC_DRAW);
/*:51*//*60:*/
#line 1440 "weaver-interface.tex"

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
#line 1688 "weaver-interface.tex"

MUTEX_INIT(&linked_list_mutex);
/*:65*//*69:*/
#line 1782 "weaver-interface.tex"

default_shader= new_shader(NULL);
/*:69*//*76:*/
#line 1988 "weaver-interface.tex"

_Wmark_history_interface();
/*:76*//*84:*/
#line 2218 "weaver-interface.tex"

previous_time= 0;
/*:84*/
#line 944 "weaver-interface.tex"

}
/*:36*//*38:*/
#line 978 "weaver-interface.tex"

void _Wfinish_interface(void){
/*44:*/
#line 1107 "weaver-interface.tex"

shader_library= "";
/*:44*//*52:*/
#line 1306 "weaver-interface.tex"

glDeleteBuffers(1,&interface_vbo);
/*:52*//*61:*/
#line 1456 "weaver-interface.tex"

glDeleteTextures(1,&default_texture);
/*:61*//*66:*/
#line 1696 "weaver-interface.tex"

MUTEX_DESTROY(&linked_list_mutex);
/*:66*//*71:*/
#line 1805 "weaver-interface.tex"

destroy_shader(default_shader);
/*:71*//*77:*/
#line 1994 "weaver-interface.tex"


while(last_marking->previous_marking!=NULL){
_Wrestore_history_interface();
}

_Wrestore_history_interface();

if(permanent_free!=NULL)
permanent_free(last_marking);
last_marking= NULL;
last_structure= NULL;
/*:77*//*80:*/
#line 2119 "weaver-interface.tex"

if(z_list!=NULL&&permanent_free!=NULL)
permanent_free(z_list);
z_list= NULL;
list_z_size= 0;
/*:80*/
#line 980 "weaver-interface.tex"

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
#line 1093 "weaver-interface.tex"

void _Wset_interface_shader_library(char*source){
shader_library= source;
}
/*:43*//*72:*/
#line 1819 "weaver-interface.tex"

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
#line 1930 "weaver-interface.tex"

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
/*:74*//*75:*/
#line 1961 "weaver-interface.tex"

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
/*:75*//*78:*/
#line 2035 "weaver-interface.tex"

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
/*81:*/
#line 2132 "weaver-interface.tex"

if(z_list!=NULL&&permanent_free!=NULL)
permanent_free(z_list);
z_list= NULL;
list_z_size= 0;
/*:81*/
#line 2064 "weaver-interface.tex"

MUTEX_SIGNAL(&linked_list_mutex);
}
/*:78*//*85:*/
#line 2244 "weaver-interface.tex"

void _Wrender_interface(unsigned long long time){
/*82:*/
#line 2150 "weaver-interface.tex"

if(list_z_size!=last_marking->number_of_interfaces){
void*p;
int i,j;

if(z_list!=NULL&&permanent_free!=NULL)
permanent_free(z_list);
list_z_size= last_marking->number_of_interfaces;
z_list= (struct interface**)permanent_alloc(sizeof(struct interface*)*
list_z_size);

p= last_marking->next;
for(i= 0;i<list_z_size;i++){
if(((struct interface*)p)->type==TYPE_INTERFACE)
z_list[i]= (struct interface*)p;
else
z_list[i]= ((struct link*)p)->linked_interface;
p= ((struct interface*)p)->next;
}

for(i= 1;i<list_z_size;i++){
j= i;
while(j> 0&&z_list[j-1]->z> z_list[j]->z){
p= z_list[j];
z_list[j]= z_list[j-1];
z_list[j-1]= (struct interface*)p;
j= j-1;
}
}
}
/*:82*/
#line 2246 "weaver-interface.tex"

{
int i,elapsed_time;
if(previous_time!=0)
elapsed_time= (int)(time-previous_time);
else
elapsed_time= 0;
glEnable(GL_CULL_FACE);
glCullFace(GL_BACK);

glBindBuffer(GL_ARRAY_BUFFER,interface_vbo);

glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,
(void*)0);
glEnableVertexAttribArray(0);
for(i= 0;i<list_z_size;i++){

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
z_list[i]->max_repetition!=0&&z_list[i]->animate){
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
glBindTexture(GL_TEXTURE_2D,0);
}
previous_time= time;
}
/*:85*/
#line 426 "weaver-interface.tex"

/*:20*/
