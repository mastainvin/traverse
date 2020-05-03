#include <stdio.h>
#include <stdlib.h>
#include "saves.h"
#include "FonctionsDuJeu.h"
/* Fonctions de gestion du fichier de sauvegardes */

// Ouverture du fichier
FILE *open_saves_list(void)
{
  FILE *file = fopen("./../data/saves.txt", "r+");
  // On vérifie qu'il n'y a pas eu d'erreur lors de l'ouverture
  if (file == NULL)
  {
    printf("ERREUR D'OUVERTURE DU FICHIER 'saves.txt'\n");
    exit(1);
  }
  return file;
}

/*----------------------------------------------------*/

void close_saves_list(FILE *file)
{
  fclose(file);
}
/*----------------------------------------------------*/

saves import_saves(FILE *file)
{
  /* Le fichier est-il vide ? */
  if (feof(file))
    return newSavesList();

  /* On crée un nouvelle liste comportant tout les éléments du fichier*/
  saves_list *li = newSavesList();
  saves_value element;
  initialisationCaseVisit(element.caseVisit);
  initilisationPlayerMove(&element.move);

  // On récupère chaque lignes et on les envoie dans la liste
  while (fscanf(file, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", element.name, &element.turn, &element.parametre.joueurs, &element.parametre.ordis, &element.parametre.niveau, &element.parametre.chrono, &element.move.player, &element.move.selected, &element.move.inTurn, &element.move.firstMove, &element.move.inBorder, &element.selectedBox.x, &element.selectedBox.y, &element.time, &element.joueursEnVie[0], &element.joueursEnVie[1], &element.joueursEnVie[2], &element.joueursEnVie[3], &element.restriction[0], &element.restriction[1], &element.restriction[2], &element.restriction[3], &element.restriction[4], &element.restriction[5], &element.restriction[6], &element.restriction[7]) == 26)
  {
    for (int i = 0; i < 10; i++)
    {
      for (int j = 0; j < 10; j++)
      {
        fseek(file, 6, SEEK_CUR);
        fscanf(file, "%d %d %d", &element.tab[i][j].joueur, &element.tab[i][j].pion, &element.tab[i][j].distinction);
        int k = 0;
        while (fscanf(file, "%d", &element.caseVisit[i][j][k].compteur) == 1)
        {
          k++;
        }
      }
    }

    li = add_save(li, element);
    fscanf(file, "\n");
  }
  return li;
}
/*----------------------------------------------------*/

FILE *export_saves(saves li, FILE *file)
{
  // On réouvre le fichier pour supprimer son contenu
  fclose(file);
  file = fopen("./../data/saves.txt", "w+");
  if (file == NULL)
  {
    printf("ERREUR D'OUVERTURE DU FICHIER 'saves.txt'\n");
    exit(1);
  }
  // On parcours toute la liste et on l'écris dans le fichier
  saves_list *temp = li;
  while (temp != NULL)
  {
    fprintf(file, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d ", temp->value.name, temp->value.turn, temp->value.parametre.joueurs, temp->value.parametre.ordis, temp->value.parametre.niveau, temp->value.parametre.chrono, temp->value.move.player, temp->value.move.selected, temp->value.move.inTurn, temp->value.move.firstMove, temp->value.move.inBorder, temp->value.selectedBox.x, temp->value.selectedBox.y, temp->value.time, temp->value.joueursEnVie[0], temp->value.joueursEnVie[1], temp->value.joueursEnVie[2], temp->value.joueursEnVie[3]);
    for (int i = 0; i < 8; i++)
    {
      fprintf(file, "%d ", temp->value.restriction[i]);
    }
    fprintf(file, "\n");
    for (int i = 0; i < 10; i++)
    {
      for (int j = 0; j < 10; j++)
      {
        fprintf(file, "# %d %d ", i, j);
        fprintf(file, "%d %d %d ", temp->value.tab[i][j].joueur, temp->value.tab[i][j].pion, temp->value.tab[i][j].distinction);

        for (int k = 0; k < PION_LIST_LENGTH; k++)
        {
          fprintf(file, "%d ", temp->value.caseVisit[i][j][k].compteur);
        }

        fprintf(file, "\n");
      }
    }
    temp = temp->next;
  }
  return file;
}
/*----------------------------------------------------*/
/* Fonctions gérant la liste de sauvegardes */
/*----------------------------------------------------*/

// Création d'une nouvelle liste vide
saves newSavesList(void)
{
  return NULL;
}
/*----------------------------------------------------*/

// La liste est-elle vide ?
bool is_empty_savesList(saves li)
{
  // Si le premier élément n'existe pas alors l'ensemble n'existe pas (utilisation des boolléen créés)
  if (li == NULL)
    return true;

  return false;
}
/*----------------------------------------------------*/

bool exist_save(saves li, char *name)
{
  saves_list *temp = li;
  while (temp != NULL)
  {
    if (strcmp(temp->value.name, name) == 0)
    {
      return true;
    }
    temp = temp->next;
  }

  return false;
}
/*----------------------------------------------------*/

// Fonction de suppression d'une sauvegarde
saves delete_save(saves li, char *name)
{
  /* Création des variables temporaires permettant de naviguer
  la liste sans la modifier */
  saves_list *temp = li;
  saves_list *before = li;

  // On parcours toute la liste en vérifiant les noms
  while (temp != NULL)
  {
    if (strcmp(temp->value.name, name) == 0)
    {
      /* Si cela correspond au premier*/
      if (temp == li)
      {
        saves_list *element;
        element = malloc(sizeof(*element));
        /*On verfie que l'allocation dynamique a bien fonctionné*/
        if (element == NULL)
        {
          printf("ERREUR D'ALLOCATION DYNAMIQUE.\n");
          exit(1);
        }
        // On définie l'élément suivant comme premier élément
        element = li->next;
        // On libère l'ancien premier élément
        free(li);
        return element;
      }
      /* Si cela correspond au dernier : on le libère directement*/
      if (li->next == NULL)
      {
        free(li);
        li = NULL;
        return newSavesList();
      }
      /* Sinon entre deux valeurs : vide la valeur puis raccorde avec celles autour*/
      before->next = temp->next;
      free(temp);
      temp = NULL;
      return li;
    }
    /* Décale d'un rang en gardant le précédant*/
    before = temp;
    temp = temp->next;
  }
  return li;
}
/*----------------------------------------------------*/

// Fonction de rajout d'une sauvegarde
saves add_save(saves li, saves_value newValue)
{
  // Allocation de mémoire pour la nouvelle sauvegarde
  saves_list *newSave;
  newSave = malloc(sizeof(*newSave));
  
  /*On verfie que l'allocation dynamique a bien fonctionné*/
  if (newSave == NULL)
  {
    printf("ERREUR D'ALLOCATION DYNAMIQUE.\n");
    exit(1);
  }

  // On attribut la valeur spécifiée
  newSave->value = newValue;
  newSave->next = NULL;

  // Si la liste est vide on renvoie directement la valeur
  if (is_empty_savesList(li))
  {
    return newSave;
  }

  saves temp = li;
  // Sinon on parcours l'ensemble de la liste pour arriver jusqu'au dernier
  while (temp->next != NULL)
  {
    temp = temp->next;
  }

  temp->next = newSave;
 
  return li;
}

/*----------------------------------------------------*/

// Fonction de suppression du premier élément de la liste (utilisé pour la suppression de la liste entière)
saves pop_front_savesList(saves li)
{
  // Si la liste est déjà vide ont retourne une liste vide
  if (is_empty_savesList(li))
  {
    return newSavesList();
  }
  /* Si cela correpond au dernier */
  if (li->next == NULL)
  {
    free(li);
    li = NULL;
    return newSavesList();
  }
  /* On récupère l'élément suivant pour le définir comme la liste et on libère le premier*/
  saves_list *element;
  element = malloc(sizeof(*element));
  element = li->next;
  free(li);
  return element;
}

/*----------------------------------------------------*/

// Fonction de suppression entière de la liste (Pour libèrer la mémoire en fin de programme)
saves deleteAll_savesList(saves li)
{
  // On parcours toute la liste et on supprime chaque éléments
  while (li != NULL)
  {
    li = pop_front_savesList(li);
  }
  return li;
}

/*----------------------------------------------------*/
