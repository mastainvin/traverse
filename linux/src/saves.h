#ifndef SAVES_H
#define SAVES_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FonctionsDuJeu.h"

/*----------------------------------------------------*/
/* Structures */

/*Structure de la liste des pions joués sur une case et de la valeur que chaque élément porte*/

typedef struct saves_list
{
  saves_value value;
  struct saves_list *next;
} saves_list, *saves;

/*----------------------------------------------------*/
/* Prototypes */
/*----------------------------------------------------*/
/* Fonctions principales */

/* Fonctions de gestion du fichier de sauvegardes */
FILE *open_saves_list(void);
void close_saves_list(FILE *file);
saves import_saves(FILE *file);
FILE *export_saves(saves li, FILE *file);

/* Fonctions de gestion de la liste des sauvegardes */
saves newSavesList(void);
bool is_empty_savesList(saves li);
bool exist_save(saves li, char *name);
saves delete_save(saves li, char name[NAME_LENGTH]);
saves add_save(saves li, saves_value newValue);
saves pop_front_savesList(saves li);
saves deleteAll_savesList(saves li);

#endif
