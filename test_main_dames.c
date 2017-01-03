#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

/* --Le--nommage--de--certaines--valeurs----------------------------------------------------------------------------- */

#define NMAX 10

#define PIECES ( N * ( N - 2 ) / 4 )

#define PRISE 12
#define PROF 12

#define OUI 1
#define NON 0

#define NeutreMAX -10000
#define NeutreMIN 10000

#define BonusDame 3

#define DameBL 2
#define PionBL 1
#define DameNO -2
#define PionNO -1

#define BLANC 1
#define RIEN 0
#define NOIR -1

#define AVANT 1
#define ARRIERE -1

#define GAUCHE -1
#define DROITE 1

#define AUCUN_MOUV 0
#define MOUV_SANS_PRISE 1
#define MOUV_AVEC_PRISE 2



/* --Une--structure-------------------------------------------------------------------------------------------------- */

typedef struct
{int tdepl ;
    int li ;
    int co ;
    int piece ;
    int liar ;
    int coar ;
    int piecear ;
    int lipr ;
    int copr ;
    int piecepr ;
}
        tmm ;

/* --Les--variables--globales---------------------------------------------------------------------------------------- */

int N ;

int T[ NMAX ][ NMAX ] ;

int Verbeux_minimax = NON ;
int Verbeux_mouvements = NON ;
int Verbeux_mouvements_indent = NON ;

tmm Memo[ PROF ][ PRISE ] ;
tmm Memo_local[ PROF ][ PRISE ] ;

int Memo_num ;

/* --Les--prototypes--des--fonctions--------------------------------------------------------------------------------- */

int main ( int argc , char * argv[ ] ) ;

void main_test_prise ( void ) ;
void main_bouge_sans_prise ( void ) ;
void main_bouge_sans_prise_prof_deux ( void ) ;
void main_bouge_avec_prise ( void ) ;
void main_grand_test ( int prof ) ;
void main_petit_test ( int prof ) ;

void remplis_echiquier ( void ) ;
inline void remplis_case ( int li , int co , int piece ) ;
int compte_pieces ( int coul ) ;

inline int est_dame ( int li , int co , int coul ) ;
inline int est_pion ( int li , int co , int coul ) ;
inline int est_piece ( int li , int co , int coul ) ;

void memo_local ( int tdepl , int prof , int index , int li , int co , int piece ,
                  int liar , int coar , int piecear , int lipr , int copr , int piecepr ) ;
void memo ( tmm m[ PRISE ] , int tdepl , int index , int li , int co , int piece ,
            int liar , int coar , int piecear , int lipr , int copr , int piecepr ) ;
void enregistre_alea ( int reset , int prof , tmm to_memo[ PROF ][ PRISE ] , tmm from_memo[ PROF ][ PRISE ]  ) ;
void annule_mouvements ( tmm m[ PROF ][ PRISE ] ) ;

void joue_sequence ( int profdeb , int proffin ) ;
void joue_mouvements ( tmm m[ PRISE ] ) ;
void joue_mouv ( tmm m ) ;

void print_echiquier ( void ) ;
void print_echiquier_ligne ( int li ) ;
void print_echiquier_colonnes ( int li ) ;
void print_line ( char separator_char , char fill_char ) ;

void affiche ( int prise_ou_non , int li , int co , int piece , int liar , int coar , int piecear ,
               int lipr , int copr , int piecepr , int prof ) ;
void print_les_mouvements ( int prof ) ;
void print_mouvement ( int prof , int indent ) ;
void print_mouv ( tmm m[ PRISE ] , int indent ) ;

int prise_possible_avant ( int coul ) ;
int prise_possible_toutes ( int li , int co , int coul ) ;
int prise_possible_case ( int li , int co , int coul , int sens , int direct ) ;

void itere_jeu ( int coul_ordinateur , int prof ) ;
int suite_ou_pas ( void ) ;
int analyse_mouvement ( int dep , int arr , int prise_ou_pas , int couleur , int * num_mouv ,
                        tmm depl[ PRISE ] , int * suite , int * lisuite , int * cosuite ) ;
int est_coince ( int couleur ) ;

int minimax_call ( int prof , int coul ) ;
int minimax ( int numBL , int numNO , int prof , int coul , tmm m[ PROF ][ PRISE ] ) ;
int relance_minimax ( int numBL , int numNO , int coul , int prof , int minmax , tmm m[ PROF ][ PRISE ] ) ;

int cherche_depl ( int numBL , int numNO , int coul , int prof , int minmax , tmm m[ PROF ][ PRISE ] ) ;

int cherche_depl_avec_prise ( int li , int co , int piece , int coul , int numBL , int numNO , int prof ,
                              int minmax , int num_mouv , int seulement_avant , tmm m[ PROF ][ PRISE ] ) ;
int effectue_depl_avec_prise ( int li , int co , int piece , int coul , int lipr , int copr , int liar , int coar ,
                               int numBL , int numNO , int prof , int minmax , int num_mouv , tmm m[ PROF ][ PRISE ] ) ;

int cherche_depl_pion_sans_prise ( int li , int co , int coul , int numBL ,
                                   int numNO , int prof , int minmax , tmm m[ PROF ][ PRISE ] ) ;
int cherche_depl_dame_sans_prise ( int li , int co , int coul , int numBL ,
                                   int numNO , int prof , int minmax , tmm m[ PROF ][ PRISE ] ) ;
int effectue_depl_sans_prise ( int lidp , int codp , int piece , int coul , int liar , int coar ,
                               int numBL , int numNO , int prof , int minmax , tmm m[ PROF ][ PRISE ] ) ;

/* --La--fonction--principale---------------------------------------------------------------------------------------- */


int main ( int argc , char * argv[ ] )
{
    main_test_prise( ) ;
    return 0;
}

void main_test_prise ( void )
{assert( N == 10 ) ;
    remplis_echiquier( ) ;
    (void)printf( "\n\nTests de prises possibles\n\n" ) ;
    (void)printf( "Prises possibles pour les blancs (grille initiale) : %d\n" , prise_possible_avant( BLANC ) ) ;
    (void)printf( "Prises possibles pour les noirs (grille initiale) : %d\n" , prise_possible_avant( NOIR ) ) ;
    remplis_case( 5 , 5 , PionBL ) ;
    remplis_case( 7 , 7 , RIEN ) ;
    remplis_case( 5 , 9 , DameBL ) ;
    remplis_case( 6 , 8 , RIEN ) ;
    remplis_case( 9 , 5 , RIEN ) ;
    remplis_case( 6 , 2 , DameNO ) ;
    remplis_case( 2 , 6 , RIEN ) ;
    remplis_case( 7 , 3 , PionBL ) ;
    remplis_case( 8 , 4 , RIEN ) ;
    remplis_case( 3 , 7 , PionNO ) ;
    print_echiquier( ) ;
    (void)printf( "Prise depuis %d %d vers la droite et l'avant : %d\n" ,
                  5 , 5 , prise_possible_case( 5 , 5 , BLANC , AVANT , DROITE ) ) ;
    (void)printf( "Prise depuis %d %d vers la gauche et l'avant : %d\n" ,
                  5 , 5 , prise_possible_case( 5 , 5 , BLANC , AVANT , GAUCHE ) ) ;
    (void)printf( "Prise depuis %d %d vers la gauche et l'avant : %d\n" ,
                  6 , 4 , prise_possible_case( 6 , 4 , NOIR , AVANT , GAUCHE ) ) ;
    (void)printf( "Prise depuis %d %d vers la droite et l'avant : %d\n" ,
                  6 , 6 , prise_possible_case( 6 , 6 , NOIR , AVANT , DROITE ) ) ;
    (void)printf( "Prise depuis %d %d vers la gauche et l'avant : %d\n" ,
                  6 , 6 , prise_possible_case( 6 , 6 , NOIR , AVANT , GAUCHE ) ) ;
    (void)printf( "Prise depuis %d %d vers la gauche et l'avant : %d\n" ,
                  5 , 9 , prise_possible_case( 5 , 9 , BLANC , AVANT , GAUCHE ) ) ;
    (void)printf( "Prise depuis %d %d vers la droite et l'avant : %d\n" ,
                  5 , 9 , prise_possible_case( 5 , 9 , BLANC , AVANT , DROITE ) ) ;
    (void)printf( "Prise depuis %d %d vers la gauche et l'avant : %d\n" ,
                  6 , 2 , prise_possible_case( 6 , 2 , NOIR , AVANT , GAUCHE ) ) ;
    (void)printf( "Prise depuis %d %d vers la droite et l'avant : %d\n" ,
                  6 , 2 , prise_possible_case( 6 , 2 , NOIR , AVANT , DROITE ) ) ;
    (void)printf( "Prise depuis %d %d vers la gauche et l'arrière : %d\n" ,
                  6 , 2 , prise_possible_case( 6 , 2 , NOIR , ARRIERE , GAUCHE ) ) ;
    (void)printf( "Prise depuis %d %d vers la droite et l'arrière : %d\n" ,
                  6 , 2 , prise_possible_case( 6 , 2 , NOIR , ARRIERE , DROITE ) ) ;
    (void)printf( "Prise depuis %d %d vers la gauche et l'arrière : %d\n" ,
                  5 , 9 , prise_possible_case( 5 , 9 , BLANC , ARRIERE , GAUCHE ) ) ;
    (void)printf( "Prises possibles pour les blancs : %d\n" , prise_possible_avant( BLANC ) ) ;
    (void)printf( "Prises possibles pour les noirs : %d\n" , prise_possible_avant( NOIR ) ) ;
    (void)printf( "\n\n" ) ;
}


/* --Les--fonctions--de--remplissage--------------------------------------------------------------------------------- */

void remplis_echiquier ( void )
{
    int i, j;

    for(i = 0; i < ((N/2)-1); i++){					//Mise en place des pions blancs
        for(j = 0; j < N; j++){
            if((i+j)%2 == 0)
                remplis_case(i, j, PionBL);
            else
                remplis_case(i, j, RIEN);
        }
    }

    for(i = ((N/2)+1); i < N; i++){					//Mise en place des pions noirs
        for(j = 0; j < N; j++){
            if((i+j)%2 == 0)
                remplis_case(i, j, PionNO);
            else
                remplis_case(i, j, RIEN);
        }
    }
}

/* La fonction qui modifie une case de l'échiquier */

void remplis_case ( int li , int co , int piece )
{T[ li ][ co ] = piece ;
}

/* Ici, on compte le nombre de pièces de la couleur donnée. Chaque dame est créditée d'un bonus BonusDame. */

int compte_pieces ( int coul )
{
    int li, co;
    int cpt = 0;

    //parcours du damier
    for(li = 0; li < N; ++li ) {
        for(co = 0; co < N; ++co) {
            if(est_piece(li, co, coul)) {
                if(est_dame(li, co, coul))
                    cpt += BonusDame;
                else // pour un pion
                    cpt++;
            }
        }
    }

    return cpt;
}

/* --Des--fonctions--auxiliaires------------------------------------------------------------------------------------- */

/* Ces prédicats disent si, oui ou non, on a affaire à une dame, un pion ou une pièce de la couleur indiquée. */
int est_dame ( int li , int co , int coul )
{return( T[ li ][ co ] * coul == 2 ) ;
}

int est_pion ( int li , int co , int coul )
{return( T[ li ][ co ] * coul == 1 ) ;
}

int est_piece ( int li , int co , int coul )
{return( T[ li ][ co ] * coul > 0 ) ;
}




/* --Les--fonctions--d--impression--de--l--échiquier----------------------------------------------------------------- */

/* L'impression de l'échiquier est faite à l'aide de print_echiquier */
void print_echiquier ( void )
{
    int i;
    print_line('+', '-');


    for(i = (N-1); i >= 0; i--){				//boucle d'affichage des lignes, de manière décroissante
        print_echiquier_ligne(i);

    }
    printf("   ");

    for(i = 0; i < N; i++) printf("   %d  ", i);		//affichage du numéro des colonnes

    printf("\n\n");
}


void print_echiquier_ligne ( int li )
{
    print_line('|', ' ');					//affichage de la ligne avec mise en forme...
    print_echiquier_colonnes(li);
    print_line('|', ' ');
    print_line('+', '-');
}


void print_echiquier_colonnes ( int li )
{
    int j;
    printf(" %d ", li);							//numéro de la ligne

    for(j = 0; j < N; j++){						//affichage de b, n, B, N ou rien selon la valeur
        printf("| ");

        if(T[li][j] == PionBL) printf(" b ");
        else if(T[li][j] == PionNO) printf(" n ");
        else if(T[li][j] == DameBL) printf(" B ");
        else if(T[li][j] == DameNO) printf(" N ");
        else printf("   ");

        printf(" ");
    }
    printf("|\n");								//bordure de fin
}


void print_line ( char separator_char , char fill_char )
{
    int i, j;
    printf("   |");							//première bordure "|"

    for(i = 0; i < (N-1); i++){
        for(j = 0; j < 5; j++)
            printf("%c", fill_char);		//toutes les cases sauf la dernière
        printf("%c", separator_char);
    }

    for(j = 0; j < 5; j++)
        printf("%c", fill_char);			//dernière case et bordure de fin

    printf("|\n");
}

/* --Les--fonctions--pour--tester--les--prises----------------------------------------------------------------------- */

/* On teste si une prise AVANT est possible. Pour les pions, on est limité aux options AVANT-GAUCHE et AVANT-DROITE.
   Une dame peut se diriger sur les 4 axes AVANT-GAUCHE, AVANT-DROITE, ARRIERE-GAUCHE ou ARRIERE-DROITE. Ceci est
   analysée dans prise_possible_toutes. */

int prise_possible_avant ( int coul )
{
    int prise_possible = NON; // initiation à NON
    int li = 0, co = 0; // initiation des variables pour le parcour du tableau

    // pour chaque pions et dames de la coul et tant que aucune prise possible
    while((! prise_possible) && li < N ) {
        if( est_pion(li, co, coul)  ) { // si pion de couleur coul et possibilité d'avancer

            // avant gauche
            if( prise_possible_case(li, co, coul, AVANT, GAUCHE) == 1)
                prise_possible = OUI;

                // avant droite
            else if( prise_possible_case(li, co, coul, AVANT, DROITE) == 1)
                prise_possible = OUI;
        }

        else if( est_dame(li, co, coul) )// si dame de couleur coul
            if( prise_possible_toutes(li, co, coul) )
                prise_possible= OUI;

        if(co == N-1) {    // on passe à la ligne suivante si on a parcouru toutes les colonnes
            ++li;
            co = 0;
        } else
            ++co;
    }

    // return NON ou Distance si OUI
    return prise_possible;
}

/* On teste toutes les possibités de prise à partir du point de départ. */

int prise_possible_toutes ( int li , int co , int coul )
{
    int prise_possible; // initialisation à NON

    // avant gauche
    prise_possible = prise_possible_case(li, co, coul, AVANT, GAUCHE);

    // avant droite
    if( prise_possible == NON )
        prise_possible = prise_possible_case(li, co, coul, AVANT, DROITE);

    // arrière gauche
    if( prise_possible == NON )
        prise_possible = prise_possible_case(li, co, coul, ARRIERE, GAUCHE);

    // arrière droite
    if( prise_possible == NON )
        prise_possible = prise_possible_case(li, co, coul, ARRIERE, DROITE);

    return prise_possible;
}


int prise_possible_case ( int li , int co , int coul , int sens , int direct )
{
    int dist = 1;
    int piece_trouvee = NON;        // initialisation à NON
    int prise_possible = NON;       // initialisation à NON
    int ligne_obs = li+dist*coul*sens;
    int colonne_obs = co+dist*coul*direct;

    while( (! piece_trouvee) &&
           ligne_obs < N && ligne_obs >= 0 &&
           colonne_obs < N && colonne_obs >= 0 )  // tant que aucune pièce trouvée et reste dans le tableau
    {
        if( cases_vides(li, co, dist, coul, sens, direct) == NON) {          // si on detecte une pièce
            piece_trouvee = OUI;
            if (est_piece(ligne_obs, colonne_obs, coul * (-1))) {              // si piece ennemie
                if (cases_vides(ligne_obs, colonne_obs, 1, coul, sens, direct)) { // si la case derrière la pièce est vide
                    prise_possible = dist;
                }
            }
        }

        ++dist;
        ligne_obs = li+dist*coul*sens;
        colonne_obs = co+dist*coul*direct;
    }

    return prise_possible;
}

/* --Le--test--de--cases--vides-------------------------------------------------------------------------------------- */

int cases_vides ( int li , int co , int num , int coul , int sens , int direct )
{
    int nb_cases_vides = 0;
    int case_vide = OUI;
    int cpt_cases = 1;

    if( (li + num*coul*sens >= N || li + num*coul*sens < 0) ||
        ( co + num*coul*direct >= N || co + num*coul*direct < 0 )) // test debordement
        case_vide = NON;

    else
        while( case_vide && nb_cases_vides < num) {
            if( T[li + cpt_cases*coul*sens][co + cpt_cases*coul*direct] == RIEN )
                ++nb_cases_vides;
            else
                case_vide = NON;

            ++cpt_cases;
        }
    return case_vide;
}





/* --La--recherche--des--deplacements--avec--prise------------------------------------------------------------------- */


int cherche_depl ( int numBL , int numNO , int coul , int prof , int minmax , tmm m[ PROF ][ PRISE ] )
    { 
        int li, col, piece;
        int num_mouv = 0;
        int seulement_avant; // booleen
        if( prise_possible_avant(coul) ) {
            // cherche les prises possibles pour chaque piece de la couleur coul sur l'echiquier
            for(li = 0; li < N; ++li)
                for(col = 0; col < N; ++col)
                    if(est_piece(li, col, coul)) {
                        piece = T[li][col]; 
                            if(est_pion(li, col, coul))
                                seulement_avant = OUI;
                            else
                                seulement_avant = NON;

                        minmax = cherche_depl_avec_prise(li ,col ,piece ,coul , numBL, numNO, prof, minmax ,num_mouv ,seulement_avant ,m);
                    }
        }
        else { // on se deplace sans faire de prise
            for(li = 0; li < N; ++li)
                for(col = 0; col < N; ++col)
                    if(est_piece(li, col, coul)) {
                            if(est_pion(li, col, coul))
                                minmax = cherche_depl_pion_sans_prise(li, co, coul ,numBL , numNO, prof, minmax , m);
                            else    // c'est une dame
                                minmax = cherche_depl_dame_sans_prise(li, co, coul ,numBL , numNO, prof, minmax , m);
                    }
        }

        return minmax;

    }


int cherche_depl_avec_prise ( int li , int co , int piece , int coul , int numBL , int numNO , int prof ,
                              int minmax , int num_mouv , int seulement_avant , tmm m[ PROF ][ PRISE ] )
    { 
        if(seulement_avant && piece*coul == 1)                              // c'est le premier mouv d'un pion donc on ne prends les prises que vers l'avant.
        {
            if(prise_possible_case(li, co, coul, AVANT, GAUCHE) == 1)
                minmax = effectue_depl_avec_prise(li, co, piece, coul, li+AVANT*coul, co+GAUCHE*coul , li+AVANT*coul*2, 
                                        co+GAUCHE*coul*2 , numBL, numNO, prof, minmax, num_mouv, m);
            if(prise_possible_case(li, co, coul, AVANT, DROITE) == 1)
                minmax = effectue_depl_avec_prise(li, co, piece, coul, li+AVANT*coul, co+DROITE*coul , li+AVANT*coul*2, 
                                        co+DROITE*coul*2 , numBL, numNO, prof, minmax, num_mouv, m);
        }
        else {
            int dist_prise;

            dist_prise = prise_possible_case(li, co, coul, AVANT, GAUCHE);
            if(dist_prise > 0 && ( (piece*coul==2) || (dist_prise == 1) ))   // Si c'est une dame ou si c'est un pion qui à une prise à distance de 1
                minmax = effectue_depl_avec_prise(li, co, piece, coul, li+AVANT*coul*dist_prise, co+GAUCHE*coul*dist_prise , li+AVANT*coul*(dist_prise+1), 
                                        co+GAUCHE*coul*(dist_prise+1) , numBL, numNO, prof, minmax, num_mouv, m);

            dist_prise = prise_possible_case(li, co, coul, AVANT, DROITE);
            if(dist_prise > 0 && ( (piece*coul==2) || (dist_prise == 1) ))
                minmax = effectue_depl_avec_prise(li, co, piece, coul, li+AVANT*coul*dist_prise, co+DROITE*coul*dist_prise , li+AVANT*coul*(dist_prise+1), 
                                        co+DROITE*coul*(dist_prise+1) , numBL, numNO, prof, minmax, num_mouv, m);

            dist_prise = prise_possible_case(li, co, coul, ARRIERE, GAUCHE);
            if(dist_prise > 0 && ( (piece*coul==2) || (dist_prise == 1) ))
                minmax = effectue_depl_avec_prise(li, co, piece, coul, li+ARRIERE*coul*dist_prise, co+GAUCHE*coul*dist_prise , li+ARRIERE*coul*(dist_prise+1), 
                                        co+GAUCHE*coul*(dist_prise+1) , numBL, numNO, prof, minmax, num_mouv, m);

            dist_prise = prise_possible_case(li, co, coul, AVANT, DROITE);
            if(dist_prise > 0 && ( (piece*coul==2) || (dist_prise == 1) ))
                minmax = effectue_depl_avec_prise(li, co, piece, coul, li+ARRIERE*coul*dist_prise, co+DROITE*coul*dist_prise , li+ARRIERE*coul*(dist_prise+1), 
                                        co+DROITE*coul*(dist_prise+1) , numBL, numNO, prof, minmax, num_mouv, m);
        }

        return minmax;
    }


int effectue_depl_avec_prise ( int li , int co , int piece , int coul , int lipr , int copr , int liar , int coar ,
                               int numBL , int numNO , int prof , int minmax , int num_mouv , tmm m[ PROF ][ PRISE ] )
    { . . .
    }

/* --La--recherche--des--deplacements--sans--prise------------------------------------------------------------------- */

/* cherche_depl_pion_sans_prise part d'un pion de couleur coul en ( li , co ) et détermine à l'aide de cases_vides
   si la case AVANT-GAUCHE ou AVANT-DROITE est vide. Si tel est le cas, elle appelle effectue_depl_sans_prise avec
   les paramètres convenables. La gestion de minmax et m sont dorénavant standard. */

int cherche_depl_pion_sans_prise ( int li , int co , int coul , int numBL ,
                                   int numNO , int prof , int minmax , tmm m[ PROF ][ PRISE ] )
    {  
        int piece = T[li][co]; 
        if(cases_vides(li, co, 1, coul, AVANT, DROITE))
            minmax = effectue_depl_sans_prise( li, co, piece, coul, li+AVANT*coul, co+DROITE*coul, numBL, numNO, prof, minmax , m);

        if(cases_vides(li, co, 1, coul, AVANT, GAUCHE))
            minmax = effectue_depl_sans_prise( li, co, piece, coul, li+AVANT*coul, co+GAUCHE*coul, numBL, numNO, prof, minmax , m);

        return minmax;
    }


int cherche_depl_dame_sans_prise ( int li , int co , int coul , int numBL ,
                                   int numNO , int prof , int minmax , tmm m[ PROF ][ PRISE ] )
    { 
        int dist = 0; // initialise dist

        while(case_vide(li ,co , dist+1, coul, AVANT , GAUCHE))
            dist++;
        if(dist_prise > 0)   // Si c'est une dame ou si c'est un pion qui à une prise à distance de 1-
            minmax = effectue_depl_sans_prise(li, co, piece, coul, li+AVANT*coul*dist, co+GAUCHE*coul*dist, numBL, numNO, prof, minmax, num_mouv, m);

        dist = 0; // initialise dist

        while(case_vide(li ,co , dist+1, coul, AVANT , DROITE))
            dist++;
        if(dist > 0)
            minmax = effectue_depl_sans_prise(li, co, piece, coul, li+AVANT*coul*dist, co+DROITE*coul*dist, numBL, numNO, prof, minmax, num_mouv, m);

        dist = 0; // initialise dist

        while(case_vide(li ,co , dist+1, coul, ARRIERE , GAUCHE))
            dist++;
        if(dist_prise > 0)
            minmax = effectue_depl_sans_prise(li, co, piece, coul, li+ARRIERE*coul*dist, co+GAUCHE*coul*dist, numBL, numNO, prof, minmax, num_mouv, m);

        dist = 0; // initialise dist

        while(case_vide(li ,co , dist+1, coul, ARRIERE , DROITE))
            dist++;
        if(dist_prise > 0)
            minmax = effectue_depl_sans_prise(li, co, piece, coul, li+ARRIERE*coul*dist, co+DROITE*coul*dist, numBL, numNO, prof, minmax, num_mouv, m);


        return minmax;
    }


int effectue_depl_sans_prise ( int li , int co , int piece , int coul , int liar , int coar ,
                               int numBL , int numNO , int prof , int minmax , tmm m[ PROF ][ PRISE ] )
    {
       
    }

