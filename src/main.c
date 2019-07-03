#define GL3_PROTOTYPES 1
#include <GLES3/gl3.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif

typedef struct Game {
    SDL_Window* window;
    char* title;
    int x;
    int y;
    int counter;
    int running;
    int mouseX;
    int mouseY;
    int mouseDown;
    double delta;
    int width;
    int height;
    int keys[256];
} Game;

// Game g;

/**
 */
inline void logSDLError(const char* msg){
    printf("%s error: %s", msg, SDL_GetError());
}

static inline void checkSDLError(int line)
{
#ifndef NDEBUG
	const char *error = SDL_GetError();
	if (*error != '\0')
	{
		printf("SDL Error: %s\n", error);
        printf(" + line: %i\n", line);
		SDL_ClearError();
	}
#endif
}

/**
 * Game::handleEvents
 * 
 */
void handleEvents(Game* g) {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT:    
                g->running = 0;            
                break;
            case SDL_KEYDOWN: 
                g->keys[event.key.keysym.sym] = 1;
                break;
            case SDL_KEYUP:   
                g->keys[ event.key.keysym.sym ] = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                g->mouseDown = 1;
                break;
            case SDL_MOUSEBUTTONUP:
                g->mouseDown = 0;
                break;
            case SDL_MOUSEMOTION:
                g->mouseX = event.motion.x;
                g->mouseY = event.motion.y;
                break;
        }
    }    
}

/**
 * Game::draw
 * 
 */
void draw(Game* g)
{
    glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    SDL_GL_SwapWindow(g->window);
    if (g->keys[SDLK_ESCAPE] == 1) g->running = 0;
}

/**
 * create_game
 * 
 */
Game* create_game(char* title, int width, int height) {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_AUDIO)) {
        logSDLError("Init SDL");
        return 0;
    }

    /* Request opengl 3.3 context.
     * SDL doesn't have the ability to choose which profile at this time of writing,
     * but it should default to the core profile */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
#ifdef __EMSCRIPTEN__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif

    /* Turn on double buffering with a 24bit Z buffer.
     * You may need to change this to 16 or 32 for your system */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    Game* g = malloc(sizeof(Game));

    g->title = title;
    g->width = width;
    g->height = height;
    g->running = 1;
    g->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_GLContext maincontext = SDL_GL_CreateContext(g->window);
    checkSDLError(__LINE__);


    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        logSDLError("Init image");
    }
    TTF_Init();
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        logSDLError("Init mixer");
    }

    #ifndef __EMSCRIPTEN__
    // Load OpenGL EntryPoints for desktop
    glewExperimental = GL_TRUE;
    glewInit();
    glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.
    #endif
    
    // OpenGL configuration
    glViewport(0, 0, width, height);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return g;
}

int main(int argc, char** argv){

    Game* game = create_game("Hello World", 640, 480);

    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(draw, game, 60, 0);
    #else
    printf("before while\n");
    while (game->running) {
        handleEvents(game);
        draw(game);
    }
    printf("after while\n");
    SDL_DestroyWindow(game->window);
	IMG_Quit();
    SDL_Quit();
    printf("after terminate\n");
    free(game);
    #endif
    return 0;
}

