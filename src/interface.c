#include "interface.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

void initTabJoueur(joueur tab[4]){
  for (int i = 0; i < 4; i++) {
    tab[i].joue = false;
    tab[i].humain = true;
  }
}
void creationFond(SDL_Renderer *renderer,SDL_Rect plateau,SDL_Rect fenetre,SDL_Texture *textureDamier){


  SDL_Rect damierShadow;
  damierShadow.x = plateau.x - 21;
  damierShadow.y = plateau.y - 21;
  damierShadow.w = plateau.w + 42;
  damierShadow.h = plateau.h + 42;

  SDL_SetRenderDrawColor(renderer,0,0,0,1);
  SDL_RenderDrawRect(renderer,&damierShadow);

  SDL_Rect contour;
  contour.x = plateau.x - 20;
  contour.y = plateau.y - 20;
  contour.w = plateau.w + 40;
  contour.h = plateau.h + 40;

  SDL_SetRenderDrawColor(renderer,206,206,206,255);
  SDL_RenderFillRect(renderer,&contour);


  SDL_RenderCopy(renderer,textureDamier,NULL,&plateau);
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

  if (coordCurseur.x >= 425 && coordCurseur.x <= 775) {
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

  if (coordClic.x >= 425 && coordClic.x <= 775){
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

location menu(SDL_Rect fenetre, SDL_Window *window, SDL_Renderer *renderer, SDL_Rect plateau, joueur tableauJoueurs[4]){

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


      limit_fps(frame_limit);
      SDL_RenderClear(renderer);
      creationBackground(renderer,fenetre);

      if (selection == rien)
        selection = menu_principal(renderer,window,fenetre,coordMouse,coordClic);


      if (selection != rien && selection != quitter)
        loc = menu_secondaire(renderer,window,fenetre, selection,coordMouse,coordClic, tableauJoueurs);

      afficherCurseur(monCurseur,renderer);
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
        frame_limit = SDL_GetTicks() + FRAME_PER_SECOND;

        if (selection == quitter)
          loc = quit;
    }
    detruireCurseur(monCurseur);




    return loc;
}

location menu_secondaire(SDL_Renderer *renderer, SDL_Window *window, SDL_Rect fenetre, menu_bouton selection, coord coordCurseur, coord coordClic, joueur tab[4]){
  location loc = inMenu;

  switch (selection) {
    case jouer:
        //int nbreJoueurs;

        /* ----- checkbox -----*/
        // Affichage des checkbox
        for ( int i = 0; i < 4; i++) {
            char nomDuFichier[500];

            sprintf(nomDuFichier,"./../img/menu_secondaire/checkbox_%d.bmp",tab[i].joue);

            SDL_Surface *imageCheck = NULL;
            SDL_Texture *textureCheck = NULL;

            imageCheck = SDL_LoadBMP(nomDuFichier);

            if(imageCheck == NULL){
              SDL_FreeSurface(imageCheck);
              SDL_ExitWithError("Creation image checkbox echouee",renderer,window);
            }

            textureCheck = SDL_CreateTextureFromSurface(renderer, imageCheck);
            SDL_FreeSurface(imageCheck);

            if(textureCheck == NULL){
              SDL_DestroyTexture(textureCheck);
              SDL_ExitWithError("Creation texture checkbox echouee",renderer,window);
            }

            SDL_Rect emplacementCheck;

            if(SDL_QueryTexture(textureCheck,NULL,NULL,&emplacementCheck.w,&emplacementCheck.h) != 0){
              SDL_DestroyTexture(textureCheck);
              SDL_ExitWithError("Chargement en memoire texture checkbox ",renderer,window);
            }

            emplacementCheck.x =  (fenetre.w - emplacementCheck.w)/2 - 450;
            emplacementCheck.y = 160 + i * 60;

            SDL_RenderCopy(renderer,textureCheck,NULL,&emplacementCheck);
            SDL_DestroyTexture(textureCheck);
          }

        // Mise à jour des checkbox
        if (coordClic.x >= (fenetre.w - 30)/2 - 450 && coordClic.x <= (fenetre.w - 30)/2 - 420){
        for (int i = 0; i < 4; i++) {
              if (coordClic.y >= 160 + i * 60 && coordClic.y <= 190 + i * 60){
                if(tab[i].joue == true)
                  tab[i].joue = false;
                else
                  tab[i].joue = true;
              }
            }
          }

        /* ----- checkbox -----*/
        // Affichage des boutons
        for ( int i = 0; i < 4; i++) {
            char nomDuFichier1[500];
            char nomDuFichier2[500];

            sprintf(nomDuFichier1,"./../img/menu_secondaire/humain_%d.bmp",tab[i].humain);
            sprintf(nomDuFichier2,"./../img/menu_secondaire/ia_%d.bmp",!tab[i].humain);

            SDL_Surface *imageHumain = NULL;
            SDL_Texture *textureHumain = NULL;
            SDL_Surface *imageIA = NULL;
            SDL_Texture *textureIA = NULL;

            imageHumain = SDL_LoadBMP(nomDuFichier1);
            imageIA = SDL_LoadBMP(nomDuFichier2);

            if(imageHumain == NULL){
              SDL_FreeSurface(imageHumain);
              SDL_ExitWithError("Creation image humain bouton echouee",renderer,window);
            }

            if(imageIA == NULL){
              SDL_FreeSurface(imageIA);
              SDL_ExitWithError("Creation image ia bouton echouee",renderer,window);
            }

            textureHumain = SDL_CreateTextureFromSurface(renderer, imageHumain);
            SDL_FreeSurface(imageHumain);

            textureIA = SDL_CreateTextureFromSurface(renderer, imageIA);
            SDL_FreeSurface(imageIA);

            if(textureHumain == NULL){
              SDL_DestroyTexture(textureHumain);
              SDL_ExitWithError("Creation texture humain bouton echouee",renderer,window);
            }

            if(textureIA == NULL){
              SDL_DestroyTexture(textureIA);
              SDL_ExitWithError("Creation texture ia bouton echouee",renderer,window);
            }

            SDL_Rect emplacementHumain;
            SDL_Rect emplacementIA;

            if(SDL_QueryTexture(textureHumain,NULL,NULL,&emplacementHumain.w,&emplacementHumain.h) != 0){
              SDL_DestroyTexture(textureHumain);
              SDL_ExitWithError("Chargement en memoire texture humain bouton ",renderer,window);
            }

            if(SDL_QueryTexture(textureIA,NULL,NULL,&emplacementIA.w,&emplacementIA.h) != 0){
              SDL_DestroyTexture(textureIA);
              SDL_ExitWithError("Chargement en memoire texture ia bouton ",renderer,window);
            }

            emplacementHumain.x =  (fenetre.w - emplacementHumain.w)/2;
            emplacementHumain.y = 150 + i * 60;

            emplacementIA.x =  (fenetre.w - emplacementIA.w)/2 + 130;
            emplacementIA.y = 150 + i * 60;

            SDL_RenderCopy(renderer,textureHumain,NULL,&emplacementHumain);
            SDL_DestroyTexture(textureHumain);

            SDL_RenderCopy(renderer,textureIA,NULL,&emplacementIA);
            SDL_DestroyTexture(textureIA);

          }

        // Mise à jour des boutons
        if (coordClic.x >= (fenetre.w - 120)/2  && coordClic.x <= (fenetre.w - 120)/2 + 120 ){
        for (int i = 0; i < 4; i++) {
              if (coordClic.y >= 150 + i * 60 && coordClic.y <= 200 + i * 60){
                if(tab[i].humain == false)
                  tab[i].humain = true;
              }
            }
          }

          if (coordClic.x >= (fenetre.w - 120)/2  + 130 && coordClic.x <= (fenetre.w - 120)/2 + 250 ){
          for (int i = 0; i < 4; i++) {
                if (coordClic.y >= 150 + i * 60 && coordClic.y <= 200 + i * 60){
                  if(tab[i].humain == true)
                    tab[i].humain = false;

                }
              }
            }
        /* Démarrage de la partie */

        // Création du bouton de démarrage
        SDL_Surface *imageStart = NULL;
        SDL_Texture *textureStart = NULL;

        imageStart = SDL_LoadBMP("./../img/menu_secondaire/commencer.bmp");

        if(imageStart == NULL){
          SDL_FreeSurface(imageStart);
          SDL_ExitWithError("Creation image checkbox echouee",renderer,window);
        }

        textureStart = SDL_CreateTextureFromSurface(renderer, imageStart);
        SDL_FreeSurface(imageStart);

        if(textureStart == NULL){
          SDL_DestroyTexture(textureStart);
          SDL_ExitWithError("Creation texture checkbox echouee",renderer,window);
        }

        SDL_Rect emplacementStart;

        if(SDL_QueryTexture(textureStart,NULL,NULL,&emplacementStart.w,&emplacementStart.h) != 0){
          SDL_DestroyTexture(textureStart);
          SDL_ExitWithError("Chargement en memoire texture checkbox ",renderer,window);
        }

        emplacementStart.x =  (fenetre.w - emplacementStart.w)/2  + 30;
        emplacementStart.y = 500;

        SDL_RenderCopy(renderer,textureStart,NULL,&emplacementStart);
        SDL_DestroyTexture(textureStart);

        // Mise à jour du bouton de démarrage

        if (coordClic.x >= (fenetre.w - 500)/2  +30 && coordClic.x <= (fenetre.w - 500)/2 + 530  && coordClic.y >= 500 && coordClic.y <= 700)
          loc = inGame;


    break;

    default:
    break;
  }

  return loc;

}

location jeu(SDL_Rect plateau,SDL_Rect fenetre,SDL_Window *window, SDL_Renderer *renderer,cell tab[10][10]){

  location loc= inGame;
  curseur monCurseur = creerCurseur(renderer,window);
  SDL_Surface *imageDamier = NULL;
  SDL_Texture *textureDamier = NULL;

  coordInt selectedBox;
  selectedBox.x = -1;
  selectedBox.y = -1;

  imageDamier= IMG_Load("./../img/damier 500X500.png");
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


  if(SDL_QueryTexture(textureDamier,NULL,NULL,&plateau.w,&plateau.h) != 0){
    SDL_DestroyTexture(textureDamier);
    SDL_ExitWithError("Chargement en memoire texture",renderer,window);
  }

  SDL_bool program_launched = SDL_TRUE;

  unsigned int frame_limit = 0;

  frame_limit = SDL_GetTicks() + FRAME_PER_SECOND;

  while(program_launched) {
      SDL_Event event;

      while(SDL_PollEvent(&event)) {
          switch (event.type) {
            case SDL_QUIT:
              loc = quit;
              program_launched = SDL_FALSE;
              break;
            case SDL_KEYDOWN:
              if (event.key.keysym.sym == SDLK_ESCAPE)
                loc = quit;
                program_launched = SDL_FALSE;
              break;
            case SDL_MOUSEBUTTONDOWN:
              if(selectedBox.x < 0 || selectedBox.y < 0){
                selectedBox = selectionPion(tab,event.button.x,event.button.y, plateau);

              }
                else {
                selectionMove(tab,event.button.x,event.button.y, plateau, &selectedBox);
                selectedBox.x = -1;
                selectedBox.y = -1;
              }
              break;
            case SDL_MOUSEMOTION:
              monCurseur.position.x = event.motion.x;
              monCurseur.position.y = event.motion.y;
            break;
            default:
              break;
          }
        }
      limit_fps(frame_limit);
      // Clear screen
      SDL_RenderClear(renderer);
      // Draw
      creationBackground(renderer,fenetre);
      creationFond(renderer,plateau,fenetre,textureDamier);
      generatePion(window,renderer,plateau,tab);
      afficherCurseur(monCurseur,renderer);
      // Show what was drawn
      SDL_RenderPresent(renderer);
      frame_limit = SDL_GetTicks() + FRAME_PER_SECOND;

  }
  detruireCurseur(monCurseur);
  SDL_DestroyTexture(textureDamier);
  return loc;
}

void limit_fps(unsigned int limit){
  unsigned int tick = SDL_GetTicks();

  if(limit < tick)
    return;
  else if (limit > tick + FRAME_PER_SECOND)
    SDL_Delay(16);
  else
    SDL_Delay(limit - tick);

}
