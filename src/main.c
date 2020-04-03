#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "interface.h"
#include <time.h>



int main(void){

    /*  Tableau de tests */
    cell tab[10][10];
    initTab(tab);
    tab[1][2].joueur = 1;
    tab[1][2].pion = 4;

    tab[4][4].joueur = 2;
    tab[4][4].pion = 4;
    /* ------------------------- */

    /* Création des variables d'affichage celles-ci pourront être modifiés dans le programme*/

    // Taille de la fenetre
    SDL_Rect fenetre;
    fenetre.h = 800;
    fenetre.w = 1280;

    // Taille du plateau

    /* ------------------------- */

    partie *param_partie;
    param_partie = malloc(sizeof(*param_partie));
    initPartie(param_partie);

    /*SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
*/
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;


    if (SDL_Init(SDL_INIT_VIDEO) != 0)
      SDL_ExitWithError("Initialisation de la SDL",renderer,window);

    if(SDL_CreateWindowAndRenderer(fenetre.w,fenetre.h,SDL_WINDOW_FULLSCREEN,&window,&renderer) != 0)
      SDL_ExitWithError("Creation fenetre et rendu echouee",renderer,window);

    if(TTF_Init() != 0)
      SDL_ExitWithError("Chargement de ttf",renderer,window);

    //SDL_GetWindowSize(window,&fenetre.w,&fenetre.h);


    SDL_ShowCursor(SDL_DISABLE);

    location loc = inMenu;

    while (loc != quit) {
      switch (loc) {
        case inGame:
          loc = jeu(fenetre,window,renderer,tab);
        break;
        case inMenu:
          loc = menu(fenetre,window,renderer, param_partie);
        break;
        default:
          SDL_ExitWithError("ERREUR : localisation utilisation", renderer, window);
        break;
      }
  }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
