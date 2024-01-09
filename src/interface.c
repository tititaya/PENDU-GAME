/*
 * general.c
 * PoPS ESR v1.3
 *
 */

#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <crypt.h>
#include <ctype.h>  // Pour les fonctions de manipulation de caractères
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <stdbool.h>

#include "generale.h"
#include "interface.h"



// Définition de la fonction afficherMessage
void afficherMessage(int x, int y, const char *message) {
    gotoxy(x, y);
    printf("%s", message);
}

/*

*/

// Fonction pour afficher le menu initial
void affiche_titre(int x, int y) {
    clrscr();
    dessine_cadre();
    gotoxy(x, y - 6);
    printf("JEUX DE PENDU\n\n");
    // ... (autre code d'affichage)
}

// Fonction pour effacer le menu
void effacerMenu(int x, int y) {
    clrscr();
    dessine_cadre();
    
    gotoxy(x, y - 6);
}

/*

*/



// Dessine un cadre
void dessine_cadre() {
    int l, h;
    struct termsize taille;
    taille = gettermsize();
    int Ldebut, Hdebut;
    Hdebut = taille.cols - 80;    
    Hdebut /= 2;
    Ldebut = taille.rows - 45;
    Ldebut /= 2;
    resetterminal();
    for (h = 1; h <= 45; ++h) {
        for (l = 1; l <= 81; ++l) {
            if ((h == 1) || (h == 45) || (l == 1) || (l == 81)) {
                gotoxy(Hdebut + l, Ldebut + h);
                printf("*");
            } else {
                gotoxy(Hdebut + l, Ldebut + h);
                printf(" ");
            }
        }
    }
}




void affichePendu(int Hdebut, int Ldebut) {
    int xPendu = Hdebut + 30;
    int yPendu = Ldebut + 40;

    gotoxy(xPendu, yPendu);
    setfontcolor(38);
    printf("   _______\n");

    gotoxy(xPendu, yPendu + 1);
    printf("  |       |\n");

    gotoxy(xPendu, yPendu + 2);
    printf("  |       |\n");

    gotoxy(xPendu, yPendu + 3);
    printf("  |       O\n");

    gotoxy(xPendu, yPendu + 4);
    printf("  |      /|\\\n");

    gotoxy(xPendu, yPendu + 5);
    printf("  |      / \\\n");

    gotoxy(xPendu, yPendu + 6);
    printf(" _|_\n");
    resetcolors();
}



/*
*chargement: permet de faire une simulation du chargement
 
*/
void chargement(){char c='%';

 struct termsize taille;int Ldebut,Hdebut,r,i;
  taille = gettermsize();Hdebut=taille.cols-80;Hdebut/=2;Ldebut=taille.rows-45;Ldebut/=2;
setfontbold(1);hidecursor();
for(i=20;i<=40;++i){gotoxy(Hdebut+i,Ldebut+10);setbgrcolor(32);printf(" \n");}resetcolors();
setfontcolor(33);gotoxy(Hdebut+20,Ldebut+11);printf("CHARGEMENT EN COUR..."); resetcolors();
for(i=0;i<=20;++i){r=(i*100)/20;delai(0.16);gotoxy(Hdebut+20+i,Ldebut+10);setbgrcolor(34);printf(" \n"); resetcolors();gotoxy(Hdebut+41,Ldebut+10);printf("%d%c",r,c);}
 resetcolors();
for(i=0;i<=24;++i){gotoxy(Hdebut+20+i,Ldebut+10);printf(" \n");}

}



/*
*enregistrement: permet de faire une simulation de l'enrestrement

*/
void enregistrement(){char c='%';
 struct termsize taille;int Ldebut,Hdebut,r,i;
  taille = gettermsize();Hdebut=taille.cols-80;Hdebut/=2;Ldebut=taille.rows-45;Ldebut/=2;
setfontbold(1);hidecursor();
for(i=20;i<=40;++i){gotoxy(Hdebut+i,Ldebut+10);setbgrcolor(35);printf(" \n");}resetcolors();
setfontcolor(36);gotoxy(Hdebut+20,Ldebut+11);printf("ENREGISTREMENT EN COUR..."); resetcolors();
for(i=0;i<=20;++i){r=(i*100)/20;delai(0.16);gotoxy(Hdebut+20+i,Ldebut+10);setbgrcolor(33);printf(" \n"); resetcolors();gotoxy(Hdebut+41,Ldebut+10);printf("%d%c",r,c);}
 resetcolors();
for(i=0;i<=24;++i){gotoxy(Hdebut+20+i,Ldebut+10);printf(" \n");}
}





int choisirDestination(int x, int y) {
    int choixDestination;

    do {
        gotoxy(x -3, y);
        printf("Choisissez votre destination (1 pour le menu principal, 2 pour retour) : ");
        scanf(" %d", &choixDestination);
    char c;
        // Vider le tampon d'entrée
        while ((c = getchar()) != '\n' && c != EOF);

        if (choixDestination == 1) {
            // Revenir au menu principal
            afficherMenu();
        } else if (choixDestination == 2) {
            // Revenir au gestionnaire de comptes
            gererComptes();
        } else {
            // Gérer les choix invalides
            gotoxy(x - 8, y + 24);
            printf("Choix invalide. Revenir au Menu Principal par défaut.\n");

            // Attendre que l'utilisateur appuie sur Entrée
            while (getchar() != '\n');
        }
    } while (choixDestination != 1 && choixDestination != 2);

    return choixDestination;
}



/*


*/

// Fonction pour lancer une nouvelle partie
void nouvellePartie(int x, int y) {
    dessine_cadre();
      
int utilisateurTrouve;
gotoxy(x , y - 6);
    printf("JEUX DE PENDU\n\n");
    // Déplacement du curseur au début du menu dans le cadre
    gotoxy(x, y - 6);

    // Affichage du titre
    printf("Mode de jeu disponible\n");

    // Déplacement du curseur pour afficher les options de niveau
    gotoxy(x - 10, y);

    // Affichage des options de niveau
    printf("Choisissez un niveau de difficulte :\n");

    gotoxy(x - 20, y + 4);
    printf("1. Niveau B1");

    gotoxy(x + 20, y + 4);
    printf("2. Niveau B2");

    gotoxy(x - 20, y + 8);
    printf("3. Niveau C1");

    gotoxy(x + 20, y + 8);
    printf("4. Retour au menu principal");

    gotoxy(x, y + 12);
    printf("5. Quitter");

    gotoxy(x - 10, y + 18);

    // Demander à l'utilisateur de faire un choix
    int choixNiveau = getInt("Entrez le numero de votre choix : ");
    
    // Traiter le choix de l'utilisateur
    switch (choixNiveau) {
        case 1:
            
            affiche_titre(x, y) ;
                gotoxy(x -5, y );
            // Avant de lancer le niveau B1, demander le nom d'utilisateur
            utilisateurTrouve = 0; 
            do {
                affiche_titre(x, y);
                gotoxy(x - 5, y);

                char nomUtilisateur[50];
                getString(nomUtilisateur, sizeof(nomUtilisateur), "Entrez votre nom d'utilisateur : ");

                // Vérifier si l'utilisateur est connecté
                utilisateurTrouve = verifierUtilisateur(nomUtilisateur);

                if (utilisateurTrouve) {
                    // Utilisateur connecté, lancer le niveau B1
                    affiche_titre(x, y);
                    gotoxy(x - 5, y);
                    chargement();
                     affiche_titre(x, y);
                    jouerPartieB1(x, y+10);
                } else {
                    // Utilisateur non connecté, afficher un message
                    affiche_titre(x, y);
                    gotoxy(x - 27, y);
                    printf("Utilisateur non trouvé. Veuillez réessayer ou vous inscrire avant de jouer.\n");

                    // Ajout d'une petite pause pour laisser le temps à l'utilisateur de lire le message
                    // avant de redemander le nom d'utilisateur
                    sleep(2);
                    
                }
            } while (!utilisateurTrouve);

            break;
        case 2:
             affiche_titre(x, y) ;
                gotoxy(x -5, y );
            // Avant de lancer le niveau B1, demander le nom d'utilisateur
            utilisateurTrouve = 0; 
            do {
                affiche_titre(x, y);
                gotoxy(x - 5, y);

                char nomUtilisateur[50];
                getString(nomUtilisateur, sizeof(nomUtilisateur), "Entrez votre nom d'utilisateur : ");

                // Vérifier si l'utilisateur est connecté
                utilisateurTrouve = verifierUtilisateur(nomUtilisateur);

                if (utilisateurTrouve) {
                    // Utilisateur connecté, lancer le niveau B2
                    affiche_titre(x, y);
                    gotoxy(x - 8, y );
                    chargement();
                     affiche_titre(x, y);
                    jouerPartieB2(x, y+13);
                } else {
                    // Utilisateur non connecté, afficher un message
                    affiche_titre(x, y);
                    gotoxy(x - 27, y);
                    printf("Utilisateur non trouvé. Veuillez réessayer ou vous inscrire avant de jouer.\n");

                    // Ajout d'une petite pause pour laisser le temps à l'utilisateur de lire le message
                    // avant de redemander le nom d'utilisateur
                    sleep(2);
                    
                }
            } while (!utilisateurTrouve);
            break;
        case 3:
             affiche_titre(x, y) ;
                gotoxy(x -5, y );
            // Avant de lancer le niveau B1, demander le nom d'utilisateur
            utilisateurTrouve = 0; 
            do {
                affiche_titre(x, y);
                gotoxy(x - 5, y);

                char nomUtilisateur[50];
                getString(nomUtilisateur, sizeof(nomUtilisateur), "Entrez votre nom d'utilisateur : ");

                // Vérifier si l'utilisateur est connecté
                utilisateurTrouve = verifierUtilisateur(nomUtilisateur);

                if (utilisateurTrouve) {
                    // Utilisateur connecté, lancer le niveau B2
                    affiche_titre(x, y);
                    gotoxy(x - 5, y);
                    chargement();
                     affiche_titre(x, y);
                    //jouerPartieC1(x, y-10);
                } else {
                    // Utilisateur non connecté, afficher un message
                    affiche_titre(x, y);
                    gotoxy(x - 27, y);
                    printf("Utilisateur non trouvé. Veuillez réessayer ou vous inscrire avant de jouer.\n");

                    // Ajout d'une petite pause pour laisser le temps à l'utilisateur de lire le message
                    // avant de redemander le nom d'utilisateur
                    sleep(2);
                    
                }
            } while (!utilisateurTrouve);
            break;
        case 4:
            // Retour au menu principal
             afficherMenu();
            break;
        case 5:
            
            break;
        default:
            gotoxy(x - 10, y + 24); // Ajustement de la position du message
            printf("Choix invalide. Veuillez reessayer.\n");
            break;
    }
}





/* 
 * afficherMenu: Affiche le menu principal
 */
void afficherMenu() {
    struct termsize taille;
    taille = gettermsize();
    dessine_cadre();
    // Coordonnées pour positionner le menu au centre du cadre
    int xDebutMenu = (taille.cols - 18) / 2;  // Centré horizontalement
    int yDebutMenu = (taille.rows - 45) / 2 + 10;   // Centré verticalement
    
    gotoxy(xDebutMenu, yDebutMenu - 6);
    printf("JEUX DE PENDU\n\n");

    // Déplacement du curseur au début du menu dans le cadre
    gotoxy(xDebutMenu, yDebutMenu - 3);

    // Affichage du menu principal
    setfontbold(1);  // Activer le mode gras
    setfontcolor(34);  // Couleur du texte

    printf("    Menu\n\n");

    // Options de base
    gotoxy(xDebutMenu - 20, yDebutMenu);
    printf("1. Comptes");

    gotoxy(xDebutMenu + 20, yDebutMenu);
    printf("2. Changer de thème");

    gotoxy(xDebutMenu - 20, yDebutMenu + 3);
    printf("3. Nouvelle Partie");

    gotoxy(xDebutMenu + 20, yDebutMenu + 3);
    printf("4. Changer de Catégorie");

    gotoxy(xDebutMenu - 20, yDebutMenu + 6);
    printf("5. Scores/Statistiques");

    gotoxy(xDebutMenu + 20, yDebutMenu + 6);
    printf("6. Changer de Thème Graphique");

    gotoxy(xDebutMenu + 4, yDebutMenu + 9);
    printf("7. Quitter");

    resetcolors();
    setfontbold(0);  // Désactiver le mode gras

    // Déplacer le curseur juste en dessous du menu
    gotoxy(xDebutMenu , yDebutMenu + 11);

    // Demander à l'utilisateur de faire un choix
    int choix;
   // printf("Entrez votre choix : ");
    choix = getInt("Entrez votre choix : ");

    // Traiter le choix de l'utilisateur
    switch (choix) {
        case 1:
            gotoxy(xDebutMenu , yDebutMenu + 11);
            gererComptes();  // Appeler la fonction pour gérer les comptes
            break;
        case 2:
            // Code pour changer de thème
           
            break;
        case 3:
            nouvellePartie(xDebutMenu , yDebutMenu );  // Appeler la fonction pour une nouvelle partie
            break;
        case 4:
            // Code pour changer de catégorie
            break;
        case 5:
            // Code pour les scores/statistiques
            break;
        case 6:
            // Code pour changer de thème graphique
            break;
        case 7:
            // Retour au menu principal
                afficherMenu();
            break;
        default:
            gotoxy(xDebutMenu , yDebutMenu + 11);
            printf("Choix invalide. Veuillez réessayer.\n");
            break;
    }
}



/*


*/


void introduction(){
 struct termsize taille;int Ldebut,Hdebut,i,r,j;

   dessine_cadre();
 taille = gettermsize();Hdebut=taille.cols-80;Hdebut/=2;Ldebut=taille.rows-45;Ldebut/=2;


for(i=Ldebut+2;i<=Ldebut+22;++i){
for(r=Hdebut+2;r<=Hdebut+80;++r){gotoxy(r,i);setbgrcolor(33);printf(" \n");}resetcolors();}

for(i=Ldebut+23;i<=Ldebut+44;++i){
for(r=Hdebut+2;r<=Hdebut+80;++r){gotoxy(r,i);setbgrcolor(35);printf(" \n");}resetcolors();}
delai(2);
j=Ldebut+22;

for(i=0;i<=22;++i){delai(0.15);
if(i<21){
for(r=Hdebut+2;r<=Hdebut+80;++r){gotoxy(r,j-i);setbgrcolor(39);printf(" \n");}
}
for(r=Hdebut+2;r<=Hdebut+80;++r){gotoxy(r,j+i);setbgrcolor(39);printf(" \n");}

if(i==1){gotoxy(Hdebut+25,j);setfontcolor(38);delai(0.15);printf("BIENVENUE DANS LE JEU DE PENDU\n");}


else if(i==10){gotoxy(Hdebut+7,j+i);setfontcolor(32);printf("PLAYER1\n");
gotoxy(Hdebut+67,j+i);setfontcolor(12);printf("PLAYER2\n");
}

}

chargement();
enregistrement();

    // Afficher le pendu
   affichePendu(Hdebut+35,Ldebut-3);
  affichePendu(Hdebut-25,Ldebut-3);
//afficherMenu();
 gotoxy(Hdebut + 25, Ldebut + 26);
    // Attendre avant de revenir au menu des comptes
    printf("Appuyez sur Entrée pour continuer...");
 // Attendre que l'utilisateur appuie sur Entrée
        while (getchar() != '\n');
 afficherMenu();
    delai(2);
resetcolors();

}/*

//wrestlinng   faire du catch 


*/



void afficherEtatJeu(JeuPendu jeu, int x, int y) {
    

    // Afficher l'état actuel du jeu à des coordonnées spécifiques
    gotoxy(x - 5, y);
    printf("Mot à deviner : %s", jeu.motAffiche);
    gotoxy(x -25 , y + 3);
    printf("Tentatives restantes : %d   ", jeu.tentativesRestantes);
    gotoxy(x -25, y + 5);
    printf("Lettres utilisées : ");
    for (int i = 0; i < 26; i++) {
        if (jeu.lettresUtilisees[i] != 0) {
            printf("%c ", jeu.lettresUtilisees[i]);
        }
    }
    gotoxy(x -30, y + 4);
}


/*



*//
// Fonction pour afficher le pendu en fonction du nombre d'essais restants
void afficherPendu(int tentativesRestantes, int x, int y, int animationFrame) {
  

    // Afficher le pendu en fonction du nombre d'essais restants
    switch (tentativesRestantes) {
        case 6:
            gotoxy(x + 2, y);
            printf("  ------");
            gotoxy(x + 2, y + 1);
            printf("  |    |");
            gotoxy(x + 2, y + 2);
            printf("       |");
            gotoxy(x + 2, y + 3);
            printf("       |");
            gotoxy(x + 2, y + 4);
            printf("       |");
            gotoxy(x + 2, y + 5);
            printf("       |");
            gotoxy(x + 2, y + 6);
            printf("=========");
            break;
        case 5:
            gotoxy(x + 2, y);
            printf("  ------");
            gotoxy(x + 2, y + 1);
            printf("  |    |");
            gotoxy(x + 2, y + 2);
            printf("  O    |");
            gotoxy(x + 2, y + 3);
            printf("       |");
            gotoxy(x + 2, y + 4);
            printf("       |");
            gotoxy(x + 2, y + 5);
            printf("       |");
            gotoxy(x + 2, y + 6);
            printf("=========");
            break;
        case 4:
            gotoxy(x + 2, y);
            printf("  ------");
            gotoxy(x + 2, y + 1);
            printf("  |    |");
            gotoxy(x + 2, y + 2);
            printf("  O    |");
            gotoxy(x + 2, y + 3);
            printf("  |    |");
            gotoxy(x + 2, y + 4);
            printf("       |");
            gotoxy(x + 2, y + 5);
            printf("       |");
            gotoxy(x + 2, y + 6);
            printf("=========");
            break;
        case 3:
            gotoxy(x + 2, y);
            printf("  ------");
            gotoxy(x + 2, y + 1);
            printf("  |    |");
            gotoxy(x + 2, y + 2);
            printf("  O    |");
            gotoxy(x + 2, y + 3);
            printf(" /|    |");
            gotoxy(x + 2, y + 4);
            printf("       |");
            gotoxy(x + 2, y + 5);
            printf("       |");
            gotoxy(x + 2, y + 6);
            printf("=========");
            break;
        case 2:
            gotoxy(x + 2, y);
            printf("  ------");
            gotoxy(x + 2, y + 1);
            printf("  |    |");
            gotoxy(x + 2, y + 2);
            printf("  O    |");
            gotoxy(x + 2, y + 3);
            printf(" /|\\   |");
            gotoxy(x + 2, y + 4);
            printf("       |");
            gotoxy(x + 2, y + 5);
            printf("       |");
            gotoxy(x + 2, y + 6);
            printf("=========");
            break;
        case 1:
            gotoxy(x + 2, y);
            printf("  ------");
            gotoxy(x + 2, y + 1);
            printf("  |    |");
            gotoxy(x + 2, y + 2);
            printf("  O    |");
            gotoxy(x + 2, y + 3);
            printf(" /|\\   |");
            gotoxy(x + 2, y + 4);
            printf(" /     |");
            gotoxy(x + 2, y + 5);
            printf("       |");
            gotoxy(x + 2, y + 6);
            printf("=========");
            break;
        case 0:
            gotoxy(x + 2, y);
            printf("  ------");
            gotoxy(x + 2, y + 1);
            printf("  |    |");
            gotoxy(x + 2, y + 2);
            printf("  O    |");
            gotoxy(x + 2, y + 3);
            printf(" /|\\   |");
            gotoxy(x + 2, y + 4);
            printf(" / \\   |");
            gotoxy(x + 2, y + 5);
            printf("       |");
            gotoxy(x + 2, y + 6);
            printf("=========");
            break;
         case -1: // Joueur gagne, animation avec les bras et les pieds levés
            gotoxy(x + 2, y);
            printf("  ------");
            gotoxy(x + 2, y + 1);
            printf("  |    |");
            gotoxy(x + 2, y + 2);
            printf("  O    |");
            gotoxy(x + 2, y + 3);
            printf(" /|\\   |");
            gotoxy(x + 2, y + 4);
            printf(" / \\   |");
            gotoxy(x + 2, y + 5);
            printf("       |");
            gotoxy(x + 2, y + 6);
            printf("=========");

            // Animation des bras et des pieds
            if (animationFrame % 2 == 0) {
                gotoxy(x + 5, y + 2);
                printf("\\o/"); // Bras levés
                gotoxy(x + 5, y + 4);
                printf("/ \\"); // Jambes levées
            } else {
                gotoxy(x + 5, y + 2);
                printf(" | "); // Bras baissés
                gotoxy(x + 5, y + 4);
                printf(" | "); // Jambes baissées
            }
            break;
    }
}


/***

*//

// Fonction pour demander à l'utilisateur s'il veut rejouer
bool voulezRejouer(int x, int y) {
    struct termsize taille;
    taille = gettermsize();
    dessine_cadre();
    
    int xDebutMenu = (taille.cols - 18) / 2;  // Centré horizontalement
    int yDebutMenu = (taille.rows - 45) / 2 + 10;   // Centré verticalement
    
    gotoxy(xDebutMenu, yDebutMenu - 6);
    char reponse;
    printf("\n\nVoulez-vous jouer à nouveau ? (O/N): ");
    scanf(" %c", &reponse);

    // Ignorer les caractères supplémentaires entrés par l'utilisateur
    while (getchar() != '\n');

    return (reponse == 'O' || reponse == 'o');
}



/*



*/

// Fonction pour jouer une partie au niveau B1
void jouerPartieB1(int xDebutCadre, int yDebutCadre) {
    JeuPendu jeu;


 int xPendu = xDebutCadre;  
    int yPendu = yDebutCadre + 5;  
   gotoxy(xDebutCadre, yDebutCadre - 15);
    printf("NIVEAU SIMPLE\n\n");

    // Choisi r un mot aléatoire pour la partie
    strcpy(jeu.motSecret, choisirMotAleatoire("niveau_B1.txt"));

    // Initialiser le mot affiché avec des underscores
    int longueurMot = strlen(jeu.motSecret);
    for (int i = 0; i < longueurMot; i++) {
        jeu.motAffiche[i] = '_';
    }
    jeu.motAffiche[longueurMot] = '\0';

    // Initialiser le reste de la structure
    jeu.tentativesMax = 7;  // Vous pouvez ajuster ce nombre
    jeu.tentativesRestantes = jeu.tentativesMax;
    memset(jeu.lettresUtilisees, 0, sizeof(jeu.lettresUtilisees));

    // Boucle principale du jeu
    while (1) {
        // Demander à l'utilisateur de deviner une lettre
        char lettre;
        do {
            // Obtenir la taille de la console
            struct termsize taille = gettermsize();

            // Centrer le jeu dans la console
            xPendu = (taille.cols - 25) / 2;  // 25 est la largeur du pendu
            yPendu = 5;  // Position verticale fixe

            // Afficher l'état actuel du jeu (mot affiché, tentatives restantes, lettres utilisées)
            //gotoxy(xDebutCadre, yDebutCadre);
            afficherEtatJeu(jeu, xDebutCadre, yDebutCadre);

            // Afficher le pendu à sa position dans la console
            afficherPendu(jeu.tentativesRestantes, xPendu , yPendu + 5 , 4);

            // Afficher le message à des positions spécifiques
            gotoxy(xDebutCadre , yDebutCadre + 9);
            printf("Devinez une lettre :    ");

            lettre = getLettre(" ");
            if (lettreDejaUtilisee(lettre, jeu.lettresUtilisees)) {
                // Afficher le message à des positions spécifiques
                gotoxy(xDebutCadre - 15, yDebutCadre + 20);
                printf("Vous avez déjà deviné cette lettre. Veuillez réessayer.  ");
               
                delai(0.2);dessine_cadre();
                    
                        
            }
        } while (lettreDejaUtilisee(lettre, jeu.lettresUtilisees));

        // Marquer la lettre comme utilisée
        marquerLettreUtilisee(lettre, jeu.lettresUtilisees);

        // Vérifier si la lettre est dans le mot secret
        if (lettreDansMot(lettre, jeu.motSecret)) {
            // Afficher le message à des positions spécifiques
            gotoxy(xDebutCadre - 15, yDebutCadre + 20);
            printf("Bonne devinette ! ");

            // Mettre à jour le mot affiché
            mettreAJourMotAffiche(lettre, jeu.motSecret, jeu.motAffiche);
        } else {
            // Afficher le message à des positions spécifiques
            gotoxy(xDebutCadre - 15, yDebutCadre + 20);
            printf("Mauvaise devinette. Vous perdez une tentative. ");
            clrline();
                delai(0.5);dessine_cadre();
            // Décrémenter les tentatives restantes
            jeu.tentativesRestantes--;
        }

        // Vérifier si le joueur a gagné ou perdu
        if (motDevinerTrouve(jeu.motSecret, jeu.motAffiche)) {
            // Afficher l'état final du jeu
            affiche_titre(xDebutCadre , yDebutCadre);
            afficherEtatJeu(jeu, xDebutCadre , yDebutCadre);
            afficherPendu(jeu.tentativesRestantes, xPendu, yPendu + 6  , 4);
            // Afficher le message à des positions spécifiques
            gotoxy(xDebutCadre -25 , yDebutCadre + 20);
            printf("Félicitations ! Vous avez deviné le mot : %s    ", jeu.motSecret);
            
                       // Demander à l'utilisateur s'il veut rejouer
                if (voulezRejouer(xDebutCadre - 25, yDebutCadre + 22)) {
                    affiche_titre(xDebutCadre, yDebutCadre+10);
                    jouerPartieB1(xDebutCadre,  yDebutCadre+10);// Redémarrer une nouvelle partie
                    
                } else {
                    nouvellePartie(xDebutCadre, yDebutCadre);// Quitter le jeu
                    break;
                }
        } else if (jeu.tentativesRestantes == 0) {
            // Afficher l'état final du jeu
            afficherEtatJeu(jeu, xDebutCadre, yDebutCadre);
            afficherPendu(jeu.tentativesRestantes, xPendu, yPendu + 2 , 4);
            // Afficher le message à des positions spécifiques
            gotoxy(xDebutCadre - 25, yDebutCadre + 22);
            printf("Désolé, vous avez épuisé toutes vos tentatives. Le mot était : %s", jeu.motSecret);
                        // Demander à l'utilisateur s'il veut rejouer
                if (voulezRejouer(xDebutCadre - 25, yDebutCadre + 23)) {
                    // Redémarrer une nouvelle partie
                    affiche_titre(xDebutCadre, yDebutCadre+10);
                    jouerPartieB1(xDebutCadre, yDebutCadre);
                        
                } else {
                    nouvellePartie(xDebutCadre, yDebutCadre);// Quitter le jeu
                    break;
                }
        }
    }
}


/*


*/


// Fonction pour jouer une partie au niveau B2
void jouerPartieB2(int xDebutCadre, int yDebutCadre) {
    JeuPendu jeu;

    int xPendu = xDebutCadre;
    int yPendu = yDebutCadre + 5;

    gotoxy(xDebutCadre, yDebutCadre - 15);
    printf("NIVEAU INTERMEDIAIRE\n\n");

    // Choisi r un mot aléatoire pour la partie
    strcpy(jeu.motSecret, choisirMotAleatoire("niveau_B2.txt"));

    // Initialiser le mot affiché avec des underscores
    int longueurMot = strlen(jeu.motSecret);
    for (int i = 0; i < longueurMot; i++) {
        jeu.motAffiche[i] = '_';
    }
    jeu.motAffiche[longueurMot] = '\0';

    // Initialiser le reste de la structure
    jeu.tentativesMax = 7;  // Vous pouvez ajuster ce nombre
    jeu.tentativesRestantes = jeu.tentativesMax;
    memset(jeu.lettresUtilisees, 0, sizeof(jeu.lettresUtilisees));

    // Boucle principale du jeu
    while (1) {
        // Demander à l'utilisateur de deviner une lettre
        char lettre;
        do {
            // Obtenir la taille de la console
            struct termsize taille = gettermsize();

            // Centrer le jeu dans la console
            xPendu = (taille.cols - 25) / 2;  // 25 est la largeur du pendu
            yPendu = 5;  // Position verticale fixe

            // Afficher l'état actuel du jeu (mot affiché, tentatives restantes, lettres utilisées)
            afficherEtatJeu(jeu, xDebutCadre, yDebutCadre);

            // Afficher le pendu à sa position dans la console
            afficherPendu(jeu.tentativesRestantes, xPendu, yPendu + 5, 4);

            // Afficher le message à des positions spécifiques
            gotoxy(xDebutCadre, yDebutCadre + 9);
            printf("Devinez une lettre :    ");

            lettre = getLettre(" ");
            if (lettreDejaUtilisee(lettre, jeu.lettresUtilisees)) {
                // Afficher le message à des positions spécifiques
                gotoxy(xDebutCadre - 15, yDebutCadre + 20);
                printf("Vous avez déjà deviné cette lettre. Veuillez réessayer.  ");

                delai(0.2);
                dessine_cadre();
            }
        } while (lettreDejaUtilisee(lettre, jeu.lettresUtilisees));

        // Marquer la lettre comme utilisée
        marquerLettreUtilisee(lettre, jeu.lettresUtilisees);

        // Vérifier si la lettre est dans le mot secret
        if (lettreDansMot(lettre, jeu.motSecret)) {
            // Afficher le message à des positions spécifiques
            gotoxy(xDebutCadre - 15, yDebutCadre + 20);
            printf("Bonne devinette ! ");

            // Mettre à jour le mot affiché
            mettreAJourMotAffiche(lettre, jeu.motSecret, jeu.motAffiche);
        } else {
            // Afficher le message à des positions spécifiques
            gotoxy(xDebutCadre - 15, yDebutCadre + 20);
            printf("Mauvaise devinette. Vous perdez une tentative. ");
            clrline();
            delai(0.5);
            dessine_cadre();
            // Décrémenter les tentatives restantes
            jeu.tentativesRestantes--;
        }

        // Vérifier si le joueur a gagné ou perdu
        if (motDevinerTrouve(jeu.motSecret, jeu.motAffiche)) {
            // Afficher l'état final du jeu
            affiche_titre(xDebutCadre, yDebutCadre);
            afficherEtatJeu(jeu, xDebutCadre, yDebutCadre);
            afficherPendu(jeu.tentativesRestantes, xPendu, yPendu + 6, 4);
            // Afficher le message à des positions spécifiques
            gotoxy(xDebutCadre - 25, yDebutCadre + 20);
           printf("Félicitations ! Vous avez deviné le mot : %s          ", jeu.motSecret);

            // Demander à l'utilisateur s'il veut rejouer
            if (voulezRejouer(xDebutCadre - 25, yDebutCadre + 22)) {
                affiche_titre(xDebutCadre, yDebutCadre + 10);
                jouerPartieB2(xDebutCadre, yDebutCadre);// Redémarrer une nouvelle partie
            } else {
                nouvellePartie(xDebutCadre, yDebutCadre);// Quitter le jeu
                break;
            }
        } else if (jeu.tentativesRestantes == 0) {
            // Afficher l'état final du jeu
            afficherEtatJeu(jeu, xDebutCadre, yDebutCadre);
            afficherPendu(jeu.tentativesRestantes, xPendu, yPendu + 2, 4);
            // Afficher le message à des positions spécifiques
            gotoxy(xDebutCadre - 25, yDebutCadre + 22);
            printf("Désolé, vous avez épuisé toutes vos tentatives. Le mot était : %s", jeu.motSecret);

            // Demander à l'utilisateur s'il veut rejouer
            if (voulezRejouer(xDebutCadre - 25, yDebutCadre + 23)) {
                // Redémarrer une nouvelle partie
                affiche_titre(xDebutCadre, yDebutCadre + 10);
                jouerPartieB2(xDebutCadre, yDebutCadre);
            } else {
                nouvellePartie(xDebutCadre, yDebutCadre);// Quitter le jeu
                break;
            }
        }
    }
}

/**

*/



// Fonction pour jouer une partie au niveau C1
void jouerPartieC1(int xDebutCadre, int yDebutCadre) {
    JeuPendu jeu;

    int xPendu = xDebutCadre;
    int yPendu = yDebutCadre + 5;

    gotoxy(xDebutCadre, yDebutCadre - 15);
    printf("NIVEAU AVANCE\n\n");

    // Choisi r un mot aléatoire pour la partie
    strcpy(jeu.motSecret, choisirMotAleatoire("niveau_C1.txt"));

    // Initialiser le mot affiché avec des underscores
    int longueurMot = strlen(jeu.motSecret);
    for (int i = 0; i < longueurMot; i++) {
        jeu.motAffiche[i] = '_';
    }
    jeu.motAffiche[longueurMot] = '\0';

    // Initialiser le reste de la structure
    jeu.tentativesMax = 7;  // Vous pouvez ajuster ce nombre
    jeu.tentativesRestantes = jeu.tentativesMax;
    memset(jeu.lettresUtilisees, 0, sizeof(jeu.lettresUtilisees));

    // Boucle principale du jeu
    while (1) {
        // Demander à l'utilisateur de deviner une lettre
        char lettre;
        do {
            // Obtenir la taille de la console
            struct termsize taille = gettermsize();

            // Centrer le jeu dans la console
            xPendu = (taille.cols - 25) / 2;  // 25 est la largeur du pendu
            yPendu = 5;  // Position verticale fixe

            // Afficher l'état actuel du jeu (mot affiché, tentatives restantes, lettres utilisées)
            afficherEtatJeu(jeu, xDebutCadre, yDebutCadre);

            // Afficher le pendu à sa position dans la console
            afficherPendu(jeu.tentativesRestantes, xPendu, yPendu + 5, 4);

            // Afficher le message à des positions spécifiques
            gotoxy(xDebutCadre, yDebutCadre + 9);
            printf("Devinez une lettre :    ");

            lettre = getLettre(" ");
            if (lettreDejaUtilisee(lettre, jeu.lettresUtilisees)) {
                // Afficher le message à des positions spécifiques
                gotoxy(xDebutCadre - 15, yDebutCadre + 20);
                printf("Vous avez déjà deviné cette lettre. Veuillez réessayer.  ");

                delai(0.2);
                dessine_cadre();
            }
        } while (lettreDejaUtilisee(lettre, jeu.lettresUtilisees));

        // Marquer la lettre comme utilisée
        marquerLettreUtilisee(lettre, jeu.lettresUtilisees);

        // Vérifier si la lettre est dans le mot secret
        if (lettreDansMot(lettre, jeu.motSecret)) {
            // Afficher le message à des positions spécifiques
            gotoxy(xDebutCadre - 15, yDebutCadre + 20);
            printf("Bonne devinette ! ");

            // Mettre à jour le mot affiché
            mettreAJourMotAffiche(lettre, jeu.motSecret, jeu.motAffiche);
        } else {
            // Afficher le message à des positions spécifiques
            gotoxy(xDebutCadre - 15, yDebutCadre + 20);
            printf("Mauvaise devinette. Vous perdez une tentative. ");
            clrline();
            delai(0.5);
            dessine_cadre();
            // Décrémenter les tentatives restantes
            jeu.tentativesRestantes--;
        }

        // Vérifier si le joueur a gagné ou perdu
        if (motDevinerTrouve(jeu.motSecret, jeu.motAffiche)) {
            // Afficher l'état final du jeu
            affiche_titre(xDebutCadre, yDebutCadre);
            afficherEtatJeu(jeu, xDebutCadre, yDebutCadre);
            afficherPendu(jeu.tentativesRestantes, xPendu, yPendu + 6, 4);
            // Afficher le message à des positions spécifiques
            gotoxy(xDebutCadre - 25, yDebutCadre + 20);
            printf("Félicitations ! Vous avez deviné le mot : %s          ", jeu.motSecret);


            // Demander à l'utilisateur s'il veut rejouer
            if (voulezRejouer(xDebutCadre - 25, yDebutCadre + 22)) {
                affiche_titre(xDebutCadre, yDebutCadre + 10);
                jouerPartieC1(xDebutCadre, yDebutCadre);// Redémarrer une nouvelle partie
            } else {
                nouvellePartie(xDebutCadre, yDebutCadre);// Quitter le jeu
                break;
            }
        } else if (jeu.tentativesRestantes == 0) {
            // Afficher l'état final du jeu
            afficherEtatJeu(jeu, xDebutCadre, yDebutCadre);
            afficherPendu(jeu.tentativesRestantes, xPendu, yPendu + 2, 4);
            // Afficher le message à des positions spécifiques
            gotoxy(xDebutCadre - 25, yDebutCadre + 22);
            printf("Désolé, vous avez épuisé toutes vos tentatives. Le mot était : %s", jeu.motSecret);
            // Demander à l'utilisateur s'il veut rejouer
            if (voulezRejouer(xDebutCadre - 25, yDebutCadre + 23)) {
                // Redémarrer une nouvelle partie
                affiche_titre(xDebutCadre, yDebutCadre + 10);
                jouerPartieC1(xDebutCadre, yDebutCadre);
            } else {
                nouvellePartie(xDebutCadre, yDebutCadre);// Quitter le jeu
                break;
            }
        }
    }
}








 

