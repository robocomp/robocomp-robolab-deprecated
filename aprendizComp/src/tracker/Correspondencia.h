#ifndef CORRESPONDENCIA_H
#define CORRESPONDENCIA_H
#include <tracker/Config.h>


void Correspondencia(TRACKER *track, int sec);
void HermanamientoNivelEstereo(PIR ***pirright, BOX **boxnivel, int *tam, int nivel, int *clases);
void HermanamientoInversoNivelEstereo(PIR ***pirright, BOX **boxnivel, int *tam, int nivel,int *clases);
void Disparidad (PIR ***pirder, BOX *boxizq, int *clases, float *disparidad);
void Refinar(TRACKER *track);


#endif

