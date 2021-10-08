/*20:*/
#line 415 "weaver-interface.tex"

#include "interface.h"
/*34:*/
#line 865 "weaver-interface.tex"

#include <stdarg.h> 
/*:34*//*42:*/
#line 1020 "weaver-interface.tex"

#include <stdio.h> 
/*:42*//*61:*/
#line 1538 "weaver-interface.tex"

#include <math.h> 
/*:61*/
#line 417 "weaver-interface.tex"

/*23:*/
#line 477 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#define MUTEX_INIT(mutex) pthread_mutex_init(mutex, NULL);
#elif defined(_WIN32)
#define MUTEX_INIT(mutex) InitializeCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#define MUTEX_INIT(mutex)
#endif
/*:23*//*24:*/
#line 492 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#define MUTEX_DESTROY(mutex) pthread_mutex_destroy(mutex);
#elif defined(_WIN32)
#define MUTEX_DESTROY(mutex) DeleteCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#define MUTEX_DESTROY(mutex)
#endif
/*:24*//*25:*/
#line 509 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#define MUTEX_WAIT(mutex) pthread_mutex_lock(mutex);
#elif defined(_WIN32)
#define MUTEX_WAIT(mutex) EnterCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#define MUTEX_WAIT(mutex)
#endif
/*:25*//*26:*/
#line 524 "weaver-interface.tex"

#if defined(__linux__) || defined(BSD)
#define MUTEX_SIGNAL(mutex) pthread_mutex_unlock(mutex);
#elif defined(_WIN32)
#define MUTEX_SIGNAL(mutex) LeaveCriticalSection(mutex);
#elif defined(__EMSCRIPTEN__)
#define MUTEX_SIGNAL(mutex)
#endif
/*:26*//*29:*/
#line 604 "weaver-interface.tex"

#define TYPE_INTERFACE 1 
#define TYPE_LINK      2 
#define TYPE_MARKING   3 
/*:29*//*36:*/
#line 929 "weaver-interface.tex"

#if !defined(W_GLSL_VERSION)
#define W_GLSL_VERSION "#version 100\n"
#endif
/*:36*/
#line 418 "weaver-interface.tex"

/*30:*/
#line 707 "weaver-interface.tex"

struct marking{
int type;
void*next;
struct marking*previous_marking;
};
/*:30*//*31:*/
#line 734 "weaver-interface.tex"

struct link{
int type;
void*next;
struct interface*linked_interface;
};
/*:31*//*32:*/
#line 766 "weaver-interface.tex"

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
#line 419 "weaver-interface.tex"

/*14:*/
#line 268 "weaver-interface.tex"

static void*(*permanent_alloc)(size_t)= malloc;
static void*(*temporary_alloc)(size_t)= malloc;
static void(*permanent_free)(void*)= free;
static void(*temporary_free)(void*)= free;
/*:14*//*15:*/
#line 284 "weaver-interface.tex"

static void*(*before_loading_interface)(void)= NULL;
static void*(*after_loading_interface)(void)= NULL;
/*:15*//*17:*/
#line 320 "weaver-interface.tex"

static int*window_width= NULL,*window_height= NULL;
/*:17*//*37:*/
#line 951 "weaver-interface.tex"

static const char vertex_shader_macro[]= "#define VERTEX_SHADER\n";
static const char fragment_shader_macro[]= "#define FRAGMENT_SHADER\n";
/*:37*//*38:*/
#line 973 "weaver-interface.tex"

static const char precision_qualifier[]= "precision highp float;\n"
"precision highp int;\n";
/*:38*//*39:*/
#line 985 "weaver-interface.tex"

static char*shader_library= "";
/*:39*//*46:*/
#line 1148 "weaver-interface.tex"

static const char default_shader_source[]= ""
/*50:*/
#line 1214 "weaver-interface.tex"

"#if defined(VERTEX_SHADER)\n"
"attribute vec3 vertex_position;\n"
"#endif\n"
/*:50*//*51:*/
#line 1229 "weaver-interface.tex"

"uniform vec4 foreground_color, background_color;\n"
"uniform mat4 model_view_matrix;\n"
"uniform vec2 interface_size;\n"
"uniform float time;\n"
"uniform int integer;\n"
"uniform sampler2D texture1;\n"
/*:51*//*52:*/
#line 1245 "weaver-interface.tex"

"varying mediump vec2 texture_coordinate;\n"
/*:52*/
#line 1150 "weaver-interface.tex"

"#if defined(VERTEX_SHADER)\n"
/*48:*/
#line 1186 "weaver-interface.tex"

"void main(){\n"
"  gl_Position = model_view_matrix * vec4(vertex_position, 1.0);\n"
"  texture_coordinate = vec2(vertex_position.x, vertex_position.y);\n"
"}\n"
/*:48*/
#line 1152 "weaver-interface.tex"

"#else\n"
/*49:*/
#line 1198 "weaver-interface.tex"

"void main(){\n"
"  vec4 texture = texture2D(texture1, texture_coordinate);\n"
"  gl_FragData[0] = texture;\n"
"}\n"
/*:49*/
#line 1154 "weaver-interface.tex"

"#endif\n"
"";
/*:46*//*47:*/
#line 1172 "weaver-interface.tex"

static const float interface_vertices[12]= {0.0,0.0,0.0,
1.0,0.0,0.0,1.0,1.0,0.0,0.0,1.0,0.0};
/*:47*//*53:*/
#line 1254 "weaver-interface.tex"

static GLuint default_shader_program;
/*:53*//*57:*/
#line 1318 "weaver-interface.tex"

static GLuint default_texture;
/*:57*//*62:*/
#line 1572 "weaver-interface.tex"

static void*last_structure= NULL,*last_marking= NULL;
/*:62*/
#line 420 "weaver-interface.tex"

/*43:*/
#line 1032 "weaver-interface.tex"

/*44:*/
#line 1076 "weaver-interface.tex"

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
#line 1104 "weaver-interface.tex"

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
#line 1033 "weaver-interface.tex"

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
/*:43*//*56:*/
#line 1288 "weaver-interface.tex"

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
/*:56*//*60:*/
#line 1502 "weaver-interface.tex"

static void set_model_view_matrix(struct interface*in){
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
/*:60*/
#line 421 "weaver-interface.tex"

/*33:*/
#line 796 "weaver-interface.tex"

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
/*54:*/
#line 1262 "weaver-interface.tex"

default_shader_program= new_shader(default_shader_source);
/*:54*//*58:*/
#line 1331 "weaver-interface.tex"

{
GLubyte pixels[3]= {255,255,255};
glGenTextures(1,&default_texture);
glBindTexture(GL_TEXTURE_2D,default_texture);
glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,1,1,0,GL_RGB,GL_UNSIGNED_BYTE,
pixels);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}
/*:58*/
#line 847 "weaver-interface.tex"

}
/*:33*//*35:*/
#line 881 "weaver-interface.tex"

void _Wfinish_interface(void){
/*41:*/
#line 1009 "weaver-interface.tex"

shader_library= "";
/*:41*//*55:*/
#line 1270 "weaver-interface.tex"

glDeleteProgram(default_shader_program);
/*:55*//*59:*/
#line 1347 "weaver-interface.tex"

glDeleteTextures(1,&default_texture);
/*:59*/
#line 883 "weaver-interface.tex"

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
#line 995 "weaver-interface.tex"

void _Wset_interface_shader_library(char*source){
shader_library= source;
}
/*:40*//*63:*/
#line 1587 "weaver-interface.tex"

struct interface*_Wnew_interface(char*filename,char*shader_filename,
float x,float y,float z,float width,
float height){
struct interface*new_interface;
if(before_loading_interface!=NULL)
before_loading_interface();
new_interface= permanent_alloc(sizeof(struct interface));
if(new_interface!=NULL){
new_interface->type= TYPE_INTERFACE;
new_interface->next= NULL;
new_interface->x= x;
new_interface->y= y;
new_interface->z= z;
new_interface->rotation= 0;

}
if(after_loading_interface!=NULL)
after_loading_interface();
return new_interface;
}
/*:63*/
#line 422 "weaver-interface.tex"

/*:20*/
