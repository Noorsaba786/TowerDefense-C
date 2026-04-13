#ifndef TOWERDEFEND_H_INCLUDED
#define TOWERDEFEND_H_INCLUDED

#include "stdbool.h"
#include "stdio.h"

#define LARGEURJEU 11
#define HAUTEURJEU 19
#define NBCOORDPARCOURS 34
#define X 0
#define Y 1

#define PARTIEBIN "partiebin.tdb"
#define PARTIESEQ "partieseq.tds"

typedef enum
{
    tourSol,
    tourAir,
    tourRoi,
    archer,
    chevalier,
    dragon,
    gargouille
} TuniteDuJeu;
typedef enum
{
    sol,
    solEtAir,
    air
} Tcible;

typedef struct
{
    TuniteDuJeu nom;
    Tcible cibleAttaquable; // indique le type des unit�s que l�on peut attaquer
    Tcible maposition;      // indique soit � air � soit � sol �, utile pour savoir
                            // qui peut nous attaquer
    int pointsDeVie;
    float vitesseAttaque; // en seconde, plus c�est petit plus c�est rapide
    int degats;
    int portee; // en m�tre, distance sur laquelle on peut atteindre une
                // cible

    float vitessedeplacement; // en m/s
    int posX, posY;           // position sur le plateau de jeu
    int peutAttaquer;         // permet de g�rer le fait que chaque unit� attaque une
                              // seule fois par tour ;
                              // 0 = a d�j� attaqu�, 1 = peut attaquer ce tour-ci
                              //  � remettre � 1 au d�but de chaque tour

    struct Tunite *cible; // NULL si pas de cible. mettre � jour ce pointeur permet l'aninamtion (simpliste certe) du tir
    // non utilis� au final -> utiliser directement dessineAttaque

    int score_emplacement; // un clin d'oeil pour suscister une id�e de tri
} Tunite;

typedef struct T_cell
{
    struct T_cell *suiv;
    Tunite *pdata; // pointeur vers une unit�
} *TListePlayer;

typedef Tunite ***TplateauJeu; ////tableau a deux dimensions de largeur 11 et hauteur 19 contenant des pointeurs (Tunite*)

// ==================== Tien: Allocation et initialisation ====================
TplateauJeu AlloueTab2D(int largeur, int hauteur);
void initPlateauAvecNULL(TplateauJeu jeu, int largeur, int hauteur);
void libererPlateau(TplateauJeu jeu, int largeur, int hauteur);
// ==================== Tien: Gestion des joueurs ====================
bool tourRoiDetruite(TListePlayer player);

void PositionnePlayerOnPlateau(TListePlayer player, TplateauJeu jeu);

void supprimerUnite(TListePlayer *player, Tunite *UniteDetruite, TplateauJeu jeu);
void AjouterUnite(TListePlayer *player, Tunite *nouvelleUnite);
void libererListePlayer(TListePlayer *player, int libererUnites);

// ==================== Tien: Phase combat ====================
TListePlayer quiEstAPortee(TplateauJeu jeu, Tunite *UniteAttaquante); // retourne la liste des cibles possibles

void combat(SDL_Surface *surface, Tunite *UniteAttaquante, Tunite *UniteCible);

void reinitialiseAttaque(TListePlayer player);
TListePlayer trierParPV(TListePlayer player);

void phaseCombat(SDL_Surface *surface, TplateauJeu jeu, TListePlayer attaquants, TListePlayer *defense);

// ==================== Tien: Phase d�placement ====================
int indexParcours(int **tabParcours, int x, int y);
bool estSurChemin(int **tabParcours, int x, int y);
void phaseDeplacement(TListePlayer playerHorde, TplateauJeu jeu, int **tabParcours);

// ==================== Tien: Phase cr�ation et Placement intelligent des tours (T�che C) ====================
void phaseCreation(TplateauJeu jeu, int **tabParcours, TListePlayer *playerRoi, TListePlayer *playerHorde, int tours);

void updatePlayerListsDepuisPlateau(TplateauJeu jeu, TListePlayer *playerRoi, TListePlayer *playerHorde);

bool trouverMeilleureCaseTour(TplateauJeu jeu, int **tabParcours, int portee, int *outX, int *outY);

// ====================Tien: Phase Affichage du jeu ====================

void affichePlateauConsole(TplateauJeu jeu, int largeur, int hauteur);

// ==================== Tien: Chemin al�atoire ====================
void ecritCheminVersleHaut(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante);
void ecritCheminVerslaDroite(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante);
void ecritCheminVerslaGauche(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante);
int **initChemin(); // retourne le chemin emprunter par la horde, sous forme d'un tableau[NBCOORDPARCOURS][2]

void afficheCoordonneesParcours(int **t, int nbcoord);
void freeChemin(int **tab);

// ==================== Tien: Cr�ation d'unit�s ====================
Tunite *creeTourSol(int posx, int posy);
Tunite *creeTourAir(int posx, int posy);
Tunite *creeTourRoi(int posx, int posy);

////Tien : ajouter creeChevalier,creeArcher,creeGargouille ++++
Tunite *creeDragon(int posx, int posy);
Tunite *creeArcher(int posx, int posy);
Tunite *creeGargouille(int posx, int posy);
Tunite *creeChevalier(int posx, int posy);

Tunite *creeUniteHordeAleatoire(int posx, int posy);
Tunite *creeTourDefenseAleatoire(int posx, int posy);

// ====================Tien: Fichiers de sauvegarde ====================
int sauvegardeSequentiel(const char *nomFichier, TplateauJeu jeu, int largeur, int hauteur);
int restaurationSequentiel(const char *nomFichier, TplateauJeu jeu, int largeur, int hauteur);
int sauvegardeBinaire(const char *nomFichier, TplateauJeu jeu, int largeur, int hauteur);
int restaurationBinaire(const char *nomFichier, TplateauJeu jeu, int largeur, int hauteur);

#endif // TOWERDEFEND_H_INCLUDED
