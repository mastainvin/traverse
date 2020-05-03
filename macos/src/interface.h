#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_video.h>
#include <time.h>
#include <math.h>
#include "FonctionsDuJeu.h"
#include "saves.h"
#include "config.h"

#define FRAME_PER_SECOND 60
#define CENTER 99999

/* Structures */

typedef enum location
{
  inMenu,
  inGame,
  inSave,
  quit
} location;

typedef enum menu_bouton
{
  jouer,
  rejouer,
  regles,
  options,
  quitter,
  sauvegarder,
  continuer,
  commencer,
  retour,
  pause,
  finir,
  rien

} menu_bouton;

typedef struct curseur
{
  SDL_Surface *image;
  SDL_Texture *texture;
  SDL_Rect position;
} curseur;

typedef struct coord
{
  float x;
  int y;
} coord;

typedef struct nom_sauvegarde
{
  char nom[NAME_LENGTH];
  int lettre;
  int position;
} nom_sauvegarde;

typedef struct fleche
{
  bool haut;
  bool bas;
  int element;
} fleche;

/* Prototype des fonctions */

void SDL_ExitWithError(const char *message, SDL_Renderer *renderer, SDL_Window *window);

void creationBackground(SDL_Renderer *renderer, SDL_Rect *fenetre, config_type config);
void initPartie(game param_partie);

menu_bouton menu_principal(SDL_Renderer *renderer, SDL_Window *window, SDL_Rect *fenetre, coord coordCurseur, coord coordClic, config_type config);
void menu_secondaire(SDL_Renderer *renderer, SDL_Window *window, SDL_Rect *fenetre, menu_bouton *selection, coord coordCurseur, coord coordClic, saves_value *partie, fleche *bouton_fleche, config_type *config, saves *li);

void plateauDeJeu(SDL_Renderer *renderer, SDL_Window *window, SDL_Rect *fenetre, coord coordClic, coord coordCurseur, bool *inPause, saves_value *parti, config_type config);
void generatePion(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect plateau, saves_value *partie, config_type config);
void afficherPion(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect plateau, int joueur, int pion, int x, int y, config_type config);

location menu_pause(SDL_Renderer *renderer, SDL_Window *window, SDL_Rect *fenetre, coord coordCurseur, coord coordClic, bool *inPause, saves_value *partie, config_type config);
location menu_save(SDL_Renderer *renderer, SDL_Window *window, SDL_Rect *fenetre, cell tab[10][10], coord coordCurseur, coord coordClic, bool *inPause, nom_sauvegarde *save_name, bool *saving, config_type config);
void initNomSauvegarde(nom_sauvegarde *save_name);

void afficherTexte(char *text, float x, float y, SDL_Color color, int taille, SDL_Renderer *renderer, SDL_Window *window, SDL_Rect *fenetre);
void afficherImage(char *lienImage, float x, float y, SDL_Renderer *renderer, SDL_Window *window, SDL_Rect *fenetre, config_type config);

void limit_fps(unsigned int limit, SDL_Rect *fenetre, SDL_Window *window, SDL_Renderer *renderer, config_type config);
void FiltreDeplacement(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect plateau, saves_value *partie);

#endif
