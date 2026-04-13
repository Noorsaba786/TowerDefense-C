#include <stdio.h>
#include <stdlib.h>
#include "stdbool.h"

#include "SDL.h"
#include "maSDL.h" //Tien: ajouter

#include "towerdefend.h"

// typedef Tunite* ** TplateauJeu;

// ==================== TienSaba: Allocation et initialisation ====================
TplateauJeu AlloueTab2D(int largeur, int hauteur)
{
    TplateauJeu jeu;
    jeu = (Tunite ***)malloc(sizeof(Tunite **) * largeur);
    for (int i = 0; i < largeur; i++)
    {
        jeu[i] = (Tunite **)malloc(sizeof(Tunite *) * hauteur);
    }
    return jeu; // tab2D contenant des pointeurs
}
void initPlateauAvecNULL(TplateauJeu jeu, int largeur, int hauteur)
{
    for (int i = 0; i < largeur; i++)
    {
        for (int j = 0; j < hauteur; j++)
        {
            jeu[i][j] = NULL;
        }
    }
    /*
        //POUR LA DEMO D'AFFICHAGE UNIQUEMENT, A SUPPRIMER
        //(les tours ici ne sont pas liées aux listes des unités de vos joueurs)
        jeu[5][3]=creeTourSol(5,3);
        jeu[3][3]=creeTourAir(3,3);
        jeu[4][1]=creeTourRoi(4,1);
        jeu[4][15]=creeTourAir(4,15);
        jeu[5][17]=creeDragon(5,17);
        //FIN DEMO AFFICHAGE

    */
}

// TienSaba:
void libererPlateau(TplateauJeu jeu, int largeur, int hauteur)
{
    for (int i = 0; i < largeur; i++)
    {
        for (int j = 0; j < hauteur; j++)
        {
            if (jeu[i][j] != NULL)
            {
                free(jeu[i][j]);
                jeu[i][j] = NULL;
            }
        }
        free(jeu[i]); // libère chaque colonne allouée par AlloueTab2D
    }
    free(jeu); // libère le tableau principal
}

// ==================== TienSaba: Gestion des joueurs ====================

// TienSaba: Vérifie si l'unité "tourRoi" est détruite
bool tourRoiDetruite(TListePlayer player)
{
    TListePlayer tmp = player; // TienSaba: pointeur pour parcourir la liste

    // TienSaba: Parcours de toute la liste des unités
    while (tmp != NULL)
    {
        // Vérifie que pdata n'est pas NULL
        if (tmp->pdata != NULL)
        {
            // TienSaba: Si on trouve une unité de type "tourRoi" encore en vie
            if (tmp->pdata->nom == tourRoi && tmp->pdata->pointsDeVie > 0)
            {
                return false; // TienSaba: Le tourRoi est toujours en vie
            }
        }
        // TienSaba: Passer à l'élément suivant
        tmp = tmp->suiv;
    }
    // TienSaba: Si aucune tourRoi vivante n'est trouvée
    return true; // TienSaba: Le tourRoi est mort ou n'existe pas.
}

// TienSaba: Place toutes les unités des joueurs sur le plateau de jeu
void PositionnePlayerOnPlateau(TListePlayer player, TplateauJeu jeu)
{
    TListePlayer tmp = player; // TienSaba: pointeur pour parcourir la liste

    // TienSaba: Parcours de la liste des unités
    while (tmp != NULL)
    {
        Tunite *unite = tmp->pdata;
        // Vérifie que l'unité existe
        if (unite != NULL &&
            unite->posX >= 0 && unite->posX < LARGEURJEU &&
            unite->posY >= 0 && unite->posY < HAUTEURJEU)
        {
            // TienSaba: Placement de l'unité sur le plateau
            jeu[unite->posX][unite->posY] = unite;
        }
        // TienSaba: Passer à l'unité suivante
        tmp = tmp->suiv;
    }
}

// TienSaba: Supprime une unité (UniteDetruite) de la liste des joueurs et met à jour le plateau de jeu
/*
void supprimerUnite(TListePlayer *player, Tunite *UniteDetruite, TplateauJeu jeu){


    TListePlayer tmp = *player; //TienSaba: pointeur pour parcourir la liste
    TListePlayer avant =NULL; //TienSaba: pointeur vers l'élément précédent

    while(tmp != NULL){
        //TienSaba: Si on trouve l'unité à supprimer
        if (tmp->pdata == UniteDetruite){
            //TienSaba: l'élément à supprimer est en tête de liste
            if (avant == NULL)
                *player = tmp->suiv; //TienSaba: on déplace la tête vers le suivant

             //TienSaba: l'élément est au milieu ou à la fin
            else
                 avant->suiv = tmp->suiv; //TienSaba: on saute l'élément courant

            //TienSaba: Libération de la mémoire
            free(tmp->pdata); //TienSaba: libère l'unité
            free(tmp);  //TienSaba: libère le maillon de la liste
            return; //TienSaba: fin de la fonction après suppression
        }
        //TienSaba: continue cherche player est UniteDetruite
        else{
        avant = tmp; //TienSaba: on mémorise l'élément courant
        tmp = tmp->suiv; //TienSaba: on passe au suivant
        }
    }
}
*/
void supprimerUnite(TListePlayer *player, Tunite *UniteDetruite, TplateauJeu jeu)
{
    TListePlayer tmp = *player; // TienSaba: pointeur pour parcourir la liste
    TListePlayer avant = NULL;  // TienSaba: pointeur vers l'élément précédent

    while (tmp != NULL)
    {
        // TienSaba: Si on trouve l'unité à supprimer
        if (tmp->pdata == UniteDetruite)
        {
            // TienSaba: l'élément à supprimer est en tête de liste
            if (avant == NULL)
                *player = tmp->suiv; // TienSaba: on déplace la tête vers le suivant

            // TienSaba: l'élément est au milieu ou à la fin
            else
                avant->suiv = tmp->suiv; // TienSaba: on saute l'élément courant

            // TienSaba: Suppression de l'unité sur le plateau de jeu
            int x = UniteDetruite->posX;
            int y = UniteDetruite->posY;
            jeu[x][y] = NULL;

            // TienSaba: Libération de la mémoire
            free(tmp->pdata); // TienSaba: libère l'unité
            free(tmp);        // TienSaba: libère le maillon de la liste
            return;           // TienSaba: fin de la fonction après suppression
        }
        // TienSaba: continue cherche player est UniteDetruite
        else
        {
            avant = tmp;     // TienSaba: on mémorise l'élément courant
            tmp = tmp->suiv; // TienSaba: on passe au suivant
        }
    }
}
// TienSaba: Ajoute une nouvelle unité au début de la liste chainée
void AjouterUnite(TListePlayer *player, Tunite *nouvelleUnite)
{
    if (nouvelleUnite != NULL)
    {
        // TienSaba: Allocation mémoire pour un nouveau maillon
        TListePlayer nouv = (TListePlayer)malloc(sizeof(struct T_cell));
        if (nouv != NULL)
        {
            // TienSaba: Initialisation du maillon
            nouv->pdata = nouvelleUnite; // TienSaba: stocke l'unité
            nouv->suiv = *player;        // TienSaba: pointe vers l'ancienne tête
            // TienSaba: Mise à jour de la tête de liste
            *player = nouv;
        }
    }
}

// TienSaba:
void libererListePlayer(TListePlayer *player, int libererUnites)
{
    TListePlayer courant = *player;
    while (courant != NULL)
    {
        TListePlayer suivant = courant->suiv;
        if (libererUnites && courant->pdata != NULL)
        {
            free(courant->pdata);
        }
        free(courant);
        courant = suivant;
    }
    *player = NULL;
}

// ==================== TienSaba: Phase combat ====================
// TienSaba: retourne la liste des cibles possibles
static bool estCibleValide(Tunite *UniteAttaquante, Tunite *UniteCible)
{
    if (((UniteAttaquante != NULL && UniteCible != NULL) &&
         UniteAttaquante != UniteCible) &&

        (UniteAttaquante->cibleAttaquable == solEtAir ||
         (UniteAttaquante->cibleAttaquable == sol && UniteCible->maposition == sol) ||
         (UniteAttaquante->cibleAttaquable == air && UniteCible->maposition == air)))
        return true;

    return false;
}

// TienSaba: Vérifie si la cible est dans la portée de l’attaquant
static bool estDansPortee(Tunite *UniteAttaquante, Tunite *uniteCible)
{

    // calcul de la distance en x et y
    int dx = UniteAttaquante->posX - uniteCible->posX;
    int dy = UniteAttaquante->posY - uniteCible->posY;

    // distance au carré
    int distanceCarree = dx * dx + dy * dy;

    // portée au carré
    int porteeCarree = UniteAttaquante->portee * UniteAttaquante->portee;

    // comparaison
    if (distanceCarree <= porteeCarree)
    {
        return true;
    }

    return false;
}

TListePlayer quiEstAPortee(TplateauJeu jeu, Tunite *UniteAttaquante)
{
    TListePlayer cibles = NULL;

    if (UniteAttaquante == NULL)
        return NULL;
    for (int i = 0; i < LARGEURJEU; i++)
    {
        for (int j = 0; j < HAUTEURJEU; j++)
        {
            Tunite *uniteCible = jeu[i][j];
            if (uniteCible != NULL &&
                estCibleValide(UniteAttaquante, uniteCible) &&
                estDansPortee(UniteAttaquante, uniteCible))
            {
                AjouterUnite(&cibles, uniteCible);
            }
        }
    }
    return cibles;
}

// TienSaba:

void combat(SDL_Surface *surface, Tunite *UniteAttaquante, Tunite *UniteCible)
{

    UniteCible->pointsDeVie -= UniteAttaquante->degats;
    UniteAttaquante->peutAttaquer = 0;
    const char *nomsUnites[] = {"tourSol", "tourAir", "tourRoi", "archer", "chevalier", "dragon", "gargouille"};
    printf("[Combat] unite %s attaque unite %s | pdv restants : %d\n",
           nomsUnites[UniteAttaquante->nom], nomsUnites[UniteCible->nom], UniteCible->pointsDeVie);
    dessineAttaque(surface, UniteAttaquante, UniteCible); /* animation SDL */
}

// TienSaba
void reinitialiseAttaque(TListePlayer player)
{
    TListePlayer tmp = player; // TienSaba: pointeur pour parcourir la liste

    while (tmp != NULL)
    {
        tmp->pdata->peutAttaquer = 1;
        tmp = tmp->suiv;
    }
}

TListePlayer trierParPV(TListePlayer player)
{
    TListePlayer triee = NULL;

    while (player != NULL)
    {
        TListePlayer courant = player;
        player = player->suiv;

        if (triee == NULL || courant->pdata->pointsDeVie < triee->pdata->pointsDeVie)
        {
            courant->suiv = triee;
            triee = courant;
        }
        else
        {
            TListePlayer tmp = triee;
            while (tmp->suiv != NULL &&
                   tmp->suiv->pdata->pointsDeVie < courant->pdata->pointsDeVie)
            {
                tmp = tmp->suiv;
            }
            courant->suiv = tmp->suiv;
            tmp->suiv = courant;
        }
    }

    return triee;
}

// TienSaba:
static bool estUniteDuRoi(Tunite *uniteCourante)
{
    //
    if (uniteCourante != NULL &&
        (uniteCourante->nom == tourSol ||
         uniteCourante->nom == tourAir ||
         uniteCourante->nom == tourRoi))
        return true;

    return false;
}

// TienSaba:
static bool estDansListe(TListePlayer player, Tunite *uniteCherchee)
{
    TListePlayer tmp = player;
    while (tmp != NULL)
    {
        if (tmp->pdata == uniteCherchee)
            return true;

        tmp = tmp->suiv;
    }
    return false;
}

// TienSaba:
void phaseCombat(SDL_Surface *surface, TplateauJeu jeu, TListePlayer attaquants, TListePlayer *defense)
{
    TListePlayer joueurActuel = attaquants;
    while (joueurActuel != NULL)
    {
        TListePlayer joueurSuivant = joueurActuel->suiv;
        Tunite *uniteAttaquante = joueurActuel->pdata;
        if (uniteAttaquante != NULL && uniteAttaquante->pointsDeVie > 0)
        {
            TListePlayer ciblesValides = quiEstAPortee(jeu, uniteAttaquante);
            /*            Tunite *uniteCible = NULL;
                        int pointsDeVieMin = 2147483647;

                        TListePlayer cible = ciblesValides;
                        while(cible != NULL) {
                            if (estDansListe(*defense, cible->pdata)) {

                                if (estUniteDuRoi(uniteAttaquante) || cible->pdata->nom == tourRoi){
                                    if (cible->pdata->pointsDeVie < pointsDeVieMin) {
                                        pointsDeVieMin = cible->pdata->pointsDeVie;
                                        uniteCible = cible->pdata;
                                    }
                                }
                            }
                            cible = cible->suiv;
                        }



                        if (uniteCible != NULL) {
                            combat(surface, uniteAttaquante, uniteCible);

                            if (uniteCible->pointsDeVie <= 0) {
                                jeu[uniteCible->posX][uniteCible->posY] = NULL;
                                supprimerUnite(defense, uniteCible, jeu);
                            }
              */

            ciblesValides = trierParPV(ciblesValides);
            TListePlayer tmp = ciblesValides;
            while (tmp != NULL)
            {

                if (estDansListe(*defense, tmp->pdata))
                {
                    Tunite *cible = tmp->pdata;

                    // Règle prof : les unités de la horde n'attaquent QUE la tour du roi
                    // Les tours de défense (tourSol, tourAir) sont inattaquables
                    if (!estUniteDuRoi(uniteAttaquante) && cible->nom != tourRoi)
                    {
                        tmp = tmp->suiv;
                        continue;
                    }

                    combat(surface, uniteAttaquante, cible);

                    if (cible->pointsDeVie <= 0)
                    {
                        jeu[cible->posX][cible->posY] = NULL;
                        supprimerUnite(defense, cible, jeu);
                    }
                    break; // une seule attaque par unite par tour
                }
                tmp = tmp->suiv;
            }

            libererListePlayer(&ciblesValides, 0);
        }
        joueurActuel = joueurSuivant;
    }
}

// ==================== TienSaba: Phase déplacement ====================
int indexParcours(int **tabParcours, int x, int y)
{
    for (int i = 0; i < NBCOORDPARCOURS; i++)
    {
        if (tabParcours[i][X] == x && tabParcours[i][Y] == y)
            return i;
    }
    return -1;
}

bool estSurChemin(int **tabParcours, int x, int y)
{
    if (indexParcours(tabParcours, x, y) >= 0)
    {
        return true;
    }
    return false;
}

void phaseDeplacement(TListePlayer playerHorde, TplateauJeu jeu, int **tabParcours)
{
    // Traiter du BOUT du chemin vers l'entrée : les unités les plus avancées
    // bougent en premier, libérant de la place pour celles derrière elles.
    for (int i = NBCOORDPARCOURS - 2; i >= 0; i--)
    {
        int x = tabParcours[i][X];
        int y = tabParcours[i][Y];
        Tunite *unit = jeu[x][y];

        if (unit == NULL)
            continue;
        if (unit->vitessedeplacement == 0.0f)
            continue; // ignorer les tours (statiques)

        int nbCases = (int)(unit->vitessedeplacement);
        if (nbCases < 1)
            nbCases = 1;

        for (int step = 0; step < nbCases; step++)
        {
            int idx = indexParcours(tabParcours, unit->posX, unit->posY);
            if (idx < 0 || idx >= NBCOORDPARCOURS - 1)
                break;

            int nx = tabParcours[idx + 1][X];
            int ny = tabParcours[idx + 1][Y];

            if (jeu[nx][ny] == NULL)
            {
                jeu[unit->posX][unit->posY] = NULL;
                unit->posX = nx;
                unit->posY = ny;
                jeu[nx][ny] = unit;
            }
            else
            {
                break; // chemin bloqué, on attend
            }
        }
    }
}

// ==================== TienSaba: Phase création et Placement intelligent des tours (Tâche C) ====================
void phaseCreation(TplateauJeu jeu, int **tabParcours, TListePlayer *playerRoi, TListePlayer *playerHorde, int tours)
{
    // --- Création d'une unité de la Horde ---
    if ((rand() % 100) < 38)
    {
        int hx = tabParcours[0][X];
        int hy = tabParcours[0][Y];

        if (jeu[hx][hy] == NULL)
        {
            Tunite *nouvelleHorde = creeUniteHordeAleatoire(hx, hy);
            if (nouvelleHorde != NULL)
            {
                AjouterUnite(playerHorde, nouvelleHorde);
            }
        }
    }

    // --- Création d'une tour du Roi ---
    if (tours % 6 == 0)
    {
        if ((rand() % 100) < 15)
        {
            int tx = 0, ty = 0;
            Tunite *nouvelleTour = creeTourDefenseAleatoire(0, 0);

            if (nouvelleTour != NULL)
            {
                if (trouverMeilleureCaseTour(jeu, tabParcours, nouvelleTour->portee, &tx, &ty))
                {
                    nouvelleTour->posX = tx;
                    nouvelleTour->posY = ty;
                    AjouterUnite(playerRoi, nouvelleTour);
                }
                else
                {
                    free(nouvelleTour);
                }
            }
        }
    }
}
// TienSaba:
void updatePlayerListsDepuisPlateau(TplateauJeu jeu, TListePlayer *playerRoi, TListePlayer *playerHorde)
{
    libererListePlayer(playerRoi, 0);
    libererListePlayer(playerHorde, 0);
    for (int i = 0; i < LARGEURJEU; i++)
    {
        for (int j = 0; j < HAUTEURJEU; j++)
        {
            if (jeu[i][j] != NULL)
            {
                if (estUniteDuRoi(jeu[i][j]))
                    AjouterUnite(playerRoi, jeu[i][j]);
                else
                    AjouterUnite(playerHorde, jeu[i][j]);
            }
        }
    }
}

bool trouverMeilleureCaseTour(TplateauJeu jeu, int **tabParcours, int portee, int *outX, int *outY)
{
    int meilleurScore = -1;
    int bestx = -1, besty = -1;
    int porteeSquared = portee * portee;

    for (int x = 0; x < LARGEURJEU; x++)
    {
        for (int y = 0; y < HAUTEURJEU; y++)
        {
            if (jeu[x][y] == NULL || jeu[x][y]->pointsDeVie <= 0)
            {
                if (!estSurChemin(tabParcours, x, y))
                {
                    int score = 0;
                    for (int i = 0; i < NBCOORDPARCOURS; i++)
                    {
                        int dx = x - tabParcours[i][X];
                        int dy = y - tabParcours[i][Y];
                        int distanceSquared = dx * dx + dy * dy;

                        if (distanceSquared <= porteeSquared)
                        {
                            score += (porteeSquared - distanceSquared + 1);
                        }
                    }

                    if (score > meilleurScore)
                    {
                        meilleurScore = score;
                        bestx = x;
                        besty = y;
                    }
                }
            }
        }
    }

    if (bestx == -1)
        return false;
    *outX = bestx;
    *outY = besty;
    return true;
}

// ====================TienSaba: Phase Affichage du jeu ====================

void affichePlateauConsole(TplateauJeu jeu, int largeur, int hauteur)
{
    // pour un affichage sur la console, en relation avec enum TuniteDuJeu
    const char *InitialeUnite[7] = {"s", "a", "r", "A", "C", "D", "G"};

    printf("\n");
    for (int j = 0; j < hauteur; j++)
    {
        for (int i = 0; i < largeur; i++)
        {
            // A ne pas donner aux etudiants
            if (jeu[i][j] != NULL)
            {
                printf("%s", InitialeUnite[jeu[i][j]->nom]);
            }
            else
                printf(" "); // cad pas d'unit� sur cette case
        }
        printf("\n");
    }
}
// ==================== TienSaba: Chemin aléatoire ====================

/*
void ecritCheminVersleHaut  : permet d'initilaiser le tab chemin de distance cases (avec des coord x y) dans une direction, à partir d'un point x y donné

int **chemin  : tab de coordonnées x y du chemin
int *ichemin  : indice de la case du chemin d'où on part
int *xdepart, int *ydepart : valeur en x y de départ pouri la premiere case
int distance  : distance sur laquelle on va écrire des coordonnées dans le tab chemin
int *distanceMaxRestante : securité pour ne pas sortir de la plage d'indice de chemin
*/
void ecritCheminVersleHaut(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante)
{
    if ((*distanceMaxRestante - distance) >= 0)
    {
        int y;
        for (y = *ydepart; y > *ydepart - distance; y--)
        {
            chemin[*ichemin][X] = *xdepart;
            chemin[*ichemin][Y] = y;
            (*ichemin)++;
        }
        *ydepart = y;
    }
    else
        printf("erreur longueur chemin\n");
}
void ecritCheminVerslaDroite(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante)
{
    if ((*distanceMaxRestante - distance) >= 0)
    {
        int x;
        for (x = *xdepart; x < *xdepart + distance; x++)
        {
            chemin[*ichemin][X] = x;
            chemin[*ichemin][Y] = *ydepart;
            (*ichemin)++;
        }
        *xdepart = x;
    }
    else
        printf("erreur longueur chemin\n");
}
void ecritCheminVerslaGauche(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante)
{
    if ((*distanceMaxRestante - distance) >= 0)
    {
        int x;
        for (x = *xdepart; x > *xdepart - distance; x--)
        {
            chemin[*ichemin][X] = x;
            chemin[*ichemin][Y] = *ydepart;
            (*ichemin)++;
        }
        *xdepart = x;
    }
    else
        printf("erreur longueur chemin\n");
}

// TienSaba:
static bool estCaseValideChemin(int x, int y, int **statusCase)
{
    if (x >= 0 && x < LARGEURJEU && y >= 0 && y < HAUTEURJEU)
    {
        if (!statusCase[x][y])
        {
            return true;
        }
    }
    return false;
}

// TienSaba:
static int calculerNX(int x, int direction)
{
    if (direction == 1)
        return x - 1;
    if (direction == 2)
        return x + 1;
    return x;
}

// TienSaba:

static int calculerNY(int y, int direction)
{
    if (direction == 0)
        return y - 1;
    return y;
}

/*
int **initChemin(){
    int **chemin = (int**)malloc(sizeof(int*)*NBCOORDPARCOURS);

    for (int j=0;j<NBCOORDPARCOURS;j++){
        chemin[j] = (int*)malloc(sizeof(int)*2);  //2 cases :indice 0 pour la coord en X, indice 1 pour la coord en Y
    }

   int ydepart = 18;  //et non 19
    int xdepart = 5;  //5 = milieu de la fenetre de 11 de largeur (0-10)
    int i = 0;  //parcourt les i cases du chemin
    int distanceMaxRestante = NBCOORDPARCOURS;

    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 3, &distanceMaxRestante);
    ecritCheminVerslaDroite(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVerslaGauche(chemin, &i, &xdepart, &ydepart, 5, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVerslaDroite(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 3, &distanceMaxRestante);
    ecritCheminVerslaGauche(chemin, &i, &xdepart, &ydepart, 4, &distanceMaxRestante);
    ecritCheminVersleHaut(chemin, &i, &xdepart, &ydepart, 3, &distanceMaxRestante);

    return chemin;
}
*/

// TienSaba:
int **initChemin()
{
    int **chemin = (int **)malloc(sizeof(int *) * NBCOORDPARCOURS);
    for (int i = 0; i < NBCOORDPARCOURS; i++)
    {
        chemin[i] = (int *)malloc(sizeof(int) * 2);
    }

    int **status = (int **)malloc(sizeof(int *) * LARGEURJEU);
    for (int x = 0; x < LARGEURJEU; x++)
    {
        status[x] = (int *)malloc(sizeof(int) * HAUTEURJEU);
    }

    bool estValide = false;

    for (int essai = 0; essai < 200 && !estValide; essai++)
    {

        // reset status
        for (int x = 0; x < LARGEURJEU; x++)
        {
            for (int y = 0; y < HAUTEURJEU; y++)
            {
                status[x][y] = 0;
            }
        }

        int x = LARGEURJEU / 2;
        int y = HAUTEURJEU - 1;

        int direction = 0;
        int longueurCourse = 1;
        bool echec = false;

        chemin[0][X] = x;
        chemin[0][Y] = y;
        status[x][y] = 1;

        for (int i = 1; i < NBCOORDPARCOURS && !echec; i++)
        {

            int options[3];
            int nbOptions = 0;

            if (longueurCourse < 2)
            {
                options[nbOptions++] = direction;
            }
            else
            {
                options[nbOptions++] = 0;
                options[nbOptions++] = 1;
                options[nbOptions++] = 2;
            }

            int choixValides[3];
            int nbValides = 0;

            for (int c = 0; c < nbOptions; c++)
            {
                int d = options[c];

                int nx = calculerNX(x, d);
                int ny = calculerNY(y, d);

                if (estCaseValideChemin(nx, ny, status))
                {
                    choixValides[nbValides++] = d;
                }
            }

            if (nbValides == 0)
            {
                echec = true;
            }
            else
            {

                int choixFinal = choixValides[rand() % nbValides];

                int nx = calculerNX(x, choixFinal);
                int ny = calculerNY(y, choixFinal);

                if (choixFinal == direction)
                {
                    longueurCourse++;
                }
                else
                {
                    direction = choixFinal;
                    longueurCourse = 1;
                }

                x = nx;
                y = ny;

                chemin[i][X] = x;
                chemin[i][Y] = y;
                status[x][y] = 1;
            }
        }

        estValide = !echec && (chemin[NBCOORDPARCOURS - 1][Y] <= 2);
    }

    if (!estValide)
    {
        for (int i = 0; i < NBCOORDPARCOURS; i++)
        {
            chemin[i][X] = LARGEURJEU / 2;
            chemin[i][Y] = HAUTEURJEU - 1 - i;
        }
    }

    for (int x = 0; x < LARGEURJEU; x++)
    {
        free(status[x]);
    }
    free(status);

    return chemin;
}

void afficheCoordonneesParcours(int **chemin, int nbcoord)
{
    printf("Liste coordonnees: ");
    for (int i = 0; i < nbcoord; i++)
    {
        printf("(%d, %d)", chemin[i][X], chemin[i][Y]);
    }
    printf("\nfin liste coordonn�es\n");
}

void freeChemin(int **tab)
{
    for (int j = 0; j < NBCOORDPARCOURS; j++)
    {
        free(tab[j]); // libere chaque case, qui est un tableau de 2 cases
    }
    free(tab);
}

// ==================== TienSaba: Création d'unités ====================
Tunite *creeTourSol(int posx, int posy)
{
    Tunite *nouv = (Tunite *)malloc(sizeof(Tunite));
    nouv->nom = tourSol;
    nouv->cibleAttaquable = sol;
    nouv->maposition = sol;
    nouv->pointsDeVie = 500;
    nouv->vitesseAttaque = 1.5f;
    nouv->degats = 120;
    nouv->portee = 5;
    nouv->vitessedeplacement = 0.0f;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->cible = NULL;
    nouv->score_emplacement = 0;

    return nouv;
}
Tunite *creeTourAir(int posx, int posy)
{
    Tunite *nouv = (Tunite *)malloc(sizeof(Tunite));
    nouv->nom = tourAir;
    nouv->cibleAttaquable = air;
    nouv->maposition = sol;
    nouv->pointsDeVie = 500;
    nouv->vitesseAttaque = 1.0f;
    nouv->degats = 100;
    nouv->portee = 3;
    nouv->vitessedeplacement = 0.0f;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->cible = NULL;
    nouv->score_emplacement = 0;
    return nouv;
}
Tunite *creeTourRoi(int posx, int posy)
{
    Tunite *nouv = (Tunite *)malloc(sizeof(Tunite));
    nouv->nom = tourRoi;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = sol;
    nouv->pointsDeVie = 800;
    nouv->vitesseAttaque = 1.2f;
    nouv->degats = 180;
    nouv->portee = 4;
    nouv->vitessedeplacement = 0.0f;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->cible = NULL;
    nouv->score_emplacement = 0;
    return nouv;
}
Tunite *creeDragon(int posx, int posy)
{
    Tunite *nouv = (Tunite *)malloc(sizeof(Tunite));
    nouv->nom = dragon;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = air;
    nouv->pointsDeVie = 200;
    nouv->vitesseAttaque = 1.1f;
    nouv->degats = 70; // TienSaba:
    nouv->portee = 2;
    nouv->vitessedeplacement = 2.0f;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->cible = NULL;
    nouv->score_emplacement = 0;
    return nouv;
}

// TienSaba: ajouter creeChevalier,creeArcher,creeGargouille +++++
Tunite *creeChevalier(int posx, int posy)
{
    Tunite *nouv = (Tunite *)malloc(sizeof(Tunite));
    nouv->nom = chevalier;
    nouv->cibleAttaquable = sol;
    nouv->maposition = sol;
    nouv->pointsDeVie = 400;
    nouv->vitesseAttaque = 1.5f;
    nouv->degats = 250;
    nouv->portee = 1;
    nouv->vitessedeplacement = 2.0f;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->cible = NULL;
    nouv->score_emplacement = 0;
    return nouv;
}
Tunite *creeArcher(int posx, int posy)
{
    Tunite *nouv = (Tunite *)malloc(sizeof(Tunite));
    nouv->nom = archer;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = sol;
    nouv->pointsDeVie = 80;
    nouv->vitesseAttaque = 0.7f;
    nouv->degats = 120;
    nouv->portee = 3;
    nouv->vitessedeplacement = 1.0f;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->cible = NULL;
    nouv->score_emplacement = 0;
    return nouv;
}
Tunite *creeGargouille(int posx, int posy)
{
    Tunite *nouv = (Tunite *)malloc(sizeof(Tunite));
    nouv->nom = gargouille;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = air;
    nouv->pointsDeVie = 80;
    nouv->vitesseAttaque = 0.6f;
    nouv->degats = 90;
    nouv->portee = 1;
    nouv->vitessedeplacement = 3.0f;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->cible = NULL;
    nouv->score_emplacement = 0;
    return nouv;
}

// TienSaba:
Tunite *creeUniteHordeAleatoire(int posx, int posy)
{
    int choixAleatoire = rand() % 4;
    if (choixAleatoire == 0)
        return creeArcher(posx, posy);
    if (choixAleatoire == 1)
        return creeChevalier(posx, posy);
    if (choixAleatoire == 2)
        return creeDragon(posx, posy);
    return creeGargouille(posx, posy);
}
// TienSaba:

Tunite *creeTourDefenseAleatoire(int posx, int posy)
{
    int choixAleatoire = rand() % 2;
    if (choixAleatoire == 0)
        return creeTourSol(posx, posy);
    return creeTourAir(posx, posy);
}

// ====================TienSaba: Fichiers de sauvegarde ====================

//=== TienSaba: Fichier binaire ====

int sauvegardeSequentiel(const char *nomFichier, TplateauJeu jeu, int largeur, int hauteur)
{
    FILE *f_out = fopen(nomFichier, "w");
    if (f_out == NULL)
    {
        fprintf(stderr, "\nErreur: Impossible d'ecrire dans le fichier %s\n", nomFichier);
        return (EXIT_FAILURE);
    }

    fprintf(f_out, "%d %d\n", largeur, hauteur);
    for (int j = 0; j < hauteur; j++)
    {
        for (int i = 0; i < largeur; i++)
        {

            Tunite *u = jeu[i][j];
            int val;
            if (u == NULL)
            {
                val = 0; // la cellule est vide
            }
            else
            {
                val = 1; // la cellule a l'unité
            }
            fprintf(f_out, "%d ", val);

            if (u != NULL)
            {
                if (fprintf(
                        f_out,
                        "%d %d %d %d %d %f %d %d %f %d %d\n",
                        u->nom,
                        u->cibleAttaquable,
                        u->maposition,
                        u->pointsDeVie,
                        u->degats,
                        u->vitesseAttaque,
                        u->portee,
                        u->peutAttaquer,
                        u->vitessedeplacement,
                        u->posX,
                        u->posY) <= 0)
                {
                    fclose(f_out);
                    return (EXIT_FAILURE);
                }
            }
        }
        fprintf(f_out, "\n");
    }

    fclose(f_out);
    printf("Le fichier partieseq.tds a ete cree, essayez de le lire avec un notepad++, gedit, etc.\n");
    return (EXIT_SUCCESS);
}

int restaurationSequentiel(const char *nomFichier, TplateauJeu jeu, int largeur, int hauteur)
{
    FILE *f_in = fopen(nomFichier, "r");
    if (f_in == NULL)
    {
        fprintf(stderr, "\nErreur: Impossible de lire le fichier %s\n", nomFichier);
        return (EXIT_FAILURE);
    }

    int largFichier = 0;
    int hautFichier = 0;
    if (fscanf(f_in, "%d %d", &largFichier, &hautFichier) != 2 ||
        largFichier != largeur || hautFichier != hauteur)
    {
        fclose(f_in);
        return (EXIT_FAILURE);
    }

    initPlateauAvecNULL(jeu, largeur, hauteur);
    for (int j = 0; j < hauteur; j++)
    {
        for (int i = 0; i < largeur; i++)
        {
            int present = 0;
            if (fscanf(f_in, "%d", &present) != 1)
            {
                fclose(f_in);
                return (EXIT_FAILURE);
            }
            printf("Case [%d][%d] = %d\n", i, j, present);

            if (present == 0)
            {
                jeu[i][j] = NULL;
            }
            else
            {
                Tunite *u = (Tunite *)malloc(sizeof(Tunite));
                int nom, cibleAtt, maPos;
                if (u == NULL || fscanf(
                                     f_in,
                                     "%d %d %d %d %d %f %d %d %f %d %d",
                                     &nom, &cibleAtt, &maPos, &u->pointsDeVie, &u->degats,
                                     &u->vitesseAttaque, &u->portee, &u->peutAttaquer,
                                     &u->vitessedeplacement, &u->posX, &u->posY) != 11)
                {
                    free(u);
                    fclose(f_in);
                    return (EXIT_FAILURE);
                }
                u->nom = (TuniteDuJeu)nom;
                u->cibleAttaquable = (Tcible)cibleAtt;
                u->maposition = (Tcible)maPos;
                jeu[i][j] = u;

                printf("  Unit[%d][%d]: nom=%d, PV=%d, degats=%d\n",
                       i, j, nom, u->pointsDeVie, u->degats);
            }
        }
    }

    printf("Le fichier partieseq.tds a lu\n");
    fclose(f_in);
    return (EXIT_SUCCESS);
}

int sauvegardeBinaire(const char *nomFichier, TplateauJeu jeu, int largeur, int hauteur)
{
    FILE *f_out = fopen(nomFichier, "wb");
    if (f_out == NULL)
    {
        fprintf(stderr, "\nErreur: Impossible d'ecrire dans le fichier %s\n", nomFichier);
        return (EXIT_FAILURE);
    }

    if (fwrite(&largeur, sizeof(int), 1, f_out) != 1 ||
        fwrite(&hauteur, sizeof(int), 1, f_out) != 1)
    {
        fclose(f_out);
        return (EXIT_FAILURE);
    }

    for (int j = 0; j < hauteur; j++)
    {
        for (int i = 0; i < largeur; i++)
        {
            int present = (jeu[i][j] != NULL);
            if (fwrite(&present, sizeof(int), 1, f_out) != 1)
            {
                fclose(f_out);
                return (EXIT_FAILURE);
            }
            printf("Sauvegarde [%d][%d] = %d\n", i, j, present);

            if (present)
            {
                if (fwrite(jeu[i][j], sizeof(Tunite), 1, f_out) != 1)
                {
                    fclose(f_out);
                    return (EXIT_FAILURE);
                }
            }
        }
    }

    fclose(f_out);
    printf("Le fichier partiebin.tdb a ete cree, essayez de le lire avec un notepad++, gedit, etc.\n");
    return (EXIT_SUCCESS);
}

int restaurationBinaire(const char *nomFichier, TplateauJeu jeu, int largeur, int hauteur)
{
    FILE *f_in = fopen(nomFichier, "rb");
    if (f_in == NULL)
    {
        fprintf(stderr, "\nErreur: Impossible de lire le fichier %s\n", nomFichier);
        return (EXIT_FAILURE);
    }

    int largFichier = 0;
    int hautFichier = 0;
    if (fread(&largFichier, sizeof(int), 1, f_in) != 1 ||
        fread(&hautFichier, sizeof(int), 1, f_in) != 1 ||
        largFichier != largeur || hautFichier != hauteur)
    {

        fprintf(stderr, "Erreur: dimensions du fichier ne correspondent pas\n");
        fclose(f_in);
        return (EXIT_FAILURE);
    }

    initPlateauAvecNULL(jeu, largeur, hauteur);
    for (int j = 0; j < hauteur; j++)
    {
        for (int i = 0; i < largeur; i++)
        {
            int present = 0;
            if (fread(&present, sizeof(int), 1, f_in) != 1)
            {
                fclose(f_in);
                return (EXIT_FAILURE);
            }
            printf("Case [%d][%d] = %d\n", i, j, present);

            if (!present)
            {
                jeu[i][j] = NULL;
            }
            else
            {
                Tunite *u = (Tunite *)malloc(sizeof(Tunite));
                if (u == NULL || fread(u, sizeof(Tunite), 1, f_in) != 1)
                {
                    free(u);
                    fclose(f_in);
                    return (EXIT_FAILURE);
                }
                jeu[i][j] = u;
                printf("  Unit[%d][%d]: nom=%d, PV=%d, degats=%d, pos=(%d,%d), portee=%d\n",
                       i, j,
                       u->nom, u->pointsDeVie, u->degats,
                       u->posX, u->posY, u->portee);
            }
        }
    }

    printf("Le fichier partiebin.tdb a lu\n");
    fclose(f_in);
    return (EXIT_SUCCESS);
}
