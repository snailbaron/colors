#include <SDL.h>

#include <chrono>
#include <cstdlib>
#include <stdexcept>
#include <thread>
#include <vector>

constexpr char windowTitle[] = "colors";
constexpr int fps = 30;

template <class T>
T* sdlCheck(T* ptr)
{
    if (!ptr) {
        throw std::runtime_error{SDL_GetError()};
    }
    return ptr;
}

int sdlCheck(int code)
{
    if (code != 0) {
        throw std::runtime_error{SDL_GetError()};
    }
    return code;
}

int main(int argc, char* argv[])
{
    SDL_Window* window = sdlCheck(SDL_CreateWindow(
        windowTitle,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        0,
        0,
        SDL_WINDOW_FULLSCREEN_DESKTOP));

    SDL_Renderer* renderer = sdlCheck(SDL_CreateRenderer(window, -1, 0));

    auto colors = std::vector<SDL_Color> {
        {0, 0, 0, 255},
        {255, 255, 255, 255},
        {255, 0, 0, 255},
        {0, 255, 0, 255},
        {0, 0, 255, 255},
        {255, 255, 0, 255},
        {255, 0, 255, 255},
        {0, 255, 255, 255},
    };
    size_t index = 0;

    using Clock = std::chrono::high_resolution_clock;

    const auto frameDuration = std::chrono::duration_cast<Clock::duration>(
        std::chrono::duration<double>(1.0 / fps));
    const auto startTime = Clock::now();

    size_t lastFrameIndex = 0;
    bool done = false;
    while (!done) {
        for (SDL_Event e; SDL_PollEvent(&e); ) {
            if (e.type == SDL_QUIT) {
                done = true;
                break;
            } else if (e.type == SDL_KEYDOWN && !e.key.repeat) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    index = (index + 1) % colors.size();
                } else if (e.key.keysym.sym == SDLK_ESCAPE) {
                    auto quitEvent = SDL_Event{
                        .quit = SDL_QuitEvent{
                            .type = SDL_QUIT,
                            .timestamp = SDL_GetTicks(),
                        }
                    };
                    SDL_PushEvent(&quitEvent);
                }
            } 
        }

        auto currentTime = Clock::now();
        size_t currentFrame = (currentTime - startTime) / frameDuration;
        if (currentFrame > lastFrameIndex) {
            lastFrameIndex = currentFrame;

            const auto& color = colors.at(index);
            sdlCheck(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a));
            sdlCheck(SDL_RenderClear(renderer));
            SDL_RenderPresent(renderer);

            std::this_thread::sleep_until(currentTime + frameDuration);
        }
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return EXIT_SUCCESS;
}