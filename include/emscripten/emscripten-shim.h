#pragma once
#if __EMSCRIPTEN__ 
#include <emscripten.h>
#else
    
typedef void (*em_arg_callback_func)(void*);

void emscripten_set_main_loop_arg(em_arg_callback_func func, void *arg, int fps, int simulate_infinite_loop)
{
    printf("emscripten_set_main_loop_arg stub\n");
}

#endif
