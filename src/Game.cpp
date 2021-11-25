#include "Game.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int thickness = 15;
const int paddleH = 100;

Game::Game(){
    mWindow = nullptr;
    mRenderer = nullptr;
    mIsRunning = true;
}

bool Game::Initialize(){
    bool success = SDL_Init(SDL_INIT_VIDEO);
    if(success != 0){
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow("Pong", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(mWindow == nullptr){
        SDL_Log("Could not create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(mRenderer == nullptr){
        SDL_Log("Could not create Renderer: %s", SDL_GetError());
        return false;
    }

    return true;
}

void Game::Shutdown(){
    SDL_DestroyWindow(mWindow);
    mWindow = nullptr;
    SDL_Quit();
}

void Game::RunLoop(){
    
    while(mIsRunning){
        ProcessInput();
        // UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput(){
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        switch(e.type){
            case SDL_QUIT:
                mIsRunning = false;
                break;
        }
    }
} 

void Game:: GenerateOutput(){
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 0);
    SDL_RenderClear(mRenderer);

    SDL_Rect topWall{
        0, 0, SCREEN_WIDTH, thickness
    };
    SDL_Rect bottomWall{
        0, SCREEN_HEIGHT - thickness, SCREEN_WIDTH, thickness
    };
    SDL_Rect rightWall{
        SCREEN_WIDTH - thickness, 0, thickness, SCREEN_HEIGHT
    };


    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    SDL_RenderFillRect(mRenderer, &topWall);
    SDL_RenderFillRect(mRenderer, &bottomWall);
    SDL_RenderFillRect(mRenderer, &rightWall);
    SDL_RenderPresent(mRenderer);
}