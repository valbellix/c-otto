#include "SDLSystem.h"
#include "exceptions/VideoException.h"

#include <SDL.h>
#include <format>

#include "Cpu.h"

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

bool SDLSystem::readKeyEvent(Cpu& cpu) {
    bool quit = false;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_1:
                    cpu.setKeyPad(1, 1);
                    break;
                case SDLK_2:
                    cpu.setKeyPad(2, 1);
                    break;
                case SDLK_3:
                    cpu.setKeyPad(3, 1);
                    break;
                case SDLK_4:
                    cpu.setKeyPad(13, 1);
                    break;
                case SDLK_q:
                    cpu.setKeyPad(4, 1);
                    break;
                case SDLK_w:
                    cpu.setKeyPad(5, 1);
                    break;
                case SDLK_e:
                    cpu.setKeyPad(6, 1);
                    break;
                case SDLK_r:
                    cpu.setKeyPad(13, 1);
                    break;
                case SDLK_a:
                    cpu.setKeyPad(7, 1);
                    break;
                case SDLK_s:
                    cpu.setKeyPad(8, 1);
                    break;
                case SDLK_d:
                    cpu.setKeyPad(9, 1);
                    break;
                case SDLK_f:
                    cpu.setKeyPad(14, 1);
                    break;
                case SDLK_z:
                    cpu.setKeyPad(10, 1);
                    break;
                case SDLK_x:
                    cpu.setKeyPad(0, 1);
                    break;
                case SDLK_c:
                    cpu.setKeyPad(11, 1);
                    break;
                case SDLK_v:
                    cpu.setKeyPad(15, 1);
                    break;
                default:
                    // do nothing
            }
        } else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                case SDLK_1:
                    cpu.setKeyPad(1, 0);
                    break;
                case SDLK_2:
                    cpu.setKeyPad(2, 0);
                    break;
                case SDLK_3:
                    cpu.setKeyPad(3, 0);
                    break;
                case SDLK_4:
                    cpu.setKeyPad(13, 0);
                    break;
                case SDLK_q:
                    cpu.setKeyPad(4, 0);
                    break;
                case SDLK_w:
                    cpu.setKeyPad(5, 0);
                    break;
                case SDLK_e:
                    cpu.setKeyPad(6, 0);
                    break;
                case SDLK_r:
                    cpu.setKeyPad(13, 0);
                    break;
                case SDLK_a:
                    cpu.setKeyPad(7, 0);
                    break;
                case SDLK_s:
                    cpu.setKeyPad(8, 0);
                    break;
                case SDLK_d:
                    cpu.setKeyPad(9, 0);
                    break;
                case SDLK_f:
                    cpu.setKeyPad(14, 0);
                    break;
                case SDLK_z:
                    cpu.setKeyPad(10, 0);
                    break;
                case SDLK_x:
                    cpu.setKeyPad(0, 0);
                    break;
                case SDLK_c:
                    cpu.setKeyPad(11, 0);
                    break;
                case SDLK_v:
                    cpu.setKeyPad(15, 0);
                    break;
                default:
                    // do nothing
            }
        } else if (event.type == SDL_QUIT) {
            quit = true;
        }
    }
    return quit;
}

void SDLSystem::update(const void *buffer)
{
    SDL_UpdateTexture(m_texture, nullptr, buffer, 256);
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
}
