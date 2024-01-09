#ifndef INTERFACE_H
#define INTERFACE_H

#include "general.h"
#include "interface.h"

// Structure pour stocker l'état du jeu
typedef struct {
    char motSecret[50];              // Mot à deviner
    char motAffiche[50];             // Mot affiché avec les lettres trouvées
    int tentativesMax;               // Nombre maximum de tentatives autorisées
    int tentativesRestantes;         // Nombre de tentatives restantes
    char lettresUtilisees[26];       // Lettres déjà utilisées
} JeuPendu;


// Définition de la fonction afficherMessage
void afficherMessage(int x, int y, const char *message);

// Dessine un cadre
void dessine_cadre();

void affichePendu(int Hdebut, int Ldebut);
/*
*chargement: permet de faire une simulation du chargement
 
*/
void chargement();
/*
*enregistrement: permet de faire une simulation de l'enrestrement

*/
void enregistrement();

/*.............................................................affiche le menu et autres----------------*/

int choisirDestination(int x, int y);


// Fonction pour afficher le menu initial
void affiche_titre(int x, int y);

// Fonction pour effacer le menu
void effacerMenu(int x, int y);
// Fonction pour lancer une nouvelle partie
void nouvellePartie(int x, int y);

/* 
 * afficherMenu: Affiche le menu principal
 */
void afficherMenu();

void afficherEtatJeu(JeuPendu jeu, int x, int y);            ///afffiche l'etat du jeu

// Fonction pour afficher le pendu en fonction du nombre d'essais restants
void afficherPendu(int tentativesRestantes, int x, int y, int animationFrame);
 //Fonction pour demander à l'utilisateur s'il veut rejouer
bool voulezRejouer(int x, int y);

// Fonction pour jouer une partie au niveau B1
void jouerPartieB1(int xDebutCadre, int yDebutCadre);
// Fonction pour jouer une partie au niveau B2
void jouerPartieB2(int xDebutCadre, int yDebutCadre) ;



#endif /*UTILISATEUR_H */


















