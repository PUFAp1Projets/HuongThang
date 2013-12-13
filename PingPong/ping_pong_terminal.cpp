#include <iostream>
#include <math.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <termios.h>
using namespace std;

/* ------------------------------------------------------------------------------------------------------------- */

#define LIGNE 25     
#define COLONNE 70 
#define LARGEUR 10   

#define BALLE_DX 0.7
#define BALLE_DY 0.5

/* ------------------------------------------------------------------------------------------------------------- */

typedef struct 
{ double x;
  double y;
  double dx;
  double dy; } Sprite;

char screen[LIGNE][COLONNE];

/* ------------------------------------------------------------------------------------------------------------- */

bool touche_appuyee();

void init_element (Sprite & s, double x, double y, double dx, double dy);
void cadre ();
void affichage_ecran (); 
void mouvement_balle (Sprite & s);

/********************************************* ------ MAIN ------ ***********************************************/

int main()
{
  Sprite balle;
  init_element (balle, 12, 35, BALLE_DX, BALLE_DY);


  Sprite galletGauche[LARGEUR];
  for (int k = 0 ; k < LARGEUR ; k++)
    { 
      galletGauche[0].x = 5;
      galletGauche[k + 1].x = galletGauche[k].x + 1;
      init_element(galletGauche[k], galletGauche[k].x, 5, 0, 0); 
    }
  

  Sprite galletDroite[LARGEUR];
  for (int l = 0 ; l < LARGEUR ; l++) {
    galletDroite[0].x = 5;
    galletDroite[l + 1].x = galletDroite[l].x + 1;
    init_element(galletDroite[l], galletDroite[l].x, 65, 0, 0); 
  } 

  int joueur1 = 0, joueur2 = 0; // les points pour decider qui va ganer
  int niveau;                   // demander aux utilisateurs d'entrer le nombre de niveaux qu'ils veulent jouer
  char repondre;

/* ------------------------------------------------------------------------------------------------------------- */

                            /* CONTROLER LE MOUVEMENT DU GALLET A L'AIDE DU CLAVIER */

  cout << "Entrez le nombre de niveaux : " ; cin >> niveau;


  // afficher un compte a rebours (pendant que les joueurs preparent pour le jeu) 
  for (int o = 5 ; o >=0  ; o--) {
    system ("clear");
    cout << "     Le joueur 1 va controler le gallet a Gauche et le joueur 2 controle celui a Droite     " << endl;
    cout << endl << endl << endl << "                                               " << o << endl;
    usleep (1*1000000); // 1 seconde entre les numeros affiches
  }


  int n = 1; 
    
  for (int t = 0 ; ; t++) {
    while ( n <= niveau) {
    recommencer:

      system ("stty raw");  
      usleep (40 * 1000);
      
      if (touche_appuyee()) { 
	char c = getchar();
	system("stty cooked");
	  
	for (int k = 0 ; k < LARGEUR ; k++) {
	  switch (c) {
	  case 'q' : 
	    galletGauche[k].x -= 2; 
	    break;
	  case 'a' :
	    galletGauche[k].x += 2; 
	    break;

	  case 'p' :
	    galletDroite[k].x -= 2;
	    break;
	  case 'l' :
	    galletDroite[k].x += 2;
	    break;
		    
	  case 13 :
	    system ("stty cooked");
	    exit(0); 
	  }
	}
      }	    
    
      system ("stty cooked");

/* ------------------------------------------------------------------------------------------------------------- */

               /* LIMITER L'ESPACE DE DEPLACEMENT DU GALLET (QUAND IL TOUCHE LES BORDS DU CADRE) */
      
      for (int k = 0 ; k < LARGEUR ; k++)
	{	  
	if (galletGauche[LARGEUR - 1].x > LIGNE - 1)
	  galletGauche[k].x -= 2;

	if (galletDroite[LARGEUR - 1].x > LIGNE - 1)
	  galletDroite[k].x -= 2;
	}


      for (int l = 9 ; l >= 0 ; l--) 
	{ 
	if (galletGauche[0].x < 1)
	  galletGauche[l].x += 2;
	if (galletDroite[0].x < 1)
	  galletDroite[l].x += 2;
	}

/* ------------------------------------------------------------------------------------------------------------- */

                           /* AFFICHER LES ELEMENTS DU PROGRAMMES : CADRE, BALLE, GALLET */

      cadre();
 
      screen[(int) balle.x][(int) balle.y] = 'O';

      for (int k = 0 ; k < LARGEUR ; k++) 
	{ 
	  screen[(int) galletGauche[k].x][(int) galletGauche[k].y] = 'H';
	  screen[(int) galletDroite[k].x][(int) galletDroite[k].y] = 'H';
	}
	  
      affichage_ecran(); 

/* ------------------------------------------------------------------------------------------------------------- */

                                       /* CONTROLER LE MOUVEMENT DE LA BALLE */

      mouvement_balle(balle);

      //si la balle touche les gallets
      if ((int) balle.y == 6)
	for (int k = 0 ; k < LARGEUR ; k++)
	  if ((int)balle.x == (int)galletGauche[k].x)
	    balle.dy = BALLE_DX;
      
      if ((int) balle.y == 64)
	for (int l = 0 ; l < LARGEUR ; l++)
	  if ((int)balle.x == (int)galletDroite[l].x)
	    balle.dy = -BALLE_DX;
      

      //si la balle touche les murs  
      if (balle.y >= COLONNE - 2) { //les murs a Droite
	joueur1 += 1;
	balle.x = 12;
	balle.y = 35;
	if (n < niveau) {
	  cout << "Fin du niveau " << n << ". Etes-vous pret ? (press any key) : ";
	  cin >> repondre;
	  n += 1;
	  goto recommencer;
	}
	else {
	  cout << "Le jeu est termine" << endl;
	  usleep (2*1000000);
	  goto fin;
	}
      }
      
      if (balle.y <= 1) { //les murs a Gauche
	joueur2 += 1;
	balle.x = 12;
	balle.y = 35;
	if (n < niveau) {
	  cout << "Fin du niveau " << n << ". Etes-vous pret ? (press any key) : ";
	  cin >> repondre;
	  n += 1;
	  goto recommencer;
	}
	else {
	  cout << "Le jeu est termine" << endl;
	  usleep (2*1000000);
	  goto fin;
	}
      }
    }   
  }
  
/* ------------------------------------------------------------------------------------------------------------- */

                                       /* AFFICHER LE RESULTAT DU JEU */

 fin:
  
  cout << "Joueur 1 (Gauche) = " << joueur1 << endl << "Joueur 2 (Droite) = " << joueur2 << endl;
  
  if (joueur1 > joueur2)
    cout << "Joueur 1 a gagne" << endl;
  else if (joueur1 == joueur2)
    cout << "Les 2 ont le meme point" << endl;
  else
    cout << "Joueur 2 a gagne" << endl;
}

/******************************************* ------ FONCTIONS ------ *********************************************/

void init_element (Sprite & s, double x, double y, double dx, double dy) 
{ 
  s.x  = x ; 
  s.y  = y ;
  s.dx = dx;
  s.dy = dy; 
} 


void cadre ()
{
  for (int i = 0 ; i < LIGNE ; i++) {
    for (int j = 0 ; j < COLONNE ; j++)
      if (i == 0 || i == LIGNE - 1 || j == 0 || j == COLONNE - 1)
	screen[i][j] = '#';
      else
	screen[i][j] = ' ';
  }
}


void affichage_ecran()
{
  system("clear");
  for (int i = 0 ; i < LIGNE ; i++) {
    for (int j = 0 ; j < COLONNE ; j++)
      cout << screen[i][j];
    cout << endl; }
}


void mouvement_balle(Sprite & s) 
{ 
  s.x += s.dx;  
  s.y += s.dy; 
  
  if (s.x > LIGNE - 2)   
    s.dx = -BALLE_DX;
    
  if (s.y > COLONNE - 2)
    s.dy = -BALLE_DY; 

  if (s.x < 1)
    s.dx =  BALLE_DX;

  if (s.y < 1)
    s.dy =  BALLE_DY; 
}


bool touche_appuyee () 
{ 
  struct timeval tv;
  fd_set fds;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
  return (FD_ISSET(0, &fds)); 
}




