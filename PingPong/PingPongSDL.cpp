#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <termios.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
using namespace std;

/********************************************* -----CONSTANTS----- ************************************************/

#define BALLE_TAILLE 36
#define BALLE_DX 10
#define BALLE_DY 10

#define GALLET_LONG 32
#define GALLET_LAR 120
#define V_GALLET 50

#define ECRAN_LONG 1024
#define ECRAN_LAR 640

/********************************************* -----FONCTIONS----- ************************************************/

//le mouvement de la balle : la balle se deplace, touche les murs
void mouvementBalle(SDL_Rect & balle, int & balle_dx, int & balle_dy) {
  balle.x += balle_dx;
  balle.y += balle_dy;

  if (balle.x > ECRAN_LONG - BALLE_TAILLE || balle.x < 1)
    balle_dx *= -1;
    //exit(0); si la balle touche le mur vertical, le joueur va perdu
  
  if (balle.y > ECRAN_LAR - BALLE_TAILLE || balle.y < 1)
    balle_dy *= -1;
}


//la balle touche les gallets et rebondit
void balleGallet(SDL_Rect balle, SDL_Rect gauche, SDL_Rect droite, int & v_balle) {
  if ((balle.x <= gauche.x + GALLET_LONG &&
       balle.y >= gauche.y &&
       balle.y <= gauche.y + GALLET_LAR) ||
      (balle.x >= droite.x - GALLET_LONG &&
       balle.y >= droite.y &&
       balle.y <= droite.y + GALLET_LAR))
    v_balle *= -1;
}


//limiter l'espace de deplacement des gallets
void limiteGallet(SDL_Rect & gauche, SDL_Rect & droite) {
  if (gauche.y < 0)
    gauche.y += V_GALLET;

  if (gauche.y > ECRAN_LAR - GALLET_LAR)
    gauche.y -= V_GALLET;

  if (droite.y < 0)
    droite.y += V_GALLET;

  if (droite.y > ECRAN_LAR - GALLET_LAR)
    droite.y -= V_GALLET;    
}

/******************************************************************************************************************/

int main() {

  /*----------------------------------------------DECLARER LES VARIABLES------------------------------------------*/

  int balle_dx = BALLE_DX;
  int balle_dy = BALLE_DY;

  SDL_Event event;
  bool bRun = 1;
  SDL_Surface *screen, *galletG, *galletD, *balle, *background;

  SDL_Rect backgroundRect;
  backgroundRect.x = 0;
  backgroundRect.y = 0;

  SDL_Rect balleRect;
  balleRect.x = ECRAN_LONG/2;
  balleRect.y = ECRAN_LAR/2;
  
  SDL_Rect galletRectG;
  galletRectG.x = 70;
  galletRectG.y = 120;
    
  SDL_Rect galletRectD;
  galletRectD.x = ECRAN_LONG - GALLET_LONG - 70;
  galletRectD.y = 120;

  /*-----------------------------------------INITIALISER LA FENETRE DU JEU----------------------------------------*/

  atexit(SDL_Quit);
  
  if (SDL_Init(SDL_INIT_VIDEO) < 0) exit(1);

  SDL_WM_SetCaption("A Very Ping Pong Christmas", NULL);

  screen = SDL_SetVideoMode(ECRAN_LONG, ECRAN_LAR, 24, 0);

  /*---------------------------------AFFICHER DES IMAGES : ECRAN, BALLE, GALLETS----------------------------------*/
  
  background = SDL_LoadBMP("background.bmp");
  balle = IMG_Load("balle.png");
  galletG = IMG_Load("candyGauche.png");
  galletD = IMG_Load("candyDroite.png");

  /*--------------------------------------------------------------------------------------------------------------*/

  while (bRun) {
    SDL_FillRect(screen, NULL, 0x221122);
    SDL_BlitSurface(background, NULL, screen, &backgroundRect);
    SDL_BlitSurface(balle, NULL, screen, &balleRect);
    SDL_BlitSurface(galletG, NULL, screen, &galletRectG);
    SDL_BlitSurface(galletD, NULL, screen, &galletRectD);
    SDL_Flip(screen);
    SDL_Delay(10);
    
    mouvementBalle(balleRect, balle_dx, balle_dy);

    /*-----------------------------------------CONTROLER LES GALLETS---------------------------------------------*/

    while (SDL_PollEvent(&event)) {
      switch(event.type) {
      case SDL_KEYDOWN:
	switch(event.key.keysym.sym) {
	case SDLK_UP:
	  galletRectD.y -= V_GALLET;
	  break;
	case SDLK_DOWN:
	  galletRectD.y += V_GALLET;
	  break;
	case SDLK_a:
	  galletRectG.y -= V_GALLET;
	  break;
	case SDLK_z:
	  galletRectG.y += V_GALLET;
	  break;
	case SDLK_ESCAPE:
	  bRun = 0;
	  break;
	default:
	  break;
	}
	break;

      case SDL_QUIT:
	bRun = 0;
	break;
	
      default:
	break;
      }      
    }

    limiteGallet(galletRectG, galletRectD);

    balleGallet(balleRect, galletRectG, galletRectD, balle_dx);

  }

  return 0;
}
      
/******************************************************************************************************************/
