#include "FonctionsDuJeu.h"
#include "interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>


//initialisons le tableau (chaque case vide repésentée par un 0)
void initialisationTab(cell tab[10][10],int nbrJoueurs){

  int temp = 0;
  for (int i = 0; i <= 9; i++) {
    for (int j = 0; j <= 9; j++) {
      tab[i][j].joueur = 0;
      tab[i][j].pion = 0;
    }
  }
//Placons les pions avec le carré = 1, le triangle = 2, le losange = 3, le rond = 4.
//En fonction du nombre de joueurs.Ici 2 joueurs
  for (int i = 1; i <= 4; i++) {
    tab[0][i].pion = (temp%4) + 1;
    tab[0][i].joueur = 2;
    tab[9][i].pion = (temp%4) + 1;
    tab[9][i].joueur = 1;
    temp = temp + 1;
  }
  temp = 4;
  for (int i = 5; i <= 8; i++) {
    tab[0][i].pion = temp;
    tab[0][i].joueur = 2;
    tab[9][i].pion = temp;
    tab[9][i].joueur = 1;
    temp = temp - 1;
  }
    temp = 0;

//Ici 3 joueurs
  if (nbrJoueurs == 3 || nbrJoueurs == 4) {
    for (int i = 1; i <= 4; i++) {
      tab[i][0].pion = (temp%4) + 1;
      tab[i][0].joueur = 3;
      temp = temp + 1;
    }
    temp = 4;
    for (int i = 5; i <= 8; i++) {
      tab[i][0].pion = temp;
      tab[i][0].joueur = 3;
      temp = temp - 1;
    }

    temp = 0;

    //Ici 4 joueurs
    if (nbrJoueurs == 4) {
      for (int i = 1; i <= 4; i++) {
        temp = temp + 1;
        tab[i][9].pion = temp;
        tab[i][9].joueur = 4;
      }
      temp = 4;
      for (int i = 5; i <= 8; i++) {
        tab[i][9].pion = temp;
        tab[i][9].joueur = 4;
        temp = temp - 1 ;
      }
    } // fin if (nbrJoueurs == 4)
  }// fin if (nbrJoueurs == 3 || nbrJoueurs == 4)}
}

void initialisationTabRest(bool tab[8], bool valeur){
  for (int i = 0; i < 8; i++) {
    tab[i] = valeur;
  }
}

void initilisationPlayerMove(playerMove *move){
  move->player = 1;
  move->selected = false;
  move->inTurn = false;
  move->firstMove = false;
}

int elementCorrespondant(int i, int j){
  if (i == -1 && j == -1)
    return 0;
  else if (i == -1 && j == 0)
    return 1;
  else if (i == -1 && j == 1)
    return 2;
  else if (i == 0 && j == -1)
    return 3;
  else if (i == 0 && j == 1)
    return 4;
  else if (i == 1 && j == -1)
    return 5;
  else if (i == 1 && j == 0)
    return 6;
  else if (i == 1 && j == 1)
    return 7;
  else
    return -1;
}

coordInt remplirCoordInt(int i, int j){
  coordInt valeur;
  valeur.x = i;
  valeur.y = j;

  return valeur;
}

coordInt elementCorrespondantInverse(int i){
  if (i == 0)
    return remplirCoordInt(-1,-1);
  else if(i == 1)
    return remplirCoordInt(-1,0);
  else if(i == 2)
    return remplirCoordInt(-1,1);
  else if(i == 3)
    return remplirCoordInt(0,-1);
  else if(i == 4)
    return remplirCoordInt(0,1);
  else if(i == 5)
    return remplirCoordInt(1,-1);
  else if(i == 6)
    return remplirCoordInt(1,0);
  else if(i == 7)
    return remplirCoordInt(1,1);
  else
    return remplirCoordInt(0,0);
}
// i, j, k et l sont les coordonnées de la pièce au début et après le tour.
int autorisation(int i, int j, int k, int l,cell tab[10][10]){
    int invalide = 0;
//cas pour le carre
    if( tab[i][j].pion == 1)
    {
      if(((i == k && j == l - 1) && (tab[i][j+1].pion == 0)) || ((tab[i][j+1].pion > 0) && (i == k && j == l - 2)))
      {
        invalide = 0;
      }
      else if(((i == k && j == l + 1) && (tab[i][j-1].pion == 0)) || ((tab[i][j-1].pion > 0) && (i == k && j == l + 2)))
      {
        invalide = 0;
      }
      else if(((i == k + 1 && j == l ) && (tab[i-1][j].pion == 0)) || ((tab[i-1][j].pion > 0) && (i == k + 2 && j == l)))
      {
        invalide = 0;
      }
      else if(((i == k - 1 && j == l ) && (tab[i+1][j].pion == 0)) || ((tab[i+1][j].pion > 0) && (i == k - 2 && j == l)))
      {
        invalide = 0;
      }
      else
      {
        invalide = 1;
      }
    }
    //cas pour le losange
    else if( tab[i][j].pion == 3)
    {
      if((i == k + 1 && j == l + 1)  || ((tab[i-1][j-1].pion > 0) && (i == k + 2 && j == l + 2)))
      {
        invalide = 0;
      }
      else if((i == k + 1 && j == l - 1) || ((tab[i-1][j+1].pion > 0) && (i == k + 2 && j == l - 2)))
      {
        invalide = 0;
      }
      else if((i == k - 1 && j == l + 1)  || ((tab[i+1][j-1].pion > 0) && (i == k - 2 && j == l + 2)))
      {
        invalide = 0;
      }
      else if((i == k - 1 && j == l - 1)  || ((tab[i+1][j+1].pion > 0) && (i == k - 2 && j == l - 2)))
      {
        invalide = 0;
      }
      else
      {
        invalide = 1;
      }
    }
    //cas pour le rond
    else if( tab[i][j].pion == 4)
    { //Si le rond veut se déplacer comme un carré
      if(((i == k && j == l - 1) && (tab[i][j+1].pion == 0)) || ((tab[i][j+1].pion > 0) && (i == k && j == l - 2)))
      {
        invalide = 0;
      }
      else if(((i == k && j == l + 1) && (tab[i][j-1].pion == 0)) || ((tab[i][j-1].pion > 0) && (i == k && j == l + 2)))
      {
        invalide = 0;
      }
      else if(((i == k + 1 && j == l ) && (tab[i-1][j].pion == 0)) || ((tab[i-1][j].pion > 0) && (i == k + 2 && j == l)))
      {
        invalide = 0;
      }
      else if(((i == k - 1 && j == l ) && (tab[i+1][j].pion == 0)) || ((tab[i+1][j].pion > 0) && (i == k - 2 && j == l)))
      {
        invalide = 0;
      }
       // Si le rond veut se déplacer comme un losange
      else if((i == k + 1 && j == l + 1)  || ((tab[i-1][j-1].pion > 0) && (i == k + 2 && j == l + 2)))
      {
        invalide = 0;
      }
      else if((i == k + 1 && j == l - 1) || ((tab[i-1][j+1].pion > 0) && (i == k + 2 && j == l - 2)))
      {
        invalide = 0;
      }
      else if((i == k - 1 && j == l + 1)  || ((tab[i+1][j-1].pion > 0) && (i == k - 2 && j == l + 2)))
      {
        invalide = 0;
      }
      else if((i == k - 1 && j == l - 1)  || ((tab[i+1][j+1].pion > 0) && (i == k - 2 && j == l - 2)))
      {
        invalide = 0;
      }
      else
      {
        invalide = 1;
      }
    }
    //cas pour le triangle
    else if(tab[i][j].pion == 2)
    {
      if(tab[i][j].joueur == 1)
      {
        if((i == k - 1 && j == l ) || ((tab[i+1][j].pion > 0) && (i == k - 2 && j == l)))
        {
          invalide = 0;
        }
        else if((i == k + 1 && j == l + 1)  || ((tab[i-1][j-1].pion > 0) && (i == k + 2 && j == l + 2)))
        {
          invalide = 0;
        }
        else if((i == k + 1 && j == l - 1) || ((tab[i-1][j+1].pion > 0) && (i == k + 2 && j == l - 2)))
        {
          invalide = 0;
        }
        else
        {
          invalide = 1;
        }
      } //Si le triangle appartient au joueur 2
      else if(tab[i][j].joueur == 2)
      {
        if((i == k + 1 && j == l ) || ((tab[i-1][j].pion > 0) && (i == k + 2 && j == l)))
        {
          invalide = 0;
        }
        else if((i == k - 1 && j == l + 1)  || ((tab[i+1][j-1].pion > 0) && (i == k - 2 && j == l + 2)))
        {
          invalide = 0;
        }
        else if((i == k - 1 && j == l - 1)  || ((tab[i+1][j+1].pion > 0) && (i == k - 2 && j == l - 2)))
        {
          invalide = 0;
        }
        else
        {
          invalide = 1;
        }
      } //Si le triangle appartient au joueur 3
      else if(tab[i][j].joueur == 3)
      {
        if((i == k && j == l + 1) || ((tab[i][j-1].pion > 0) && (i == k && j == l + 2)))
        {
          invalide = 0;
        }
        else if((i == k + 1 && j == l - 1) || ((tab[i-1][j+1].pion > 0) && (i == k + 2 && j == l - 2)))
        {
          invalide = 0;
        }
        else if((i == k - 1 && j == l - 1)  || ((tab[i+1][j+1].pion > 0) && (i == k - 2 && j == l - 2)))
        {
          invalide = 0;
        }
        else
        {
          invalide = 1;
        }
      } //Si le triangle appartient au joueur 4
      else if(tab[i][j].joueur == 4)
      {
        if((i == k && j == l - 1) || ((tab[i][j+1].pion > 0) && (i == k && j == l - 2)))
        {
          invalide = 0;
        }
        else if((i == k + 1 && j == l + 1)  || ((tab[i-1][j-1].pion > 0) && (i == k + 2 && j == l + 2)))
        {
          invalide = 0;
        }
        else if((i == k - 1 && j == l + 1)  || ((tab[i+1][j-1].pion > 0) && (i == k - 2 && j == l + 2)))
        {
          invalide = 0;
        }
        else
        {
          invalide = 1;
        }
      }
    }

    return invalide;
}

void remplirTabRest(cell tab[10][10], bool restriction[8], playerMove *move,int i,int j){
  switch (tab[i][j].pion) {
    case 1:
      if(i-1 != 0)
          restriction[1] = tab[i-1][j].pion > 0 && tab[i-2][j].pion == 0;
      if(j-1 != 0)
          restriction[3] = tab[i][j-1].pion > 0 && tab[i][j-2].pion == 0;
      if(j+1 != 9)
          restriction[4] = tab[i][j+1].pion > 0 && tab[i][j+2].pion == 0;
      if(i+1 != 9)
          restriction[6] = tab[i+1][j].pion > 0 && tab[i+2][j].pion == 0;
    break;

    case 2:
    switch (move->player) {
      case 1:
        if(i-1 != 0  && j-1 != 0)
          restriction[0] = tab[i-1][j-1].pion > 0 && tab[i-2][j-2].pion == 0;
        if(i-1 != 0 && j+1 != 9)
          restriction[2] = tab[i-1][j+1].pion > 0 && tab[i-2][j+2].pion == 0;
        if(i+1 != 9)
          restriction[6] = tab[i+1][j].pion > 0 && tab[i+2][j].pion == 0;
      break;

      case 2:
        if(i-1 != 0)
          restriction[1] = tab[i-1][j].pion > 0 && tab[i-2][j].pion == 0;
        if(i+1 != 9 && j-1 != 0)
          restriction[5] = tab[i+1][j-1].pion > 0 && tab[i-2][j+2].pion == 0;
        if(i+1 != 9 && j+1 != 9)
          restriction[7] = tab[i+1][j+1].pion > 0 && tab[i+2][j+2].pion  == 0;
      break;

      case 3:
        if(i-1 != 0 && j+1 != 9)
          restriction[2] = tab[i-1][j+1].pion > 0 && tab[i-2][j+2].pion == 0;
        if(j-1 != 0)
          restriction[3] = tab[i][j-1].pion > 0 && tab[i][j-2].pion == 0;
        if(i+1 != 9 && j+1 != 9)
          restriction[7] = tab[i+1][j+1].pion > 0 && tab[i+2][j+2].pion  == 0;
      break;

      case 4:
        if(i-1 != 0  && j-1 != 0)
          restriction[0] = tab[i-1][j-1].pion > 0 && tab[i-2][j-2].pion == 0;
        if(j+1 != 9)
          restriction[4] = tab[i][j+1].pion > 0 && tab[i][j+2].pion == 0;
        if(i+1 != 9 && j-1 != 0)
          restriction[5] = tab[i+1][j-1].pion > 0 && tab[i-2][j+2].pion == 0;
      break;
    }

    break;

    case 3:
      if(i-1 != 0 && j-1 != 0)
        restriction[0] = tab[i-1][j-1].pion > 0 && tab[i-2][j-2].pion == 0;
      if(i-1 != 0 && j+1 != 9)
        restriction[2] = tab[i-1][j+1].pion > 0 && tab[i-2][j+2].pion == 0;
      if(i+1 != 9 && j-1 != 0)
        restriction[5] = tab[i+1][j-1].pion > 0 && tab[i-2][j+2].pion == 0;
      if(i+1 != 9 && j+1 != 9)
        restriction[7] = tab[i+1][j+1].pion > 0 && tab[i+2][j+2].pion == 0;
    break;

    case 4:
      if(i-1 != 0  && j-1 != 0)
        restriction[0] = tab[i-1][j-1].pion > 0 && tab[i-2][j-2].pion == 0;
      if(i-1 != 0)
        restriction[1] = tab[i-1][j].pion > 0 && tab[i-2][j].pion == 0;
      if(i-1 != 0 && j+1 != 9)
        restriction[2] = tab[i-1][j+1].pion > 0 && tab[i-2][j+2].pion == 0;
      if(j-1 != 0)
        restriction[3] = tab[i][j-1].pion > 0 && tab[i][j-2].pion == 0;
      if(j+1 != 9)
        restriction[4] = tab[i][j+1].pion > 0 && tab[i][j+2].pion == 0;
      if(i+1 != 9 && j-1 != 0)
        restriction[5] = tab[i+1][j-1].pion > 0 && tab[i-2][j+2].pion == 0;
      if(i+1 != 9)
        restriction[6] = tab[i+1][j].pion > 0 && tab[i+2][j].pion == 0;
      if(i+1 != 9 && j+1 != 9)
        restriction[7] = tab[i+1][j+1].pion > 0 && tab[i+2][j+2].pion == 0;
        break;
  }
}

bool autorisation_bord(cell tab[10][10], playerMove *move, coordInt depart, coordInt arrive, bool first_recusive){
/* Fonction permettant la vérification du fait que le jour puisse quitter le bord après son coup pendant le même tour
  Cette fonction fonctionne récursivement, c'est à dire qu'elle va s'appeller elle même pour vérifié qu'il existe un
  mouvement qui permette de faire quitter le pion du bord.
*/


  // On crée un ensemble de case possible à emprunter par le pion sélectionner
  int ensemblePossible[8] = {-1};

  // On crée un tableau de restrictions temporaires pour vérifier ensuite la possibilité ou non d'un mouvement
  bool restriction_temp[8];
  initialisationTabRest(restriction_temp, false);

  // En fonction du joueur le bord est différent de même pour les case possible de jouer sur un bord
  if(move->player == 1 || move->player == 2){
    if(arrive.y == 0){
      ensemblePossible[0] = 1; ensemblePossible[1] = 2; ensemblePossible[2] = 4; ensemblePossible[3] = 6; ensemblePossible[4] = 7;
    } else if(arrive.y == 9){
      ensemblePossible[0] = 0; ensemblePossible[1] = 1; ensemblePossible[2] = 3; ensemblePossible[3] = 5; ensemblePossible[4] = 6;
    }
  } else if (move->player == 3 || move->player == 4){
    if(arrive.x == 0){
      ensemblePossible[0] = 3; ensemblePossible[1] = 4; ensemblePossible[2] = 5; ensemblePossible[3] = 6; ensemblePossible[4] = 7;
    } else if(arrive.x == 9){
      ensemblePossible[0] = 0; ensemblePossible[1] = 1; ensemblePossible[2] = 2; ensemblePossible[3] = 3; ensemblePossible[4] = 4;
    }
  }

  // Ici on vérifie que le joueur est sur un bord
    if(ensemblePossible[0] != -1){
      // S'il est sur un bord on vérifie qu'il y'est au moins une possibilité lui permettant de quitter le bord
      for (int i = 0; i < 5; i++) {
        coordInt case_autour = elementCorrespondantInverse(ensemblePossible[i]);

        // On crée un tableau temporaire permettant d'anticipé les futurs coups
        cell tab_temp[10][10];
        for (int j = 0; j < 10; j++) {
          for (int k = 0; k < 10; k++) {
            tab_temp[j][k] = tab[j][k];
          }
        }

        // On fait un déplacement du pion dans le tableau temporaire pour prévoir le coup possible ensuite
        tab_temp[arrive.x][arrive.y].joueur = tab_temp[depart.x][depart.y].joueur;
        tab_temp[arrive.x][arrive.y].pion = tab_temp[depart.x][depart.y].pion;

        /* Si on est dans la première récursivité on enlève le pion, sinon on le fait pas
          cela permettera de ne pas être dans le cas où la pièce est bloquée dans le bord, cela empêche en fait
          de faire le même coup mais dans le sens inverse et donc d'entré dans une boucle infinie.
        */
        if(first_recusive){
          tab_temp[depart.x][depart.y].joueur = 0;
          tab_temp[depart.x][depart.y].pion = 0;
        }

        // On crée les coordonnées d'arrivée temporaire pour vérifié qu'il existe au moins une façon de quitter le bord
        coordInt arrive_temp;
        arrive_temp.x = depart.x + case_autour.x;
        arrive_temp.y = depart.y + case_autour.y;

        // On remplir les restrictions correspond au coup anticipé
        remplirTabRest(tab_temp,restriction_temp,move,arrive.x,arrive.y);

        // si toutes les conditions de règles sont vérifiés, on peut alors accomplir récursive la condition
        if(autorisation(arrive.x,arrive.y,arrive_temp.x,arrive_temp.y, tab) == 0 && restriction_temp[ensemblePossible[i]])
          return autorisation_bord(tab,move,arrive,arrive_temp,false);
      }
    } else {
      return  true;
    }
  return false;
}

void TourJoueurs(cell tab[10][10],game param_partie, float x, float y, SDL_Rect plateau, coordInt *selectedBox, bool restriction[8],playerMove *move){

  // Création des coordonées de la case sélectionnée
  coordInt coord_temp;

  // Si le joueur n'a pas encore désigné son pion de départ
  if(!move->selected){
    // Sélection du pion temporaire de départ
      coord_temp = selectionPion(tab,x,y,plateau);
      // Si cela correspond à un des ses pions
      if (tab[coord_temp.x][coord_temp.y].joueur == move->player && tab[coord_temp.x][coord_temp.y].pion != 0) {
        move->selected = true; // Il a alors sélectionné son pion
        *selectedBox = coord_temp; // On valide que la case appuyée correspond à un pion pour pouvoir le déplacer plus tard
        move->inTurn = true; // Le joueur est alors en tour tant le code sera répété tant qu'il n'a pas finis
        move->firstMove = false; // Le joueur n'a pas encore fait son premier déplacement et ne peut donc pas finir son tour
      }
  } else {
    // Sélection du pion d'arrivée

    // Coordonnées temporaires de la position du pion d'arrivé en fonction du pion de départ
    int i_temp = 0;
    int j_temp = 0;

    // Sélection de la case temporaire d'arrivé
    coord_temp = selectionMove(tab,x,y,plateau);

    // Si il est valide
    if(coord_temp.x >= 0 && coord_temp.y >= 0){

      /* Calcul de la position du pion d'arrivé en fonction d'un autre
        (-1,1) ou (1,1) ou (-1,0) etc...
        Cela permettera de savoir si le saut est possible par la suite.
        Le code fonctionne très bien si l'utilisateur veut sauter ou pas
        car il fait la partie entière de sa division par 2.
      */
      float distance_i_temp = fabs((coord_temp.x - selectedBox->x)/2.0);
      float distance_j_temp = fabs((coord_temp.y - selectedBox->y)/2.0);
      int signe_i_temp = (coord_temp.x - selectedBox->x)/(2.0 * distance_i_temp);
      int signe_j_temp = (coord_temp.y - selectedBox->y)/(2.0 * distance_j_temp);

      if(distance_i_temp <= 1)
        i_temp = ceil(distance_i_temp) * signe_i_temp;
      else
        i_temp = floor(distance_i_temp) * signe_i_temp;

      if(distance_j_temp <= 1)
        j_temp = ceil(distance_j_temp) * signe_j_temp;
      else
        j_temp = floor(distance_j_temp) * signe_j_temp;

        // Si la coordonnée temporaire repecte les règles de jeux et qu'il respecte les restrictions apportées par son saut (elles n'existent pas si il n'a pas encore joué (voir init du tab))
      if(autorisation(selectedBox->x,selectedBox->y,coord_temp.x,coord_temp.y,tab)==0 && restriction[elementCorrespondant(i_temp,j_temp)] && autorisation_bord(tab,move,*selectedBox,coord_temp,true)){

        // Le mouvement est donc validé et le pion se déplace
        tab[coord_temp.x][coord_temp.y].joueur = tab[selectedBox->x][selectedBox->y].joueur;
        tab[coord_temp.x][coord_temp.y].pion = tab[selectedBox->x][selectedBox->y].pion;
        tab[selectedBox->x][selectedBox->y].joueur = 0;
        tab[selectedBox->x][selectedBox->y].pion = 0;

        // La case sélectionné prend alors la valeur de la case temporaire pour pouvoir faire des sauts et recommencer se code avec cette nouvelle case
        *selectedBox = coord_temp;

        // Le joueur a fait son premier coup ou la déjà fait, il peut alors finir son tour ou pas si les restrictions l'autorise
        move->firstMove = true;

        if(((move->player == 1 || move->player == 2) && (selectedBox->y == 0 || selectedBox->y == 9)) || ((move->player == 3 || move->player == 4) && (selectedBox->x == 0 || selectedBox->x == 9)))
          move->inBorder = true;
        else
          move->inBorder = false;

        // Création des restrictions en fonction de chaque joueur et chaque pion
        initialisationTabRest(restriction,false);

        // Conditions correspondants aux 8 cases qui entourent le pion sélectionné
        remplirTabRest(tab,restriction,move,selectedBox->x,selectedBox->y);

      // On regarde si le joueur peut encore jouer
      move->inTurn = false;
      for (int i = 0; i < 8; i++) {
        // Si il existe une seule restriction possible le joueur peut jouer
        if (restriction[i])
          move->inTurn = true;
      }
    }
  }

  // Si le joueur n'a pas cliqué sur un case valable du tableau, vérifie qu'il veuille re-sélectionner un nouveau pion
  coord_temp = selectionPion(tab,x,y,plateau);
  // Le code est similaire à la sélection
      if (tab[coord_temp.x][coord_temp.y].joueur == move->player && move->firstMove == false)
        *selectedBox = coord_temp;


     // Si le tour d'un joueur est finir alors on passe au joueur suivant et on remet les valeurs à zéro
     if(!move->inTurn) {
      move->player = move->player%(param_partie->joueurs + param_partie->ordis) + 1;
      move->selected = false;
      selectedBox->x = -1;
      selectedBox->y = -1;
      move->inTurn = false;
      initialisationTabRest(restriction,true);
    }
 }

 return;
}
