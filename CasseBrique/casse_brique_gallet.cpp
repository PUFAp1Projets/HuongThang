#include <iostream>
#include <math.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <termios.h>
using namespace std;

/*-------------------------------------------------------------------------------------------------------*/

#define LIGNE 25     
#define COLONNE 70 
#define LARGEUR 10   // la largeur du gallet : il se compose de 10 caracteres '=' succesifs

/*-------------------------------------------------------------------------------------------------------*/

typedef struct 
{ double x;
  double y;
  double dx;
  double dy; } Sprite;

char screen[LIGNE][COLONNE];

/*-------------------------------------------------------------------------------------------------------*/

bool touche_appuyee();

void init_element (Sprite & s, double x, double y, double dx, double dy);
void cadre ();
void affichage_screen ();
void mouvement_balle (Sprite & s);

/*-------------------------------------------------------------------------------------------------------*/

int main()
{

                                       /* DECLARER LES VARIABLES */

  Sprite balle;
  init_element (balle, 17, 8, 0.7, 0.5);


  Sprite gallet[LARGEUR]; 
  for (int k = 0 ; k < LARGEUR ; k++)
    { gallet[0].y = 1;
      gallet[k + 1].y = gallet[k].y + 1;
      init_element (gallet[k], 20, gallet[k].y, 0, 0); } // la boucle FOR pour les valeurs gallet[k].y pour que 
                                                         // 10 '=' soient affiches successivement sur l'horizontal

/*-------------------------------------------------------------------------------------------------------*/

                        /* CONTROLER LE MOUVEMENT DU GALLET A L'AIDE DU CLAVIER */

  for (int t = 0 ; ; t++)
    { system ("stty raw");
      usleep (40 * 1000);
	  
      if (touche_appuyee()) 
	{ char c = getchar();
	  system("stty cooked");
	  
	  for (int k = 0 ; k < LARGEUR ; k++)
	    { switch (c)
		{ case 'f' : 
		    gallet[k].y -= 2; // le gallet se deplace vers la gauche
		    break;
		  case 'g' :
		    gallet[k].y += 2; // ____________________________ droite
		    break;
		  case 13 :
		    system ("stty cooked");
		    exit(0); }
	    }
	}	    
    
      system ("stty cooked");

/*--------------------------------------------------------------------------------------------------------*/
  
            /* LIMITER L'ESPACE DE DEPLACEMENT DU GALLET (QUAND IL TOUCHE LES BORDS DU CADRE) */

      for (int k = 0 ; k < LARGEUR ; k++) // le bord a droite
	if (gallet[9].y > COLONNE - 1)
	  gallet[k].y -= 2;
    

      for (int l = 9 ; l >= 0 ; l--)      // le bord a gauche
	if (gallet[0].y < 1)
	  gallet[l].y += 2;

/*--------------------------------------------------------------------------------------------------------*/

                      /* AFFICHER LES ELEMENTS DU PROGRAMMES : CADRE, BALLE, GALLET */

      cadre();
 
      screen[(int) balle.x][(int) balle.y] = 'O';

      for (int k = 0 ; k < LARGEUR ; k++)
	screen[(int) gallet[k].x][(int) gallet[k].y] = '=';

      affichage_screen(); 

/*-------------------------------------------------------------------------------------------------------*/

                                /* CONTROLER LE MOUVEMENT DE LA BALLE */

      mouvement_balle(balle);


      if ((int) balle.x == 19)
	for (int k = 0 ; k < LARGEUR ; k++)
	  if ((int)balle.y == (int)gallet[k].y)
	    balle.dx = -0.7;
      // si la balle touche le gallet, elle va rebondir, c'est-a-dire seulement sa vitesse verticale est changee,
      // alors on ne modifie que balle.dx


      if (balle.x >= LIGNE - 2)
	{ cout << "                        ----- GAME OVER -----" << endl;
	  exit(0); } 
      // si la balle tombe au bas du cadre, le jeu termine : GAME OVER
    
    }
}

/*-------------------------------------------------------------------------------------------------------*/

void init_element (Sprite & s, double x, double y, double dx, double dy) 
{ s.x  = x ; 
  s.y  = y ;
  s.dx = dx;
  s.dy = dy; } 


void cadre ()
{
  for (int i = 0 ; i < LIGNE ; i++)
    { for (int j = 0 ; j < COLONNE ; j++)
	if (i == 0 || i == LIGNE - 1 || j == 0 || j == COLONNE - 1)
	  screen[i][j] = '#';
	else
	  screen[i][j] = ' '; }
}



void affichage_screen()
{
  system("clear");
  for (int i = 0 ; i < LIGNE ; i++)
    { for (int j = 0 ; j < COLONNE ; j++)
	cout << screen[i][j];
      cout << endl; }
}



void mouvement_balle(Sprite & s) 
{ s.x += s.dx;  
  s.y += s.dy; 
  
  if (s.x > LIGNE - 2)   
    s.dx = -0.7;
    
  if (s.y > COLONNE - 2)
    s.dy = -0.5; 

  if (s.x < 1)
    s.dx =  0.7;

  if (s.y < 1)
    s.dy =  0.5; }


bool touche_appuyee () 
{ struct timeval tv;
  fd_set fds;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
  return (FD_ISSET(0, &fds)); }
