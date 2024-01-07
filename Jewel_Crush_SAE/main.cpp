#include "mingl/gui/text.h"
#include <unistd.h>
#define FPS_LIMIT 60
#include <iostream>
#include <vector>
#include <iomanip>
#include <thread>
#include "mingl/mingl.h"
#include "mingl/shape/rectangle.h"
#include "mingl/shape/circle.h"
#include "mingl/shape/line.h"
#include "mingl/shape/triangle.h"
#include "mingl/gui/sprite.h"
#include <fstream>

using namespace std;

typedef ushort contenueDUneCase;
typedef vector <contenueDUneCase> CVLigne; // un type représentant une ligne de la grille
typedef vector <CVLigne> CMatrice; // un type représentant la grille

const contenueDUneCase KAIgnorer = 0;
const contenueDUneCase KPlusGrandNombreDansLaMatrice = 4;

using namespace std;

/**
 * @brief The CMyParam class : Classe qui va permettre de stocké les informations de mouvements et de jeu
 */
struct CMyParam {
    std::map <std::string, char> mapParamChar;
    std::map <std::string, unsigned> mapParamUnsigned;
};

/**
 * @brief clearScreen : Fonction qui sert a réintialisser la couleur
 */
void clearScreen () {
    cout << "\033[H\033[2J";
}

const unsigned KReset   (0);
const unsigned KNoir    (30);
const unsigned KRouge   (31);
const unsigned KVert    (32);
const unsigned KJaune   (33);
const unsigned KBleu    (34);
const unsigned KMAgenta (35);
const unsigned KCyan    (36);
const unsigned KBGNoir    (40);
const unsigned KBGRouge   (41);
const unsigned KGBBleu    (44);
const unsigned KBGCyan    (46);

void couleur (const unsigned & coul) {
    cout << "\033[" << coul <<"m";
}

/**
 * @brief initMat : Fonction qui initialise la matrice de jeu
 * @param mat : Matrice utilisé dans le niveau
 * @param numLigne : numéro de la ligne du premier indice de la suite de 3 gemmes
 * @param numColonne : numéro de la colonne du premier indice de la suite des 3 gemmes
 * @param nbMax : nombre de bonbon différent dans la matrice de jeu
 */
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

/**
 * @brief initParams : Fonction qui sert a initialiser les parametres par défault si le fichier yaml est introuvable
 * @param Param : Structure qui enregistre les paramètre de mouvement de l'utilisateur
 */
void initParams (CMyParam & Param)
{
    //touche du joueur
    Param.mapParamChar["toucheHaut"] = 'z';
    Param.mapParamChar["toucheGauche"] = 'q';
    Param.mapParamChar["toucheDroite"] = 'd';
    Param.mapParamChar["toucheBas"] = 'x';
    Param.mapParamChar["toucheHautDroite"] = 'e';
    Param.mapParamChar["toucheHautGauche"] = 'a';
    Param.mapParamChar["toucheBasGauche"] = 'w';
    Param.mapParamChar["toucheBasDroite"] = 'c';

    //taille de la grille - on suppose que c'est un rectangle
    Param.mapParamUnsigned["nbColonnes"] = 10;
    Param.mapParamUnsigned["nbLignes"] = 10;

}
/**
 * @brief chargerConfig Fonction qui charge la configuration de mouvement entrée dans le fichier yaml
 * @param param : Structure qui enregistre les paramètres de mouvements de l'utilisateur
 * @param fichier : Nom du fichier dans lequel chercher les parametres de mouvement
 */
void chargerConfig(CMyParam & param, const string & fichier){
    ifstream ifs (fichier);
    if (!ifs) cerr <<"pas de fichier du nom de : "<<fichier;
    string cle;
    while (ifs >> cle){
        char deuxPts;
        ifs >> deuxPts;
        if (param.mapParamUnsigned.find(cle) != param.mapParamUnsigned.end()){
            unsigned val;
            ifs >> val;
            param.mapParamUnsigned[cle] = val;
        }
        else if (param.mapParamChar.find(cle) != param.mapParamChar.end()){
            char val;
            ifs >> val;
            param.mapParamChar[cle] = val;
        }
        else {
            string tmp;
            getline(ifs, tmp);

        }
    }
}

/**
 * @brief afficheMatriceV0 : Fonction qui sert a afficher la matrice sur le terminal
 * @param Mat : Matrice utilisé dans le niveau
 */
void afficheMatriceV0( const CMatrice & Mat  ){
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
/**
 * @brief afficheMatrice_Interface_G : Fonction qui sert a afficher la Matrice de jeu sur MinGl
 * @param Mat : Matrice utilisé dans le niveau
 * @param window : Fênetre ouverte par l'interface graphique MinGL
 */
void afficheMatrice_Interface_G(const CMatrice & Mat,MinGL & window){
    unsigned coord_colonne=200;
    for(unsigned k=0;k<Mat.size();k=k+1){
        unsigned coord_ligne=250;
        for(unsigned i=0;i<Mat[0].size();i=i+1){
            if(Mat[k][i]==1){
                window << nsGui::Sprite("../Jewel_Crush_SAE/gemme-1-reussi",nsGraphics::Vec2D(coord_ligne,coord_colonne));
            }
            if(Mat[k][i]==2){
                window << nsGui::Sprite("../Jewel_Crush_SAE/gemme-2-reussi",nsGraphics::Vec2D(coord_ligne,coord_colonne));
            }
            if(Mat[k][i]==3){
                window << nsGui::Sprite("../Jewel_Crush_SAE/gemme-3-reussi",nsGraphics::Vec2D(coord_ligne,coord_colonne));
            }
            if(Mat[k][i]==4){
                window << nsGui::Sprite("../Jewel_Crush_SAE/gemme-4-reussi",nsGraphics::Vec2D(coord_ligne,coord_colonne));
            }
            if(Mat[k][i]==5){
                window << nsGui::Sprite("../Jewel_Crush_SAE/lingot-dor",nsGraphics::Vec2D(coord_ligne,coord_colonne));
            }
            if(Mat[k][i]==0){
                couleur(KBGRouge);
                cout << " ";
                couleur(KReset);
            }
            coord_ligne=coord_ligne+80;
        }
        coord_colonne=coord_colonne+80;
    }
}

/**
 * @brief explositionUneBombeHorizontale : fonction qui sert a faire exploser la suite de 3 gemmes horizontal, et faire tomber les autres fruits pour continuer le jeu
 * @param mat : Matrice utilisé dans le niveau
 * @param numLigne : numéro de la ligne du premier indice de la suite de 3 gemmes
 * @param numColonne : numéro de la colonne du premier indice de la suite des 3 gemmes
 * @param combien : Nombres de gemmes dans la suite détecté
 * @param nbMax : nombre de bonbon différent dans la matrice de jeu
 */
void explositionUneBombeHorizontale (CMatrice & mat, const size_t & numLigne,const size_t & numColonne, const size_t & combien,const unsigned & nbMax=KPlusGrandNombreDansLaMatrice){
    for(unsigned k=numLigne;k>0;k=k-1){
        for(unsigned i=numColonne;i<combien+numColonne;i=i+1){
            mat[k][i]=mat[k-1][i];
        }
    }
    for(unsigned i=numColonne;i<combien+numColonne;i=i+1){
        mat[0][i]=1+rand()%(nbMax - 1 + 1);
    }
}

/**
 * @brief detectionExplositionUneBombeHorizontale : Fonction qui sert a détecter une suite de gemmes horizontal de 3 gemmes ou +
 * @param mat : Matrice utilisé dans le niveau
 * @param score : Score actuel dans le niveau
 * @return : return si il y a une suite de 3 gemmes ou + ( booléen )
 */
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
    return auMoinsUneExplosionHorizontal;
}
/**
 * @brief explositionUneBombeVertical : fonction qui sert a faire exploser la suite de 3 gemmes vertical, et faire tomber les autres fruits pour continuer le jeu
 * @param mat : Matrice utilisé dans le niveau
 * @param numLigne : numéro de la ligne du premier indice de la suite de 3 gemmes
 * @param numColonne : numéro de la colonne du premier indice de la suite des 3 gemmes
 * @param combien : Nombres de gemmes dans la suite détecté
 * @param nbMax : nombre de bonbon différent dans la matrice de jeu
 */
void explositionUneBombeVertical (CMatrice & mat, const size_t & numLigne,const size_t & numColonne, const size_t & combien,const unsigned & nbMax=KPlusGrandNombreDansLaMatrice){
    for(unsigned k=numLigne-1;k>combien-1;k=k-1){
        mat[k][numColonne]=mat[k-3][numColonne];
    }
    for(unsigned i=0;i<combien;i=i+1){
        mat[i][numColonne]=1+rand()%(nbMax - 1 + 1);
    }
}
/**
 * @brief detectionExplositionUneBombeVertical : Fonction qui sert a détecter une suite de gemmes vertical de 3 gemmes ou +
 * @param mat : Matrice utilisé dans le niveau
 * @param score : Score actuel dans le niveau
 * @return return si il y a une suite de 3 gemmes ou + ( booléen )
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
/**
 * @brief faitUnMouvement : Fonction qui sert a intervertir deux gemmes dans la matrice de jeu
 * @param mat : Matrice utilisé dans le niveau
 * @param deplacment : Sens dans lequel le joueur veut faire le mouvement
 * @param numLigne : indice de ligne ou la gemme que le joueur veut bouger
 * @param numCol : indice de colonne ou la gemme que le joueur veut bouger
 * @param param : Parametre défini par le fichier yaml
 */
void faitUnMouvement (CMatrice & mat, const char & deplacment, const size_t & numLigne,
                     const size_t & numCol, const CMyParam & param) {
    size_t nouvellePositionLigne (numLigne), nouvellePositionColonne (numCol);
    auto it (param.mapParamChar.begin());
    for ( ; it != param.mapParamChar.end() && it ->second != tolower(deplacment); ++it){
    }
            if(it->first == "toucheHautGauche"){
                nouvellePositionColonne = numCol-1;
                nouvellePositionLigne = numLigne-1;
            }
            if(it->first == "toucheHaut"){
                nouvellePositionColonne = numCol;
                nouvellePositionLigne = numLigne-1;
            }
            if(it->first == "toucheHautDroite"){
                nouvellePositionColonne = numCol+1;
                nouvellePositionLigne = numLigne-1;
            }
            if(it->first == "toucheGauche"){
                nouvellePositionColonne = numCol-1;
                nouvellePositionLigne = numLigne;
            }
            if(it->first == "toucheDroite"){
                nouvellePositionColonne = numCol+1;
                nouvellePositionLigne = numLigne;
            }
            if(it->first == "toucheBasGauche"){
                nouvellePositionColonne = numCol-1;
                nouvellePositionLigne = numLigne+1;
            }
            if(it->first == "toucheBas"){
                nouvellePositionColonne = numCol;
                nouvellePositionLigne = numLigne+1;
            }
            if(it->first == "toucheBasDroite"){
                nouvellePositionColonne = numCol+1;
                nouvellePositionLigne = numLigne+1;
            }
    swap(mat[numLigne][numCol],mat[nouvellePositionLigne][nouvellePositionColonne]);
}

/**
 * @brief FaireUnTour : Fonction qui sert a tour dans le niveau
 * @param mat : Matrice utilisé dans le niveau
 * @param score : score actuel du joueur dans le niveau
 */
void FaireUnTour(CMatrice & mat,unsigned & score){
    afficheMatriceV0 (mat);
    CMyParam Param;
    string fichier="config.yaml";
    initParams(Param);
    chargerConfig(Param,fichier);
    cout << Param.mapParamChar["MouvementHautGauche"] << endl;
    cout << "Fait un mouvement ";
    cout << "numero de ligne : "<<endl;
    size_t numLigne;
    cin >> numLigne;
    cout << "numero de colonne : "<<endl;
    size_t numCol;
    cin >> numCol;
    cout << "Sens du deplacement : (" << Param.mapParamChar["toucheHautGauche"] << "|" << Param.mapParamChar["toucheHaut"] << "|" << Param.mapParamChar["toucheHautDroite"]
         << "|" << Param.mapParamChar["toucheGauche"] << "|" << Param.mapParamChar["toucheDroite"] << "|" << Param.mapParamChar["toucheBasDroite"] << "|" << Param.mapParamChar["toucheBas"] << "|" << Param.mapParamChar["toucheBasDroite"] << ")" << endl;
    char deplacement;
    cin >> deplacement;
    faitUnMouvement (mat, deplacement, numLigne, numCol,Param);
    while(true){
        if(detectionExplositionUneBombeVertical(mat,score)==false && detectionExplositionUneBombeHorizontale(mat,score)==false) {
            break;
        }
    }
}

/**
 * @brief cree_niveau : Fonction qui sert a créer un niveau ( fonction incomplete ), en choississant le nombre de bonbon, la taille de la matrice et le mode de jeu
 */
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
        //A_Complété
    }
    if(ModeDeJeu=="Bonbon") {
        //A_Complété
    }
}

/**
 * @brief Niveau_1 : Fonction qui sert a jouer au niveau 1 sur le terminal
 */
void Niveau_1(){
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

//Niveau 3
/**
 * @brief detectionExplositionUneBombeHorizontaleGemme : Fonction qui sert a détecter une suite de gemmes horizontal de 3 gemmes ou +
 * @param mat : Matrice utilisé dans le niveau
 * @param gemmeRestante : Nombre de gemme restante a detruire dans le niveau
 * @param gemmeADetruire : Le type de gemme a detruire dans le niveau
 * @return
 */
bool detectionExplositionUneBombeHorizontaleGemme (CMatrice & mat, unsigned & gemmeRestante, const unsigned & gemmeADetruire){
    bool auMoinsUneExplosionHorizontal=false;
    for(unsigned k=0;k<mat.size();k=k+1){
        size_t combienALaSuite=1;
        unsigned gemmeEnCours=0;

        for(unsigned i=0;i<mat[k].size();i=i+1){
            if(mat[k][i]==KAIgnorer){
                gemmeEnCours=0;
                continue;
            }
            else if(mat[k][i]==gemmeEnCours){
                combienALaSuite=combienALaSuite+1;
            }
            if (combienALaSuite >= 3){
                unsigned numCol=i+1-combienALaSuite;
                unsigned numLigne=k+1;
                auMoinsUneExplosionHorizontal = true;
                cout << "on a une suite en position numLigne = " << numLigne<< "; colonne = " << numCol<< "; sur  " << combienALaSuite << " cases" << endl;
                cout << string (20, '-') << endl << "matrice avant suppresion et " << auMoinsUneExplosionHorizontal << endl;

                afficheMatriceV0(mat);
                cout << mat[k][i] << " est le chiffre de notre suppression et " << gemmeEnCours << " aussi" << endl;
                if(gemmeADetruire == gemmeEnCours){
                    gemmeRestante = gemmeRestante-3;
                }
                else{
                    cout << "non";
                }
                explositionUneBombeHorizontale (mat, k, numCol, combienALaSuite);
                cout << string (20, '-') << endl << "matrice après suppresion" << endl;
                            afficheMatriceV0(mat);

                cout << "il reste "<< gemmeRestante << " gemme(s)" << endl;

                gemmeEnCours=mat[k][i];
                combienALaSuite=1;

            }
            if(mat[k][i]!=gemmeEnCours){
                gemmeEnCours=mat[k][i];
                combienALaSuite=1;
            }
        }
    }
    return auMoinsUneExplosionHorizontal;
}

/**
 * @brief detectionExplositionUneBombeVerticalGemme : Fonction qui sert a détecter une suite de gemmes vertical de 3 gemmes ou +
 * @param mat : Matrice utilisé dans le niveau
 * @param gemmeRestante : Nombre de gemme restante a detruire dans le niveau
 * @param gemmeADetruire : Le type de gemme a detruire dans le niveau
 * @return
 */
bool detectionExplositionUneBombeVerticalGemme (CMatrice & mat,unsigned & gemmeRestante, const unsigned gemmeADetruire){
    bool auMoinsUneExplosionVertical=false;
    for(unsigned k=0;k<mat.size()-2;k=k+1){
        for(unsigned i=0;i<mat[0].size();i=i+1){
            size_t combienALaSuite=1;
            unsigned gemmeEnCours=mat[k][i];
            while(mat[k+combienALaSuite][i]==gemmeEnCours && k+combienALaSuite<mat.size()-1){
                combienALaSuite=combienALaSuite+1;
                if(mat[k][i]==KAIgnorer){
                    gemmeEnCours=0;
                    break;
                }
                if(combienALaSuite >= 3){
                    size_t numCol=i;
                    size_t numLigne=k+combienALaSuite;
                    auMoinsUneExplosionVertical=true;
                    cout << " on a une suite en position numLigne = " << numLigne<< "; colonne = " << numCol<< "; sur  " << combienALaSuite << " cases" << endl;
                    cout << string (20, '-') << endl << "matrice avant suppresion" << endl;
                    afficheMatriceV0(mat);
                    cout << mat[k][i] << " est le chiffre de notre suppression" << endl;
                    if(gemmeADetruire == gemmeEnCours){
                        gemmeRestante = gemmeRestante-3;
                    }
                    else{
                        cout << "nonV";
                    }

                    explositionUneBombeVertical(mat, numLigne, numCol, combienALaSuite);
                    cout << string (20, '-') << endl << "matrice après suppresion" << endl;
                                afficheMatriceV0(mat);
                    if(gemmeADetruire == mat[k][i]){
                        gemmeRestante = gemmeRestante - 3;
                    }


                    break;
                }
            }
        }
    }
    return auMoinsUneExplosionVertical;
}

/**
 * @brief FaireUnTourGemme : Fonction qui sert a faire un tour dans les niveaux de type gemme
 * @param mat : Matrice utilisé dans le niveau
 * @param gemmeRestante : Nombre de gemme restante a detruire dans le niveau
 * @param gemmeADetruire : Le type de gemme a detruire dans le niveau
 */
void FaireUnTourGemme(CMatrice & mat,unsigned & gemmeRestante, const unsigned & gemmeADetruire){
    afficheMatriceV0 (mat);
    CMyParam Param;
    string fichier="config.yaml";
    initParams(Param);
    chargerConfig(Param,fichier);
    cout << Param.mapParamChar["MouvementHautGauche"] << endl;
    cout << "Fait un mouvement ";
    cout << "numero de ligne : "<<endl;
    size_t numLigne;
    cin >> numLigne;
    cout << "numero de colonne : "<<endl;
    size_t numCol;
    cin >> numCol;
    cout << "Sens du deplacement : (" << Param.mapParamChar["toucheHautGauche"] << "|" << Param.mapParamChar["toucheHaut"] << "|" << Param.mapParamChar["toucheHautDroite"]
         << "|" << Param.mapParamChar["toucheGauche"] << "|" << Param.mapParamChar["toucheDroite"] << "|" << Param.mapParamChar["toucheBasDroite"] << "|" << Param.mapParamChar["toucheBas"] << "|" << Param.mapParamChar["toucheBasDroite"] << ")" << endl;
    char deplacement;
    cin >> deplacement;
    faitUnMouvement (mat, deplacement, numLigne, numCol,Param);
    while(detectionExplositionUneBombeVerticalGemme(mat,gemmeRestante, gemmeADetruire) == true || detectionExplositionUneBombeHorizontaleGemme(mat, gemmeRestante, gemmeADetruire) == true){
    }
}

/**
 * @brief Niveau_3 : Fonction qui sert a jouer au niveau 3
 */
void Niveau_3(){
    CMatrice mat;
    initMat(mat,7,7);
    unsigned gemmeRestante = 0, gemmeADetruire = (rand() % (4-1+1))+1;
    while (detectionExplositionUneBombeVerticalGemme(mat, gemmeRestante, gemmeADetruire)==true
           || detectionExplositionUneBombeHorizontaleGemme(mat, gemmeRestante, gemmeADetruire)==true){}
    cout << endl <<  "Initialisation complete." << endl << gemmeRestante << " gemmes restantes avant commencer." << endl;
    gemmeRestante = 50;
    cout << gemmeRestante << " pour le début de la partie. C'est partie !!!" << endl << endl;
    cout << "Bienvenue dans le mode de jeu Gemme" << endl << endl << "La gemme a détruire est : " << gemmeADetruire << endl;
    unsigned nbTour=0,nbTourMax=20;
    while(true){
        nbTour = nbTour + 1;
        FaireUnTourGemme(mat, gemmeRestante, gemmeADetruire);
        cout << "tu es au tour :" << nbTour << endl;
        cout << "il ne te reste plus que "  << gemmeRestante << " gemme numéro " << gemmeADetruire << " à détruire !" << endl;
                nbTour=nbTour+1;
        if(gemmeRestante <= 0){
            cout << "tu as gagné gg !!!" << endl;
                break;
        }
        else if(nbTour == nbTourMax){
            cout << "désolé ta perdu :(" << endl;
                break;
        }

    }
}

//MinGL

/**
 * @brief affiche_niveau2 : Fonction qui sert a afficher le niveau 2 sur MinGL
 * @param Mat : Matrice utilisé dans le niveau
 * @param window : Fênetre ouverte par l'interface graphique MinGL
 * @param nbTourRestant : Nombre de tour Restant dans le niveau 2
 */
void affiche_niveau2(const CMatrice & Mat,MinGL & window,const unsigned & nbTourRestant){
    window << nsGui::Sprite("../Jewel_Crush_SAE/fond-ecran-jolie",nsGraphics::Vec2D(0,0));
    window << nsGui::Text(nsGraphics::Vec2D(30,30),std::string("Pour utiliser les touches du clavier il faut laisser la touche enfoncer un certain temps !"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    afficheMatrice_Interface_G(Mat,window);
    window << nsShape::Rectangle(nsGraphics::Vec2D(1100,100),nsGraphics::Vec2D(1550,700),nsGraphics::KMagenta);
    window << nsGui::Text(nsGraphics::Vec2D(1270,200),std::string("Niveau 2"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(1150,300),std::string("Objectif : "),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(1250,300),std::string("Faire tomber le lingot d'or tout en bas"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(1150,400),std::string("Nombre de tour restant : "),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(1420,400),std::string(to_string(nbTourRestant)),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
}

/**
 * @brief affiche_niveau1 : Fonction qui sert a afficher le niveau 1 sur MinGL
 * @param Mat : Matrice utilisé dans le niveau
 * @param window : Fênetre ouverte par l'interface graphique MinGL
 * @param score : Score actuel du joueur dans le niveau
 * @param nbTourRestant : Nombre de tour Restant dans le niveau 1
 * @param objectif : Objectif de point a atteindre dans le niveau pour gagner
 */
void affiche_niveau1(const CMatrice & Mat,MinGL & window,const unsigned & score,const unsigned & nbTourRestant,const unsigned & objectif){
    window << nsGui::Sprite("../Jewel_Crush_SAE/fond-ecran-jolie",nsGraphics::Vec2D(0,0));
    window << nsGui::Text(nsGraphics::Vec2D(30,30),std::string("Pour utiliser les touches du clavier il faut laisser la touche enfoncer un certain temps !"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    afficheMatrice_Interface_G(Mat,window);
    window << nsShape::Rectangle(nsGraphics::Vec2D(1100,100),nsGraphics::Vec2D(1550,700),nsGraphics::KMagenta);
    window << nsGui::Text(nsGraphics::Vec2D(1270,200),std::string("Niveau 1"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(1150,300),std::string("Score :"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(1250,300),std::string(to_string(score)),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(1150,400),std::string("Nombre de tour restant : "),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(1420,400),std::string(to_string(nbTourRestant)),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(1150,500),std::string("objectif a atteindre :"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(1380,500),std::string(to_string(objectif)),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
}

/**
 * @brief affiche_niveau3 : Fonction qui sert a afficher le niveau 3 sur MinGL
 * @param Mat : Matrice utilisé dans le niveau
 * @param window : Fênetre ouverte par l'interface graphique MinGL
 * @param nbTourRestant : Nombre de tour Restant dans le niveau 3
 * @param gemmeRestante : Nombre de gemme restante a detruire dans le niveau 3
 * @param gemmeADetruire : Le type de gemme a detruire dans le niveau
 */
void affiche_niveau3(const CMatrice & Mat,MinGL & window,const unsigned & nbTourRestant,const unsigned & gemmeRestante,unsigned & gemmeADetruire){
    window << nsGui::Sprite("../Jewel_Crush_SAE/fond-ecran-jolie",nsGraphics::Vec2D(0,0));
    window << nsGui::Text(nsGraphics::Vec2D(30,30),std::string("Pour utiliser les touches du clavier il faut laisser la touche enfoncer un certain temps !"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    afficheMatrice_Interface_G(Mat,window);
    window << nsShape::Rectangle(nsGraphics::Vec2D(1100,100),nsGraphics::Vec2D(1550,700),nsGraphics::KMagenta);
    window << nsGui::Text(nsGraphics::Vec2D(1270,200),std::string("Niveau 3"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(1150,300),std::string("Bonbon a casser :"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    if(gemmeADetruire==1){
        window << nsGui::Sprite("../Jewel_Crush_SAE/gemme-1-reussi",nsGraphics::Vec2D(1250,300));
    }
    if(gemmeADetruire==2){
        window << nsGui::Sprite("../Jewel_Crush_SAE/gemme-2-reussi",nsGraphics::Vec2D(1250,300));
    }
    if(gemmeADetruire==3){
        window << nsGui::Sprite("../Jewel_Crush_SAE/gemme-3-reussi",nsGraphics::Vec2D(1250,300));
    }
    if(gemmeADetruire==4){
        window << nsGui::Sprite("../Jewel_Crush_SAE/gemme-4-reussi",nsGraphics::Vec2D(1250,300));
    }
    window << nsGui::Text(nsGraphics::Vec2D(1150,400),std::string("Nombre de bonbon restant a casse :"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(1300,440),std::string(to_string(gemmeRestante)),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(1150,500),std::string("Nombre de tour restant : "),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(1440,500),std::string(to_string(nbTourRestant)),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
}

/**
 * @brief FaireUnTourMingl_Niveau3 : Fonction qui sert a faire un tour dans le niveau 3 et l'afficher avec MinGl
 * @param mat : Matrice utilisé dans le niveau
 * @param window : Fênetre ouverte par l'interface graphique MinGL
 * @param taille_grille : Taille de la grille de jeu en longueur et largeur
 * @param nbTourRestant : Nombre de tour Restant dans le niveau 3
 * @param gemmeRestante : Nombre de gemme restante a detruire dans le niveau
 * @param gemmeADetruire : Le type de gemme a detruire dans le niveau
 */
void FaireUnTourMingl_Niveau3(CMatrice & mat,MinGL & window,const unsigned taille_grille,unsigned & nbTourRestant,unsigned & gemmeRestante,unsigned & gemmeADetruire){
    afficheMatriceV0(mat);
    affiche_niveau3(mat,window,nbTourRestant,gemmeRestante,gemmeADetruire);
    CMyParam Param;
    string fichier="config.yaml";
    initParams(Param);
    chargerConfig(Param,fichier);
    vector<char> Paramfichier;
    Paramfichier.push_back(Param.mapParamChar["toucheHautGauche"]);
    Paramfichier.push_back(Param.mapParamChar["toucheHaut"]);
    Paramfichier.push_back(Param.mapParamChar["toucheHautDroite"]);
    Paramfichier.push_back(Param.mapParamChar["toucheGauche"]);
    Paramfichier.push_back(Param.mapParamChar["toucheDroite"]);
    Paramfichier.push_back(Param.mapParamChar["toucheBasGauche"]);
    Paramfichier.push_back(Param.mapParamChar["toucheBas"]);
    Paramfichier.push_back(Param.mapParamChar["toucheBasGauche"]);
    char deplacement;
    size_t NumLigne,NumCol;
    window << nsGui::Text(nsGraphics::Vec2D(400,850),std::string("Choisi la ligne du joyau que tu souhaites bouger (de 0 au nombre de ligne-1)"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window.finishFrame();
    while(true){
        bool quitte=false;
        for(unsigned k=0;k<taille_grille;k=k+1){
                string t = to_string(k);
                char char_a_tester=t[0];
                cout << char_a_tester << endl;
                cout << "yo" << endl;
                if(window.isPressed({char_a_tester,false})){
                cout << "bonjour" << endl;
                string texte="La ligne choisi est : ";
                texte.push_back(char_a_tester);
                affiche_niveau3(mat,window,nbTourRestant,gemmeRestante,gemmeADetruire);
                window << nsGui::Text(nsGraphics::Vec2D(400,900),std::string(texte),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
                using std::this_thread::sleep_for;
                sleep_for(std::chrono::milliseconds(1000));
                window.finishFrame();
                NumLigne=k;
                quitte=true;
                }
                using std::this_thread::sleep_for;
                sleep_for(std::chrono::milliseconds(200));
                window.finishFrame();
        }
        if(quitte==true){
                break;
        }
    }
    affiche_niveau3(mat,window,nbTourRestant,gemmeRestante,gemmeADetruire);
    window << nsGui::Text(nsGraphics::Vec2D(400,850),std::string("Choisi la colonne du joyau que tu souhaites bouger (de 0 au nombre de ligne-1)"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window.finishFrame();
    while(true){
        bool quitte2=false;
        for(unsigned k=0;k<taille_grille;k=k+1){
                string t = to_string(k);
                char char_a_tester=t[0];
                if(window.isPressed({char_a_tester,false})){
                string texte="La colonne choisi est : ";
                texte.push_back(char_a_tester);
                affiche_niveau3(mat,window,nbTourRestant,gemmeRestante,gemmeADetruire);
                window << nsGui::Text(nsGraphics::Vec2D(400,900),std::string(texte),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
                using std::this_thread::sleep_for;
                sleep_for(std::chrono::milliseconds(1000));
                window.finishFrame();
                NumCol=k;
                quitte2=true;
                }
                using std::this_thread::sleep_for;
                sleep_for(std::chrono::milliseconds(200));
                window.finishFrame();
        }
        if(quitte2==true){
                break;
        }
    }
    string texte_direction="Choisi la direction dans laquelle tu veux que le joyau bouge entre : ";
    texte_direction.push_back(Param.mapParamChar["toucheHautGauche"]);
    texte_direction.push_back(' ');
    texte_direction.push_back('|');
    texte_direction.push_back(' ');
    texte_direction.push_back(Param.mapParamChar["toucheHaut"]);
    texte_direction.push_back(' ');
    texte_direction.push_back('|');
    texte_direction.push_back(' ');
    texte_direction.push_back(Param.mapParamChar["toucheHautDroite"]);
    texte_direction.push_back(' ');
    texte_direction.push_back('|');
    texte_direction.push_back(' ');
    texte_direction.push_back(Param.mapParamChar["toucheGauche"]);
    texte_direction.push_back(' ');
    texte_direction.push_back('|');
    texte_direction.push_back(' ');
    texte_direction.push_back(Param.mapParamChar["toucheDroite"]);
    texte_direction.push_back(' ');
    texte_direction.push_back('|');
    texte_direction.push_back(' ');
    texte_direction.push_back(Param.mapParamChar["toucheBasGauche"]);
    texte_direction.push_back(' ');
    texte_direction.push_back('|');
    texte_direction.push_back(' ');
    texte_direction.push_back(Param.mapParamChar["toucheBas"]);
    texte_direction.push_back(' ');
    texte_direction.push_back('|');
    texte_direction.push_back(' ');
    texte_direction.push_back(Param.mapParamChar["toucheBasDroite"]);
    texte_direction.push_back('.');
    affiche_niveau3(mat,window,nbTourRestant,gemmeRestante,gemmeADetruire);
    window << nsGui::Text(nsGraphics::Vec2D(400,850),std::string(texte_direction),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window.finishFrame();
    while(true){
        bool quitte3=false;
        for(unsigned k=0;k<Paramfichier.size();k=k+1){
                cout << Paramfichier[k] << endl;
                if(window.isPressed({Paramfichier[k],false})){
                deplacement=Paramfichier[k];
                quitte3=true;
                }
                window.finishFrame();
                using std::this_thread::sleep_for;
                sleep_for(std::chrono::milliseconds(200));
        }
        if(quitte3==true){
                break;
        }
    }
    faitUnMouvement(mat, deplacement, NumLigne, NumCol,Param);
    while(true){
        affiche_niveau3(mat,window,nbTourRestant,gemmeRestante,gemmeADetruire);
        window.finishFrame();
        using std::this_thread::sleep_for;
        sleep_for(std::chrono::milliseconds(200));
        if(detectionExplositionUneBombeVerticalGemme(mat, gemmeRestante, gemmeADetruire)==false && detectionExplositionUneBombeHorizontaleGemme(mat, gemmeRestante, gemmeADetruire)==false) {
                break;
        }

    }
}

/**
 * @brief Niveau_3_Mingl : Fonction qui sert a afficher et éxecuter le niveau 3 avec MinGL
 * @param window : Fênetre ouverte par l'interface graphique MinGL
 */
void Niveau_3_Mingl(MinGL & window){
    CMatrice mat;
    initMat(mat,7,7);
    unsigned gemmeRestante = 0, gemmeADetruire = (rand() % (4-1+1))+1;
    while (detectionExplositionUneBombeVerticalGemme(mat, gemmeRestante, gemmeADetruire)==true
           || detectionExplositionUneBombeHorizontaleGemme(mat, gemmeRestante, gemmeADetruire)==true){}
    cout << endl <<  "Initialisation complete." << endl << gemmeRestante << " gemmes restantes avant commencer." << endl;
    gemmeRestante = 50;
    cout << gemmeRestante << " pour le début de la partie. C'est partie !!!" << endl << endl;
    cout << "Bienvenue dans le mode de jeu Gemme" << endl << endl << "La gemme a détruire est : " << gemmeADetruire << endl;
    unsigned nbTour=0,nbTourMax=20;
    while(true){
        unsigned nbTourRestant=nbTourMax-nbTour;
        FaireUnTourMingl_Niveau3(mat,window,mat[0].size(),nbTourRestant,gemmeRestante, gemmeADetruire);
        cout << "tu es au tour :" << nbTour << endl;
        cout << "il ne te reste plus que "  << gemmeRestante << " gemme numéro " << gemmeADetruire << " à détruire !" << endl;
        nbTour=nbTour+1;
        if(gemmeRestante <= 0){
            affiche_niveau3(mat,window,nbTourRestant,gemmeRestante,gemmeADetruire);
            window << nsGui::Text(nsGraphics::Vec2D(280,850),std::string("Bien joué tu as gagné ! hésite pas a tester un autre niveau !"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);                                                                                                                                   window.finishFrame();
            using std::this_thread::sleep_for;
            sleep_for(std::chrono::milliseconds(8000));
            break;
        }
        else if(nbTour == nbTourMax){
            affiche_niveau3(mat,window,nbTourRestant,gemmeRestante,gemmeADetruire);
            window << nsGui::Text(nsGraphics::Vec2D(280,850),std::string("Désolé tu as perdu :( , Tu peux réessayer !"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
            window.finishFrame();
            using std::this_thread::sleep_for;
            sleep_for(std::chrono::milliseconds(8000));
            break;
        }

    }
}

/**
 * @brief FaireUnTourMingl : Fonction qui sert a faire un tour dans un niveau afficher avec MinGL
 * @param mat : Matrice utilisé dans le niveau
 * @param score : Score actuel du joueur dans le niveau
 * @param window : Fênetre ouverte par l'interface graphique MinGL
 * @param taille_grille : Taille de la grille de jeu en longueur et largeur
 * @param nbTourRestant : Nombre de tour Restant dans le niveau 3
 * @param objectif : objectif a atteindre par le joueur dans ce niveau
 * @param niveau : Le niveau qui est en cours
 */
void FaireUnTourMingl(CMatrice & mat,unsigned & score,MinGL & window,const unsigned taille_grille,const unsigned & nbTourRestant,const unsigned & objectif,const unsigned & niveau){
    afficheMatriceV0(mat);
    if(niveau==1){
        affiche_niveau1(mat,window,score,nbTourRestant,objectif);
    }
    if(niveau==2){
        affiche_niveau2(mat,window,nbTourRestant);
    }
    CMyParam Param;
    string fichier="config.yaml";
    initParams(Param);
    chargerConfig(Param,fichier);
    vector<char> Paramfichier;
    Paramfichier.push_back(Param.mapParamChar["toucheHautGauche"]);
    Paramfichier.push_back(Param.mapParamChar["toucheHaut"]);
    Paramfichier.push_back(Param.mapParamChar["toucheHautDroite"]);
    Paramfichier.push_back(Param.mapParamChar["toucheGauche"]);
    Paramfichier.push_back(Param.mapParamChar["toucheDroite"]);
    Paramfichier.push_back(Param.mapParamChar["toucheBasGauche"]);
    Paramfichier.push_back(Param.mapParamChar["toucheBas"]);
    Paramfichier.push_back(Param.mapParamChar["toucheBasGauche"]);
    char deplacement;
    size_t NumLigne,NumCol;
    window << nsGui::Text(nsGraphics::Vec2D(400,850),std::string("Choisi la ligne du joyau que tu souhaites bouger (de 0 au nombre de ligne-1)"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window.finishFrame();
    while(true){
        bool quitte=false;
        for(unsigned k=0;k<taille_grille;k=k+1){
            string t = to_string(k);
            char char_a_tester=t[0];
            cout << char_a_tester << endl;
            if(window.isPressed({char_a_tester,false})){
                    string texte="La ligne choisi est : ";
                    texte.push_back(char_a_tester);
                    if(niveau==1){
                    affiche_niveau1(mat,window,score,nbTourRestant,objectif);
                    }
                    if(niveau==2){
                    affiche_niveau2(mat,window,nbTourRestant);
                    }
                    window << nsGui::Text(nsGraphics::Vec2D(400,900),std::string(texte),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
                    using std::this_thread::sleep_for;
                    sleep_for(std::chrono::milliseconds(1000));
                    window.finishFrame();
                    NumLigne=k;
                    quitte=true;
            }
            using std::this_thread::sleep_for;
            sleep_for(std::chrono::milliseconds(200));
            window.finishFrame();
       }
        if(quitte==true){
            break;
        }
    }
    if(niveau==1){
        affiche_niveau1(mat,window,score,nbTourRestant,objectif);
    }
    if(niveau==2){
        affiche_niveau2(mat,window,nbTourRestant);
    }
    window << nsGui::Text(nsGraphics::Vec2D(400,850),std::string("Choisi la colonne du joyau que tu souhaites bouger (de 0 au nombre de ligne-1)"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window.finishFrame();
    while(true){
        bool quitte2=false;
        for(unsigned k=0;k<taille_grille;k=k+1){
            string t = to_string(k);
            char char_a_tester=t[0];
            if(window.isPressed({char_a_tester,false})){
                    string texte="La colonne choisi est : ";
                    texte.push_back(char_a_tester);
                    if(niveau==1){
                    affiche_niveau1(mat,window,score,nbTourRestant,objectif);
                    }
                    if(niveau==2){
                    affiche_niveau2(mat,window,nbTourRestant);
                    }
                    window << nsGui::Text(nsGraphics::Vec2D(400,900),std::string(texte),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
                    using std::this_thread::sleep_for;
                    sleep_for(std::chrono::milliseconds(1000));
                    window.finishFrame();
                    NumCol=k;
                    quitte2=true;
            }
            using std::this_thread::sleep_for;
            sleep_for(std::chrono::milliseconds(200));
            window.finishFrame();
        }
        if(quitte2==true){
            break;
        }
    }
    string texte_direction="Choisi la direction dans laquelle tu veux que le joyau bouge entre : ";
    texte_direction.push_back(Param.mapParamChar["toucheHautGauche"]);
    texte_direction.push_back(' ');
    texte_direction.push_back('|');
    texte_direction.push_back(' ');
    texte_direction.push_back(Param.mapParamChar["toucheHaut"]);
    texte_direction.push_back(' ');
    texte_direction.push_back('|');
    texte_direction.push_back(' ');
    texte_direction.push_back(Param.mapParamChar["toucheHautDroite"]);
    texte_direction.push_back(' ');
    texte_direction.push_back('|');
    texte_direction.push_back(' ');
    texte_direction.push_back(Param.mapParamChar["toucheGauche"]);
    texte_direction.push_back(' ');
    texte_direction.push_back('|');
    texte_direction.push_back(' ');
    texte_direction.push_back(Param.mapParamChar["toucheDroite"]);
    texte_direction.push_back(' ');
    texte_direction.push_back('|');
    texte_direction.push_back(' ');
    texte_direction.push_back(Param.mapParamChar["toucheBasGauche"]);
    texte_direction.push_back(' ');
    texte_direction.push_back('|');
    texte_direction.push_back(' ');
    texte_direction.push_back(Param.mapParamChar["toucheBas"]);
    texte_direction.push_back(' ');
    texte_direction.push_back('|');
    texte_direction.push_back(' ');
    texte_direction.push_back(Param.mapParamChar["toucheBasDroite"]);
    texte_direction.push_back('.');
    if(niveau==1){
        affiche_niveau1(mat,window,score,nbTourRestant,objectif);
    }
    if(niveau==2){
        affiche_niveau2(mat,window,nbTourRestant);
    }
    window << nsGui::Text(nsGraphics::Vec2D(400,850),std::string(texte_direction),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window.finishFrame();
    while(true){
        bool quitte3=false;
        for(unsigned k=0;k<Paramfichier.size();k=k+1){
            cout << Paramfichier[k] << endl;
            if(window.isPressed({Paramfichier[k],false})){
                    deplacement=Paramfichier[k];
                    quitte3=true;
            }
            window.finishFrame();
            using std::this_thread::sleep_for;
            sleep_for(std::chrono::milliseconds(200));
        }
        if(quitte3==true){
            break;
        }
    }
    faitUnMouvement(mat, deplacement, NumLigne, NumCol,Param);
    while(true){
        if(niveau==1){
            affiche_niveau1(mat,window,score,nbTourRestant,objectif);
        }
        if(niveau==2){
            affiche_niveau2(mat,window,nbTourRestant);
        }
        window.finishFrame();
        using std::this_thread::sleep_for;
        sleep_for(std::chrono::milliseconds(200));
        if(detectionExplositionUneBombeVertical(mat,score)==false && detectionExplositionUneBombeHorizontale(mat,score)==false) {
            break;
        }

    }
}

/**
 * @brief Niveau_1_mingl : Fonction qui sert a afficher et éxecuter le niveau 1 avec MinGl
 * @param window : Fênetre ouverte par l'interface graphique MinGL
 */
void Niveau_1_mingl(MinGL & window){
    CMatrice mat;
    initMat(mat,5,5);
    unsigned score=0;
    while (detectionExplositionUneBombeVertical(mat,score)==true && detectionExplositionUneBombeHorizontale(mat,score)==true) {
    }
    unsigned objectif=1500,nbTour=0,nbTourMax=10;
    score=0;
    while(true){
        unsigned nbTourRestant=nbTourMax-nbTour;
        FaireUnTourMingl(mat,score,window,mat.size(),nbTourRestant,objectif,1);
        cout << "tu es au tour : " << nbTour << endl;
        cout << "tu as "  << score << " points !" << endl;
        nbTour=nbTour+1;
        if(nbTour == nbTourMax){
            affiche_niveau1(mat,window,score,nbTourRestant,objectif);
            window << nsGui::Text(nsGraphics::Vec2D(280,850),std::string("Désolé tu as perdu :( , Tu peux réessayer !"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
            window.finishFrame();
            using std::this_thread::sleep_for;
            sleep_for(std::chrono::milliseconds(8000));
            break;
        }
        if(score>=objectif){
            affiche_niveau1(mat,window,score,nbTourRestant,objectif);
            window << nsGui::Text(nsGraphics::Vec2D(280,850),std::string("Bien joué tu as gagné ! hésite pas a tester un autre niveau !"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);                                                                                                                                   window.finishFrame();
            using std::this_thread::sleep_for;
            sleep_for(std::chrono::milliseconds(8000));
            break;
        }
    }
}
/**
 * @brief Niveau_2_Mingl : Fonction qui sert a afficher et éxecuter le niveau 2 avec MinGl
 * @param window : Fênetre ouverte par l'interface graphique MinGL
 */
void Niveau_2_Mingl(MinGL & window){
    CMatrice mat;
    initMat(mat,8,8);
    unsigned score=0;
    while (detectionExplositionUneBombeVertical(mat,score)==true && detectionExplositionUneBombeHorizontale(mat,score)==true) {
    }
    unsigned objectif=1500,nbTour=0,nbTourMax=10;
    score=0;
    bool victoire;
    mat[0][4]=5;
    while(true){
        unsigned nbTourRestant=nbTourMax-nbTour;
        FaireUnTourMingl(mat,score,window,mat.size(),nbTourRestant,objectif,2);
        cout << "tu es au tour :" << nbTour << endl;
        cout << "tu as "  << score << " points !" << endl;
        nbTour=nbTour+1;
        if(nbTour == nbTourMax){
            affiche_niveau2(mat,window,nbTourRestant);
            window << nsGui::Text(nsGraphics::Vec2D(280,850),std::string("Désolé tu as perdu :( , Tu peux réessayer !"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
            window.finishFrame();
            using std::this_thread::sleep_for;
            sleep_for(std::chrono::milliseconds(8000));
            break;
        }
        for (unsigned k=0;k<mat[0].size()-1;k=k+1){
            if(mat[mat.size()-1][k]==5){
                victoire=true;
                affiche_niveau1(mat,window,score,nbTourRestant,objectif);
                window << nsGui::Text(nsGraphics::Vec2D(280,850),std::string("Bien joué tu as gagné ! hésite pas a tester un autre niveau !"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);                                                                                                                                   window.finishFrame();
                using std::this_thread::sleep_for;
                sleep_for(std::chrono::milliseconds(8000));
                break;
            }
        }
        if (victoire==true){
            break;
        }
    }
}

/**
 * @brief VraiEcranAccueil : Fonction qui sert a afficher l'écran d'accueil savec MinGl
 * @param window : Fênetre ouverte par l'interface graphique MinGL
 */
void VraiEcranAccueil(MinGL & window){
    window << nsGui::Sprite("../Jewel_Crush_SAE/fond-ecran-jolie",nsGraphics::Vec2D(0,0));
    window << nsGui::Text(nsGraphics::Vec2D(30,30),std::string("Pour utiliser les touches du clavier il faut laisser la touche enfoncer un certain temps !"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsShape::Rectangle(nsGraphics::Vec2D(530,230),nsGraphics::Vec2D(1400,550),nsGraphics::KMagenta);
    window << nsGui::Text(nsGraphics::Vec2D(870,150),std::string("Jewel Crush"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(570,350),std::string("Jeu de type 'Candy Crush' devellope par Evan, Mariam, Badiss, Noah et Edson"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(650,450),std::string("3 niveaux disponibles pour l'instant auquel vous acces ci-dessous : "),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsShape::Rectangle(nsGraphics::Vec2D(10,700),nsGraphics::Vec2D(630,1000),nsGraphics::KMagenta);
    window << nsShape::Rectangle(nsGraphics::Vec2D(650,700),nsGraphics::Vec2D(1270,1000),nsGraphics::KMagenta);
    window << nsShape::Rectangle(nsGraphics::Vec2D(1290,700),nsGraphics::Vec2D(1910,1000),nsGraphics::KMagenta);
    window << nsGui::Text(nsGraphics::Vec2D(280,850),std::string("Niveau 1"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(60,950),std::string("Rester appuye sur la touche 1 pour lancer le niveau 1"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(920,850),std::string("Niveau 2"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(700,950),std::string("Rester appuye sur la touche 2 pour lancer le niveau 2"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(1570,850),std::string("Niveau 3"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(1350,950),std::string("Rester appuye sur la touche 3 pour lancer le niveau 3"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window.finishFrame();
}

int main() {
    MinGL Jewel_Crush("Jewel_Crush", nsGraphics::Vec2D(1920,1080), nsGraphics::Vec2D(1920, 1080), nsGraphics::KBlack);
    Jewel_Crush.initGlut();
    Jewel_Crush.initGraphic();
    while (Jewel_Crush.isOpen())
    {
        VraiEcranAccueil(Jewel_Crush);
        unsigned niveau_choisi = 0;
        if(Jewel_Crush.isPressed({'&',false}) || Jewel_Crush.isPressed({'1',false}) ){
            niveau_choisi=1;
        }
        if(Jewel_Crush.isPressed({'2',false}) ){
            niveau_choisi=2;
        }
        if(Jewel_Crush.isPressed({'3',false}) || Jewel_Crush.isPressed({'"',false})){
            niveau_choisi=3;
        }
        if(Jewel_Crush.isPressed({'a',false}) || Jewel_Crush.isPressed({'b',false}) ){
            cout << "coucou" << endl;
        }
        if(niveau_choisi==1){
                Niveau_1_mingl(Jewel_Crush);
        }
        if(niveau_choisi==2){
                Niveau_2_Mingl(Jewel_Crush);
        }
        if(niveau_choisi==3){
                Niveau_3_Mingl(Jewel_Crush);
        }

        Jewel_Crush.finishFrame();

        // On vide la queue d'évènements
        Jewel_Crush.getEventManager().clearEvents();
    }
}
