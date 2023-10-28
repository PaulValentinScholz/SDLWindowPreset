#include <stdlib.h>
#include <stdbool.h>
#include "./constants.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

bool exit_app = false;

SDL_Window* window;
SDL_Renderer* renderer;
Uint32 render_flags;
SDL_Surface* surface;
SDL_Texture* texture;

int last_frame_time = 0;
int time_to_wait = 0;

SDL_Rect rect;

bool initialize_window(){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "Error initializing SDL.\n");
        return true;
    }

    window = SDL_CreateWindow(
        "My Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );
    if(!window){
        fprintf(stderr, "Error initializing SDL Window.\n");
        return true;
    }

    render_flags = SDL_RENDERER_ACCELERATED;

    renderer = SDL_CreateRenderer(window, -1, render_flags);
    if(!renderer){
        fprintf(stderr, "Error initializing SDL Renderer.\n");
        return true;
    }

    surface = IMG_Load("texture.png");
    if(!surface){
        fprintf(stderr, "Error initializing SDL Surface.\n");
        return true;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(!texture){
        fprintf(stderr, "Error initializing SDL Texture.\n");
        return true;
    }
    SDL_FreeSurface(surface);

    return false;
}

// Process the users' input
void process_input(){
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type){
        case SDL_QUIT:
            exit_app = true;
            break;
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE){
                exit_app = true;
                break;
            }
            break;
        case SDL_KEYUP:
            break;
    }
}

// Before game loop
void setup(){

    rect.h *= 10;
    rect.w *= 10;
    rect.x = (WINDOW_WIDTH - rect.w) / 2;
    rect.y = (WINDOW_HEIGHT - rect.h) /2;
}

//Updating
void update(){
    // cap the framerate
    time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
    if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME){
        SDL_Delay(time_to_wait);
    }
    // delta time
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
    last_frame_time = SDL_GetTicks();

    // update
    rect.x += 100 * delta_time;
    rect.y += 100 * delta_time;

}

// Rendering
void render(){

    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);

    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, NULL, &rect);


    SDL_RenderPresent(renderer);
}

void destroy_window(){
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(){
    
    exit_app = initialize_window();

    setup();

    while(!exit_app){
        process_input();
        update();
        render();
    }


    destroy_window();

    return 0;
}
