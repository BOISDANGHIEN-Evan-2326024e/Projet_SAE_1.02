#include <main.cpp>

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
