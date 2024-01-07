#include "explosion.h"

{
    for(unsigned k=numLigne;k>0;k=k-1){
        for(unsigned i=numColonne;i<combien+numColonne;i=i+1){
            mat[k][i]=mat[k-1][i];
        }
    }
    for(unsigned i=numColonne;i<combien+numColonne;i=i+1){
        mat[0][i]=1+rand()%(nbMax - 1 + 1);
    }
}

{
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

{
    for(unsigned k=numLigne-1;k>combien-1;k=k-1){
        mat[k][numColonne]=mat[k-3][numColonne];
    }
    for(unsigned i=0;i<combien;i=i+1){
        mat[i][numColonne]=1+rand()%(nbMax - 1 + 1);
    }
}

{
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

