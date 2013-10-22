#include <iostream>
#include <math.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <termios.h>
using namespace std;

/**************************************************************************************************************/

#define LIGNE 25       // la taille du cadre
#define COLONNE 74

#define LIG_BRI 6      // la ligne des briques
#define C_BRI 9        // la colonne des briques
#define LAR_BRI 6      // la largeur d'une brique " HHHHHH "

#define BALLE_DX 0.7   // la vitesse verticale   de la balle
#define BALLE_DY 0.5   // __________ horizontale ___________
 
#define LAR_GAL 14     // la largeur du galet    " ============== "

/**************************************************************************************************************/

typedef struct 
{ double x;
  double y;
  double dx;
  double dy; } Sprite;      // pour la balle


typedef struct
{ int a;
  int b[LAR_BRI];          // tableau pour regrouper les caracteres H : 6 H forment 1 brique "HHHHHH"   
  bool presente; } Block;  // pour les briques

/**************************************************************************************************************/
 
char ecran[LIGNE][COLONNE];

bool touche_appuyee();

void init_element (Sprite & s, double x, double y, double dx, double dy);
void cadre ();
void balle_brique(Block & p, Sprite & s);
void affichage_ecran ();
void mouvement_balle (Sprite & s);

/**************************************************************************************************************/

int main()
{     
  /* DECLARER LES VARIABLES */
  
  Sprite balle;
  init_element (balle, 17, 8, BALLE_DX, BALLE_DY);


  Sprite gallet[LAR_GAL];
  for (int k = 0 ; k < LAR_GAL ; k++)
    { gallet[k].y = k + 1;
      init_element (gallet[k], 20, gallet[k].y, 0, 0); }


  Block brique[LIG_BRI][C_BRI]; // tableau de 2 dimensions pour afficher les briques sur les lignes & les colonnes
  for (int m = 0 ; m < LIG_BRI ; m++)
    for (int n = 0 ; n < C_BRI ; n++)
      { 
        brique[m][n].a = m + 2; 
        for (int e = 0 ; e < LAR_BRI ; e++)
          { brique[m][n].presente = true; 
            brique[m][n].b[e] = 8*n + 2 + e; }// le coefficient (8) decide la distance entre les colonnes de briques  
      }      
  // la premiere brique sera affichee aux coordonnees (2;2)


  int test = 0 ; // 1 variable supplementaire pour terminer le jeu quand toutes les briques disparaissent 

/**************************************************************************************************************/

  /* CONTROLER LE GALLET AVEC LE CLAVIER */

  for (int t = 0 ; ; t++)
    { system ("stty raw");   
      usleep (30 * 1000);
          
      if (touche_appuyee()) 
        { char c = getchar();
          system("stty cooked");
          
          for (int k = 0 ; k < LAR_GAL ; k++)
            { switch (c)
                { case 'f' : 
                    gallet[k].y -= 2;
                    break;
		case 'g' :
		  gallet[k].y += 2; 
		  break;
		case 13 :
		  system ("stty cooked");
		  exit(0); }
            }
        }            
    
      system ("stty cooked");

/**************************************************************************************************************/

      /* LIMITER L'ESPACE DE DEPLACEMENT DU GALLET */

      for (int k = 0 ; k < LAR_GAL ; k++) 
        if (gallet[LAR_GAL - 1].y > COLONNE - 1)
          gallet[k].y -= 2;
    

      for (int l = LAR_GAL - 1 ; l >= 0 ; l--)      
        if (gallet[0].y < 1)
          gallet[l].y += 2;

/***************************************************************************************************************/

      /* AFFICHER LES ELEMENTS : CADRE, BALLE, GALLET, BRIQUES */

      cadre();


      ecran[(int) balle.x][(int) balle.y] = 'O';


      for (int k = 0 ; k < LAR_GAL ; k++)
        ecran[(int) gallet[k].x][(int) gallet[k].y] = '=';


      for (int m = LIG_BRI ; m >= 0 ; m--)  // detecter si la balle touche les briques
        for (int n = 0 ; n < C_BRI ; n++)
          for (int e = 0 ; e < LAR_BRI ; e++)
            if (   (int) balle.x + 1 == brique[m][n].a  
                   && (int) balle.y == brique[m][n].b[e] 
                   && brique[m][n].presente == true) 

              { balle.dx = BALLE_DX;        // la balle rebondit
                brique[m][n].presente = false; 
		test += 1; } // quand la balle touche 1 brique, le programme effectue test += 1; 
        
      // balle_brique (brique[LIG_BRI][C_BRI], balle); 
      // Nous avons essaye a utiliser cette fonction pour rendre les codes plus clairs, mais elle ne marche pas :(


      for (int m = 0 ; m < LIG_BRI ; m++)
        for (int n = 0 ; n < C_BRI ; n++)
          for (int e = 0 ; e < LAR_BRI ; e++)
            if (brique[m][n].presente)  
              ecran[brique[m][n].a][brique[m][n].b[e]] = 'H'; 
      // afficher seulement les briques que la balle ne touchent pas encore
        
        
      affichage_ecran(); 
      
/****************************************************************************************************************/

      /* LE MOUVEMENT DE LA BALLE */


      mouvement_balle(balle);


      if ((int) balle.x == 19)
        for (int k = 0 ; k < LAR_GAL ; k++)
          if ((int)balle.y == (int)gallet[k].y)
            balle.dx = - BALLE_DX; 


      if (test >= 54) //total : 54 briques, quand test atteint ce nombre, toutes les briques disparaissent -> gagner
	{ cout << endl << "VOUS AVEZ GAGNE" << endl << endl; 
	  exit(0); }
    

      if (balle.x >= LIGNE - 2)
        { cout << endl << "----- GAME OVER -----" << endl << endl;
	  exit(0); }  
      // si la balle tombe du gallet, le joueur va perdre
          
    }
}

/****************************************************************************************************************/

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
          ecran[i][j] = '#';
        else
          ecran[i][j] = ' '; }
}


void affichage_ecran()
{
  system("clear");
  for (int i = 0 ; i < LIGNE ; i++)
    { for (int j = 0 ; j < COLONNE ; j++)
        cout << ecran[i][j];
      cout << endl; }
}


void balle_brique(Block & p, Sprite & s)
{
  for (int m = LIG_BRI ; m >= 0 ; m--)
    for (int n = 0 ; n < C_BRI ; n++)
      for (int e = 0 ; e < LAR_BRI ; e++)
        if (    (int)s.x + 1 == p.a 
                && (int)s.y == p.b[e] 
                && p.presente == true)
          
          { s.dx = BALLE_DX; 
            p.presente = false; }
}


void mouvement_balle(Sprite & s) 
{ s.x += s.dx;  
  s.y += s.dy; 
  
  if (s.x > LIGNE - 2)   
    s.dx = -BALLE_DX;
    
  if (s.y > COLONNE - 2)
    s.dy = -BALLE_DY; 

  if (s.x < 1)
    s.dx =  BALLE_DX;

  if (s.y < 1)
    s.dy =  BALLE_DY; }


bool touche_appuyee () 
{ struct timeval tv;
  fd_set fds;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
  return (FD_ISSET(0, &fds)); }
