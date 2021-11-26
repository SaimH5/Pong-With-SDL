#include <SDL.h>

struct Vector2{
    float x;
    float y;
};

enum PlayerDirection{
    PLAYER_DIRECTION_UP = -1,
    PLAYER_DIRECTION_DOWN = 1,
    PLAYER_DIRECTION_NEUTRAL = 0
};

class Game{
    // Properties
    private:
        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;
        bool mIsRunning;
        float mTicksCount;
        PlayerDirection mPlayerDir;
        Vector2 mPlayerPaddlePos;
        Vector2 mBallPos;
        Vector2 mBallVelocity;

    // Methods
    public:
        Game();
        bool Initialize();
        void RunLoop();
        void Shutdown();

    private:
        void ProcessInput();
        void UpdateGame();
        void GenerateOutput();
};

