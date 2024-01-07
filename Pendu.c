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
#include <stdio.h>
#include <stdbool.h>
 



// Structure pour stocker l'état du jeu
typedef struct {
    char motSecret[50];              // Mot à deviner
    char motAffiche[50];             // Mot affiché avec les lettres trouvées
    int tentativesMax;               // Nombre maximum de tentatives autorisées
    int tentativesRestantes;         // Nombre de tentatives restantes
    char lettresUtilisees[26];       // Lettres déjà utilisées
} JeuPendu;




//void afficherPendu(int tentativesRestantes, int x, int y);
// Déclaration de la structure termsize
struct termsize {
    int cols;
    int rows;
};

// Récupération de la taille du terminal
struct termsize gettermsize(void) {
    struct termsize size;
    struct winsize win;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);
    size.cols = win.ws_col;
    size.rows = win.ws_row;
    return size;
}


// Récupération de la taille du terminal
struct termsize gettermsize(void);


// Structure pour représenter un compte utilisateur
struct Compte {
    char nom[50];
    char prenom[50];
    char username[50];
    char motDePasse[100];
    
};



// Déclaration de la fonction pour obtenir une lettre de l'utilisateur
char getLettre(const char *message);







/*

définition des prototype de fonction

*/void afficherMenu();

// Fonction pour afficher l'état du jeu à des coordonnées spécifiques
void afficherEtatJeu(JeuPendu jeu, int x, int y);
void affichePendu(int Hdebut, int Ldebut);

// Fonction pour afficher le pendu en fonction du nombre d'essais restants
void afficherPendu(int tentativesRestantes, int x, int y, int animationFrame);

// Fonction principale pour saisir les informations d'identification
void infoIdentification(struct Compte *nouveauCompte, int x, int y);

// Fonction pour gérer les comptes utilisateurs
void creerNouveauCompte();
void gererComptes();

// Fonction pour vérifier si un utilisateur existe dans le fichier de comptes
int verifierUtilisateur(const char* nomUtilisateur);

void enregistrerCompte(struct Compte *nouveauCompte);
void dessine_cadre();
void chargement();
void enregistrement();

void introduction();
void afficherMessage(int x, int y, const char *message);
void affiche_titre(int x, int y);

void effacerLigne(int x, int y, int longueur);




// Fonctions liées au jeu de pendu

// Fonction pour demander à l'utilisateur s'il veut rejouer
bool voulezRejouer(int x, int y);

// Fonction pour jouer une partie au niveau B1
void jouerPartieB1(int xDebutCadre, int yDebutCadre);

// Fonction pour jouer une partie au niveau B2
void jouerPartieB2(int xDebutCadre, int yDebutCadre);

// Fonction pour jouer une partie au niveau C1
void jouerPartieC1(int xDebutCadre, int yDebutCadre);

void nouvellePartie(int x, int y);
int choisirDestination(int x, int y);

void jouerPartie();


// Déclaration de la fonction pour obtenir une lettre de l'utilisateur
char getLettre(const char *message);



// Définition de la fonction afficherMessage
void afficherMessage(int x, int y, const char *message);


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

void genererSel(unsigned char *sel, size_t taille);
void saisirNomOuPrenom(char *champ, const char *message, int x, int y);


// Fonction pour saisir un nom d'utilisateur et vérifier les caractères spéciaux
void saisirNomUtilisateur(char *username, int x, int y);



// Fonction pour saisir un mot de passe en masquant l'entrée
void saisirMotDePasse(char *motDePasse, int x, int y) ;









/*

definition des fonctions 


***/






/*


*/



// Fonction pour lire une ligne depuis l'entrée standard

void readLine(char *buffer, int bufferSize) {
    fgets(buffer, bufferSize, stdin);
    // Supprimer le saut de ligne final, s'il est présent
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

/****



***/


// Fonction pour obtenir une chaîne sécurisée
void getString(char *str, int maxSize, const char *prompt) {
    printf("%s", prompt);
    readLine(str, maxSize);
}



/***



**/


// Fonction pour obtenir un entier sécurisé
int getInt(const char *prompt) {
    int value;
    char buffer[256]; // Taille maximale du tampon

    do {
        getString(buffer, sizeof(buffer), prompt);
    } while (sscanf(buffer, "%d", &value) != 1);

    return value;
}






/*
delai: fonction permettant de faire un temps d'attente en fonction de son paramètre
@param: paramètre qui désigne le temps d'attente

*/
void delai(float nbsecondes) { 
  /* Calcul: chaque système définit le nombre de périodes
   * d'horloge par seconde
   * La fonction clock() renvoie le temps en nombre de périodes
   * d'horloge (ou clock ticks)
   */
 float pause = nbsecondes*(float)CLOCKS_PER_SEC;

  /* Mémorisation de l'heure de départ */
  clock_t start_time = clock(); 
     float temps= (float)start_time;
  /* On boucle tant que la temporisation n'est pas atteinte */
  while ((float)clock()< temps + pause); 
}





/* Attente d'un appui sur Entrée */
void wait(void) {
    while (fgetc(stdin) != '\n');
}


/* Efface l'affichage */
void clrscr(void) {
  printf("\033[2J\033[?6h");
}


/* Déplacement en x,y du curseur */
void gotoxy(int x, int y) {
  printf("\033[%d;%dH", y, x);
}


void setfontcolor(int color) {
  printf("\033[%dm", color);
}

void setbgrcolor(int color) {
  printf("\033[%dm", color+10);
}

/* Police en mode bold (gras ou en évidence) */
void setfontbold(int status) {
  printf("\033[%dm", status);
}

void setunderline(int status) {
  if (status) status = 4;
  printf("\033[%dm", status);
}

void setblink(int status) {
  if (status) status = 5;
  printf("\033[%dm", status);
}

/* Définit le titre du terminal */
void settitle(char const* title) {
  printf("\033]0;%s\x7", title);
}

/* Désactive l'affichage du curseur */
void hidecursor(void) {
  printf("\033[?25l");
}

/* Réactive l'affichage du curseur */
void unhidecursor(void) {
  printf("\033[?25h");
}

void setcurshape(int shape) {
  // vt520/xterm-style; linux terminal uses ESC[?1;2;3c, not implemented
  printf("\033[%d q", shape);
}




/* Récupération d'un caractère au clavier
 * avec écho du caractère sans besoin d'appuyer
 * sur Entrée
 */
int getche(void) {
  struct termios oldattr, newattr;
  tcgetattr(STDIN_FILENO, &oldattr);

  newattr = oldattr;
  newattr.c_lflag &= ~ICANON;
  tcsetattr(STDIN_FILENO, TCSANOW, &newattr);

  int ch = getc(stdin);

  tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

  return ch;
}

void clrline(void) {
  printf("\033[2K\033E");
}

void resetcolors(void) {
  printf("\033[0m");
}

void resetterminal(void) {
  printf("\033c");
}

/* sous Unix/Linux la sortie standard
 * stdout est bufférisée, i.e., un printf
 * ne s'affiche que si un \n est dans le buffer
 * Cette fonction désactive le buffer
 */
void nostdoutbuff(void) {
  setvbuf(stdout, NULL, _IONBF, 0);
}

/* Réactive le buffer de la sortie standard */
void setstdoutbuff(void) {
  setvbuf(stdout, NULL, _IOLBF, 0);
}

/* Pour vider les caractères restant dans le
 * buffer de saisie de caractères
 */
void flushstdin(void) {
  char c;
  while((c = getchar()) != '\n' && c != EOF);
}







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
    clrscr();
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



void effacerLigne(int x, int y, int longueur) {
    gotoxy(x, y);
    for (int i = 0; i < longueur; ++i) {
        printf(" ");
    }
}



/*




*/




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

/*

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







// Fonction pour créer un nouveau compte

// Fonction pour saisir une chaîne et vérifier si elle ne contient que des lettres
void saisirNomOuPrenom(char *champ, const char *message, int x, int y) {
    do {
        gotoxy(x, y);
        printf("%s", message);
        getString(champ, sizeof(champ), "");
    } while (!estAlpha(champ));
}

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


// Définition de la fonction afficherMessage
void afficherMessage(int x, int y, const char *message) {
    gotoxy(x, y);
    printf("%s", message);
}


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




// Fonction pour lancer le niveau B1
void jouerPartie_B1() {
    // Ajoutez le code spécifique pour le niveau B1 ici
    printf("Lancement du niveau B1...\n");
}


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
    // Vous pouvez ajouter d'autres éléments à afficher après l'effacement du menu si nécessaire
}




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
* introduction: fonction permettant de faire l'affichage lors du début du jeu
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



char getLettre(const char *message) {
    char lettre;

    // Demander à l'utilisateur de deviner une lettre
    printf("%s", message);
    scanf(" %c", &lettre);

    // Ignorer les caractères supplémentaires dans le buffer
    while (getchar() != '\n');

    return lettre;
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




// Fonction pour vérifier si une lettre a déjà été utilisée
int lettreDejaUtilisee(char lettre, const char lettresUtilisees[]) {
    for (int i = 0; i < 26; i++) {
        if (lettresUtilisees[i] == lettre) {
            return 1;  // Lettre déjà utilisée
        }
    }
    return 0;  // Lettre non utilisée
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










int main(void) {
  
   introduction();
 
   setstdoutbuff();
     
    return 0;
}









