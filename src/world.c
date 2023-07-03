#ifndef LIBSDEFS_INCLUDED
#define LIBDEFS_INCLUDED
    #include "libsdefs.c"
#endif

#include "player.c"
#include "tile.c"

enum WORLD_STATUS {WORLD_STATUS_RUNNING, WORLD_STATUS_QUIT};
struct world {
    SDL_Window* window;
    SDL_Renderer* renderer;
    enum WORLD_STATUS status;
    struct player player;
    struct tilemap* tilemap;
};

struct world* world_init() {
    struct world* world = malloc(sizeof(struct world));
    if (world == NULL) {
        printf("world_init(): Failed to allocate memory for world. Exit.\n");
        return NULL;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("world_init(): Failed to initialise SDL.\n"
        "SDL_GetError(): %s.\n"
        "Exiting.\n", SDL_GetError());
        free(world);
        return NULL;
    }

    world->window = SDL_CreateWindow("Window",\
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,\
    SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (world->window == NULL) {
        printf("world_init(): Failed to initialise SDL Window.\n"
        "SDL_GetError(): %s.\n"
        "Exiting.\n", SDL_GetError());
        SDL_Quit();
        free(world);
        return NULL;
    }

    world->renderer = SDL_CreateRenderer(world->window, -1, SDL_RENDERER_ACCELERATED);
    if (world->renderer == NULL) {
        printf("world_init(): Failed to initialise SDL Renderer.\n"
        "SDL_GetError(): %s.\n"
        "Exiting.\n", SDL_GetError());
        if (world->window != NULL) SDL_DestroyWindow(world->window);
        SDL_Quit();
        free(world);
        return NULL;
    }

    world->tilemap = tilemap_init("src/maps/map1");
    if (world->tilemap == NULL) {
         printf("world_init(): Failed to initialise tilemap.\n"
        "Exiting.\n");
        if (world->renderer != NULL) SDL_DestroyRenderer(world->renderer);
        if (world->window != NULL) SDL_DestroyWindow(world->window);
        SDL_Quit();
        free(world);
        return NULL;
    }

    world->player.x = 0;
    world->player.y = 0;

    world->status = WORLD_STATUS_RUNNING;
    return world;
}

void world_quit(struct world* world) {
    if (world == NULL) return;
    if (world->renderer != NULL) SDL_DestroyRenderer(world->renderer);
    if (world->window != NULL) SDL_DestroyWindow(world->window);
    if (world->tilemap != NULL) tilemap_free(world->tilemap);
    SDL_Quit();
    free(world);
}

void world_input(struct world* world) {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            world->status = WORLD_STATUS_QUIT;
        }
    }
}

void world_loop(void* world_loop_argument) {
    struct world* world = (struct world*)world_loop_argument;
    if (world == NULL) return;

    world_input(world);
    SDL_SetRenderDrawColor(world->renderer, 0, 0, 0, 0);
    SDL_RenderClear(world->renderer);
    SDL_SetRenderDrawColor(world->renderer, 0, 128, 128, 0);
    SDL_RenderDrawLine(world->renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(world->renderer);
}   