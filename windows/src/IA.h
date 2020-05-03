#ifndef IA_H
#define IA_H

#include "FonctionsDuJeu.h"

#define INFINI 100000

/* Structures nécessaires à la transition*/
typedef struct transition_list
{
	saves_value value;
	struct transition_list *next;
} transition_list, *transit;

typedef struct coup_IA
{
	int valeur;
	saves_value position_coup;
} coup_IA;

int evaluationPosition(saves_value position, int player);
coup_IA minimax(saves_value position, int depth, int alpha, int beta, bool maximizingPlayer);

/* Fonctions liés à la transition*/

// Ici pour la gestion générale de chaque liste
transit newTransitionList(void);
bool is_empty_transitionList(transit li);
transit add_transition(transit li, saves_value newValue);
transit pop_front_transitionList(transit li);
transit deleteAll_transitionList(transit li);
void afficher_transition_list(transit li);

// Pour l'algorithme minimax
transit transition_function(saves_value position, transit transition_list, int nombre_saut);
transit creationTransition(saves_value position, transit transition_list);

#endif