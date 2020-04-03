#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_video.h>
#include <time.h>
#include <math.h>


#define FRAME_PER_SECOND 33

#define MENU_SEC_W 800
#define MENU_SEC_H 400



/* Structures */

typedef enum bool{
  false,
  true
}bool;

typedef enum location{
  inMenu,
  inGame,
  quit
}location;

typedef enum menu_bouton{
  jouer,
  regles,
  options,
  quitter,
  sauvegarder,
  continuer,
  commencer,
  retour,
  pause,
  rien

}menu_bouton;

typedef struct curseur{
  SDL_Surface *image;
  SDL_Texture *texture;
  SDL_Rect position;
}curseur;

typedef struct coordInt{
  int x;
  int y;
}coordInt;

typedef struct coord{
  float x;
  int y;
}coord;

typedef struct cell{
  int joueur;
  int pion;
}cell;

typedef struct partie{
  int joueurs;
  int ordis;
}partie, *game;

/* Prototype des fonctions */
void initTab(cell tab[10][10]);
void SDL_ExitWithError(const char *message, SDL_Renderer *renderer, SDL_Window *window);

curseur creerCurseur(SDL_Renderer *renderer, SDL_Window *window);
void afficherCurseur(curseur monCurseur, SDL_Renderer *renderer);
void detruireCurseur(curseur monCurseur);

void creationBackground(SDL_Renderer *renderer, SDL_Rect fenetre);
void initPartie(game param_partie);

location menu(SDL_Rect fenetre, SDL_Window *window, SDL_Renderer *renderer, game param_partie);
menu_bouton menu_principal(SDL_Renderer *renderer,SDL_Window *window, SDL_Rect fenetre, coord coordCurseur, coord coordClic);
menu_bouton menu_secondaire(SDL_Renderer *renderer, SDL_Window *window, SDL_Rect fenetre, menu_bouton selection, coord coordCurseur, coord coordClic, game para);
location jeu(SDL_Rect fenetre,SDL_Window *window, SDL_Renderer *renderer,cell tab[10][10]);
SDL_Rect creationFond(SDL_Renderer *renderer,SDL_Window *window,SDL_Rect fenetre,coord coordClic, coord coordCurseur, bool *pause);
void generatePion(SDL_Window *window, SDL_Renderer *renderer,SDL_Rect plateau,cell tab[10][10]);
void afficherPion(SDL_Window *window, SDL_Renderer *renderer,SDL_Rect plateau,int joueur,int pion,int x,int y);
coordInt selectionPion(cell tab[10][10], float x, float y, SDL_Rect plateau);
void selectionMove(cell tab[10][10], float x, float y, SDL_Rect plateau, coordInt *movedPion);
location menu_pause(SDL_Renderer *renderer, SDL_Window *window, SDL_Rect fenetre,coord coordCurseur, coord coordClic, bool *inPause);

void limit_fps(unsigned int limit, SDL_Rect fenetre,SDL_Window *window, SDL_Renderer *renderer);
#endif
