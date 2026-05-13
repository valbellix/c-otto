#ifndef _SDLSystem_h
#define _SDLSystem_h

#include "System.h"

class SDL_Window;
class SDL_Renderer;
class SDL_Texture;

class SDLSystem: public System {
public:
    SDLSystem(const std::string& title, int height, int width);
    virtual ~SDLSystem();

    bool readKeyEvent(Cpu& cpu);
    void update(const void* buffer);
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Texture* m_texture;
};

#endif
