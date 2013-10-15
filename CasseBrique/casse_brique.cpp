#include <iostream>
#include <math.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <termios.h>
using namespace std;

#define LIGNE 25     //pour afficher un cadre rectangulaire de 25 lignes & 40 colonnes
#define COLONNE 40 

typedef struct 
{ double x;
  double y;
  double dx;
  double dy; } Sprite;


char screen[LIGNE][COLONNE];


void cadre ();
void affichage_screen ();
void init_balle (Sprite & s, double x, double y, double dx, double dy);
void mouvement_balle (Sprite & s);


int main()
{
  Sprite balle;

  init_balle (balle, 17, 8, 0.5, 0.9);
  /* La balle est a ligne 17, colonne 8, vitesse verticale = 1.2, vitesse horizontale = 0.8
     Il faut choisir les valeurs differentes pour dx & dy pour que la balle suive un trajectoire aleatoire, sinon
     elle se deplace toujours selon un trajectoire fixe (dx = dy --> 1 carre) */

  for (int t = 0 ; t < 10000 ; t++)
    { 
      usleep (40 * 1000);

      cadre();
 
      screen[(int) balle.x][(int) balle.y] = 'O';

      affichage_screen(); 
      // cette procedure est placee apres "cadre()" & "screen[..][..]" pour afficher a la fois le cadre & la balle

      mouvement_balle (balle); 
    }
}


void init_balle(Sprite & s, double x, double y, double dx, double dy) 
{ s.x  = x ; // attribuer a s.x une valeur equivalente de x
  s.y  = y ;
  s.dx = dx;
  s.dy = dy; } 


void cadre () // attribuer les valeurs (les caracteres) au tableau screen[i][j] (on n'affiche pas encore le cadre) 
{
  for (int i = 0 ; i < LIGNE ; i++)
    for (int j = 0 ; j < COLONNE ; j++)
      { if (i == 0 || i == LIGNE - 1 || j == 0 || j == COLONNE - 1)
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


void mouvement_balle(Sprite & s) // la procedure qui decide le mouvement de la balle
{ s.x += s.dx; // modifier la position verticale
  s.y += s.dy; // ____________________ horizontale
 

  if (s.x > LIGNE - 2) 
    s.dx = -1.2;

  if (s.y > COLONNE - 2)
    s.dy = -0.8; 

  if (s.x < 2)
    s.dx =  1.2;

  if (s.y < 2)
    s.dy =  0.8; 

  // les conditions ci-dessus servent a changer le mouvement de la balle des qu'elle est  en contact avec un mur

}
