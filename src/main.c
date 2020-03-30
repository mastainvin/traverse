#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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
    fenetre.h = 700;
    fenetre.w = 800;

    // Taille du plateau
    SDL_Rect plateau;
    plateau.h = 500;
    plateau.w = 500;
    plateau.x = (fenetre.w-plateau.w)/2; // Placer au mileu
    plateau.y = (fenetre.h-plateau.h)/2; // Placer au milieu

    /* ------------------------- */


    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;



    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
      SDL_ExitWithError("Initialisation de la SDL",NULL,NULL,renderer,window);

    if(SDL_CreateWindowAndRenderer(fenetre.w,fenetre.h,0,&window,&renderer) != 0)
      SDL_ExitWithError("Creation fenetre et rendu echouee",NULL,NULL,renderer,window);

    location loc = inMenu;

    switch (loc) {
      case inGame:
        jeu(plateau,fenetre,window,renderer,tab);
      break;
      case inMenu:
        menu(fenetre,window,renderer);
      break;
      default:
        exit(EXIT_FAILURE);
      break;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
