#pragma once
#define GL3_PROTOTYPES 1
#include <GLES3/gl3.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#ifndef __EMSCRIPTEN__ 
#define GLEW_STATIC
#include <GL/glew.h>
#endif

/**
 * Desktop only - load glProc addresses
 */
void vexInit() {
    #ifndef __EMSCRIPTEN__ 
    glewExperimental = GL_TRUE;
    glewInit();
    glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.
    #endif
}

/**
 * Decode the input events
 */
int event_type(SDL_Event* event){
    return event->type;
}
int KeyboardSym(SDL_Event* event){

    SDL_KeyboardEvent* e = (SDL_KeyboardEvent*)event;
    return e->keysym.sym;
}
int MouseX(SDL_Event* event){
    SDL_MouseMotionEvent* e = (SDL_MouseMotionEvent*)event;
    return e->x;
}
int MouseY(SDL_Event* event){
    SDL_MouseMotionEvent* e = (SDL_MouseMotionEvent*)event;
    return e->y ;
}
int ButtonX(SDL_Event* event){
    SDL_MouseButtonEvent* e = (SDL_MouseButtonEvent*)event;
    return e->x;
}
int ButtonY(SDL_Event* event){
    SDL_MouseButtonEvent* e = (SDL_MouseButtonEvent*)event;
    return e->y;
}
