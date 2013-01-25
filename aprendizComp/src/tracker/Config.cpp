#include <math.h>
//#include <cmath>
#include <iostream>
#include <tracker/Config.h>


TRACKER::TRACKER(){

register int i,j,k,l;
int aux1,aux2;

im.alto=ALTO;
im.ancho=ANCHO;
seg.alto=ALTO;
seg.ancho=ANCHO;
imder.alto=ALTO;
imder.ancho=ANCHO;
segder.alto=ALTO;
segder.ancho=ANCHO;
imdispar.alto = ALTO*2;
imdispar.ancho = ANCHO*2;
silueta.alto = ALTO*2;
silueta.ancho = ANCHO*2;
esqueleto.alto = ALTO*2;
esqueleto.ancho = ANCHO*2;

aux1=(int)((log10((long double)ALTO)/log10((long double)2)));
aux2=(int)((log10((long double)ANCHO)/log10((long double)2)));

if (aux1>aux2) niveles=aux1;
else niveles=aux2;

im.pixel=new unsigned char*[im.alto];
for(i=0;i<im.alto;i++)
   im.pixel[i]=new unsigned char [im.ancho*3];

im.T=new unsigned char*[im.alto];
for(i=0;i<im.alto;i++)
   im.T[i]=new unsigned char [im.ancho];

im.S=new unsigned char*[im.alto];
for(i=0;i<im.alto;i++)
   im.S[i]=new unsigned char [im.ancho];

im.L=new unsigned char*[im.alto];
for(i=0;i<im.alto;i++)
   im.L[i]=new unsigned char [im.ancho];

seg.pixel=new unsigned char*[im.alto];
for(i=0;i<im.alto;i++)
   seg.pixel[i]=new unsigned char [im.ancho];

imder.pixel=new unsigned char*[im.alto];
for(i=0;i<im.alto;i++)
   imder.pixel[i]=new unsigned char [im.ancho*3];

imder.T=new unsigned char*[im.alto];
for(i=0;i<im.alto;i++)
   imder.T[i]=new unsigned char [im.ancho];

imder.S=new unsigned char*[im.alto];
for(i=0;i<im.alto;i++)
   imder.S[i]=new unsigned char [im.ancho];

imder.L=new unsigned char*[im.alto];
for(i=0;i<im.alto;i++)
   imder.L[i]=new unsigned char [im.ancho];

segder.pixel=new unsigned char*[im.alto];
for(i=0;i<im.alto;i++)
   segder.pixel[i]=new unsigned char [im.ancho];

imdispar.pixel=new unsigned char*[imdispar.alto];
for(i=0;i<imdispar.alto;i++)
   imdispar.pixel[i]=new unsigned char [imdispar.ancho];

silueta.pixel=new unsigned char*[silueta.alto];
for(i=0;i<silueta.alto;i++)
   silueta.pixel[i]=new unsigned char [silueta.ancho*3];

esqueleto.pixel=new unsigned char*[esqueleto.alto];
for(i=0;i<esqueleto.alto;i++)
   esqueleto.pixel[i]=new unsigned char [esqueleto.ancho];

TAMNiveles_i= new int [niveles];
TAMNiveles_j= new int [niveles];

TAMNiveles_i[0]=seg.alto;
for(i=1;i<niveles;i++)
  TAMNiveles_i[i]=TAMNiveles_i[i-1]/2;

TAMNiveles_j[0]=seg.ancho;
for(i=1;i<niveles;i++)
  TAMNiveles_j[i]=TAMNiveles_j[i-1]/2;


pir=new PIR*** [2];
for(k=0;k<2;k++){
  pir[k]=new PIR** [niveles];
  for(i=0;i<niveles;i++){
   pir[k][i]=new PIR* [TAMNiveles_i[i]];
   for(j=0;j<TAMNiveles_i[i];j++)
     pir[k][i][j]=new PIR [TAMNiveles_j[i]];
  }
}

pirder=new PIR** [niveles];

for(i=0;i<niveles;i++){
   pirder[i]=new PIR* [TAMNiveles_i[i]];
   for(j=0;j<TAMNiveles_i[i];j++)
     pirder[i][j]=new PIR [TAMNiveles_j[i]];
}

clases=new int [NUMMARCS];

nivel= new int [NUMMARCS];

mov_v=new int*[MAXNIVEL+1];
mov_h=new int*[MAXNIVEL+1];
for(i=0;i<MAXNIVEL+1;i++){
  mov_v[i]=new int [NUMMARCS];
  mov_h[i]=new int [NUMMARCS];
}

boxnivel= new BOX* [MAXNIVEL+1];
for(i=0;i<MAXNIVEL+1;i++)
   boxnivel[i]= new BOX [NUMMARCS];

boxAnt= new BOX* [MAXNIVEL+1];
for(i=0;i<MAXNIVEL+1;i++)
   boxAnt[i]= new BOX [NUMMARCS];

mask_ant=new MASCARA***[NUMMARCS];
mask_sig=new MASCARA***[NUMMARCS];
for(k=0;k<NUMMARCS;k++){
        mask_ant[k]=new MASCARA**[MAXNIVEL+1];
        mask_sig[k]=new MASCARA**[MAXNIVEL+1];
        for(i=0;i<MAXNIVEL+1;i++){
                mask_ant[k][i]=new MASCARA* [TAMMASCARA];
                mask_sig[k][i]=new MASCARA* [TAMMASCARA];
                for(j=0;j<TAMMASCARA;j++){
                        mask_ant[k][i][j]=new MASCARA [TAMMASCARA];
                        mask_sig[k][i][j]=new MASCARA [TAMMASCARA];
                }
        }
}

boxes= new BOX [300];

disparidad=new float [NUMMARCS];
for(i=0;i<NUMMARCS;i++)
   disparidad[i]=ANCHO;



x = new double [NUMMARCS];
y = new double [NUMMARCS];
z = new double [NUMMARCS];



for(l=0;l<MAXNIVEL+1;l++){
        nivel[l]=MAXNIVEL;
        for(i=0;i<NUMMARCS;i++){
                mov_v[i][l]=0;
                mov_h[i][l]=0;
        }
}


for(l=0;l<NUMMARCS;l++)
for(k=0;k<MAXNIVEL+1;k++)
   for(i=0;i<TAMMASCARA;i++)
        for(j=0;j<TAMMASCARA;j++){
          mask_ant[l][k][i][j].piel=0;
          mask_sig[l][k][i][j].marcar=0;
          mask_ant[l][k][i][j].marcar=0;
          mask_sig[l][k][i][j].piel=0;
          mask_ant[l][k][i][j].imin=TAMMASCARA;
          mask_ant[l][k][i][j].jmin=TAMMASCARA;
          mask_ant[l][k][i][j].imax=0;
          mask_ant[l][k][i][j].jmax=0;
          mask_sig[l][k][i][j].imin=TAMMASCARA;
          mask_sig[l][k][i][j].jmin=TAMMASCARA;
          mask_sig[l][k][i][j].imax=0;
          mask_sig[l][k][i][j].jmax=0;
          mask_sig[l][k][i][j].p=0;
          mask_ant[l][k][i][j].p=0;
        }

a=0;
b=1;

}
 

