/**
 * XNA like framework
 * 
 * Copyright (c) 2019 darkoverlordofdata MIT license
 */
 
module vex

#flag linux -lSDL2
#flag linux -lSDL2_image
#flag linux -lGLEW
#flag linux -lGL
#flag linux  -I @VROOT/thirdparty/vex

#include "vex.h"

const (
    SDL_WINDOWPOS_CENTERED = 0
    SDL_WINDOW_OPENGL = 2
    SDL_WINDOW_SHOWN = 4
    SDL_INIT_TIMER = 0x00000001
    SDL_INIT_AUDIO = 0x00000010
    SDL_INIT_VIDEO = 0x00000020
    SDL_INIT_EVENTS = 0x00004000
    SDL_QUIT = 0x0100
    SDL_KEYDOWN = 0x0300
    SDL_KEYUP = 0x0301
    SDL_MOUSEMOTION = 0x0400
    SDL_MOUSEBUTTONDOWN = 0x0401
    SDL_MOUSEBUTTONUP = 0x0402
    SDL_GL_DEPTH_SIZE = 6
    SDL_GL_DOUBLEBUFFER = 0x0c32
    SDL_GL_CONTEXT_MAJOR_VERSION = 17
    SDL_GL_CONTEXT_MINOR_VERSION = 18
    IMG_INIT_PNG = 0x00000002
    SDLK_ESCAPE = 27
)

const (
    GL_CULL_FACE =                  0x00000b44
    GL_BLEND =                      0x0be2
    GL_SRC_ALPHA =                  0x0302
    GL_ONE_MINUS_SRC_ALPHA =        0x0303
    GL_COLOR_BUFFER_BIT =           0x00004000
    GL_DEPTH_BUFFER_BIT =           0x000000100
    GL_STENCIL_BUFFER_BIT =         0x000000400
    GL_TEXTURE_1D =                 0x0de0
    GL_TEXTURE_2D =                 0x0de1
    GL_TEXTURE_WRAP_S = 		    0x2802
    GL_TEXTURE_WRAP_T = 		    0x2803
    GL_TEXTURE_MAG_FILTER = 		0x2800
    GL_TEXTURE_MIN_FILTER = 		0x2801
    GL_RGB = 					    0x1907
    GL_RGBA	= 				        0x1908
    GL_REPEAT = 				    0x2901
    GL_LINEAR =				        0x2601
    GL_UNSIGNED_BYTE =			    0x1401

)


struct Game {
pub mut:
	window voidptr
	title string
    width int
    height int
	x int
	y  int
    running bool
    mouseX int
    mouseY int
    mouseDown bool
    delta double
    keys []int
}

struct Config 
{
    title string
    width int
    height int
    major int
    minor int
    images int
}


struct Event {
pub:
    typ u32
    timestamp u32
    windowID u32
    pad0 int
    pad1 int
    pad2 int
    pad3 int
    pad4 int
    pad5 int
    pad6 int
    pad7 int
    pad8 int
    pad9 int
    pada int
}

struct Texture2D {
pub mut:
    id  u32
    width u32
    height u32
    internal_format u32
    image_format u32
    wrap_s u32
    wrap_t u32
    filter_min u32
    filter_mag u32
    path string
}

struct Shader {
pub:
    id u32
}

struct ResourceManager {
pub:
    shaders map[string]Shader
    textures map[string]Texture2D
}

struct SpriteRenderer {
pub:
    shader Shader
    quad_vao u32
}