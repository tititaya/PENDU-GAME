#ifndef UTILISATEUR_H
#define UTILISATEUR_H

#include "interfac.h"

#include "general.h"

// Structure pour le compte utilisateur
struct Compte {
    char nom[50];
    char prenom[50];
    char username[50];
    char motDePasse[50];
};

// Prototypes des fonctions liées aux utilisateurs
void enregistrerCompte(struct Compte *nouveauCompte);
int choisirDestination(int x, int y);
void saisirNomOuPrenom(char *champ, const char *message, int x, int y);

// Fonction pour saisir un nom d'utilisateur et vérifier les caractères spéciaux
void saisirNomUtilisateur(char *username, int x, int y);

// Fonction pour saisir un mot de passe en masquant l'entrée
void saisirMotDePasse(char *motDePasse, int x, int y) ;

void infoIdentification(struct Compte *nouveauCompte, int x, int y);

// Fonction pour vérifier si un utilisateur existe dans le fichier de comptes
int verifierUtilisateur(const char* nomUtilisateur);

// Fonction pour gérer les comptes utilisateurs
void creerNouveauCompte();
void gererComptes();

void genererSel(unsigned char *sel, size_t taille);




#endif // UTILISATEUR_H

