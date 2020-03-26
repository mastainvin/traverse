#include "saves.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*----------------------------------------------------*/
/* Fonctions secondaires */
/*----------------------------------------------------*/

void init(cell tab[10][10]){
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      tab[i][j].joueur = 0;
      tab[i][j].pion = 0;
      tab[i][j].pion_list = newPionList();
    }
  }

}
/*----------------------------------------------------*/
/* Fonctions principales */
/*----------------------------------------------------*/

/* Fonctions de gestion du fichier de sauvegardes */

// Ouverture du fichier
FILE *open_saves_list(void){
  FILE *file = fopen("./../data/saves.txt","r+");
  // On vérifie qu'il n'y a pas eu d'erreur lors de l'ouverture
  if(file == NULL){
    printf("ERREUR D'OUVERTURE DU FICHIER 'saves.txt'\n");
    exit(1);
  }
  return file;
}
/*----------------------------------------------------*/

void close_saves_list(FILE *file){
  fclose(file);
}
/*----------------------------------------------------*/

saves import_saves(FILE *file){
  /* Le fichier est-il vide ? */
  if (feof(file))
    return newSavesList();

    /* On crée un nouvelle liste comportant tout les éléments du fichier*/
    saves_list *li = newSavesList();
    saves_value element;
    cell_pion_list_value element2;


  // On récupère chaque lignes et on les envoie dans la liste
  while (fscanf(file,"%s %d\n", element.name, &element.turn) == 2){
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      element.tab[i][j].pion_list = newPionList();
      fseek(file,6,SEEK_CUR);
      fscanf(file,"%d %d",&element.tab[i][j].joueur,&element.tab[i][j].pion);

      while (fscanf(file,"%d %d", &element2.joueur,&element2.pion) == 2) {
        element.tab[i][j].pion_list = add_pion(element.tab[i][j].pion_list,element2);
      }
    }
  }

  li = add_save(li,element);
  fscanf(file,"\n");
}
  return li;
}
/*----------------------------------------------------*/

FILE *export_saves(saves li,FILE *file){
  // On réouvre le fichier pour supprimer son contenu
  fclose(file);
  file = fopen("./../data/saves.txt","w+");
  if(file == NULL){
    printf("ERREUR D'OUVERTURE DU FICHIER 'saves.txt'\n");
    exit(1);
  }
  // On parcours toute la liste et on l'écris dans le fichier
  saves_list *temp = li;
  while (temp != NULL) {
    fprintf(file, "%s %d\n", temp->value.name, temp->value.turn);

    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
        fprintf(file, "# %d %d ", i,j);
        cell_pion_list *temp2 = temp->value.tab[i][j].pion_list;
        fprintf(file,"%d %d ",temp->value.tab[i][j].joueur,temp->value.tab[i][j].pion);
        while (temp2 != NULL) {

          fprintf(file, "%d %d ", temp2->value.joueur, temp2->value.pion);
          temp2 = temp2->next;
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
saves newSavesList(void){
  return NULL;
}
/*----------------------------------------------------*/

// La liste est-elle vide ?
bool is_empty_savesList(saves li){
  // Si le premier élément n'existe pas alors l'ensemble n'existe pas (utilisation des boolléen créés)
  if (li == NULL)
    return true;

  return false;
}
/*----------------------------------------------------*/

bool exist_save(saves li, char *name){
  saves_list *temp = li;
  while (temp != NULL) {
    if (strcmp(temp->value.name, name) == 0) {
      return true;
    }
    temp = temp->next;
  }

  return false;
}
/*----------------------------------------------------*/

// Fonction de suppression d'une sauvegarde
saves delete_save(saves li, char *name){
  /* Création des variables temporaires permettant de naviguer
  la liste sans la modifier */
  saves_list *temp = li;
  saves_list *before = li;

  // On parcours toute la liste en vérifiant les noms
  while (temp != NULL) {
    if (strcmp(temp->value.name,name) == 0) {
      /* Si cela correspond au premier*/
      if (temp == li) {
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
      if (li->next == NULL){
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
saves add_save(saves li, saves_value newValue){
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
  // Si la liste n'est pas vide alors on crée le lien entre le nouvel élément est l'ancien premier élément
  if(!is_empty_savesList(li)){
    newSave->next = li;
  }
  return newSave;
}

/*----------------------------------------------------*/

// Fonction de suppression du premier élément de la liste (utilisé pour la suppression de la liste entière)
saves pop_front_savesList(saves li) {
  // Si la liste est déjà vide ont retourne une liste vide
  if(is_empty_savesList(li)){
    return newSavesList();
  }
  /* Si cela correpond au dernier */
  if(li->next == NULL){
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
saves deleteAll_savesList(saves li){
  // On parcours toute la liste et on supprime chaque éléments
  while (li != NULL) {
    li = pop_front_savesList(li);

    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
        // On parcours toute les cases du tableau de chaque sauvegarde et on vide chaque liste de pions qui ont déjà été sur la case
        saves_list *temp = li;
        while (temp != NULL) {
          temp->value.tab[i][j].pion_list = deleteAll_pionList(temp->value.tab[i][j].pion_list);
          temp = temp->next;
        }
      }
    }
  }
  return li;
}
/*----------------------------------------------------*/
/* Fonctions de gestion des listes de Pions qui ont été joués sur une case */
/*----------------------------------------------------*/

// Création d'une nouvelle liste de Pions
pion_list newPionList(void){
  return NULL;
}

// La liste est-elle vide ?
bool is_empty_pionList(pion_list li){
  // Si le premier élément n'existe pas alors l'ensemble n'existe pas (utilisation des boolléen créés)
  if (li == NULL)
    return true;

  return false;
}
/*----------------------------------------------------*/

// Fonction de rajout d'un pion dans la liste
pion_list add_pion(pion_list li, cell_pion_list_value newValue){
  // Allocation dynamique de la mémoire du nouveau pion
  cell_pion_list *newPion;
  newPion = malloc(sizeof(*newPion));
  /*On verfie que l'allocation dynamique a bien fonctionné*/
  if (newPion == NULL)
  {
    printf("ERREUR D'ALLOCATION DYNAMIQUE.\n");
    exit(1);
  }
  // On attribut la valeur spécifiée
  newPion->value = newValue;
  // Si la liste n'est pas vide alors on crée le lien entre le nouvel élément est l'ancien premier élément
  if(!is_empty_pionList(li)){
    newPion->next = li;
  }
  return newPion;
}

/*----------------------------------------------------*/

// Fonction de suppression du premier élément
/* Pour cette liste, on ne créé pas de fonctions
de suppression en fonction d'un nom ou d'une valeur
car dans la partie cela ne se produira jamais*/
pion_list pop_front_pionList(pion_list li) {
  //Si la liste est déjà vide on revoit une liste vide
  if(is_empty_pionList(li)){
    return newPionList();
  }
  /* Si cela correpond au dernier */
  if(li->next == NULL){
    free(li);
    li = NULL;
    return newPionList();
  }
  /* On récupère l'élément suivant pour le définir comme la liste et on libère le premier*/
  cell_pion_list *element;
  element = malloc(sizeof(*element));
  element = li->next;
  free(li);
  return element;
}
/*----------------------------------------------------*/

// Fonctions de suppression de toutes une liste de pions
pion_list deleteAll_pionList(pion_list li){
  // On parcours toute la liste et on supprime chaque éléments
  while (li != NULL) {
    li = pop_front_pionList(li);
  }
  return li;
}
