****************************************************************************************************
****************************************************************************************************
****************************************************************************************************

                             --- LE PROJET DU JEU "CASSE BRIQUE" ---

****************************************************************************************************
****************************************************************************************************
****************************************************************************************************

REPONSE A QUESTION 1 : le fichier "casse_brique_balle.cpp"

Dans ce fichier nous avons mis les codes dont les fonctions :

- faire afficher le cadre  

- la balle qui va rebondir chaque fois qu'elle est en contact avec les murs "O"

****************************************************************************************************

REPONSE A QUESTION 2 : le fichier "casse_brique_gallet.cpp"

Dans ce fichier nous avons ajoute les codes dont les fonctions :

- faire afficher un gallet que l'on peut controler a l'aide du clavier  "=============="

- la balle qui va rebondir chaque fois qu'elle est en contact avec la gallet
 
****************************************************************************************************

REPONSE A QUESTION 3 : le fichier "casse_brique_ensemble.cpp"

Dans ce fichier nous avons mis les codes pour l'ensemble du programme :

- faire afficher les briques "HHHHHH" qui vont disparaitre des qu'elles sont en contact avec la balle

- faire affichier le cadre, le gallet que l'on peut controler, la balle qui va rebondir chaque fois qu'elle est en contact avec les murs, avec le gallet ou avec les briques

- si la balle tombe au bas : le joueur perd.

  si la balle fait disparaitre toutes les briques : le joueur gagne.

*****************************************************************************************************

LES ERREURS DANS NOTRE PROGRAMME :

Bien que notre jeu marche (au moins il n'y a pas de fautes graves), nous avons encore quelques problemes que nous n'arrivons pas a resoudre : (dans le dernier fichier "casse_brique_ensemble.cpp")
 

1) Nous ne pouvons pas utiliser la fonction "int code_touche"; nous avons essaye, mais cette modification derange le mouvement de la balle : elle se deplace beaucoup plus vite et puis "Segmentation fault (core dumped)".


2) La partie ou on detecte si la balle touche les briques, les fait disparaitre et rebondit : nous ecrivons une fonction (void balle_brique (...), nous la mettons dans les commentaire //) afin de rendre le "main" plus propre. Quand on compile le fichier, le Terminal n'avertit aucune faute, mais la balle ne rebondit plus quand elle touche les briques. C'est pourquoi, nous mettons tous les codes necessaires dans le "main".






