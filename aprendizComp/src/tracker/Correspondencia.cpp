#include <tracker/Correspondencia.h>
#include <stdlib.h>
#include <math.h>
int *rango;
BOX *boxder;

#include <iostream>
using std::cout;
using std::endl;

void BuscarMaximos(int ***busqueda,int tam,int nivel,int clase,PUNTO *p,PUNTO *p1){

register int i;
int i1,j1,i2,j2;

i1=0;
j1=0;
i2=0;
j2=0;

for(i=0;i<tam;i++){
   if((busqueda[nivel][clase][i]>=i1)){
      i2=i1;
      j2=j1;
      i1=busqueda[nivel][clase][i];
      j1=i;
   }else if (busqueda[nivel][clase][i]>i2){
      i2=busqueda[nivel][clase][i];
      j2=i;
   }
}
p->i=i1;
p->j=j1;
p1->i=i2;
p1->j=j2;
}


void MarcarAbajo(PIR ***pirright,BOX **boxnivel,int nivel, int *tam, int *clases){

register int i,j,l;
int k,a1,b,c,d;
int clase;
int i1,j1,i2,j2;
k=nivel;

  for(l=0;l<NUMMARCS;l++){
  clase=clases[l];
  a1=boxnivel[k][l].imin-rango[k];
  if(a1<0)a1=0;
  b=boxnivel[k][l].imax+rango[k];
  if(b>=tam[k])b=tam[k]-1;
  for(i=a1;i<=b;i++){
    c=boxnivel[k][l].jmin-rango[k];
    if(c<0)c=0;
    d=boxnivel[k][l].jmax+rango[k];
    if(d>=tam[k])d=tam[k]-1;

    i1=i*2;
    i2=i1+1;

    for(j=c;j<=d;j++){
       if(pirright[k][i][j].Homog==1){

          j1=j*2;
          j2=j1+1;

          if((pirright[k][i][j].Clase==clase)
         &&(pirright[k-1][i1][j1].Clase==-1||pirright[k-1][i1][j1].Clase==clase)
         &&(pirright[k-1][i1][j2].Clase==-1||pirright[k-1][i1][j2].Clase==clase)
         &&(pirright[k-1][i2][j1].Clase==-1||pirright[k-1][i2][j1].Clase==clase)
         &&(pirright[k-1][i2][j2].Clase==-1||pirright[k-1][i2][j2].Clase==clase))
         {

          pirright[k-1][i1][j1].Clase=clase;
          pirright[k-1][i1][j2].Clase=clase;
          pirright[k-1][i2][j1].Clase=clase;
          pirright[k-1][i2][j2].Clase=clase;


       }
       }
   }
  }
}

}

void Correspondencia(TRACKER *track, int sec){

register int i,j,k,l,m,des,cont;
int a,b,c,d;
int porcentaje;
PUNTO p,p1;
int aux;
int x,y;
bool fin=false;
int clase;
int ***busqueda;

BOX **boxnivel;
PIR ***pirleft;
PIR ***pirright;

boxnivel=track->boxnivel;
pirleft=track->pir[sec];
pirright=track->pirder;

//Reserva de memoria de la matriz de b�squeda
busqueda=new int** [MAXNIVEL+1];
for(i=0;i<MAXNIVEL+1;i++){
   busqueda[i]=new int* [NUMMARCS];
   for(j=0;j<NUMMARCS;j++){
     busqueda[i][j]=new int [MAXDISP+1];
   }
}


//Inicializaci�n de la matriz de b�squeda
for(k=0;k<MAXNIVEL+1;k++){
  for(i=0;i<NUMMARCS;i++){
     for(j=0;j<=MAXDISP;j++){
       busqueda[k][i][j]=0;
     }
  }
}

cont=0;

rango= new int [MAXNIVEL+1];

rango[0]=MAXDISP;
rango[1]=rango[0]/2;
rango[2]=rango[1]/2;
if(TAMGRANDE)
        rango[3]=rango[2]/2;

for(k=MAXNIVEL;k>-1&& fin==false;k--){
fin=true;
for(i=0;i<NUMMARCS ;i++){
   p.j=-1;
   clase=track->clases[i];
  if(boxnivel[k][i].en_estereo==false){
      fin=false;

     if(boxnivel[k][i].areanivel>2){
            for(des=0;des<=rango[k]&&boxnivel[k][i].en_estereo==false;des++){

               a=boxnivel[k][i].imin;
               b=boxnivel[k][i].imax;
               c=boxnivel[k][i].jmin-des;
               if(c<0) c=0;
               d=boxnivel[k][i].jmax-des;
               if(d<0) d=0;

               cont=0;
             if(a!=b){
               for(l=a;l<=a+((b-a)/2);l++){
                  for(m=c;m<=d;m++){
                     if((pirright[k][l][m].Homog==1)&&(pirright[k][l][m].Clase==-1)
                       && (pirleft[k][l][m+des].Homog==1)&&(pirleft[k][l][m+des].Clase==clase)
                       &&((pirright[k][l][m].X==-1)||
                         (pirright[k+1][pirright[k][l][m].X][pirright[k][l][m].Y].Clase==-1))){

                        cont++;
                     }/*if*/
                  }/*for*/
               }/*for*/
               aux=(int)(cont*100/boxnivel[k][i].areanivel);
              if (aux>90){
                 busqueda[k][i][des]=cont*100/boxnivel[k][i].areanivel;
                 boxnivel[k][i].en_estereo=true;
                 p.j=0;
                 p.i=100;
              }else if (aux<70){
               if(aux>=20 || k==0){
                for(l=a+((b-a)/2)+1;l<=b;l++){
                  for(m=c;m<=d;m++){
                     if((pirright[k][l][m].Homog==1)&&(pirright[k][l][m].Clase==-1)
                       && (pirleft[k][l][m+des].Homog==1)&&(pirleft[k][l][m+des].Clase==clase)
                       &&((pirright[k][l][m].X==-1)||
                         (pirright[k+1][pirright[k][l][m].X][pirright[k][l][m].Y].Clase==-1))){


                          cont++;
                     }/*if(homog)*/
                  }/*for*/
               } /*for*/
                 busqueda[k][i][des]=(int)(cont*100/boxnivel[k][i].areanivel);
              }else{
                busqueda[k][i][des]=0;
              }/*if(aux>20)*/
            }/*if(aux<70)*/
            else
               busqueda[k][i][des]=(int)(cont*100/boxnivel[k][i].areanivel);
          }else{
              for(l=a;l<=b;l++){
                  for(m=c;m<=c+((d-c)/2);m++){
                     if((pirright[k][l][m].Homog==1)&&(pirright[k][l][m].Clase==-1)
                       && (pirleft[k][l][m+des].Homog==1)&&(pirleft[k][l][m+des].Clase==clase)
                       &&((pirright[k][l][m].X==-1)||
                         (pirright[k+1][pirright[k][l][m].X][pirright[k][l][m].Y].Clase==-1))){

                        cont++;
                     }/*if*/
                  }/*for*/
               }/*for*/
               aux=(int)(cont*100/boxnivel[k][i].areanivel);
              if (aux>90){
                 busqueda[k][i][des]=cont*100/boxnivel[k][i].areanivel;
                 boxnivel[k][i].en_estereo=true;
                 p.j=0;
                 p.i=100;
              }else if (aux<70){
               if(aux>=20 || k==0){
                for(l=a;l<=b;l++){
                  for(m=c+((d-c)/2)+1;m<=d;m++){
                     if((pirright[k][l][m].Homog==1)&&(pirright[k][l][m].Clase==-1)
                       && (pirleft[k][l][m+des].Homog==1)&&(pirleft[k][l][m+des].Clase==clase)
                       &&((pirright[k][l][m].X==-1)||
                         (pirright[k+1][pirright[k][l][m].X][pirright[k][l][m].Y].Clase==-1))){

                          cont++;
                     }/*if(homog)*/
                  }/*for*/
               } /*for*/
                 busqueda[k][i][des]=(int)(cont*100/boxnivel[k][i].areanivel);
              }else{
                  busqueda[k][i][des]=0;
              }/*if(aux>20)*/
            }/*if(aux<70)*/
           else
               busqueda[k][i][des]=(int)(cont*100/boxnivel[k][i].areanivel);
          }/*if(a!=b)*/
            }/*for(des)*/

            if(p.j==-1){
            BuscarMaximos(busqueda,rango[k]+1,k,i,&p,&p1);
           }
           if (p.i>=70 ||(k==0 && p.i>0)){

                boxnivel[k][i].en_estereo=true;
                if(k>0)
                boxnivel[k-1][i].en_estereo=true;

               c=boxnivel[k][i].jmin-(int)p.j;
               if(c<0) c=0;
               d=boxnivel[k][i].jmax-(int)p.j;
               if(d<0) d=0;

               for(l=a;l<=b;l++){
                  for(m=c;m<=d;m++){

                     if((pirright[k][l][m].Homog==1)&&(pirright[k][l][m].Clase==-1)
                       && (pirleft[k][l][m+(int)p.j].Homog==1)&&(pirleft[k][l][m+(int)p.j].Clase==clase)
                       &&((pirright[k][l][m].X==-1)||
                         (pirright[k+1][pirright[k][l][m].X][pirright[k][l][m].Y].Clase==-1))){

                          pirright[k][l][m].Clase=clase;


                     }/*if*/
                  }/*for*/
               }/*for*/

            }/*if (p.i>70)*/

        //Tengo que encontrar el maximo y si es !=0 marcar la caja como encontrada
        //irme a la posicion de la caja y clasificar los nodos q cumplen los requisitos
      //aki si la encuentro tb tengo q marcarla como encontrada abajo

     }/*if (PORAREA)*/

  }else {
      if (k>0)
          boxnivel[k-1][i].en_estereo=true;

  }/*if(no encontrada)*/

}/*for (clase)*/

if(k>0){

   MarcarAbajo(pirright,boxnivel,k,track->TAMNiveles, track->clases);
}


}/*for (k)*/


//Marcamos hacia arriba

for(k=1;k<MAXNIVEL+1;k++){
  for(i=0;i<track->TAMNiveles[k];i++){
     for(j=0;j<track->TAMNiveles[k];j++){
        if(pirright[k][i][j].Homog==1&&pirright[k][i][j].Clase==-1){
                if(pirright[k-1][i*2][j*2].Clase!=-1&&pirright[k-1][i*2][j*2+1].Clase!=-1&&
                   pirright[k-1][i*2+1][j*2].Clase!=-1&&pirright[k-1][i*2+1][j*2+1].Clase!=-1){
                     pirright[k][i][j].Clase=pirright[k-1][i*2][j*2].Clase;
                }
        }
     }
  }
}
 if(TAMGRANDE)
        HermanamientoNivelEstereo(pirright,boxnivel,track->TAMNiveles,3,track->clases);
HermanamientoNivelEstereo(pirright,boxnivel,track->TAMNiveles,2,track->clases);
HermanamientoNivelEstereo(pirright,boxnivel,track->TAMNiveles,1,track->clases);
HermanamientoNivelEstereo(pirright,boxnivel,track->TAMNiveles,0,track->clases);


for(k=1;k<MAXNIVEL+1;k++){
  for(i=0;i<track->TAMNiveles[k];i++){
     for(j=0;j<track->TAMNiveles[k];j++){
        if(pirright[k][i][j].Homog==1&&pirright[k][i][j].Clase==-1){
                if(pirright[k-1][i*2][j*2].Clase!=-1&&pirright[k-1][i*2][j*2+1].Clase!=-1&&
                   pirright[k-1][i*2+1][j*2].Clase!=-1&&pirright[k-1][i*2+1][j*2+1].Clase!=-1){
                     pirright[k][i][j].Clase=pirright[k-1][i*2][j*2].Clase;
                }
        }
     }
  }
}
if(TAMGRANDE)
        HermanamientoInversoNivelEstereo(pirright,boxnivel,track->TAMNiveles,3,track->clases);
HermanamientoInversoNivelEstereo(pirright,boxnivel,track->TAMNiveles,2,track->clases);
HermanamientoInversoNivelEstereo(pirright,boxnivel,track->TAMNiveles,1,track->clases);
HermanamientoInversoNivelEstereo(pirright,boxnivel,track->TAMNiveles,0,track->clases);



}



void HermanamientoNivelEstereo(PIR ***pirright, BOX **boxnivel, int *tam, int nivel, int *clases){

int i,j,k,l,m,a1,b,c,d;
double distmin;
int tamano;
int clase;

k=nivel;

tamano=tam[k];
for(m=0;m<NUMMARCS;m++){
 clase=clases[m];

a1=boxnivel[k][m].imin-rango[k];
if(a1<0)a1=0;
 b=boxnivel[k][m].imax+rango[k];
if(b>=tamano)b=tamano-1;

for (i=a1;i<=b;i++){
    c=boxnivel[k][m].jmin-rango[k];
    if(c<0)c=0;
    d=boxnivel[k][m].jmax+rango[k];
    if(d>=tamano)d=tamano-1;
    for(j=c;j<=d;j++){

       if ((pirright[k][i][j].Homog==1)&&(pirright[k][i][j].Clase==-1)
           &&((pirright[k][i][j].X==-1)||
           (pirright[k+1][pirright[k][i][j].X][pirright[k][i][j].Y].Clase==-1)))
           {

          if(((j-1)>=c)&&(pirright[k][i][j-1].Homog==1)
             &&(pirright[k][i][j-1].Clase==clase)){
              pirright[k][i][j].Clase=clase;
          }


          else if(((j-1)>=c)&&((i-1)>=a1)&&(pirright[k][i-1][j-1].Homog==1)
              &&(pirright[k][i-1][j-1].Clase==clase)){
              pirright[k][i][j].Clase=clase;
          }

          else if(((i-1)>=a1)&&(pirright[k][i-1][j].Homog==1)
             &&(pirright[k][i-1][j].Clase==clase)){
              pirright[k][i][j].Clase=clase;
          }
          else if(((i-1)>=a1)&&((j+1)<=d)&&(pirright[k][i-1][j+1].Homog==1)
             &&(pirright[k][i-1][j+1].Clase==clase)){
              pirright[k][i][j].Clase=clase;
          }
          else if(((j+1)<=d)&&(pirright[k][i][j+1].Homog==1)
            &&(pirright[k][i][j+1].Clase==clase)){
              pirright[k][i][j].Clase=clase;
          }
          else if(((j+1)<=d) &&((i+1)<=b)&&(pirright[k][i+1][j+1].Homog==1)
             &&(pirright[k][i+1][j+1].Clase==clase)){
              pirright[k][i][j].Clase=clase;
          }
          else if(((i+1)<=b)&&(pirright[k][i+1][j].Homog==1)
             &&(pirright[k][i+1][j].Clase==clase)){
              pirright[k][i][j].Clase=clase;
          }
          else if(((j-1)>=c)&&((i+1)<=b)&&(pirright[k][i+1][j-1].Homog==1)
             &&(pirright[k][i+1][j-1].Clase==clase)){
              pirright[k][i][j].Clase=clase;
          }


    }
  }
 }
}
if(k>0)
  MarcarAbajo(pirright,boxnivel,k,tam,clases);
}

void HermanamientoInversoNivelEstereo(PIR ***pirright, BOX **boxnivel, int *tam, int nivel, int *clases){

int i,j,k,l,m,a1,b,c,d;
double distmin;
int tamano;
int clase;

k=nivel;

tamano=tam[k];
for(m=0;m<3;m++){
 clase=clases[m];

a1=boxnivel[k][m].imin-rango[k];
if(a1<0)a1=0;
 b=boxnivel[k][m].imax+rango[k];
if(b>=tamano)b=tamano-1;

for (i=b;i>=a1;i--){
    c=boxnivel[k][m].jmin-rango[k];
    if(c<0)c=0;
    d=boxnivel[k][m].jmax+rango[k];
    if(d>=tamano)d=tamano-1;
    for(j=d;j>=c;j--){

        if ((pirright[k][i][j].Homog==1)&&(pirright[k][i][j].Clase==-1)
           &&((pirright[k][i][j].X==-1)||
           (pirright[k+1][pirright[k][i][j].X][pirright[k][i][j].Y].Clase==-1)))
           {

          if(((j-1)>=c)&&(pirright[k][i][j-1].Homog==1)
             &&(pirright[k][i][j-1].Clase==clase)){
              pirright[k][i][j].Clase=clase;
          }


          else if(((j-1)>=c)&&((i-1)>=a1)&&(pirright[k][i-1][j-1].Homog==1)
              &&(pirright[k][i-1][j-1].Clase==clase)){
              pirright[k][i][j].Clase=clase;
          }

          else if(((i-1)>=a1)&&(pirright[k][i-1][j].Homog==1)
             &&(pirright[k][i-1][j].Clase==clase)){
              pirright[k][i][j].Clase=clase;
          }
          else if(((i-1)>=a1)&&((j+1)<=d)&&(pirright[k][i-1][j+1].Homog==1)
             &&(pirright[k][i-1][j+1].Clase==clase)){
              pirright[k][i][j].Clase=clase;
          }
          else if(((j+1)<=d)&&(pirright[k][i][j+1].Homog==1)
            &&(pirright[k][i][j+1].Clase==clase)){
              pirright[k][i][j].Clase=clase;
          }
          else if(((j+1)<=d) &&((i+1)<=b)&&(pirright[k][i+1][j+1].Homog==1)
             &&(pirright[k][i+1][j+1].Clase==clase)){
              pirright[k][i][j].Clase=clase;
          }
          else if(((i+1)<=b)&&(pirright[k][i+1][j].Homog==1)
             &&(pirright[k][i+1][j].Clase==clase)){
              pirright[k][i][j].Clase=clase;
          }
          else if(((j-1)>=c)&&((i+1)<=b)&&(pirright[k][i+1][j-1].Homog==1)
             &&(pirright[k][i+1][j-1].Clase==clase)){
              pirright[k][i][j].Clase=clase;
          }


    }
  }
 }
}
if(k>0)
  MarcarAbajo(pirright,boxnivel,k,tam,clases);
}

void SacarABC(double *cont, int disp, double *a, double *b, double *c){
register int i,j;
double y0, y1, y2;
int x0,x1,x2;
double aux1, aux2, aux3,aux4;



x0=disp-1;
x1=disp;
x2=disp+1;

y0=cont[x0];
y1=cont[x1];
y2=cont[x2];

aux1=((double)(x1-x0)/(double)(x1*x1-x0*x0));
aux2=(x0*x0*aux1)-(x2*x2*aux1)+x2-x0;

aux3=x1*x1-x0*x0;
aux4=y2-((double)(y1*x2*x2)/(double)(aux3))+((double)(y0*x2*x2)/(double)(aux3))-
        y0+((double)(y1*x0*x0)/(double)(aux3))-((double)(y0*x0*x0)/(double)(aux3));

*b=aux4/aux2;

*a=((double)(y1-*b*(x1-x0)-y0)/aux3);

*c=y0-*a*x0*x0-*b*x0;
}


void SacarXYZ( float *x, float *y,float *z, float disp, float icent, float jcent){

float cci1,fci1,ccd1,fcd1,cci2,fci2;

if(TAMGRANDE){
  cci1=CCI1;
  fci1=FCI1;
  ccd1=CCD1;
  fcd1=FCD1;
  cci2=CCI2;
  fci2=FCI2;
}else{
  cci1=CCI1/2;
  fci1=FCI1/2;
  ccd1=CCD1/2;
  fcd1=FCD1/2;
  cci2=CCI2/2;
  fci2=FCI2/2;

}
*z=DISTANCIA/(((jcent-cci1)/fci1)-((jcent-disp-ccd1)/fcd1));
*x=(*z*(jcent-cci1))/fci1;
*y=(*z*(icent-cci2))/fci2;

}

double DisColor(unsigned char T,unsigned char S,unsigned char L,
                unsigned char T1,unsigned char S1,unsigned char L1){

double d;
double di,dc;
double H1,H2;
double teta;

H1=T*360/255;
H2=T1*360/255;

if (abs((int)H1-(int)H2)<180) teta=(double)(abs((int)H1-(int)H2));
else teta=360-(double)(abs((int)H1-(int)H2));

teta=(teta*3.1415)/180;

di= (double)abs(L-L1);
dc= sqrt((double)(S*S)+(double)(S1*S1)-
          (2*S*S1*cos(teta)));

d=sqrt((di*di)+(dc*dc));

return d;
}

void Refinar(TRACKER *track){

register int i,j,k,l,m;
int imin,imax,jmin,jmax,des;
double **cont;
cont= new double*[3];
for(i=0;i<3;i++)
        cont[i]=new double[13];
int cont1;
double min;
int disp;
register int n;
double a,b,c;
double redisp;
float aux;

for(k=0;k<NUMMARCS;k++){

if(track->boxnivel[0][k].jmin>3&&track->boxnivel[0][k].jmax<ANCHO-1&&boxder[k].jmin>3&&boxder[k].jmax<ANCHO-1){
   des=track->disparidad[k];

   for(m=0;m<13;m++)
      cont[k][m]=0.0;
   n=0;
   for(l=des-6;l<=des+6;l++){

        imin=track->boxnivel[0][k].imin;
        imax=track->boxnivel[0][k].imax;
        jmin=track->boxnivel[0][k].jmin-l;
        if (jmin<0) jmin=0;
        else if (jmin>=ANCHO) jmin=ANCHO;
        jmax=track->boxnivel[0][k].jmax-l;
        if (jmax<0) jmax=0;
        else if (jmax>=ANCHO) jmax=ANCHO;

        cont1=0;
        for(i=imin;i<=imax;i++)
                for(j=jmin;j<=jmax;j++){
                   if(track->seg.pixel[i][j+l]==255/*&&track->segder.pixel[i][j]==255*/){
                        cont[k][n]=cont[k][n]+DisColor(track->im.T[i][j+l],track->im.S[i][j+l],
                                        track->im.L[i][j+l],track->imder.T[i][j],
                                        track->imder.S[i][j],track->imder.L[i][j]);
                        cont1++;
                   }
                }
        n++;
   } //for l=des-3...
   min=100000.0;
   //int disp1,aux;
   for(i=0;i<13;i++){
      if(cont[k][i]<min){
         min=cont[k][i];
         disp=i;
      }
   }

 track->disparidad[k]=des+disp-6;
 track->prob[k]=min/cont1;

#if APROXIMACION_POLINOMIAL
 //Aproximacion polinomial
 SacarABC(cont[k],disp,&a,&b,&c);
 redisp=-1*(b/(2*a));
 if (2*a>0)
        track->disparidad[k]=des+redisp-6;
#endif

if(track->disparidad[k]==track->dispold2[k])
        track->dispold1[k]=track->disparidad[k];

track->dispnew[k]= (track->disparidad[k]+track->dispold1[k]+track->dispold2[k]+track->dispold3[k]+track->dispold4[k]+track->dispold5[k]+track->dispold6[k]+track->dispold7[k]+track->dispold8[k]+track->dispold9[k])/10;
 
SacarXYZ(&(track->x[k]),&(track->y[k]),&(track->z[k]),track->dispnew[k],track->boxnivel[0][k].icent,track->boxnivel[0][k].jcent);

   aux=track->dispold1[k],
   track->dispold1[k]=track->disparidad[k];
   track->dispold9[k]=track->dispold8[k];
   track->dispold8[k]=track->dispold7[k];
   track->dispold7[k]=track->dispold6[k];
   track->dispold6[k]=track->dispold5[k];
   track->dispold5[k]=track->dispold4[k];
   track->dispold4[k]=track->dispold3[k];
   track->dispold3[k]=track->dispold2[k];
   track->dispold2[k]=aux;

}
}
delete(boxder);
#if NORMALIZACION
if (track->z[1]>track->z[0])
        track->z[1]=track->z[0];
if (track->z[2]>track->z[0])
        track->z[2]=track->z[0];
#endif
}

void Disparidad (PIR ***pirder, BOX *boxizq, int *clases, float *disparidad){

register int i,j,m,l;
int num,clase;

boxder= new BOX [NUMMARCS];

  for(j=0;j<NUMMARCS;j++){
    boxder[j].imin=ALTO;
    boxder[j].jmin=ANCHO;
    boxder[j].imax=0;
    boxder[j].jmax=0;
    boxder[j].areanivel=0;
    boxder[j].sumi=0;
    boxder[j].sumj=0;
    boxder[j].icent=-1;
    boxder[j].jcent=-1;
  }

for(i=0;i<ALTO;i++){
  for(j=0;j<ANCHO;j++){
   clase=pirder[0][i][j].Clase;
   num=-1;

  for (m=0;m<NUMMARCS; m++){
   if(clase==clases[m])
        num=m;
  }
   if(num!=-1){

     boxder[num].sumi=boxder[num].sumi+i;
     boxder[num].sumj=boxder[num].sumj+j;

     boxder[num].areanivel=boxder[num].areanivel+1;

     if(boxder[num].imin>i)
         boxder[num].imin=i;

     if(boxder[num].jmin>j)
         boxder[num].jmin=j;

     if(boxder[num].imax<i)
         boxder[num].imax=i;

     if(boxder[num].jmax<j)
         boxder[num].jmax=j;

   }
  }
}
for(l=0;l<NUMMARCS;l++){
  boxder[l].icent=boxder[l].sumi/boxder[l].areanivel;//(boxder[l].imin+boxder[l].imax)/2;
  boxder[l].jcent=boxder[l].sumj/boxder[l].areanivel;//(boxder[l].jmin+boxder[l].jmax)/2;
}

for(i=0;i<NUMMARCS;i++){

        disparidad[i]=boxder[i].jcent-boxizq[i].jcent;
        if(disparidad[i]<0) disparidad[i]=-disparidad[i];

}
}

