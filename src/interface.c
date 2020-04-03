#include "interface.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_rotozoom.h>

#include <time.h>
#include <math.h>


void initTab(cell tab[10][10]){
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      tab[i][j].joueur = 0;
      tab[i][j].pion = 0;
    }
  }
}

// Fonction de retour d'erreur permet fermeture propre de la fenêtre
void SDL_ExitWithError(const char *message, SDL_Renderer *renderer, SDL_Window *window){
  SDL_Log("ERREUR : %s > %s\n", message,SDL_GetError());
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

void creationBackground(SDL_Renderer *renderer, SDL_Rect fenetre){
  SDL_Rect background;
  background.x = 0;
  background.y = 0;
  background.w = fenetre.w;
  background.h = fenetre.h;

  SDL_SetRenderDrawColor(renderer,40,40,40,255);
  SDL_RenderFillRect(renderer,&background);
}

void initPartie(game param_partie){
  param_partie->joueurs = 1;
  param_partie->ordis = 1;
}
SDL_Rect creationFond(SDL_Renderer *renderer,SDL_Window *window,SDL_Rect fenetre,coord coordClic, coord coordCurseur, bool *inPause){


  SDL_Surface *imageDamier = NULL;
  SDL_Texture *textureDamier = NULL;


  imageDamier= SDL_LoadBMP("./../img/damier.bmp");
  if(imageDamier == NULL){
    SDL_FreeSurface(imageDamier);
    SDL_ExitWithError("Creation imageDamier echouee",renderer,window);
  }

  textureDamier = SDL_CreateTextureFromSurface(renderer, imageDamier);
  SDL_FreeSurface(imageDamier);

  if(textureDamier == NULL){
    SDL_DestroyTexture(textureDamier);
    SDL_ExitWithError("Creation texture echouee",renderer,window);
  }


  SDL_Rect positionDamier;


  if(SDL_QueryTexture(textureDamier,NULL,NULL,&positionDamier.w,&positionDamier.h) != 0){
    SDL_DestroyTexture(textureDamier);
    SDL_ExitWithError("Chargement en memoire texture",renderer,window);
  }


  positionDamier.x = (fenetre.w - positionDamier.w)/2 ;
  positionDamier.y = (fenetre.h - positionDamier.h)/2 ;

  SDL_RenderCopy(renderer,textureDamier,NULL,&positionDamier);
  SDL_DestroyTexture(textureDamier);

  SDL_Surface *imageDamierFond = NULL;
  SDL_Texture *textureDamierFond = NULL;


  imageDamierFond= SDL_LoadBMP("./../img/damier_fond.bmp");
  if(imageDamierFond == NULL){
    SDL_FreeSurface(imageDamierFond);
    SDL_ExitWithError("Creation imageDamier echouee",renderer,window);
  }

  textureDamierFond = SDL_CreateTextureFromSurface(renderer, imageDamierFond);
  SDL_FreeSurface(imageDamierFond);

  if(textureDamierFond == NULL){
    SDL_DestroyTexture(textureDamierFond);
    SDL_ExitWithError("Creation texture echouee",renderer,window);
  }


  SDL_Rect positionDamierFond;


  if(SDL_QueryTexture(textureDamierFond,NULL,NULL,&positionDamierFond.w,&positionDamierFond.h) != 0){
    SDL_DestroyTexture(textureDamierFond);
    SDL_ExitWithError("Chargement en memoire texture",renderer,window);
  }


  positionDamierFond.x = (fenetre.w - positionDamierFond.w)/2 + 20;
  positionDamierFond.y = (fenetre.h - positionDamierFond.h)/2 + 21;

  SDL_RenderCopy(renderer,textureDamierFond,NULL,&positionDamierFond);
  SDL_DestroyTexture(textureDamierFond);

  //Création bouton Pause
  menu_bouton selection_hover = rien;

  if(!*inPause){
    if (coordCurseur.x >= 465 && coordCurseur.x <= 815 && coordCurseur.y >= 700 && coordCurseur.y <= 750)
      selection_hover = pause;

    if (coordClic.x >= 465 && coordClic.x <= 815 && coordClic.y >= 700 && coordClic.y <= 750)
      *inPause = true;
  }
  SDL_Surface *imageBouton = NULL;
  SDL_Texture *textureBouton = NULL;

  int j = 0;

  char nomDuFichier[500];
  if (pause == selection_hover)
     j = 1;

  sprintf(nomDuFichier,"./../img/jeu/pause_%d.bmp",j);

  imageBouton = SDL_LoadBMP(nomDuFichier);
  if(imageBouton == NULL){
    SDL_FreeSurface(imageBouton);
    SDL_ExitWithError("Creation image_bouton echouee",renderer,window);
  }

  textureBouton = SDL_CreateTextureFromSurface(renderer, imageBouton);
  SDL_FreeSurface(imageBouton);

  if(textureBouton == NULL){
    SDL_DestroyTexture(textureBouton);
    SDL_ExitWithError("Creation texture_bouton echouee",renderer,window);
  }

  SDL_Rect emplacementBouton;

  if(SDL_QueryTexture(textureBouton,NULL,NULL,&emplacementBouton.w,&emplacementBouton.h) != 0){
    SDL_DestroyTexture(textureBouton);
    SDL_ExitWithError("Chargement en memoire texture_bouton",renderer,window);
  }

  emplacementBouton.x =  (fenetre.w - emplacementBouton.w) /2;
  emplacementBouton.y = 700;

  SDL_RenderCopy(renderer,textureBouton,NULL,&emplacementBouton);
  SDL_DestroyTexture(textureBouton);


  return positionDamier;
}

void generatePion(SDL_Window *window, SDL_Renderer *renderer,SDL_Rect plateau,cell tab[10][10]){
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      if(tab[i][j].joueur > 0)
        afficherPion(window,renderer,plateau,tab[i][j].joueur,tab[i][j].pion,i,j);
    }
  }
}

void afficherPion(SDL_Window *window, SDL_Renderer *renderer,SDL_Rect plateau,int joueur,int pion,int x,int y){
  SDL_Surface *imagePion = NULL;
  SDL_Texture *texturePion = NULL;

  char nomDuFichier[500];
  sprintf(nomDuFichier,"./../img/%d_joueur%d.png",pion,joueur);

  imagePion = IMG_Load(nomDuFichier);

  if(imagePion == NULL){
    SDL_FreeSurface(imagePion);
    SDL_ExitWithError("Creation image Pion echouee",renderer,window);
  }

  texturePion = SDL_CreateTextureFromSurface(renderer, imagePion);
  SDL_FreeSurface(imagePion);

  if(texturePion == NULL){
    SDL_DestroyTexture(texturePion);
    SDL_ExitWithError("Creation texture Pion echouee",renderer,window);
  }

  SDL_Rect RectPion;

  if(SDL_QueryTexture(texturePion,NULL,NULL,&RectPion.w,&RectPion.h) != 0){
    SDL_DestroyTexture(texturePion);
    SDL_ExitWithError("Chargement en memoire texture",renderer,window);
  }

  RectPion.x = plateau.x + 50 * x;
  RectPion.y = plateau.y + 50 * y;

  SDL_RenderCopy(renderer,texturePion,NULL,&RectPion);
  SDL_DestroyTexture(texturePion);
}

coordInt selectionPion(cell tab[10][10], float x, float y, SDL_Rect plateau){
  coordInt selectedPion;
  selectedPion.x = -1;
  selectedPion.y = -1;
  if(x >= plateau.x && x < plateau.x + plateau.w && y >= plateau.y && y < plateau.x + plateau.h){
    int i,j;
    i = floor((x - plateau.x)/50);
    j = floor((y - plateau.y)/50);

    if(tab[i][j].joueur != 0){
      selectedPion.x = i;
      selectedPion.y = j;
    }
  }
  return selectedPion;
}

void selectionMove(cell tab[10][10], float x, float y, SDL_Rect plateau, coordInt *movedPion){

  if(x >= plateau.x && x < plateau.x + plateau.w && y >= plateau.y && y < plateau.x + plateau.h){
    int i,j;
    i = floor((x - plateau.x )/50);
    j = floor((y - plateau.y)/50);
    if(tab[i][j].joueur == 0){

      tab[i][j].joueur = tab[movedPion->x][movedPion->y].joueur;
      tab[i][j].pion = tab[movedPion->x][movedPion->y].pion;
      tab[movedPion->x][movedPion->y].joueur = 0;
      tab[movedPion->x][movedPion->y].pion = 0;
    }
  }
}

menu_bouton menu_principal(SDL_Renderer *renderer,SDL_Window *window, SDL_Rect fenetre, coord coordCurseur, coord coordClic){

  SDL_Surface *imageTitre = NULL;
  SDL_Texture *textureTitre = NULL;

  imageTitre = SDL_LoadBMP("./../img/menu_principal/titre.bmp");

  if(imageTitre == NULL){
    SDL_FreeSurface(imageTitre);
    SDL_ExitWithError("Creation imageTitre echouee",renderer,window);
  }

  textureTitre = SDL_CreateTextureFromSurface(renderer, imageTitre);
  SDL_FreeSurface(imageTitre);

  if(textureTitre == NULL){
    SDL_DestroyTexture(textureTitre);
    SDL_ExitWithError("Creation texture echouee",renderer,window);
  }

  SDL_Rect emplacementTitre;

  if(SDL_QueryTexture(textureTitre,NULL,NULL,&emplacementTitre.w,&emplacementTitre.h) != 0){
    SDL_DestroyTexture(textureTitre);
    SDL_ExitWithError("Chargement en memoire texture",renderer,window);
  }

  emplacementTitre.x =  (fenetre.w - emplacementTitre.w)/2;
  emplacementTitre.y = 20;

  SDL_RenderCopy(renderer,textureTitre,NULL,&emplacementTitre);
  SDL_DestroyTexture(textureTitre);

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

  SDL_Surface *imageBouton = NULL;
  SDL_Texture *textureBouton = NULL;

  int j = 0;

  char nomDuFichier[500];
  if (i == selection_hover)
     j = 1;

  sprintf(nomDuFichier,"./../img/menu_principal/%s_%d.bmp",nom_bouton[i],j);

  imageBouton = SDL_LoadBMP(nomDuFichier);
  if(imageBouton == NULL){
    SDL_FreeSurface(imageBouton);
    SDL_ExitWithError("Creation image_bouton echouee",renderer,window);
  }

  textureBouton = SDL_CreateTextureFromSurface(renderer, imageBouton);
  SDL_FreeSurface(imageBouton);

  if(textureBouton == NULL){
    SDL_DestroyTexture(textureBouton);
    SDL_ExitWithError("Creation texture_bouton echouee",renderer,window);
  }

  SDL_Rect emplacementBouton;

  if(SDL_QueryTexture(textureBouton,NULL,NULL,&emplacementBouton.w,&emplacementBouton.h) != 0){
    SDL_DestroyTexture(textureBouton);
    SDL_ExitWithError("Chargement en memoire texture_bouton",renderer,window);
  }

  emplacementBouton.x =  (fenetre.w - emplacementBouton.w) /2;
  emplacementBouton.y = 300 + i*75;

  SDL_RenderCopy(renderer,textureBouton,NULL,&emplacementBouton);
  SDL_DestroyTexture(textureBouton);
}



  return selection;
}

location menu(SDL_Rect fenetre, SDL_Window *window, SDL_Renderer *renderer, game param_partie){

  location loc = inMenu;

  curseur monCurseur = creerCurseur(renderer,window);
  coord coordClic;
  coord coordMouse;

  coordClic.x = 0;
  coordClic.y = 0;

  coordMouse.x = 0;
  coordMouse.y = 0;

  menu_bouton selection = rien;
  unsigned int frame_limit = 0;

  frame_limit = SDL_GetTicks() + FRAME_PER_SECOND;

    while (loc == inMenu) {
      SDL_Event  event;


      SDL_RenderClear(renderer);
      creationBackground(renderer,fenetre);

      if (selection == rien)
        selection = menu_principal(renderer,window,fenetre,coordMouse,coordClic);


      if (selection != rien && selection != quitter)
        selection = menu_secondaire(renderer,window,fenetre, selection,coordMouse,coordClic, param_partie);

      afficherCurseur(monCurseur,renderer);
      frame_limit = SDL_GetTicks() + FRAME_PER_SECOND;
      limit_fps(frame_limit,fenetre,window,renderer);
      frame_limit = SDL_GetTicks() + FRAME_PER_SECOND;

      SDL_RenderPresent(renderer);

      // Réinitalisation des coordonnées du clic pour évité boucles
      coordClic.x = 0;
      coordClic.y = 0;

        while (SDL_PollEvent(&event)) {
          switch (event.type) {
            case SDL_QUIT:
              selection = quitter;
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
                selection = quitter;
            break;

            default:
            break;
          }
        }


        if (selection == quitter)
          loc = quit;
        if (selection == commencer)
          loc = inGame;
    }
    detruireCurseur(monCurseur);




    return loc;
}

menu_bouton menu_secondaire(SDL_Renderer *renderer, SDL_Window *window, SDL_Rect fenetre, menu_bouton selection, coord coordCurseur, coord coordClic, game param_partie){
menu_bouton newSelection = rien;
  switch (selection) {
    case jouer:{
        // Affichage du titre nombre de joueurs
        SDL_Surface *imageNbrJoueursTitre = NULL;
        SDL_Texture *textureNbrJoueursTitre = NULL;

        imageNbrJoueursTitre = SDL_LoadBMP("./../img/menu_secondaire/nbrJoueurs_titre.bmp");

        if(imageNbrJoueursTitre == NULL){
          SDL_FreeSurface(imageNbrJoueursTitre);
          SDL_ExitWithError("Creation image start echouee",renderer,window);
        }

        textureNbrJoueursTitre = SDL_CreateTextureFromSurface(renderer, imageNbrJoueursTitre);
        SDL_FreeSurface(imageNbrJoueursTitre);

        if(textureNbrJoueursTitre == NULL){
          SDL_DestroyTexture(textureNbrJoueursTitre);
          SDL_ExitWithError("Creation texture start echouee",renderer,window);
        }

        SDL_Rect emplacementNbrJoueursTitre;

        if(SDL_QueryTexture(textureNbrJoueursTitre,NULL,NULL,&emplacementNbrJoueursTitre.w,&emplacementNbrJoueursTitre.h) != 0){
          SDL_DestroyTexture(textureNbrJoueursTitre);
          SDL_ExitWithError("Chargement en memoire start checkbox ",renderer,window);
        }

        emplacementNbrJoueursTitre.x =  (fenetre.w - emplacementNbrJoueursTitre.w)/2 + 26;
        emplacementNbrJoueursTitre.y = 30;

        SDL_RenderCopy(renderer,textureNbrJoueursTitre,NULL,&emplacementNbrJoueursTitre);
        SDL_DestroyTexture(textureNbrJoueursTitre);

        // Affichage du paramètre nombre joueurs
        SDL_Surface *imageNbrJoueurs = NULL;
        SDL_Texture *textureNbrJoueurs = NULL;

        imageNbrJoueurs = SDL_LoadBMP("./../img/menu_secondaire/nbrJoueurs.bmp");

        if(imageNbrJoueurs == NULL){
          SDL_FreeSurface(imageNbrJoueurs);
          SDL_ExitWithError("Creation image start echouee",renderer,window);
        }

        textureNbrJoueurs = SDL_CreateTextureFromSurface(renderer, imageNbrJoueurs);
        SDL_FreeSurface(imageNbrJoueurs);

        if(textureNbrJoueurs == NULL){
          SDL_DestroyTexture(textureNbrJoueurs);
          SDL_ExitWithError("Creation texture start echouee",renderer,window);
        }

        SDL_Rect emplacementNbrJoueurs;

        if(SDL_QueryTexture(textureNbrJoueurs,NULL,NULL,&emplacementNbrJoueurs.w,&emplacementNbrJoueurs.h) != 0){
          SDL_DestroyTexture(textureNbrJoueurs);
          SDL_ExitWithError("Chargement en memoire start checkbox ",renderer,window);
        }

        emplacementNbrJoueurs.x =  (fenetre.w - emplacementNbrJoueurs.w)/2 + 16;
        emplacementNbrJoueurs.y = 150;

        SDL_RenderCopy(renderer,textureNbrJoueurs,NULL,&emplacementNbrJoueurs);
        SDL_DestroyTexture(textureNbrJoueurs);

        // Gestion nombre de nbrJoueurs

        if (coordClic.y >= 150 && coordClic.y <= 250) {
            if(coordClic.x >= (fenetre.w- 532)/2 && coordClic.x <= (fenetre.w- 532)/2 + 100 && (param_partie->joueurs > 1))
              param_partie->joueurs--;

            if(coordClic.x >= (fenetre.w + 500)/2 - 100 && coordClic.x <= (fenetre.w + 500)/2 && (param_partie->joueurs < 4 ))
              param_partie->joueurs++;
        }

        // Affichage nombre d'ordinateurs
        SDL_Surface *imageNbrOrdis = NULL;
        SDL_Texture *textureNbrOrdis = NULL;

        imageNbrOrdis = SDL_LoadBMP("./../img/menu_secondaire/nbrOrdis.bmp");

        if(imageNbrOrdis == NULL){
          SDL_FreeSurface(imageNbrOrdis);
          SDL_ExitWithError("Creation image start echouee",renderer,window);
        }

        textureNbrOrdis = SDL_CreateTextureFromSurface(renderer, imageNbrOrdis);
        SDL_FreeSurface(imageNbrOrdis);

        if(textureNbrOrdis == NULL){
          SDL_DestroyTexture(textureNbrOrdis);
          SDL_ExitWithError("Creation texture start echouee",renderer,window);
        }

        SDL_Rect emplacementNbrOrdis;

        if(SDL_QueryTexture(textureNbrOrdis,NULL,NULL,&emplacementNbrOrdis.w,&emplacementNbrOrdis.h) != 0){
          SDL_DestroyTexture(textureNbrOrdis);
          SDL_ExitWithError("Chargement en memoire start checkbox ",renderer,window);
        }

        emplacementNbrOrdis.x =  (fenetre.w - emplacementNbrOrdis.w)/2 + 16;
        emplacementNbrOrdis.y = 300;

        SDL_RenderCopy(renderer,textureNbrOrdis,NULL,&emplacementNbrOrdis);
        SDL_DestroyTexture(textureNbrOrdis);

        // Gestion nombre d'ordinateurs

        if (coordClic.y >= 300 && coordClic.y <= 400 ) {
            if(coordClic.x >= (fenetre.w- 532)/2 && coordClic.x <= (fenetre.w- 532)/2 + 100 && (param_partie->ordis > 0))
              param_partie->ordis--;

            if(coordClic.x >= (fenetre.w + 500)/2 - 100 && coordClic.x <= (fenetre.w + 500)/2 && (param_partie->ordis < 4 ))
              param_partie->ordis++;
        }

        if(param_partie->ordis == 0 && param_partie->joueurs == 1)
          param_partie->ordis = 1;

        if(param_partie->joueurs + param_partie->ordis > 4)
          param_partie->ordis = abs( 4 - param_partie->joueurs );

        // Affichage des valeurs joueurs et ordinateurs

        for (int i = 0; i<2; i++){

          SDL_Surface *imageValeur = NULL;
          SDL_Texture *textureValeur = NULL;

          char nomDuFichier[500];
        if (i == 0)
          sprintf(nomDuFichier,"./../img/menu_secondaire/%d.bmp",param_partie->joueurs);
        else
          sprintf(nomDuFichier,"./../img/menu_secondaire/%d.bmp",param_partie->ordis);

          imageValeur = SDL_LoadBMP(nomDuFichier);
          if(imageValeur == NULL){
            SDL_FreeSurface(imageValeur);
            SDL_ExitWithError("Creation image_bouton echouee",renderer,window);
          }

          textureValeur = SDL_CreateTextureFromSurface(renderer, imageValeur);
          SDL_FreeSurface(imageValeur);

          if(textureValeur == NULL){
            SDL_DestroyTexture(textureValeur);
            SDL_ExitWithError("Creation texture_bouton echouee",renderer,window);
          }

          SDL_Rect emplacementValeur;

          if(SDL_QueryTexture(textureValeur,NULL,NULL,&emplacementValeur.w,&emplacementValeur.h) != 0){
            SDL_DestroyTexture(textureValeur);
            SDL_ExitWithError("Chargement en memoire texture_bouton",renderer,window);
          }

          emplacementValeur.x =  (fenetre.w - emplacementValeur.w) /2;
          if (i == 0)
            emplacementValeur.y = 160;
          else
            emplacementValeur.y = 310;
          SDL_RenderCopy(renderer,textureValeur,NULL,&emplacementValeur);
          SDL_DestroyTexture(textureValeur);

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

        SDL_Surface *imageBouton = NULL;
        SDL_Texture *textureBouton = NULL;

        int j = 0;

        char nomDuFichier[500];
        if (i+6 == selection_hover)
           j = 1;

        sprintf(nomDuFichier,"./../img/menu_secondaire/%s_%d.bmp",nom_bouton[i],j);

        imageBouton = SDL_LoadBMP(nomDuFichier);
        if(imageBouton == NULL){
          SDL_FreeSurface(imageBouton);
          SDL_ExitWithError("Creation image_bouton echouee",renderer,window);
        }

        textureBouton = SDL_CreateTextureFromSurface(renderer, imageBouton);
        SDL_FreeSurface(imageBouton);

        if(textureBouton == NULL){
          SDL_DestroyTexture(textureBouton);
          SDL_ExitWithError("Creation texture_bouton echouee",renderer,window);
        }

        SDL_Rect emplacementBouton;

        if(SDL_QueryTexture(textureBouton,NULL,NULL,&emplacementBouton.w,&emplacementBouton.h) != 0){
          SDL_DestroyTexture(textureBouton);
          SDL_ExitWithError("Chargement en memoire texture_bouton",renderer,window);
        }

        emplacementBouton.x =  (fenetre.w - emplacementBouton.w) /2;
        emplacementBouton.y = 600 + i*75;

        SDL_RenderCopy(renderer,textureBouton,NULL,&emplacementBouton);
        SDL_DestroyTexture(textureBouton);
      }




    break;
  }
    default:
    break;
  }

  return newSelection;

}

location jeu(SDL_Rect fenetre,SDL_Window *window, SDL_Renderer *renderer,cell tab[10][10]){

  location loc= inGame;
  curseur monCurseur = creerCurseur(renderer,window);
  bool inPause = false;

  coordInt selectedBox;
  selectedBox.x = -1;
  selectedBox.y = -1;

  coord coordClic;
  coord coordCurseur;

  SDL_Rect plateau;

  SDL_bool program_launched = SDL_TRUE;

  unsigned int frame_limit = 0;


  while(program_launched) {
      SDL_Event event;
      coordClic.x = 0;
      coordClic.x = 0;
      while(SDL_PollEvent(&event)) {
          switch (event.type) {
            case SDL_QUIT:
              loc = quit;
              break;
            case SDL_KEYDOWN:
              if (event.key.keysym.sym == SDLK_ESCAPE)
                inPause = !inPause;

              break;
            case SDL_MOUSEBUTTONDOWN:
              if(!inPause){
                if(selectedBox.x < 0 || selectedBox.y < 0){
                  selectedBox = selectionPion(tab,event.button.x,event.button.y, plateau);

                }
                  else {
                  selectionMove(tab,event.button.x,event.button.y, plateau, &selectedBox);
                  selectedBox.x = -1;
                  selectedBox.y = -1;
                }
              }
              coordClic.x = event.button.x;
              coordClic.y = event.button.y;
              break;
            case SDL_MOUSEMOTION:
              monCurseur.position.x = event.motion.x;
              monCurseur.position.y = event.motion.y;
              coordCurseur.x = monCurseur.position.x;
              coordCurseur.y = monCurseur.position.y;

            break;
            default:
              break;
          }
        }
      // Clear screen
      SDL_RenderClear(renderer);
      // Draw
      creationBackground(renderer,fenetre);
      plateau = creationFond(renderer,window,fenetre,coordClic,coordCurseur, &inPause);
      generatePion(window,renderer,plateau,tab);
      if(inPause){
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(renderer, NULL);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        loc = menu_pause(renderer,window,fenetre, coordCurseur,coordClic, &inPause);
      }

      afficherCurseur(monCurseur,renderer);
      // Show what was drawn
      frame_limit = SDL_GetTicks() + FRAME_PER_SECOND;
      limit_fps(frame_limit,fenetre,window,renderer);
      frame_limit = SDL_GetTicks() + FRAME_PER_SECOND;

      SDL_RenderPresent(renderer);

      if(loc != inGame)
        program_launched = SDL_FALSE;

  }
  detruireCurseur(monCurseur);
  return loc;
}


location menu_pause(SDL_Renderer *renderer, SDL_Window *window, SDL_Rect fenetre,coord coordCurseur, coord coordClic, bool *inPause){

  menu_bouton selection_hover = rien;
  location loc = inGame;
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
  if (coordClic.x >= 465 && coordClic.x <= 815) {
    if (coordClic.y >= 400 && coordClic.y <= 450)
      loc = inMenu;
    //else if(coordClic.y >= 325 && coordClic.y <= 375)

    else if (coordClic.y >= 250 && coordClic.y <= 300)
      *inPause = false;

  }

  char *nom_bouton[] ={"quitter","sauvegarder","continuer"};

  for (int i = 0; i < 3; i++) {

  SDL_Surface *imageBouton = NULL;
  SDL_Texture *textureBouton = NULL;

  int j = 0;

  char nomDuFichier[500];
  if (i+3 == selection_hover)
     j = 1;

  sprintf(nomDuFichier,"./../img/jeu/%s_%d.bmp",nom_bouton[i],j);

  imageBouton = SDL_LoadBMP(nomDuFichier);
  if(imageBouton == NULL){
    SDL_FreeSurface(imageBouton);
    SDL_ExitWithError("Creation image_bouton echouee",renderer,window);
  }

  textureBouton = SDL_CreateTextureFromSurface(renderer, imageBouton);
  SDL_FreeSurface(imageBouton);

  if(textureBouton == NULL){
    SDL_DestroyTexture(textureBouton);
    SDL_ExitWithError("Creation texture_bouton echouee",renderer,window);
  }

  SDL_Rect emplacementBouton;

  if(SDL_QueryTexture(textureBouton,NULL,NULL,&emplacementBouton.w,&emplacementBouton.h) != 0){
    SDL_DestroyTexture(textureBouton);
    SDL_ExitWithError("Chargement en memoire texture_bouton",renderer,window);
  }

  emplacementBouton.x =  (fenetre.w - emplacementBouton.w) /2;
  emplacementBouton.y = 400 - i*75;

  SDL_RenderCopy(renderer,textureBouton,NULL,&emplacementBouton);
  SDL_DestroyTexture(textureBouton);
}


  return loc;

}
void limit_fps(unsigned int limit, SDL_Rect fenetre,SDL_Window *window, SDL_Renderer *renderer){
  unsigned int tick = SDL_GetTicks();

  if(limit < tick)
    return;
  else if (limit > tick + FRAME_PER_SECOND)
    SDL_Delay(16);
  else
    SDL_Delay(limit - tick);

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

  positionFPS.x = fenetre.w - positionFPS.w - 5;
  positionFPS.y = 5;

  SDL_RenderCopy(renderer, textureFPS, NULL, &positionFPS);
  TTF_CloseFont(font);
  SDL_DestroyTexture(textureFPS);

}
