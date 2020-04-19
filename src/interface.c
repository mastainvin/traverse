#include "interface.h"
#include "FonctionsDuJeu.h"
#include "saves.h"
#include "config.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Fonction de retour d'erreur permet fermeture propre de la fenêtre
void SDL_ExitWithError(const char *message, SDL_Renderer *renderer, SDL_Window *window)
{
  SDL_Log("%s > %s\n", message, SDL_GetError());
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();
  exit(EXIT_FAILURE);
}

curseur creerCurseur(SDL_Renderer *renderer, SDL_Window *window, config_type config)
{
  curseur monCurseur;
  monCurseur.image = NULL;
  monCurseur.texture = NULL;

  if (config.theme == 1)
    monCurseur.image = SDL_LoadBMP("./../img/medieval/curseur.bmp");
  if (config.theme == 2)
    monCurseur.image = SDL_LoadBMP("./../img/starcraft/curseur.bmp");

  if (monCurseur.image == NULL)
  {
    SDL_FreeSurface(monCurseur.image);
    SDL_ExitWithError("ERREUR : Ouverture image curseur", renderer, window);
  }

  monCurseur.texture = SDL_CreateTextureFromSurface(renderer, monCurseur.image);
  SDL_FreeSurface(monCurseur.image);

  if (monCurseur.texture == NULL)
  {
    detruireCurseur(monCurseur);
    SDL_ExitWithError("ERREUR : Création texture curseur", renderer, window);
  }

  if (SDL_QueryTexture(monCurseur.texture, NULL, NULL, &monCurseur.position.w, &monCurseur.position.h) != 0)
  {
    detruireCurseur(monCurseur);
    SDL_ExitWithError("ERREUR : Chargement en mémoire curseur", renderer, window);
  }

  return monCurseur;
}

void afficherCurseur(curseur monCurseur, SDL_Renderer *renderer)
{
  SDL_RenderCopy(renderer, monCurseur.texture, NULL, &monCurseur.position);
}

void detruireCurseur(curseur monCurseur)
{
  SDL_DestroyTexture(monCurseur.texture);
}

void creationBackground(SDL_Renderer *renderer, SDL_Rect *fenetre, config_type config)
{
  SDL_Rect background;
  background.x = 0;
  background.y = 0;
  background.w = fenetre->w;
  background.h = fenetre->h;

  if (config.theme == 1)
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
  if (config.theme == 2)
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255); // à modifié pour starcraft

  SDL_RenderFillRect(renderer, &background);
}

void initPartie(game param_partie)
{
  param_partie->joueurs = 1;
  param_partie->ordis = 1;
  param_partie->niveau = 1;
  param_partie->chrono = false;
}

void plateauDeJeu(SDL_Renderer *renderer, SDL_Window *window, SDL_Rect *fenetre, coord coordClic, coord coordCurseur, bool *inPause, saves_value *partie, config_type config)
{

  char nomDuFichier[500];

  //Affichage du damier ainsi que de son contour
  afficherImage("jeu/damier.bmp", CENTER, CENTER, renderer, window, fenetre, config);
  afficherImage("jeu/damier_fond.bmp", CENTER, CENTER, renderer, window, fenetre, config);

  //Création bouton Pause
  char *bouton_list[2] = {"pause", "finir"};
  menu_bouton selection_hover = rien;

  if (!*inPause)
  {
    if (coordCurseur.x >= 212 && coordCurseur.x <= 338 && coordCurseur.y >= 12 && coordCurseur.y <= 138)
      selection_hover = pause;

    if (coordClic.x >= 212 && coordClic.x <= 338 && coordClic.y >= 12 && coordClic.y <= 138)
      *inPause = true;

    if (partie->move.inTurn && partie->move.firstMove && !partie->move.inBorder)
    {
      if (coordCurseur.x >= 942 && coordCurseur.x <= 1067 && coordCurseur.y >= 12 && coordCurseur.y <= 138)
        selection_hover = finir;

      if (coordClic.x >= 942 && coordClic.x <= 1067 && coordClic.y >= 12 && coordClic.y <= 138)
        partie->move.inTurn = false;
    }
  }

  // Affichage des 2 boutons : pause + finir le tour
  for (int i = 0; i < 2; i++)
  {
    if (i == 0 || (partie->move.inTurn == true && partie->move.firstMove && !partie->move.inBorder))
    {
      int j = 0;

      if (i + 9 == selection_hover)
        j = 1;

      sprintf(nomDuFichier, "jeu/%s_%d.bmp", bouton_list[i], j);
      afficherImage(nomDuFichier, 200 + i * (fenetre->w - 550), 0, renderer, window, fenetre, config);
    }
  }

  // Affichage du joueur à qui c'est le tour
  sprintf(nomDuFichier, "jeu/joueur_%d.bmp", partie->move.player);
  afficherImage(nomDuFichier, CENTER, 0, renderer, window, fenetre, config);

  // On affiche le numéro du tour
  SDL_Color white = {255, 255, 255};
  char nombreTours[3];
  sprintf(nombreTours, "%d", partie->turn);
  afficherImage("jeu/fond_turn.bmp", CENTER, 635, renderer, window, fenetre, config);

  if (partie->turn >= 100)
  {
    afficherTexte(nombreTours, 720, 685, white, 85, renderer, window, fenetre);
  }
  else if (partie->turn >= 10)
  {
    afficherTexte(nombreTours, 760, 685, white, 85, renderer, window, fenetre);
  }
  else
  {
    afficherTexte(nombreTours, 800, 685, white, 85, renderer, window, fenetre);
  }

  // On affiche le temps restant si l'option a été activé
  if (partie->parametre.chrono)
  {
    char secondes[3];
    sprintf(secondes, "%d", partie->time);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_Color time_color;
    time_color.b = 0;
    time_color.a = 125;

    if (partie->time >= 2 * CHRONO / 3)
    {
      time_color.r = (30 - partie->time) * 8;
      time_color.g = 255 - partie->time * 5.75;
    }
    else if (partie->time >= CHRONO / 3)
    {
      time_color.r = 255;
      time_color.g = 140 - partie->time * 14;
    }
    else
    {
      time_color.r = 255;
      time_color.g = 0;
    }

    afficherTexte(secondes, CENTER, (fenetre->h - 525) / 2, time_color, 480, renderer, window, fenetre);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
  }
}

void generatePion(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect plateau, saves_value *partie, config_type config)
{
  /* Génération graphique des pions est de leur subrillance de sélection */
  if (partie->selectedBox.x != -1 && partie->selectedBox.y != -1)
  {
    SDL_Rect surbrillance;
    surbrillance.w = 50;
    surbrillance.h = 50;
    surbrillance.x = (plateau.x + partie->selectedBox.y * 50);
    surbrillance.y = (plateau.y + partie->selectedBox.x * 50);

    switch (partie->tab[partie->selectedBox.x][partie->selectedBox.y].joueur)
    {
    case 1:
      SDL_SetRenderDrawColor(renderer, 220, 20, 60, 255);
      break;

    case 2:
      SDL_SetRenderDrawColor(renderer, 0, 127, 255, 255);
      break;

    case 3:
      SDL_SetRenderDrawColor(renderer, 50, 205, 50, 255);
      break;

    case 4:
      SDL_SetRenderDrawColor(renderer, 255, 228, 54, 255);
      break;
    }
    SDL_RenderFillRect(renderer, &surbrillance);
  }
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      if (partie->tab[i][j].joueur > 0)
        afficherPion(window, renderer, plateau, partie->tab[i][j].joueur, partie->tab[i][j].pion, j, i, config);
    }
  }
}
void afficherPion(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect plateau, int joueur, int pion, int x, int y, config_type config)
{
  /* Affichage d'un pion*/
  char nomDuFichier[500];
  sprintf(nomDuFichier, "jeu/%d_joueur%d.bmp", pion, joueur);
  afficherImage(nomDuFichier, plateau.x + 50 * x, plateau.y + 50 * y, renderer, window, NULL, config);
}

menu_bouton menu_principal(SDL_Renderer *renderer, SDL_Window *window, SDL_Rect *fenetre, coord coordCurseur, coord coordClic, config_type config)
{
  // Affichage du titre principal
  afficherImage("menu_principal/titre.bmp", CENTER, 20, renderer, window, fenetre, config);

  menu_bouton selection_hover = rien;
  menu_bouton selection = rien;

  // On vérifie si l'utilisateur a cliqué ou survollé un bouton
  if (coordCurseur.x >= 465 && coordCurseur.x <= 815)
  {
    if (coordCurseur.y >= 300 && coordCurseur.y <= 350)
      selection_hover = jouer;
    else if (coordCurseur.y >= 375 && coordCurseur.y <= 425)
      selection_hover = rejouer;
    else if (coordCurseur.y >= 450 && coordCurseur.y <= 500)
      selection_hover = regles;
    else if (coordCurseur.y >= 525 && coordCurseur.y <= 575)
      selection_hover = options;
    else if (coordCurseur.y >= 600 && coordCurseur.y <= 650)
      selection_hover = quitter;
    else
      selection_hover = rien;
  }
  else
  {
    selection_hover = rien;
  }

  if (coordClic.x >= 465 && coordClic.x <= 815)
  {
    if (coordClic.y >= 300 && coordClic.y <= 350)
    {
      selection = jouer;
    }
    else if (coordClic.y >= 375 && coordClic.y <= 425)
    {
      selection = rejouer;
    }
    else if (coordClic.y >= 450 && coordClic.y <= 500)
    {
      selection = regles;
    }
    else if (coordClic.y >= 525 && coordClic.y <= 575)
    {
      selection = options;
    }
    else if (coordClic.y >= 600 && coordClic.y <= 650)
    {
      selection = quitter;
    }
    else
    {
      selection = rien;
    }
  }
  else
  {
    selection = rien;
  }

  char *nom_bouton[] = {"jouer", "rejouer", "regles", "options", "quitter"};

  for (int i = 0; i < 5; i++)
  {
    int j = 0;

    char nomDuFichier[500];
    if (i == selection_hover)
      j = 1;

    sprintf(nomDuFichier, "menu_principal/%s_%d.bmp", nom_bouton[i], j);
    // Affichage de chaque bouton
    afficherImage(nomDuFichier, CENTER, 300 + i * 75, renderer, window, fenetre, config);
  }

  return selection;
}

saves menu_secondaire(SDL_Renderer *renderer, SDL_Window *window, SDL_Rect *fenetre, menu_bouton *selection, coord coordCurseur, coord coordClic, saves_value *partie, saves_list *li, fleche *bouton_fleche, config_type *config)
{

  char nomDuFichier[500];

  switch (*selection)
  {
  case jouer:
  {
    // Affichage du titre nombre de joueurs
    afficherImage("menu_secondaire/nbrJoueurs_titre.bmp", CENTER, 30, renderer, window, fenetre, *config);

    // Affichage du paramètre nombre joueurs
    afficherImage("menu_secondaire/nbrJoueurs.bmp", CENTER, 150, renderer, window, fenetre, *config);

    // Gestion nombre de nbrJoueurs

    if (coordClic.y >= 150 && coordClic.y <= 250)
    {
      if (coordClic.x >= (fenetre->w - 532) / 2 && coordClic.x <= (fenetre->w - 532) / 2 + 100 && (partie->parametre.joueurs > 1))
        partie->parametre.joueurs--;

      if (coordClic.x >= (fenetre->w + 500) / 2 - 100 && coordClic.x <= (fenetre->w + 500) / 2 && (partie->parametre.joueurs < 4))
        partie->parametre.joueurs++;
    }

    // Affichage nombre d'ordinateurs

    afficherImage("menu_secondaire/nbrOrdis.bmp", CENTER, 300, renderer, window, fenetre, *config);
    // Gestion nombre d'ordinateurs

    if (coordClic.y >= 300 && coordClic.y <= 400)
    {
      if (coordClic.x >= (fenetre->w - 532) / 2 && coordClic.x <= (fenetre->w - 532) / 2 + 100 && (partie->parametre.ordis > 0))
        partie->parametre.ordis--;

      if (coordClic.x >= (fenetre->w + 500) / 2 - 100 && coordClic.x <= (fenetre->w + 500) / 2 && (partie->parametre.ordis < 4))
        partie->parametre.ordis++;
    }

    if (partie->parametre.ordis == 0 && partie->parametre.joueurs == 1)
      partie->parametre.ordis = 1;

    if (partie->parametre.joueurs + partie->parametre.ordis > 4)
      partie->parametre.ordis = abs(4 - partie->parametre.joueurs);

    // Affichage des valeurs joueurs et ordinateurs
    for (int i = 0; i < 2; i++)
    {
      if (i == 0)
        sprintf(nomDuFichier, "/menu_secondaire/%d.bmp", partie->parametre.joueurs);
      else
        sprintf(nomDuFichier, "/menu_secondaire/%d.bmp", partie->parametre.ordis);

      afficherImage(nomDuFichier, CENTER, 160 + i * 150, renderer, window, fenetre, *config);
    }
    /* Démarrage de la partie */

    menu_bouton selection_hover = rien;

    if (coordCurseur.x >= 465 && coordCurseur.x <= 815)
    {
      if (coordCurseur.y >= 600 && coordCurseur.y <= 650)
        selection_hover = commencer;
      else
        selection_hover = rien;
    }
    else
    {
      selection_hover = rien;
    }

    if (coordClic.x >= 465 && coordClic.x <= 815 && coordClic.y >= 600 && coordClic.y <= 650)
    {
      *selection = commencer;

      // Initialisation du tableau qui sert de plateau de jeu en fonction du nombre de joueur
      initialisationTab(partie->tab, partie->parametre.joueurs + partie->parametre.ordis);
      initialisationCaseVisit(partie->caseVisit);
      initilisationPlayerMove(&partie->move);
      initialisationTabRest(partie->restriction, true);
      initialisationCaseVisit(partie->caseVisit);
      initialisationAlive(partie->joueursEnVie, partie->parametre.joueurs + partie->parametre.ordis);
      partie->turn = 1;
      partie->time = CHRONO;
    }

    // Checkbox tours chronos
    if (coordClic.x >= 390 && coordClic.x <= 890 && coordClic.y >= 525 && coordClic.y <= 575)
      partie->parametre.chrono = !partie->parametre.chrono;

    sprintf(nomDuFichier, "menu_secondaire/chrono_%d.bmp", partie->parametre.chrono);
    afficherImage(nomDuFichier, CENTER, 500, renderer, window, fenetre, *config);

    // Affichage du bouton de démarrage
    if (selection_hover == commencer)
      afficherImage("menu_secondaire/commencer_1.bmp", CENTER, 600, renderer, window, fenetre, *config);
    else
      afficherImage("menu_secondaire/commencer_0.bmp", CENTER, 600, renderer, window, fenetre, *config);
  }
  break;
  case rejouer: // Si l'utilisateur à cliqué sur charger une partie
  {
    // Affichage du titre du menu
    afficherImage("menu_secondaire/sauvegarde_titre.bmp", CENTER, -20, renderer, window, fenetre, *config);

    // création de la liste temporaire pour caculé le nombre d'élément de la liste de sauvegarde
    saves_list *temp_nbrElement = li;
    int nombre_element = 0;
    while (temp_nbrElement != NULL)
    {
      nombre_element++;
      temp_nbrElement = temp_nbrElement->next;
    }

    SDL_Color white = {255, 255, 255};
    if (nombre_element == 0)
    {
      afficherTexte("Aucunes sauvegardes", CENTER, CENTER, white, 50, renderer, window, fenetre);
    }

    // créatOin de fond dynamoques en fonctione du nombre de sauvegardes
    if (nombre_element - bouton_fleche->element + 1 >= 1)
      afficherImage("menu_secondaire/sauvegarde_param.bmp", CENTER, 120, renderer, window, fenetre, *config);
    if (nombre_element - bouton_fleche->element + 1 >= 2)
      afficherImage("menu_secondaire/sauvegarde_param.bmp", CENTER, 280, renderer, window, fenetre, *config);
    if (nombre_element - bouton_fleche->element + 1 >= 3)
      afficherImage("menu_secondaire/sauvegarde_param.bmp", CENTER, 440, renderer, window, fenetre, *config);

    // Création de la liste temporaire permettant de parcourir la liste de sauvegardes
    saves_list *temp = li;

    // Repositionnement du curseur vers la première sauvegarde visé
    int j = 0;
    while (temp != NULL && j < bouton_fleche->element - 1)
    {
      j++;
      temp = temp->next;
    }

    saves_value *ancienne_partie;

    int save_select = 0;
    int choix = 0; //0 aucun 1 jouer 2 supprimer

    // Affichage de chaque valeur correspondant à chaque donné de la sauvegarde
    for (int i = 0; i < 3; i++)
    {
      if (temp != NULL)
      {
        char nbjoueur[10] = "";
        char nbordis[10] = "";
        char nbniveau[10] = "";
        char tour[10] = "";

        sprintf(nbjoueur, "%d", temp->value.parametre.joueurs);
        sprintf(nbordis, "%d", temp->value.parametre.ordis);
        sprintf(nbniveau, "%d", temp->value.parametre.niveau);
        sprintf(tour, "%d", temp->value.turn);

        afficherTexte(temp->value.name, 185, 210 + i * 160, white, 30, renderer, window, fenetre);
        afficherTexte(nbjoueur, 580, 180 + i * 160, white, 50, renderer, window, fenetre);
        afficherTexte(nbordis, 580, 235 + i * 160, white, 50, renderer, window, fenetre);
        afficherTexte(nbniveau, 825, 180 + i * 160, white, 50, renderer, window, fenetre);
        afficherTexte(tour, 825, 235 + i * 160, white, 50, renderer, window, fenetre);

        // Et vérifcation du clic sur jouer ou supprimer pour chaque sauvegarde
        if (coordClic.x >= 915 && coordClic.x <= 1090)
        {
          if (coordCurseur.y >= 180 && coordClic.y <= 230)
          {
            save_select = bouton_fleche->element;
            choix = 1;
          }
          else if (coordCurseur.y >= 235 && coordClic.y <= 285)
          {
            save_select = bouton_fleche->element;
            choix = 2;
          }
          else if (coordCurseur.y >= 340 && coordClic.y <= 390)
          {
            if (temp->next != NULL)
            {
              save_select = bouton_fleche->element + 1;
              choix = 1;
            }
          }
          else if (coordCurseur.y >= 395 && coordClic.y <= 445)
          {
            if (temp->next != NULL)
            {
              choix = 2;
              save_select = bouton_fleche->element + 1;
            }
          }
          else if (coordCurseur.y >= 500 && coordClic.y <= 550)
          {
            if (temp->next != NULL)
            {
              if (temp->next->next != NULL)
              {
                choix = 1;
                save_select = bouton_fleche->element + 2;
              }
            }
          }
          else if (coordCurseur.y >= 555 && coordClic.y <= 1005)
          {
            if (temp->next != NULL)
            {
              if (temp->next->next != NULL)
              {
                choix = 2;
                save_select = bouton_fleche->element + 2;
              }
            }
          }
        }

        temp = temp->next;
      }
    }

    // Si l'utilisateur à cliqué sur jouer ou sur supprimer on repositionne le curseur vers la sauvegarde concerné
    if (choix != 0)
    {
      temp = li;
      j = 0;
      while (temp != NULL && j < save_select - 1)
      {
        j++;
        temp = temp->next;
      }

      if (choix == 1)
      {
        // On reprend la partie
        ancienne_partie = &temp->value;
        *selection = commencer;
      }
      if (choix == 2)
      {
        // On supprime la partie
        li = delete_save(li, temp->value.name);
      }
    }

    // Si on reprend la partie, on copie alors l'ancienne sauvegarde vers l'actuelle
    if (*selection == commencer)
    {
      for (int i = 0; i < 10; i++)
      {
        for (int j = 0; j < 10; j++)
        {
          partie->tab[i][j].joueur = ancienne_partie->tab[i][j].joueur;
          partie->tab[i][j].pion = ancienne_partie->tab[i][j].pion;
          partie->tab[i][j].distinction = ancienne_partie->tab[i][j].distinction;

          for (int k = 0; k < PION_LIST_LENGTH; k++)
          {
            partie->caseVisit[i][j][k].compteur = ancienne_partie->caseVisit[i][j][k].compteur;
          }
        }
      }

      for (int i = 0; i < NAME_LENGTH; i++)
      {
        partie->name[i] = ancienne_partie->name[i];
      }

      partie->turn = ancienne_partie->turn;

      partie->parametre.joueurs = ancienne_partie->parametre.joueurs;
      partie->parametre.ordis = ancienne_partie->parametre.ordis;
      partie->parametre.niveau = ancienne_partie->parametre.niveau;
      partie->parametre.chrono = ancienne_partie->parametre.chrono;

      partie->move.firstMove = ancienne_partie->move.firstMove;
      partie->move.inBorder = ancienne_partie->move.inBorder;
      partie->move.inTurn = ancienne_partie->move.inTurn;
      partie->move.player = ancienne_partie->move.player;
      partie->move.selected = ancienne_partie->move.selected;

      partie->selectedBox.x = ancienne_partie->selectedBox.x;
      partie->selectedBox.y = ancienne_partie->selectedBox.y;

      for (int i = 0; i < 8; i++)
      {
        partie->restriction[i] = ancienne_partie->restriction[i];
      }

      partie->time = ancienne_partie->time;
    }

    if (bouton_fleche->element > 1)
    {
      afficherImage("menu_secondaire/haut.bmp", 150, 115, renderer, window, fenetre, *config);
      if (bouton_fleche->haut)
        bouton_fleche->element--;
    }

    if (bouton_fleche->element < nombre_element - 2)
    {
      afficherImage("menu_secondaire/bas.bmp", 150, 620, renderer, window, fenetre, *config);
      if (bouton_fleche->bas)
        bouton_fleche->element++;
    }
  }
  // Le programme lancera alors la partie au mpeme endroit que l'ancienne
  break;

  case options:
  {
    // Affichafe du titre du menu
    afficherImage("menu_secondaire/config_titre.bmp", CENTER, -20, renderer, window, fenetre, *config);

    // Affichage des trois sous titre correspondant à chaque paramètre
    afficherImage("menu_secondaire/theme_titre.bmp", CENTER, 125, renderer, window, fenetre, *config);
    afficherImage("menu_secondaire/fullscreen_titre.bmp", CENTER, 275, renderer, window, fenetre, *config);
    afficherImage("menu_secondaire/fps_titre.bmp", CENTER, 425, renderer, window, fenetre, *config);

    // Vérification du clic pour le paramètre thème
    if (coordClic.y >= 250 && coordClic.y <= 300)
    {
      if (coordClic.x >= (fenetre->w - 150) / 2 - 175 && coordClic.x <= (fenetre->w - 150) / 2 - 25)
      {
        config->theme = 1;
      }
      else if (coordClic.x >= (fenetre->w - 150) / 2 && coordClic.x <= (fenetre->w - 150) / 2 + 150)
      {
        config->theme = 2;
      }
      else if (coordClic.x >= (fenetre->w - 150) / 2 + 175 && coordClic.x <= (fenetre->w - 150) / 2 + 325)
      {
        config->theme = 3;
      }
    }

    // Affichage du bouton correspondant
    if (config->theme == 1)
    {
      afficherImage("/menu_secondaire/medieval_1.bmp", (fenetre->w - 150) / 2 - 225, 200, renderer, window, fenetre, *config);
      afficherImage("/menu_secondaire/medieval_0.bmp", CENTER, 200, renderer, window, fenetre, *config);                       // à changer pour starcraft
      afficherImage("/menu_secondaire/medieval_0.bmp", (fenetre->w - 150) / 2 + 125, 200, renderer, window, fenetre, *config); // à changer pour troisème thème
    }
    else if (config->theme == 2)
    {
      afficherImage("/menu_secondaire/medieval_0.bmp", (fenetre->w - 150) / 2 - 225, 200, renderer, window, fenetre, *config);
      afficherImage("/menu_secondaire/medieval_1.bmp", CENTER, 200, renderer, window, fenetre, *config);                       // à changer pour starcraft
      afficherImage("/menu_secondaire/medieval_0.bmp", (fenetre->w - 150) / 2 + 125, 200, renderer, window, fenetre, *config); // à changer pour troisème thème
    }
    else
    {
      afficherImage("/menu_secondaire/medieval_0.bmp", (fenetre->w - 150) / 2 - 225, 200, renderer, window, fenetre, *config);
      afficherImage("/menu_secondaire/medieval_1.bmp", CENTER, 200, renderer, window, fenetre, *config);                       // à changer pour starcraft
      afficherImage("/menu_secondaire/medieval_0.bmp", (fenetre->w - 150) / 2 + 125, 200, renderer, window, fenetre, *config); // à changer pour troisème thème
    }

    // Vérification du clique correspondant au paramètre plein écran et fps
    if (coordClic.x >= (fenetre->w - 150) / 2 && coordClic.x <= (fenetre->w - 150) / 2 + 150)
    {
      if (coordClic.y >= 400 && coordClic.y <= 450)
      {
        config->fullscreen = !config->fullscreen;
      }
      else if (coordClic.y >= 550 && coordClic.y <= 600)
      {
        config->fps = !config->fps;
      }
    }

    // Afficahge du bouton correspondant
    if (config->fullscreen)
      afficherImage("/menu_secondaire/valide_1.bmp", CENTER, 350, renderer, window, fenetre, *config);
    else
      afficherImage("/menu_secondaire/valide_0.bmp", CENTER, 350, renderer, window, fenetre, *config);

    if (config->fps)
      afficherImage("/menu_secondaire/valide_1.bmp", CENTER, 500, renderer, window, fenetre, *config);
    else
      afficherImage("/menu_secondaire/valide_0.bmp", CENTER, 500, renderer, window, fenetre, *config);

    SDL_Color white = {255, 255, 255};
    afficherTexte("Veuillez redemarrer le jeu pour que les modifications prennent effet.", CENTER, 625, white, 30, renderer, window, fenetre);
  }
  break;
  case regles:
  {
    afficherImage("menu_secondaire/regles_titre.bmp", CENTER, -20, renderer, window, fenetre, *config);
  }
  break;
  default:
    break;
  }
  // Bouton retour
  menu_bouton selection_hover = rien;

  if (coordCurseur.x >= 465 && coordCurseur.x <= 815)
  {
    if (coordCurseur.y >= 675 && coordCurseur.y <= 725)
      selection_hover = retour;
    else
      selection_hover = rien;
  }
  else
  {
    selection_hover = rien;
  }
  if (coordClic.x >= 465 && coordClic.x <= 815)
  {
    if (coordCurseur.y >= 675 && coordClic.y <= 725)
      *selection = rien;
  }
  if (selection_hover == retour)
    afficherImage("menu_secondaire/retour_1.bmp", CENTER, 675, renderer, window, fenetre, *config);
  else
    afficherImage("menu_secondaire/retour_0.bmp", CENTER, 675, renderer, window, fenetre, *config);

  return li;
}

location menu_pause(SDL_Renderer *renderer, SDL_Window *window, SDL_Rect *fenetre, coord coordCurseur, coord coordClic, bool *inPause, saves_value *partie, config_type config)
{

  char nomDuFichier[500];
  menu_bouton selection_hover = rien;
  location loc = inGame;

  // Passage du curseur sur les boutons
  if (coordCurseur.x >= 465 && coordCurseur.x <= 815)
  {
    if (coordCurseur.y >= 400 && coordCurseur.y <= 450)
      selection_hover = quitter;
    else if (coordCurseur.y >= 325 && coordCurseur.y <= 375)
      selection_hover = sauvegarder;
    else if (coordCurseur.y >= 250 && coordCurseur.y <= 300)
      selection_hover = continuer;
    else
      selection_hover = rien;
  }
  else
  {
    selection_hover = rien;
  }
  // Clic sur les boutons
  if (coordClic.x >= 465 && coordClic.x <= 815)
  {
    if (coordClic.y >= 400 && coordClic.y <= 450)
    {
      initPartie(&partie->parametre);
      initilisationPlayerMove(&partie->move);
      *inPause = false;
      loc = inMenu;
    }

    else if (coordClic.y >= 325 && coordClic.y <= 375)
    {
      loc = inSave;
    }

    else if (coordClic.y >= 250 && coordClic.y <= 300)
    {
      *inPause = false;
    }
  }

  // Affichage des boutons
  char *nom_bouton[] = {"quitter", "sauvegarder", "continuer"};
  for (int i = 0; i < 3; i++)
  {
    int j = 0;

    if (i + 4 == selection_hover)
      j = 1;

    sprintf(nomDuFichier, "jeu/%s_%d.bmp", nom_bouton[i], j);
    afficherImage(nomDuFichier, CENTER, 400 - i * 75, renderer, window, fenetre, config);
  }

  return loc;
}

void initNomSauvegarde(nom_sauvegarde *save_name)
{
  for (int i = 0; i < NAME_LENGTH; i++)
  {
    save_name->nom[i] = ' ';
  }
  save_name->lettre = -1;
  save_name->position = 0;
}

location menu_save(SDL_Renderer *renderer, SDL_Window *window, SDL_Rect *fenetre, cell tab[10][10], coord coordCurseur, coord coordClic, bool *inPause, nom_sauvegarde *save_name, bool *saving, config_type config)
{

  location loc = inSave;
  // Affichage du titre du menu
  afficherImage("jeu/sauvegarder_titre.bmp", CENTER, 0, renderer, window, fenetre, config);

  // Affichage du rectangle blanc qui permet d'écrire
  SDL_Rect form;
  form.w = 400;
  form.h = 50;
  form.x = (fenetre->w - form.w) / 2;
  form.y = (fenetre->w - form.w) / 2 - 150;
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &form);

  // Si l'utilisateur à appuyer sur une lettre
  if (save_name->lettre >= 0)
  {
    // Cela correspond à supprimer
    if (save_name->lettre == 26 && save_name->position > 0)
    {
      save_name->position--;
      save_name->nom[save_name->position] = ' ';
    }
    // Sinon et si il y a encore de la place
    if (save_name->lettre < 26 && save_name->position < NAME_LENGTH)
    {
      char alphabet[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
      save_name->nom[save_name->position] = alphabet[save_name->lettre];
      save_name->position++;
    }

    // On remet la lettre à son état initial
    save_name->lettre = -1;
  }

  // On affiche le text correspondant au mot que l'utilisateur est en train de marqué
  SDL_Color black = {0, 0, 0};
  afficherTexte(save_name->nom, form.x, form.y - 5, black, 50, renderer, window, fenetre);

  menu_bouton selection_hover = rien;
  // Vérificatin du clic et survolage des boutons du menu
  if (coordCurseur.x >= 465 && coordCurseur.x <= 815)
  {
    if (coordCurseur.y >= 480 && coordCurseur.y <= 530)
    {
      selection_hover = quitter;
    }
    else if (coordCurseur.y >= 405 && coordCurseur.y <= 465)
    {
      selection_hover = sauvegarder;
    }
    else
    {
      selection_hover = rien;
    }
  }
  else
  {
    selection_hover = rien;
  }

  // Clic sur les boutons
  if (coordClic.x >= 465 && coordClic.x <= 815)
  {
    if (coordClic.y >= 480 && coordClic.y <= 530)
    {
      loc = inGame;
    }
    else if (coordClic.y >= 405 && coordClic.y <= 465)
    {
      *saving = true;
    }
  }

  // Affichage des boutons
  char nomDuFichier[500];
  char *nom_bouton[] = {"quitter", "sauvegarder"};
  for (int i = 0; i < 2; i++)
  {
    int j = 0;

    if (i + 4 == selection_hover)
      j = 1;

    sprintf(nomDuFichier, "jeu/%s_%d.bmp", nom_bouton[i], j);
    afficherImage(nomDuFichier, CENTER, 480 - i * 75, renderer, window, fenetre, config);
  }

  SDL_Color white = {255, 255, 255};
  afficherTexte("ATTENTION, si le nom de votre sauvegarde existe deja,", CENTER, 600, white, 30, renderer, window, fenetre);
  afficherTexte("l'ancienne sera ecrase par la nouvelle", CENTER, 650, white, 30, renderer, window, fenetre);

  return loc;
}

void limit_fps(unsigned int limit, SDL_Rect *fenetre, SDL_Window *window, SDL_Renderer *renderer, config_type config)
{
  //On récupère tick actuel
  unsigned int tick = SDL_GetTicks();
  unsigned int periode = 1000 / FRAME_PER_SECOND;

  char fps_text[3];

  // Si notre ancienne mesure est plus petite : on est en retard -> aucune limitation
  if (limit < tick)
  {
    sprintf(fps_text, "%d", 0);
  }
  else if (limit > tick + periode) // Si on est en avance d'une pariode
  {
    sprintf(fps_text, "%d", 1000 / periode);
    SDL_Delay(periode);
  }
  else // Sinon
  {
    sprintf(fps_text, "%d", 1000 / (limit - tick));
    SDL_Delay(limit - tick);
  }
  if (config.fps) // Affichage des fps en fonction de la configuration
  {
    SDL_Color white = {255, 255, 255};
    afficherTexte(fps_text, fenetre->w - 30, 5, white, 25, renderer, window, NULL);
  }
}
void afficherTexte(char *text, float x, float y, SDL_Color color, int taille, SDL_Renderer *renderer, SDL_Window *window, SDL_Rect *fenetre)
{

  // Création des pointeurs vers la surface de l'image et de sa texture
  SDL_Surface *surfaceTexte = NULL;
  SDL_Texture *textureTexte = NULL;

  // Récupératoin de la police
  TTF_Font *font = TTF_OpenFont("arial.ttf", taille);

  // Vérification que sa récupération s'est bien passée
  if (font == NULL)
  {
    SDL_ExitWithError("Récupération police", renderer, window);
  }

  surfaceTexte = TTF_RenderText_Blended(font, text, color);
  if (surfaceTexte == NULL)
  {
    SDL_ExitWithError("Création surface police", renderer, window);
  }

  textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
  SDL_FreeSurface(surfaceTexte);

  // Vérification de la création de la texture
  if (textureTexte == NULL)
  {
    SDL_DestroyTexture(textureTexte);
    SDL_ExitWithError("Creation texture echouee", renderer, window);
  }

  SDL_Rect positionTexte;

  // Chargement en mémoire de la texture
  if (SDL_QueryTexture(textureTexte, NULL, NULL, &positionTexte.w, &positionTexte.h) != 0)
  {
    SDL_DestroyTexture(textureTexte);
    SDL_ExitWithError("Chargement en memoire texture", renderer, window);
  }

  // Positionnement du texte
  if (x == CENTER)
    positionTexte.x = (fenetre->w - positionTexte.w) / 2;
  else
    positionTexte.x = x;
  if (y == CENTER)
    positionTexte.y = (fenetre->h - positionTexte.h) / 2;
  else
    positionTexte.y = y;

  // Envoi de la texture au renderer
  SDL_RenderCopy(renderer, textureTexte, NULL, &positionTexte);

  // Fermeture de la police
  TTF_CloseFont(font);

  // Destruction de la texture
  SDL_DestroyTexture(textureTexte);
}
void afficherImage(char *lienImage, float x, float y, SDL_Renderer *renderer, SDL_Window *window, SDL_Rect *fenetre, config_type config)
{
  char lien_Theme[100];
  // détermination thème à partir de la config
  if (config.theme == 1)
    sprintf(lien_Theme, "./../img/medieval/%s", lienImage);
  if (config.theme == 2)
    sprintf(lien_Theme, "./../img/starcraft/%s", lienImage);

  // Création des pointeurs vers la surface de l'image et de sa texture
  SDL_Surface *surfaceImage = NULL;
  SDL_Texture *textureImage = NULL;

  // Récupératoin de l'image
  surfaceImage = SDL_LoadBMP(lien_Theme);

  // Vérification que sa récupération s'est bien passée
  if (surfaceImage == NULL)
  {
    SDL_FreeSurface(surfaceImage);
    SDL_ExitWithError("Creation image echouee", renderer, window);
  }

  // Création de la texture de l'image à partir de l'image et libération de la surface
  textureImage = SDL_CreateTextureFromSurface(renderer, surfaceImage);
  SDL_FreeSurface(surfaceImage);

  // Vérification de la création de la texture
  if (textureImage == NULL)
  {
    SDL_DestroyTexture(textureImage);
    SDL_ExitWithError("Creation texture echouee", renderer, window);
  }

  // Création du rectangle contenant l'image
  SDL_Rect positionImage;

  // Chargement en mémoire de la texture
  if (SDL_QueryTexture(textureImage, NULL, NULL, &positionImage.w, &positionImage.h) != 0)
  {
    SDL_DestroyTexture(textureImage);
    SDL_ExitWithError("Chargement en memoire texture", renderer, window);
  }

  // Position de la texture
  if (x == CENTER)
    positionImage.x = (fenetre->w - positionImage.w) / 2;
  else
    positionImage.x = x;
  if (y == CENTER)
    positionImage.y = (fenetre->h - positionImage.h) / 2;
  else
    positionImage.y = y;

  // Envoi de la texture au renderer
  SDL_RenderCopy(renderer, textureImage, NULL, &positionImage);

  // Destruction de la texture
  SDL_DestroyTexture(textureImage);
}

void FiltreDeplacement(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect plateau, saves_value *partie)
{
  if (partie->move.inTurn)
  {
    for (int i = -2; i < 3; i++)
    {
      for (int j = -2; j < 3; j++)
      {
        coordInt *coord_temp;
        coord_temp = malloc(sizeof(*coord_temp));
        coord_temp->x = partie->selectedBox.x + i;
        coord_temp->y = partie->selectedBox.y + j;
        if (coord_temp->x <= 9 && coord_temp->y <= 9 && coord_temp->x >= 0 && coord_temp->y >= 0 && partie->tab[coord_temp->x][coord_temp->y].joueur == 0)
        {

          /* Calcul de la position du pion d'arrivé en fonction d'un autre
            (-1,1) ou (1,1) ou (-1,0) etc...
            Cela permettera de savoir si le saut est possible par la suite.
            Le code fonctionne très bien si l'utilisateur veut sauter ou pas
            car il fait la partie entière de sa division par 2.
          */
          int *i_temp;
          int *j_temp;

          i_temp = malloc(sizeof(int));
          j_temp = malloc(sizeof(int));

          *i_temp = 0;
          *j_temp = 0;
          float *distance_i_temp;
          float *distance_j_temp;

          distance_i_temp = malloc(sizeof(float));
          distance_j_temp = malloc(sizeof(float));
          *distance_i_temp = fabs((coord_temp->x - partie->selectedBox.x) / 2.0);
          *distance_j_temp = fabs((coord_temp->y - partie->selectedBox.y) / 2.0);

          int *signe_i_temp;
          int *signe_j_temp;

          signe_i_temp = malloc(sizeof(int));
          signe_j_temp = malloc(sizeof(int));
          *signe_i_temp = (coord_temp->x - partie->selectedBox.x) / (2.0 * *distance_i_temp);
          *signe_j_temp = (coord_temp->y - partie->selectedBox.y) / (2.0 * *distance_j_temp);

          if (*distance_i_temp <= 1)
            *i_temp = ceil(*distance_i_temp) * *signe_i_temp;
          else
            *i_temp = floor(*distance_i_temp) * *signe_i_temp;

          if (*distance_j_temp <= 1)
            *j_temp = ceil(*distance_j_temp) * *signe_j_temp;
          else
            *j_temp = floor(*distance_j_temp) * *signe_j_temp;

          // Si la case pourra être jouer par l'utilisateur alors on affiche la surbrillance
          if (autorisation(partie->selectedBox.x, partie->selectedBox.y, coord_temp->x, coord_temp->y, partie->tab) && partie->restriction[elementCorrespondant(*i_temp, *j_temp)] && autorisation_bord(partie, partie->selectedBox, *coord_temp, true))
          {
            SDL_Rect surbrillance;
            surbrillance.w = 50;
            surbrillance.h = 50;
            surbrillance.x = (plateau.x + coord_temp->y * 50);
            surbrillance.y = (plateau.y + coord_temp->x * 50);
            SDL_SetRenderDrawColor(renderer, 176, 176, 176, 255);
            SDL_RenderFillRect(renderer, &surbrillance);
          }
          free(i_temp);
          free(j_temp);

          free(distance_i_temp);
          free(distance_j_temp);

          free(signe_i_temp);
          free(signe_j_temp);
        }
        free(coord_temp);
      }
    }
  }
}
