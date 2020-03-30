#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_video.h>
#include <time.h>
#include <math.h>




/* Structures */

typedef enum bool{
  false,
  true
}bool;

typedef enum location{
  inMenu,
  inGame,
  inPause
}location;


typedef struct coordInt{
  int x;
  int y;
}coordInt;

typedef struct cell{
  int joueur;
  int pion;
}cell;

/* Prototype des fonctions */
void initTab(cell tab[10][10]);
void SDL_ExitWithError(const char *message, SDL_Texture *texture,SDL_Surface *surface, SDL_Renderer *renderer, SDL_Window *window);
void creationFond(SDL_Renderer *renderer,SDL_Rect plateau,SDL_Rect fenetre,SDL_Texture *textureDamier);
void generatePion(SDL_Window *window, SDL_Renderer *renderer,SDL_Rect plateau,cell tab[10][10]);
void afficherPion(SDL_Window *window, SDL_Renderer *renderer,SDL_Rect plateau,int joueur,int pion,int x,int y);
coordInt selectionPion(cell tab[10][10], float x, float y, SDL_Rect plateau);
void selectionMove(cell tab[10][10], float x, float y, SDL_Rect plateau, coordInt *movedPion);


void creationFondMenu(SDL_Renderer *renderer, SDL_Rect fenetre, SDL_Texture *textureTitre,SDL_Rect emplacementTitre);
void menu(SDL_Rect fenetre, SDL_Window *window, SDL_Renderer *renderer);
void jeu(SDL_Rect plateau,SDL_Rect fenetre,SDL_Window *window, SDL_Renderer *renderer,cell tab[10][10]);
#endif
