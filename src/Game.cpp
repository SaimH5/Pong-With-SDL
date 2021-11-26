#include "Game.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int thickness = 15;
const int paddleH = 100;

Game::Game(){
    mWindow = nullptr;
    mRenderer = nullptr;
    mIsRunning = true;
    mPlayerDir = PLAYER_DIRECTION_NEUTRAL;
    mTicksCount = 0;
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

    mPlayerPaddlePos.x = 10.0f;
    mPlayerPaddlePos.y = SCREEN_HEIGHT / 2.0f;
    mBallPos.x = SCREEN_WIDTH / 2.0f;
    mBallPos.y = SCREEN_HEIGHT / 2.0f;
    mBallVelocity.x = 110.0f;
    mBallVelocity.y = -100.0f;
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
        UpdateGame();
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

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_ESCAPE]){
        mIsRunning = false;
    }
    if(state[SDL_SCANCODE_UP] && state[SDL_SCANCODE_DOWN]){
        mPlayerDir = PLAYER_DIRECTION_NEUTRAL;
    }
    else if(state[SDL_SCANCODE_UP]){
        mPlayerDir = PLAYER_DIRECTION_UP;
    }
    else if(state[SDL_SCANCODE_DOWN]){
        mPlayerDir = PLAYER_DIRECTION_DOWN;
    } 
    else{
        mPlayerDir = PLAYER_DIRECTION_NEUTRAL;
    }
} 

void Game::UpdateGame(){
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16)){;}

    float deltaTime = (SDL_GetTicks() - mTicksCount / 1000.0f);

    mTicksCount = SDL_GetTicks();
    if(deltaTime > 0.05f){
        deltaTime = 0.05f;
    }

    // Player Paddle Position Update
    mPlayerPaddlePos.y += mPlayerDir * 300.0f * deltaTime;

    // Boundary Collision Check and Clamping
    if(mPlayerPaddlePos.y - paddleH / 2.0f < thickness){
        mPlayerPaddlePos.y = paddleH / 2.0f + thickness;
    }

    if(mPlayerPaddlePos.y + paddleH/ 2.0f > SCREEN_HEIGHT-thickness){
        mPlayerPaddlePos.y = SCREEN_HEIGHT - paddleH / 2.0f - thickness;
    }

    // Ball Position Update
    mBallPos.x += mBallVelocity.x * deltaTime;
    mBallPos.y += mBallVelocity.y * deltaTime;

    // Collision Detection with Walls
    if(mBallPos.y - thickness / 2.0f < thickness && mBallVelocity.y < 0.0f){
        mBallVelocity.y *= -1;
    }
    if(mBallPos.y + thickness / 2.0f > SCREEN_HEIGHT - thickness && mBallVelocity.y > 0.0f){
        mBallVelocity.y *= -1;
    }
    if(mBallPos.x + thickness / 2.0f > SCREEN_WIDTH - thickness && mBallVelocity.x > 0.0f){
        mBallVelocity.x *= -1;
    }

    // Collision Detection with Paddle
    float diff = abs(mPlayerPaddlePos.y - mBallPos.y);
    if( 
        diff <= paddleH / 2.0f && 
        mBallVelocity.x < 0.0f &&
        mBallPos.x < thickness + 15.0f && mBallPos.x > thickness + 10.0f)
    {
        mBallVelocity.x *= -1;
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

    SDL_Rect Paddle{
        static_cast<int>(mPlayerPaddlePos.x - thickness/2.0f), 
        static_cast<int>(mPlayerPaddlePos.y - paddleH / 2.0f),
        thickness,
        paddleH
    };

    SDL_Rect Ball{
        static_cast<int>(mBallPos.x - thickness/2.0f),
        static_cast<int>(mBallPos.y - thickness/2.0f),
        thickness,
        thickness
    };

    SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
    SDL_RenderFillRect(mRenderer, &Paddle);
    SDL_RenderFillRect(mRenderer, &Ball);

    SDL_SetRenderDrawColor(mRenderer, 50, 50, 50, 255);
    SDL_RenderFillRect(mRenderer, &topWall);
    SDL_RenderFillRect(mRenderer, &bottomWall);
    SDL_RenderFillRect(mRenderer, &rightWall);
    SDL_RenderPresent(mRenderer);
}