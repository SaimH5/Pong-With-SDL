#include <SDL.h>


class Game{
    // Properties
    private:
        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;
        bool mIsRunning;

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

