#ifndef FONCTIONDUJEU_H
#define FONCTIONDUJEU_H

#include <SDL2/SDL.h>

typedef struct cell{
    int joueur;
    int pion;
}cell;

typedef struct partie{
  int joueurs;
  int ordis;
}partie, *game;

typedef enum bool{
  false,
  true
}bool;

typedef struct coordInt{
  int x;
  int y;
}coordInt;

void initialisationTab(cell tab[10][10],int nbrJoueurs);
void initialisationTabRest(bool tab[8]);
void Tour2Joueurs(cell tab[10][10],float x, float y, SDL_Rect plateau, coordInt *selectedBox, bool *selected, int *player,  bool *inTurn);
int autorisation(int i, int j, int k, int l, cell tab[10][10]);

#endif
