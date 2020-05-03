#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "interface.h"
#include "FonctionsDuJeu.h"
#include "saves.h"
#include "config.h"
#include <time.h>
#include <stdlib.h>

int main(int argc, char **argv)
{

	/* Création des variables générales*/

	// Taille de la fenetre
	SDL_Rect *fenetre;
	fenetre = malloc(sizeof(*fenetre));
	fenetre->h = 800;
	fenetre->w = 1280;

	// Position du dernierclic
	coord coordClic;
	coordClic.x = 0;
	coordClic.y = 0;

	// Position du curseur
	coord coordMouse;
	coordMouse.x = 0;
	coordMouse.y = 0;

	// Initialisation partie
	saves_value partie_en_cours;
	initialisationCaseVisit(partie_en_cours.caseVisit);
	initilisationPlayerMove(&partie_en_cours.move);
	initialisationTabRest(partie_en_cours.restriction, true);
	initialisationCaseVisit(partie_en_cours.caseVisit);
	initPartie(&partie_en_cours.parametre);
	partie_en_cours.selectedBox.x = -1;
	partie_en_cours.selectedBox.y = -1;

	// Variable utilse pour compté le temps d'un tour
	float acc_time = 0;

	// fichier config

	FILE *fichier_configuration = open_config();

	config_type configuration;
	configuration = import_config(fichier_configuration);

	// On ouvre le fichier de sauvegardes
	FILE *fichier_sauvegarde = open_saves_list();

	// On importe sous forme de liste les sauvegardes
	saves_list *liste_sauvegarde;
	liste_sauvegarde = import_saves(fichier_sauvegarde);


	// config temporaire pour fermer le jeu si il y a modfication
	config_type config_temp;
	config_temp.theme = configuration.theme;
	config_temp.fps = configuration.fps;
	config_temp.fullscreen = configuration.fullscreen;

	// Variable utilisé pour la limite des FPS
	unsigned int frame_limit = 0;

	// Variables d'évenement
	bool croix_fermeture = false;
	bool echap = false;
	bool saving = false;
	/* Déclaration des variables utilent pour le menu */

	// Sélection actuel
	menu_bouton selection = rien;

	/* Déclation des variables utilent pour le jeu */

	bool inPause = false;

	// Listes des restrictions de saut envoyés au Pion en cours de mouvement
	nom_sauvegarde save_name;
	initNomSauvegarde(&save_name);

	fleche bouton_fleche;
	bouton_fleche.haut = false;
	bouton_fleche.bas = false;
	bouton_fleche.element = 1;

	// Définition à l'aide de la SDL du plateau
	SDL_Rect plateau;
	plateau.w = 500;
	plateau.h = 500;
	plateau.x = (fenetre->w - plateau.w) / 2;
	plateau.y = (fenetre->h - plateau.h) / 2;

	/* Déclaration propres à la SDL*/

	// Déclaration des variables de la fenêtre et du renderer pour faire fontionner la SDL
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	// Initialisation de la SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		SDL_ExitWithError("Initialisation de la SDL", renderer, window);

	// Création de la fenêtre et du renderer en fonction paramètres d'entrés
	int modeOuverture = 0;
	if (configuration.fullscreen)
		modeOuverture = SDL_WINDOW_FULLSCREEN;

	if (SDL_CreateWindowAndRenderer(fenetre->w, fenetre->h, modeOuverture, &window, &renderer) != 0)
		SDL_ExitWithError("Creation fenetre et rendu echouee", renderer, window);

	// Initialise de la librairie texte de SDL
	if (TTF_Init() != 0)
		SDL_ExitWithError("Chargement de ttf", renderer, window);

	// Effacement du curseur original
	SDL_ShowCursor(SDL_DISABLE);

	// Création d'un nouveau curseur
	curseur monCurseur;

	// Initialisation de la varable loc qui désignera où se situe l'utilisateur
	location loc = inMenu;
	// Boucle du jeu tant que la location de l'utilisateur est différent de quitter
	while (loc != quit)
	{

		frame_limit = SDL_GetTicks() + FRAME_PER_SECOND;
		// Mise à zéro événements pour empêcher des actions non désirées (clic infini)
		coordClic.x = 0;
		coordClic.y = 0;

		bouton_fleche.haut = false;
		bouton_fleche.bas = false;

		croix_fermeture = false;
		echap = false;

		// Déclaration de la variable événement
		SDL_Event event;

		// Récupération des événement avec la SDL
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
			{
				croix_fermeture = true;
			}
			break;
			case SDL_MOUSEMOTION:
			{
				monCurseur.position.x = event.motion.x;
				monCurseur.position.y = event.motion.y;
				coordMouse.x = monCurseur.position.x;
				coordMouse.y = monCurseur.position.y;
			}
			break;
			case SDL_MOUSEBUTTONDOWN:
			{
				coordClic.x = event.button.x;
				coordClic.y = event.button.y;
			}
			break;
			case SDL_KEYDOWN:
			{
				// ON détermine si le bouton appuyé est autorisé
				save_name.lettre = -1;
				int alphabet[28] = {SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f, SDLK_g, SDLK_h, SDLK_i, SDLK_j, SDLK_k, SDLK_l, SDLK_m, SDLK_n, SDLK_o, SDLK_p, SDLK_q, SDLK_r, SDLK_s, SDLK_t, SDLK_u, SDLK_v, SDLK_w, SDLK_x, SDLK_y, SDLK_z, SDLK_DELETE, SDLK_RETURN};
				if (event.key.keysym.sym == SDLK_ESCAPE)
					echap = true;
				for (int i = 0; i < 28; i++)
				{
					if (event.key.keysym.sym == alphabet[i])
						save_name.lettre = i;
				}
				if (event.key.keysym.sym == 8)
				{
					save_name.lettre = 26;
				}

				if (event.key.keysym.sym == SDLK_UP)
					bouton_fleche.haut = true;

				if (event.key.keysym.sym == SDLK_DOWN)
					bouton_fleche.bas = true;
			}
			break;

			default:
				break;
			}
		}
		// Suppression du rendu
		SDL_RenderClear(renderer);
		// Affichage du fond
		creationBackground(renderer, fenetre, configuration);
		// Utilisation des évenements récupérés en fonction de la position en menu ou non de l'utilisateur
		switch (loc)
		{
		// Si l'utilisateur est en jeu
		case inGame:
		{
			if (croix_fermeture)
				loc = quit;

			if (echap)
				inPause = !inPause;

			// Test des conditions de fin de partie
			fin_partie endGame = conditionDeFin(&partie_en_cours);
			if (endGame.fin == 0)
			{
				if (!inPause)
					TourJoueurs(&partie_en_cours, coordClic.x, coordClic.y, plateau);

				plateauDeJeu(renderer, window, fenetre, coordClic, coordMouse, &inPause, &partie_en_cours, configuration);
				generatePion(window, renderer, plateau, &partie_en_cours, configuration);
				FiltreDeplacement(window, renderer, plateau, &partie_en_cours);
			}
			else if (endGame.fin == 2) // Si un utilisateur a perdu on l'élimne de la partie
			{
				TourJoueurs(&partie_en_cours, 0, 0, plateau);
				plateauDeJeu(renderer, window, fenetre, coordClic, coordMouse, &inPause, &partie_en_cours, configuration);
				generatePion(window, renderer, plateau, &partie_en_cours, configuration);
				FiltreDeplacement(window, renderer, plateau, &partie_en_cours);
			}
			else // Sinon il y'a soit égalité soit une victoire
			{
				char *liste_joueurs[4] = {"joueur1", "joueur2", "joueur3", "joueur4"};
				char nomDuFichier[50];
				if (endGame.fin == 3)
				{
					// Egalité
					sprintf(nomDuFichier, "jeu/egalite.bmp");
				}
				else
				{
					// Victoire
					sprintf(nomDuFichier, "jeu/victoire_%s.bmp", liste_joueurs[endGame.joueur - 1]);
				}

				// On affiche l'image correspondante
				afficherImage(nomDuFichier, CENTER, CENTER, renderer, window, fenetre, configuration);
				SDL_Color white = {255, 255, 255};
				afficherTexte("Appuyez sur ENTRER pour revenir au menu principal.", CENTER, 600, white, 50, renderer, window, fenetre);
				if (save_name.lettre == 27)
				{
					loc = inMenu;
					selection = rien;
				}
			}

			// Si on est en pause
			if (inPause)
			{
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
				SDL_RenderFillRect(renderer, NULL);
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
				loc = menu_pause(renderer, window, fenetre, coordMouse, coordClic, &inPause, &partie_en_cours, configuration);
			}
		}

		break;
		case inSave: // Si l'utilisateur est dans le menu de sauvegarde
		{
			if (croix_fermeture)
				loc = quit;
			if (echap)
				loc = inGame;

			loc = menu_save(renderer, window, fenetre, partie_en_cours.tab, coordMouse, coordClic, &inPause, &save_name, &saving, configuration);
		}
		break;
		// Si l'utilisateur est dans le menu
		case inMenu:
		{

			// Utilisation des événements
			if (croix_fermeture)
				selection = quitter;

			switch (selection)
			{

			// Si l'utiliseur est en train de quitter
			case quitter:
				loc = quit;
				break;

			// Si l'utiliseur est en train de commencer
			case commencer:
				initNomSauvegarde(&save_name);
				loc = inGame;
				selection = rien; // ->Remise à zéro pour le moment où on termine la partie
				break;

			// Si l'utiliseur ne fait rien et n'a rien fait : il est dans le menu principal
			case rien:
				selection = menu_principal(renderer, window, fenetre, coordMouse, coordClic, configuration);
				if (echap)
					selection = quitter;

				if (selection == rejouer)
					bouton_fleche.element = 1;
				break;

			// Si l'utiliseur ne fait rien mais à fait quelque chose qui ne pas quitter ou commencer : il est dans un menu secondaire
			default:
				if (echap)
					selection = rien;

				menu_secondaire(renderer, window, fenetre, &selection, coordMouse, coordClic, &partie_en_cours, &bouton_fleche, &configuration, &liste_sauvegarde);

				if (selection == rien && ((configuration.theme != config_temp.theme) || (configuration.fps != config_temp.fps) || (configuration.fullscreen != config_temp.fullscreen)))
					loc = quit;

				break;
			}
		}
		break;

		// L'utiliseur quitte forcément
		default:
			break;
		}

		// Affichage des éléments toujours présents curseur + FPS
		// Limitation des FPS
		afficherImage("curseur.bmp", monCurseur.position.x, monCurseur.position.y, renderer, window, fenetre, configuration);
		limit_fps(frame_limit, fenetre, window, renderer, configuration);
		frame_limit = SDL_GetTicks() + FRAME_PER_SECOND;
		SDL_RenderPresent(renderer);

		// Sauvegarde de la partie actuelle
		if (saving)
		{
			

			// On copie le nom que l'on a précédamment créé

			for (int i = 0; i < NAME_LENGTH; i++)
			{
				partie_en_cours.name[i] = save_name.nom[i];
			}
			// On rajoute la sauvegarde dans la liste
			liste_sauvegarde = add_save(liste_sauvegarde, partie_en_cours);

			// On retourne en jeu
			saving = false;
			loc = inGame;

			
		}

		// Décrémentation du temps de jeu si l'option tour chrono est activé
		if (!inPause && partie_en_cours.parametre.chrono)
		{
			// Si le joueur n'est pas en pause on calcule le temp que parcours le jeu pour atteindre une seconde
			acc_time += 1.0 / (frame_limit - SDL_GetTicks());
			if (acc_time >= 1)
			{
				// Si il vient de s'écouler une seconde on décrémente le temps du tour du joueur
				acc_time = 0;
				partie_en_cours.time--;
			}
		}
	}
	// On exporte la liste dans le fichier
	fichier_sauvegarde = export_saves(liste_sauvegarde, fichier_sauvegarde);

	// On supprimer la liste
	liste_sauvegarde = deleteAll_savesList(liste_sauvegarde);
	close_saves_list(fichier_sauvegarde);

	// Exportation fichier configuration
	fichier_configuration = export_config(configuration, fichier_configuration);
	// On ferme le fichier de configuration
	close_config(fichier_configuration);
	// On détruit les éléments graphique et on ferme la SDL
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();

	return 0;
}
