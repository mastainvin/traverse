#include "FonctionsDuJeu.h"
#include "interface.h"
#include "IA.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>

void copierTab(cell tab[10][10], cell newTab[10][10])
{
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      newTab[i][j].joueur = tab[i][j].joueur;
      newTab[i][j].pion = tab[i][j].pion;
      newTab[i][j].distinction = tab[i][j].distinction;
    }
  }
}

void copierTabRest(bool tab[8], bool newTab[8])
{
  for (int i = 0; i < 8; i++)
  {
    newTab[i] = tab[i];
  }
}
void initialisationCaseVisit(list_pion_visit tab[10][10][PION_LIST_LENGTH])
{

  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      for (int k = 0; k < PION_LIST_LENGTH; k++)
      {
        tab[i][j][k].joueur = k / 8 + 1;
        tab[i][j][k].pion = k % 4 + 1;
        tab[i][j][k].distinction = (k / 4) % 2 + 1;
        tab[i][j][k].compteur = 0;
      }
    }
  }
}
//initialisons le tableau (chaque case vide repésentée par un 0)
void initialisationTab(cell tab[10][10], int nbrJoueurs)
{

  int temp = 0;
  for (int i = 0; i <= 9; i++)
  {
    for (int j = 0; j <= 9; j++)
    {
      tab[i][j].joueur = 0;
      tab[i][j].pion = 0;
      tab[i][j].distinction = 0;
    }
  }
  //Placons les pions avec le carré = 1, le triangle = 2, le losange = 3, le rond = 4.
  //En fonction du nombre de joueurs.Ici 2 joueurs
  for (int i = 1; i <= 4; i++)
  {
    tab[0][i].pion = (temp % 4) + 1;
    tab[0][i].joueur = 2;
    tab[0][i].distinction = 1;
    tab[9][i].pion = (temp % 4) + 1;
    tab[9][i].joueur = 1;
    tab[9][i].distinction = 1;
    temp = temp + 1;
  }
  temp = 4;
  for (int i = 5; i <= 8; i++)
  {
    tab[0][i].pion = temp;
    tab[0][i].joueur = 2;
    tab[0][i].distinction = 2;
    tab[9][i].pion = temp;
    tab[9][i].joueur = 1;
    tab[9][i].distinction = 2;
    temp = temp - 1;
  }
  temp = 0;

  //Ici 3 joueurs
  if (nbrJoueurs == 3 || nbrJoueurs == 4)
  {
    for (int i = 1; i <= 4; i++)
    {
      tab[i][0].pion = (temp % 4) + 1;
      tab[i][0].joueur = 3;
      tab[i][0].distinction = 1;
      temp = temp + 1;
    }
    temp = 4;
    for (int i = 5; i <= 8; i++)
    {
      tab[i][0].pion = temp;
      tab[i][0].joueur = 3;
      tab[i][0].distinction = 2;
      temp = temp - 1;
    }

    temp = 0;

    //Ici 4 joueurs
    if (nbrJoueurs == 4)
    {
      for (int i = 1; i <= 4; i++)
      {
        temp = temp + 1;
        tab[i][9].pion = temp;
        tab[i][9].joueur = 4;
        tab[i][9].distinction = 1;
      }
      temp = 4;
      for (int i = 5; i <= 8; i++)
      {
        tab[i][9].pion = temp;
        tab[i][9].joueur = 4;
        tab[i][9].distinction = 2;
        temp = temp - 1;
      }
    } // fin if (nbrJoueurs == 4)
  }   // fin if (nbrJoueurs == 3 || nbrJoueurs == 4)}
}

void initialisationTabRest(bool tab[8], bool valeur)
{
  for (int i = 0; i < 8; i++)
  {
    tab[i] = valeur;
  }
}

void initilisationPlayerMove(playerMove *move)
{
  move->player = 1;
  move->selected = false;
  move->inTurn = false;
  move->firstMove = false;
}
void initialisationAlive(bool tab[4], int nombre_joueur)
{
  for (int i = 0; i < 4; i++)
  {
    tab[i] = false;
  }

  for (int i = 0; i < nombre_joueur; i++)
  {
    tab[i] = true;
  }
}

coordInt selectionPion(cell tab[10][10], float x, float y, SDL_Rect plateau)
{
  /* Fonction qui permet de sélectionne un pion sur le plateau*/
  coordInt selectedPion;
  selectedPion.x = -1;
  selectedPion.y = -1;

  if (x > plateau.x && x < plateau.x + plateau.w && y > plateau.y && y < plateau.x + plateau.h)
  {
    int *i, *j;
    i = malloc(sizeof(int));
    j = malloc(sizeof(int));

    *j = floor((x - plateau.x) / 50);
    *i = floor((y - plateau.y) / 50);

    if (*i >= 0 && *i <= 9 && *j >= 0 && *i <= 9 && tab[*i][*j].joueur != 0)
    {
      selectedPion.x = *i;
      selectedPion.y = *j;
    }
    free(i);
    free(j);
  }
  return selectedPion;
}

coordInt selectionMove(cell tab[10][10], float x, float y, SDL_Rect plateau)
{
  /* Fonction qui permet de sélectionner une case d'arriver sur le plateau*/
  coordInt selectedPion;
  selectedPion.x = -1;
  selectedPion.y = -1;

  if (x > plateau.x && x < plateau.x + plateau.w && y > plateau.y && y < plateau.x + plateau.h)
  {
    int i, j;
    j = floor((x - plateau.x) / 50);
    i = floor((y - plateau.y) / 50);
    if (i >= 0 && i <= 9 && j >= 0 && i <= 9 && tab[i][j].joueur == 0)
    {
      selectedPion.x = i;
      selectedPion.y = j;
    }
  }
  return selectedPion;
}

int elementCorrespondant(int i, int j)
{
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

coordInt remplirCoordInt(int i, int j)
{
  coordInt valeur;
  valeur.x = i;
  valeur.y = j;

  return valeur;
}

coordInt elementCorrespondantInverse(int i)
{
  if (i == 0)
    return remplirCoordInt(-1, -1);
  else if (i == 1)
    return remplirCoordInt(-1, 0);
  else if (i == 2)
    return remplirCoordInt(-1, 1);
  else if (i == 3)
    return remplirCoordInt(0, -1);
  else if (i == 4)
    return remplirCoordInt(0, 1);
  else if (i == 5)
    return remplirCoordInt(1, -1);
  else if (i == 6)
    return remplirCoordInt(1, 0);
  else if (i == 7)
    return remplirCoordInt(1, 1);
  else
    return remplirCoordInt(0, 0);
}
// i, j, k et l sont les coordonnées de la pièce au début et après le tour.
bool autorisation(int i, int j, int k, int l, cell tab[10][10])
{

  //cas pour le carre
  if (tab[i][j].pion == 1)
  {
    if (((i == k && j == l - 1) && (tab[i][j + 1].pion == 0)) || ((tab[i][j + 1].pion > 0) && (i == k && j == l - 2)))
    {
      return true;
    }
    else if (((i == k && j == l + 1) && (tab[i][j - 1].pion == 0)) || ((tab[i][j - 1].pion > 0) && (i == k && j == l + 2)))
    {
      return true;
    }
    else if (((i == k + 1 && j == l) && (tab[i - 1][j].pion == 0)) || ((tab[i - 1][j].pion > 0) && (i == k + 2 && j == l)))
    {
      return true;
    }
    else if (((i == k - 1 && j == l) && (tab[i + 1][j].pion == 0)) || ((tab[i + 1][j].pion > 0) && (i == k - 2 && j == l)))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  //cas pour le losange
  else if (tab[i][j].pion == 3)
  {
    if ((i == k + 1 && j == l + 1) || ((tab[i - 1][j - 1].pion > 0) && (i == k + 2 && j == l + 2)))
    {
      return true;
    }
    else if ((i == k + 1 && j == l - 1) || ((tab[i - 1][j + 1].pion > 0) && (i == k + 2 && j == l - 2)))
    {
      return true;
    }
    else if ((i == k - 1 && j == l + 1) || ((tab[i + 1][j - 1].pion > 0) && (i == k - 2 && j == l + 2)))
    {
      return true;
    }
    else if ((i == k - 1 && j == l - 1) || ((tab[i + 1][j + 1].pion > 0) && (i == k - 2 && j == l - 2)))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  //cas pour le rond
  else if (tab[i][j].pion == 4)
  { //Si le rond veut se déplacer comme un carré
    if (((i == k && j == l - 1) && (tab[i][j + 1].pion == 0)) || ((tab[i][j + 1].pion > 0) && (i == k && j == l - 2)))
    {
      return true;
    }
    else if (((i == k && j == l + 1) && (tab[i][j - 1].pion == 0)) || ((tab[i][j - 1].pion > 0) && (i == k && j == l + 2)))
    {
      return true;
    }
    else if (((i == k + 1 && j == l) && (tab[i - 1][j].pion == 0)) || ((tab[i - 1][j].pion > 0) && (i == k + 2 && j == l)))
    {
      return true;
    }
    else if (((i == k - 1 && j == l) && (tab[i + 1][j].pion == 0)) || ((tab[i + 1][j].pion > 0) && (i == k - 2 && j == l)))
    {
      return true;
    }
    // Si le rond veut se déplacer comme un losange
    else if ((i == k + 1 && j == l + 1) || ((tab[i - 1][j - 1].pion > 0) && (i == k + 2 && j == l + 2)))
    {
      return true;
    }
    else if ((i == k + 1 && j == l - 1) || ((tab[i - 1][j + 1].pion > 0) && (i == k + 2 && j == l - 2)))
    {
      return true;
    }
    else if ((i == k - 1 && j == l + 1) || ((tab[i + 1][j - 1].pion > 0) && (i == k - 2 && j == l + 2)))
    {
      return true;
    }
    else if ((i == k - 1 && j == l - 1) || ((tab[i + 1][j + 1].pion > 0) && (i == k - 2 && j == l - 2)))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  //cas pour le triangle
  else if (tab[i][j].pion == 2)
  {
    if (tab[i][j].joueur == 1)
    {
      if ((i == k - 1 && j == l) || ((tab[i + 1][j].pion > 0) && (i == k - 2 && j == l)))
      {
        return true;
      }
      else if ((i == k + 1 && j == l + 1) || ((tab[i - 1][j - 1].pion > 0) && (i == k + 2 && j == l + 2)))
      {
        return true;
      }
      else if ((i == k + 1 && j == l - 1) || ((tab[i - 1][j + 1].pion > 0) && (i == k + 2 && j == l - 2)))
      {
        return true;
      }
      else
      {
        return false;
      }
    } //Si le triangle appartient au joueur 2
    else if (tab[i][j].joueur == 2)
    {
      if ((i == k + 1 && j == l) || ((tab[i - 1][j].pion > 0) && (i == k + 2 && j == l)))
      {
        return true;
      }
      else if ((i == k - 1 && j == l + 1) || ((tab[i + 1][j - 1].pion > 0) && (i == k - 2 && j == l + 2)))
      {
        return true;
      }
      else if ((i == k - 1 && j == l - 1) || ((tab[i + 1][j + 1].pion > 0) && (i == k - 2 && j == l - 2)))
      {
        return true;
      }
      else
      {
        return false;
      }
    } //Si le triangle appartient au joueur 3
    else if (tab[i][j].joueur == 3)
    {
      if ((i == k && j == l + 1) || ((tab[i][j - 1].pion > 0) && (i == k && j == l + 2)))
      {
        return true;
      }
      else if ((i == k + 1 && j == l - 1) || ((tab[i - 1][j + 1].pion > 0) && (i == k + 2 && j == l - 2)))
      {
        return true;
      }
      else if ((i == k - 1 && j == l - 1) || ((tab[i + 1][j + 1].pion > 0) && (i == k - 2 && j == l - 2)))
      {
        return true;
      }
      else
      {
        return false;
      }
    } //Si le triangle appartient au joueur 4
    else if (tab[i][j].joueur == 4)
    {
      if ((i == k && j == l - 1) || ((tab[i][j + 1].pion > 0) && (i == k && j == l - 2)))
      {
        return true;
      }
      else if ((i == k + 1 && j == l + 1) || ((tab[i - 1][j - 1].pion > 0) && (i == k + 2 && j == l + 2)))
      {
        return true;
      }
      else if ((i == k - 1 && j == l + 1) || ((tab[i + 1][j - 1].pion > 0) && (i == k - 2 && j == l + 2)))
      {
        return true;
      }
      else
      {
        return false;
      }
    }
  }
  return false;
}

void remplirTabRest(cell tab[10][10], bool restriction[8], playerMove *move, int i, int j)
{
  /* Fonction qui remplis, en fonction du joueur est du pion les coups possible après le premier mouvement pendant le même tour */

  switch (tab[i][j].pion)
  {
  case 1:
    if (i - 1 != 0)
      restriction[1] = tab[i - 1][j].pion > 0 && tab[i - 2][j].pion == 0;
    if (j - 1 != 0)
      restriction[3] = tab[i][j - 1].pion > 0 && tab[i][j - 2].pion == 0;
    if (j + 1 != 9)
      restriction[4] = tab[i][j + 1].pion > 0 && tab[i][j + 2].pion == 0;
    if (i + 1 != 9)
      restriction[6] = tab[i + 1][j].pion > 0 && tab[i + 2][j].pion == 0;
    break;

  case 2:
    switch (move->player)
    {
    case 1:
      if (i - 1 != 0 && j - 1 != 0)
        restriction[0] = tab[i - 1][j - 1].pion > 0 && tab[i - 2][j - 2].pion == 0;
      if (i - 1 != 0 && j + 1 != 9)
        restriction[2] = tab[i - 1][j + 1].pion > 0 && tab[i - 2][j + 2].pion == 0;
      if (i + 1 != 9)
        restriction[6] = tab[i + 1][j].pion > 0 && tab[i + 2][j].pion == 0;
      break;

    case 2:
      if (i - 1 != 0)
        restriction[1] = tab[i - 1][j].pion > 0 && tab[i - 2][j].pion == 0;
      if (i + 1 != 9 && j - 1 != 0)
        restriction[5] = tab[i + 1][j - 1].pion > 0 && tab[i + 2][j - 2].pion == 0;
      if (i + 1 != 9 && j + 1 != 9)
        restriction[7] = tab[i + 1][j + 1].pion > 0 && tab[i + 2][j + 2].pion == 0;
      break;

    case 3:
      if (i - 1 != 0 && j + 1 != 9)
        restriction[2] = tab[i - 1][j + 1].pion > 0 && tab[i - 2][j + 2].pion == 0;
      if (j - 1 != 0)
        restriction[3] = tab[i][j - 1].pion > 0 && tab[i][j - 2].pion == 0;
      if (i + 1 != 9 && j + 1 != 9)
        restriction[7] = tab[i + 1][j + 1].pion > 0 && tab[i + 2][j + 2].pion == 0;
      break;

    case 4:
      if (i - 1 != 0 && j - 1 != 0)
        restriction[0] = tab[i - 1][j - 1].pion > 0 && tab[i - 2][j - 2].pion == 0;
      if (j + 1 != 9)
        restriction[4] = tab[i][j + 1].pion > 0 && tab[i][j + 2].pion == 0;
      if (i + 1 != 9 && j - 1 != 0)
        restriction[5] = tab[i + 1][j - 1].pion > 0 && tab[i + 2][j - 2].pion == 0;
      break;
    }

    break;

  case 3:
    if (i - 1 != 0 && j - 1 != 0)
      restriction[0] = tab[i - 1][j - 1].pion > 0 && tab[i - 2][j - 2].pion == 0;
    if (i - 1 != 0 && j + 1 != 9)
      restriction[2] = tab[i - 1][j + 1].pion > 0 && tab[i - 2][j + 2].pion == 0;
    if (i + 1 != 9 && j - 1 != 0)
      restriction[5] = tab[i + 1][j - 1].pion > 0 && tab[i + 2][j - 2].pion == 0;
    if (i + 1 != 9 && j + 1 != 9)
      restriction[7] = tab[i + 1][j + 1].pion > 0 && tab[i + 2][j + 2].pion == 0;
    break;

  case 4:
    if (i - 1 != 0 && j - 1 != 0)
      restriction[0] = tab[i - 1][j - 1].pion > 0 && tab[i - 2][j - 2].pion == 0;
    if (i - 1 != 0)
      restriction[1] = tab[i - 1][j].pion > 0 && tab[i - 2][j].pion == 0;
    if (i - 1 != 0 && j + 1 != 9)
      restriction[2] = tab[i - 1][j + 1].pion > 0 && tab[i - 2][j + 2].pion == 0;
    if (j - 1 != 0)
      restriction[3] = tab[i][j - 1].pion > 0 && tab[i][j - 2].pion == 0;
    if (j + 1 != 9)
      restriction[4] = tab[i][j + 1].pion > 0 && tab[i][j + 2].pion == 0;
    if (i + 1 != 9 && j - 1 != 0)
      restriction[5] = tab[i + 1][j - 1].pion > 0 && tab[i + 2][j - 2].pion == 0;
    if (i + 1 != 9)
      restriction[6] = tab[i + 1][j].pion > 0 && tab[i + 2][j].pion == 0;
    if (i + 1 != 9 && j + 1 != 9)
      restriction[7] = tab[i + 1][j + 1].pion > 0 && tab[i + 2][j + 2].pion == 0;
    break;
  }
}

bool autorisation_bord(saves_value *partie,coordInt depart, coordInt arrive)
{

  // On regarde tout les mouvements possibles du pion si il est au bord
  if (((partie->move.player == 1 || partie->move.player == 2)  && (arrive.y == 0 || arrive.y == 9)) || ((partie->move.player == 3 || partie->move.player == 4) && (arrive.x == 0 || arrive.x == 9)))
  {
      for (int k = -1; k < 2; k++)
      {
        for (int l = -1; l < 2; l++)
        {
          coordInt coord_temp;
          coord_temp.x = arrive.x + k*2;
          coord_temp.y = arrive.y + l*2;
          if (coord_temp.x <= 9 && coord_temp.y <= 9 && coord_temp.x >= 0 && coord_temp.y >= 0 && partie->tab[coord_temp.x][coord_temp.y].joueur == 0)
          {
            cell temp_tab[10][10];
            
            copierTab(partie->tab,temp_tab);

            temp_tab[arrive.x][arrive.y].joueur = temp_tab[depart.x][depart.y].joueur;
            temp_tab[arrive.x][arrive.y].pion = temp_tab[depart.x][depart.y].pion;
            temp_tab[arrive.x][arrive.y].distinction = temp_tab[depart.x][depart.y].distinction;

            temp_tab[depart.x][depart.y].joueur = 0;
            temp_tab[depart.x][depart.y].pion = 0;
            temp_tab[depart.x][depart.y].distinction = 0;

            // On vérfie qu'il puisse faire un saut pour sortir du bord
            if (autorisation(arrive.x, arrive.y, coord_temp.x, coord_temp.y, temp_tab))
              return true;
          }
        }
    }
    return false;
  }

  return true;
  
}

void TourJoueurs(saves_value *partie, float x, float y, SDL_Rect plateau)
{
  /* Fonction correspondant au tour d'un joueur elle fonctionne seule*/

  if (partie->parametre.ordis > 0 && partie->move.player == 1)
  {

    coup_IA coup = minimax(*partie, 2, -100000, 100000, true);

    copierTab(coup.position_coup.tab, partie->tab);
    partie->move.player = 2;
    // Incrémentation du nombre de tours

    // On réinitialise le tour
    partie->move.selected = false;
    partie->selectedBox.x = -1;
    partie->selectedBox.y = -1;
    partie->move.inTurn = false;
    partie->time = CHRONO;
    initialisationTabRest(partie->restriction, true);
  }
  else
  {
    // Création des coordonées de la case sélectionnée
    coordInt *coord_temp;
    coord_temp = malloc(sizeof(*coord_temp));
    // Si le joueur n'a pas encore désigné son pion de départ
    if (!partie->move.selected)
    {
      // Sélection du pion temporaire de départ
      *coord_temp = selectionPion(partie->tab, x, y, plateau);
      // Si cela correspond à un des ses pions
      if (partie->tab[coord_temp->x][coord_temp->y].joueur == partie->move.player && partie->tab[coord_temp->x][coord_temp->y].pion != 0)
      {
        partie->move.selected = true;      // Il a alors sélectionné son pion
        partie->selectedBox = *coord_temp; // On valide que la case appuyée correspond à un pion pour pouvoir le déplacer plus tard
        partie->move.inTurn = true;        // Le joueur est alors en tour tant le code sera répété tant qu'il n'a pas finis
        partie->move.firstMove = false;    // Le joueur n'a pas encore fait son premier déplacement et ne peut donc pas finir son tour
      }
    }
    else
    {
      // Sélection du pion d'arrivée

      // Coordonnées temporaires de la position du pion d'arrivé en fonction du pion de départ
      int i_temp = 0;
      int j_temp = 0;

      // Sélection de la case temporaire d'arrivé
      *coord_temp = selectionMove(partie->tab, x, y, plateau);

      // Si il est valide
      if (coord_temp->x >= 0 && coord_temp->y >= 0)
      {

        /* Calcul de la position du pion d'arrivé en fonction d'un autre
        (-1,1) ou (1,1) ou (-1,0) etc...
        Cela permettera de savoir si le saut est possible par la suite.
        Le code fonctionne très bien si l'utilisateur veut sauter ou pas
        car il fait la partie entière de sa division par 2.
      */
        float distance_i_temp = fabs((coord_temp->x - partie->selectedBox.x) / 2.0);
        float distance_j_temp = fabs((coord_temp->y - partie->selectedBox.y) / 2.0);

        int signe_i_temp = 0;
        int signe_j_temp = 0;

        if (distance_i_temp != 0)
          signe_i_temp = (coord_temp->x - partie->selectedBox.x) / (2.0 * distance_i_temp);

        if (distance_j_temp != 0)
          signe_j_temp = (coord_temp->y - partie->selectedBox.y) / (2.0 * distance_j_temp);

        if (distance_i_temp <= 1)
          i_temp = ceil(distance_i_temp) * signe_i_temp;
        else
          i_temp = floor(distance_i_temp) * signe_i_temp;

        if (distance_j_temp <= 1)
          j_temp = ceil(distance_j_temp) * signe_j_temp;
        else
          j_temp = floor(distance_j_temp) * signe_j_temp;

        // Si la coordonnée temporaire repecte les règles de jeux et qu'il respecte les restrictions apportées par son saut (elles n'existent pas si il n'a pas encore joué (voir init du tab))
        if (autorisation(partie->selectedBox.x, partie->selectedBox.y, coord_temp->x, coord_temp->y, partie->tab) && partie->restriction[elementCorrespondant(i_temp, j_temp)] && autorisation_bord(partie, partie->selectedBox,*coord_temp))
        {
          // Le mouvement est donc validé et le pion se déplace
          partie->tab[coord_temp->x][coord_temp->y].joueur = partie->tab[partie->selectedBox.x][partie->selectedBox.y].joueur;
          partie->tab[coord_temp->x][coord_temp->y].pion = partie->tab[partie->selectedBox.x][partie->selectedBox.y].pion;
          partie->tab[coord_temp->x][coord_temp->y].distinction = partie->tab[partie->selectedBox.x][partie->selectedBox.y].distinction;

          int i = 0;
          bool joueurVise = partie->caseVisit[coord_temp->x][coord_temp->y][i].joueur != partie->tab[coord_temp->x][coord_temp->y].joueur;
          bool PionVise = partie->caseVisit[coord_temp->x][coord_temp->y][i].pion != partie->tab[coord_temp->x][coord_temp->y].pion;
          bool distinctionVise = partie->caseVisit[coord_temp->x][coord_temp->y][i].distinction != partie->tab[coord_temp->x][coord_temp->y].distinction;
          while ((joueurVise || PionVise || distinctionVise) && i < PION_LIST_LENGTH)
          {
            i++;
            joueurVise = partie->caseVisit[coord_temp->x][coord_temp->y][i].joueur != partie->tab[coord_temp->x][coord_temp->y].joueur;
            PionVise = partie->caseVisit[coord_temp->x][coord_temp->y][i].pion != partie->tab[coord_temp->x][coord_temp->y].pion;
            distinctionVise = partie->caseVisit[coord_temp->x][coord_temp->y][i].joueur != partie->tab[coord_temp->x][coord_temp->y].distinction;
          }

          partie->caseVisit[coord_temp->x][coord_temp->y][i].compteur++;

          partie->tab[partie->selectedBox.x][partie->selectedBox.y].joueur = 0;
          partie->tab[partie->selectedBox.x][partie->selectedBox.y].pion = 0;
          partie->tab[partie->selectedBox.x][partie->selectedBox.y].distinction = 0;

          // La case sélectionné prend alors la valeur de la case temporaire pour pouvoir faire des sauts et recommencer se code avec cette nouvelle case
          partie->selectedBox = *coord_temp;

          // Le joueur a fait son premier coup ou la déjà fait, il peut alors finir son tour ou pas si les restrictions l'autorise
          partie->move.firstMove = true;

          if (((partie->move.player == 1 || partie->move.player == 2) && (partie->selectedBox.y == 0 || partie->selectedBox.y == 9)) || ((partie->move.player == 3 || partie->move.player == 4) && (partie->selectedBox.x == 0 || partie->selectedBox.x == 9)))
            partie->move.inBorder = true;
          else
            partie->move.inBorder = false;

          // Création des restrictions en fonction de chaque joueur et chaque pion
          initialisationTabRest(partie->restriction, false);

          // Conditions correspondants aux 8 cases qui entourent le pion sélectionné
          remplirTabRest(partie->tab, partie->restriction, &partie->move, partie->selectedBox.x, partie->selectedBox.y);

          // On regarde si le joueur peut encore jouer
          partie->move.inTurn = false;
          for (int i = 0; i < 8; i++)
          {
            coordInt coord_temp_nextTurn;
            coord_temp_nextTurn.x = partie->selectedBox.x + elementCorrespondantInverse(i).x * 2;
            coord_temp_nextTurn.y = partie->selectedBox.y + elementCorrespondantInverse(i).y * 2;
            // Si il existe une seule restriction possible le joueur peut jouer
            if (autorisation(partie->selectedBox.x, partie->selectedBox.y, coord_temp_nextTurn.x, coord_temp_nextTurn.y, partie->tab) && partie->restriction[i] && autorisation_bord(partie,partie->selectedBox, coord_temp_nextTurn))
              partie->move.inTurn = true;
          }
        }
      }

      // Si le joueur n'a pas cliqué sur un case valable du tableau, vérifie qu'il veuille re-sélectionner un nouveau pion
      *coord_temp = selectionPion(partie->tab, x, y, plateau);
      // Le code est similaire à la sélection
      if (partie->tab[coord_temp->x][coord_temp->y].joueur == partie->move.player && partie->move.firstMove == false)
        partie->selectedBox = *coord_temp;

      // Si le tour d'un joueur est finir alors on passe au joueur suivant et on remet les valeurs à zéro
      if (!partie->move.inTurn)
      {
        partie->move.player = partie->move.player % (partie->parametre.joueurs + partie->parametre.ordis) + 1;
        // Incrémentation du nombre de tours
        if (partie->move.player == 1)
          partie->turn++;

        // On cherche le premier joueur encore en jeu
        while (partie->joueursEnVie[partie->move.player - 1] == false)
        {
          partie->move.player = partie->move.player % (partie->parametre.joueurs + partie->parametre.ordis) + 1;
        }
        // On réinitialise le tour
        partie->move.selected = false;
        partie->selectedBox.x = -1;
        partie->selectedBox.y = -1;
        partie->move.inTurn = false;
        partie->time = CHRONO;
        initialisationTabRest(partie->restriction, true);
      }
    }
    free(coord_temp);

    return;
  }
}

fin_partie conditionDeFin(saves_value *partie)
{
  /* Fonction qui test tout les cas de figues de fin de partie*/
  fin_partie testFin;
  testFin.joueur = 0;
  testFin.fin = 0;

  int accJoueur1 = 0;
  int accJoueur2 = 0;
  int accJoueur3 = 0;
  int accJoueur4 = 0;

  // Test de victoire
  for (int i = 0; i < 8; i++)
  {
    // Joueur 1
    if (partie->tab[0][1 + i].joueur == 1)
      accJoueur1++;
    // Joueur 2
    if (partie->tab[9][1 + i].joueur == 2)
      accJoueur2++;
    // Joueur 3
    if (partie->tab[1 + i][9].joueur == 3)
      accJoueur3++;
    // Joueur 4
    if (partie->tab[1 + i][0].joueur == 4)
      accJoueur4++;
  }

  if (accJoueur1 == 8)
    testFin.joueur = 1;
  else if (accJoueur2 == 8)
    testFin.joueur = 2;
  else if (accJoueur3 == 8)
    testFin.joueur = 3;
  else if (accJoueur4 == 8)
    testFin.joueur = 4;

  if (accJoueur1 == 8 || accJoueur2 == 8 || accJoueur3 == 8 || accJoueur4 == 8)
    testFin.fin = 1;

  // Test de défaite
  accJoueur1 = 0;
  accJoueur2 = 0;
  accJoueur3 = 0;
  accJoueur4 = 0;

  for (int i = 0; i < 8; i++)
  {
    if (partie->turn > 30)
    {
      // Joueur 1
      if (partie->tab[9][1 + i].joueur == 1)
        accJoueur1++;
      // Joueur 2
      if (partie->tab[0][1 + i].joueur == 2)
        accJoueur2++;
      // Joueur 3
      if (partie->tab[1 + i][0].joueur == 3)
        accJoueur3++;
      // Joueur 4
      if (partie->tab[1 + i][9].joueur == 4)
        accJoueur4++;
    }
  }

  if (accJoueur1 > 0)
    testFin.joueur = 1;
  else if (accJoueur2 > 0)
    testFin.joueur = 2;
  else if (accJoueur3 > 0)
    testFin.joueur = 3;
  else if (accJoueur4 > 0)
    testFin.joueur = 4;

  if (partie->time <= 0)
    testFin.joueur = partie->move.player;

  if (testFin.fin == 0 && (testFin.joueur == 1 || testFin.joueur == 2 || testFin.joueur == 3 || testFin.joueur == 4))
    testFin.fin = 2;

  // Test d'égalité
  accJoueur1 = 0;
  accJoueur2 = 0;
  accJoueur3 = 0;
  accJoueur4 = 0;

  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      for (int k = 0; k < PION_LIST_LENGTH; k++)
      {
        if (partie->caseVisit[i][j][k].compteur >= 3)
        {
          testFin.fin = 3;
          return testFin;
        }
      }
    }
  }

  // Élimination d'un joueur s'il a perdu
  if (testFin.fin == 2)
  {
    if (partie->joueursEnVie[0] + partie->joueursEnVie[1] + partie->joueursEnVie[2] + partie->joueursEnVie[3] == 2) // Si ils ne sont plus que deux
    {
      testFin.fin = 1;
      do
      { // On recherche le premier joueur encore en jeu
        testFin.joueur = testFin.joueur % (partie->parametre.joueurs + partie->parametre.ordis) + 1;
      } while (partie->joueursEnVie[testFin.joueur - 1] == false);
    }
    else
    { // Sinon en vide le plateau du joueur qui a perdu
      for (int i = 0; i < 10; i++)
      {
        for (int j = 0; j < 10; j++)
        {
          if (partie->tab[i][j].joueur == testFin.joueur)
          {
            partie->tab[i][j].joueur = 0;
            partie->tab[i][j].pion = 0;
            partie->tab[i][j].distinction = 0;
          }
        }
      }
      // On envoie la valeur de défaite au joueur correspondant
      partie->joueursEnVie[testFin.joueur - 1] = false;

      testFin.joueur = 0;

      partie->move.inTurn = false;
      partie->move.selected = true;
    }
  }

  return testFin;
}