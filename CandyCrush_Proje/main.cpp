#define FPS_LIMIT 60
#include <iostream>
#include <vector>
#include <iomanip>
#include <thread>
#include <affichage.h>



using namespace std;

//***********************************************************************************/
//***********************    R1.01 – Prog#10 Exercice 1   ***************************/
//***********************************************************************************/

typedef ushort contenueDUneCase;
typedef vector <contenueDUneCase> CVLigne; // un type représentant une ligne de la grille
typedef vector <CVLigne> CMatrice; // un type représentant la grille

const contenueDUneCase KAIgnorer = 0;
const contenueDUneCase KPlusGrandNombreDansLaMatrice = 4;

void clearScreen () {
    cout << "\033[H\033[2J";
}

void couleur (const unsigned & coul) {
    cout << "\033[" << coul <<"m";
}

void initMat(CMatrice & mat , const size_t & nbLignes,const size_t & nbColonnes,const unsigned & nbMax= KPlusGrandNombreDansLaMatrice ){
    mat.resize(nbColonnes);
    for(unsigned k=0;k<nbColonnes;k=k+1){
        CVLigne nouvelle_matrice(nbLignes,0);
        for(unsigned i=0;i<nbColonnes;i=i+1){
            ushort contenueDUneCase=1+rand()%(nbMax - 1 + 1);
            nouvelle_matrice[i]=contenueDUneCase;
        }
        mat[k]=nouvelle_matrice;
    }
}

void afficheMatriceV0( const CMatrice & Mat ){
    int m =0;
    for(unsigned k=0;k<Mat.size();k=k+1){
        cout << "|" << " " << m << "| " ;
        for(unsigned i=0;i<Mat[0].size();i=i+1){
            if(Mat[k][i]==1){
                couleur(KRouge);
                cout << Mat[k][i];
                couleur(KReset);
            }
            if(Mat[k][i]==2){
                couleur(KCyan);
                cout << Mat[k][i];
                couleur(KReset);
            }
            if(Mat[k][i]==3){
                couleur(KMAgenta);
                cout << Mat[k][i];
                couleur(KReset);
            }
            if(Mat[k][i]==4){
                couleur(KJaune);
                cout << Mat[k][i];
                couleur(KReset);
            }
            if(Mat[k][i]==5){
                couleur(KBleu);
                cout << Mat[k][i];
                couleur(KReset);
            }
            if(Mat[k][i]==0){
                couleur(KBGRouge);
                cout << " ";
                couleur(KReset);
            }
            cout << " | ";
        }
        cout << endl;
        m=m+1;
    }
}


//***********************************************************************************/
//***********************    R1.01 – Prog#10 Exercice 2   ***************************/
//***********************************************************************************/


// on remplira cela plus tard, juste la définition de la fonction

void explositionUneBombeHorizontale (CMatrice & mat, const size_t & numLigne,const size_t & numColonne, const size_t & combien,const unsigned & nbMax=KPlusGrandNombreDansLaMatrice){
    for(unsigned k=numLigne;k>0;k=k-1){
        for(unsigned i=numColonne;i<combien+numColonne;i=i+1){
            mat[k][i]=mat[k-1][i];
        }
    }
    /* for(unsigned i=numColonne;i<combien+numColonne;i=i+1){
        mat[0][i]=KAIgnorer;
    }*/
    // deuxieme version ou il y a des fruits qui reviennent
    for(unsigned i=numColonne;i<combien+numColonne;i=i+1){
        mat[0][i]=1+rand()%(nbMax - 1 + 1);
    }
}


bool detectionExplositionUneBombeHorizontale (CMatrice & mat,unsigned & score){
    bool auMoinsUneExplosionHorizontal=false;
    for(unsigned k=0;k<mat.size();k=k+1){
        size_t combienALaSuite=1;
        int nombreEnCours=0;
        for(unsigned i=0;i<mat[k].size();i=i+1){
            if(mat[k][i]==KAIgnorer){
                nombreEnCours=0;
                continue;
            }
            if(mat[k][i]==nombreEnCours){
                combienALaSuite=combienALaSuite+1;
            }
            if (combienALaSuite >= 3){
                int numCol=i+1-combienALaSuite;
                int numLigne=k+1;
                auMoinsUneExplosionHorizontal = true;
                cout << "on a une suite en position numLigne = " << numLigne<< "; colonne = " << numCol<< "; sur  " << combienALaSuite << " cases" << endl;
                cout << string (20, '-') << endl << "matrice avant suppresion et " << auMoinsUneExplosionHorizontal << endl;
                afficheMatriceV0(mat);
                explositionUneBombeHorizontale (mat, k, numCol, combienALaSuite);
                cout << string (20, '-') << endl << "matrice après suppresion" << endl;
                            afficheMatriceV0(mat);
                score=score+100;
                nombreEnCours=mat[k][i];
                combienALaSuite=1;

            }
            if(mat[k][i]!=nombreEnCours){
                nombreEnCours=mat[k][i];
                combienALaSuite=1;
            }
        }
    }
    //on parcours la matrice case / case
    // si on tombe sur la valeur KAIgnorer, on passe a la case suivante
    // sinon on compte combien de fois on a la même valeur
    //si on a aun moins 3 chiffres identiques a la suite
    return auMoinsUneExplosionHorizontal;
}
/**
 * @brief explositionUneBombeVertical=fonction decalant les chiffres après explosion
 * @param mat=
 * @param numLigne= numéro de la ligne
 * @param numColonne= numéro de la colonne
 * @param combien =
 * @param nbMax = chiffre a placé < =4
 */
void explositionUneBombeVertical (CMatrice & mat, const size_t & numLigne,const size_t & numColonne, const size_t & combien,const unsigned & nbMax=KPlusGrandNombreDansLaMatrice){
    for(unsigned k=numLigne-1;k>combien-1;k=k-1){
        mat[k][numColonne]=mat[k-3][numColonne];
    }
    for(unsigned i=0;i<combien;i=i+1){
        mat[i][numColonne]=1+rand()%(nbMax - 1 + 1);
    }
    // deuxieme version ou il y a des fruits qui reviennent
    /*    for(unsigned i=numColonne;i<combien+numColonne;i=i+1){
        mat[0][i]=1+rand()%(nbMax - 1 + 1);
    }*/

}
/**
 * @brief detectionExplositionUneBombeVertical= fonction verifiant si 3 chiffre identique sont alignées
 * @param mat = initialisation de la matrice
 * @param score = compte le score
 * @return
 */
bool detectionExplositionUneBombeVertical (CMatrice & mat,unsigned & score){
    bool auMoinsUneExplosionVertical=false;
    for(unsigned k=0;k<mat.size()-2;k=k+1){
        for(unsigned i=0;i<mat[0].size();i=i+1){
            size_t combienALaSuite=1;
            unsigned nombreEnCours=mat[k][i];
            while(mat[k+combienALaSuite][i]==nombreEnCours && k+combienALaSuite<mat.size()-1){
                combienALaSuite=combienALaSuite+1;
                if(mat[k][i]==KAIgnorer){
                    nombreEnCours=0;
                    break;
                }
                if(combienALaSuite >= 3){
                    size_t numCol=i;
                    size_t numLigne=k+combienALaSuite;
                    auMoinsUneExplosionVertical=true;
                    cout << " on a une suite en position numLigne = " << numLigne<< "; colonne = " << numCol<< "; sur  " << combienALaSuite << " cases" << endl;
                    cout << string (20, '-') << endl << "matrice avant suppresion" << endl;
                    afficheMatriceV0(mat);
                    explositionUneBombeVertical(mat, numLigne, numCol, combienALaSuite);
                    cout << string (20, '-') << endl << "matrice après suppresion" << endl;
                                afficheMatriceV0(mat);
                    score=score+100;
                    break;
                }
            }
        }
    }
    return auMoinsUneExplosionVertical;
}

//***********************************************************************************/
//***********************    R1.01 – Prog#10 Exercice 3   ***************************/
//***********************************************************************************/


//fait descendre toutes les cases d'une unité suite à une explosition




int ppalExo03 (){
    CMatrice mat;
    initMat(mat,10,10);
    unsigned score=0;
    // affichage de la matrice sans les numéros de lignes / colonnes en haut / à gauche
    afficheMatriceV0(mat);
    detectionExplositionUneBombeVertical(mat,score);

    while (detectionExplositionUneBombeVertical(mat,score)==true) {
        cout << "bonjour" << endl;
        //zafficheMatriceV0 (mat);
    }
    return 0;
}

int ppalExo04 (){
    CMatrice mat;
    initMat(mat,10,10);
    // affichage de la matrice sans les numéros de lignes / colonnes en haut / à gauche
    //detectionExplositionUneBombeHorizontale (mat);
    //afficheMatriceV2 (mat);
    //condition de victoire a trouver
    while (true) {
        afficheMatriceV0 (mat);
        cout << "Fait un mouvement ";
        cout << "numero de ligne : "<<endl;
        size_t numLigne;
        cin >> numLigne;
        cout << "numero de colonne : "<<endl;
        size_t numCol;
        cin >> numCol;
        cout << "Sens du deplacement : (A|Z|E|Q|D|W|X|C) : " << endl;
        char deplacement;
        cin >> deplacement;
        faitUnMouvement (mat, deplacement, numLigne, numCol);
        //detectionExplositionUneBombeHorizontale (mat);
        afficheMatriceV0 (mat);
    }
    return 0;
}

void FaireUnTour(CMatrice & mat,unsigned & score){
    afficheMatriceV0 (mat);
    cout << "Fait un mouvement ";
    cout << "numero de ligne : "<<endl;
    size_t numLigne;
    cin >> numLigne;
    cout << "numero de colonne : "<<endl;
    size_t numCol;
    cin >> numCol;
    cout << "Sens du deplacement : (A|Z|E|Q|D|W|X|C) : " << endl;
    char deplacement;
    cin >> deplacement;
    faitUnMouvement (mat, deplacement, numLigne, numCol);
    //detectionExplositionUneBombeHorizontale (mat);
    //afficheMatriceV0 (mat);
    while(true){
        if(detectionExplositionUneBombeVertical(mat,score)==false && detectionExplositionUneBombeHorizontale(mat,score)==false) {
            break;
        }
    }
}

void Niveau_1(){
    CMatrice mat;
    initMat(mat,10,10);
    unsigned score=0;
    while (detectionExplositionUneBombeVertical(mat,score)==true && detectionExplositionUneBombeHorizontale(mat,score)==true) {
    }
    unsigned objectif=1500,nbTour=0,nbTourMax=30;
    score=0;
    while(nbTour != nbTourMax || score<objectif){
        FaireUnTour(mat,score);
        cout << "tu es au tour :" << nbTour << endl;
        cout << "tu as "  << score << " points !" << endl;
        nbTour=nbTour+1;
    }
    if(nbTour == 30){
        cout << "désolé ta perdu :(" << endl;
    }
    else{
        cout << "ta gagné gg !!!" << endl;
    }
}

void cree_niveau(){
    cout << "Tu va pouvoir crée ton niveau en déterminant la taille du plateau de jeu, le nombre de bonbon et enfin le mode de jeu que tu souhaites ( entre ceux qui sont disponiles )"<< endl;
    cout << "Commence pas choisir la taille du plateau ( 4*4 au plus petit et 10*10 ) au plus grand " << endl;
    unsigned longueur,nb_bonbon;
    string ModeDeJeu;
    cin >> longueur;
    cout << "Maintenant choisi le nombre de bonbon que tu veux dans le jeu ( entre 3 et 5 )";
    cin >> nb_bonbon;
    cout << "Choix mode de jeu : Score , Noisette , Bonbon";
    cin >> ModeDeJeu;
    while(ModeDeJeu!="Score" || ModeDeJeu!="Noisette" || ModeDeJeu!="Bonbon"){
        cin >> ModeDeJeu;
    }
    if(ModeDeJeu=="Score"){
        CMatrice mat;
        initMat(mat,10,10);
        unsigned Tour_max,scoreAAtteindre,score=0,nbTour=1;
        cout << "Combien de tour max vous voulez ? ( entre 5 et 30 )" << endl;
        cin >> Tour_max;
        cout << "Quel est le score a atteindre pour le niveau ? " << endl;
        cin >> scoreAAtteindre;
        while (detectionExplositionUneBombeVertical(mat,score)==true && detectionExplositionUneBombeHorizontale(mat,score)==true) {
        }
        while(true){
            FaireUnTour(mat,score);
            cout << "tu es au tour :" << nbTour << endl;
            cout << "tu as "  << score << " points !" << endl;
            nbTour=nbTour+1;
            if(nbTour == Tour_max){
                cout << "désolé ta perdu :(" << endl;
                    break;
            }
            if(score>=scoreAAtteindre){
                cout << "ta gagné gg !!!" << endl;
                    break;
            }
        }
    }
    if(ModeDeJeu=="Noisette"){
        //A_Faire
    }
    if(ModeDeJeu=="Bonbon"){
        //A_Faire
    }
}

void Niveau_1V2(){
    CMatrice mat;
    initMat(mat,10,10);
    unsigned score=0;
    while (detectionExplositionUneBombeVertical(mat,score)==true && detectionExplositionUneBombeHorizontale(mat,score)==true) {
    }
    unsigned objectif=1500,nbTour=0,nbTourMax=10;
    score=0;
    while(true){
        FaireUnTour(mat,score);
        cout << "tu es au tour :" << nbTour << endl;
        cout << "tu as "  << score << " points !" << endl;
        nbTour=nbTour+1;
        if(nbTour == nbTourMax){
            cout << "désolé ta perdu :(" << endl;
                break;
        }
        if(score>=objectif){
            cout << "ta gagné gg !!!" << endl;
                break;
        }
    }
}




int main() {
    Niveau_1V2();

}
