/*
 * mot.h
 * PoPS ESR v1.3
 *
 */
#ifndef MOT_H
#define MOT_H

#include "general.h"
#include "interface.h"




// Fonction pour vérifier si une chaîne contient uniquement des lettres
int estAlpha(const char *str);


// Déclaration de la fonction pour obtenir une lettre de l'utilisateur
char getLettre(const char *message);
char *choisirMotAleatoire(const char *nomFichier);


// Fonction pour marquer une lettre comme utilisée
void marquerLettreUtilisee(char lettre, char lettresUtilisees[]);;
// Fonction pour mettre à jour le mot affiché avec une lettre trouvée
void mettreAJourMotAffiche(char lettre, const char motSecret[], char motAffiche[]);


// Fonction pour vérifier si le mot a été entièrement deviné
int motDevinerTrouve(const char motSecret[], const char motAffiche[]);


// Définition de la fonction afficherMessage
void afficherMessage(int x, int y, const char *message);


///propre au programme



// Fonction pour choisir un mot aléatoire à partir d'un fichier
char *choisirMotAleatoire(const char *nomFichier);

// Fonction pour vérifier si une lettre a déjà été utilisée
int lettreDejaUtilisee(char lettre, const char lettresUtilisees[]);

// Fonction pour marquer une lettre comme utilisée
void marquerLettreUtilisee(char lettre, char lettresUtilisees[]);

// Fonction pour vérifier si une lettre est dans le mot secret
int lettreDansMot(char lettre, const char motSecret[]);

// Fonction pour mettre à jour le mot affiché avec une lettre trouvée
void mettreAJourMotAffiche(char lettre, const char motSecret[], char motAffiche[]);

// Fonction pour vérifier si le mot a été entièrement deviné
int motDevinerTrouve(const char motSecret[], const char motAffiche[]);


int estAlpha(const char *str);
int estAlphaAvecCaracteresSpeciaux(const char *str);


// Fonction pour lire une ligne depuis l'entrée standard

void readLine(char *buffer, int bufferSize);

// Fonction pour obtenir une chaîne sécurisée
void getString(char *str, int maxSize, const char *prompt);

// Fonction pour obtenir un entier sécurisé
int getInt(const char *prompt);
void saisirNomOuPrenom(char *champ, const char *message, int x, int y);




#endif /* MOT_H */

