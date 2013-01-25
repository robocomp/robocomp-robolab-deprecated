#include <tracker/Silueta.h>
#include <stdlib.h>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace std;

typedef struct PUNTO1 {
  int x;
  int y;
}PUNTO1;

unsigned char DisparidadMedia(TRACKER *track, int imin,int imax, int jmin, int jmax)
{
  unsigned char media;

  //if ( (indice_region<0)||(indice_region>=NUMMARCS) )
  // {
  //  cerr<<"ERROR EN DisparidadMedia (tracker/Silueta.cpp). Indice de región no válido"<<endl;
  //  return(0);
  // }

  int total = 0;
  int totalptos = 0;

  for (int i=imin; i<imax; i++)
    for (int j=jmin; j<jmax; j++)
      {
	total += (int)(track->imdispar.pixel[i][j]);
	totalptos++;
      }

  if (totalptos != 0) media = (unsigned char)(total/totalptos);

  return(media);
}

void UmbralizarDisparidad(IMAGEN *in, unsigned char **imagen_out, int umbral_adelante, int umbral_atras, unsigned char disparidad_media)
{
  unsigned char** imagen_in = in->pixel;

  for(int i=0;i<in->alto;i++)
    for(int j=0;j<in->ancho;j++)
      {
	if ( (imagen_in[i][j]>=disparidad_media) && (imagen_in[i][j]<=disparidad_media+umbral_adelante) )
	  imagen_out[i][j]=255;//imagen_in[i][j];
	else if ( (imagen_in[i][j]<=disparidad_media) && (imagen_in[i][j]>=disparidad_media-umbral_atras) )
	  imagen_out[i][j]=255;//imagen_in[i][j];
	else imagen_out[i][j]=0;
      }
}

bool Distancia(int umbral, int g1,  int g2)
{
 int distancia;

  if ((g1==0)||(g2==0)){
      return(true);
      }
  else{
  distancia=abs(g1-g2);
   if (distancia>umbral){
     return (true);
  }
   else{
     return (false);
    }
   }
}


int Fuego(unsigned char **in_image, unsigned char ** out_image,int fil, int col,int num,
int N1, int M1, int N2, int M2, int umbral){

int i,j;
bool valido;

out_image[fil][col]=1;

 for (i=-1; i<=1; i++)
   for (j=-1; j<=1; j++)
     if (((fil+i)>=0) && ((fil+i)<N2) && ((col+j)>=0) && ((col+j)<M2)){
       // if((i!=0)&&(j!=0)){
     if(out_image[fil+i][col+j]==0){
       valido=Distancia(umbral,in_image[fil][col],in_image[i+fil][col+j]);
       if (!valido){
         Fuego(in_image,out_image,fil+i,col+j,num,N1,M1,N2,M2,umbral);
          }
      else{
        out_image[fil+i][col+j]=255;
        }
      }
    //}
   }
  return(0);
 }
//----------------------------------------------
void PrenderFuegoRecursivo(unsigned char **in_image, unsigned char **out_image,
			   int NS, int N1, int M1, int N2, int M2, int umbral)
{
int i,j,num;

  num=0;
  for (i=N1; i<N2; i++)
   for (j=M1; j<M2; j++)
     {
       Fuego(in_image,out_image,i,j,num+1,N1,M1,N2,M2,umbral);
       num++;
       if (num>=NS) return;
     }
}
 
//-------------------------

void RefinarSilueta(TRACKER *track)
{
  unsigned char **aux, **aux2;
  int alto_mitad = track->silueta.alto/2;
  int ancho_mitad = track->silueta.ancho/2;
  int alto_cuarto = track->silueta.alto/4;
  int ancho_cuarto = track->silueta.ancho/4;
  int i,j;

  aux= new unsigned char*[alto_mitad];
  for(i=0;i<alto_mitad;i++)
    aux[i]=new unsigned char [ancho_mitad];

  aux2= new unsigned char*[alto_cuarto];
  for(i=0;i<alto_cuarto;i++)
    aux2[i]=new unsigned char [ancho_cuarto];

  // Limpieza de aux
  for (i=0;i<alto_mitad;i++)
    for (j=0;j<ancho_mitad;j++)
      aux[i][j] = 0;

  // Limpieza de aux2
  for (i=0;i<alto_cuarto;i++)
    for (j=0;j<ancho_cuarto;j++)
      aux2[i][j] = 0;

  // Disminución a la mitad de tamaño y dilatación
  for (i=1;i<(alto_mitad-1);i++)
    for (j=1;j<(ancho_mitad-1);j++)
      {
	unsigned char vm1 = (track->silueta.pixel[2*i][2*(3*j)] + track->silueta.pixel[2*i][2*(3*j) + 1] + track->silueta.pixel[2*i][2*(3*j) + 2])/3;
	unsigned char vm2 = (track->silueta.pixel[2*i-1][2*(3*j)] + track->silueta.pixel[2*i-1][2*(3*j) + 1] + track->silueta.pixel[2*i-1][2*(3*j) + 2])/3;
	unsigned char vm3 = (track->silueta.pixel[2*i][2*(3*j) - 3] + track->silueta.pixel[2*i][2*(3*j) - 3 + 1] + track->silueta.pixel[2*i][2*(3*j) - 3 + 2])/3;
	unsigned char vm4 = (track->silueta.pixel[2*i-1][2*(3*j) - 3] + track->silueta.pixel[2*i-1][2*(3*j) - 3 + 1] + track->silueta.pixel[2*i-1][2*(3*j) - 3 + 2])/3;

	if ( (vm1>0)||(vm2>0)||(vm3>0)||(vm4>0) )
	  {
	    aux[i-1][j-1] = 255;
	    aux[i-1][j] = 255;
	    aux[i-1][i+1] = 255;
	    aux[i][j-1] = 255;
	    aux[i][j] = 255; 
	    aux[i][j+1] = 255;
	    aux[i+1][j-1] = 255;
	    aux[i+1][j] = 255;
	    aux[i+1][j+1] = 255;
	  }
      }

  // Disminución a un cuarto de tamaño y dilatación
  for (i=1;i<(alto_cuarto-1);i++)
    for (j=1;j<(ancho_cuarto-1);j++)
      {
	unsigned char vm1 = aux[2*i][2*j];
	unsigned char vm2 = aux[2*i-1][2*j];
	unsigned char vm3 = aux[2*i][2*j-1];
	unsigned char vm4 = aux[2*i-1][2*j-1];

	if ( (vm1>0)||(vm2>0)||(vm3>0)||(vm4>0) )
	  {
	    aux2[i-1][j-1] = 255;
	    aux2[i-1][j] = 255;
	    aux2[i-1][i+1] = 255;
	    aux2[i][j-1] = 255;
	    aux2[i][j] = 255; 
	    aux2[i][j+1] = 255;
	    aux2[i+1][j-1] = 255;
	    aux2[i+1][j] = 255;
	    aux2[i+1][j+1] = 255;
	  }
      }

  // Erosión de aux2
  for (i=1;i<(alto_cuarto-1);i++)
    for (j=1;j<(ancho_cuarto-1);j++)
      {
	if ( (aux2[i-1][j-1] == 0)||(aux2[i-1][j] == 0)||(aux2[i-1][j+1] == 0)||
	     (aux2[i][j-1] == 0)||(aux2[i][j] == 0)||(aux2[i][j+1] == 0)||
	     (aux2[i+1][j-1] == 0)||(aux2[i+1][j] == 0)||(aux2[i+1][j+1] == 0) )
	  aux2[i][j] = 128;
      }  

  // Paso a tamaño mitad (de aux2 a aux)
  for (i=0;i<alto_mitad;i++)
    for (j=0;j<ancho_mitad;j++)
      {
	if ( aux2[i/2][j/2] == 255 )
	  aux[i][j] = 255;
	else
	  aux[i][j] = 0;
      }  

  // Erosión
  for (i=1;i<(alto_mitad-1);i++)
    for (j=1;j<(ancho_mitad-1);j++)
      {
	if ( (aux[i-1][j-1] == 0)||(aux[i-1][j] == 0)||(aux[i-1][j+1] == 0)||
	     (aux[i][j-1] == 0)||(aux[i][j] == 0)||(aux[i][j+1] == 0)||
	     (aux[i+1][j-1] == 0)||(aux[i+1][j] == 0)||(aux[i+1][j+1] == 0) )
	  aux[i][j] = 128;
      }  

  // Dibujar la silueta en track->silueta
  for (i=0;i<track->silueta.alto;i++)
    for (j=0;j<track->silueta.ancho;j++)
      {
	if (aux[i/2][j/2] == 255 )
	  {
	    if (track->imdispar.pixel[i][j]!=0) {
	      track->silueta.pixel[i][3*j] = track->imdispar.pixel[i][j];//track->im.pixel[(i/2)][3*(j/2)];//255;
	      track->silueta.pixel[i][3*j + 1] = track->imdispar.pixel[i][j];//track->im.pixel[(i/2)][3*(j/2) + 1];//255;
	      track->silueta.pixel[i][3*j + 2] = track->imdispar.pixel[i][j];//track->im.pixel[(i/2)][3*(j/2) + 2];//255;
	    }
	    else {
	      track->silueta.pixel[i][3*j] = -1;
	      track->silueta.pixel[i][3*j + 1] = -1;
	      track->silueta.pixel[i][3*j + 2] = -1;
	    }
	  }
	else
	  {
	    track->silueta.pixel[i][3*j] = 0; 
	    track->silueta.pixel[i][3*j + 1] = 0;
	    track->silueta.pixel[i][3*j + 2] = 0;
	  }
      }

  for(i=0;i<alto_mitad;i++)
    delete aux[i];
  delete aux;  

  for(i=0;i<alto_cuarto;i++)
    delete aux2[i];
  delete aux2;  
    
}

void ObtenerSilueta(TRACKER *track, int umbral_palante, int umbral_patras, int imin, int imax, int jmin, int jmax, bool solo_conectadas)
{
  unsigned char disp_media;
  unsigned char **aux, **quemado;
  int i, j;

  aux= new unsigned char*[track->imdispar.alto];
  for(i=0;i<track->imdispar.alto;i++)
    aux[i]=new unsigned char [track->imdispar.ancho];

  quemado= new unsigned char*[track->imdispar.alto];
  for(i=0;i<track->imdispar.alto;i++)
    quemado[i]=new unsigned char [track->imdispar.ancho];
  
  // Inicializacion de quemado
  for (i=0;i<track->silueta.alto;i++)
    for (j=0;j<track->silueta.ancho;j++)
      {
	quemado[i][j] = 0;
      }	 

  //cout<<"Que pasa?"<<endl;

  // Calcula el valor medio de la disparidad de la cara
  disp_media = DisparidadMedia(track, imin,imax,jmin,jmax);
  // cout<<"DispMedia: "<<(int)disp_media<<endl;
  // Comprobación de errores
  if (disp_media == 0) //disp_media=100;
     return;
  // Umbraliza la disparidad en torno al valor de disparidad de la cara. El resultado se 
  // guarda en aux
  UmbralizarDisparidad(&track->imdispar, aux, umbral_palante, umbral_patras, disp_media);

 // Selecciona la silueta
  if (solo_conectadas) // si sólo se quiere la silueta, se aplica el fuego recursivo 
    PrenderFuegoRecursivo(aux, quemado, 1, 
			  (imax+imin)/2, (jmin+jmax)/2,
			  track->silueta.alto, track->silueta.ancho, 1);
  
  for (i=0;i<track->silueta.alto;i++)
    for(j=0;j<track->silueta.ancho;j++){
      if ( ((solo_conectadas)&&(quemado[i][j]==1)) || ((!solo_conectadas)&&(aux[i][j]==255)) ) {
  	track->silueta.pixel[i][3*j] = track->imdispar.pixel[i][j];//255;//track->im.pixel[(i/2)][3*(j/2)];
	track->silueta.pixel[i][3*j+1] = track->imdispar.pixel[i][j];//255;//track->im.pixel[(i/2)][3*(j/2)+1];
	track->silueta.pixel[i][3*j+2] = track->imdispar.pixel[i][j];//255;//track->im.pixel[(i/2)][3*(j/2)+2];
      }
      else{
  	track->silueta.pixel[i][3*j] =0;
  	track->silueta.pixel[i][3*j+1] = 0;
  	track->silueta.pixel[i][3*j+2] = 0;
      }
    }

  // Si luego se quiere obtener el eje medio, es NECESARIO refinar la silueta para eliminar los huecos intennos.
  //RefinarSilueta(track);  
  
  for(i=0;i<track->silueta.alto;i++)
    {
      delete aux[i];
      if (solo_conectadas) delete quemado[i];
    }
  delete aux;  
  if (solo_conectadas) delete quemado;

}



/**************

                     NUMERICAL POTENTIAL FIELD PATH PLANNER


      Basado en los trabajos de:

             Bent David Olsen - Aalborg University.
             http://www.vision.auc.dk/~einstein/s10/main/node21.html

      Algoritmo principal:

         - Usando el campo de repulsi¢n que generan los obst culos, se genera un
           esqueleto de la regi¢n libre.
         - Se une el punto de fin a dicho esqueleto, y se calcula el campo de
           potencial en este esqueleto aumentado.
         - Utilizando el potencial del esqueleto, se calcula el campo de potencial
           en el resto de la imagen.
         - Partiendo del punto de inicio se llega hasta el fin usando un algoritmo A*
           cuyo coste es, simplemente, el potencial calculado.

      Entradas:

         - Nombre mapa probabil¡stico.
         - Inicio del camino (x e y).
         - Fin del camino (x e y).
         - Tama¤o del mapa (TAMxTAM).

      Problemas detectados:

         - El esqueleto obtenido no siempre est  perfectamente conectado, con lo cual es
           posible que el campo de potencial que se debe obtener a partir de ste no quede
           bien definido. Se puede mejorar la conectividad disminuyendo la constante Tskel,
           pero entonces la esqueletizaci¢n no tiene porqu ser correcta (aparecen m s
           tramos).

         - Se detecta un posible mal funcionamiento cuando el mapa no est  bordeado de
           obst culos, ya que en la periferia no se genera esqueleto.

         - La uni¢n del punto de fin al esqueleto por cercan¡a puede dar problemas, si
           desde ese punto no se ve en l¡nea recta el esqueleto.

      Fecha:

         - 7/6/00
         - Modificado en Septiembre del 2002 para que solo genere
         el esqueleto, y no el camino.

***************/

/*******

   DistV4 - Calcula la distancia usando conectividad-4 entre (ax,ay) y (bx,by).

*******/

float DistV4(int ax, int ay, int bx, int by)
   {
   return(((float) abs(ax-bx))+((float) abs(ay-by)));
   }

double Esqueleto(unsigned char **im, unsigned char **mapa3,int TAM, float Tskel){

   div_t d1, d2;
   FILE *punt;
   int i,j,k;

   unsigned char **mapa2;      /* Mapa del entorno */

   unsigned char **mapa;      /* Mapa del entorno */
   int **mapaox;              /* Mapa de or¡genes en x */
   int **mapaoy;              /* Mapa de or¡genes en y */
   int **mapapert;            /* Mapa de pertenencias */
   PUNTO1 **L;                 /* Nodos a marcar para crear potenciales */
   PUNTO1 *S;                  /* Nodos que forman el esqueleto */
   int xin,yin,xfin,yfin;     /* Coordenadas de inicio y fin del camino */

   int NumNodos,NumNodosNuevo;
   int NumNodosEsq;
   int Min,xmin,ymin,Max;
   int xc,yc;
   int cont=1;
   int salir=0;

   //unsigned char **mapa3;

   ptime time_inicio;
   ptime time_fin;
   time_duration intervalo;

   /* Lectura de datos */

   /*N=     argv[1];        /* Nombre del mapa de probabilidad de ocupaci¢n */
   /*TAM=   atoi(argv[2]);  /* Tama¤o del mapa en cuadrado (TAMxTAM) */

   /* Creaci¢n del mapa y de L */

   if ((mapa=(unsigned char **) malloc (TAM*sizeof(unsigned char *)))==NULL)
     {cout<<"Falta memoria"<<endl; exit(-1);}
   for (i=0;i<TAM;i++)
      if ((mapa[i]=(unsigned char *) malloc (TAM*sizeof(unsigned char)))==NULL)
	{cout<<"Falta memoria"<<endl; exit(-1);}
   if ((mapa2=(unsigned char **) malloc (TAM*sizeof(unsigned char *)))==NULL)
     {cout<<"Falta memoria"<<endl; exit(-1);}
   for (i=0;i<TAM;i++)
      if ((mapa2[i]=(unsigned char *) malloc (TAM*sizeof(unsigned char)))==NULL)
         {cout<<"Falta memoria"<<endl; exit(-1);}

   if ((mapapert=(int **) malloc (TAM*sizeof(int *)))==NULL)
      {cout<<"Falta memoria"<<endl; exit(-1);}
   for (i=0;i<TAM;i++)
      if ((mapapert[i]=(int *) malloc (TAM*sizeof(int)))==NULL)
         {cout<<"Falta memoria"<<endl; exit(-1);}

   if ((mapaox=(int **) malloc (TAM*sizeof(int *)))==NULL)
      {cout<<"Falta memoria"<<endl; exit(-1);}
   for (i=0;i<TAM;i++)
      if ((mapaox[i]=(int *) malloc (TAM*sizeof(int)))==NULL)
         {cout<<"Falta memoria"<<endl; exit(-1);}

   if ((mapaoy=(int **) malloc (TAM*sizeof(int *)))==NULL)
      {cout<<"Falta memoria"<<endl; exit(-1);}
   for (i=0;i<TAM;i++)
      if ((mapaoy[i]=(int *) malloc (TAM*sizeof(int)))==NULL)
         {cout<<"Falta memoria"<<endl; exit(-1);}

   if ((L=(PUNTO1 **) malloc (2*sizeof(PUNTO1 *)))==NULL)
      {cout<<"Falta memoria"<<endl; exit(-1);}
   for (i=0;i<2;i++)
      if ((L[i]=(PUNTO1 *) malloc (TAM*TAM*sizeof(PUNTO1)))==NULL)
         {cout<<"Falta memoria"<<endl; exit(-1);}

   if ((S=(PUNTO1 *) malloc (TAM*TAM*sizeof(PUNTO1)))==NULL)
      {cout<<"Falta memoria"<<endl; exit(-1);}

   /* Leer el mapa de probabilidad y convertirlo en un mapa binario */

   time_inicio = microsec_clock::local_time();

   /*if ((punt=fopen(N,"rb"))==NULL)
      {printf("Error lectura fichero\n");exit(-1);}*/

   for (i=0;i<TAM;i++)
      for (j=0;j<TAM;j++)
         {
         mapapert[i][j]=0;
         if (im[i][j*3]==0)
            mapa[i][j]=255;
         else
            mapa[i][j]=0;
         }
   //fclose(punt);

   /********
      Paso 1: B£squeda del esqueleto de la regi¢n libre de obst culos.
   ********/

   /* Busca los puntos del borde de los obst culos y los almacena en L[0].
      En NumNodos cuenta el n£mero de nodos en L[0] y usa mapa para luego
      calcular el campo de potencial por repulsi¢n. */

   /* Estudio esquinas mapa probabil¡stico */

   NumNodos=0;

   if (mapa[0][0]==0)
     if (mapa[1][0]==255 || mapa[0][1]==255)
       {
       L[0][NumNodos].x=0;L[0][NumNodos].y=0;
       NumNodos++;
       mapa[0][0]=1;
       }

   if (mapa[0][TAM-1]==0)
     if (mapa[1][TAM-1]==255 || mapa[0][TAM-2]==255)
       {
       L[0][NumNodos].x=0;L[0][NumNodos].y=TAM-1;
       NumNodos++;
       mapa[0][TAM-1]=1;
       }

   if (mapa[TAM-1][0]==0)
     if (mapa[TAM-1][1]==255 || mapa[TAM-2][0]==255)
       {
       L[0][NumNodos].x=TAM-1;L[0][NumNodos].y=0;
       NumNodos++;
       mapa[TAM-1][0]=1;
       }

   if (mapa[TAM-1][TAM-1]==0)
     if (mapa[TAM-2][TAM-1]==255 || mapa[TAM-1][TAM-2]==255)
       {
       L[0][NumNodos].x=TAM-1;L[0][NumNodos].y=TAM-1;
       NumNodos++;
       mapa[TAM-1][TAM-1]=1;
       }

   /* Estudio bordes mapa probabil¡stico */

   for (i=1;i<TAM-1;i++)
      if (mapa[i][0]==0)
         if (mapa[i-1][0]==255 || mapa[i+1][0]==255 || mapa[i][1]==255)
            {
            L[0][NumNodos].x=i;L[0][NumNodos].y=0;
            NumNodos++;
            mapa[i][0]=1;
            }

   for (i=1;i<TAM-1;i++)
      if (mapa[i][TAM-1]==0)
         if (mapa[i-1][TAM-1]==255 || mapa[i+1][TAM-1]==255 || mapa[i][TAM-2]==255)
            {
            L[0][NumNodos].x=i;L[0][NumNodos].y=TAM-1;
            NumNodos++;
            mapa[i][TAM-1]=1;
            }

   for (j=1;j<TAM-1;j++)
      if (mapa[0][j]==0)
         if (mapa[0][j-1]==255 || mapa[0][j+1]==255 || mapa[1][j]==255)
            {
            L[0][NumNodos].x=0;L[0][NumNodos].y=j;
            NumNodos++;
            mapa[0][j]=1;
            }

   for (j=1;j<TAM-1;j++)
      if (mapa[TAM-1][j]==0)
         if (mapa[TAM-1][j-1]==255 || mapa[TAM-1][j+1]==255 || mapa[TAM-2][j]==255)
            {
            L[0][NumNodos].x=TAM-1;L[0][NumNodos].y=j;
            NumNodos++;
            mapa[TAM-1][j]=1;
            }

   /* Estudio resto mapa probabil¡stico */

   for (i=1;i<TAM-1;i++)
      for (j=1;j<TAM-1;j++)
         {
         if (mapa[i][j]==0)
            if ((mapa[i+1][j]==255) || (mapa[i-1][j]==255)
             || (mapa[i][j+1]==255) || (mapa[i][j-1]==255))
             {
             L[0][NumNodos].x=i;L[0][NumNodos].y=j;
             NumNodos++;
             mapa[i][j]=1;
             }
         }

/*#if PRUEBAS
   ShowMessage("Creado mapa de potencial por obstaculos ");
#endif*/

   /* C lculo del esqueleto. Para ello, se propaga L[0] sobre L[1], despus L[1] sobre
      L[0], y as¡ sucesivamente. De esta forma con s¢lo dos vectores de nodos se puede
      expandir todo el campo de potencial. Los nodos del esqueleto ser n aquellos que
      se encuentran en la uni¢n de dos ondas distintas que provienen de lugares
      suficientemente alejados en el mapa (esa medida de lejan¡a la impone Tskel). El
      esqueleto se guarda en mapapert (valor 30000 - esqueleto). */

   /* Almacenamiento de los or¡genes de los frentes de onda. */

   for (i=0;i<NumNodos;i++)
      {
      mapaox[L[0][i].x][L[0][i].y]=L[0][i].x;
      mapaoy[L[0][i].x][L[0][i].y]=L[0][i].y;
      }

   k=0;j=0;
   NumNodosEsq=0;
   while (NumNodos!=0)
      {
      NumNodosNuevo=0;
      for (i=0;i<NumNodos;i++)
         {
         if (L[j][i].x+1<TAM)
            if (mapa[L[j][i].x+1][L[j][i].y]==0)
               {
               mapa[L[j][i].x+1][L[j][i].y]=k+2;
               L[(j+1)%2][NumNodosNuevo].x=L[j][i].x+1;
               L[(j+1)%2][NumNodosNuevo].y=L[j][i].y;
               mapaox[L[j][i].x+1][L[j][i].y]=mapaox[L[j][i].x][L[j][i].y];
               mapaoy[L[j][i].x+1][L[j][i].y]=mapaoy[L[j][i].x][L[j][i].y];
               NumNodosNuevo++;
               }
            else
               {
               if ((mapa[L[j][i].x+1][L[j][i].y]!=255) &&
                   (DistV4(mapaox[L[j][i].x][L[j][i].y],mapaoy[L[j][i].x][L[j][i].y],
                    mapaox[L[j][i].x+1][L[j][i].y],mapaoy[L[j][i].x+1][L[j][i].y])>Tskel)
                    && mapapert[L[j][i].x][L[j][i].y]==0)
                  {
                  S[NumNodosEsq].x=L[j][i].x+1;
                  S[NumNodosEsq].y=L[j][i].y;
                  mapapert[L[j][i].x+1][L[j][i].y]=30000;
                  NumNodosEsq++;
                  }
               }
         if (L[j][i].y+1<TAM)
            if (mapa[L[j][i].x][L[j][i].y+1]==0)
               {
               mapa[L[j][i].x][L[j][i].y+1]=k+2;
               L[(j+1)%2][NumNodosNuevo].x=L[j][i].x;
               L[(j+1)%2][NumNodosNuevo].y=L[j][i].y+1;
               mapaox[L[j][i].x][L[j][i].y+1]=mapaox[L[j][i].x][L[j][i].y];
               mapaoy[L[j][i].x][L[j][i].y+1]=mapaoy[L[j][i].x][L[j][i].y];
               NumNodosNuevo++;
               }
            else
               {
               if ((mapa[L[j][i].x][L[j][i].y+1]!=255) &&
                   (DistV4(mapaox[L[j][i].x][L[j][i].y],mapaoy[L[j][i].x][L[j][i].y],
                    mapaox[L[j][i].x][L[j][i].y+1],mapaoy[L[j][i].x][L[j][i].y+1])>Tskel)
                    && mapapert[L[j][i].x][L[j][i].y]==0)
                  {
                  S[NumNodosEsq].x=L[j][i].x;
                  S[NumNodosEsq].y=L[j][i].y+1;
                  mapapert[L[j][i].x][L[j][i].y+1]=30000;
                  NumNodosEsq++;
                  }
               }
         if (L[j][i].x-1>=0)
            if (mapa[L[j][i].x-1][L[j][i].y]==0)
               {
               mapa[L[j][i].x-1][L[j][i].y]=k+2;
               L[(j+1)%2][NumNodosNuevo].x=L[j][i].x-1;
               L[(j+1)%2][NumNodosNuevo].y=L[j][i].y;
               mapaox[L[j][i].x-1][L[j][i].y]=mapaox[L[j][i].x][L[j][i].y];
               mapaoy[L[j][i].x-1][L[j][i].y]=mapaoy[L[j][i].x][L[j][i].y];
               NumNodosNuevo++;
               }
            else
               {
               if ((mapa[L[j][i].x-1][L[j][i].y]!=255) &&
                   (DistV4(mapaox[L[j][i].x][L[j][i].y],mapaoy[L[j][i].x][L[j][i].y],
                    mapaox[L[j][i].x-1][L[j][i].y],mapaoy[L[j][i].x-1][L[j][i].y])>Tskel)
                    && mapapert[L[j][i].x][L[j][i].y]==0)
                  {
                  S[NumNodosEsq].x=L[j][i].x-1;
                  S[NumNodosEsq].y=L[j][i].y;
                  mapapert[L[j][i].x-1][L[j][i].y]=30000;
                  NumNodosEsq++;
                  }
               }
         if (L[j][i].y-1>=0)
            if (mapa[L[j][i].x][L[j][i].y-1]==0)
               {
               mapa[L[j][i].x][L[j][i].y-1]=k+2;
               L[(j+1)%2][NumNodosNuevo].x=L[j][i].x;
               L[(j+1)%2][NumNodosNuevo].y=L[j][i].y-1;
               mapaox[L[j][i].x][L[j][i].y-1]=mapaox[L[j][i].x][L[j][i].y];
               mapaoy[L[j][i].x][L[j][i].y-1]=mapaoy[L[j][i].x][L[j][i].y];
               NumNodosNuevo++;
               }
            else
               {
               if ((mapa[L[j][i].x][L[j][i].y-1]!=255) &&
                   (DistV4(mapaox[L[j][i].x][L[j][i].y],mapaoy[L[j][i].x][L[j][i].y],
                    mapaox[L[j][i].x][L[j][i].y-1],mapaoy[L[j][i].x][L[j][i].y-1])>Tskel)
                    && mapapert[L[j][i].x][L[j][i].y]==0)
                  {
                  S[NumNodosEsq].x=L[j][i].x;
                  S[NumNodosEsq].y=L[j][i].y-1;
                  mapapert[L[j][i].x][L[j][i].y-1]=30000;
                  NumNodosEsq++;
                  }
               }
         }
      NumNodos=NumNodosNuevo;
      j=(j+1)%2;
      k++;
      }

      for (i=0; i<TAM; i++){
        for(j=0;j<TAM; j++){
          mapa3[i][j]=255;
        }
      }
      for (i=0; i<NumNodosEsq; i++){
           mapa3[S[i].x][S[i].y]=0;
        }

/*#if PRUEBAS
   punt=fopen("repuls.raw","wb");
   for (i=0;i<TAM;i++)
      for (j=0;j<TAM;j++)
         fputc(mapa3[i][j],punt);
   fclose(punt);
#endif*/

   time_fin = microsec_clock::local_time();
   intervalo = time_fin - time_inicio;
   return((double)(intervalo.total_microseconds())/1000000);

}

void Dilatacion(unsigned char **im,unsigned char **aux, int TAM,bool claro){
unsigned char s;
int i,j,k,l;

if (claro){
for(k=1;k<TAM-1;k++){
   for(l=1;l<TAM-1;l++){
      s=im[k][l];
      for(i=0;i<3;i++){
         for(j=0;j<3;j++){
           if(s<im[k+i-1][l+j-1]) s=im[k+i-1][l+j-1];
         }
      }
      aux[k][l]=s;
   }
}
}else{
for(k=1;k<TAM-1;k++){
   for(l=1;l<TAM-1;l++){
      s=im[k][l];
      for(i=0;i<3;i++){
         for(j=0;j<3;j++){
           if(s>im[k+i-1][l+j-1]) s=im[k+i-1][l+j-1];
         }
      }
      aux[k][l]=s;
   }
}
}
}

void Erosion(unsigned char **im,unsigned char **aux, int TAM,bool claro){
unsigned char s;
int i,j,k,l;

if(claro){
for(k=1;k<TAM-1;k++){
   for(l=1;l<TAM-1;l++){
      s=im[k][l];
      for(i=0;i<3;i++){
         for(j=0;j<3;j++){
           if(s>im[k+i-1][l+j-1]) s=im[k+i-1][l+j-1];
         }
      }
      aux[k][l]=s;
   }
}
}else{
for(k=1;k<TAM-1;k++){
   for(l=1;l<TAM-1;l++){
      s=im[k][l];
      for(i=0;i<3;i++){
         for(j=0;j<3;j++){
           if(s<im[k+i-1][l+j-1]) s=im[k+i-1][l+j-1];
         }
      }
      aux[k][l]=s;
   }
}
}
}

