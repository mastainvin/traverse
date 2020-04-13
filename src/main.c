#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "interface.h"
#include "FonctionsDuJeu.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char **argv){

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

    // Paramètres de la partie
    partie *param_partie;
    param_partie = malloc(sizeof(*param_partie));
    initPartie(param_partie);

    // Variable utilisé pour la limite des FPS
    unsigned int frame_limit = 0;

    // Variables d'évenement
    bool croix_fermeture = false;
    bool echap = false;

    /* Déclaration des variables utilent pour le menu */

    // Sélection actuel
    menu_bouton selection = rien;

    /* Déclation des variables utilent pour le jeu */

    bool inPause = false;

    // Listes des restrictions de saut envoyés au Pion en cours de mouvement
    bool restriction[8];
    initialisationTabRest(restriction,true);

    cell tab[10][10];

    // Voir structure playerMove
    playerMove *move;
    move = malloc(sizeof(*move));
    initilisationPlayerMove(move);

    // Coordonnées entières de la case sélectionné
    coordInt selectedBox;
    selectedBox.x = -1;
    selectedBox.y = -1;

    // Définition à l'aide de la SDL du plateau
    SDL_Rect plateau;
    plateau.w = 500;
    plateau.h = 500;
    plateau.x = (fenetre->w - plateau.w)/2;
    plateau.y = (fenetre->h - plateau.h)/2;


    /* Déclaration propres à la SDL*/

    // Déclaration des variables de la fenêtre et du renderer pour faire fontionner la SDL
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;


    // Initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
      SDL_ExitWithError("Initialisation de la SDL",renderer,window);

    // Création de la fenêtre et du renderer en fonction paramètres d'entrés
    int modeOuverture = 0;
    if (argc == 2 && strcmp(argv[1]," fullscreen")){
        modeOuverture = SDL_WINDOW_FULLSCREEN;
    }
    if(SDL_CreateWindowAndRenderer(fenetre->w,fenetre->h,modeOuverture,&window,&renderer) != 0)
      SDL_ExitWithError("Creation fenetre et rendu echouee",renderer,window);

    // Initialise de la librairie texte de SDL
    if(TTF_Init() != 0)
      SDL_ExitWithError("Chargement de ttf",renderer,window);

    // Effacement du curseur original
    SDL_ShowCursor(SDL_DISABLE);

    // Création d'un nouveau curseur
    curseur monCurseur = creerCurseur(renderer,window);

    // Initialisation de la varable loc qui désignera où se situe l'utilisateur
    location loc = inMenu;

    frame_limit = SDL_GetTicks() + FRAME_PER_SECOND;

    // Boucle du jeu tant que la location de l'utilisateur est différent de quitter
    while (loc != quit) {

      // Mise à zéro événements pour empêcher des actions non désirées (clic infini)
      coordClic.x = 0;
      coordClic.y = 0;
      croix_fermeture = false;
      echap = false;

      // Déclaration de la variable événement
      SDL_Event  event;

      // Récupération des événement avec la SDL
      while (SDL_PollEvent(&event)) {
        switch (event.type) {
          case SDL_QUIT:
            croix_fermeture = true;
          break;
          case SDL_MOUSEMOTION:
            monCurseur.position.x = event.motion.x;
            monCurseur.position.y = event.motion.y;
            coordMouse.x = monCurseur.position.x;
            coordMouse.y = monCurseur.position.y;
          break;
          case SDL_MOUSEBUTTONDOWN:
            coordClic.x = event.button.x;
            coordClic.y = event.button.y;
          break;
          case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
              echap = true;
          break;

          default:
          break;
        }
      }

      // Suppression du rendu
      SDL_RenderClear(renderer);
      // Affichage du fond
      creationBackground(renderer,fenetre);
      // Utilisation des évenements récupérés en fonction de la position en menu ou non de l'utilisateur
      switch (loc) {
        // Si l'utilisateur est en jeu
        case inGame:{
          if(croix_fermeture)
            loc = quit;
          if(echap)
            inPause = !inPause;

          TourJoueurs(tab,param_partie,coordClic.x,coordClic.y, plateau, &selectedBox,restriction, move);
          creationFond(renderer,window,fenetre,coordClic,coordMouse, &inPause, move);
          generatePion(window,renderer,plateau,tab);

          // Si on est en pause
          if(inPause){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_RenderFillRect(renderer, NULL);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
            loc = menu_pause(renderer,window,fenetre,coordMouse,coordClic, &inPause);
          }
        }

        break;

        // Si l'utilisateur est dans le menu
        case inMenu:{

          // Utilisation des événements
          if(croix_fermeture || echap)
            selection = quitter;

          switch (selection) {

            // Si l'utiliseur est en train de quitter
            case quitter:
              loc = quit;
            break;

            // Si l'utiliseur est en train de commencer
            case commencer:
              // Initialisation du tableau qui sert de plateau de jeu en fonction du nombre de joueur
              initialisationTab(tab,param_partie->joueurs + param_partie->ordis);
              loc = inGame;
              selection = rien; // ->Remise à zéro pour le moment où on termine la partie
            break;

            // Si l'utiliseur ne fait rien et n'a rien fait : il est dans le menu principal
            case rien:
              selection = menu_principal(renderer,window,fenetre,coordMouse,coordClic);
            break;

            // Si l'utiliseur ne fait rien mais à fait quelque chose qui ne pas quitter ou commencer : il est dans un menu secondaire
            default:
              selection = menu_secondaire(renderer,window,fenetre, selection,coordMouse,coordClic, param_partie);
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
      afficherCurseur(monCurseur,renderer);
      frame_limit = SDL_GetTicks() + FRAME_PER_SECOND;
      limit_fps(frame_limit,fenetre,window,renderer);
      frame_limit = SDL_GetTicks() + FRAME_PER_SECOND;

      SDL_RenderPresent(renderer);



  }

    // Destruction du curseur, du renderer et de la fenêtre et fermeture de la SDL
    detruireCurseur(monCurseur);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
