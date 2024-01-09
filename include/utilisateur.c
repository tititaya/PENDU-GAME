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


// Fonction pour saisir un nom d'utilisateur et vérifier les caractères spéciaux
void saisirNomUtilisateur(char *username, int x, int y) {
    do {
        gotoxy(x, y);
        printf("Nom d'utilisateur : ");
        getString(username, sizeof(username), "");
    } while (!estAlphaAvecCaracteresSpeciaux(username));
}


// Fonction pour saisir un mot de passe en masquant l'entrée
void saisirMotDePasse(char *motDePasse, int x, int y) {
    gotoxy(x, y);
    printf("Mot de passe : ");
    strcpy(motDePasse, getpass(""));
}


*/
void genererSel(unsigned char *sel, size_t taille) {
    if (RAND_bytes(sel, taille) != 1) {
        fprintf(stderr, "Erreur lors de la génération du sel\n");
        exit(EXIT_FAILURE);
    }
}



void enregistrerCompte(struct Compte *nouveauCompte) {
    FILE *fichierComptes = fopen("comptes.txt", "a");

    if (fichierComptes == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Générer un sel aléatoire pour le hachage
    unsigned char salt[16];
    genererSel(salt, sizeof(salt));

    // Hacher le mot de passe avec SHA-256
    unsigned char hashMotDePasse[SHA256_DIGEST_LENGTH];
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();

    if (mdctx == NULL) {
        fprintf(stderr, "Erreur lors de la création du contexte de hachage\n");
        fclose(fichierComptes);
        exit(EXIT_FAILURE);
    }

    if (EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL) != 1 ||
        EVP_DigestUpdate(mdctx, salt, sizeof(salt)) != 1 ||
        EVP_DigestUpdate(mdctx, nouveauCompte->motDePasse, strlen(nouveauCompte->motDePasse)) != 1 ||
        EVP_DigestFinal_ex(mdctx, hashMotDePasse, NULL) != 1) {
        fprintf(stderr, "Erreur lors du hachage du mot de passe\n");
        EVP_MD_CTX_free(mdctx);
        fclose(fichierComptes);
        exit(EXIT_FAILURE);
    }

    EVP_MD_CTX_free(mdctx);

    // Convertir le sel et le hachage en représentation hexadécimale pour stockage
    char hexSalt[sizeof(salt) * 2 + 1];
    char hexHash[sizeof(hashMotDePasse) * 2 + 1];

    // Utiliser strncpy pour copier le sel
    strncpy(hexSalt, "", sizeof(hexSalt));
    for (size_t i = 0; i < sizeof(salt); i++) {
        snprintf(hexSalt + 2 * i, 3, "%02x", salt[i]);
    }
    hexSalt[sizeof(hexSalt) - 1] = '\0'; // Assurer la null-termination

    // Utiliser strncpy pour copier le hachage
    strncpy(hexHash, "", sizeof(hexHash));
    for (size_t i = 0; i < sizeof(hashMotDePasse); i++) {
        snprintf(hexHash + 2 * i, 3, "%02x", hashMotDePasse[i]);
    }
    hexHash[sizeof(hexHash) - 1] = '\0'; // Assurer la null-termination

    // Écrire les informations du compte dans le fichier
    if (fprintf(fichierComptes, "%s %s %s %s\n", nouveauCompte->nom, nouveauCompte->prenom, nouveauCompte->username, hexHash) < 0) {
        perror("Erreur lors de l'écriture dans le fichier");
        exit(EXIT_FAILURE);
    }

    // Fermer le fichier
    fclose(fichierComptes);
}



/*

*/


// Fonction principale pour saisir les informations d'identification
void infoIdentification(struct Compte *nouveauCompte, int x, int y) {
    // Effacer l'écran complet
    clrscr();
    // Redessiner le cadre
    dessine_cadre();

    // Afficher le sous-menu des comptes
    struct termsize taille;
    taille = gettermsize();

    // Coordonnées pour positionner le contenu au centre du cadre
    int xDebutContenu = (taille.cols - 18) / 2;  // Centré horizontalement
    int yDebutContenu = (taille.rows - 45) / 2 + 10;   // Centré verticalement
    gotoxy(xDebutContenu , yDebutContenu - 6);
    printf("JEUX DE PENDU\n\n");
    gotoxy(xDebutContenu - 10, yDebutContenu - 3);
    printf("BIENVENUE DANS L'ESPACE DE CRÉATION DE COMPTE\n\n");

    // Vider le tampon d'entrée
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    // Appeler les fonctions pour saisir les informations
    saisirNomOuPrenom(nouveauCompte->nom, "Nom : ", xDebutContenu - 10, yDebutContenu + 11);
    saisirNomOuPrenom(nouveauCompte->prenom, "Prénom : ", xDebutContenu - 10, yDebutContenu + 13);
    saisirNomUtilisateur(nouveauCompte->username, xDebutContenu - 10, yDebutContenu + 15);
    saisirMotDePasse(nouveauCompte->motDePasse, xDebutContenu - 10, yDebutContenu + 18);

    // Enregistrement du compte
    enregistrerCompte(nouveauCompte);
    enregistrement();

    // Effacer l'écran complet
    clrscr();
    // Redessiner le cadre
    dessine_cadre();
    gotoxy(xDebutContenu , yDebutContenu - 6);
    printf("JEUX DE PENDU\n\n");
    // Message de succès
    afficherMessage(xDebutContenu - 10, yDebutContenu, "Nouveau compte créé avec succès !");

    // Utiliser la fonction choisirDestination
    choisirDestination(xDebutContenu - 22, yDebutContenu + 3);

    // Vider le tampon d'entrée après la saisie
    flushstdin();

    setfontcolor(0);
}



/*



*/

void creerNouveauCompte() {
    struct Compte nouveauCompte;

    // Afficher le sous-menu des comptes
    struct termsize taille;
    taille = gettermsize();

    // Coordonnées pour positionner le contenu au centre du cadre
    int xDebutContenu = (taille.cols - 18) / 2;  // Centré horizontalement
    int yDebutContenu = (taille.rows - 45) / 2 + 10;   // Centré verticalement

    gotoxy(xDebutContenu , yDebutContenu - 6);
    printf("JEUX DE PENDU\n\n");

    // Appel de la fonction infoIdentification avec les arguments appropriés
    infoIdentification(&nouveauCompte, xDebutContenu - 22, yDebutContenu + 3);


    // Ajoutez d'autres champs si nécessaire

    

    gotoxy(xDebutContenu -10, yDebutContenu + 22);
    // Attendre avant de revenir au menu des comptes
    printf("Appuyez sur Entrée pour continuer...");
    while (getchar() != '\n');
flushstdin();  // Vider le tampon d'entrée après la saisie

}


/*

*/


void gererComptes() {
    int choix;
    do {
        // Effacer l'écran complet
        clrscr();
        
        // Afficher le sous-menu des comptes
        struct termsize taille;
        taille = gettermsize();

        // Coordonnées pour positionner le contenu au centre du cadre
        int xDebutContenu = (taille.cols - 18) / 2;  // Centré horizontalement
        int yDebutContenu = (taille.rows - 45) / 2 + 10;   // Centré verticalement

        // Redessiner le cadre
        dessine_cadre();
        
        gotoxy(xDebutContenu , yDebutContenu - 6);
        printf("JEUX DE PENDU\n\n");
        setfontcolor(34);

        // Positionner le curseur au début du cadre
        gotoxy(xDebutContenu - 4, yDebutContenu -3);

        printf("    GESTION DES COMPTES UTILISATEURS\n\n");
        setfontbold(1);

        // Options de base
        gotoxy(xDebutContenu - 20, yDebutContenu);
        printf("1. Créer un Nouveau Compte\n");
        
        gotoxy(xDebutContenu + 20, yDebutContenu);
        printf("2. Se Connecter\n");

        gotoxy(xDebutContenu - 20, yDebutContenu + 3);
        printf("3. Se Déconnecter\n");
    
        gotoxy(xDebutContenu + 20, yDebutContenu + 3);
        printf("4. Retour au Menu Principal\n");
        setfontbold(0);
        
        gotoxy(xDebutContenu, yDebutContenu + 11);
        //printf("Entrez votre choix : ");
        choix = getInt("Entrez votre choix : ");

        switch (choix) {
            case 1:
                // Code pour créer un nouveau compte
                creerNouveauCompte();
                break;
            case 2:
                // Code pour se connecter
                  gotoxy(xDebutContenu , yDebutContenu - 19);
                    //printf("se connecter ");
                    //seConnecter();
                break;
                
            case 3:
                // Code pour se déconnecter
                break;
            case 4:
                // Retour au menu principal
                afficherMenu();
                break;
            default:
                // Positionner le curseur avant d'afficher le message
                gotoxy(xDebutContenu, yDebutContenu + 11);
                printf("Choix invalide. Veuillez réessayer.\n");
                break;
        }

    } while (choix != 4);
}


/*

*/

// Fonction pour vérifier si un utilisateur existe dans le fichier de comptes
int verifierUtilisateur(const char* nomUtilisateur) {
    FILE* fichier = fopen("comptes.txt", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier de comptes");
        exit(EXIT_FAILURE);
    }

    struct Compte compte;
    int utilisateurExiste = 0;
    while (fscanf(fichier, "%s %s %s %s", compte.nom, compte.prenom, compte.username, compte.motDePasse) == 4) {
        if (strcmp(compte.username, nomUtilisateur) == 0) {
            utilisateurExiste = 1;
            break;
        }
    }

    fclose(fichier);

    return utilisateurExiste;
}




