#include "explosiongemme.h"

{
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


{
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
