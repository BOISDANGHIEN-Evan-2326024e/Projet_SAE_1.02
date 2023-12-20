#include "explosion.h"

{
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
