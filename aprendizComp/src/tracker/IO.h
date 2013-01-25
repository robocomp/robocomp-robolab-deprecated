#ifndef IOBUILDER_H
#define IOBUILDER_H

#include <tracker/Config.h>


void LeerImagenFicheroI(TImage *Image, int cont,IMAGEN *im);
void LeerImagenFicheroD(TImage *Image, int cont,IMAGEN *im);
void PintarImagen(PIR ***pir, TImage *Image);
void DibujarCajas(BOX *cajas, int *clases, TImage *Image, PIR ***pir);
void PintarZonaBusqueda(BOX **boxnivel, int **rango, TImage *Image);
void DibujarClasesEstereo(TRACKER *track,TImage *Image1,int sec, TImage *Image);
void PintarResultadoTracking(TRACKER *track, TImage *Image, int sec);
void GuardarNivelDerechoFichero(TRACKER *track,int nivel);
void GuardarResultadoTracking(TRACKER *track);

#endif


