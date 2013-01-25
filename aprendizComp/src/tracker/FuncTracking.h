#ifndef FUNCTRACKING_H
#define FUNCTRACKING_H

#include <tracker/Config.h>


void PrimerFrame(TRACKER *track, bool primera_vez);
int BoundingBoxNiveles1(TRACKER *track, int a);
void InicializarMaskAnt(TRACKER *track, int a);
void InicializarMaskSig(MASCARA ****mask_sig, MASCARA ****mask_ant, BOX **boxnivel);
void ActualizarMaskAnt(MASCARA ****mask_ant, MASCARA ****mask_sig);
void Tracking(TRACKER *track, int sec);
void HermanamientoInversoNivel(PIR ****pir,int a, BOX **boxnivel, int *tam_i,int *tam_j, MASCARA **** mask_sig,int nivel, int *clases, int marc);
void HermanamientoNivel(PIR ****pir, int a,BOX **boxnivel, int *tam_i, int *tam_j, MASCARA **** mask_sig,int nivel, int *clases, int marc);

int RealizarTracking(TRACKER *track, int *run);

#endif

