#ifndef FONCTIONDUJEU_H
#define FONCTIONDUJEU_H

#include <SDL2/SDL.h>

#define PION_LIST_LENGTH 32
#define NAME_LENGTH 14
#define CHRONO 15

typedef struct cell
{
  int joueur;
  int pion;
  int distinction;
} cell;

typedef struct list_pion_visit
{
  int joueur;
  int pion;
  int distinction;
  int compteur;
} list_pion_visit;

typedef enum bool
{
  false,
  true
} bool;

typedef struct partie
{
  int joueurs;
  int ordis;
  int niveau;
  bool chrono;
} partie, *game;

typedef struct coordInt
{
  int x;
  int y;
} coordInt;

typedef struct playerMove
{
  int player;
  bool selected;
  bool inTurn;
  bool firstMove;
  bool inBorder;
} playerMove;
typedef struct alive_players
{
  bool joueur1;
  bool joueur2;
  bool joueur3;
  bool joueur4;
} alive_players;
typedef struct saves_value
{
  list_pion_visit caseVisit[10][10][PION_LIST_LENGTH];
  cell tab[10][10];
  char name[NAME_LENGTH];
  int turn;
  partie parametre;
  playerMove move;
  coordInt selectedBox;
  bool restriction[8];
  bool joueursEnVie[4];
  int time;
} saves_value;

typedef struct fin_partie
{
  int joueur;
  int fin;
} fin_partie;

/* Prototypes de fonctions */

// Fonctions d'initialisatoin
void copierTab(cell tab[10][10], cell newTab[10][10]);
void copierTabRest(bool tab[8], bool newTab[8]);
void initialisationCaseVisit(list_pion_visit tab[10][10][PION_LIST_LENGTH]);
void initialisationTab(cell tab[10][10], int nbrJoueurs);
void initialisationTabRest(bool tab[8], bool valeur);
void initilisationPlayerMove(playerMove *move);
void initialisationAlive(bool tab[4], int nombre_joueur);

// Fonctions de sélection
coordInt selectionPion(cell tab[10][10], float x, float y, SDL_Rect plateau);
coordInt selectionMove(cell tab[10][10], float x, float y, SDL_Rect plateau);

// Fonction utilitaires
int elementCorrespondant(int i, int j);
coordInt elementCorrespondantInverse(int i);
coordInt remplirCoordInt(int i, int j);

// Fonction de validation de coup
bool autorisation(int i, int j, int k, int l, cell tab[10][10]);
void remplirTabRest(cell tab[10][10], bool restriction[8], playerMove *move, int i, int j);
bool autorisation_bord(saves_value *partie ,coordInt depart, coordInt arrive);

// Fonction de gestion de pion
void TourJoueurs(saves_value *partie, float x, float y, SDL_Rect plateau);
fin_partie conditionDeFin(saves_value *partie);
#endif
