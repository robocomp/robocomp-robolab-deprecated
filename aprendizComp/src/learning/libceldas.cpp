#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libceldas.h"

// Constructor 
manejadorceldas::manejadorceldas()
{
  // Valores por defecto para el espacio

  espacio.delta_x = 7;   // 14
  espacio.delta_y = 13;  // 26 
  espacio.delta_z = 7;   // 14
  
  espacio.xmax	= 200;
  espacio.ymax	= 200;
  espacio.zmax	= 200;
}

void manejadorceldas::cambiarDeltas(int x, int y, int z)
{
  espacio.delta_x = x;
  espacio.delta_y = y;
  espacio.delta_z = z;
}

void manejadorceldas::cambiarEspacio(int x, int y, int z)
{
  espacio.xmax = x;
  espacio.ymax = y;
  espacio.zmax = z;
}


CELDA manejadorceldas::calculaCelda(COORDENADA micoordenada) const
{
  CELDA micelda;
  uchar bx, by, bz;
  uchar signox, signoy, signoz;
  
  if(micoordenada.x < 0){
    signox = 1;
    micoordenada.x = -micoordenada.x;
  }
  else
    signox = 0;
  
  if(micoordenada.y < 0){
    signoy = 1;
    micoordenada.y = -micoordenada.y;
  }
  else
    signoy = 0;
  
  if(micoordenada.z < 0){
    signoz = 1;
    micoordenada.z = -micoordenada.z;
  }
  else
    signoz = 0;
  
  if( (micoordenada.x > espacio.xmax) || (micoordenada.y > espacio.ymax) || (micoordenada.z > espacio.zmax) ){
    printf("Coordenadas x,y,z = %d, %d, %d fuera de los límites del espacio\n", micoordenada.x, micoordenada.y, micoordenada.z);
    //exit(-1);
  }
  
  if( (micoordenada.x / espacio.delta_x > 255) || (micoordenada.y / espacio.delta_y > 255)  || (micoordenada.z / espacio.delta_z > 255)  ){
    printf("El número de celdas en cada una de las coordenadas del espacio no debe ser mayor que 255\n");
    //exit(-1);
  }
  
  bx = (uchar) ((micoordenada.x + (espacio.delta_x /2)) / espacio.delta_x);
  by = (uchar) ((micoordenada.y + (espacio.delta_y /2)) / espacio.delta_y);
  bz = (uchar) ((micoordenada.z + (espacio.delta_z /2)) / espacio.delta_z);

  /* Para corregir celdas con coordenada 0 y signo negativo; otra opcion seria calcular el signo después de dividir ... pero entonces seria mas complidado */

  if(bx==0)
	signox=0;
  if(by==0)
	signoy=0;
  if(bz==0)
	signoz=0;

  micelda = (signox*4 + signoy*2 + signoz) * 16777216 + bx * 65536 + by*256 + bz;
  return micelda;
}


COORDENADA manejadorceldas::coordenadaCelda(CELDA micelda) const
{
  COORDENADA micoord;
  
  micoord.z = micelda % 256 * espacio.delta_z; //+ espacio.delta_z/2;
  micelda/=256;
  micoord.y = micelda % 256 * espacio.delta_y; // + espacio.delta_y/2;
  micelda/=256;
  micoord.x = micelda % 256 * espacio.delta_x; // + espacio.delta_x/2;
  micelda/=256;
  if(micelda & 4)
    micoord.x=-micoord.x;
  if(micelda & 2)
    micoord.y=-micoord.y;
  if(micelda & 1)
    micoord.z=-micoord.z;
  
  return micoord;
}

double manejadorceldas::distanciaEntreCoordenadasAlCuadrado(COORDENADA coord1, COORDENADA coord2) const
{
  return( (double)(pow((coord1.x - coord2.x),2) + pow((coord1.y - coord2.y),2) + pow((coord1.z - coord2.z),2)));
}

double manejadorceldas::distanciaEntreCoordenadas(COORDENADA coord1, COORDENADA coord2) const
{
  return(sqrt( pow((coord1.x - coord2.x),2) + pow((coord1.y - coord2.y),2) + pow((coord1.z - coord2.z),2)));
}

double manejadorceldas::distanciaEntreCeldas(CELDA celda1, CELDA celda2) const
{
  return (distanciaEntreCoordenadas(coordenadaCelda(celda1), coordenadaCelda(celda2)));
}




