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



