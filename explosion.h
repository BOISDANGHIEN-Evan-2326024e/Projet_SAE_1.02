#ifndef EXPLOSION_H
#define EXPLOSION_H
#include<main.cpp>

/**
 * @brief explositionUneBombeHorizontale
 * @param mat
 * @param numLigne
 * @param numColonne
 * @param combien
 * @param nbMax
 */
void explositionUneBombeHorizontale (CMatrice & mat, const size_t & numLigne,const size_t & numColonne, const size_t & combien,const unsigned & nbMax=KPlusGrandNombreDansLaMatrice);

/**
 * @brief detectionExplositionUneBombeHorizontale
 * @param mat
 * @param score
 * @return
 */
bool detectionExplositionUneBombeHorizontale (CMatrice & mat,unsigned & score);

/**
 * @brief explositionUneBombeVertical
 * @param mat
 * @param numLigne
 * @param numColonne
 * @param combien
 * @param nbMax
 */
void explositionUneBombeVertical (CMatrice & mat, const size_t & numLigne,const size_t & numColonne, const size_t & combien,const unsigned & nbMax=KPlusGrandNombreDansLaMatrice);

/**
 * @brief detectionExplositionUneBombeVertical
 * @param mat
 * @param score
 * @return
 */
bool detectionExplositionUneBombeVertical (CMatrice & mat,unsigned & score);



#endif // EXPLOSION_H
