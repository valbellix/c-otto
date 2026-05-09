#include "SDLSystem.h"
#include "exceptions/VideoException.h"

#include <SDL.h>
#include <format>

SDLSystem::SDLSystem(const std::string& title, const int width, const int height): System(title, width, height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw VideoException(std::format("SDL initialization error: {}", SDL_GetError()));
    }
    m_window = SDL_CreateWindow(title.c_str(), 0, 0, width, height, SDL_WINDOW_SHOWN);
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(m_renderer, width, height);
    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
}

SDLSystem::~SDLSystem() {
    SDL_DestroyTexture(m_texture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

bool SDLSystem::keyEvent(const uint8_t keyEvent) {
    // TODO
    return false;
}

void SDLSystem::update(const void *buffer)
{
    // TODO
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        }
    }
    SDL_UpdateTexture(m_texture, nullptr, buffer, 256);
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
}
