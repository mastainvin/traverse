#include "interface.h"
#include "FonctionsDuJeu.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>




// Fonction de retour d'erreur permet fermeture propre de la fenêtre
void SDL_ExitWithError(const char *message, SDL_Renderer *renderer, SDL_Window *window){
  SDL_Log("%s > %s\n", message,SDL_GetError());
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();
  exit(EXIT_FAILURE);
}

curseur creerCurseur(SDL_Renderer *renderer, SDL_Window *window){
  curseur monCurseur;
  monCurseur.image = NULL;
  monCurseur.texture = NULL;
  monCurseur.image = SDL_LoadBMP("./../img/curseur.bmp");

  if (monCurseur.image == NULL){
    SDL_FreeSurface(monCurseur.image);
    SDL_ExitWithError("ERREUR : Ouverture image curseur",renderer, window);
  }

  monCurseur.texture = SDL_CreateTextureFromSurface(renderer,monCurseur.image);
  SDL_FreeSurface(monCurseur.image);

  if (monCurseur.texture == NULL){
    detruireCurseur(monCurseur);
    SDL_ExitWithError("ERREUR : Création texture curseur", renderer, window);
  }

  if (SDL_QueryTexture(monCurseur.texture,NULL,NULL,&monCurseur.position.w,&monCurseur.position.h) != 0){
    detruireCurseur(monCurseur);
    SDL_ExitWithError("ERREUR : Chargement en mémoire curseur", renderer, window);
  }

  return monCurseur;
}

void afficherCurseur(curseur monCurseur, SDL_Renderer *renderer){
  SDL_RenderCopy(renderer,monCurseur.texture,NULL,&monCurseur.position);
}

void detruireCurseur(curseur monCurseur){
  SDL_DestroyTexture(monCurseur.texture);
}

void creationBackground(SDL_Renderer *renderer, SDL_Rect *fenetre){
  SDL_Rect background;
  background.x = 0;
  background.y = 0;
  background.w = fenetre->w;
  background.h = fenetre->h;

  SDL_SetRenderDrawColor(renderer,40,40,40,255);
  SDL_RenderFillRect(renderer,&background);
}

void initPartie(game param_partie){
  param_partie->joueurs = 1;
  param_partie->ordis = 1;
}

void creationFond(SDL_Renderer *renderer,SDL_Window *window,SDL_Rect *fenetre,coord coordClic, coord coordCurseur, bool *inPause, playerMove *move){

  char nomDuFichier[500];

  //Affichage du damier ainsi que de son contour
  afficherImage("./../img/damier.bmp",CENTER,CENTER,renderer,window,fenetre);
  afficherImage("./../img/damier_fond.bmp",CENTER,CENTER,renderer,window,fenetre);

  //Création bouton Pause
  char *bouton_list[2] = {"pause", "finir"};
  menu_bouton selection_hover = rien;

  if(!*inPause){
    if (coordCurseur.x >= 465 && coordCurseur.x <= 815 && coordCurseur.y >= 740 && coordCurseur.y <= 790)
      selection_hover = pause;

    if (coordClic.x >= 465 && coordClic.x <= 815 && coordClic.y >= 740 && coordClic.y <= 790)
      *inPause = true;

    if(move->inTurn && move->firstMove){
      if (coordCurseur.x >= 465 && coordCurseur.x <= 815 && coordCurseur.y >= 685 && coordCurseur.y <= 735)
        selection_hover = finir;

      if (coordClic.x >= 465 && coordClic.x <= 815 && coordClic.y >= 685 && coordClic.y <= 735)
        move->inTurn = false;
    }
  }

  // Affichage des 2 boutons : pause + finir le tour
  for(int i = 0; i < 2; i++){
  if(i == 0 || (move->inTurn == true && move->firstMove)){
    int j = 0;

    if (i+8 == selection_hover)
       j = 1;

    sprintf(nomDuFichier,"./../img/jeu/%s_%d.bmp",bouton_list[i],j);
    afficherImage(nomDuFichier,CENTER,740 - i*55,renderer,window,fenetre);
  }
}

  // Affichage du joueur à qui c'est le tour
  sprintf(nomDuFichier,"./../img/jeu/joueur_%d.bmp",move->player);
  afficherImage(nomDuFichier,CENTER,0,renderer,window,fenetre);

}

void generatePion(SDL_Window *window, SDL_Renderer *renderer,SDL_Rect plateau,cell tab[10][10]){
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      if(tab[i][j].joueur > 0)
        afficherPion(window,renderer,plateau,tab[i][j].joueur,tab[i][j].pion,j,i);
    }
  }
}

void afficherPion(SDL_Window *window, SDL_Renderer *renderer,SDL_Rect plateau,int joueur,int pion,int x,int y){
  char nomDuFichier[500];
  sprintf(nomDuFichier,"./../img/jeu/%d_joueur%d.bmp",pion,joueur);
  afficherImage(nomDuFichier,plateau.x + 50 * x,plateau.y + 50 * y,renderer,window,NULL);
}

coordInt selectionPion(cell tab[10][10], float x, float y, SDL_Rect plateau){
  coordInt selectedPion;
  selectedPion.x = -1;
  selectedPion.y = -1;

  if(x >= plateau.x && x < plateau.x + plateau.w && y >= plateau.y && y < plateau.x + plateau.h){
    int i,j;
    j = floor((x - plateau.x)/50);
    i = floor((y - plateau.y)/50);

    if(tab[i][j].joueur != 0){
      selectedPion.x = i;
      selectedPion.y = j;
    }
  }
  return selectedPion;
}

coordInt selectionMove(cell tab[10][10], float x, float y, SDL_Rect plateau){

  coordInt selectedPion;
  selectedPion.x = -1;
  selectedPion.y = -1;

  if(x >= plateau.x && x < plateau.x + plateau.w && y >= plateau.y && y < plateau.x + plateau.h){
    int i,j;
    j = floor((x - plateau.x )/50);
    i = floor((y - plateau.y)/50);
    if(tab[i][j].joueur == 0){
      selectedPion.x = i;
      selectedPion.y = j;
    }
  }
  return selectedPion;
}

menu_bouton menu_principal(SDL_Renderer *renderer,SDL_Window *window, SDL_Rect *fenetre, coord coordCurseur, coord coordClic){
  // Affichage du titre principal
  afficherImage("./../img/menu_principal/titre.bmp",CENTER,20,renderer,window,fenetre);

  menu_bouton selection_hover = rien;
  menu_bouton selection = rien;

  if (coordCurseur.x >= 465 && coordCurseur.x <= 815) {
    if (coordCurseur.y >= 300 && coordCurseur.y <= 350)
      selection_hover = jouer;
    else if(coordCurseur.y >= 375 && coordCurseur.y <= 425)
      selection_hover = regles;
    else if (coordCurseur.y >= 450 && coordCurseur.y <= 500)
      selection_hover = options;
    else if (coordCurseur.y >= 525 && coordCurseur.y <= 575)
      selection_hover = quitter;
    else
      selection_hover = rien;
  }
  else{
    selection_hover = rien;
  }

  if (coordClic.x >= 465 && coordClic.x <= 815){
    if (coordClic.y >= 300 && coordClic.y <= 350){
      selection = jouer;
    }else if(coordClic.y >= 375 && coordClic.y <= 425){
      selection = regles;
    }else if (coordClic.y >= 450 && coordClic.y <= 500){
      selection = options;
    } else if (coordClic.y >= 525 && coordClic.y <= 575){
      selection = quitter;
    } else {
      selection = rien;
    }
    }else {
    selection = rien;
    }


  char *nom_bouton[] ={"jouer","regles","options","quitter"};

  for (int i = 0; i < 4; i++) {
  int j = 0;

  char nomDuFichier[500];
  if (i == selection_hover)
     j = 1;

  sprintf(nomDuFichier,"./../img/menu_principal/%s_%d.bmp",nom_bouton[i],j);
  // Affichage de chaque bouton
  afficherImage(nomDuFichier,CENTER,300 + i*75,renderer,window,fenetre);
}

  return selection;
}

menu_bouton menu_secondaire(SDL_Renderer *renderer, SDL_Window *window, SDL_Rect *fenetre, menu_bouton selection, coord coordCurseur, coord coordClic, game param_partie){

menu_bouton newSelection = rien;
char nomDuFichier[500];

  switch (selection) {
    case jouer:{
        // Affichage du titre nombre de joueurs
        afficherImage("./../img/menu_secondaire/nbrJoueurs_titre.bmp",CENTER,30,renderer,window,fenetre);

        // Affichage du paramètre nombre joueurs
        afficherImage("./../img/menu_secondaire/nbrJoueurs.bmp",CENTER,150,renderer,window,fenetre);

        // Gestion nombre de nbrJoueurs

        if (coordClic.y >= 150 && coordClic.y <= 250) {
            if(coordClic.x >= (fenetre-> w - 532)/2 && coordClic.x <= (fenetre->w- 532)/2 + 100 && (param_partie->joueurs > 1))
              param_partie->joueurs--;

            if(coordClic.x >= (fenetre->w + 500)/2 - 100 && coordClic.x <= (fenetre->w + 500)/2 && (param_partie->joueurs < 4 ))
              param_partie->joueurs++;
        }

        // Affichage nombre d'ordinateurs

        afficherImage("./../img/menu_secondaire/nbrOrdis.bmp",CENTER,300,renderer,window,fenetre);
        // Gestion nombre d'ordinateurs

        if (coordClic.y >= 300 && coordClic.y <= 400 ) {
            if(coordClic.x >= (fenetre->w- 532)/2 && coordClic.x <= (fenetre->w- 532)/2 + 100 && (param_partie->ordis > 0))
              param_partie->ordis--;

            if(coordClic.x >= (fenetre->w + 500)/2 - 100 && coordClic.x <= (fenetre->w + 500)/2 && (param_partie->ordis < 4 ))
              param_partie->ordis++;
        }

        if(param_partie->ordis == 0 && param_partie->joueurs == 1)
          param_partie->ordis = 1;

        if(param_partie->joueurs + param_partie->ordis > 4)
          param_partie->ordis = abs( 4 - param_partie->joueurs );

        // Affichage des valeurs joueurs et ordinateurs
        for (int i = 0; i<2; i++){
          if (i == 0)
            sprintf(nomDuFichier,"./../img/menu_secondaire/%d.bmp",param_partie->joueurs);
          else
            sprintf(nomDuFichier,"./../img/menu_secondaire/%d.bmp",param_partie->ordis);

          afficherImage(nomDuFichier,CENTER,160 + i*150,renderer,window,fenetre);
        }
        /* Démarrage de la partie */

        menu_bouton selection_hover = rien;
        newSelection = jouer;


        if (coordCurseur.x >= 465 && coordCurseur.x <= 815) {
          if (coordCurseur.y >= 600 && coordCurseur.y <= 650)
            selection_hover =  commencer;
          else if(coordCurseur.y >= 675 && coordCurseur.y <= 725)
            selection_hover = retour;
          else
            selection_hover = rien;
        }
        else{
          selection_hover = rien;
        }

        if (coordClic.x >= 465 && coordClic.x <= 815) {
          if (coordClic.y >= 600 && coordClic.y <= 650)
            newSelection = commencer;
          else if(coordCurseur.y >= 675 && coordClic.y <= 725)
            newSelection = rien;
        }

        // Création du bouton de démarrage


        char *nom_bouton[] ={"commencer", "retour"};


        for (int i = 0; i < 2; i++) {
        int j = 0;
        if (i+6 == selection_hover)
           j = 1;

        sprintf(nomDuFichier,"./../img/menu_secondaire/%s_%d.bmp",nom_bouton[i],j);
        afficherImage(nomDuFichier,CENTER,600 + i*75,renderer,window,fenetre);
      }

    break;
  }
    default:
    break;
  }

  return newSelection;

}

location menu_pause(SDL_Renderer *renderer, SDL_Window *window, SDL_Rect *fenetre,coord coordCurseur, coord coordClic, bool *inPause){

  char nomDuFichier[500];
  menu_bouton selection_hover = rien;
  location loc = inGame;

  // Passage du curseur sur les boutons
  if (coordCurseur.x >= 465 && coordCurseur.x <= 815) {
    if (coordCurseur.y >= 400 && coordCurseur.y <= 450)
      selection_hover = quitter;
    else if(coordCurseur.y >= 325 && coordCurseur.y <= 375)
      selection_hover = sauvegarder;
    else if (coordCurseur.y >= 250 && coordCurseur.y <= 300)
      selection_hover = continuer;
    else
      selection_hover = rien;
  }
  else{
    selection_hover = rien;
  }
  // Clic sur les boutons
  if (coordClic.x >= 465 && coordClic.x <= 815) {
    if (coordClic.y >= 400 && coordClic.y <= 450)
      loc = inMenu;
    //else if(coordClic.y >= 325 && coordClic.y <= 375)

    else if (coordClic.y >= 250 && coordClic.y <= 300)
      *inPause = false;

  }

  // Affichage des boutons
  char *nom_bouton[] ={"quitter","sauvegarder","continuer"};
  for (int i = 0; i < 3; i++) {
    int j = 0;

    if (i+3 == selection_hover)
       j = 1;

    sprintf(nomDuFichier,"./../img/jeu/%s_%d.bmp",nom_bouton[i],j);
    afficherImage(nomDuFichier,CENTER,400 - i*75,renderer,window,fenetre);
  }


  return loc;

}

void limit_fps(unsigned int limit, SDL_Rect *fenetre,SDL_Window *window, SDL_Renderer *renderer){
  unsigned int tick = SDL_GetTicks();



  SDL_Surface *surfaceFPS = NULL;
  SDL_Texture *textureFPS = NULL;

  TTF_Font *font = TTF_OpenFont("arial.ttf", 25);

  SDL_Color white = {255,255,255};
  char fps_text[3];

  sprintf(fps_text,"%d",1000/(limit-tick));

  surfaceFPS = TTF_RenderText_Solid(font,fps_text,white);
  textureFPS = SDL_CreateTextureFromSurface(renderer, surfaceFPS);
  SDL_FreeSurface(surfaceFPS);

  SDL_Rect positionFPS;
  SDL_QueryTexture(textureFPS,NULL,NULL,&positionFPS.w, &positionFPS.h);

  positionFPS.x = fenetre->w - positionFPS.w - 5;
  positionFPS.y = 5;

  SDL_RenderCopy(renderer, textureFPS, NULL, &positionFPS);
  TTF_CloseFont(font);
  SDL_DestroyTexture(textureFPS);

  if(limit < tick)
    return;
  else if (limit > tick + FRAME_PER_SECOND)
    SDL_Delay(16);
  else
    SDL_Delay(limit - tick);

}


void afficherImage(char *lienImage,float x, float y, SDL_Renderer *renderer, SDL_Window *window, SDL_Rect *fenetre){

    // Création des pointeurs vers la surface de l'image et de sa texture
    SDL_Surface *surfaceImage = NULL;
    SDL_Texture *textureImage = NULL;

    // Récupératin de l'image
    surfaceImage = SDL_LoadBMP(lienImage);

    // Vérification que sa récupération s'est bien passée
    if(surfaceImage == NULL){
      SDL_FreeSurface(surfaceImage);
      SDL_ExitWithError("Creation image echouee",renderer,window);
    }

    // Création de la texture de l'image à partir de l'image et libération de la surface
    textureImage = SDL_CreateTextureFromSurface(renderer, surfaceImage);
    SDL_FreeSurface(surfaceImage);

    // Vérification de la créati de la texture
    if(textureImage == NULL){
      SDL_DestroyTexture(textureImage);
      SDL_ExitWithError("Creation texture echouee",renderer,window);
    }

    // Création du rectangle contenant l'image
    SDL_Rect positionImage;

    // Chargement en mémoire de la texture
    if(SDL_QueryTexture(textureImage,NULL,NULL,&positionImage.w,&positionImage.h) != 0){
      SDL_DestroyTexture(textureImage);
      SDL_ExitWithError("Chargement en memoire texture",renderer,window);
    }

    // Position de la texture
    if(x == CENTER)
      positionImage.x = (fenetre->w - positionImage.w)/2;
    else
      positionImage.x = x;
    if(y == CENTER)
      positionImage.y = (fenetre->h - positionImage.h)/2;
    else
      positionImage.y = y;

    // Envoi de la texture au renderer
    SDL_RenderCopy(renderer,textureImage,NULL,&positionImage);

    // Destruction de la texture
    SDL_DestroyTexture(textureImage);

}
