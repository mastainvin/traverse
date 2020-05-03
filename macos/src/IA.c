#include <stdio.h>
#include <stdlib.h>

#include "FonctionsDuJeu.h"
#include "IA.h"
#include <math.h>

int evaluationPosition(saves_value position, int player)
{
  /* Fonction d'évaluation d'un partie suivant un joueur donné, retourne la distance la plus courte d'un pion pour aller à une case
  d'arriver, elle prend en cote les déplacement possible d'un pion pour que le joueur ne soit pas bloqué*/
  int acc = 0;

  // Le pion qui pose soucis est le losange car il ne peut pas changer de couleur de case (s'il démarre sur un case noir il doit finir sur un case noir)
  int cases_impaires = 0;
  int cases_paires = 0;

  bool losange_impaire = false;
  bool losange_paire = false;

  // On compte le nombre de case noir est blanche encore disponiblent à l'arriver, on vérifie aussi que les losanges soit placés ou non à l'arrivé
  for (int i = 1; i < 9; i++)
  {
    if ((position.tab[9 * (player - 1)][i].joueur == (player % 2) + 1) || (position.tab[9 * (player - 1)][i].joueur == 0))
    {
      if (i % 2 == 0)
        cases_paires++;
      else
        cases_impaires++;
    }
    else if (position.tab[9 * (player - 1)][i].joueur == player && position.tab[9 * (player - 1)][i].pion == 3)
    {
      if (position.tab[9 * (player - 1)][i].distinction == 1)
        losange_impaire = true;
      else
        losange_paire = true;
    }
  }

  // Pour chaque case du tableau
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {

      // On vérifie que le pion sur la case appartient au joueur désigner
      if (position.tab[i][j].joueur == player)
      {
        // Selon le case ou le joueur est sur la case d'arriver on va attribuer des point différents
        if (i == 9 * (player - 1))
        {
          // Ici, on vérifie que le pion sur la case ne prend pas la place d'un losange au sinon, il y aurai bloquage de la partie
          if (((j % 2 == 1 && (losange_paire || cases_impaires >= 1)) || (j % 2 == 0 && (losange_impaire || cases_paires >= 1))) || (position.tab[i][j].pion == 3))
            acc -= 100;
          else
            acc += 1000;
        }
        else // Ici, le pion n'est pas sur une case d'arrrivé, on vérifie si le coup est favorable ou non
        {

          // L'objectif est de calculer la distance minimale entre ce pion est l'arrivé
          int distanceMin = 1000;
          for (int k = 1; k < 9; k++)
          {
            // Ici, des booléens qui vérifie, selon pion si le coup est vraiment favorable car le but n'est pas de juste ce rapprocher de l'arriver mais d'aller sur la case de l'arrivé la plus proche et possible d'accès

            // Le triangle peut change de couleur de case, cependant il favorable qu'il y reste on le fait donc changer seulement si nécessaire
            bool triangle = (position.tab[i][j].pion == 2) && (j % 2 == (k + 1) % 2 || cases_paires == 0 || cases_impaires == 0);

            // On vérifie que la case la plus proche soit possible d'accès pour le losange

            bool losange = (position.tab[i][j].pion == 3) && ((position.tab[i][j].distinction == 1 && k % 2 == 0) || (position.tab[i][j].distinction == 2 && k % 2 == 1));

            // On réunit les conditions, case vide  + triangle + losange
            if ((position.turn <= 30 || i != 9 * ((player + 1) % 2)) && ((position.tab[9 * (player - 1)][k].joueur == (player % 2) + 1) || (position.tab[9 * (player - 1)][k].joueur == 0)) && ((position.tab[i][j].pion != 2 && position.tab[i][j].pion != 3) || triangle || losange))
            {
              // On calcule la distance
              int temp = (abs(9 * (player - 1) - i) + abs(j - k));

              // Parfois le triange peut se bloquer, on l'empèche ici.
              if (position.tab[i][j].pion == 2 && i == 7 * (player - 1) + 1 && (position.tab[9 * (player - 1)][j].joueur == 0 || (abs(j - k) == 1 && ((i % 2 == 0 && cases_impaires == 1) || (i % 2 == 1 && cases_paires == 1)))))
                temp += 3;

              // On vérifie que cette distance est la plus courte
              if (temp < distanceMin)
                distanceMin = temp;
            }
          }

          // On rajoute la distance à l'accumulateur
          acc += distanceMin;
        }
      }
    }
  }
  // On retourne l'apposer car minmax cherche à maximiser un coup et ici un veut minimiser des distances
  return -acc;
}

coup_IA minimax(saves_value position, int depth, int alpha, int beta, bool maximizingPlayer)
{
  coup_IA coup;

  // Si on est la profondeur 0  on calcul l'évaluation du noeud
  if (depth == 0)
  {
    coup.valeur = evaluationPosition(position, (maximizingPlayer + 1) % 2 + 1);
    return coup;
  }

  // Si on cherche à maximiser le joueur
  if (maximizingPlayer)
  {
    coup.valeur = -INFINI;
    coup_IA eval;
    transit transition_list = newTransitionList();
    position.move.player = 1;
    position.turn++;

    // On créer une liste de tous déplacements possibles
    transition_list = creationTransition(position, transition_list);
    bool continuer = true;
    transit temp = transition_list;

    // On parcours les déplacements
    while (temp != NULL && continuer)
    {
      copierTab(temp->value.tab, eval.position_coup.tab);
      eval.position_coup.move.player = 1;
      eval.position_coup.turn = temp->value.turn;
      eval.valeur = minimax(eval.position_coup, depth - 1, alpha, beta, false).valeur;

  
      // On cherche ici la meilleur évaluation possible
      if (coup.valeur < eval.valeur)
      {
        copierTab(eval.position_coup.tab, coup.position_coup.tab);
        coup.position_coup.turn = eval.position_coup.turn;
        coup.valeur = eval.valeur;
      }

      // élage alpha beta
      if (alpha < eval.valeur)
        alpha = eval.valeur;

      if (beta <= alpha)
      {
        continuer = false;
      }
      else
      {
        temp = temp->next;
      }
    }
  

    // On supprime la liste de transition
    transition_list = deleteAll_transitionList(transition_list);
    return coup;
  }
  else // On effectue ici la même chose sauf qu'on cherche à minimiser
  {
    coup.valeur = +INFINI;
    coup_IA eval;
    transit transition_list = newTransitionList();
    position.move.player = 2;
    position.turn++;
    transition_list = creationTransition(position, transition_list);
    transit temp = transition_list;
    bool continuer = true;
    while (temp != NULL && continuer)
    {

      copierTab(temp->value.tab, eval.position_coup.tab);
      eval.position_coup.move.player = 2;
      eval.position_coup.turn = temp->value.turn;
      eval.valeur = minimax(eval.position_coup, depth - 1, alpha, beta, true).valeur;
    

      if (coup.valeur > eval.valeur)
      {

        copierTab(eval.position_coup.tab, coup.position_coup.tab);
        coup.position_coup.turn = eval.position_coup.turn;
        coup.valeur = eval.valeur;
      }

      if (beta > eval.valeur)
        beta = eval.valeur;

      if (beta <= alpha)
      {
        continuer = false;
      }
      else
      {
        temp = temp->next;
      }
    }

    transition_list = deleteAll_transitionList(transition_list);
    return coup;
  }
}

/* Fonctions pour la liste de transitions */

// Création d'une nouvelle liste
transit newTransitionList(void)
{
  return NULL;
}

bool is_empty_transitionList(transit li)
{
  // Si le premier élément n'existe pas alors l'ensemble n'existe pas (utilisation des boolléen créés)
  if (li == NULL)
    return true;

  return false;
}

/*----------------------------------------------------*/

// Fonction de rajout d'une sauvegarde
transit add_transition(transit li, saves_value newValue)
{
  // Allocation de mémoire pour la nouvelle sauvegarde
  transition_list *newPosition;
  newPosition = malloc(sizeof(*newPosition));
  /*On verfie que l'allocation dynamique a bien fonctionné*/
  if (newPosition == NULL)
  {
    printf("ERREUR D'ALLOCATION DYNAMIQUE.\n");
    exit(1);
  }
  // On attribut la valeur spécifiée
  newPosition->value = newValue;

  // Si la liste n'est pas vide alors on crée le lien entre le nouvel élément est l'ancien premier élément
  if (!is_empty_transitionList(li))
  {
    newPosition->next = li;
  }
  else
  {
    newPosition->next = NULL;
  }

  return newPosition;
}

/*----------------------------------------------------*/

// Fonction de suppression du premier élément de la liste (utilisé pour la suppression de la liste entière)
transit pop_front_transitionList(transit li)
{
  // Si la liste est déjà vide ont retourne une liste vide
  if (is_empty_transitionList(li))
  {
    return newTransitionList();
  }
  /* Si cela correpond au dernier */
  if (li->next == NULL)
  {
    free(li);
    li = NULL;
    return newTransitionList();
  }
  /* On récupère l'élément suivant pour le définir comme la liste et on libère le premier*/
  transition_list *element;
  element = malloc(sizeof(*element));
  element = li->next;
  free(li);
  li = NULL;
  return element;
}

/*----------------------------------------------------*/

// Fonction de suppression entière de la liste (Pour libèrer la mémoire en fin de programme)
transit deleteAll_transitionList(transit li)
{
  // On parcours toute la liste et on supprime chaque éléments
  while (li != NULL)
  {
    li = pop_front_transitionList(li);
  }
  return NULL;
}

void afficher_transition_list(transit li)
{
  transit temp = NULL;
  temp = li;
  while (temp != NULL)
  {
    printf("\n");
    for (int i = 0; i < 10; i++)
    {
      for (int j = 0; j < 10; j++)
      {
        printf(" | %d - %d | ", temp->value.tab[i][j].joueur, temp->value.tab[i][j].pion);
      }
      printf("\n");
    }

    temp = temp->next;
  }
}
/*----------------------------------------------------*/

// Fonction qui créé une liste de position en fonction d'une position de base
transit transition_function(saves_value position, transit transition_list, int nombre_saut)
{

  // On regarde tout les mouvements possibles d'un pion est on les ajoute à la liste de transitions
  if (position.tab[position.selectedBox.x][position.selectedBox.y].joueur == position.move.player)
  {
    for (int k = -2; k < 3; k++)
    {
      for (int l = -2; l < 3; l++)
      {
        coordInt coord_temp;
        coord_temp.x = position.selectedBox.x + k;
        coord_temp.y = position.selectedBox.y + l;
        if (coord_temp.x <= 9 && coord_temp.y <= 9 && coord_temp.x >= 0 && coord_temp.y >= 0 && position.tab[coord_temp.x][coord_temp.y].joueur == 0)
        {

          /* Calcul de la position du pion d'arrivé en fonction d'un autre
					(-1,1) ou (1,1) ou (-1,0) etc...
					Cela permettera de savoir si le saut est possible par la suite.
					Le code fonctionne très bien si l'utilisateur veut sauter ou pas
					car il fait la partie entière de sa division par 2.*/

          int i_temp = 0;
          int j_temp = 0;

          float distance_i_temp = fabs((coord_temp.x - position.selectedBox.x) / 2.0);
          float distance_j_temp = fabs((coord_temp.y - position.selectedBox.y) / 2.0);

          int signe_i_temp = 0;
          int signe_j_temp = 0;

          if (distance_i_temp != 0)
            signe_i_temp = (coord_temp.x - position.selectedBox.x) / (2.0 * distance_i_temp);

          if (distance_j_temp != 0)
            signe_j_temp = (coord_temp.y - position.selectedBox.y) / (2.0 * distance_j_temp);

          if (distance_i_temp != 0)
          {
            if (distance_i_temp <= 1)
              i_temp = ceil(distance_i_temp) * signe_i_temp;
            else
              i_temp = floor(distance_i_temp) * signe_i_temp;
          }

          if (distance_j_temp)
          {
            if (distance_j_temp <= 1)
              j_temp = ceil(distance_j_temp) * signe_j_temp;
            else
              j_temp = floor(distance_j_temp) * signe_j_temp;
          }

          bool back_jump = false;

          // On vérifie que le joueurs ne saute pas vers l'arrière, ce qui inutile
          if (position.move.selected)
          {
            switch (position.move.player)
            {
            case 1:
              if (coord_temp.x > position.selectedBox.x)
                back_jump = true;
              break;
            case 2:
              if (coord_temp.x < position.selectedBox.x)
                back_jump = true;
              break;
            default:
              break;
            }
          }

          if (autorisation(position.selectedBox.x, position.selectedBox.y, coord_temp.x, coord_temp.y, position.tab) && position.restriction[elementCorrespondant(i_temp, j_temp)] && autorisation_bord(&position, position.selectedBox, coord_temp) && !back_jump)
          {
            // On créé un tableau temporaire pour les sauts
            saves_value position_temp;

            copierTab(position.tab, position_temp.tab);
            position_temp.parametre.niveau = position.parametre.niveau;
            position_temp.move.player = position.move.player;
            position_temp.selectedBox.x = position.selectedBox.x;
            position_temp.selectedBox.y = position.selectedBox.y;

            position_temp.tab[coord_temp.x][coord_temp.y].joueur = position_temp.tab[position.selectedBox.x][position.selectedBox.y].joueur;
            position_temp.tab[coord_temp.x][coord_temp.y].pion = position_temp.tab[position.selectedBox.x][position.selectedBox.y].pion;
            position_temp.tab[coord_temp.x][coord_temp.y].distinction = position_temp.tab[position.selectedBox.x][position.selectedBox.y].distinction;

            position_temp.tab[position_temp.selectedBox.x][position_temp.selectedBox.y].joueur = 0;
            position_temp.tab[position_temp.selectedBox.x][position_temp.selectedBox.y].pion = 0;
            position_temp.tab[position_temp.selectedBox.x][position_temp.selectedBox.y].distinction = 0;

            position_temp.selectedBox.x = coord_temp.x;
            position_temp.selectedBox.y = coord_temp.y;

            position_temp.turn = position.turn;
            // Création des restrictions en fonction de chaque joueur et chaque pion
            initialisationTabRest(position_temp.restriction, false);

            // On rend possible les sauts seulement si la difficulté est de deux
            if (position_temp.parametre.niveau == 2)
              remplirTabRest(position_temp.tab, position_temp.restriction, &position_temp.move, position_temp.selectedBox.x, position_temp.selectedBox.y);

            // On regarde si le joueur peut encore jouer
            position_temp.move.inTurn = false;
            for (int i = 0; i < 8; i++)
            {
              coordInt coord_temp_nextTurn;
              coord_temp_nextTurn.x = position_temp.selectedBox.x + elementCorrespondantInverse(i).x * 2;
              coord_temp_nextTurn.y = position_temp.selectedBox.y + elementCorrespondantInverse(i).y * 2;
              // Si il existe une seule restriction possible le joueur peut jouer
              if (position_temp.restriction[i] && autorisation_bord(&position_temp, position_temp.selectedBox, coord_temp_nextTurn))
                position_temp.move.inTurn = true;
            }

            // On ajoute un nombre limiter de sauts dans la liste
            if (position_temp.move.inTurn && nombre_saut < 3)
            {
              position_temp.move.selected = true;
              transition_list = transition_function(position_temp, transition_list, nombre_saut + 1);
            }
            else
            {
              transition_list = add_transition(transition_list, position_temp);
              //return transition_list;
              position_temp.move.inTurn = false;
            }
          }
        }
      }
    }
  }
  else
  {
    position.move.inTurn = false;
  }
  return transition_list;
}

transit creationTransition(saves_value position, transit transition_list)
{
  // Fonction qui ajoute tous les mouvements possibles à partir d'un tableau d'un position donnée
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      position.move.selected = false;
      position.selectedBox.x = i;
      position.selectedBox.y = j;
      initialisationTabRest(position.restriction, true);

      transition_list = transition_function(position, transition_list, 0);
    }
  }

  return transition_list;
}