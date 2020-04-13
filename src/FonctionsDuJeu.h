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


typedef struct playerMove{
  int player;
  bool selected;
  bool inTurn;
  bool firstMove;
  bool inBorder;
}playerMove;

void initialisationTab(cell tab[10][10],int nbrJoueurs);
void initialisationTabRest(bool tab[8], bool valeur);
void initilisationPlayerMove(playerMove *move);
int elementCorrespondant( int i, int j);
coordInt remplirCoordInt(int i, int j);
coordInt elementCorrespondantInverse(int i);
void TourJoueurs(cell tab[10][10],game param_partie, float x, float y, SDL_Rect plateau, coordInt *selectedBox, bool restriction[8],playerMove *move);
int autorisation(int i, int j, int k, int l, cell tab[10][10]);
void remplirTabRest(cell tab[10][10], bool restriction[8], playerMove *move,int i,int j);
bool autorisation_bord(cell tab[10][10], playerMove *move, coordInt depart, coordInt arrive, bool first_recusive);
#endif
