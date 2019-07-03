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


// #define GL3_PROTOTYPES 1
// #include <GLES3/gl3.h>
// #include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
#include <string.h>
#include "vex.h"

struct Game {
pub mut:
	window      voidptr
	title       string
    width       int
    height      int
	x           int
	y           int
    running     bool
    mouseX      int
    mouseY      int
    mouseDown   bool
    delta       double
    keys        []int
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

struct KeyboardEvent {
pub:
    typ u32
    timestamp u32
    windowID u32
    state byte
    repeat byte
    scancode int // SDL_Scancode
    sym int     // SDL_Keycode
    mod u16
}

struct MouseMotionEvent {
pub:
    typ u32
    timestamp u32
    windowID u32
    which u32
    state u32
    x int
    y int
    xrel int
    yrel int
}

struct MouseButtonEvent {
pub:
    typ u32
    timestamp u32
    windowID u32
    which u32
    button byte
    state byte // SDL_PRESSED or SDL_RELEASED
    clicks byte
    x int
    y int

}

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
    GL_CULL_FACE = 0x00000b44
    GL_BLEND = 0x0be2
    GL_SRC_ALPHA = 0x0302
    GL_ONE_MINUS_SRC_ALPHA = 0x0303
    GL_COLOR_BUFFER_BIT = 0x00004000
    GL_DEPTH_BUFFER_BIT = 0x000000100
    GL_STENCIL_BUFFER_BIT = 0x000000400
    SDLK_ESCAPE = 27

)

pub fn create_game(title string, width int, height int) *Game 
{
    if C.SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_AUDIO) {
        println('Unable to Init SDL')
        return 0
    }

    C.SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3)
    C.SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0)
    C.SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1)
    C.SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24)

    if C.IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG {
        println('Unable to Init image')
    }

    w := C.SDL_CreateWindow(title.str, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN)
    maincontext := C.SDL_GL_CreateContext(w)

    if w == 0 {
        println('Unable to create window')
    }
    
    C.vexInit()

    C.glViewport(0, 0, width, height)
    C.glEnable(GL_CULL_FACE)
    C.glEnable(GL_BLEND)
    C.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)


    g := &Game{ window: w, title: title, running: true, keys: [0; 256] }
    return g
}

pub fn (g &Game) process_event()
{
    mut event := Event{}
    mut key := 0

//    return (SDL_KeyboardEvent)event->common.type)

    for C.SDL_PollEvent(&event) != 0 {
        switch event.typ {
            case  u32(SDL_QUIT):
                g.running = false

            case u32(SDL_KEYDOWN):
                key = C.KeyboardSym(&event)
                if key >= 0 && key < 256 {
                    g.keys[key] = 1
                }

            case u32(SDL_KEYUP):
                key = C.KeyboardSym(&event)
                if key >= 0 && key < 256 {
                    g.keys[key] = 0
                }

            case u32(SDL_MOUSEMOTION):
                g.mouseX = C.MouseX(&event)
                g.mouseY = C.MouseY(&event)
            
            case u32(SDL_MOUSEBUTTONDOWN):
                g.mouseDown = true
                g.mouseX = C.ButtonX(&event)
                g.mouseY = C.ButtonY(&event)
            
            case u32(SDL_MOUSEBUTTONUP):
                g.mouseDown = false
                g.mouseX = C.ButtonX(&event)
                g.mouseY = C.ButtonY(&event)

        }

    }

} 

pub fn (g &Game) is_running() bool {
    g.process_event()

    if g.keys[SDLK_ESCAPE] == 1 {
        g.running = false
    }
    return g.running
}

pub fn (g &Game) render()
{
    C.glClearColor(1.0, 0.0, 0.0, 1.0)
	C.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)

    C.SDL_GL_SwapWindow(g.window)
}

pub fn (g &Game) terminate() {
    C.SDL_DestroyWindow(g.window)
	C.IMG_Quit()
    C.SDL_Quit()

}

