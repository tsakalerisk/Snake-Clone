#include "Game.hpp"

//enum { START, MAIN_CHANGE_SKIN, MAIN_QUIT, N_MAIN_OPTIONS };
//enum { SKIN_NUMBER, BACK, N_SKIN_OPTIONS };

// void MainMenu();
// void SkinMenu();
// void RenderSkinPreview(SDL_Rect preview_rect, int skin_previewed);
// bool SkinExists(int n);

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

std::string gResourcesPath = "..\\Resources\\";

long gScore = 0, gHighScore;
//std::string gMain_Options[N_MAIN_OPTIONS] = {"START", "CHANGE  SKIN", "QUIT"};
//std::string gSkin_Options[N_SKIN_OPTIONS] = {"000", "BACK"};

SDL_Window* gWindow;
SDL_Renderer* gRenderer;

int main(int argc, char* agrs[]) {
    Game("Worm", SCREEN_WIDTH, SCREEN_HEIGHT);
    return 0;
}

// void MainMenu() {
//     SDL_Rect menu_rect;
//     menu_rect.x = gGameRect.x;  // 0
//     menu_rect.h = N_MAIN_OPTIONS * TTF_FontHeight(gFont) + 1;
//     menu_rect.y = (gGameRect.h - menu_rect.h) / 2;
//     menu_rect.w = gGameRect.w;
//     int selection = 0;  // START
//     RenderMenu(menu_rect, N_MAIN_OPTIONS, gMain_Options, selection);
//     bool stop = false;
//     while (!stop) {
//         SDL_Event e;
//         while (SDL_PollEvent(&e) != 0) {
//             if (e.type == SDL_QUIT) {
//                 stop = true;
//                 close();
//                 exit(0);
//             } else if (e.type == SDL_KEYDOWN) {
//                 switch (e.key.keysym.sym) {
//                     case SDLK_UP:
//                         if (selection > 0) selection--;
//                         RenderMenu(menu_rect, N_MAIN_OPTIONS, gMain_Options,
//                                    selection);
//                         break;
//                     case SDLK_DOWN:
//                         if (selection < N_MAIN_OPTIONS - 1) selection++;
//                         RenderMenu(menu_rect, N_MAIN_OPTIONS, gMain_Options,
//                                    selection);
//                         break;
//                     case SDLK_SPACE:
//                     case SDLK_RETURN:  // enter
//                         switch (selection) {
//                             case START:
//                                 stop = true;
//                                 break;
//                             case MAIN_CHANGE_SKIN: {
//                                 SDL_Surface* temp_surface =
//                                     SDL_CreateRGBSurface(0, SCREEN_WIDTH,
//                                                          SCREEN_HEIGHT, 32,
//                                                          0, 0, 0, 0);
//                                 SDL_RenderReadPixels(gRenderer, NULL, 0,
//                                                      temp_surface->pixels,
//                                                      temp_surface->pitch);
//                                 SDL_Texture* temp_texture =
//                                     SDL_CreateTextureFromSurface(gRenderer,
//                                                                  temp_surface);
//                                 SkinMenu();
//                                 SDL_RenderCopy(gRenderer, temp_texture, NULL,
//                                                NULL);
//                                 SDL_RenderPresent(gRenderer);
//                                 SDL_DestroyTexture(temp_texture);
//                                 SDL_FreeSurface(temp_surface);
//                             } break;
//                             case MAIN_QUIT:
//                                 stop = true;
//                                 close();
//                                 exit(0);
//                         }
//                         break;
//                 }
//             }
//         }
//     }
// }

// void SkinMenu() {
//     SDL_Rect menu_rect;
//     menu_rect.x = gGameRect.x;  // 0
//     menu_rect.h = N_SKIN_OPTIONS * TTF_FontHeight(gFont) + 1;
//     menu_rect.y = (gGameRect.h + 3 * 20 - menu_rect.h) / 2;
//     menu_rect.w = gGameRect.w;

//     SDL_Rect preview_rect;
//     preview_rect.x = gGameRect.x;
//     preview_rect.h = 3 * 20;
//     preview_rect.y = (gGameRect.h - menu_rect.h - preview_rect.h) / 2;
//     preview_rect.w = gGameRect.w;

//     static int skin_selected = 0;
//     int selection = 0, skin_previewed = skin_selected;
//     if (skin_previewed == 0) {
//         gSkin_Options[SKIN_NUMBER].assign("DEFAULT  SKIN");
//     } else {
//         gSkin_Options[SKIN_NUMBER].assign("SKIN  ");
//         gSkin_Options[SKIN_NUMBER].append(std::to_string(skin_previewed));
//     }
//     RenderMenu(menu_rect, N_SKIN_OPTIONS, gSkin_Options, selection);
//     RenderSkinPreview(preview_rect, skin_previewed);
//     bool stop = false;
//     while (!stop) {
//         SDL_Event e;
//         while (SDL_PollEvent(&e) != 0) {
//             if (e.type == SDL_QUIT) {
//                 stop = true;
//                 close();
//                 exit(0);
//             } else if (e.type == SDL_KEYDOWN) {
//                 switch (e.key.keysym.sym) {
//                     case SDLK_UP:
//                         if (selection > 0) selection--;
//                         RenderMenu(menu_rect, N_SKIN_OPTIONS, gSkin_Options,
//                                    selection);
//                         break;
//                     case SDLK_DOWN:
//                         if (selection < N_SKIN_OPTIONS - 1) selection++;
//                         RenderMenu(menu_rect, N_SKIN_OPTIONS, gSkin_Options,
//                                    selection);
//                         break;
//                     case SDLK_LEFT:
//                         if (selection == SKIN_NUMBER &&
//                             SkinExists(skin_previewed - 1)) {
//                             skin_previewed--;

//                             if (skin_previewed == 0) {
//                                 gSkin_Options[SKIN_NUMBER].assign(
//                                     "DEFAULT  SKIN");
//                             } else {
//                                 gSkin_Options[SKIN_NUMBER].assign("SKIN  ");
//                                 gSkin_Options[SKIN_NUMBER].append(
//                                     std::to_string(skin_previewed));
//                             }

//                             RenderMenu(menu_rect, N_SKIN_OPTIONS,
//                             gSkin_Options,
//                                        selection);
//                             RenderSkinPreview(preview_rect, skin_previewed);
//                         }
//                         break;
//                     case SDLK_RIGHT:
//                         if (selection == SKIN_NUMBER &&
//                             SkinExists(skin_previewed + 1)) {
//                             skin_previewed++;

//                             if (skin_previewed == 0) {
//                                 gSkin_Options[SKIN_NUMBER].assign(
//                                     "DEFAULT  SKIN");
//                             } else {
//                                 gSkin_Options[SKIN_NUMBER].assign("SKIN  ");
//                                 gSkin_Options[SKIN_NUMBER].append(
//                                     std::to_string(skin_previewed));
//                             }

//                             RenderMenu(menu_rect, N_SKIN_OPTIONS,
//                             gSkin_Options,
//                                        selection);
//                             RenderSkinPreview(preview_rect, skin_previewed);
//                         }
//                         break;
//                     case SDLK_SPACE:
//                     case SDLK_RETURN:  // enter
//                         switch (selection) {
//                             case SKIN_NUMBER:

//                                 break;
//                             case BACK:
//                                 skin_selected = skin_previewed;

//                                 SDL_DestroyTexture(gTextureWorm);

//                                 std::string path = gResourcesPath;
//                                 path.append("worm");
//                                 if (skin_previewed != 0)
//                                     path.append(std::to_string(skin_previewed));
//                                 path.append(".png");

//                                 gTextureWorm = loadTexture(path);

//                                 stop = true;
//                                 break;
//                         }
//                         break;
//                 }
//             }
//         }
//     }
// }

// void RenderSkinPreview(SDL_Rect preview_rect, int skin_previewed) {
//     SDL_SetRenderDrawColor(gRenderer, 0x10, 0x10, 0x10, 0xff);
//     SDL_RenderFillRect(gRenderer, &preview_rect);

//     std::string path = gResourcesPath;
//     path.append("worm");
//     if (skin_previewed != 0) path.append(std::to_string(skin_previewed));
//     path.append(".png");
//     SDL_Texture* preview_texture = loadTexture(path);
//     int parts[] = {TAIL_BLACK,  BODY_YELLOW, BODY_BLACK,
//                    BODY_YELLOW, BODY_BLACK,  HEAD};

//     for (int i = 0; i < 6; i++) {
//         SDL_Rect src = {parts[i] * 20, 0, 20, 20};
//         SDL_Rect dst = {(preview_rect.x + preview_rect.w - 6 * 20) / 2 + i *
//         20,
//                         preview_rect.y + (preview_rect.h - 20) / 2, 20, 20};
//         SDL_RenderCopyEx(gRenderer, preview_texture, &src, &dst, 90, NULL,
//                          SDL_FLIP_NONE);
//     }

//     SDL_Texture* arrow_texture = loadTexture(ResourcesSlash("arrow.png"));
//     if (SkinExists(skin_previewed - 1)) {
//         SDL_Rect dst = {(preview_rect.x + preview_rect.w - 30) / 2 - 100,
//                         preview_rect.y + (preview_rect.h - 30) / 2, 30, 30};
//         SDL_RenderCopy(gRenderer, arrow_texture, NULL, &dst);
//     }
//     if (SkinExists(skin_previewed + 1)) {
//         SDL_Rect dst = {(preview_rect.x + preview_rect.w - 30) / 2 + 100,
//                         preview_rect.y + (preview_rect.h - 30) / 2, 30, 30};
//         SDL_RenderCopyEx(gRenderer, arrow_texture, NULL, &dst, 0, NULL,
//                          SDL_FLIP_HORIZONTAL);
//     }

//     SDL_RenderPresent(gRenderer);
// }

// bool SkinExists(int n) {
//     std::string path = gResourcesPath;
//     path.append("worm");
//     if (n != 0) path.append(std::to_string(n));
//     path.append(".png");
//     FILE* temp_file = fopen(path.c_str(), "r");
//     if (temp_file == NULL) {
//         return false;
//     } else {
//         fclose(temp_file);
//         return true;
//     }
// }
