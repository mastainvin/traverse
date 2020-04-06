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
    tab[i][0].pion = (temp%4) + 1;
    tab[i][0].joueur = 2;
    tab[i][9].pion = (temp%4) + 1;
    tab[i][9].joueur = 1;
    temp = temp + 1;
  }
  temp = 4;
  for (int i = 5; i <= 8; i++) {
    tab[i][0].pion = temp;
    tab[i][0].joueur = 2;
    tab[i][9].pion = temp;
    tab[i][9].joueur = 1;
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

void initialisationTabRest(bool tab[8]){
  for (int i = 0; i < 8; i++) {
    tab[i] = true;
  }
}

void initilisationPlayerMove(playerMove *move){
  move->player = 1;
  move->selected = false;
  move->inTurn = false;
  move->firstMove = false;
}

int elementCorrespondant(bool tab[8], int i, int j){
  if (i == -1 && j == -1)
    return 0;
  else if (i == 0 && j == -1)
    return 1;
  else if (i == 1 && j == -1)
    return 2;
  else if (i == -1 && j == 0)
    return 3;
  else if (i == 1 && j == 0)
    return 4;
  else if (i == -1 && j == 1)
    return 5;
  else if (i == 0 && j == 1)
    return 6;
  else if (i == 1 && j == 1)
    return 7;
  else
    return -1;
}

void remplirRestriction(bool tab[8],bool un, bool deux, bool trois, bool quatre, bool cinq, bool six, bool sept, bool huit){
  tab[0]= un;
  tab[1]= deux;
  tab[2]= trois;
  tab[3]= quatre;
  tab[4]= cinq;
  tab[5]= six;
  tab[6]= sept;
  tab[7]= huit;
}
// i, j, k et l sont les coordonnées de la pièce au début et après le tour.
int autorisation(int i, int j, int k, int l, cell tab[10][10]){
    int invalide = 0, invalide2 = 0;
    int saut1 = 0, saut2 = 0, saut3 = 0, saut4 = 0, saut5 = 0, saut6 = 0, saut7 = 0, saut8 = 0;
    //cas pour le carre
    if( tab[i][j].pion == 1)
    {   //Lorsque le pion veut effectuer un déplacement simple
        if((i == k && j == l - 1) || (i == k && j == l + 1) || (i == k + 1 && j == l) || (i == k - 1 && j == l))
        {
          invalide = 0;
        } //Lorsque le pion veut effectuer un saut
        else if(tab[i][j+1].pion && ((tab[k][l].pion == tab[i][j+2].pion) ||(i == k && j == l - 1) || (i == k && j == l + 1) || (i == k + 1 && j == l) || (i == k - 1 && j == l)))
        {
          invalide = 0;
        }
        else if(tab[i][j-1].pion && ((tab[k][l].pion == tab[i][j-2].pion) ||(i == k && j == l - 1) || (i == k && j == l + 1) || (i == k + 1 && j == l) || (i == k - 1 && j == l)))
        {
          invalide = 0;
        }
        else if(tab[i-1][j].pion && ((tab[k][l].pion == tab[i-2][j].pion) ||(i == k && j == l - 1) || (i == k && j == l + 1) || (i == k + 1 && j == l) || (i == k - 1 && j == l)))
        {
          invalide = 0;
        }
        else if(tab[i+1][j].pion && ((tab[k][l].pion == tab[i+2][j].pion) ||(i == k && j == l - 1) || (i == k && j == l + 1) || (i == k + 1 && j == l) || (i == k - 1 && j == l)))
        {
          invalide = 0;
        } //Si le déplacement voulu est interdit
        else
        {
          invalide = 1;
        }
    }
    //cas pour le losange
    if( tab[i][j].pion == 3)
    {   //Lorsque le pion effectue un déplacement simple
        if(((tab[i-1][j-1].pion == 0) && (k == i - 2 && l == j - 2)) || ((tab[i-1][j+1].pion == 0) && (k == i - 2 && l == j + 2)) || ((tab[i+1][j-1].pion == 0) && (k == i + 2 && l == j - 2)) ||((tab[i+1][j+1].pion == 0) && (k == i + 2 && l == j + 2)))
        {
          invalide = 1;
        } //Lorsque le pion effectue un saut
        else if(!((i == k && j == l - 1) || (i == k && j == l + 1) || (i == k + 1 && j == l) || (i == k - 1 && j == l)))
        {
          invalide = 0;
        } //Lorsque le déplacement voulu est interdit
        else
        {
          invalide = 1;
        }
    }
    //cas pour le rond
    if( tab[i][j].pion == 4)
    { //Si le rond veut se déplacer comme un carré
      if(i == k || j == l)
      { //Lorsque le pion veut effectuer un déplacement simple
        if((i == k && j == l - 1) || (i == k && j == l - 1) || (i == k + 1 && j == l) || (i == k - 1 && j == l))
        {
          invalide = 0;
        } //Lorsque le pion veut effectuer un saut
        else if(tab[i][j+1].pion && ((tab[k][l].pion == tab[i][j+2].pion) ||(i == k && j == l - 1) || (i == k && j == l + 1) || (i == k + 1 && j == l) || (i == k - 1 && j == l)))
        {
          invalide = 0;
        }
        else if(tab[i][j-1].pion && ((tab[k][l].pion == tab[i][j-2].pion) ||(i == k && j == l - 1) || (i == k && j == l + 1) || (i == k + 1 && j == l) || (i == k - 1 && j == l)))
        {
          invalide = 0;
        }
        else if(tab[i-1][j].pion && ((tab[k][l].pion == tab[i-2][j].pion) ||(i == k && j == l - 1) || (i == k && j == l + 1) || (i == k + 1 && j == l) || (i == k - 1 && j == l)))
        {
          invalide = 0;
        }
        else if(tab[i+1][j].pion && ((tab[k][l].pion == tab[i+2][j].pion) ||(i == k && j == l - 1) || (i == k && j == l + 1) || (i == k + 1 && j == l) || (i == k - 1 && j == l)))
        {
          invalide = 0;
        } //Lorsque le déplacement est interdit
        else
        {
          invalide = 1;
        }
      } // Si le rond veut se déplacer comme un losange
      else
      {  //Lorsque le pion veut effectuer un déplacement simple
        if(((tab[i-1][j-1].pion == 0) && (k == i - 2 && l == j - 2)) || ((tab[i-1][j+1].pion == 0) && (k == i - 2 && l == j + 2)) || ((tab[i+1][j-1].pion == 0) && (k == i + 2 && l == j - 2)) ||((tab[i+1][j+1].pion == 0) && (k == i + 2 && l == j + 2)))
        {
          invalide = 1;
        } //Lorsque le pion veut effectuer un saut
        else if(!((i == k && j == l - 1) || (i == k && j == l + 1) || (i == k + 1 && j == l) || (i == k - 1 && j == l)))
        {
          invalide = 0;
        } //Lorsque le mouvement voule est interdit
        else
        {
          invalide = 1;
        }
      }
    }
    //cas pour le triangle
    if(tab[i][j].pion == 2)
    {  //Si le triangle appartient au joueur 1
      if(tab[i][j].joueur == 1)
      { //Si il veut se déplacer verticalement
        if(j == l)
        {
          if( i == k - 1 || ((tab[i+1][j].pion > 0) && (tab[k][l].pion == tab[i+2][j].pion )))
          {
            invalide = 0;
          } //Si le déplacement en "ligne" est interdit
          if((i == 9 && j == 7) && (k == 8 && l == 7))
          {
            invalide = 1;
          }
          else
          {
            invalide = 1;
          }
        } // Si le déplacement voulu est en diagonale
        else
        {
          if((i == k + 1 && j == l - 1) || (i == k + 1 && j == l + 1) || (tab[i-1][j-1].pion > 0 && tab[k][l].pion == tab[i-2][j-2].pion) || (tab[i-1][j+1].pion && tab[k][l].pion == tab[i-2][j+2].pion))
          {
            invalide = 0;
          } //Si le déplacement en diagonale est interdit
          else
          {
            invalide = 1;
          }
        }
      } //Si le triangle appartient au joueur 2
      if(tab[i][j].joueur == 2)
      {
        if(j == l)
        {
          if((i == k + 1) || ((tab[i-1][j].pion > 0 ) && (tab[k][l].pion  == tab[i-2][j].pion )))
          {
            invalide = 0;
          }
          if((i == 0 && j == 2) && (k == 1 && l == 2))
          {
            invalide = 1;
          }
          else
          {
            invalide = 1;
          }
        } // Si le déplacement voulu est en diagonale
        else
        {
          if((i == k - 1 && j == l - 1) || (i == k - 1 && j == l + 1) || (tab[i+1][j-1].pion > 0 && tab[k][l].pion == tab[i+2][j-2].pion) || (tab[i+1][j+1].pion && tab[k][l].pion == tab[i+2][j+2].pion))
          {
            invalide = 0;
          } //Si le déplacement en diagonale est interdit
          else
          {
            invalide = 1;
          }
        }
      } //Si le triangle appartient au joueur 3
      if(tab[i][j].joueur == 3)
      {
        if(i == k)
        { //Si le déplacement voulu est horizontal
          if((j == l + 1) || ((tab[i][j-1].pion > 0 ) && (tab[k][l].pion  == tab[i][j-2].pion )))
          {
            invalide = 0;
          } //Si le déplacment en "ligne" est interdit
          else
          {
            invalide = 1;
          }
        } // Si le déplacement voulu est en diagonale
        else
        {
          if((i == k - 1 && j == l - 1) || (i == k + 1 && j == l - 1) || (tab[i+1][j+1].pion > 0 && tab[k][l].pion == tab[i+2][j+2].pion) || (tab[i-1][j+1].pion && tab[k][l].pion == tab[i-2][j+2].pion))
          {
            invalide = 0;
          } //Si le déplacement en diagonale est interdit
          else
          {
            invalide = 1;
          }
        }
      } //Si le triangle appartient au joueur 4
      if(tab[i][j].joueur == 4)
      { //Si le déplacement voulu est horizontal
        if(i == k)
        {
          if( (j == l - 1) || ((tab[i][j+1].pion > 0 ) && (tab[k][l].pion  == tab[i][j+2].pion )))
          {
            invalide = 0;
          } //Si le déplacment en "ligne" est interdit
          else
          {
            invalide = 1;
          }
        } // Si le déplacement voulu est en diagonale
        else
        {
          if((i == k - 1 && j == l + 1) || (i == k + 1 && j == l + 1) || (tab[i+1][j-1].pion > 0 && tab[k][l].pion == tab[i+2][j-2].pion) || (tab[i-1][j-1].pion && tab[k][l].pion == tab[i-2][j-2].pion))
          {
            invalide = 0;
          }  //Si le déplacement en diagonale est interdit
          else
          {
            invalide = 1;
          }
        }
      }
    }
    if( (invalide > 0) || (invalide2 > 0))
    {
      invalide = 1;
    }
     else
     {
       invalide = 0;
       invalide2 = 0;
     }
    return invalide;
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

    // Sélection du pion temporaire d'arrivé
    coord_temp = selectionMove(tab,x,y,plateau);

    // Si il est valide
    if(coord_temp.x >= 0 && coord_temp.y >= 0){

      /* Calcul de la position du pion d'arrivé en fonction d'un autre
        (-1,1) ou (1,1) ou (-1,0) etc...
        Cela permettera de savoir si le saut est possible par la suite.
        Le code fonctionne très bien si l'utilisateur veut sauter ou pas
        car il fait la partie entière de sa division par 2.
      */

      if((selectedBox->x - coord_temp.x)/2 >=0)
        i_temp = -ceil(fabs((selectedBox->x - coord_temp.x)/2.0));
      else
        i_temp = ceil(fabs((selectedBox->x - coord_temp.x)/2.0));

      if((selectedBox->y - coord_temp.y)/2 >= 0)
        j_temp = -ceil(fabs((selectedBox->y - coord_temp.y)/2.0));
      else
        j_temp = ceil(fabs((selectedBox->y - coord_temp.y)/2.0));

        // Si la coordonnée temporaire repecte les règles de jeux et qu'il respecte les restrictions apportées par son saut (elles n'existent pas si il n'a pas encore joué (voir init du tab))
      if(autorisation(selectedBox->x,selectedBox->y,coord_temp.x,coord_temp.y,tab)==0 && restriction[elementCorrespondant(restriction,i_temp,j_temp)]){

        // Le mouvement est donc validé et le pion se déplace
        tab[coord_temp.x][coord_temp.y].joueur = tab[selectedBox->x][selectedBox->y].joueur;
        tab[coord_temp.x][coord_temp.y].pion = tab[selectedBox->x][selectedBox->y].pion;
        tab[selectedBox->x][selectedBox->y].joueur = 0;
        tab[selectedBox->x][selectedBox->y].pion = 0;

        // La case sélectionné prend alors la valeur de la case temporaire pour pouvoir faire des sauts et recommencer se code avec cette nouvelle case
        *selectedBox = coord_temp;

        // Le joueur a fait son premier coup ou la déjà, il peut alors finir son tour
        move->firstMove= true;

        // Création des restrictions en fonction de chaque joueur et chaque pion
        switch (move->player) {
          //Tour joueur 1 -----------------------------------------------------------------------------------------------------------------------------------------
          case 1:

            switch (tab[selectedBox->x][selectedBox->y].pion) {
              case 1:
                  // En fonction des règles on regarde si le saut est possible, il ne peut donc pas faire de coup normal à la place d'un saut
                  remplirRestriction(restriction,false,tab[selectedBox->x][selectedBox->y-1].pion > 0,tab[selectedBox->x-1][selectedBox->y].pion > 0,tab[selectedBox->x+1][selectedBox->y].pion > 0,false,false,tab[selectedBox->x][selectedBox->y+1].pion > 0,false);
              break;

              case 2:
                  remplirRestriction(restriction,tab[selectedBox->x-1][selectedBox->y-1].pion > 0,false,false,false,tab[selectedBox->x+1][selectedBox->y].pion > 0,tab[selectedBox->x-1][selectedBox->y+1].pion > 0,false,false);
              break;

              case 3:
                  remplirRestriction(restriction,tab[selectedBox->x-1][selectedBox->y-1].pion > 0,false,tab[selectedBox->x+1][selectedBox->y-1].pion > 0 ,false,false,tab[selectedBox->x-1][selectedBox->y+1].pion > 0,false,tab[selectedBox->x+1][selectedBox->y+1].pion > 0);
              break;

              case 4:
                  remplirRestriction(restriction,tab[selectedBox->x-1][selectedBox->y-1].pion > 0,tab[selectedBox->x][selectedBox->y-1].pion > 0,tab[selectedBox->x+1][selectedBox->y-1].pion > 0,tab[selectedBox->x-1][selectedBox->y].pion > 0,tab[selectedBox->x+1][selectedBox->y].pion > 0,tab[selectedBox->x-1][selectedBox->y+1].pion > 0,tab[selectedBox->x][selectedBox->y+1].pion > 0,tab[selectedBox->x+1][selectedBox->y+1].pion > 0);
              break;
              }

          break;
          //Tour joueur 2 -----------------------------------------------------------------------------------------------------------------------------------------
          case 2:
              switch (tab[selectedBox->x][selectedBox->y].pion) {
                case 1:
                  remplirRestriction(restriction,false,tab[selectedBox->x-1][selectedBox->y].pion > 0,tab[selectedBox->x][selectedBox->y-1].pion > 0,false,tab[selectedBox->x+1][selectedBox->y].pion > 0,false,tab[selectedBox->x][selectedBox->y+1].pion > 0,false);
                break;

                case 2:
                  remplirRestriction(restriction,false,false,tab[selectedBox->x+1][selectedBox->y-1].pion > 0,false,false,false,tab[selectedBox->x-1][selectedBox->y].pion > 0,tab[selectedBox->x+1][selectedBox->y+1].pion > 0 );
                break;

                case 3:
                  remplirRestriction(restriction,tab[selectedBox->x-1][selectedBox->y-1].pion > 0,false,tab[selectedBox->x+1][selectedBox->y-1].pion > 0,false,false,tab[selectedBox->x-1][selectedBox->y+1].pion > 0,false,tab[selectedBox->x+1][selectedBox->y+1].pion > 0);
                break;

                case 4:
                  remplirRestriction(restriction,tab[selectedBox->x-1][selectedBox->y-1].pion > 0,tab[selectedBox->x][selectedBox->y-1].pion > 0,tab[selectedBox->x+1][selectedBox->y-1].pion > 0,tab[selectedBox->x-1][selectedBox->y].pion > 0,tab[selectedBox->x+1][selectedBox->y].pion > 0,tab[selectedBox->x-1][selectedBox->y+1].pion > 0,tab[selectedBox->x][selectedBox->y+1].pion > 0,tab[selectedBox->x+1][selectedBox->y+1].pion > 0);
                break;

                  }
          break;

          //Tour joueur 3 -----------------------------------------------------------------------------------------------------------------------------------------
          case 3:
          break;

          //Tour joueur 4 -----------------------------------------------------------------------------------------------------------------------------------------
          case 4:
          break;
        }
          // On regarde si le joueur peut encore jouer
          move->inTurn = false;
          for (int i = 0; i < 8; i++) {
            // Si il existe une seule restriction possible le joueur peut jouer
            if (restriction[i] == true)
              move->inTurn = true;
          }
      }
    } else {
      // Si le joueur n'a pas cliqué sur un case valable du tableau, vérifie qu'il veuille re-sélection un nouveau pion
      coord_temp = selectionPion(tab,x,y,plateau);
      // Le code est similaire à la sélection
          if (tab[coord_temp.x][coord_temp.y].joueur == move->player && tab[coord_temp.x][coord_temp.y].pion != 0 && move->firstMove == false) {
            *selectedBox = coord_temp;
          }
        }
      // Si le tour d'un joueur est finir alors on passe au joueur suivant et on remet les valeurs à zéro
     if(!move->inTurn) {
      move->player = move->player%(param_partie->joueurs + param_partie->ordis) + 1;
      move->selected = false;
      selectedBox->x = -1;
      selectedBox->y = -1;
      move->inTurn = false;
      initialisationTabRest(restriction);
    }
 }

 return;
}
