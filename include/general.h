
/*
 * general.h
 * PoPS ESR v1.3
 *
 */
#ifndef GENERAL_H
#define GENERAL_H


#define BLACK   30
#define RED     31
#define GREEN   32
#define YELLOW  33
#define BLUE    34
#define MAGENTA 35
#define CYAN    36
#define WHITE   37

/* Couleurs supplémentaires
 * ce sont les mêmes qui sont utilisées
 * pour le mode "bold" de la police de caractères
 */
#define BRIGHTBLACK   90
#define BRIGHTRED     91
#define BRIGHTGREEN   92
#define BRIGHTYELLOW  93
#define BRIGHTBLUE    94
#define BRIGHTMAGENTA 95
#define BRIGHTCYAN    96
#define BRIGHTWHITE   97

#define BLOCK_BLINK     1
#define BLOCK           2
#define UNDERLINE_BLINK 3
#define UNDERLINE       4
#define BAR_BLINK       5
#define BAR             6
#define INVERT          7
#define HIDDEN          8

#define TRUE    1
#define FALSE   0


// Prototypes des fonctions générales

struct termsize {
    int cols;
    int rows;
};


struct termsize gettermsize(void);


void wait(void);

void clrscr(void);

void gotoxy(int x, int y);

void setfontcolor(int color);

void setbgrcolor(int color);

void setfontbold(int status);

void setunderline(int status);

void setblink(int status);

void settitle(char const* title);

void hidecursor(void);
void unhidecursor(void);

void setcurshape(int shape);



/* Récupération d'un caractère au clavier
 * avec écho du caractère sans besoin d'appuyer
 * sur Entrée
 */

int getch(void);

int getche(void);

void clrline(void);

void resetcolors(void);
void resetterminal(void);


/* sous Unix/Linux la sortie standard
 * stdout est bufférisée, i.e., un printf
 * ne s'affiche que si un \n est dans le buffer
 * Cette fonction désactive le buffer
 */


void nostdoutbuff(void);
void setstdoutbuff(void);  /* Réactive le buffer de la sortie standard */


/* Pour vider les caractères restant dans le
 * buffer de saisie de caractères
 */
void flushstdin(void);

//ajouter par moi  

void effacerLigne(int x, int y, int longueur);

void delai(float nbsecondes);



#endif /* GENERAL_H */




