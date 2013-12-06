#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <termios.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
using namespace std;

/*****************************************************/

const int ECRAN_LONG = 1024;
const int ECRAN_LAR = 640;

const int GALLET_LONG = 120;
const int GALLET_LAR = 32;
const int V_GALLET = 16;

const int BALLE_TAILLE = 36;
const int V_BALLE = 6;

/*****************************************************/

typedef struct {
  int x;
  int y;
  int dx;
  int dy;
} Sprite;

/*****************************************************/

SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
SDL_Surface *balle = NULL;
SDL_Surface *galletG = NULL;
SDL_Surface *galletD = NULL;
SDL_Surface *point = NULL;

static Sprite ball, galG, galD;

int player1 = 0;
int player2 = 0;
int fois = 0;
bool run = 1;

/*****************************************************/

void init_elem() {
  ball.x = ECRAN_LONG/2;
  ball.y = ECRAN_LAR/2;
  ball.dx = V_BALLE;
  ball.dy = V_BALLE;

  galG.x = 70;
  galG.y = 120;
  galG.dx = 0;
  galG.dy = V_GALLET;

  galD.x = ECRAN_LONG - GALLET_LAR - 70;
  galD.y = 120;
  galD.dx = 0;
  galD.dy = V_GALLET;
} 
 
/*---------------------------------------------------*/

void updateImg(SDL_Surface *src, SDL_Surface *dest,
	       int x, int y, SDL_Rect * clip = NULL) {
  SDL_Rect offset;
  offset.x = x;
  offset.y = y;
  SDL_BlitSurface(src, clip, dest, &offset);
}

/*---------------------------------------------------*/

void reInitBalle() {
  usleep(300*10000);
  ball.x = ECRAN_LONG/2;
  ball.y = ECRAN_LAR/2;
  fois += 1;
}


/*---------------------------------------------------*/

void resultat() {
  cout << "RESULTAT : " << endl;
  if (player1 > player2)
    cout << "Player 1 a gagne" << endl;
  else
    cout << "Player 2 a gagne" << endl;
}

/*---------------------------------------------------*/

void mouvementBalle() {
  ball.x += ball.dx;
  ball.y += ball.dy;

  if (ball.x > ECRAN_LONG - BALLE_TAILLE) {
    reInitBalle();
    //updateImg(point, screen, 10, 10);
    player1 += 1;
  }

  if (ball.x < 0) {
    reInitBalle();
    //updateImg(point, screen, ECRAN_LONG - 100, 10);
    player2 += 2;
  }

  if (ball.y > ECRAN_LAR - BALLE_TAILLE || ball.y < 0)
    ball.dy *= -1;

  if (fois == 3) {
    run = 0;
    resultat();
  }
}

/*---------------------------------------------------*/

void mouvementGallet() {
  Uint8 *keystates = SDL_GetKeyState(NULL);

  if (keystates[SDLK_UP]) 
    galD.y -= galG.dy;
  else if (keystates[SDLK_DOWN])
    galD.y += galG.dy;
  if (galD.y < 1)
    galD.y = 0;
  else if (galD.y > ECRAN_LAR - GALLET_LONG)
    galD.y = ECRAN_LAR - GALLET_LONG;


  if (keystates[SDLK_a]) 
    galG.y -= galG.dy;
  else if (keystates[SDLK_z])
    galG.y += galG.dy;
  if (galG.y < 1)
    galG.y = 0;
  else if (galG.y > ECRAN_LAR - GALLET_LONG)
    galG.y = ECRAN_LAR - GALLET_LONG;
}
  
/*---------------------------------------------------*/

void balleGallet() {
  if ((ball.x <= galG.x + GALLET_LAR &&
       ball.y >= galG.y &&
       ball.y <= galG.y + GALLET_LONG) ||
      (ball.x >= galD.x - GALLET_LAR &&
       ball.y >= galD.y &&
       ball.y <= galD.y + GALLET_LONG))
    ball.dx *= -1;
}

/*---------------------------------------------------*/

bool test() {
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) 
    return false;
  
  SDL_WM_SetCaption("Ping Pong Christmas", NULL);
  
  screen = SDL_SetVideoMode(ECRAN_LONG, ECRAN_LAR, 24, SDL_SWSURFACE);
  
  return true;
}

/*---------------------------------------------------*/

SDL_Surface *traiterImg (string filename) {
  SDL_Surface *image = NULL;
  image = SDL_LoadBMP( filename.c_str() );
  SDL_Surface * ephinh = NULL;
  
  if (image != NULL) {
    ephinh = SDL_DisplayFormat(image);
    SDL_FreeSurface(image);
    if (ephinh != NULL)
      SDL_SetColorKey(ephinh, SDL_SRCCOLORKEY, SDL_MapRGB(ephinh -> format, 0xFF, 0xFF, 0xFF));
  }
  
  return ephinh;
}

/*---------------------------------------------------*/

bool saisirImg() {
  background = traiterImg("Images/background.bmp");
  balle = traiterImg("Images/ball.bmp");
  galletG = traiterImg("Images/galletG.bmp");
  galletD = traiterImg("Images/galletD.bmp");
  point = traiterImg("Images/point.bmp");
  
  if (background == NULL || balle == NULL || 
      galletG == NULL || galletD == NULL || 
      point == NULL)
    return false;

  return true;  
}

/*---------------------------------------------------*/

void afficherImg() {
  updateImg(background, screen, 0, 0);
  updateImg(balle, screen, ball.x, ball.y);
  updateImg(galletG, screen, galG.x, galG.y);
  updateImg(galletD, screen, galD.x, galD.y);
}

/*---------------------------------------------------*/

int main() {
  SDL_Event event;

  test();
  saisirImg();

  init_elem();
    
  while (run) {    
    while (SDL_PollEvent(&event)) {
      switch(event.type) {
      case SDL_KEYDOWN:
	switch(event.key.keysym.sym) {
	case SDLK_ESCAPE:
	  run = 0;
	  break;
	default:
	  break;
	}
	break;

      case SDL_QUIT:
	run = 0;
	break;
	
      default:
	break;
      }      
    }
      
    afficherImg();    
    mouvementBalle();
    mouvementGallet();
    balleGallet();
    
    SDL_Flip(screen);
  }
}
