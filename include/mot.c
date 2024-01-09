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

#include "general.h"
#include "interface.h"

// Fonction pour vérifier si une chaîne contient uniquement des lettres
int estAlpha(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i])) {
            return 0;  // Non alphabétique trouvé
        }
    }
    return 1;  // Tous les caractères sont alphabétiques
}

// Fonction pour vérifier si une chaîne contient uniquement des lettres et des caractères spéciaux autorisés
int estAlphaAvecCaracteresSpeciaux(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i]) && str[i] != '@' && str[i] != '#' && str[i] != '$' && str[i] != '~') {
            return 0;  // Caractère non autorisé trouvé
        }
    }
    return 1;  // Tous les caractères sont autorisés
}




char getLettre(const char *message) {
    char lettre;

    // Demander à l'utilisateur de deviner une lettre
    printf("%s", message);
    scanf(" %c", &lettre);

    // Ignorer les caractères supplémentaires dans le buffer
    while (getchar() != '\n');

    return lettre;
}



// Fonction pour vérifier si une lettre a déjà été utilisée
int lettreDejaUtilisee(char lettre, const char lettresUtilisees[]) {
    for (int i = 0; i < 26; i++) {
        if (lettresUtilisees[i] == lettre) {
            return 1;  // Lettre déjà utilisée
        }
    }
    return 0;  // Lettre non utilisée
}




// Fonction pour choisir un mot aléatoire à partir d'un fichier
char *choisirMotAleatoire(const char *nomFichier) {
    // Ouvrir le fichier en mode lecture
    FILE *fichier = fopen(nomFichier, "r");

    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Compter le nombre de mots dans le fichier
    int nombreDeMots = 0;
    char mot[50];

    while (fscanf(fichier, "%s", mot) == 1) {
        nombreDeMots++;
    }

    // Choisir un mot aléatoire en se positionnant au hasard dans le fichier
    srand(time(NULL));
    int motAleatoire = rand() % nombreDeMots;

    // Retourner au début du fichier
    fseek(fichier, 0, SEEK_SET);

    // Lire les mots jusqu'à atteindre le mot choisi aléatoirement
    for (int i = 0; i < motAleatoire; i++) {
        fscanf(fichier, "%s", mot);
    }

    // Fermer le fichier
    fclose(fichier);

    // Allouer de la mémoire pour le mot choisi
    char *motChoisi = strdup(mot);

    if (motChoisi == NULL) {
        perror("Erreur lors de l'allocation de mémoire");
        exit(EXIT_FAILURE);
    }

    return motChoisi;
}




// Fonction pour marquer une lettre comme utilisée
void marquerLettreUtilisee(char lettre, char lettresUtilisees[]) {
    for (int i = 0; i < 26; i++) {
        if (lettresUtilisees[i] == 0) {
            lettresUtilisees[i] = lettre;
            break;
        }
    }
}

// Fonction pour vérifier si une lettre est dans le mot secret
int lettreDansMot(char lettre, const char motSecret[]) {
    for (int i = 0; motSecret[i] != '\0'; i++) {
        if (motSecret[i] == lettre) {
            return 1;  // Lettre trouvée
        }
    }
    return 0;  // Lettre non trouvée
}

// Fonction pour mettre à jour le mot affiché avec une lettre trouvée
void mettreAJourMotAffiche(char lettre, const char motSecret[], char motAffiche[]) {
    for (int i = 0; motSecret[i] != '\0'; i++) {
        if (motSecret[i] == lettre) {
            motAffiche[i] = lettre;
        }
    }
}

// Fonction pour vérifier si le mot a été entièrement deviné
int motDevinerTrouve(const char motSecret[], const char motAffiche[]) {
    return strcmp(motSecret, motAffiche) == 0;
}


// Fonction pour lire une ligne depuis l'entrée standard

void readLine(char *buffer, int bufferSize) {
    fgets(buffer, bufferSize, stdin);
    // Supprimer le saut de ligne final, s'il est présent
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}



// Fonction pour obtenir une chaîne sécurisée
void getString(char *str, int maxSize, const char *prompt) {
    printf("%s", prompt);
    readLine(str, maxSize);
}


// Fonction pour obtenir un entier sécurisé
int getInt(const char *prompt) {
    int value;
    char buffer[256]; // Taille maximale du tampon

    do {
        getString(buffer, sizeof(buffer), prompt);
    } while (sscanf(buffer, "%d", &value) != 1);

    return value;
}

// Fonction pour saisir une chaîne et vérifier si elle ne contient que des lettres
void saisirNomOuPrenom(char *champ, const char *message, int x, int y) {
    do {
        gotoxy(x, y);
        printf("%s", message);
        getString(champ, sizeof(champ), "");
    } while (!estAlpha(champ));
}



