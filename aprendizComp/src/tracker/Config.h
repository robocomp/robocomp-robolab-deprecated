#ifndef CONFIG_H
/* Parámetros de caracterización del color piel*/
#define CONFIG_H

#include <vector>
#include <cstdlib>

#if defined __BORLANDC__
        #include <vcl.h>
#endif

#define MT 149.0228  //Valor medio de T
#define MS 23.0944   //Valor medio de S
#define CTT 0.0058
#define CTS 0.0009   //Valores de covarianza
#define CSS 0.0094

#define NORMALIZACION 1

#define UMBRAL_IZQ 18.0
#define UMBRAL_DER 15.0

#define UMBRAL_LIZQ 0   //Umbral de luminosidad de clasificacion (60 o 80)
#define UMBRAL_LDER 0   //Umbral de luminosidad de clasificacion (60 o 80) (0 para las pruebas de Surrey)

#define ERODEDILATE 0 //Para erosionar y dilatar la imagen de color piel

#define NUMMARCS 3

//#define TAM256 0  //Si TAM256=0 entonces el tamaño es 128

//#if TAM256
#define ALTO 120 //240
#define ANCHO 160 //320

# if (ALTO>256 || ANCHO>256)
        #define TAMGRANDE 1
        #define TAMMASCARA 128
        #define MAXNIVEL 3

#else
        //#define ALTO 128
        //#define ANCHO 128
        #define TAMGRANDE 0
        #define TAMMASCARA 64
        #define MAXNIVEL 2

#endif

#define MAXDISP 16

#define ZONABUSQUEDA 6

#define MAXZONABUSQUEDA 12

#define DEBUG 1

#define APROXIMACION_POLINOMIAL 0  //Para hacer la aproximacion polinomial en estereo y tener
                                   //subpixel accuracy en la disparidad

//Constantes de calibracion de las cÃ¡maras

#define DISTANCIA 29.6
//CÃ¡mara izquierda

#define FCI1 262 //263
#define FCI2 346//347
#define CCI1 131//134
#define CCI2 133//118

//CÃ¡mara derecha

#define FCD1 262
#define FCD2 346
#define CCD1 131
#define CCD2 133


typedef struct Imagen{
unsigned char **pixel;
int alto;
int ancho;
unsigned char **T;
unsigned char **S;
unsigned char **L;
}IMAGEN;

typedef struct Piramide{
 int Homog;
 int X;
 int Y;
 int Clase;
}PIR;

typedef struct Box{
 int imin;
 int imax;
 int jmin;
 int jmax;
 int area;
 int areanivel;
 int sumi;
 int sumj;
 bool encontrada;
 bool en_estereo;
 int icent;
 int jcent;
}BOX;  

typedef struct Punto{
 int i;
 int j;
}PUNTO;

typedef struct Mascara{
 int piel;
 int marcar; //Para indicar si ese valor hay que marcarlo abajo
 int imin,imax,jmin,jmax; //Coordenadas de la bounding box
 int p; //Esto es para la probabilidad
}MASCARA;

// Error de objeto mas grande que la mascara.
struct objeto_demasiado_grande{};

struct TRACKER{
  IMAGEN im;
  IMAGEN seg;
  IMAGEN imder;
  IMAGEN segder;
  IMAGEN imdispar;
  IMAGEN silueta;
  IMAGEN esqueleto;
  MASCARA ****mask_ant;
  MASCARA ****mask_sig;
  BOX *boxes;
  BOX **boxnivel;
  BOX **boxAnt;
  int *TAMNiveles_i;
  int *TAMNiveles_j;
  int niveles;
  int *nivel;
  int *clases;
  int **mov_v;
  int **mov_h;
  PIR ****pir;
  PIR ***pirder;
  float *disparidad;
  //  float *dispnew, *dispold1,*dispold2,*dispold3,*dispold4,*dispold5,*dispold6,*dispold7,*dispold8,*dispold9;
  double *x, *y,*z;

  // int *prob;
  int a;
  int b;
  
  // Constructor que reserva su propia memoria pra im, la imagen de entrada.
  TRACKER();

  // Constructor que toma una imagen reservada externamente como imagen de entrada
  //TRACKER(unsigned char* data, bool _stereo = false);

};



/***************************************************************************

         BIP EN NIVELES DE GRISES. 

***************************************************************************/

#define AREAMIN 50

typedef struct ImagenGris{
unsigned char **pixel;
int alto;
int ancho;
}IMAGENGRIS;

/*PIR: Estructura que contiene un nodo de la pirámide.
H,S,I - niveles H, S e I del nodo.
Hor,Sor,Ior - colores de la generación de la pirámide
Area- area de la region que corresponde al nodo en la base.
X,Y - coordenadas del nodo padre de este nodo.
Homog - indica si el nodo es homogéneo en color.
Clase- clase a la que corresponde el nodo.
*/
typedef struct PiramideGris{
 int Homog;
 int X;
 int Y;
 unsigned char I;
 int Area;
 int Clase;
 std::vector<int> contactosV;
 std::vector<PUNTO> contactosR; //Para los casos especiales en los que dos nodos regulares
                             //estan en contacto y no son vecinos espacialmente
}PIRGRIS;

/*Padre: estructura para almacenar las características de los
nodos padres de la estructura.
posi,posj - posición del nodo padre, si es un nodo basico de la estructura.
Niv - nivel del nodo padre.
NodoV - si el padre es un nodo virtual de primer orden, aquí se almacena
su posición en el array de nodos virtuales de primer orden.
H,S,I - valores H, S e I del nodo padre.
Area - area de la region que corresponde al nodo en la base.*/

typedef struct padre{
	double posi;
        double posj;
        int Niv;
	int NodoV;
        int I;
        int Area;
}Padre;

/*NV: Estructura para almacenar las características de los nodos virtuales.
H,S,I - valores H, S, e I del nodo.
Clase - clase a la que pertenece el nodo.
Area - area de la region que corresponde al nodo en la base.*/

typedef struct nv{
        unsigned char I;
        int Clase;
        int Area;
        long int padre;
        int ClaseObj;
        std::vector<int> contactos;
        std::vector<PUNTO> contactosR;
        
}NV;

/*DIST: Estructura que amacena la distancia en color entre dos nodos .
dif - distancia en color.
x,y - coordenadas de uno de los nodos que se están comparando.*/

typedef struct Dist{
       double dif;
       int x;
       int y;
}DIST;

/* BOX: Estructura para almacenar las bounding box de las clases.
imin,imax,jmin,jmax- coordenadas de los vertices de la caja.
H,S,I- valores H,S e I de la clase encerrada en la caja.
area- area de la clase.
icent,jcent- coordenadas del centroide de la caja
areanivel-  area en ese nivel
encontrada- marca si la region encerrada en la caja ha sido ya encontrada en
el template matching*/

typedef struct BoxGris{
 int imin;
 int imax;
 int jmin;
 int jmax;
 int I;
 int area;
 int sumi;
 int sumj;
 float icent;
 float jcent;
 double contraste;
 int pm; //perimetro
 double TR; //thinness ratio
 double IM; //invariant moment
 int clase;
 double X;
 double Y;
 double Z;
}BOXGRIS;


/*FUS: Para controlar la fusion de clases en la base.
clase: nuevo valor de clase resultante de la fusion.
H,S,I: valores H,S e I de la nueva clase que se genera.*/

typedef struct Fus{
 int clase;
 unsigned char I;
 int Area;
}FUS;

struct TRACKERGRIS{
  IMAGENGRIS im;
  IMAGENGRIS imdispar;
  IMAGENGRIS silueta;
  inline  TRACKERGRIS();

};

inline TRACKERGRIS::TRACKERGRIS(){

register int i;
imdispar.alto = ALTO*2;
imdispar.ancho = ANCHO*2;
silueta.alto = ALTO*2;
silueta.ancho = ANCHO*2;

im.pixel=new unsigned char*[im.alto];
for(i=0;i<im.alto;i++)
   im.pixel[i]=new unsigned char [im.ancho*3];

imdispar.pixel=new unsigned char*[imdispar.alto];
for(i=0;i<imdispar.alto;i++)
   imdispar.pixel[i]=new unsigned char [imdispar.ancho];

silueta.pixel=new unsigned char*[silueta.alto];
for(i=0;i<silueta.alto;i++)
   silueta.pixel[i]=new unsigned char [silueta.ancho*3];
}

inline unsigned char DisparidadMediaGris(TRACKERGRIS *track, int imin,int imax, int jmin, int jmax)
{
  unsigned char media;

  //if ( (indice_region<0)||(indice_region>=NUMMARCS) )
  // {
  //  cerr<<"ERROR EN DisparidadMediaGris (tracker/Silueta.cpp). Indice de región no válido"<<endl;
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

inline void UmbralizarDisparidadGris(IMAGENGRIS *in, unsigned char **imagen_out, int umbral_adelante, int umbral_atras, unsigned char disparidad_media)
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

inline bool DistanciaGris(int umbral, int g1,  int g2)
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

inline int FuegoGris(unsigned char **in_image, unsigned char ** out_image,int fil, int col,int num,
int N1, int M1, int N2, int M2, int umbral){

int i,j;
bool valido;

out_image[fil][col]=1;

 for (i=-1; i<=1; i++)
   for (j=-1; j<=1; j++)
     if (((fil+i)>=0) && ((fil+i)<N2) && ((col+j)>=0) && ((col+j)<M2)){
       // if((i!=0)&&(j!=0)){
     if(out_image[fil+i][col+j]==0){
       valido=DistanciaGris(umbral,in_image[fil][col],in_image[i+fil][col+j]);
       if (!valido){
         FuegoGris(in_image,out_image,fil+i,col+j,num,N1,M1,N2,M2,umbral);
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
inline void PrenderFuegoRecursivoGris(unsigned char **in_image, unsigned char **out_image,
			   int NS, int N1, int M1, int N2, int M2, int umbral)
{
int i,j,num;

  num=0;
  for (i=N1; i<N2; i++)
   for (j=M1; j<M2; j++)
     {
       FuegoGris(in_image,out_image,i,j,num+1,N1,M1,N2,M2,umbral);
       num++;
       if (num>=NS) return;
     }
}
 
//-------------------------
inline void ObtenerSiluetaGris(TRACKERGRIS *track, int umbral_palante, int umbral_patras, int imin, int imax, int jmin, int jmax)
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
  disp_media = DisparidadMediaGris(track, imin,imax,jmin,jmax);
  //cout<<"DispMedia: "<<(int)disp_media<<endl;
  // Comprobación de errores
  if (disp_media == 0) //disp_media=100;
     return;
  // Umbraliza la disparidad en torno al valor de disparidad de la cara. El resultado se 
  // guarda en aux
  UmbralizarDisparidadGris(&track->imdispar, aux, umbral_palante, umbral_patras, disp_media);

 // Selecciona la silueta
  PrenderFuegoRecursivoGris(aux, quemado, 1, 
			(imax+imin)/2, (jmin+jmax)/2,
  	track->silueta.alto, track->silueta.ancho, 1);
  
  for (i=0;i<track->silueta.alto;i++)
    for(j=0;j<track->silueta.ancho;j++){
      if (quemado[i][j]==1){
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

  for(i=0;i<track->silueta.alto;i++)
    {
      delete aux[i];
      delete quemado[i];
    }
  delete aux;  
  delete quemado;

}



#endif

