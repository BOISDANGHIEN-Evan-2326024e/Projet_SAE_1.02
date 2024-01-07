#ifndef EXPLOSIONGEMME_H
#define EXPLOSIONGEMME_H
#include<main.cpp>

/**
 * @brief detectionExplositionUneBombeHorizontaleGemme
 * @param mat
 * @param gemmeRestante
 * @param gemmeADetruire
 * @return
 */
bool detectionExplositionUneBombeHorizontaleGemme (CMatrice & mat, unsigned & gemmeRestante, const unsigned & gemmeADetruire);

/**
 * @brief detectionExplositionUneBombeVerticalGemme
 * @param mat
 * @param gemmeRestante
 * @param gemmeADetruire
 * @return
 */
bool detectionExplositionUneBombeVerticalGemme (CMatrice & mat,unsigned & gemmeRestante, const unsigned gemmeADetruire);


#endif // EXPLOSIONGEMME_H
