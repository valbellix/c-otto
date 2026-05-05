#include "SDLSystem.h"

#include <SDL.h>

SDLSystem::SDLSystem(const std::string& title, const int scale, const int height, const int width): System(title, scale, height, width) {
    SDL_Init(SDL_INIT_VIDEO);
    m_window = SDL_CreateWindow(title.c_str(), 0, 0, scale * width, scale * height, SDL_WINDOW_SHOWN);
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
}

SDLSystem::~SDLSystem() {
    SDL_DestroyTexture(m_texture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

bool SDLSystem::keyEvent(const uchar keyEvent) {
    // TODO
    return false;
}

void SDLSystem::update(const void *buffer)
{
    SDL_UpdateTexture(m_texture, nullptr, buffer, m_width);
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
}
