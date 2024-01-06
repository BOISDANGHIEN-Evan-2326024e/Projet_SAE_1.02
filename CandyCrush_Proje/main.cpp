#include "mingl/gui/text.h"
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

using namespace std;

struct CMyParam {
    std::map <std::string, char> mapParamChar;
    std::map <std::string, unsigned> mapParamUnsigned;
};


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

void afficheMatrice_Interface_G(const CMatrice & Mat,MinGL & window){
    unsigned coord_colonne=200;
    for(unsigned k=0;k<Mat.size();k=k+1){
        unsigned coord_ligne=250;
        for(unsigned i=0;i<Mat[0].size();i=i+1){
            if(Mat[k][i]==1){
                window << nsGui::Sprite("/home/evan/Bureau/Projet candy crush/CandyCrush_Proje/gemme-1-reussi",nsGraphics::Vec2D(coord_ligne,coord_colonne));
            }
            if(Mat[k][i]==2){
                window << nsGui::Sprite("/home/evan/Bureau/Projet candy crush/CandyCrush_Proje/gemme-2-reussi",nsGraphics::Vec2D(coord_ligne,coord_colonne));
            }
            if(Mat[k][i]==3){
                window << nsGui::Sprite("/home/evan/Bureau/Projet candy crush/CandyCrush_Proje/gemme-3-reussi",nsGraphics::Vec2D(coord_ligne,coord_colonne));
            }
            if(Mat[k][i]==4){
                window << nsGui::Sprite("/home/evan/Bureau/Projet candy crush/CandyCrush_Proje/gemme-4-reussi",nsGraphics::Vec2D(coord_ligne,coord_colonne));
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
            coord_ligne=coord_ligne+80;
        }
        coord_colonne=coord_colonne+80;
    }
}

void affiche_niveau1(const CMatrice & Mat,MinGL & window,const unsigned & score,const unsigned & nbTourRestant,const unsigned & objectif){
    window << nsGui::Sprite("/home/evan/Bureau/Projet candy crush/CandyCrush_Proje/fond-ecran-jolie",nsGraphics::Vec2D(0,0));
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

void explositionUneBombeVertical (CMatrice & mat, const size_t & numLigne,const size_t & numColonne, const size_t & combien,const unsigned & nbMax=KPlusGrandNombreDansLaMatrice){
    for(unsigned k=numLigne-1;k>combien-1;k=k-1){
        mat[k][numColonne]=mat[k-3][numColonne];
    }
    for(unsigned i=0;i<combien;i=i+1){
        mat[i][numColonne]=1+rand()%(nbMax - 1 + 1);
    }
}

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
    if(ModeDeJeu=="Bonbon") {
        //A_Faire
    }
}

void FaireUnTourMingl(CMatrice & mat,unsigned & score,MinGL & window,unsigned taille_grille){
    afficheMatriceV0(mat);
    afficheMatrice_Interface_G(mat,window);
    window.finishFrame();
    this_thread::sleep_for(chrono::milliseconds(10000 / FPS_LIMIT));
    CMyParam Param;
    string fichier="config.yaml";
    initParams(Param);
    chargerConfig(Param,fichier);
    while(true){
        window << nsGui::Text(nsGraphics::Vec2D(400,850),std::string("Choisi la Ligne du joyau que tu souhaites bouger (de 0 au nombre de ligne-1)"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
        for(unsigned k=0;k<taille_grille;k=k+1){
            char char_a_tester=k;
            if(window.isPressed({char_a_tester,false})){
                    string texte="La ligne choisi est : ";
                    texte.push_back(char_a_tester);
                    window << nsGui::Text(nsGraphics::Vec2D(400,900),std::string(texte),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
                    window.finishFrame();
                    break;
            }
        }
        window.finishFrame();
    }
    /*char toucheHautGauche=Param.mapParamChar["toucheHautGauche"],toucheHaut=Param.mapParamChar["toucheHaut"],toucheHautDroite=Param.mapParamChar["toucheHautDroite"];

    string text="Sens du deplacement : ("+ Param.mapParamChar["toucheHautGauche"]+ "|" +Param.mapParamChar["toucheHaut"] +"|" + Param.mapParamChar["toucheHautDroite"]
                  + "|"+  Param.mapParamChar["toucheGauche"] + "|"+ Param.mapParamChar["toucheDroite"] + "|" + Param.mapParamChar["toucheBasDroite"] + "|" + Param.mapParamChar["toucheBas"] + "|" + Param.mapParamChar["toucheBasDroite"] + ")";
    cout << text << endl;
    cout << Param.mapParamChar["toucheHautGauche"] << Param.mapParamChar["toucheHautGauche"] << endl;
    window << nsGui::Text(nsGraphics::Vec2D(400,850),std::string(text),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window.finishFrame();*/
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
    faitUnMouvement(mat, deplacement, numLigne, numCol,Param);
    while(true){
        afficheMatrice_Interface_G(mat,window);
        window.finishFrame();
        this_thread::sleep_for(chrono::milliseconds(10000 / FPS_LIMIT));
        if(detectionExplositionUneBombeVertical(mat,score)==false && detectionExplositionUneBombeHorizontale(mat,score)==false) {
            break;
        }

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

void EcranAccueil(MinGL & window,CMatrice mat){
    window << nsGui::Sprite("/home/evan/Bureau/Projet candy crush/CandyCrush_Proje/fond-ecran-jolie",nsGraphics::Vec2D(0,0));
    afficheMatrice_Interface_G(mat,window);
    this_thread::sleep_for(chrono::milliseconds(5000 / FPS_LIMIT));
    window.finishFrame();
}

void VraiEcranAccueil(MinGL & window){
    window << nsGui::Sprite("/home/evan/Bureau/Projet candy crush/CandyCrush_Proje/fond-ecran-jolie",nsGraphics::Vec2D(0,0));
    window << nsShape::Rectangle(nsGraphics::Vec2D(530,230),nsGraphics::Vec2D(1400,550),nsGraphics::KMagenta);
    window << nsGui::Text(nsGraphics::Vec2D(870,150),std::string("Jewel Crush"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(570,350),std::string("Jeu de type 'Candy Crush' devellope par Evan, Mariam, Badiss, Noah et Edson"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(650,450),std::string("3 niveaux disponibles pour l'instant auquel vous acces ci-dessous : "),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsShape::Rectangle(nsGraphics::Vec2D(10,700),nsGraphics::Vec2D(630,1000),nsGraphics::KMagenta);
    window << nsShape::Rectangle(nsGraphics::Vec2D(650,700),nsGraphics::Vec2D(1270,1000),nsGraphics::KMagenta);
    window << nsShape::Rectangle(nsGraphics::Vec2D(1290,700),nsGraphics::Vec2D(1910,1000),nsGraphics::KMagenta);
    window << nsGui::Text(nsGraphics::Vec2D(280,850),std::string("Niveau 1"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(920,850),std::string("Niveau 2"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window << nsGui::Text(nsGraphics::Vec2D(1570,850),std::string("Niveau 3"),nsGraphics::KBlue,nsGui::GlutFont::GlutFonts::BITMAP_TIMES_ROMAN_24);
    window.finishFrame();
    cout << "coucouaaa" << endl;
}

void Niveau_1_mingl(MinGL & window,CMatrice & mat){
    unsigned score=0;
    while (detectionExplositionUneBombeVertical(mat,score)==true && detectionExplositionUneBombeHorizontale(mat,score)==true) {
    }
    unsigned objectif=1500,nbTour=0,nbTourMax=10;
    score=0;
    while(true){
        unsigned nbTourRestant=nbTourMax-nbTour;
        affiche_niveau1(mat,window,score,nbTourRestant,objectif);
        window.finishFrame();
        FaireUnTourMingl(mat,score,window,mat.size());
        cout << "tu es au tour : " << nbTour << endl;
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

void FaireUnclick(MinGL & window,CMatrice mat){
    if(nsEvent::MouseClick){
        cout << "bonjour" << endl;
    }
}

int main() {
    MinGL Candy_Crush("Candy_Crush", nsGraphics::Vec2D(1920,1080), nsGraphics::Vec2D(1920, 1080), nsGraphics::KBlack);
    Candy_Crush.initGlut();
    Candy_Crush.initGraphic();
    CMatrice mat;
    initMat(mat,5,5);
    //EcranAccueil(Candy_Crush,mat);
    nsEvent::MouseMoveData_t souris_info;
    souris_info.x=nsEvent::MouseClick;
    while (Candy_Crush.isOpen())
    {
        VraiEcranAccueil(Candy_Crush);
        unsigned niveau_choisi = 0;
        if(Candy_Crush.isPressed({'&',false}) || Candy_Crush.isPressed({'1',false}) ){
            niveau_choisi=1;
        }
        if(Candy_Crush.isPressed({'a',false}) || Candy_Crush.isPressed({'b',false}) ){
            cout << "coucou" << endl;
        }
        if(niveau_choisi==1){
                Niveau_1_mingl(Candy_Crush,mat);
        }
        cout << "yo" << endl;
        // Récupère l'heure actuelle
        //Niveau_1V2_mingl(Candy_Crush,mat);
        chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();
        //FaireUnclick(Candy_Crush,mat);

        // On efface la fenêtre
        // Candy_Crush.clearScreen();

        // On dessine les formes géométriques
        //EcranAccueil(Candy_Crush,mat);
        //Niveau_1_mingl(Candy_Crush,mat);
        // On finit la frame en cours
        Candy_Crush.finishFrame();

        // On vide la queue d'évènements
        Candy_Crush.getEventManager().clearEvents();

        // On attend un peu pour limiter le framerate et soulager le CPU
        //this_thread::sleep_for(chrono::milliseconds(1000 / FPS_LIMIT) - chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start));

        // On récupère le temps de frame
        //frameTime = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start);

    }
    //EcranAccueil(Candy_Crush);

    //Niveau_1V2();

}
