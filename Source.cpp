#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <time.h>

#include <string>

enum { UP, DOWN, LEFT, RIGHT, DEFAULT };
enum {
    HEAD,
    BODY_YELLOW,
    BODY_BLACK,
    CORNER_YELLOW,
    CORNER_BLACK,
    TAIL_BLACK,
    TAIL_YELLOW
};
enum { START, MAIN_CHANGE_SKIN, MAIN_QUIT, N_MAIN_OPTIONS };
enum { CONTINUE, PAUSE_CHANGE_SKIN, PAUSE_QUIT, N_PAUSE_OPTIONS };
enum { SKIN_NUMBER, BACK, N_SKIN_OPTIONS };

typedef struct {
    int x, y;
} coord;

typedef struct ListNode* ListPointer;
typedef struct ListNode {
    coord Data;
    ListPointer Next;
    ListPointer Prev;
} ListNode;

typedef struct QueueNode* QueuePointer;
typedef struct QueueNode {
    SDL_Event Data;
    QueuePointer Next;
} QueueNode;
typedef struct {
    QueuePointer Front = NULL;
    QueuePointer Rear = NULL;
} QueueType;

bool init();
bool loadMedia();
void close();
SDL_Texture* loadTexture(std::string path);
void LinkedInsert(ListPointer* List, coord Item, ListPointer PredPtr,
                  ListPointer* LastPtr);
void AdvanceList(ListPointer List, coord newTile);
bool Search(ListPointer List, coord item);
void SelectTile(ListPointer CurrPtr, ListPointer worm_head,
                ListPointer worm_tail, SDL_Rect* tile, double* rotation, int i,
                char looking);
void DeleteTail(ListPointer* worm_tail);
void ResetGame(ListPointer* worm_tail, char* looking, ListPointer* worm_head,
               coord* fruit);
bool loadTextToTexture(std::string text, SDL_Color color, int* w, int* h);
int nOfDigits(long number);
long GetHighScore();
void SetHighScore(long highscore);
void Move(char* looking, ListPointer* worm_tail, ListPointer* worm_head,
          coord* fruit);
void Update(char looking, ListPointer worm_tail, ListPointer worm_head,
            coord fruit);
void AddQ(QueueType* Queue, SDL_Event Item);
int RemoveQ(QueueType* Queue, SDL_Event* Item);
void InitGame(char* looking, ListPointer* worm_tail, ListPointer* worm_head,
              coord* fruit);
void MainMenu();
void PauseMenu();
void SkinMenu();
void RenderMenu(SDL_Rect menu_rect, int nOptions, std::string* options,
                int selection);
void RenderSkinPreview(SDL_Rect preview_rect, int skin_previewed);
bool SkinExists(int n);
std::string ResourcesSlash(std::string file);

std::string gResourcesPath = "C:\\Users\\User\\Desktop\\Worm\\Resources\\";
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int GAME_HEIGHT = SCREEN_HEIGHT - 40;
const SDL_Rect gGameRect = {0, 0, SCREEN_WIDTH, GAME_HEIGHT};
const SDL_Rect gInfoRect = {0, GAME_HEIGHT, SCREEN_WIDTH,
                            SCREEN_HEIGHT - GAME_HEIGHT};
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;
SDL_Texture* gTextureApple = NULL;
SDL_Texture* gTextureWorm = NULL;
TTF_Font* gFont = NULL;
SDL_Texture* gTextTexture = NULL;
long gScore = 0, gHighScore;
std::string gMain_Options[N_MAIN_OPTIONS] = {"START", "CHANGE  SKIN", "QUIT"};
std::string gPause_Options[N_PAUSE_OPTIONS] = {"CONTINUE", "CHANGE  SKIN",
                                               "QUIT"};
std::string gSkin_Options[N_SKIN_OPTIONS] = {"000", "BACK"};

int main(int argc, char* agrs[]) {
    if (!init()) {
        printf("Falied to initialize!\n");
    } else {
        if (!loadMedia()) {
            printf("Falied to load media!\n");
        } else {
            bool quit = false;
            SDL_Event e;
            char looking;
            Uint32 start_time = SDL_GetTicks(), curr_time;
            ListPointer worm_tail = NULL, worm_head = NULL;
            coord fruit;
            srand((int)time(NULL));
            QueueType KeyPressQ;
            InitGame(&looking, &worm_tail, &worm_head, &fruit);
            MainMenu();
            while (!quit) {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    } else if (e.type == SDL_KEYDOWN) {
                        switch (e.key.keysym.sym) {
                            case SDLK_UP:
                            case SDLK_DOWN:
                            case SDLK_RIGHT:
                            case SDLK_LEFT:
                                AddQ(&KeyPressQ, e);
                                break;
                            case SDLK_SPACE:
                                LinkedInsert(&worm_tail, worm_tail->Data, NULL,
                                             &worm_head);
                                break;
                            case SDLK_ESCAPE:
                                PauseMenu();
                                break;
                        }
                    }
                }
                if (quit) break;
                curr_time = SDL_GetTicks();
                if (curr_time - start_time > 130) {
                    start_time = curr_time;
                    SDL_Event key_press;
                    bool moved = false;
                    while (RemoveQ(&KeyPressQ, &key_press) != 0) {
                        switch (key_press.key.keysym.sym) {
                            case SDLK_UP:
                                if (looking != UP && looking != DOWN) {
                                    looking = UP;
                                    Move(&looking, &worm_tail, &worm_head,
                                         &fruit);
                                    moved = true;
                                }
                                break;
                            case SDLK_DOWN:
                                if (looking != DOWN && looking != UP &&
                                    looking != DEFAULT) {
                                    looking = DOWN;
                                    Move(&looking, &worm_tail, &worm_head,
                                         &fruit);
                                    moved = true;
                                }
                                break;
                            case SDLK_LEFT:
                                if (looking != LEFT && looking != RIGHT) {
                                    looking = LEFT;
                                    Move(&looking, &worm_tail, &worm_head,
                                         &fruit);
                                    moved = true;
                                }
                                break;
                            case SDLK_RIGHT:
                                if (looking != RIGHT && looking != LEFT) {
                                    looking = RIGHT;
                                    Move(&looking, &worm_tail, &worm_head,
                                         &fruit);
                                    moved = true;
                                }
                                break;
                        }
                    }
                    if (looking != DEFAULT && !moved) {
                        Move(&looking, &worm_tail, &worm_head, &fruit);
                    }
                    Update(looking, worm_tail, worm_head, fruit);
                }
            }
        }
    }
    close();
    return 0;
}

long GetHighScore() {
    FILE* highscore_file = fopen("highscore.worm", "rb");
    long highscore = 0;
    if (highscore_file != NULL) {
        fread(&highscore, sizeof(long), 1, highscore_file);
        fclose(highscore_file);
    }
    return highscore;
}

void SetHighScore(long highscore) {
    FILE* highscore_file = fopen("highscore.worm", "wb");
    if (highscore_file != NULL) {
        fwrite(&highscore, sizeof(long), 1, highscore_file);
        fclose(highscore_file);
    }
}

int nOfDigits(long number) {
    int digit;
    if (number < 10)
        digit = 1;
    else if (number < 100)
        digit = 2;
    else if (number < 1000)
        digit = 3;
    else if (number < 10000)
        digit = 4;
    else if (number < 100000)
        digit = 5;
    // useless for int
    else if (number < 1000000)
        digit = 6;
    else if (number < 10000000)
        digit = 7;
    else if (number < 100000000)
        digit = 8;
    else if (number < 1000000000)
        digit = 9;
    else if (number < 10000000000)
        digit = 10;
    else
        digit = -1;
    return digit;
}

void ResetGame(ListPointer* worm_tail, char* looking, ListPointer* worm_head,
               coord* fruit) {
    while (*worm_tail != NULL) {
        DeleteTail(worm_tail);
    }
    *worm_head = NULL;
    for (int i = 0; i < 3; i++) {
        LinkedInsert(worm_tail,
                     {SCREEN_WIDTH / 2, GAME_HEIGHT / 2 + (i - 1) * 20}, NULL,
                     worm_head);
    }
    *looking = DEFAULT;
    gHighScore = GetHighScore();
    if (gScore > gHighScore) {
        SetHighScore(gScore);
        gHighScore = gScore;
    }
    gScore = 0;
    do {
        fruit->x = rand() % (SCREEN_WIDTH / 20) * 20;
        fruit->y = rand() % (GAME_HEIGHT / 20) * 20;
    } while (Search(*worm_tail, *fruit));
}

void DeleteTail(ListPointer* worm_tail) {
    if (*worm_tail != NULL) {
        ListPointer TmpPtr = *worm_tail;
        *worm_tail = TmpPtr->Next;
        free(TmpPtr);
        if (*worm_tail != NULL) (*worm_tail)->Prev = NULL;
    }
}

void SelectTile(ListPointer CurrPtr, ListPointer worm_head,
                ListPointer worm_tail, SDL_Rect* tile, double* rotation, int i,
                char looking) {
    if (CurrPtr == worm_head) {
        tile->x = HEAD;

        switch (looking) {
            case RIGHT:
                *rotation = 90;
                break;
            case DOWN:
                *rotation = 180;
                break;
            case LEFT:
                *rotation = -90;
                break;
            default:
                *rotation = 0;
                break;
        }
    } else if (CurrPtr == worm_tail) {
        if (i % 2 == 0) {
            tile->x = TAIL_BLACK;
        } else {
            tile->x = TAIL_YELLOW;
        }

        coord next = CurrPtr->Next->Data;
        coord curr = CurrPtr->Data;
        if (next.x == curr.x) {  // up or down
            if (next.y == curr.y - 20)
                *rotation = 0;
            else if (next.y == curr.y + 20)
                *rotation = 180;
        } else if (next.y == curr.y) {  // left or right
            if (next.x == curr.x - 20)
                *rotation = -90;
            else if (next.x == curr.x + 20)
                *rotation = 90;
        }
    } else {
        coord pred = CurrPtr->Prev->Data;
        coord curr = CurrPtr->Data;
        coord next = CurrPtr->Next->Data;

        if (pred.x == next.x) {  // vertical line
            *rotation = 0;
            if (i % 2 == 0) {
                tile->x = BODY_BLACK;
            } else {
                tile->x = BODY_YELLOW;
            }
        } else if (pred.y == next.y) {  // horizontal line
            *rotation = 90;
            if (i % 2 == 0) {
                tile->x = BODY_BLACK;
            } else {
                tile->x = BODY_YELLOW;
            }
        } else {                                                       // corner
            if (pred.y == curr.y - 20 || next.y == curr.y - 20) {      // J or L
                if (pred.x == curr.x - 20 || next.x == curr.x - 20) {  // J
                    *rotation = 0;
                    if (i % 2 == 0) {
                        tile->x = CORNER_BLACK;
                    } else {
                        tile->x = CORNER_YELLOW;
                    }
                } else {  // L
                    *rotation = 90;
                    if (i % 2 == 0) {
                        tile->x = CORNER_BLACK;
                    } else {
                        tile->x = CORNER_YELLOW;
                    }
                }
            } else {  // gamma or mirror gamma
                if (pred.x == curr.x - 20 ||
                    next.x == curr.x - 20) {  // mirror gamma
                    *rotation = -90;
                    if (i % 2 == 0) {
                        tile->x = CORNER_BLACK;
                    } else {
                        tile->x = CORNER_YELLOW;
                    }
                } else {  // gamma
                    *rotation = 180;
                    if (i % 2 == 0) {
                        tile->x = CORNER_BLACK;
                    } else {
                        tile->x = CORNER_YELLOW;
                    }
                }
            }
        }
    }
}

void AdvanceList(ListPointer List, coord newTile) {
    ListPointer CurrPtr = List;
    if (CurrPtr != NULL) {
        while (CurrPtr->Next != NULL) {
            CurrPtr->Data = CurrPtr->Next->Data;
            CurrPtr = CurrPtr->Next;
        }
        CurrPtr->Data = newTile;
    }
}

bool Search(ListPointer List, coord item) {
    ListPointer CurrPtr = List;
    while (CurrPtr != NULL) {
        if (CurrPtr->Data.x == item.x && CurrPtr->Data.y == item.y) return true;
        CurrPtr = CurrPtr->Next;
    }
    return false;
}

void LinkedInsert(ListPointer* List, coord Item, ListPointer PredPtr,
                  ListPointer* LastPtr) {
    ListPointer TempPtr;
    TempPtr = (ListPointer)malloc(sizeof(struct ListNode));
    if (TempPtr != NULL) {
        TempPtr->Data = Item;
        if (PredPtr == NULL) {
            TempPtr->Next = *List;
            if (*List != NULL) (*List)->Prev = TempPtr;
            *List = TempPtr;
            TempPtr->Prev = NULL;
        } else {
            TempPtr->Next = PredPtr->Next;
            if (TempPtr->Next != NULL) TempPtr->Next->Prev = TempPtr;
            PredPtr->Next = TempPtr;
            TempPtr->Prev = PredPtr;
        }
        if (TempPtr->Next == NULL) *LastPtr = TempPtr;
    }
}

bool init() {
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialiaze! SDL_Error: %s\n", SDL_GetError());
        success = false;
    } else {
        gWindow = SDL_CreateWindow("Worm", SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                   SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL_Error: %s\n",
                   SDL_GetError());
            success = false;
        } else {
            gRenderer =
                SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL_Error: %s\n",
                       SDL_GetError());
                success = false;
            } else {
                SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);

                if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
                    printf("SDL_Image could not initialize! SDL_Error: %s\n",
                           IMG_GetError());
                    success = false;
                }

                if (TTF_Init() == -1) {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n",
                           TTF_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}

bool loadMedia() {
    bool success = true;
    gTexture = loadTexture(ResourcesSlash("ground.png"));
    if (gTexture == NULL) {
        printf("Failed to load texture image! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    gTextureApple = loadTexture(ResourcesSlash("apple.png"));
    if (gTextureApple == NULL) {
        printf("Failed to load texture image! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    gTextureWorm = loadTexture(ResourcesSlash("worm.png"));
    if (gTextureWorm == NULL) {
        printf("Failed to load texture image! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    gFont = TTF_OpenFont(ResourcesSlash("ARCADECLASSIC.TTF").c_str(), 20);
    if (gFont == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
    return success;
}

void close() {
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;
    SDL_DestroyTexture(gTextureApple);
    gTextureApple = NULL;
    SDL_DestroyTexture(gTextureWorm);
    gTextureWorm = NULL;
    TTF_CloseFont(gFont);
    gFont = NULL;
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* loadTexture(std::string path) {
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
        printf("Unable to load image %s! SDL_Error: %s\n", path.c_str(),
               IMG_GetError());
    else {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
            printf("Unable to create texture from %s! SDL_Error: %s\n",
                   path.c_str(), SDL_GetError());
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

bool loadTextToTexture(std::string text, SDL_Color color, int* w, int* h) {
    if (gTextTexture != NULL) {
        SDL_DestroyTexture(gTextTexture);
        gTextTexture = NULL;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, text.c_str(), color);
    if (textSurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n",
               TTF_GetError());
    } else {
        gTextTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if (gTextTexture == NULL) {
            printf(
                "Unable to create texture from rendered text! SDL Error: %s\n",
                SDL_GetError());
        } else {
            *w = textSurface->w;
            *h = textSurface->h;
        }
        SDL_FreeSurface(textSurface);
    }
    return gTextTexture != NULL;
}

void Move(char* looking, ListPointer* worm_tail, ListPointer* worm_head,
          coord* fruit) {
    coord new_head = (*worm_head)->Data;
    bool moved = false;
    switch (*looking) {
        case UP:
            if (new_head.y >= 20) {
                new_head.y -= 20;
                moved = true;
            } else {
                ResetGame(worm_tail, looking, worm_head, fruit);
            }
            break;
        case DOWN:
            if (new_head.y < GAME_HEIGHT - 20) {
                new_head.y += 20;
                moved = true;
            } else {
                ResetGame(worm_tail, looking, worm_head, fruit);
            }
            break;
        case LEFT:
            if (new_head.x >= 20) {
                new_head.x -= 20;
                moved = true;
            } else {
                ResetGame(worm_tail, looking, worm_head, fruit);
            }
            break;
        case RIGHT:
            if (new_head.x < SCREEN_WIDTH - 20) {
                new_head.x += 20;
                moved = true;
            } else {
                ResetGame(worm_tail, looking, worm_head, fruit);
            }
            break;
    }
    if (moved) {
        if (Search(*worm_tail, new_head)) {
            ResetGame(worm_tail, looking, worm_head, fruit);
        } else {
            AdvanceList(*worm_tail, new_head);
            if ((*worm_head)->Data.x == fruit->x &&
                (*worm_head)->Data.y == fruit->y) {
                LinkedInsert(worm_tail, (*worm_tail)->Data, NULL, worm_head);
                gScore += 5;
                do {
                    fruit->x = rand() % (SCREEN_WIDTH / 20) * 20;
                    fruit->y = rand() % (GAME_HEIGHT / 20) * 20;
                } while (Search(*worm_tail, *fruit));
            }
        }
    }
}

void Update(char looking, ListPointer worm_tail, ListPointer worm_head,
            coord fruit) {
    SDL_RenderSetViewport(gRenderer, NULL);
    SDL_RenderCopy(gRenderer, gTexture, NULL, &gGameRect);
    ListPointer CurrPtr = worm_head;
    int i = 0;
    while (CurrPtr != NULL) {
        SDL_Rect position = {CurrPtr->Data.x, CurrPtr->Data.y, 20, 20};
        SDL_Rect tile;
        double rotation;
        tile.y = 0;
        tile.w = 20;
        tile.h = 20;
        SelectTile(CurrPtr, worm_head, worm_tail, &tile, &rotation, i, looking);
        tile.x *= 20;
        SDL_RenderCopyEx(gRenderer, gTextureWorm, &tile, &position, rotation,
                         NULL, SDL_FLIP_NONE);
        ;
        CurrPtr = CurrPtr->Prev;
        i++;
    }
    SDL_Rect fruit_rect = {fruit.x, fruit.y, 20, 20};
    SDL_RenderCopy(gRenderer, gTextureApple, NULL, &fruit_rect);

    SDL_Rect info_viewport = gInfoRect;
    info_viewport.x = SCREEN_WIDTH / 2;
    info_viewport.w /= 2;
    SDL_RenderSetViewport(gRenderer, &info_viewport);
    SDL_SetRenderDrawColor(gRenderer, 0x10, 0x10, 0x10, 0xff);
    SDL_RenderFillRect(gRenderer, NULL);

    int text_w, text_h;
    std::string text = "SCORE  ";
    for (int j = 0; j < 8 - nOfDigits(gScore); j++) text.append("0");
    text.append(std::to_string(gScore));
    loadTextToTexture(text, {0xff, 0xff, 0xff}, &text_w, &text_h);
    SDL_Rect TextRect = {info_viewport.w - text_w, 0, text_w, text_h};
    SDL_RenderCopy(gRenderer, gTextTexture, NULL, &TextRect);

    text.assign("HIGH  ");
    if (gScore > gHighScore) {
        for (int j = 0; j < 8 - nOfDigits(gScore); j++) text.append("0");
        text.append(std::to_string(gScore));
    } else {
        for (int j = 0; j < 8 - nOfDigits(gHighScore); j++) text.append("0");
        text.append(std::to_string(gHighScore));
    }
    loadTextToTexture(text, {0xff, 0xff, 0xff}, &text_w, &text_h);
    TextRect = {info_viewport.w - text_w, text_h, text_w, text_h};
    SDL_RenderCopy(gRenderer, gTextTexture, NULL, &TextRect);

    SDL_RenderPresent(gRenderer);
    SDL_RenderSetViewport(gRenderer, NULL);
}

void AddQ(QueueType* Queue, SDL_Event Item) {
    QueuePointer TempPtr;
    TempPtr = (QueuePointer)malloc(sizeof(struct QueueNode));
    if (TempPtr != NULL) {
        TempPtr->Data = Item;
        TempPtr->Next = NULL;
        if (Queue->Front == NULL)
            Queue->Front = TempPtr;
        else
            Queue->Rear->Next = TempPtr;
        Queue->Rear = TempPtr;
    }
}

int RemoveQ(QueueType* Queue, SDL_Event* Item) {
    QueuePointer TempPtr;
    if (Queue->Front == NULL) {  // empty queue
        return 0;
    } else {
        TempPtr = Queue->Front;
        *Item = TempPtr->Data;
        Queue->Front = Queue->Front->Next;
        free(TempPtr);
        if (Queue->Front == NULL) Queue->Rear = NULL;
        return 1;
    }
}

void InitGame(char* looking, ListPointer* worm_tail, ListPointer* worm_head,
              coord* fruit) {
    ResetGame(worm_tail, looking, worm_head, fruit);
    SDL_SetRenderDrawColor(gRenderer, 0x10, 0x10, 0x10, 0xff);
    SDL_RenderFillRect(gRenderer, &gInfoRect);
    int text_w, text_h;
    std::string text = "PRESS  ESC  TO  PAUSE";
    loadTextToTexture(text, {0xff, 0xff, 0xff}, &text_w, &text_h);
    SDL_Rect text_rect = gInfoRect;
    text_rect.w = text_w;
    text_rect.h = text_h;
    SDL_RenderCopy(gRenderer, gTextTexture, NULL, &text_rect);
    Update(*looking, *worm_tail, *worm_head, *fruit);
}

void MainMenu() {
    SDL_Rect menu_rect;
    menu_rect.x = gGameRect.x;  // 0
    menu_rect.h = N_MAIN_OPTIONS * TTF_FontHeight(gFont) + 1;
    menu_rect.y = (gGameRect.h - menu_rect.h) / 2;
    menu_rect.w = gGameRect.w;
    int selection = 0;  // START
    RenderMenu(menu_rect, N_MAIN_OPTIONS, gMain_Options, selection);
    bool stop = false;
    while (!stop) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                stop = true;
                close();
                exit(0);
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        if (selection > 0) selection--;
                        RenderMenu(menu_rect, N_MAIN_OPTIONS, gMain_Options,
                                   selection);
                        break;
                    case SDLK_DOWN:
                        if (selection < N_MAIN_OPTIONS - 1) selection++;
                        RenderMenu(menu_rect, N_MAIN_OPTIONS, gMain_Options,
                                   selection);
                        break;
                    case SDLK_SPACE:
                    case SDLK_RETURN:  // enter
                        switch (selection) {
                            case START:
                                stop = true;
                                break;
                            case MAIN_CHANGE_SKIN: {
                                SDL_Surface *win_surface =
                                                SDL_GetWindowSurface(gWindow),
                                            *temp_surface =
                                                SDL_CreateRGBSurface(
                                                    0, SCREEN_WIDTH,
                                                    SCREEN_HEIGHT, 32, 0, 0, 0,
                                                    0);
                                SDL_RenderReadPixels(gRenderer, NULL, 0,
                                                     temp_surface->pixels,
                                                     temp_surface->pitch);
                                SDL_Texture* temp_texture =
                                    SDL_CreateTextureFromSurface(gRenderer,
                                                                 temp_surface);
                                SkinMenu();
                                SDL_RenderCopy(gRenderer, temp_texture, NULL,
                                               NULL);
                                SDL_RenderPresent(gRenderer);
                                SDL_DestroyTexture(temp_texture);
                                SDL_FreeSurface(temp_surface);
                            } break;
                            case MAIN_QUIT:
                                stop = true;
                                close();
                                exit(0);
                        }
                        break;
                }
            }
        }
    }
}

void PauseMenu() {
    SDL_Rect menu_rect;
    menu_rect.x = gGameRect.x;  // 0
    menu_rect.h = N_PAUSE_OPTIONS * TTF_FontHeight(gFont) + 1;
    menu_rect.y = (gGameRect.h - menu_rect.h) / 2;
    menu_rect.w = gGameRect.w;
    int selection = 0;  // START
    RenderMenu(menu_rect, N_PAUSE_OPTIONS, gPause_Options, selection);
    bool stop = false;
    while (!stop) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                stop = true;
                close();
                exit(0);
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        if (selection > 0) selection--;
                        RenderMenu(menu_rect, N_PAUSE_OPTIONS, gPause_Options,
                                   selection);
                        break;
                    case SDLK_DOWN:
                        if (selection < N_MAIN_OPTIONS - 1) selection++;
                        RenderMenu(menu_rect, N_PAUSE_OPTIONS, gPause_Options,
                                   selection);
                        break;
                    case SDLK_SPACE:
                    case SDLK_RETURN:  // enter
                        switch (selection) {
                            case CONTINUE:
                                stop = true;
                                break;
                            case PAUSE_CHANGE_SKIN: {
                                SDL_Surface *win_surface =
                                                SDL_GetWindowSurface(gWindow),
                                            *temp_surface =
                                                SDL_CreateRGBSurface(
                                                    0, SCREEN_WIDTH,
                                                    SCREEN_HEIGHT, 32, 0, 0, 0,
                                                    0);
                                SDL_RenderReadPixels(gRenderer, NULL, 0,
                                                     temp_surface->pixels,
                                                     temp_surface->pitch);
                                SDL_Texture* temp_texture =
                                    SDL_CreateTextureFromSurface(gRenderer,
                                                                 temp_surface);
                                SkinMenu();
                                SDL_RenderCopy(gRenderer, temp_texture, NULL,
                                               NULL);
                                SDL_RenderPresent(gRenderer);
                                SDL_DestroyTexture(temp_texture);
                                SDL_FreeSurface(temp_surface);
                            } break;
                            case PAUSE_QUIT:
                                stop = true;
                                close();
                                exit(0);
                        }
                        break;
                }
            }
        }
    }
}

void SkinMenu() {
    SDL_Rect menu_rect;
    menu_rect.x = gGameRect.x;  // 0
    menu_rect.h = N_SKIN_OPTIONS * TTF_FontHeight(gFont) + 1;
    menu_rect.y = (gGameRect.h + 3 * 20 - menu_rect.h) / 2;
    menu_rect.w = gGameRect.w;

    SDL_Rect preview_rect;
    preview_rect.x = gGameRect.x;
    preview_rect.h = 3 * 20;
    preview_rect.y = (gGameRect.h - menu_rect.h - preview_rect.h) / 2;
    preview_rect.w = gGameRect.w;

    static int skin_selected = 0;
    int selection = 0, skin_previewed = skin_selected;
    if (skin_previewed == 0) {
        gSkin_Options[SKIN_NUMBER].assign("DEFAULT  SKIN");
    } else {
        gSkin_Options[SKIN_NUMBER].assign("SKIN  ");
        gSkin_Options[SKIN_NUMBER].append(std::to_string(skin_previewed));
    }
    RenderMenu(menu_rect, N_SKIN_OPTIONS, gSkin_Options, selection);
    RenderSkinPreview(preview_rect, skin_previewed);
    bool stop = false;
    while (!stop) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                stop = true;
                close();
                exit(0);
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        if (selection > 0) selection--;
                        RenderMenu(menu_rect, N_SKIN_OPTIONS, gSkin_Options,
                                   selection);
                        break;
                    case SDLK_DOWN:
                        if (selection < N_SKIN_OPTIONS - 1) selection++;
                        RenderMenu(menu_rect, N_SKIN_OPTIONS, gSkin_Options,
                                   selection);
                        break;
                    case SDLK_LEFT:
                        if (selection == SKIN_NUMBER &&
                            SkinExists(skin_previewed - 1)) {
                            skin_previewed--;

                            if (skin_previewed == 0) {
                                gSkin_Options[SKIN_NUMBER].assign(
                                    "DEFAULT  SKIN");
                            } else {
                                gSkin_Options[SKIN_NUMBER].assign("SKIN  ");
                                gSkin_Options[SKIN_NUMBER].append(
                                    std::to_string(skin_previewed));
                            }

                            RenderMenu(menu_rect, N_SKIN_OPTIONS, gSkin_Options,
                                       selection);
                            RenderSkinPreview(preview_rect, skin_previewed);
                        }
                        break;
                    case SDLK_RIGHT:
                        if (selection == SKIN_NUMBER &&
                            SkinExists(skin_previewed + 1)) {
                            skin_previewed++;

                            if (skin_previewed == 0) {
                                gSkin_Options[SKIN_NUMBER].assign(
                                    "DEFAULT  SKIN");
                            } else {
                                gSkin_Options[SKIN_NUMBER].assign("SKIN  ");
                                gSkin_Options[SKIN_NUMBER].append(
                                    std::to_string(skin_previewed));
                            }

                            RenderMenu(menu_rect, N_SKIN_OPTIONS, gSkin_Options,
                                       selection);
                            RenderSkinPreview(preview_rect, skin_previewed);
                        }
                        break;
                    case SDLK_SPACE:
                    case SDLK_RETURN:  // enter
                        switch (selection) {
                            case SKIN_NUMBER:

                                break;
                            case BACK:
                                skin_selected = skin_previewed;

                                SDL_DestroyTexture(gTextureWorm);

                                std::string path = gResourcesPath;
                                path.append("worm");
                                if (skin_previewed != 0)
                                    path.append(std::to_string(skin_previewed));
                                path.append(".png");

                                gTextureWorm = loadTexture(path);

                                stop = true;
                                break;
                        }
                        break;
                }
            }
        }
    }
}

void RenderMenu(SDL_Rect menu_rect, int nOptions, std::string* options,
                int selection) {
    SDL_SetRenderDrawColor(gRenderer, 0x10, 0x10, 0x10, 0xff);
    SDL_RenderFillRect(gRenderer, &menu_rect);

    int text_w, text_h;
    for (int i = 0; i < nOptions; i++) {
        loadTextToTexture(options[i], {0xff, 0xff, 0xff}, &text_w, &text_h);
        SDL_Rect text_rect;
        text_rect.x = (menu_rect.x + menu_rect.w - text_w) / 2;
        text_rect.y = menu_rect.y + i * text_h;
        text_rect.w = text_w;
        text_rect.h = text_h;
        SDL_RenderCopy(gRenderer, gTextTexture, NULL, &text_rect);
        if (i == selection) {
            SDL_SetRenderDrawColor(gRenderer, 0x88, 0, 0, 0xff);
            SDL_RenderDrawLine(gRenderer, menu_rect.x, text_rect.y,
                               menu_rect.x + menu_rect.w, text_rect.y);
            SDL_RenderDrawLine(
                gRenderer, menu_rect.x, text_rect.y + text_rect.h,
                menu_rect.x + menu_rect.w, text_rect.y + text_rect.h);
        }
    }

    SDL_RenderPresent(gRenderer);
}

void RenderSkinPreview(SDL_Rect preview_rect, int skin_previewed) {
    SDL_SetRenderDrawColor(gRenderer, 0x10, 0x10, 0x10, 0xff);
    SDL_RenderFillRect(gRenderer, &preview_rect);

    std::string path = gResourcesPath;
    path.append("worm");
    if (skin_previewed != 0) path.append(std::to_string(skin_previewed));
    path.append(".png");
    SDL_Texture* preview_texture = loadTexture(path);
    int parts[] = {TAIL_BLACK,  BODY_YELLOW, BODY_BLACK,
                   BODY_YELLOW, BODY_BLACK,  HEAD};

    for (int i = 0; i < 6; i++) {
        SDL_Rect src = {parts[i] * 20, 0, 20, 20};
        SDL_Rect dst = {(preview_rect.x + preview_rect.w - 6 * 20) / 2 + i * 20,
                        preview_rect.y + (preview_rect.h - 20) / 2, 20, 20};
        SDL_RenderCopyEx(gRenderer, preview_texture, &src, &dst, 90, NULL,
                         SDL_FLIP_NONE);
    }

    SDL_Texture* arrow_texture = loadTexture(ResourcesSlash("arrow.png"));
    if (SkinExists(skin_previewed - 1)) {
        SDL_Rect dst = {(preview_rect.x + preview_rect.w - 30) / 2 - 100,
                        preview_rect.y + (preview_rect.h - 30) / 2, 30, 30};
        SDL_RenderCopy(gRenderer, arrow_texture, NULL, &dst);
    }
    if (SkinExists(skin_previewed + 1)) {
        SDL_Rect dst = {(preview_rect.x + preview_rect.w - 30) / 2 + 100,
                        preview_rect.y + (preview_rect.h - 30) / 2, 30, 30};
        SDL_RenderCopyEx(gRenderer, arrow_texture, NULL, &dst, 0, NULL,
                         SDL_FLIP_HORIZONTAL);
    }

    SDL_RenderPresent(gRenderer);
}

bool SkinExists(int n) {
    std::string path = gResourcesPath;
    path.append("worm");
    if (n != 0) path.append(std::to_string(n));
    path.append(".png");
    FILE* temp_file = fopen(path.c_str(), "r");
    if (temp_file == NULL) {
        return false;
    } else {
        fclose(temp_file);
        return true;
    }
}

std::string ResourcesSlash(std::string file) {
    std::string path = gResourcesPath;
    path.append(file);
    return path;
}