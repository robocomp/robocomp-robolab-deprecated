#include <math.h>
#include <stdio.h>

#include <tracker/Clasificacion.h>

#include <tracker/Config.h>



void Dilatacion(unsigned char **im,unsigned char **aux, int alto, int ancho){
unsigned char s;
int i,j,k,l;


for(k=1;k<alto-1;k++){
   for(l=1;l<ancho-1;l++){
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

void Erosion(unsigned char **im,unsigned char **aux, int alto, int ancho){
unsigned char s;
int i,j,k,l;


for(k=1;k<alto-1;k++){
   for(l=1;l<ancho-1;l++){
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

void Clasificacion(IMAGEN *im, unsigned char **seg,int alto, int ancho, double umbral, int umbral_l){

register int i,j;
double Taux, Saux;
double r,g;
unsigned char R,G,B;
double t,s,l,distancia;
unsigned char **aux;

aux= new unsigned char* [alto];
for (i=0;i<alto; i++)
  aux[i]=new unsigned char [ancho];

for (i=0;i<alto;i++)
        for(j=0;j<ancho;j++){

            R=im->pixel[i][j*3];
            G=im->pixel[i][j*3+1];
            B=im->pixel[i][j*3+2];
        

            if (R==0&& G==0 && B==0){
            t=0;
            s=0;
            l=0;
            }
           else{
            r=((double)R/((double)(R+G+B)))-1.0/3.0;
            g=((double)G/((double)(R+G+B)))-1.0/3.0;

            s=sqrt((9.0/5.0)*(r*r+g*g));

            if (g>0)
                t=atan(r/g)/(double)(2*3.1416)+(1.0/4.0);
            else if (g<0)
                t=atan(r/g)/(double)(2*3.1416)+(3.0/4.0);
            else t=0;

            l=0.299*R+0.587*G+0.114*B;
           }
           t=(t*255);
           s=(s*255);

           im->T[i][j]=t;
           im->S[i][j]=s;
           im->L[i][j]=l;

           Taux= t-MT;
           Saux= s-MS;
           distancia=(Taux*(Taux*CTT + Saux*CTS))+(Saux*(Taux*CTS + Saux*CSS));
           if(distancia<=umbral && l>0 && s>0) // l>80 && s>10) normalmente,  l>0 && s>0) para Surrey 
                seg[i][j]=255;
           else
                seg[i][j]=0;
        }



#if ERODEDILATE
Erosion(seg,aux, alto,ancho);
Dilatacion(aux,seg,alto,ancho);
#endif

for(i=0;i<alto;i++)
  delete[] aux[i];
delete aux;

/* dummy */

}

