#ifndef IORAW_H
#define IORAW_H

#include <tracker/Config.h>

enum formato { raw=0, jpeg };

void LeerImagenFichero(IMAGEN *im, int cont);
void GuardarResultadoTracking(TRACKER *track, int cont);
void GuardarImagenEntrada(TRACKER *track, int cont);
void GuardarImagenColorPiel(TRACKER *track, int cont, formato fmt=raw);
void GuardarImagenDisparidad(TRACKER *track, int cont, formato fmt=raw);
void GuardarSilueta(TRACKER *track, int cont, formato fmt=raw);
void GuardarResultadoEstereo(TRACKER *track, int cont);
void GuardarMapaDisparidad(TRACKER *track, int cont);
void GuardarImagenColor(unsigned char **imagen, int alto, int ancho, int cont);
#endif


