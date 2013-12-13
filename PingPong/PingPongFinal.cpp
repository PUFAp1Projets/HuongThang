#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <SDL/SDL.h>
using namespace std;

/************************************** DECLARER LES VARIABLES, LES CONSTANTS *************************************/

//la dimension de l'ecran
const int ECRAN_LONG = 1024;
const int ECRAN_LAR = 640;

//la taille & la vitesse des gallets
const int GALLET_LONG = 120;
const int GALLET_LAR = 32;
const int V_GALLET = 16;

//la taille & la vitesse de la balle
const int BALLE_TAILLE = 36;
const int V_BALLE = 7;

//le nombre des boutons a la page d'introduction
const int N_BOUTON = 3;

/*---------------------------------------------------------------------------------*/

typedef struct {
  int x;
  int y;
  int dx;
  int dy;
} Sprite;

/*---------------------------------------------------------------------------------*/

SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
SDL_Surface *balle = NULL;
SDL_Surface *galletG = NULL;
SDL_Surface *galletD = NULL;
SDL_Surface *point = NULL;
SDL_Rect bouton[N_BOUTON];
SDL_Event event;


Sprite ball, galG, galD;

int fois = 0;

//les points des players
int player1 = 0; 
int player2 = 0;

/************************************************ LES PROTOTYPES **************************************************/

//I. LES PREPARATIONS DU JEU : L'ECRAN AFFICHE, LA POSITION INITIALE DES ELEMENTS, LA PAGE D'INTRODUCTION
bool test();
void init_elem(); 
void traiterBouton();
void Intro(bool & play, bool & run);

/*---------------------------------------------------------------------------------*/

//II. L'AFFICHAGE DES ELEMENTS PRINCIPAUX : BACKGROUND, GALLETS, BALLE
SDL_Surface *traiterImg (string filename);
bool saisirImg();

void updateImg(SDL_Surface *src, SDL_Surface *dest, int x, int y, SDL_Rect * clip = NULL) {
  SDL_Rect offset;
  offset.x = x;
  offset.y = y;
  SDL_BlitSurface(src, clip, dest, &offset);
}

void afficherImg();

/*---------------------------------------------------------------------------------*/

//III. LE MOUVEMENT DES ELEMENTS : BALLE, GALLETS, QUAND LA BALLE TOUCHE LES MURS OU LES GALLETS
void mouvementBalle();
void Rapide(int & balleTick);
void mouvementGallet();  
void balleMur(int a, int b, int & player);
void balleGallet();

/*---------------------------------------------------------------------------------*/

//IV. LE RESULTAT APRES 3 FOIS A JOUER
void Result(int x, int y, int a, int b, bool & run);
void resultat(bool & play);

/***************************************************** MAIN *******************************************************/

int main() {
  bool play = true;
  bool run = true;
  int balleTick;  
  balleTick = SDL_GetTicks(); 

  test();
  saisirImg();
  init_elem();

  while (run) {
    Intro(play, run);
    
    while (play) {
      while (SDL_PollEvent(&event)) {
	switch (event.type) {
	case SDL_KEYDOWN:
	  switch (event.key.keysym.sym) {
	  case SDLK_ESCAPE:
	    run = false;
	    play = false;
	    break;
	  }
	  break;

	case SDL_QUIT:
	  run = false;
	  play = false;
	  break;
       	}      
      }
    
      afficherImg();
      
      mouvementBalle();
      Rapide(balleTick);
      mouvementGallet();
      balleGallet();

      resultat(play);
 
      SDL_Flip(screen);
    }
  }
}

/************************************************* LES FONCTIONS **************************************************/

/* I. PREPARATION : ===============================================================*/

bool test() {
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) 
    return false;
  
  SDL_WM_SetCaption("Ping Pong Christmas", NULL);
  
  screen = SDL_SetVideoMode(ECRAN_LONG, ECRAN_LAR, 24, SDL_SWSURFACE);
  
  return true;
}

/*--------------------------------------------------------*/

void init_elem() {
  ball.x = ECRAN_LONG/2;
  ball.y = ECRAN_LAR/2;
  ball.dx = V_BALLE;
  ball.dy = V_BALLE;

  galG.x = 70;
  galG.y = ECRAN_LAR/2 - GALLET_LONG/2;
  galG.dx = 0;
  galG.dy = V_GALLET;

  galD.x = ECRAN_LONG - GALLET_LAR - 70;
  galD.y = ECRAN_LAR/2 - GALLET_LONG/2;
  galD.dx = 0;
  galD.dy = V_GALLET;
} 

/*--------------------------------------------------------*/

void traiterBouton() {
  for (int i = 0 ; i < N_BOUTON ; i++) {
    bouton[i].x = i*177;
    bouton[i].y = 0;
    bouton[i].w = 177;
    bouton[i].h = 125;
  }
}

/*--------------------------------------------------------*/

void Intro(bool & play, bool & run) { 
  bool quitter = true;  
  traiterBouton();

  background = traiterImg("Images/intro.bmp");
  balle = traiterImg("Images/button.bmp");

  while (quitter) {
    updateImg(background, screen, 0, 0);
    
    for (int i = 0 ; i < N_BOUTON ; i++)
      updateImg(balle, screen, 70, 100 + i*175, &bouton[i]);      
    
    SDL_Flip(screen);

    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_MOUSEBUTTONDOWN) {
	int a = event.button.x;
	int b = event.button.y;
	
	//play
	if (a > 95 && a < 200 && b > 120 && b < 215 && event.button.button == SDL_BUTTON_LEFT) {
	  quitter = 0;
	  saisirImg();
	  play = true;
	  fois = 0;
	  player1 = 0;
	  player2 = 0;
	}

	//help
	if (a > 95 && a < 200 && b > 295 && b < 390 && event.button.button == SDL_BUTTON_LEFT) {
	  galletG = traiterImg("Images/help.bmp");
	  updateImg(galletG, screen, 250, 130);
	  SDL_Flip(screen);
	  SDL_Delay(5000);
	}

	//quit
	if (a > 95 && a < 200 && b > 470 && b < 565 && event.button.button == SDL_BUTTON_LEFT) {
	  quitter = false;
	  play = false;
	  run = false;
	}
      }      
    }
  }
}

/* II. AFFICHAGE : ============================================================*/

SDL_Surface *traiterImg (string filename) {
  SDL_Surface *image = NULL;
  image = SDL_LoadBMP( filename.c_str() );
  SDL_Surface * imgTraite = NULL;
  
  if (image != NULL) {
    imgTraite = SDL_DisplayFormat(image);
    SDL_FreeSurface(image);
    if (imgTraite != NULL)
      SDL_SetColorKey(imgTraite, SDL_SRCCOLORKEY, SDL_MapRGB(imgTraite -> format, 0xFF, 0xFF, 0xFF));
  }
  
  return imgTraite;
}

/*--------------------------------------------------------*/

bool saisirImg() {
  background = traiterImg("Images/background.bmp");  
  balle = traiterImg("Images/ball.bmp");
  galletG = traiterImg("Images/galletG.bmp");
  galletD = traiterImg("Images/galletD.bmp");
  point = traiterImg("Images/point.bmp");
   
  if (background == NULL || balle == NULL || galletG == NULL || galletD == NULL || point == NULL)
    return false;

  return true;  
}

/*--------------------------------------------------------*/

void afficherImg() {
  updateImg(background, screen, 0, 0);

  if (fois < 3) 
    updateImg(balle, screen, ball.x, ball.y);
    
  updateImg(galletG, screen, galG.x, galG.y);
  updateImg(galletD, screen, galD.x, galD.y);
  SDL_Flip(screen);
}

/* III. MOUVEMENT : =============================================================*/

void mouvementBalle() {
  ball.x += ball.dx;
  ball.y += ball.dy;

  if (ball.x > ECRAN_LONG - BALLE_TAILLE) 
    balleMur(ECRAN_LONG/4 - 35, ECRAN_LAR/2 - 35, player1);

  if (ball.x < 0) 
    balleMur(ECRAN_LONG*3/4 - 35, ECRAN_LAR/2 - 35, player2);  

  if (ball.y > ECRAN_LAR - BALLE_TAILLE || ball.y < 0)
    ball.dy *= -1;
}

/*--------------------------------------------------------*/

void Rapide(int & balleTick) { //fonction sert a augmenter la vitesse de la balle apres toutes les 15 secondes
  if ((SDL_GetTicks() - balleTick)/1000 > 15) 
    {
      if (ball.dy > 0) 
	ball.dy += 2;
      else
	ball.dy -= 2;

      if (ball.dx > 0)
	ball.dx += 2;
      else
	ball.dy -= 2;
      
      balleTick = SDL_GetTicks();
    }
}

/*--------------------------------------------------------*/

void mouvementGallet() {
  Uint8 *keystates = SDL_GetKeyState(NULL);

  //Le gallet Droite
  if (keystates[SDLK_UP]) 
    galD.y -= galG.dy;
  else if (keystates[SDLK_DOWN])
    galD.y += galG.dy;
  if (galD.y < 1)
    galD.y = 0;
  else if (galD.y > ECRAN_LAR - GALLET_LONG)
    galD.y = ECRAN_LAR - GALLET_LONG;


  //Le gallet Gauche
  if (keystates[SDLK_a]) 
    galG.y -= galG.dy;
  else if (keystates[SDLK_z])
    galG.y += galG.dy;
  if (galG.y < 1)
    galG.y = 0;
  else if (galG.y > ECRAN_LAR - GALLET_LONG)
    galG.y = ECRAN_LAR - GALLET_LONG;
}

/*--------------------------------------------------------*/

void balleMur(int a, int b, int & player) {
  init_elem();
  fois += 1;
  updateImg(point, screen, a, b);

  SDL_Flip(screen);
  SDL_Delay(3000);
  player += 1;
}

/*--------------------------------------------------------*/

void balleGallet() {
  if ((ball.x <= galG.x + GALLET_LAR && ball.y >= galG.y && ball.y <= galG.y + GALLET_LONG) ||
      (ball.x >= galD.x - BALLE_TAILLE  && ball.y >= galD.y && ball.y <= galD.y + GALLET_LONG))
    ball.dx *= -1;
}

/* IV. RESULTAT : ===================================================================*/

void Result(int x, int y, int a, int b, bool & play) {
  balle = traiterImg("Images/winner.bmp");
  point = traiterImg("Images/loser.bmp");

  afficherImg();
  
  updateImg(balle, screen, x, y);
  updateImg(point, screen, a, b);

  SDL_Flip(screen);
  SDL_Delay(5000);
  play = false;
}

/*--------------------------------------------------------*/

void resultat(bool & run) {
  if (fois == 3) {
    if (player1 > player2) 
      Result(95, ECRAN_LAR/4, ECRAN_LONG/2 + 150, ECRAN_LAR/4, run);

    if (player1 < player2)
      Result(ECRAN_LONG/2 + 95, ECRAN_LAR/4, 150, ECRAN_LAR/4, run);
  }
}

/******************************************************************************************************************/
