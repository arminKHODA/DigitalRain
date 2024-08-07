#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int FONT_SIZE = 16;
const int NUM_TRAIL = 5;

struct Character {
    int x, y;
    char ch;
    int speed;
    int trailLength;
    std::vector<int> trailY;
};

std::vector<Character> characters;
TTF_Font* font = nullptr;
SDL_Renderer* renderer = nullptr;

char getRandomChar() {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    return charset[rand() % (sizeof(charset) - 1)];
}

void initCharacters(int numCharacters) {
    characters.clear();
    for (int i = 0; i < numCharacters; ++i) {
        Character c;
        c.x = rand() % (SCREEN_WIDTH / FONT_SIZE) * FONT_SIZE;
        c.y = rand() % (SCREEN_HEIGHT / FONT_SIZE) * FONT_SIZE;
        c.ch = getRandomChar();
        c.speed = 1 + rand() % 5;
        c.trailLength = NUM_TRAIL;
        for (int j = 0; j < c.trailLength; ++j) {
            c.trailY.push_back(c.y - j * FONT_SIZE);
        }
        characters.push_back(c);
    }
}

void renderCharacter(Character& c) {
    c.ch = getRandomChar();
    SDL_Color color = { 0, 255, 0, 255 };
    SDL_Surface* surface = TTF_RenderText_Solid(font, &c.ch, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect destRect = { c.x, c.y, FONT_SIZE, FONT_SIZE };
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Render trail
    for (int i = 1; i < c.trailLength; ++i) {
        int alpha = 255 - (i * 255 / c.trailLength);
        SDL_Color trailColor = { 0, 255, 0, alpha };
        surface = TTF_RenderText_Solid(font, &c.ch, trailColor);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        destRect.y = c.trailY[i];
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    for (int i = c.trailLength - 1; i > 0; --i) {
        c.trailY[i] = c.trailY[i - 1];
    }
    c.trailY[0] = c.y;

    c.y += c.speed;
    if (c.y >= SCREEN_HEIGHT) {
        c.y = 0;
        c.x = rand() % (SCREEN_WIDTH / FONT_SIZE) * FONT_SIZE;
        c.trailY[0] = c.y;
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Could not initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() < 0) {
        std::cerr << "Could not initialize SDL_ttf: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Matrix Rain",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Could not create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Could not create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    font = TTF_OpenFont("font.ttf", FONT_SIZE);
    if (!font) {
        std::cerr << "Could not load font: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    srand(static_cast<unsigned int>(time(0)));
    initCharacters(100);

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (auto& c : characters) {
            renderCharacter(c);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(50);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
