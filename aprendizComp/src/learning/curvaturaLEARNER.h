// -*-c++-*-
/////////////////////////////////////////////////////////////////////////////
// Nombre	:	curvaturaLEARNER.h
// Funcion	:     	algoritmo segmentacion array datos laser por curvatura
//			usando k=1, k fijo o k variable (k-salto entre datos del array)
// Autor	:      	A. Bandera
// Fecha	:     	Julio 2005
// Version	:     	v0.1
/////////////////////////////////////////////////////////////////////////////

#ifndef _CURVATURALEARNER_H
#define _CURVATURALEARNER_H

#include <vector>

using namespace std;

typedef struct DatLaser
{
  float f;
  float c;
  int kd;
  int ka;
} 
DatoLASER;

class CURVATURALEARNER
{
public:

  CURVATURALEARNER(int TamanoLaser_, int K_, int KMAX_, float UmbralAdapt_, float UmbralKey_ = 0.80);
  ~CURVATURALEARNER();
  
  DatoLASER *ArrayIN;	/* Array de datos de entrada 	         	*/
  float *Curvatura;	/* Array de datos de curvatura 			*/
  float *CurvDef;
  vector<int> Keypoints;  // Vector de keypoints
  float UmbralKey;        // Valor mínimo de curvatura (de 0 a 1) para considerar un punto como Keypoint
  int TamanoLaser;
  int K;			/* K=1,...KMax -> Salto entre valores para el 
				   calculo de curvatura (K=-1 -> adaptativo) 	*/
  
  int KMax;		/* Valor máximo de K */
  int KMin;
  
  float UmbralEsquina;	/* Umbral detección esquinas */
  float UmbralAdapt;	/* Umbral cálculo curvatura adaptativa */
  
  int  CalculoCurvatura();
  int  obtenerCurvaturaKcte();
  int  obtenerCurvaturaAdaptativo();
  void calcularKeypoints();
  bool fusionarKeypoints(int rango);

  float *  DetectarEsquinas();
  void DataInPixels(float *pixels, int inicio);
  
  void LeerArray(char *nombre);
  void PintarCurvatura(int grupo);
  inline float DistEuclidea(float a, float b, float a1, float b1);
  inline float CurvLocal(float f0, float f1, float b0, float b1);
};
#endif
