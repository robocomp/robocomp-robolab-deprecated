#ifndef FUNCPIR_H
#define FUNCPIR_H

#include <tracker/Config.h>

void BoundingBox(PIR ***pir, int tam, BOX *box);
void InicPiramide (PIR ***pir, int numniveles, int *tam_i, int *tam_j, unsigned char **im );
void Clasificar(PIR *** pir,int num, int *tam_i, int *tam_j, int *NodosVirtuales, int ContNV, int reclasificar);
int Hermanamiento(PIR ***pir, int num, int *tam_i, int *tam_j);
int Fusion(PIR ***pir, int niveles,int *tam_i, int *tam_j);
void ExtraerClases(PIR ***pir, int tam_i, int tam_j,int numClases, BOX *boxes, int *clases);

#endif


