#include "mingl.h"

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
