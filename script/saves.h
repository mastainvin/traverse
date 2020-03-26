#ifndef SAVES_H
#define SAVES_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*----------------------------------------------------*/
/* Constantes */
#define NAME_LENGTH 10
/*----------------------------------------------------*/
/* Structures */

/* Création du type booléen */
typedef enum bool{
 false,
 true
}bool;

/* Structure d'une case du plateau de jeu*/
typedef struct cell{
  int joueur;
  int pion;
  struct cell_pion_list *pion_list;
}cell;

/*Structure de la liste des pions joués sur une case et de la valeur que chaque élément porte*/

typedef struct cell_pion_list_value{
  int joueur;
  int pion;
}cell_pion_list_value;

typedef struct cell_pion_list {
  cell_pion_list_value value;
  struct cell_pion_list *next;
}cell_pion_list, *pion_list;

/* Structures listes de sauvegardes et de la valeur que chaque élément porte*/

typedef struct saves_value{
  cell tab[10][10];
  char name[NAME_LENGTH];
  int  turn;
}saves_value;

typedef struct saves_list{
  saves_value value;
  struct saves_list *next;
}saves_list, *saves;


/*----------------------------------------------------*/
/* Prototypes */

/* Fonctions secondaires */
void init(cell tab[10][10]);

/*----------------------------------------------------*/
/* Fonctions principales */

/* Fonctions de gestion du fichier de sauvegardes */
FILE *open_saves_list(void);
void close_saves_list(FILE *file);
saves import_saves(FILE *file);
FILE *export_saves(saves li,FILE *file);


/* Fonctions de gestion de la liste des sauvegardes */
saves newSavesList(void);
bool is_empty_savesList(saves li);
bool exist_save(saves li, char *name);
saves delete_save(saves li, char name[10]);
saves add_save(saves li, saves_value newValue);
saves pop_front_savesList(saves li);
saves deleteAll_savesList(saves li);

/* Fonctions de gestion des listes de Pions qui ont été joués sur une case */
pion_list newPionList(void);
bool is_empty_pionList(pion_list li);
pion_list add_pion(pion_list li, cell_pion_list_value newValue);
pion_list pop_front_pionList(pion_list li);
pion_list deleteAll_pionList(pion_list li);

#endif
