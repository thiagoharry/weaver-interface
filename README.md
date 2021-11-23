# weaver-interface

This project is part of [Weaver Game
Engine](https://github.com/thiagoharry/weaver). It assumes that we
have an existing and functional OpenGL context and manages the
creation of user interfaces. An user interface is any rectangular
object which can have animated textures, custom shaders and can be
shown in the screen, resized, rotated and moved. It also could be
interacted with, using mouse clicks and movements.

Despite being part of the Weaver Game Engine, it can be integrated in
other projects. This is only a library with two files:
[interface.c](https://github.com/thiagoharry/weaver-interface/blob/master/src/interface.c)
and
[interface.h](https://github.com/thiagoharry/weaver-interface/blob/master/src/interface.h).

These files are self-contained. However, in some environments,
additional headers need to be included. For example, on Windows, the
default OpenGL header not necessarily include every OpenGL macro or
function. In these cases, some minor adjustment could be necessary.

This is written in Literary Programming. And the code is tested on
Linux, Open BSD, Windows 10 and Web Assembly. Therefore, if you plan
to study how this library works, you can read the full source code
with additional text explanations in
[weaver-interface_en.pdf](https://github.com/thiagoharry/weaver-interface/blob/master/weaver-interface.pdf)
(engish version) or in
[weaver-window.pdf](https://github.com/thiagoharry/weaver-interface/blob/master/weaver-interface.pdf)
(portuguese version).

This code is licensed under GNU Affero Gneral Public Licence v. 3. See
the LICENSE file.

## Sample Usage

This creates a white rotating square in the center of the screen:

    #include "interface.h"
    #include <time.h>

    int main(int argc, char **argv){
      // Put here something to create a window and OpenGL context...
      // ...
      int initial_time, current_time;
      struct user_interface *i;
      int window_width, window_height;
      // Put something to initialize the window width and height above, and
      // keep these variables updated with correct values
      _Winit_interface(&window_width, &window_height, // Pointer to window size
                       malloc, free,  // Functions for short-lived allocations
		       malloc, free,  // Functions for long-lived allocations
		       NULL, NULL, // Optional pointers to functions
		       NULL); // Final NULL to finish parameter list
      i = _Wnew_interface(NULL, // File with texture
                          NULL, // FIle with custom shader
	     	          window_width / 2, window_height / 2, // Center of screen
		          0, // z-index
		          250, 250); // Square 250x250 pixels
      initial_time = time(NULL);
      current_time = time(NULL);
      do{
        glClear(GL_COLOR_BUFFER_BIT); // Cleaning screen
	_Wrotate_interface(i, i -> rotation + 0.01);
	_Wrender_interface(current_time * 1000000);
	current_time = time(NULL);
      } while(current_time - initial_time > 3);
      _Wfinish_interface();
      // Put here something to close the window
      // ...
    }

## Dependencies

The code is portable, but it requires that you use other libraries to
create a window, keep its size updated, detect mouse movements,
measure elapsed time in microsseconds and initialize an OpenGL
Context. You can use any desired library to do these things.

## Reference

### Data Structures

#### User Interfaces

An user interface is a struct defined as:

  struct user_interface{
    float x, y, z; // User-interface coordinates
    float rotation;
    float height, width;
    float background_color[4], foreground_color[4]; int integer;
    bool visible;
    void (*on_mouse_over)(struct user_interface *);
    void (*on_mouse_out)(struct user_interface *);
    void (*on_mouse_left_down)(struct user_interface *);
    void (*on_mouse_left_up)(struct user_interface *);
    void (*on_mouse_middle_down)(struct user_interface *);
    void (*on_mouse_middle_up)(struct user_interface *);
    void (*on_mouse_right_down)(struct user_interface *);
    void (*on_mouse_right_up)(struct user_interface *);
    bool animate;
    unsigned number_of_frames;
    unsigned current_frame;
    unsigned *frame_duration;
    int max_repetition;
    _MUTEX_DECLARATION(mutex);
    // ...
  };

The user interface coordinates is the coordinate in pixels (`x', `y')
of the interface's center. The coordinate `z' is the
z-index. Interfaces with higher z-indices are drawn in front of
interface with smaller values. *All the coordinate values are
read-only.* If you want to change them, use the API function
`_Wmove_interface'.

The `rotation' measure the interface counter-clockwise rotation in
radians. *This value is read-only.* If you want to change it, use the
API function `_Wrotate_interface'.

The `height' and `width' store the interface size in pixels. *These
values are read-only.* If you want to change them, use the
API function `_Wresize_interface'.

The `background_color', `foregound_color' and `integer' are values
that are passed to shaders. If you create a custom shader, you can use
them. The default shader ignores them. Feel free to use these
variables if you want to store some state about the interface.

The `visible' variable determines if the interface is visible or
not. If not, it will not be drawn in the screen. All interfaces are
visible by default, but you can change this status changing this
variable.

All the `on_mouse...' functions are pointers to functions that should
be executed over this interface respectively when the mouse hover over
it, when the mouse hover out it, when the user presses the left mouse
over it, when it releases the left mouse button over the interface,
when it presses the middle mouse button, when releases the middle
mouse button, when presses the right mouse button and when releases
the right mouse button.

If we have an animated interface, you can turn on or off the animation
setting variable `animate'. Yo can check the number of frames for the
animation with `number_of_frames' variable. And in `frame_duration' we
have an allocated array the the duration in microseconds for each
frame. The variable `max_repetition', if positive, sets how many times
the animation should loop.

We also have a mutex on each interface. If you really need to interact
with it because you are using threads, the portable way to interact
with it is calling `MUTEX_WAIT(&i -> mutex)' and `MUTEX_SIGNAL(&i ->
mutex)'. Take care with deadlocks.

### Functions

#### Initialization

  void _Winit_interface(int *window_width, int *window_height,
                        void *(*permanent_alloc)(size_t),
                        void (*permanent_free)(void *),
                        void *(*temporary_alloc)(size_t),
                        void (*temporary_free)(void *),
                        void (*before_loading_interface)(void),
                        void (*after_loading_interface)(void),
                        ...);

This function should be called before using all other functions. It is
not thread-safe. You should call this function only once. It is safe
to call more than once only if you execute the finalization function
after each initialization.

The first two arguments are pointers to places where we can check the
window width and height. We expect that these variables will be kept
updated if the window is resized.

The next four functions are the allocators and disallocators. A pair
to make short-lived allocations and another to allocate memory that
should not be freed soon.  You can pass `NULL' as a disallocator. This
means that the memory will not be freed. This could be useful if you
have some sort of garbage collector. These parameters are useful for
people using custom memory managers. Otherwise, you can just use
`malloc' and `free', like in the sample code in the "Sample Usage"
section.

The ponters `before_loading_interface' and `after_loading_interface',
if non-NULL, are pointers for functions that should be executed
respectively before loading a new interface and after loading a new
interface. This could be useful if you load textures asynchronously
and want to know if there are still pending textures to be
loaded. Otherwise, you can just pass NULL.

The last parameter should be NULL. But beforet he final NULL, we can
pass pairs of parameters consisting of a string and a function
pointer. The string should be an extension (for example, "gif") and
the pointer should then point to a function that knows how to extract
a texture from a file and store in an interface (for example,
interpreting a "gif" file and create an animated interface).

For example, a function which extracts texture from a GIF file could
be:

  void gif_extractor_sample(void *(*permanent_alloc)(size_t),
		            void (*permanent_free)(void *),
		            void *(*temporary_alloc)(size_t),
		            void (*temporary_free)(void *),
		            void (*before_loading_interface)(void),
		            void (*after_loading_interface)(void),
		            char *source_filename, struct user_interface *i){
    if(before_loading_interface != NULL)
      before_loading_interface();
    char *buffer = (char *) temporary_alloc(4);
    EXTRACT_TEXTURE(filename, buffer);
    i -> _texture1 = (GLuint *) permanent_alloc(sizeof(GLuint));
    glGenTextures(1, target -> _texture1);
    glBindTexture(GL_TEXTURE_2D, target -> _texture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 buffer);
    glBindTexture(GL_TEXTURE_2D, 0);
    i -> _loaded_texture = true; // Always set this after loading the texture
    if(temporary_free != NULL)
      temporary_free(buffer);
    if(after_loading_interface != NULL)
      after_loading_interface();
  }

The API will call these functions automatically when it needs to
interpret a filename with some texture. Our API do not know how to
interpret any file format. It expects the user to supply the
functions to interpret the needed file formats in the correct format.

#### Finalization

  void _Wfinish_interface(void);

Call this function after you finished to use the API. After calling
this function, you should not use other API functions, except if you
reinitialize it again with `_Winit_interface'.

#### Creating New Interface

  struct user_interface *_Wnew_interface(char *filename, char *shader_filename,
                                        float x, float y, float z, float width,
                                        float height);

An user interface is created with the above function The first
argument is a file with the interface texture. If the argument is NULL
or if the API do not know how to parse a file with that extension, the
interface is initialized with a default solid white texture. The
second argument represents a file with a custom shader. If NULL, we
will use a default shader shown below. Next we have the interface
coordinate (x, y) and the z-index. Finally we have the interface width
and height in pixels.

Interfaces can be destroyed only using
`_Wrestore_history_interface'. However, this function erases
all interfaces from the current scope. Scopes can be managed with
`_Wmark_history_interface', `_Wrestore_history_interface' and
`_Wlink_interface'.

To create custom shaders, you should write using GLSL language. You
could begin changing the following model taken from the default shader:

 #version 100
 
 #if GL_FRAGMENT_PRECISION_HIGH == 1
 precision highp float;
 precision highp int;
 #else
 precision mediump float;
 precision mediump int;
 #endif
 precision lowp sampler2D;
 precision lowp samplerCube;
 
 attribute vec3 vertex_position; 
 uniform vec4 foreground_color, background_color;
 uniform mat4 model_view_matrix;
 uniform float time;
 uniform int integer;
 uniform sampler2D texture1;
 uniform vec2 size;
 varying mediump vec2 texture_coordinate;
 
 #if defined(VERTEX_SHADER)
 void main(){
   gl_Position = model_view_matrix * vec4(vertex_position, 1.0);
   texture_coordinate = vec2(vertex_position.x, vertex_position.y);
 }
 #endif
 #if defined(FRAGMENT_SHADER)
 void main(){
   vec4 texture = texture2D(texture1, texture_coordinate);
   gl_FragData[0] = texture;
 }
 #endif

Notice that both the vertex shader and the fragment shader should be
stored in the same file. The API is responsible to pass correct values
for the shader and to set correctly the macros `VEREX_SHADER' and
`FRAGMENT_SHADER' that you should check to discover if the code is run
as a fragment shader or vertex shader.

The time variable in the shader is measured in seconds, modulus 1
hour. The other variables are information taken from the interface
structure, described above in the section about data structures.


#### Setting Shader Libraries

  void _Wset_interface_shader_library(char *source);

You can define additional functions for your shaders passing all the
source code for this function. The defined functions can then be used
both in vertex shaders and in fragment shaders.

#### Moving Interfaces

  void _Wmove_interface(struct user_interface *i, float x, float y, float z);

This can be used both to move the interface to a new coordinate (x, y)
as to change the z-index value of the interface.

#### Rotating Interfaces

  void _Wrotate_interface(struct user_interface *i, float rotation);

This rotates the interface. The second parameter is the new
conter-clockwise angle in radians.

#### Resizing Interfaces

  void _Wresize_interface(struct user_interface *i, float new_width,
                          float new_height);

This resizes the interface gicing it a new width and height, measured
in pixels.

#### Rendering Interfaces

  void _Wrender_interface(unsigned long long time);

This renders all active interfaces. You should pass as parameter the
time in microseconds. The time parameter is important because this is
used by animated interfaces to determine when they should change and
update their frames.

Not all interfaces are active. Only interfaces created after the last
history marking are active. See the next function
`_Wmark_history_interface'.

#### Marking Interface History (Creating Scopes for Interfaces)

  void _Wmark_history_interface(void);

This creates a time marking to determine the active interfaces. After
runnng this function, only new interfaces created after this moment
will be active. Older interfaces will not be rendered and cannot be
interacted with until the marking is destroyed.

This function is used to determine which interfaces should be
rendered. If you enter in a submenu where all the previous interfaces
should not appear (but they should not be destroyed), just create a
history marking and then create new interfaces. When exitting the
submenu, destroy the history marking (see function
`_Wrestore_history_interface'). This destroys the interfaces created
in the submenu scope and the previous interfaces will be visible again.

#### Linking Interfaces (Accessing Interfaces from Other Scopes)

  struct user_interface *_Wlink_interface(struct user_interface *i);

This creates a link to an older interface passed as argument. A link
is like a newly created interface. you can interact with it, and any
change made to a link will be reflected in the linked interface.

This is useful if you used `_Wmark_history_interface', but you want to
show in the screen and allow interaction with some interface created
before the marking. Creating a link do this without needing to
recreate a new interface.

#### Interacting with Interfaces

  void _Winteract_interface(int mouse_x, int mouse_y, bool left_click,
                            bool middle_click, bool right_click);

This function should be run each iteration in your main loop and it
shows the mouse position in the screen (first two arguments), and also
if the user is clicking in one of the three mouse buttons. Knowing
this information, we can run automatically custom functions associated
with interaction with a given interface.

This works if you set to correct function pointers the variables
associated with mouse interaction in the user interface struct.

You can only interact with active interfaces. Interfaces older than
the last marking created with `_Wmark_history_interface' cannot be
interacted with, except if you created a link to them with
`_Wlink_interface'.

#### Erasing Interfaces and History Marking (Restore Previous Scope)

  void _Wrestore_history_interface(void);

This functions erases the last marking created with
`_Wmark_history_interface'. It also erases all interfaces created
after the last marking and all links to interfaces.

If you never created a history marking, running this function erases
all user interfaces.

