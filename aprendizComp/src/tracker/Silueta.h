#ifndef SILUETA_H
#define SILUETA_H

#include <tracker/Config.h>

#define UMBRAL_ESQUELETO 25

unsigned char DisparidadMedia(TRACKER *track, int imin, int imax, int jmin, int jmax);
void UmbralizarDisparidad(IMAGEN *in, unsigned char **imagen_out, int umbral_adelante, int umbral_atras, unsigned char disparidad_media);
void RefinarSilueta(TRACKER *track);

void ObtenerSilueta(TRACKER *track, int umbral_palante, int umbral_patras, int imin, int imax, int jmin, int jmax, bool solo_conectadas = true);

/*Esqueleto: Calcula el esqueleto de una imagen .raw en blanco y negro
con un objeto negro sobre fondo blanco. Hay que pasarle la constante
de esqueletizacion. Devuelve el tiempo empleado en el algoritmo*/

double Esqueleto(unsigned char **im,unsigned char **mapa3, int TAM, float Tskel);
void Dilatacion(unsigned char **im,unsigned char **aux, int TAM,bool claro);
void Erosion(unsigned char **im,unsigned char **aux, int TAM,bool claro);

#endif
