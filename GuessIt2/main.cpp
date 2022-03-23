#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL.h>

using namespace std;

const int MAX = 100;
const int BIGGER = 1;
const int SMALLER = -1;
const int EQUAL = 0;

const string CAPTION = "Guess It";
const int SCREEN_WIDTH = 861;
const int SCREEN_HEIGHT = 517;
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *game1Img, *game2Img, *computerMsg, *humanMsg;
SDL_Texture *bigMsg, *smallMsg, *sameMsg;
SDL_Texture *bigMark, *smallMark, *sameMark;
SDL_Texture *numMsg[MAX + 1];
int markedNumbers[MAX + 1];

void logSDLError(std::ostream &os, const std::string &msg, bool fatal = false);
void initSDL(SDL_Window *&window, SDL_Renderer *&renderer);
void quitSDL(SDL_Window *window, SDL_Renderer *renderer);
void waitUntilKeyPressed();
SDL_Texture *loadTexture(const std::string &file, SDL_Renderer *ren);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);

void load_SDL_and_Images();
void unload_start_game();
void unload_SDL_and_Images();
void showStartGame();
void showRunningGame();
void pause();
void printComputerMsg();
void printHumanComparision(SDL_Texture *tex);
void printHumanMsg();
void printComputerNumber(SDL_Texture *tex);
void markNumber(int i);

int chooseSecretNumber();
void displayStartGame();
void displayRunningGame();
int getComputerGuess(int low, int high);
void displayComputerGuess(int number);
void updateAnswer(int & answer, int markedNumbers[], int number, int secretNumber);
void displayAnswer(int answer, int number);

void logSDLError(std::ostream &os, const std::string &msg, bool fatal) {
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window *&window, SDL_Renderer *&renderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(CAPTION.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void waitUntilKeyPressed() {
    SDL_Event e;
    while (true) {
        if (SDL_WaitEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_QUIT))
        return;
        SDL_Delay(100);
    }
}

SDL_Texture *loadTexture(const std::string &file, SDL_Renderer *ren) {
    SDL_Texture *texture = nullptr;
    SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
    if (loadedImage != nullptr) {
        texture = SDL_CreateTextureFromSurface(ren, loadedImage);
        SDL_FreeSurface(loadedImage);
        if (texture == nullptr) {
            logSDLError(std::cout, "CreateTextureFromSurface");
        }
    } else {
        logSDLError(std::cout, "LoadBMP");
    }
    return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

void load_SDL_and_Images() {
    initSDL(window, renderer);

    game1Img = loadTexture("images/game1.bmp", renderer);
    game2Img = loadTexture("images/game2.bmp", renderer);
    computerMsg = loadTexture("images/computer.bmp", renderer);
    humanMsg = loadTexture("images/human.bmp", renderer);
    bigMsg = loadTexture("images/big.bmp", renderer);
    smallMsg = loadTexture("images/small.bmp", renderer);
    sameMsg = loadTexture("images/same.bmp", renderer);
    bigMark = loadTexture("images/bigmark.bmp", renderer);
    smallMark = loadTexture("images/smallmark.bmp", renderer);
    sameMark = loadTexture("images/samemark.bmp", renderer);
    bool is_load_num_failed = false;
    for (int i = 1; i <= MAX; i++) {
        markedNumbers[i] = MAX;
        numMsg[i] = loadTexture("images/" + to_string(i) + ".bmp", renderer);
        if (numMsg[i] == nullptr) {
            is_load_num_failed = true;
            break;
        }
    }

    if (game1Img == nullptr || game2Img == nullptr || computerMsg == nullptr ||
        humanMsg == nullptr || bigMsg == nullptr || smallMsg == nullptr ||
        sameMsg == nullptr || is_load_num_failed) {
        unload_SDL_and_Images();
        exit(1);
    }
}

void unload_start_game() {
    SDL_DestroyTexture(game1Img);
}

void unload_SDL_and_Images() {
    SDL_DestroyTexture(game1Img);
    SDL_DestroyTexture(game2Img);
    SDL_DestroyTexture(computerMsg);
    SDL_DestroyTexture(humanMsg);
    SDL_DestroyTexture(bigMsg);
    SDL_DestroyTexture(smallMsg);
    SDL_DestroyTexture(sameMsg);
    SDL_DestroyTexture(bigMark);
    SDL_DestroyTexture(smallMark);
    SDL_DestroyTexture(sameMark);
    for (int i = 1; i <= MAX; i++) {
        SDL_DestroyTexture(numMsg[i]);
    }
    quitSDL(window, renderer);
}

void showStartGame() {
    SDL_RenderClear(renderer);
    renderTexture(game1Img, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(renderer);
}

void showRunningGame() {
    SDL_RenderClear(renderer);
    renderTexture(game2Img, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    for (int i = 1; i <= MAX; i++) {
        markNumber(i);
    }
    SDL_RenderPresent(renderer);
}

void pause() {
    int delayTime = 1500;
    do {
        SDL_Delay(10);
        delayTime -= 10;

        SDL_Event e;
        while (SDL_PollEvent( &e)) {
            if ((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                unload_SDL_and_Images();
                exit(1);
            }
        }
    } while (delayTime > 0);
}

void printComputerMsg() {
    renderTexture(humanMsg, renderer, 557, 140);
    SDL_RenderPresent(renderer);
}

void printHumanComparision(SDL_Texture *tex) {
    renderTexture(tex, renderer, 293, 115);
    SDL_RenderPresent(renderer);
}

void printHumanMsg() {
    renderTexture(computerMsg, renderer, 295, 100);
    SDL_RenderPresent(renderer);
}

void printComputerNumber(SDL_Texture *tex) {
    renderTexture(tex, renderer, 575, 132);
    SDL_RenderPresent(renderer);
}

void markNumber(int i) {
    int tableX0 = 29, tableY0 = 285, tableX1 = 829, tableY1 = 493;
    int cellWidth = (tableX1 - tableX0) / 20, cellHeight = (tableY1 - tableY0) / 5;
    int row = (i - 1) / 20, col = (i - 1) % 20;
    int x = tableX0 + col * cellWidth + 2, y = tableY0 + row * cellHeight + 5;
    if (markedNumbers[i] == BIGGER) {
        renderTexture(bigMark, renderer, x, y);
    } else if (markedNumbers[i] == SMALLER) {
        renderTexture(smallMark, renderer, x, y);
    } else if (markedNumbers[i] == EQUAL) {
        renderTexture(sameMark, renderer, x, y);
    }
}

int computeNumber(int x, int y) {
    int tableX0 = 29, tableY0 = 285, tableX1 = 829, tableY1 = 493;
    if (x < tableX0 || x > tableX1 || y < tableY0 || y > tableY1) return -1;
    int col = (x - tableX0) * 20 / (tableX1 - tableX0), row = (y - tableY0) * 5 / (tableY1 - tableY0);
    return (row * 20 + col + 1);
}

int chooseSecretNumber() {
    int number = -1;
    SDL_Event e;
    while (number <= 0 || number > MAX) {
        SDL_Delay(10);
        if (SDL_WaitEvent( &e) == 0) continue;
        if ((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            unload_SDL_and_Images();
            exit(1);
        }

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            number = computeNumber(e.button.x, e.button.y);
        }
    }
    return number;
}

void displayStartGame() {
    showStartGame();
    pause();
}

void displayRunningGame() {
    showRunningGame();
    pause();
}

int getComputerGuess(int low, int high) {
    return (low + high) / 2;
}

void displayComputerGuess(int number) {
    showRunningGame();
    printComputerMsg();
    pause();
    showRunningGame();
    printComputerNumber(numMsg[number]);
    pause();
}

void updateAnswer(int &answer, int markedNumbers[], int number, int secretNumber) {
    if (number > secretNumber) {
        answer = BIGGER;
    } else if (number < secretNumber) {
        answer = SMALLER;
    } else {
        answer = EQUAL;
    }
    markedNumbers[number] = answer;
}

void displayAnswer(int answer, int number) {
    printHumanMsg();
    pause();
    markNumber(number);

    SDL_Texture *tex;
    if (answer == BIGGER) {
        tex = bigMsg;
    } else if (answer == SMALLER) {
        tex = smallMsg;
    } else {
        tex = sameMsg;
    }
    printHumanComparision(tex);
    pause();
}

int main(int argc, char *argv[]) {
    load_SDL_and_Images();

    srand(time(NULL));
    displayStartGame();
    int secretNumber = chooseSecretNumber();
    unload_start_game();
    displayRunningGame();

    int number, answer;
    int low = 1;
    int high = MAX;
    do {
        number = getComputerGuess(low, high);
        displayComputerGuess(number);
        updateAnswer(answer, markedNumbers, number, secretNumber);
        displayAnswer(answer, number);
        if (number > secretNumber) {
            high = number - 1;
        } else if (number < secretNumber) {
            low = number + 1;
        }
    } while (answer != EQUAL);

    waitUntilKeyPressed();
    unload_SDL_and_Images();
    return 0;
}
