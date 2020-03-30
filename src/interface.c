#include "interface.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_video.h>
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
void SDL_ExitWithError(const char *message,SDL_Texture *texture, SDL_Surface *surface, SDL_Renderer *renderer, SDL_Window *window){
  SDL_Log("ERREUR : %s > %s\n", message,SDL_GetError());

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  exit(EXIT_FAILURE);
}



void creationFond(SDL_Renderer *renderer,SDL_Rect plateau,SDL_Rect fenetre,SDL_Texture *textureDamier){
  SDL_Rect background;
  background.x = 0;
  background.y = 0;
  background.w = fenetre.w;
  background.h = fenetre.h;

  SDL_SetRenderDrawColor(renderer,255,255,255,255);
  SDL_RenderFillRect(renderer,&background);

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

  if(imagePion == NULL)
    SDL_ExitWithError("Creation image Pion echouee",NULL,imagePion,renderer,window);

  texturePion = SDL_CreateTextureFromSurface(renderer, imagePion);
  SDL_FreeSurface(imagePion);

  if(texturePion == NULL)
    SDL_ExitWithError("Creation texture Pion echouee",texturePion,NULL,renderer,window);


    SDL_Rect RectPion;


    if(SDL_QueryTexture(texturePion,NULL,NULL,&RectPion.w,&RectPion.h) != 0)
      SDL_ExitWithError("Chargement en memoire texture",texturePion,NULL,renderer,window);

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



void creationFondMenu(SDL_Renderer *renderer, SDL_Rect fenetre, SDL_Texture *textureTitre,SDL_Rect emplacementTitre){
  SDL_Rect background;
  background.x = 0;
  background.y = 0;
  background.w = fenetre.w;
  background.h = fenetre.h;

  SDL_SetRenderDrawColor(renderer,255,255,255,255);
  SDL_RenderFillRect(renderer,&background);

  SDL_RenderCopy(renderer,textureTitre,NULL,&emplacementTitre);
}


void menu(SDL_Rect fenetre, SDL_Window *window, SDL_Renderer *renderer){
  SDL_Surface *imageTitre = NULL;
  SDL_Texture *textureTitre = NULL;
  imageTitre= IMG_Load("./../img/titre.png");
  if(imageTitre == NULL)
    SDL_ExitWithError("Creation imageTitre echouee",NULL,imageTitre,renderer,window);

  textureTitre = SDL_CreateTextureFromSurface(renderer, imageTitre);
  SDL_FreeSurface(imageTitre);

  if(textureTitre == NULL)
    SDL_ExitWithError("Creation texture echouee",textureTitre,NULL,renderer,window);

  SDL_Rect emplacementTitre;

  if(SDL_QueryTexture(textureTitre,NULL,NULL,&emplacementTitre.w,&emplacementTitre.h) != 0)
    SDL_ExitWithError("Chargement en memoire texture",textureTitre,NULL,renderer,window);

  emplacementTitre.x =  (fenetre.w - emplacementTitre.w)/2;
  emplacementTitre.y = 100;


  SDL_bool program_launched = SDL_TRUE;


    while (program_launched) {
      SDL_Event  event;
        while (SDL_PollEvent(&event)) {
          switch (event.type) {
            case SDL_QUIT:
              program_launched = SDL_FALSE;
            break;
            default:
            break;
          }
        }
        SDL_RenderClear(renderer);
        creationFondMenu(renderer,fenetre,textureTitre, emplacementTitre);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(textureTitre);
}


void jeu(SDL_Rect plateau,SDL_Rect fenetre,SDL_Window *window, SDL_Renderer *renderer,cell tab[10][10]){

  SDL_Surface *imageDamier = NULL;
  SDL_Texture *textureDamier = NULL;



  coordInt selectedBox;
  selectedBox.x = -1;
  selectedBox.y = -1;

  imageDamier= IMG_Load("./../img/damier 500X500.png");
  if(imageDamier == NULL)
    SDL_ExitWithError("Creation imageDamier echouee",NULL,imageDamier,renderer,window);

  textureDamier = SDL_CreateTextureFromSurface(renderer, imageDamier);
  SDL_FreeSurface(imageDamier);

  if(textureDamier == NULL)
    SDL_ExitWithError("Creation texture echouee",textureDamier,NULL,renderer,window);


  if(SDL_QueryTexture(textureDamier,NULL,NULL,&plateau.w,&plateau.h) != 0)
    SDL_ExitWithError("Chargement en memoire texture",textureDamier,NULL,renderer,window);

  SDL_bool program_launched = SDL_TRUE;

  while(program_launched) {
      SDL_Event event;

      while(SDL_PollEvent(&event)) {


          switch (event.type) {
            case SDL_QUIT:
              program_launched = SDL_FALSE;
              break;
            case SDL_KEYDOWN:
              if (event.key.keysym.sym == SDLK_ESCAPE)
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

            default:
              break;
          }
        }

      // Clear screen
      SDL_RenderClear(renderer);
      // Draw
      creationFond(renderer,plateau,fenetre,textureDamier);
      generatePion(window,renderer,plateau,tab);
      // Show what was drawn
      SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(textureDamier);
}
