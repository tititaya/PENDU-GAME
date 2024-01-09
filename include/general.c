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

/* Récupération de la taille du terminal */
struct termsize gettermsize(void) {
  struct termsize size;
  struct winsize win;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);
  size.cols = win.ws_col;
  size.rows = win.ws_row;
  return size;
}

/* Récupération d'un caractère au clavier
 * sans écho du caractère ni besoin d'appuyer
 * sur Entrée
 */
int getch(void) {
  struct termios oldattr, newattr;
  tcgetattr(STDIN_FILENO, &oldattr);

  newattr = oldattr;
  newattr.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newattr);

  int ch = getc(stdin);

  tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

  return ch;
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


void effacerLigne(int x, int y, int longueur) {
    gotoxy(x, y);
    for (int i = 0; i < longueur; ++i) {
        printf(" ");
    }
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


