#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int FONT_SIZE = 16;
const int TRAIL_LENGTH = 10; // Number of trail frames
const int TRAIL_OPACITY_DECREMENT = 25; // Opacity decrement per trail frame

struct Character {
    int x, y;
    char ch;
    int speed;
    SDL_Color color;
};

std::vector<Character> characters;
TTF_Font* font = nullptr;
SDL_Renderer* renderer = nullptr;

char getRandomChar() {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    return charset[rand() % (sizeof(charset) - 1)];
}

SDL_Color getRandomColor() {
    return SDL_Color{ 0, static_cast<Uint8>(rand() % 156 + 100), 0, 255 };
}

void initCharacters(int numCharacters) {
    characters.clear();
    for (int i = 0; i < numCharacters; ++i) {
        Character c;
        c.x = rand() % (SCREEN_WIDTH / FONT_SIZE) * FONT_SIZE;
        c.y = rand() % (SCREEN_HEIGHT / FONT_SIZE) * FONT_SIZE;
        c.ch = getRandomChar();
        c.speed = 1 + rand() % 5;
        c.color = getRandomColor();
        characters.push_back(c);
    }
}

void renderCharacter(Character& c) {
    for (int i = 0; i < TRAIL_LENGTH; ++i) {
        int opacity = 255 - i * TRAIL_OPACITY_DECREMENT;
        SDL_Color color = c.color;
        color.a = opacity;
        SDL_Surface* surface = TTF_RenderText_Solid(font, &c.ch, color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect destRect = { c.x, c.y - i * FONT_SIZE, FONT_SIZE, FONT_SIZE };
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(texture, opacity);
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    c.ch = getRandomChar();
    c.y += c.speed;
    if (c.y >= SCREEN_HEIGHT) {
        c.y = 0;
        c.x = rand() % (SCREEN_WIDTH / FONT_SIZE) * FONT_SIZE;
        c.color = getRandomColor();
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
