/////////////////////////////////////////////////////////////////////////////
// Nombre	:	curvaturaLEARNER.cpp
// Funcion	:     	algoritmo segmentacion array datos laser por curvatura
//			usando k=1, k fijo o k variable (k-salto entre datos del array)
// Autor	:      	A. Bandera
// Fecha	:     	Julio 2005
// Version	:     	v0.1
/////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <memory.h>
#include <time.h>
#include <sys/types.h>
#include "curvaturaLEARNER.h"

//#include "usr_clock.h"
CURVATURALEARNER::CURVATURALEARNER(int TamanoLaser_, int K_, int KMAX_, float UmbralAdapt_, float UmbralKey_)
{
  TamanoLaser=TamanoLaser_;
  K=K_;

  KMax=KMAX_;			/* Valor Maximo de K */
  KMin=25;//3;                  /* Valor Minimo de K : 50 (CODA) */

  UmbralEsquina=0.40;	        /* Umbral deteccion esquinas */
  UmbralAdapt=UmbralAdapt_;	/* Umbral calculo curvatura adaptativa */
  UmbralKey = UmbralKey_;       // Valor mínimo de curvatura (de 0 a 1) para considerar un punto como Keypoint

  //  std::cout<<"sizeCurvature"<<TamanoLaser<<std::endl;

  /* Creamos espacio para ArrayIN y Curvatura */

  ArrayIN=new DatoLASER[TamanoLaser];	/* Array de datos laser de entrada 	   */
  Curvatura=new float[TamanoLaser];	/* Array de datos de curvatura 		   */
  CurvDef= new float[TamanoLaser];      /* Array de datos de curvatura definitivos */	
  
  // Inicializamos el vector de keypoints
  Keypoints.clear();

}

CURVATURALEARNER::~CURVATURALEARNER()
{

  /* Borrar ArrayIN y Curvatura */
	
  delete(ArrayIN);	
  delete(Curvatura);	
  delete(CurvDef);
}

inline float CURVATURALEARNER::DistEuclidea(float a, float b, float a1, float b1)
{
  return ((float)sqrt((float)((a-a1)*(a-a1)+(b-b1)*(b-b1))));
}

int CURVATURALEARNER::CalculoCurvatura()
{
  int error=0;

  if (K>KMax) 
    {
      std::cout << "Error:: K excesivamente grande\n";
      exit(-1);	/* error=1 */
    }

  if (K<=0)	/* Calculo curvatura adaptativo 	*/
    {
      error=obtenerCurvaturaAdaptativo();
    }
  else		/* Calculo curvatura k constante 	*/
    {
      error=obtenerCurvaturaKcte();
    }
  
  // Normalizacion de la curvatura y obtención de los Keypoints (puntos de curvatura máxima). También se incluyen el primer y último puntos
  // de la secuencia, y otros puntos que se intercalan si pasó mucho tiempo desde el útimo keypoint
  calcularKeypoints();
  
  return(error);
}

inline float CURVATURALEARNER::CurvLocal(float f0, float f1, float b0, float b1)
{
  float r, signo;
  float modulo_f;
  float modulo_b;
  float cos_angulofb;

  // Formula de Rosenfeld y Johnston para la curvatura

  modulo_f=(float)sqrt((double)(f0*f0+f1*f1));
  modulo_b=(float)sqrt((double)(b0*b0+b1*b1));
  if ( (modulo_f!=0)&&(modulo_b!=0) )
    cos_angulofb=((float)(f0*b0+f1*b1))/(modulo_f*modulo_b);
  else
    cos_angulofb=-1; // ¿¿¿Qué pasa cuando alguno de los módulos es 0??? 
  // REPASAR QUÉ PASA CUANDO ALGUNO DE LOS ÁNGULOS ES CERO!!!
  r=fabs(0.5+0.5*cos_angulofb);

  // Mejorada por Pedro Reche
  signo=f0*b1>b0*f1 ? (1.0) :(-1.0);
  return(signo*r);
}

int CURVATURALEARNER::obtenerCurvaturaAdaptativo()
{
  int i;
  int cont=0;
  int error=0;
  int kdAhora;
  int kaAhora;
  float f0,f1,b0,b1;
  float distkdAhora;	/* Distancia pixel a pixel hacia delante */
  float distkaAhora;    /* Distancia pixel a pixel hacia atras */

  // FILE* punt=fopen("prueba.m","wb");
  // FILE* puntk=fopen("k.txt","wb");
  
  //  fprintf(punt,"hold on\n");
  if (TamanoLaser<=2*KMin)
    {
      std::cout << "TamanoLaser muy pequeño \n";
      exit(-1);	/* error=1 */
    }
  
  // Inicializacion: Calculo de kaAhora y distkaAhora (kdAhora=KMin) para el primer punto (i=KMin)
  kdAhora=KMin;
  kaAhora=KMin;
  distkaAhora=0.0;
  distkdAhora=0.0;
  for (i=KMin;i>0;i--)
    distkaAhora=distkaAhora+DistEuclidea(ArrayIN[i].f,ArrayIN[i].c,ArrayIN[i-1].f,ArrayIN[i-1].c);
  for (i=KMin;i<KMin+KMin;i++)
    distkdAhora=distkdAhora+DistEuclidea(ArrayIN[i].f,ArrayIN[i].c,ArrayIN[i+1].f,ArrayIN[i+1].c);
  do
    {
      kdAhora++;
      distkdAhora=distkdAhora
	+DistEuclidea(ArrayIN[KMin+kdAhora-1].f,ArrayIN[KMin+kdAhora-1].c,ArrayIN[KMin+kdAhora].f,ArrayIN[KMin+kdAhora].c);
    }
  while ((DistEuclidea(ArrayIN[KMin].f,ArrayIN[KMin].c,ArrayIN[KMin+kdAhora].f,ArrayIN[KMin+kdAhora].c)+UmbralAdapt)>distkdAhora 
	 && kdAhora<KMax);
  kdAhora--;
  distkdAhora=distkdAhora
    -DistEuclidea(ArrayIN[KMin+kdAhora].f,ArrayIN[KMin+kdAhora].c,ArrayIN[KMin+kdAhora+1].f,ArrayIN[KMin+kdAhora+1].c);
  
  // Bucle principal	:	- podemos hacer kaAhora mayor, pero sin que (i-kaAhora) sea menor que 0
  //								   - podemos hacer kdAhora mayor, pero sin que (i+kdAhora) sea mayor que TamanoLaser
  for (i=KMin;i<TamanoLaser-KMin;i++)
    {
      // Actualizacion de los campos ka y kd de ArrayIN y calculo de curvatura(cont) en funcion de kaAhora y kdAhora
      ArrayIN[i].ka=kaAhora;
      ArrayIN[i].kd=kdAhora;
      f0=ArrayIN[i+kdAhora].f-ArrayIN[i].f;
      f1=ArrayIN[i+kdAhora].c-ArrayIN[i].c;
      b0=ArrayIN[i-kaAhora].f-ArrayIN[i].f;
      b1=ArrayIN[i-kaAhora].c-ArrayIN[i].c;
      Curvatura[cont]= fabs(CurvLocal(f0,f1,b0,b1)); // se pone en absoluto porque lo que nos interesa es si hay mucha curvatura en el punto
      cont++;
      
      // fprintf(punt,"plot ([%f %f],[%f %f]) \n",ArrayIN[i].c,ArrayIN[i+kdAhora].c,ArrayIN[i].f,ArrayIN[i+kdAhora].f);
      // fprintf(punt,"plot ([%f %f],[%f %f]) \n",ArrayIN[i].c,ArrayIN[i-kaAhora].c,ArrayIN[i].f,ArrayIN[i-kaAhora].f);
      
      //  fprintf(puntk,"%d %d\n",ArrayIN[i].ka,ArrayIN[i].kd);
      
      // Valor de distkdAhora para i+1
      if (i+kdAhora+1<TamanoLaser)
	{
	  distkdAhora=distkdAhora
	    -DistEuclidea(ArrayIN[i].f,ArrayIN[i].c,ArrayIN[i+1].f,ArrayIN[i+1].c)
	    +DistEuclidea(ArrayIN[i+kdAhora].f,ArrayIN[i+kdAhora].c,ArrayIN[i+kdAhora+1].f,ArrayIN[i+kdAhora+1].c);

	  // Valores de kdAhora y distkdAhora actualizados para i+1
	  // A. Si DistEuclidea((i+1) a (i+1+kdAhora))+UmbralAdapt < distkdAhora -> reducir kdAhora
	  if (DistEuclidea(ArrayIN[i+1].f,ArrayIN[i+1].c,ArrayIN[i+kdAhora+1].f,ArrayIN[i+kdAhora+1].c)+UmbralAdapt<distkdAhora 
	      && kdAhora>KMin)
	    {
	      distkdAhora=distkdAhora-DistEuclidea(ArrayIN[i+kdAhora].f,ArrayIN[i+kdAhora].c,ArrayIN[i+kdAhora+1].f,ArrayIN[i+kdAhora+1].c);
	      kdAhora--;
	    }
	  // B. Si DistEuclidea+UmbralAdapt > distkdAhora -> aumentar kdAhora (si puede)
	  if (DistEuclidea(ArrayIN[i+1].f,ArrayIN[i+1].c,ArrayIN[i+kdAhora+1].f,ArrayIN[i+kdAhora+1].c)+UmbralAdapt>distkdAhora 
	      && kdAhora<KMax && i+kdAhora+2<TamanoLaser)
	    {
	      kdAhora++;
	      distkdAhora=distkdAhora+DistEuclidea(ArrayIN[i+kdAhora].f,ArrayIN[i+kdAhora].c,ArrayIN[i+kdAhora+1].f,ArrayIN[i+kdAhora+1].c);
	    }
	}
      else // No quedan puntos en ArrayIN, hay que recortar, forzosamente, kdAhora
	{
	  distkdAhora=distkdAhora-DistEuclidea(ArrayIN[i].f,ArrayIN[i].c,ArrayIN[i+1].f,ArrayIN[i+1].c);
	  kdAhora--;
	}
      
		// Valor de distkaAhora para i+1
      distkaAhora=distkaAhora
	+DistEuclidea(ArrayIN[i].f,ArrayIN[i].c,ArrayIN[i+1].f,ArrayIN[i+1].c)
	-DistEuclidea(ArrayIN[i-kaAhora].f,ArrayIN[i-kaAhora].c,ArrayIN[i-kaAhora+1].f,ArrayIN[i-kaAhora+1].c);
      
      // Valores de kaAhora y distkaAhora actualizados para i+1
      // A. Si DistEuclidea((i+1) a (i+1-kaAhora))+UmbralAdapt < distkaAhora -> reducir kaAhora
      if (DistEuclidea(ArrayIN[i+1].f,ArrayIN[i+1].c,ArrayIN[i-kaAhora+1].f,ArrayIN[i-kaAhora+1].c)+UmbralAdapt<distkaAhora 
	  && kaAhora>KMin)
	{
	  distkaAhora=distkaAhora-DistEuclidea(ArrayIN[i-kaAhora+1].f,ArrayIN[i-kaAhora+1].c,ArrayIN[i-kaAhora+2].f,ArrayIN[i-kaAhora+2].c);
	  kaAhora--;
	}
      // B. Si DistEuclidea+UmbralAdapt > distkaAhora -> aumentar kaAhora (siempre puede)
      if (DistEuclidea(ArrayIN[i+1].f,ArrayIN[i+1].c,ArrayIN[i-kaAhora+1].f,ArrayIN[i-kaAhora+1].c)+UmbralAdapt>distkaAhora 
	  && kaAhora<KMax)
	{
	  distkaAhora=distkaAhora+DistEuclidea(ArrayIN[i-kaAhora].f,ArrayIN[i-kaAhora].c,ArrayIN[i-kaAhora+1].f,ArrayIN[i-kaAhora+1].c);
	  kaAhora++;
	}

    }
  
  TamanoLaser=TamanoLaser-2*KMin-1; /* Actualizamos TamanoLaser */
  
  return(error);
}

int CURVATURALEARNER::obtenerCurvaturaKcte()
{
  int i;
  int error=0;
  
  TamanoLaser=TamanoLaser-K-1;
  
  if (TamanoLaser<=1)
    return(1);	/* error=1 */
  
  for (i=0;i<TamanoLaser+1;i++)
    {
      Curvatura[i]=(float)atan2((float)(ArrayIN[i+K].f-ArrayIN[i].f), (float)(ArrayIN[i+K].c-ArrayIN[i].c));
    }
  
  return(error);
}

void CURVATURALEARNER::calcularKeypoints()
{
  int intervalo_max = TamanoLaser / 1; // Como mínimo, debería haber unos 20 keypoints en el movimiento (por ehemplo).

  int sin_keys = 0;
  bool hay_key = false;

  for (int i=0;i<TamanoLaser;i++)
    {
      hay_key = false;

      // actualiza el valor de curvatura definitiva  
      if (K<=0) // K adaptativo
	CurvDef[i]=fabs(Curvatura[i]); 
      else // K constante
	CurvDef[i]=fabs(Curvatura[i+1]-Curvatura[i]);

      // extrae keypoints y los guarda en la lista
      if ( i==0 )
	{
	  Keypoints.push_back(i);
	  hay_key = true;
	  sin_keys = 0;
	}
      else if ( i==(TamanoLaser-1) )
	{
	  Keypoints.push_back(i+2*KMin);
	  hay_key = true;
	  sin_keys = 0;
	}
      else
	{
	  if ( (CurvDef[i]>=UmbralKey)
	       &&(CurvDef[i]>=Curvatura[i-1])
	       &&(CurvDef[i]>=Curvatura[i+1]) ) {
	    Keypoints.push_back(i+KMin);
	    hay_key = true;
	    sin_keys = 0;
	  }
	}
      
      // Si no ha habido keypoints en un intervalo de intervalo_max, se
      // sitúa uno
      if ( (!hay_key)&&(sin_keys>=intervalo_max) ) {
	Keypoints.push_back(i+KMin);
	hay_key = true;
	sin_keys = 0;
      }
      //Si después de todo no hay keyframes, se sigue contando
      if (!hay_key)
	sin_keys++;
    }	

  // Segundas vueltas: Fusión de Keypoints próximos
  // (no hay que preocuparse mucho por la 'recursividad', porque habrá pocos Keypoints)
  bool fusiones = true;
  while (fusiones)
    {
      fusiones = fusionarKeypoints(intervalo_max/(4*20));
    }  
}

// Función para fusionar Keypoints. Pega Keypoints cercanos, y devuelve un booleano indicando
// si se ha realizado alguna fusión o no (para que se pueda ejecutar recursivamente).
bool CURVATURALEARNER::fusionarKeypoints(int rango)
{
  bool result = false;
  int j = 0;
  for (vector<int>::iterator it = Keypoints.begin();
       it != Keypoints.end();
       it++,j++)
    {
      if ( (j!=0)&&(j<(Keypoints.size()-1)) ) // si no es el primero ni el último, es susceptible de borrarse
	{
	  if ( ((*it)-(Keypoints.at(j-1)) <= rango ) // ... si los puntos están cerca
	       &&( CurvDef[(*it)] <= CurvDef[(Keypoints.at(j-1))] ) ) // .. y el otro tiene más curvatura
	    {
	      // Hay fusión (por ahora, simplemente se elimina el punto menor)
	      result = true; 
	      Keypoints.erase(it);
	    }
	  else if ( ((Keypoints.at(j+1))-(*it) <= rango ) // ... si los puntos están cerca
	       &&( CurvDef[(*it)] <= CurvDef[(Keypoints.at(j+1))] ) ) // .. y el otro tiene más curvatura
	    {
	      // Hay fusión (por ahora, simplemente se elimina el punto menor)
	      result = true; 
	      Keypoints.erase(it);
	    }
	}
    }
  return result;  
}



float * CURVATURALEARNER::DetectarEsquinas()
{
  int i; 
  int N_ESQUINAS;int _X;int _Y;int INDICE; int n;
  int offset;
  float *esquinas;
  
  FILE *fich;

  N_ESQUINAS=20;
  _X=2;
  _Y=3;
  INDICE=1;
  offset=3;
  n=0;
  
  i=0; 
  esquinas=(float*) malloc((3*N_ESQUINAS)*sizeof(float));

  // Detectar esquinas convexas (maximos)
#if 0  
  if (Curvatura[0]>Curvatura[1] && Curvatura[0]>UmbralEsquina)
    {
      //      std::cout << "Esquina 0" << std::endl;
      esquinas[n*3+_X]=ArrayIN[i+offset].f;
      esquinas[n*3+_Y]=ArrayIN[i+offset].c;
      esquinas[n*3+INDICE]=i+offset;
      // std::cout<<"ArCurvature: FASE I "<<std::endl;
      fich=fopen("mapa.m","a");
      fprintf(fich,"plot(%f,%f,'r>');",esquinas[n*3+_X],esquinas[n*3+_Y]);
      fclose(fich);
      
      n++;
      if (n==N_ESQUINAS-1){ esquinas[0]=n;  return esquinas;}

      //      std::cout << "CURVATURA_LASER: Esquina X,Y"<<esquinas[n*3+_X]<<","<<esquinas[n*3+_Y]<< std::endl;
    }
#endif
  for (i=1;i<TamanoLaser-1;i++)
    if (Curvatura[i]>Curvatura[i-1] && Curvatura[i]>Curvatura[i+1] && Curvatura[i]>UmbralEsquina)
      {
	//	std::cout << "Esquina " << i << std::endl;
	esquinas[n*3+_X]=ArrayIN[i+offset].f;
	esquinas[n*3+_Y]=ArrayIN[i+offset].c;
	esquinas[n*3+INDICE]=i+offset;
	// std::cout<<"ArCurvature: FASE II "<<std::endl;
	fich=fopen("mapa.m","a");
	fprintf(fich,"plot(%f,%f,'r>');",esquinas[n*3+_X],esquinas[n*3+_Y]);
	fclose(fich);
	
	n++;

	if (n==N_ESQUINAS-1){ esquinas[0]=n;  return esquinas;}
	//	std::cout << "CURVATURA_LASER: Esquina X,Y"<<ArrayIN[i+offset].f<<","<<ArrayIN[i+offset].c<< std::endl;
      }
#if 0  
  if (Curvatura[i]>Curvatura[i-1] && Curvatura[i]>UmbralEsquina)
    {
      //      std::cout << "Esquina " << i << std::endl;
      esquinas[n*3+_X]=ArrayIN[i+offset].f;
      esquinas[n*3+_Y]=ArrayIN[i+offset].c;
      esquinas[n*3+INDICE]=i+offset;
      //   std::cout<<"ArCurvature: FASE III "<<std::endl;
	
      fich=fopen("mapa.m","a");
      fprintf(fich,"plot(%f,%f,'r>');",esquinas[n*3+_X],esquinas[n*3+_Y]);
      fclose(fich);
      
      n++;
      if (n==N_ESQUINAS-1){ esquinas[0]=n;  return esquinas;}

      //      std::cout << "CURVATURA_LASER: Esquina X,Y"<<esquinas[n*3+_X]<<","<<esquinas[n*3+_Y]<< std::endl;
    }
#endif
  // Detectar esquinas concavas (mÃ­nimos)
  esquinas[0]=n;

  return esquinas;
}

void CURVATURALEARNER::LeerArray(char *nombre)
{
  FILE* fd;
  int i;
  
  fd=fopen(nombre, "rb");
  if (!fd) 
    std::cout << "Error abriendo " << nombre << std::endl;
  
  for (i=0;i<TamanoLaser;i++)
    fscanf(fd,"%f %f\n",&ArrayIN[i].f,&ArrayIN[i].c);
  fclose(fd);
}

void CURVATURALEARNER::DataInPixels(float *pixels, int inicio)
{
 
  int i,k;
  k=0;
  //std::cout<<"Tamanno laser:"<<TamanoLaser<<std::endl;
  for (i=inicio;i<inicio+TamanoLaser;i++)
    {
      ArrayIN[k].f=pixels[i*3+1];
      ArrayIN[k].c=pixels[i*3+2];
      //std::cout<<"CURVATURA_LASER:"<<pixels[i*3+1]<<","<<pixels[i*3+2]<<std::endl;
      k++;
    }


}


void CURVATURALEARNER::PintarCurvatura(int grupo)
{
  int i;
  FILE *punt;
  char nombre[20];
  
  //	for (i=0;i<TamanoLaser;i++)
  //std::cout <<"pintando curva "<<grupo<<std::endl;

  sprintf(nombre,"curv%d.txt",grupo);
  punt=fopen(nombre,"wb");
  for (i=0;i<TamanoLaser;i++)
    fprintf(punt,"%f\n",CurvDef[i]);
  fclose(punt);
}




#if 0
// Filtrado

float Ak[9]={0.0051, 0.0294, 0.1107, 0.2193, 0.2710, 0.2193,0.1107, 0.0294, 0.0051};

for (i=0;i<TamanoLaser;i++)
  CurvDef[i]=Curvatura[i];

CurvDef[0]=Curvatura[0]*(Ak[0]+Ak[1]+Ak[2]+Ak[3]+Ak[4])+Curvatura[1]*Ak[5]+Curvatura[2]*Ak[6]+Curvatura[3]*Ak[7]+Curvatura[4]*Ak[8];
CurvDef[1]=Curvatura[0]*Ak[3]+Curvatura[1]*(Ak[0]+Ak[1]+Ak[2]+Ak[4])+Curvatura[2]*Ak[5]+Curvatura[3]*Ak[6]+Curvatura[4]*Ak[7]+Curvatura[5]*Ak[8];
CurvDef[2]=Curvatura[0]*Ak[2]+Curvatura[1]*Ak[3]+Curvatura[2]*(Ak[0]+Ak[1]+Ak[4])+Curvatura[3]*Ak[5]+Curvatura[4]*Ak[6]+Curvatura[5]*Ak[7]+Curvatura[6]*Ak[8];
CurvDef[3]=Curvatura[0]*Ak[1]+Curvatura[1]*Ak[2]+Curvatura[2]*Ak[3]+Curvatura[3]*(Ak[0]+Ak[4])+Curvatura[4]*Ak[5]+Curvatura[5]*Ak[6]+Curvatura[6]*Ak[7]+Curvatura[7]*Ak[8];

CurvDef[TamanoLaser-1]=Curvatura[TamanoLaser-1]*(Ak[0]+Ak[1]+Ak[2]+Ak[3]+Ak[4])
  +Curvatura[TamanoLaser-2]*Ak[5]
+Curvatura[TamanoLaser-3]*Ak[6]
+Curvatura[TamanoLaser-4]*Ak[7]
+Curvatura[TamanoLaser-5]*Ak[8];
CurvDef[TamanoLaser-2]=Curvatura[TamanoLaser-1]*Ak[3]
+Curvatura[TamanoLaser-2]*(Ak[0]+Ak[1]+Ak[2]+Ak[4])
  +Curvatura[TamanoLaser-3]*Ak[5]
+Curvatura[TamanoLaser-4]*Ak[6]
+Curvatura[TamanoLaser-5]*Ak[7]
+Curvatura[TamanoLaser-6]*Ak[8];
CurvDef[TamanoLaser-3]=Curvatura[TamanoLaser-1]*Ak[2]
+Curvatura[TamanoLaser-2]*Ak[3]
+Curvatura[TamanoLaser-3]*(Ak[0]+Ak[1]+Ak[4])
  +Curvatura[TamanoLaser-4]*Ak[5]
+Curvatura[TamanoLaser-5]*Ak[6]
+Curvatura[TamanoLaser-6]*Ak[7]
+Curvatura[TamanoLaser-7]*Ak[8];
CurvDef[TamanoLaser-4]=Curvatura[TamanoLaser-1]*Ak[1]
+Curvatura[TamanoLaser-2]*Ak[2]
+Curvatura[TamanoLaser-3]*Ak[3]
+Curvatura[TamanoLaser-4]*(Ak[0]+Ak[4])
  +Curvatura[TamanoLaser-5]*Ak[5]
+Curvatura[TamanoLaser-6]*Ak[6]
+Curvatura[TamanoLaser-7]*Ak[7]
+Curvatura[TamanoLaser-8]*Ak[8];


for (i=4;i<TamanoLaser-5;i++)
{
  CurvDef[i]=0.0;
  for (j=-4;j<5;j++)
    CurvDef[i]+=Ak[j+4]*Curvatura[i+j];
}
#endif
