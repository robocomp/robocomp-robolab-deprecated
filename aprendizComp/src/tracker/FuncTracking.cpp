#include <tracker/FuncTracking.h>

#ifndef FUNCPIR_H
#include <tracker/Funcpir.h>
#endif

#ifndef CLASIFICACION_H
#include <tracker/Clasificacion.h>
#endif

#include <tracker/IORaw.h>

#include <iostream>
using namespace std;
int **rango1;


void ComprobarTamanioObjetos(TRACKER *track)
{
  for (int j=0;j<NUMMARCS;j++) {
    if (((track->boxnivel[0][j].imax - track->boxnivel[0][j].imin) >= TAMMASCARA) ||
	((track->boxnivel[0][j].jmax - track->boxnivel[0][j].jmin) >= TAMMASCARA))
      throw objeto_demasiado_grande();
  }
}

int RealizarTracking(TRACKER *track,int *run){

int a;
int b;
 register int i,k,j;
int result = -1;


a=track->a;
b=track->b;


result = BoundingBoxNiveles1(track,a);

Clasificacion(&(track->im),track->seg.pixel,ALTO,ANCHO,UMBRAL_IZQ,UMBRAL_LIZQ);

for(i=0; i<ALTO*2;i++)
  for(j=0;j<ANCHO*2;j++){
    if ((track->silueta.pixel[i][j*3]!=0||track->silueta.pixel[i][j*3+1]!=0||track->silueta.pixel[i][j*3+2]!=0) && track->seg.pixel[i/2][j/2]==255)
      track->seg.pixel[i/2][j/2]=255;
    else track->seg.pixel[i/2][j/2]=0;
}

 if (*run==0){
   cout << "BIEN" << endl;
   
   
   // No queremos abortar... preferimos notificar el error
   if (result != -1 ) 
     return result;
   //ComprobarTamanioObjetos(track);
   
   InicializarMaskAnt(track,a);
   rango1=new int* [MAXNIVEL+1];
   for(i=0;i<MAXNIVEL+1;i++)
     rango1[i]=new int [NUMMARCS];
   
   for(k=0;k<NUMMARCS;k++){
     rango1[0][k]=ZONABUSQUEDA;
     rango1[1][k]=rango1[0][k]-1;
     rango1[2][k]=rango1[1][k]-1;
     if(MAXNIVEL==3)
       rango1[3][k]=rango1[2][k]-1;
   }
   *run=1;
 }
 
 
InicializarMaskSig(track->mask_sig, track->mask_ant, track->boxnivel);
 
InicPiramide(track->pir[b],MAXNIVEL+1, track->TAMNiveles_i,track->TAMNiveles_j, track->seg.pixel);

Tracking(track,b);

ActualizarMaskAnt(track->mask_ant,track->mask_sig);

result = BoundingBoxNiveles1(track,b);

track->b=a;
track->a=b;



return (result);
}


 
void PrimerFrame(TRACKER *track, bool primera_vez){

int *NodosV;
int contNV;
int numClases;
int i,j;

NodosV= new int[2000];


Clasificacion(&(track->im),track->seg.pixel,ALTO,ANCHO,UMBRAL_IZQ,UMBRAL_LIZQ);


if (primera_vez==false){

  for(i=0; i<ALTO*2;i++)
    for(j=0;j<ANCHO*2;j++){
      if ((track->silueta.pixel[i][j*3]!=0||track->silueta.pixel[i][j*3+1]!=0||track->silueta.pixel[i][j*3+2]!=0)
	  && track->seg.pixel[i/2][j/2]==255)
	track->seg.pixel[i/2][j/2]=255;
      else track->seg.pixel[i/2][j/2]=0;
    }
}


InicPiramide(track->pir[0], MAXNIVEL+1, track->TAMNiveles_i, track->TAMNiveles_j, track->seg.pixel);

Clasificar(track->pir[0], MAXNIVEL+1, track->TAMNiveles_i, track->TAMNiveles_j,NodosV,0,0);

contNV=Hermanamiento(track->pir[0], MAXNIVEL+1, track->TAMNiveles_i,track->TAMNiveles_j);

Clasificar(track->pir[0], MAXNIVEL+1, track->TAMNiveles_i,track->TAMNiveles_j, NodosV,contNV,1);

numClases=Fusion(track->pir[0],MAXNIVEL+1,track->TAMNiveles_i,track->TAMNiveles_j);

ExtraerClases(track->pir[0], ALTO, ANCHO, numClases,track->boxes, track->clases);


}

int BoundingBoxNiveles1(TRACKER *track, int a)
{

  register int i,j,k,l,m,clase;
  BOX **boxnivel;
  BOX **boxAnt;
  int *tam_i, *tam_j;
  int *encontrada;

  encontrada=new int [NUMMARCS];

  for(i=0;i<NUMMARCS;i++)
    encontrada[i]=0;
  //Inicialización de las cajas

  boxnivel=(track->boxnivel);
  boxAnt=(track->boxAnt);
  tam_i=(track->TAMNiveles_i);
  tam_j=(track->TAMNiveles_j);

  /* Inicialización de boxAnt: si en el frame anterior se encontró la clase */
  for(i=0;i<3;i++)
    for(j=0;j<NUMMARCS;j++){
      if (boxnivel[i][j].imin!=tam_i[i]) {
	boxAnt[i][j].imin=boxnivel[i][j].imin;
	boxAnt[i][j].jmin=boxnivel[i][j].jmin;
	boxAnt[i][j].imax=boxnivel[i][j].imax;
	boxAnt[i][j].jmax=boxnivel[i][j].jmax;
	boxAnt[i][j].areanivel=boxnivel[i][j].areanivel;
	boxAnt[i][j].jcent=boxnivel[i][j].jcent;
	boxAnt[i][j].icent=boxnivel[i][j].icent;
			   
      }
    }

  for(i=0;i<3;i++){
    for(j=0;j<NUMMARCS;j++){
      boxnivel[i][j].imin=tam_i[i];
      boxnivel[i][j].jmin=tam_j[i];
      boxnivel[i][j].imax=0;
      boxnivel[i][j].jmax=0;
      boxnivel[i][j].areanivel=0;
      boxnivel[i][j].encontrada=false;
      boxnivel[i][j].en_estereo=false;
      boxnivel[i][j].sumi=0;
      boxnivel[i][j].sumj=0;
      boxnivel[i][j].icent=-1;
      boxnivel[i][j].jcent=-1;
    }
  }
  //Calculo de las cajas
  int num;
  int error1 = 0;
  for(k=0;k<3;k++){
    for(i=0;i<tam_i[k];i++){
      for(j=0;j<tam_j[k];j++){
	clase=track->pir[a][k][i][j].Clase;
	num=-1;

	for (m=0;m<NUMMARCS; m++){
	  if(clase==track->clases[m])
	    num=m;
	}
	if(num!=-1){

	  // La region se da como encontrada solamente
	  // si se ha encontrado en la base.
	  if ((k==0) || (boxnivel[0][num].areanivel > 0))
	    {
	      encontrada[num]=1;

	      boxnivel[k][num].sumi=boxnivel[k][num].sumi+i;
	      boxnivel[k][num].sumj=boxnivel[k][num].sumj+j;

	      boxnivel[k][num].areanivel=boxnivel[k][num].areanivel+1;

	      if(boxnivel[k][num].imin>i)
		boxnivel[k][num].imin=i;

	      if(boxnivel[k][num].jmin>j)
		boxnivel[k][num].jmin=j;

	      if(boxnivel[k][num].imax<i)
		boxnivel[k][num].imax=i;

	      if(boxnivel[k][num].jmax<j)
		boxnivel[k][num].jmax=j;
	    }
	}
      }
    }
    for(l=0;l<NUMMARCS;l++){
      if((boxnivel[k][l].imin<tam_i[k]|| boxnivel[k][l].imax>0)&&
	 (boxnivel[k][l].jmin<tam_j[k]|| boxnivel[k][l].jmax>0)){
	boxnivel[k][l].icent=(boxnivel[k][l].imin+boxnivel[k][l].imax)/2;
	boxnivel[k][l].jcent=(boxnivel[k][l].jmin+boxnivel[k][l].jmax)/2;
      }        }
  }
  /* Actualizacion de boxAnt: si se ha encontrado algún píxel de la clase */
  // int error2 = 3;

  for(i=0;i<3;i++)
    for(j=0;j<NUMMARCS;j++){
      if(encontrada[j]==1){
	//			    if (boxnivel[0][j].areanivel == 0)
	//			      {
	//				error2++;
	//			      }
	boxAnt[i][j].imin=boxnivel[i][j].imin;
	boxAnt[i][j].jmin=boxnivel[i][j].jmin;
	boxAnt[i][j].imax=boxnivel[i][j].imax;
	boxAnt[i][j].jmax=boxnivel[i][j].jmax;
	boxAnt[i][j].areanivel=boxnivel[i][j].areanivel;
	boxAnt[i][j].jcent=boxnivel[i][j].jcent;
	boxAnt[i][j].icent=boxnivel[i][j].icent;

      }
    }

  int devuelto = -1;
  for (int kk=0; kk<NUMMARCS; kk++)
    {
      if ( !encontrada[kk] )
	{
	  //cout<<"BBox"<<kk<<" no encontrada"<<endl;
	  //GuardarImagenEntrada(track,0);
	  //GuardarImagenColorPiel(track,0);
	  GuardarResultadoTracking(track,0);
	  devuelto = kk;
	}
      else if (((track->boxAnt[0][kk].imax - track->boxAnt[0][kk].imin) >= TAMMASCARA) ||
	       ((track->boxAnt[0][kk].jmax - track->boxAnt[0][kk].jmin) >= TAMMASCARA))
	{
	  //cout<<"BBox"<<kk<<" demasiado grande"<<endl;
	  //GuardarImagenEntrada(track,0);
	  //GuardarImagenColorPiel(track,0);
	  GuardarResultadoTracking(track,0);
	  devuelto = kk;
	}
    }

  // Si todo ha ido bien, se devuelve -1, si no, el valor de la clase errónea
  return (devuelto);

}


void InicializarMaskAnt(TRACKER *track, int a){
int a1;
int b1;

BOX **boxnivel;
MASCARA ****mask_ant;

mask_ant=track->mask_ant;
boxnivel=track->boxnivel;
register int l,m,i,j;
        for(l=0;l<MAXNIVEL+1;l++){
                for(m=0;m<NUMMARCS;m++){
                   a1=TAMMASCARA/2-(boxnivel[l][m].icent-boxnivel[l][m].imin);
                   for(i=boxnivel[l][m].imin;i<=boxnivel[l][m].imax;i++){
                        b1=TAMMASCARA/2-(boxnivel[l][m].jcent-boxnivel[l][m].jmin);
                        for(j=boxnivel[l][m].jmin;j<=boxnivel[l][m].jmax;j++){
                           if(track->pir[0][l][i][j].Clase==track->clases[m]){

                                mask_ant[m][l][a1][b1].piel=255;
                                mask_ant[m][l][a1][b1].p=10;

                                if( mask_ant[m][l][0][0].imin>a1)
                                    mask_ant[m][l][0][0].imin=a1;

                                if( mask_ant[m][l][0][0].jmin>b1)
                                    mask_ant[m][l][0][0].jmin=b1;

                                if( mask_ant[m][l][0][0].imax<a1)
                                    mask_ant[m][l][0][0].imax=a1;

                                if( mask_ant[m][l][0][0].jmax<b1)
                                    mask_ant[m][l][0][0].jmax=b1;

                        }
                   b1++;
                }
                a1++;

                }
        }

     }
}

void InicializarMaskSig(MASCARA ****mask_sig, MASCARA ****mask_ant, BOX **boxnivel){
register int i,j,k,l;

 for(k=0;k<NUMMARCS;k++){
        for(l=0;l<MAXNIVEL+1;l++){
     //   ShowMessage(IntToStr(boxnivel[0][l].areanivel));
                for(i=0;i<TAMMASCARA;i++)
                        for(j=0;j<TAMMASCARA;j++){
                                mask_sig[k][l][i][j].piel=mask_ant[k][l][i][j].piel;
                                mask_sig[k][l][i][j].marcar=0;

			       	if (boxnivel[0][k].areanivel>0)
                                mask_sig[k][l][i][j].p=mask_ant[k][l][i][j].p-1;
				else
				  mask_sig[k][l][i][j].p=mask_ant[k][l][i][j].p;
                        if(mask_sig[k][l][i][j].p<=0) {
                        mask_sig[k][l][i][j].p=0;
                        mask_sig[k][l][i][j].piel=0;
                        }
                        }
                }
        }

}

void ActualizarMaskAnt(MASCARA ****mask_ant, MASCARA ****mask_sig){

register int m,l,i,j;

for(m=0;m<NUMMARCS;m++){
        for(l=0;l<MAXNIVEL+1;l++){
                mask_ant[m][l][0][0].imin=TAMMASCARA;
                mask_ant[m][l][0][0].imax=0;
                mask_ant[m][l][0][0].jmin=TAMMASCARA;
                mask_ant[m][l][0][0].jmax=0;
                for(i=0;i<TAMMASCARA;i++)
                        for(j=0;j<TAMMASCARA;j++){
                                mask_ant[m][l][i][j].piel=mask_sig[m][l][i][j].piel;
                                mask_ant[m][l][i][j].p=mask_sig[m][l][i][j].p;

                                if ((mask_ant[m][l][i][j].piel==255 && mask_ant[m][l][i][j].p>0)){

                                        if( mask_ant[m][l][0][0].imin>i)
                                                mask_ant[m][l][0][0].imin=i;

                                        if( mask_ant[m][l][0][0].jmin>j)
                                                mask_ant[m][l][0][0].jmin=j;

                                        if( mask_ant[m][l][0][0].imax<i)
                                                mask_ant[m][l][0][0].imax=i;

                                        if( mask_ant[m][l][0][0].jmax<j)
                                                mask_ant[m][l][0][0].jmax=j;

                                }
                        }
        }
}
}

void BuscarMaximos(int ****busqueda,int ind,int tam, int nivel,PUNTO *p,PUNTO *p2){

register int i,j;
int j1,j3;
int i1;

i1=0;
j1=0;
j3=0;

for(i=0;i<tam;i++){
 for(j=0;j<tam;j++){
   if(busqueda[nivel][ind][i][j]>=i1){

      i1=busqueda[nivel][ind][i][j];
      j1=j;
      j3=i;
    }
 }
}
p->i=i1;
p->j=j1;
p2->j=j3;
}

void MarcarAbajo(PIR ****pir,int a, int nivel, int *clases, BOX **boxnivel, int *tam_i, int *tam_j, int marc){

register int i,j,l;
int k,a1,b,c,d;
int clase;
int i1,j1,i2,j2;
k=nivel;
if (marc==-1){
for(l=0;l<NUMMARCS;l++){
  clase=clases[l];
  a1=boxnivel[k][l].imin-rango1[k][l];
  if(a1<0)a1=0;
  b=boxnivel[k][l].imax+rango1[k][l];
  if(b>=tam_i[k])b=tam_i[k]-1;
  for(i=a1;i<=b;i++){
    c=boxnivel[k][l].jmin-rango1[k][l];
    if(c<0)c=0;
    d=boxnivel[k][l].jmax+rango1[k][l];
    if(d>=tam_j[k])d=tam_j[k]-1;

    i1=i*2;
    i2=i1+1;

    for(j=c;j<=d;j++){
       if(pir[a][k][i][j].Homog==1){

          j1=j*2;
          j2=j1+1;

          if((pir[a][k][i][j].Clase==clase)
         &&(pir[a][k-1][i1][j1].Clase==-1||pir[a][k-1][i1][j1].Clase==clase)
         &&(pir[a][k-1][i1][j2].Clase==-1||pir[a][k-1][i1][j2].Clase==clase)
         &&(pir[a][k-1][i2][j1].Clase==-1||pir[a][k-1][i2][j1].Clase==clase)
         &&(pir[a][k-1][i2][j2].Clase==-1||pir[a][k-1][i2][j2].Clase==clase))
         {

          pir[a][k-1][i1][j1].Clase=clase;
          pir[a][k-1][i1][j2].Clase=clase;
          pir[a][k-1][i2][j1].Clase=clase;
          pir[a][k-1][i2][j2].Clase=clase;


       }//if
       }//if
   }//for
  }//for
}//for
}else{
l=marc;
clase=clases[l];
  a1=boxnivel[k][l].imin-rango1[k][l];
  if(a1<0)a1=0;
  b=boxnivel[k][l].imax+rango1[k][l];
  if(b>=tam_i[k])b=tam_i[k]-1;
  for(i=a1;i<=b;i++){
    c=boxnivel[k][l].jmin-rango1[k][l];
    if(c<0)c=0;
    d=boxnivel[k][l].jmax+rango1[k][l];
    if(d>=tam_j[k])d=tam_j[k]-1;

    i1=i*2;
    i2=i1+1;

    for(j=c;j<=d;j++){
       if(pir[a][k][i][j].Homog==1){

          j1=j*2;
          j2=j1+1;

          if((pir[a][k][i][j].Clase==clase)
         &&(pir[a][k-1][i1][j1].Clase==-1||pir[a][k-1][i1][j1].Clase==clase)
         &&(pir[a][k-1][i1][j2].Clase==-1||pir[a][k-1][i1][j2].Clase==clase)
         &&(pir[a][k-1][i2][j1].Clase==-1||pir[a][k-1][i2][j1].Clase==clase)
         &&(pir[a][k-1][i2][j2].Clase==-1||pir[a][k-1][i2][j2].Clase==clase))
         {

          pir[a][k-1][i1][j1].Clase=clase;
          pir[a][k-1][i1][j2].Clase=clase;
          pir[a][k-1][i2][j1].Clase=clase;
          pir[a][k-1][i2][j2].Clase=clase;


       }
       }
   }
  }
}

}

void MarcarAbajoMascara(MASCARA ****mask_ant,MASCARA ****mask_sig,int nivel, int marc){

register int i,j,k;
int imin,imax,jmin,jmax;
int i1,j1;

if (marc==-1){
for(k=0;k<NUMMARCS;k++){

imin=mask_ant[k][nivel][0][0].imin-rango1[nivel][k];
if (imin<0) imin=0;
imax=mask_ant[k][nivel][0][0].imax+rango1[nivel][k];
if (imax>=TAMMASCARA) imax=TAMMASCARA-1;
jmin=mask_ant[k][nivel][0][0].jmin-rango1[nivel][k];
if (jmin<0)jmin=0;
jmax=mask_ant[k][nivel][0][0].jmax+rango1[nivel][k];
if (jmax>=TAMMASCARA) jmax=TAMMASCARA-1;

for(i=imin;i<=imax;i++)
        for(j=jmin;j<=jmax;j++){

                if(mask_sig[k][nivel][i][j].marcar==255){
                    mask_sig[k][nivel][i][j].marcar=0;
                        i1=i*2-TAMMASCARA/2;
                        j1=j*2-TAMMASCARA/2;
                if(j1<TAMMASCARA&&i1<TAMMASCARA&&i1>-1&&j1>-1){
                        mask_sig[k][nivel-1][i1][j1].piel=255;

                   mask_sig[k][nivel-1][i1][j1].marcar=255;
                   mask_sig[k][nivel-1][i1][j1].p=10;
                  }
                        j1=j1+1;
                  if(j1<TAMMASCARA&&i1<TAMMASCARA&&i1>-1&&j1>-1){
                        mask_sig[k][nivel-1][i1][j1].piel=255;

                   mask_sig[k][nivel-1][i1][j1].marcar=255;
                   mask_sig[k][nivel-1][i1][j1].p=10;
                  }

                        j1=j1-1;
                        i1=i1+1;
                  if(j1<TAMMASCARA&&i1<TAMMASCARA&&j1>-1&&i1>-1&&j1>-1){

                        mask_sig[k][nivel-1][i1][j1].piel=255;

                   mask_sig[k][nivel-1][i1][j1].marcar=255;
                   mask_sig[k][nivel-1][i1][j1].p=10;
                   }

                         j1=j1+1;
                   if(j1<TAMMASCARA&&i1<TAMMASCARA&&i1>-1&&j1>-1){
                        mask_sig[k][nivel-1][i1][j1].piel=255;

                   mask_sig[k][nivel-1][i1][j1].marcar=255;
                   mask_sig[k][nivel-1][i1][j1].p=10;
                   }
                }
        }
 }
}else{
k=marc;
imin=mask_ant[k][nivel][0][0].imin-rango1[nivel][k];
imax=mask_ant[k][nivel][0][0].imax+rango1[nivel][k];
jmin=mask_ant[k][nivel][0][0].jmin-rango1[nivel][k];
jmax=mask_ant[k][nivel][0][0].jmax+rango1[nivel][k];

for(i=imin;i<=imax;i++)
        for(j=jmin;j<=jmax;j++){

                if(mask_sig[k][nivel][i][j].marcar==255){
                    mask_sig[k][nivel][i][j].marcar=0;
                        i1=i*2-TAMMASCARA/2;
                        j1=j*2-TAMMASCARA/2;
                if(j1<TAMMASCARA&&i1<TAMMASCARA&&i1>-1&&j1>-1){
                        mask_sig[k][nivel-1][i1][j1].piel=255;

                   mask_sig[k][nivel-1][i1][j1].marcar=255;
                   mask_sig[k][nivel-1][i1][j1].p=10;
                  }
                        j1=j1+1;
                  if(j1<TAMMASCARA&&i1<TAMMASCARA&&i1>-1&&j1>-1){
                        mask_sig[k][nivel-1][i1][j1].piel=255;

                   mask_sig[k][nivel-1][i1][j1].marcar=255;
                   mask_sig[k][nivel-1][i1][j1].p=10;
                  }

                        j1=j1-1;
                        i1=i1+1;
                  if(j1<TAMMASCARA&&i1<TAMMASCARA&&j1>-1&&i1>-1&&j1>-1){

                        mask_sig[k][nivel-1][i1][j1].piel=255;

                   mask_sig[k][nivel-1][i1][j1].marcar=255;
                   mask_sig[k][nivel-1][i1][j1].p=10;
                   }

                         j1=j1+1;
                   if(j1<TAMMASCARA&&i1<TAMMASCARA&&i1>-1&&j1>-1){
                        mask_sig[k][nivel-1][i1][j1].piel=255;

                   mask_sig[k][nivel-1][i1][j1].marcar=255;
                   mask_sig[k][nivel-1][i1][j1].p=10;
                   }
                }
        }
 }



}

void ActualizarMascara(MASCARA ****mask, PIR ***pir,int *clas, int *tam_i, int *tam_j, BOX **box){

register int i,j,l,k,m;
int num,clase,aux1,aux2;

for(k=0;k<MAXNIVEL+1;k++)
        for(i=0;i<tam_i[k];i++)
                for(j=0;j<tam_j[k];j++){
                  clase=pir[k][i][j].Clase;
                  num=-1;
                  if(clase!=-1){
                       for(m=0;m<NUMMARCS; m++){
                        if(clase==clas[m])num=m;
                       }
                        if(num!=-1)
                        {
                         aux1=box[k][num].icent-TAMMASCARA/2;
                         aux2=box[k][num].jcent-TAMMASCARA/2;
                         if(i-aux1>-1 && i-aux1<TAMMASCARA && j-aux2>-1&& j-aux2<TAMMASCARA&&mask[num][k][i-aux1][j-aux2].p!=10)
                              mask[num][k][i-aux1][j-aux2].p=10;
                        }
                  }
                }

}



void Tracking(TRACKER *track, int sec){

int i,j,k,l,m,des_h,des_v,n;
int a,b,c,d,a1,tamano,a2,b2;
int porcentaje,r;
PUNTO p,p1,p2;
int aux;
int clase;
int cont=0;
int aux1,aux2;
bool fin=false;
int ****busqueda;
int *valor;
int icent, jcent;

BOX **boxnivel;
BOX **Rdis;
int *tam_i;
int *tam_j;
MASCARA ****mask_ant, ****mask_sig;
PIR ****pir;
BOX *boxaux;

boxaux= new BOX [NUMMARCS];

boxnivel=track->boxnivel;

for(i=0;i<NUMMARCS;i++)
    boxaux[i].areanivel=boxnivel[0][i].areanivel;

Rdis=track->boxAnt;
tam_i=track->TAMNiveles_i;
tam_j=track->TAMNiveles_j;
mask_ant=track->mask_ant;
mask_sig=track->mask_sig;
pir=track->pir;

valor=new int [NUMMARCS];
for(i=0;i<NUMMARCS;i++)
        valor[i]=0;

busqueda=new int***[MAXNIVEL+1];
for(i=0;i<MAXNIVEL+1;i++){
        busqueda[i]=new int** [NUMMARCS];
        for(j=0;j<NUMMARCS;j++){
          busqueda[i][j]=new int* [2*rango1[0][j]+1];
          for(k=0;k<=2*rango1[0][j];k++){
            busqueda[i][j][k]=new int [2*rango1[0][j]+1];
          }
        }
}

for(k=0;k<MAXNIVEL+1;k++)
for(i=0;i<NUMMARCS;i++){
     for(j=0;j<=2*rango1[0][i];j++){
       for(l=0;l<=2*rango1[0][i];l++){
                busqueda[k][i][j][l]=0;
       }
     }
}

for(k=MAXNIVEL;(k>-1 && fin==false);k--){
 fin=true;
 for(n=0;n<NUMMARCS;n++){
    p2.j=-1;
    p.j=-1;
    clase=track->clases[n];
    if(boxnivel[k][n].encontrada==false ){
      fin=false;

#if defined __BORLANDC__
   //   if(boxnivel[0][n].areanivel==0)
    //  	ShowMessage("e0");
#endif

     if(boxnivel[k][n].areanivel>2){

                //Primero lo hacemos para el desplazamiento del frame anterior

                des_v=track->mov_v[k][n];
                des_h=track->mov_h[k][n];

                if(des_v<(-rango1[k][n]))des_v=-rango1[k][n];
                else if (des_v>rango1[k][n])des_v=rango1[k][n];

                if(des_h<(-rango1[k][n]))des_h=-rango1[k][n];
                else if (des_h>rango1[k][n])des_h=rango1[k][n];

               aux1=boxnivel[k][n].icent+des_v;
               aux2=boxnivel[k][n].jcent+des_h;
               a=(aux1)-(TAMMASCARA/2-mask_ant[n][k][0][0].imin);
               if(a<0) a=0;
               else if(a>tam_i[k]-1) a=tam_i[k]-1;

               b=(aux1)+(mask_ant[n][k][0][0].imax-TAMMASCARA/2);
               if(b<0) b=0;
               else if(b>tam_i[k]-1) b=tam_i[k]-1;

               c=(aux2)-(TAMMASCARA/2-mask_ant[n][k][0][0].jmin);
               if(c<0) c=0;
               else if(c>tam_j[k]-1) c=tam_j[k]-1;

               d=(aux2)+(mask_ant[n][k][0][0].jmax-TAMMASCARA/2);
               if(d<0) d=0;
               else if(d>tam_j[k]-1) d=tam_j[k]-1;


           aux1=boxnivel[k][n].icent+des_v-TAMMASCARA/2;
           aux2=boxnivel[k][n].jcent+des_h-TAMMASCARA/2;

         cont=0;
             if(a!=b){
               for(l=a;l<=a+((b-a)/2);l++){
                  for(m=c;m<=d;m++){
                     if((pir[sec][k][l][m].Homog==1)&&(pir[sec][k][l][m].Clase==-1)
                       &&((pir[sec][k][l][m].X==-1)||
                         (pir[sec][k+1][pir[sec][k][l][m].X][pir[sec][k][l][m].Y].Clase==-1))
                       &&(mask_ant[n][k][l-aux1][m-aux2].piel==255)){

                        cont=cont+mask_ant[n][k][l-aux1][m-aux2].p;
                     }/*if*/
                  }/*for*/
               }/*for*/
               aux=cont*10/boxnivel[k][n].areanivel;
              if(aux>90){
                 busqueda[k][n][des_v+rango1[k][n]][des_h+rango1[k][n]]=cont*10/boxnivel[k][n].areanivel;
                 boxnivel[0][n].encontrada=true;
                 boxnivel[1][n].encontrada=true;
                 boxnivel[2][n].encontrada=true;
                if(TAMGRANDE)
                 boxnivel[3][n].encontrada=true;


                 p2.j=0;
                 p.j=0;
                 p.i=100;
              }
              else if (aux<70){
               if(aux>=20 || k==0){
                for(l=a+((b-a)/2)+1;l<=b;l++){
                  for(m=c;m<=d;m++){
                     if((pir[sec][k][l][m].Homog==1)&&(pir[sec][k][l][m].Clase==-1)
                       &&((pir[sec][k][l][m].X==-1)||
                         (pir[sec][k+1][pir[sec][k][l][m].X][pir[sec][k][l][m].Y].Clase==-1))
                       &&(mask_ant[n][k][l-aux1][m-aux2].piel==255)){

                         cont=cont+mask_ant[n][k][l-aux1][m-aux2].p;
                     }/*if*/
                  }/*for*/
               }/*for*/

                 busqueda[k][n][des_v+rango1[k][n]][des_h+rango1[k][n]]=cont*10/boxnivel[k][n].areanivel;
              }else{
                  busqueda[k][n][des_v+rango1[k][n]][des_h+rango1[k][n]]=0;
              }/*if(aux>20)*/
            }/*if(aux<70)*/
            else
               busqueda[k][n][des_v+rango1[k][n]][des_h+rango1[k][n]]=cont*10/boxnivel[k][n].areanivel;

          }else{
              for(l=a;l<=b;l++){
                  for(m=c;m<=c+((d-c)/2);m++){
                     if((pir[sec][k][l][m].Homog==1)&&(pir[sec][k][l][m].Clase==-1)
                       &&((pir[sec][k][l][m].X==-1)||
                         (pir[sec][k+1][pir[sec][k][l][m].X][pir[sec][k][l][m].Y].Clase==-1))
                       &&(mask_ant[n][k][l-aux1][m-aux2].piel==255)){

                         cont=cont+mask_ant[n][k][l-aux1][m-aux2].p;

                     }/*if*/
                  }/*for*/
               }/*for*/
               aux=cont*10/boxnivel[k][n].areanivel;
              if(aux>90){
                busqueda[k][n][des_v+rango1[k][n]][des_h+rango1[k][n]]=cont*10/boxnivel[k][n].areanivel;

                 boxnivel[0][n].encontrada=true;
                 boxnivel[1][n].encontrada=true;
                 boxnivel[2][n].encontrada=true;
                if(TAMGRANDE)
                 boxnivel[3][n].encontrada=true;

                p2.j=0;
                p.j=0;
                p.i=100;
              }

              else if (aux<70){
               if(aux>=20 || k==0){
                for(l=a;l<=b;l++){
                  for(m=c+((d-c)/2)+1;m<=d;m++){
                     if((pir[sec][k][l][m].Homog==1)&&(pir[sec][k][l][m].Clase==-1)
                      &&((pir[sec][k][l][m].X==-1)||
                         (pir[sec][k+1][pir[sec][k][l][m].X][pir[sec][k][l][m].Y].Clase==-1))
                       &&(mask_ant[n][k][l-aux1][m-aux2].piel==255)){

                         cont=cont+mask_ant[n][k][l-aux1][m-aux2].p;

                     }/*if*/
                  }/*for*/
               }/*for*/
                 busqueda[k][n][des_v+rango1[k][n]][des_h+rango1[k][n]]=cont*10/boxnivel[k][n].areanivel;
              }else{
                  busqueda[k][n][des_v+rango1[k][n]][des_h+rango1[k][n]]=0;
              }/*if(aux>20)*/
            }/*if(aux<70)*/
           else
               busqueda[k][n][des_v+rango1[k][n]][des_h+rango1[k][n]]=cont*10/boxnivel[k][n].areanivel;

          }/*if(a!=b)*/

        //Ahora para el resto de los casos

        for(des_v=-rango1[k][n];(des_v<=rango1[k][n] && boxnivel[k][n].encontrada==false);des_v++){
               a=(boxnivel[k][n].icent+des_v)-(TAMMASCARA/2-mask_ant[n][k][0][0].imin);
               if(a<0) a=0;
               else if(a>tam_i[k]-1) a=tam_i[k]-1;
               b=(boxnivel[k][n].icent+des_v)+(mask_ant[n][k][0][0].imax-TAMMASCARA/2);
               if(b<0) b=0;
               else if(b>tam_i[k]-1) b=tam_i[k]-1;

               aux1=boxnivel[k][n].icent+des_v-TAMMASCARA/2;

            for(des_h=-rango1[k][n];(des_h<=rango1[k][n] && boxnivel[k][n].encontrada==false
                && (des_v!=track->mov_v[k][n] || des_h!=track->mov_h[k][n]));des_h++){

               c=(boxnivel[k][n].jcent+des_h)-(TAMMASCARA/2-mask_ant[n][k][0][0].jmin);
               if(c<0) c=0;
               else if(c>tam_j[k]-1) c=tam_j[k]-1;
               d=(boxnivel[k][n].jcent+des_h)+(mask_ant[n][k][0][0].jmax-TAMMASCARA/2);
               if(d<0) d=0;
               else if(d>tam_j[k]-1) d=tam_j[k]-1;

               aux2=boxnivel[k][n].jcent+des_h-TAMMASCARA/2;

         cont=0;
             if(a!=b){
               for(l=a;l<=a+((b-a)/2);l++){
                  for(m=c;m<=d;m++){
                     if((pir[sec][k][l][m].Homog==1)&&(pir[sec][k][l][m].Clase==-1)
                       &&((pir[sec][k][l][m].X==-1)||
                         (pir[sec][k+1][pir[sec][k][l][m].X][pir[sec][k][l][m].Y].Clase==-1))
                       &&(mask_ant[n][k][l-aux1][m-aux2].piel==255)){

                        cont=cont+mask_ant[n][k][l-aux1][m-aux2].p;
                     }/*if*/
                  }/*for*/
               }/*for*/
               aux=cont*10/boxnivel[k][n].areanivel;
              if(aux>90){
                 busqueda[k][n][des_v+rango1[k][n]][des_h+rango1[k][n]]=cont*10/boxnivel[k][n].areanivel;
                 boxnivel[0][n].encontrada=true;
                 boxnivel[1][n].encontrada=true;
                 boxnivel[2][n].encontrada=true;
                if(TAMGRANDE)
                 boxnivel[3][n].encontrada=true;

                 p2.j=0;
                 p.j=0;
                 p.i=100;
              }
              else if (aux<70){
               if(aux>=20 || k==0){
                for(l=a+((b-a)/2)+1;l<=b;l++){
                  for(m=c;m<=d;m++){
                     if((pir[sec][k][l][m].Homog==1)&&(pir[sec][k][l][m].Clase==-1)
                       &&((pir[sec][k][l][m].X==-1)||
                         (pir[sec][k+1][pir[sec][k][l][m].X][pir[sec][k][l][m].Y].Clase==-1))
                       &&(mask_ant[n][k][l-aux1][m-aux2].piel==255)){

                         cont=cont+mask_ant[n][k][l-aux1][m-aux2].p;
                     }/*if*/
                  }/*for*/
               }/*for*/

                 busqueda[k][n][des_v+rango1[k][n]][des_h+rango1[k][n]]=cont*10/boxnivel[k][n].areanivel;
              }else{
                  busqueda[k][n][des_v+rango1[k][n]][des_h+rango1[k][n]]=0;
              }/*if(aux>20)*/
            }/*if(aux<70)*/
            else
               busqueda[k][n][des_v+rango1[k][n]][des_h+rango1[k][n]]=cont*10/boxnivel[k][n].areanivel;

          }else{
              for(l=a;l<=b;l++){
                  for(m=c;m<=c+((d-c)/2);m++){
                     if((pir[sec][k][l][m].Homog==1)&&(pir[sec][k][l][m].Clase==-1)
                       &&((pir[sec][k][l][m].X==-1)||
                         (pir[sec][k+1][pir[sec][k][l][m].X][pir[sec][k][l][m].Y].Clase==-1))
                       &&(mask_ant[n][k][l-aux1][m-aux2].piel==255)){

                         cont=cont+mask_ant[n][k][l-aux1][m-aux2].p;

                     }/*if*/
                  }/*for*/
               }/*for*/
               aux=cont*10/boxnivel[k][n].areanivel;
              if(aux>90){
                busqueda[k][n][des_v+rango1[k][n]][des_h+rango1[k][n]]=cont*10/boxnivel[k][n].areanivel;
                boxnivel[0][n].encontrada=true;
                boxnivel[1][n].encontrada=true;
                boxnivel[2][n].encontrada=true;
               if(TAMGRANDE)
                boxnivel[3][n].encontrada=true;

                p2.j=0;
                p.j=0;
                p.i=100;
              }

              else if (aux<70){
               if(aux>=20 || k==0){
                for(l=a;l<=b;l++){
                  for(m=c+((d-c)/2)+1;m<=d;m++){
                     if((pir[sec][k][l][m].Homog==1)&&(pir[sec][k][l][m].Clase==-1)
                      &&((pir[sec][k][l][m].X==-1)||
                         (pir[sec][k+1][pir[sec][k][l][m].X][pir[sec][k][l][m].Y].Clase==-1))
                       &&(mask_ant[n][k][l-aux1][m-aux2].piel==255)){

                         cont=cont+mask_ant[n][k][l-aux1][m-aux2].p;

                     }/*if*/
                  }/*for*/
               }/*for*/
                 busqueda[k][n][des_v+rango1[k][n]][des_h+rango1[k][n]]=cont*10/boxnivel[k][n].areanivel;
              }else{
                  busqueda[k][n][des_v+rango1[k][n]][des_h+rango1[k][n]]=0;
              }/*if(aux>20)*/
            }/*if(aux<70)*/
           else
               busqueda[k][n][des_v+rango1[k][n]][des_h+rango1[k][n]]=cont*10/boxnivel[k][n].areanivel;

          }/*if(a!=b)*/
            }/*for(des_h)*/
          }//for (des_v)

           if(p2.j==-1 && p.j==-1){
            BuscarMaximos(busqueda,n,2*rango1[k][n]+1,k,&p,&p2);
            p.j=p.j-rango1[k][n];
            p2.j=p2.j-rango1[k][n];
           }
            if(p.i<=0) track->nivel[n]=-1;
            if (p.i>=70 ||(k==0 && p.i>0)){

                boxnivel[0][n].encontrada=true;
                boxnivel[1][n].encontrada=true;
                boxnivel[2][n].encontrada=true;
               if(TAMGRANDE)
                boxnivel[3][n].encontrada=true;


                track->nivel[n]=k;
                track->mov_v[k][n]=p.j;
                track->mov_h[k][n]=p2.j;

                valor[n]=p.i;

                for(r=k;r>0;r--){
                        track->mov_h[r-1][n]=(track->mov_h[r][n])*2;
                        track->mov_v[r-1][n]=(track->mov_v[r][n])*2;
                }
                for(r=k;r<MAXNIVEL;r++){
                        track->mov_h[r+1][n]=(track->mov_h[r][n])/2;
                        track->mov_v[r+1][n]=(track->mov_v[r][n])/2;
                }

              //if(k>0)
               // boxnivel[k-1][n].encontrada=true;

               a=(boxnivel[k][n].icent+(int)p2.j)-(TAMMASCARA/2-mask_ant[n][k][0][0].imin);
               if(a<0) a=0;
               else if(a>tam_i[k]-1) a=tam_i[k]-1;

               b=(boxnivel[k][n].icent+p2.j)+(mask_ant[n][k][0][0].imax-TAMMASCARA/2);
               if(b<0) b=0;
               else if(b>tam_i[k]-1) b=tam_i[k]-1;

               c=(boxnivel[k][n].jcent+(int)p.j)-(TAMMASCARA/2-mask_ant[n][k][0][0].jmin);

               if(c<0) c=0;
               else if(c>tam_j[k]-1) c=tam_j[k]-1;

               d=(boxnivel[k][n].jcent+(int)p.j)+(mask_ant[n][k][0][0].jmax-TAMMASCARA/2);
               if(d<0) d=0;
               else if(d>tam_j[k]-1) d=tam_j[k]-1;

               aux1=boxnivel[k][n].icent+(int)p2.j-TAMMASCARA/2;
               aux2=boxnivel[k][n].jcent+(int)p.j-TAMMASCARA/2;

               for(l=a;l<=b;l++){
                  for(m=c;m<=d;m++){

                     if((pir[sec][k][l][m].Homog==1)&&(pir[sec][k][l][m].Clase==-1)
                      &&((pir[sec][k][l][m].X==-1)||
                         (pir[sec][k+1][pir[sec][k][l][m].X][pir[sec][k][l][m].Y].Clase==-1))
                       &&(mask_ant[n][k][l-aux1][m-aux2].piel==255)){

                          pir[sec][k][l][m].Clase=clase;
                          mask_sig[n][k][l-aux1][m-aux2].piel==255;
                          mask_sig[n][k][l-aux1][m-aux2].marcar=255;
                          mask_sig[n][k][l-aux1][m-aux2].p=10;

                     }/*if*/

                  }/*for*/

               }/*for*/


            }/*if (p.i>70)*/

        //Tengo que encontrar el maximo y si es !=0 marcar la caja como encontrada
        //irme a la posicion de la caja y clasificar los nodos q cumplen los requisitos
      //aki si la encuentro tb tengo q marcarla como encontrada abajo

     }/*if (PORAREA)*/

  }/*if(no encontrada)*/
 // else
  //  if(k>0)
  //   boxnivel[k-1][n].encontrada=true;


}
  if(k>0){
   MarcarAbajoMascara(mask_ant,mask_sig,k,-1);
   MarcarAbajo(pir,sec,k,track->clases,boxnivel,tam_i,tam_j,-1);
  }
}


for(k=1;k<MAXNIVEL+1;k++){
  for(i=0;i<tam_i[k];i++){
     for(j=0;j<tam_j[k];j++){
        if(pir[sec][k][i][j].Homog==1&&pir[sec][k][i][j].Clase==-1){
                if(pir[sec][k-1][i*2][j*2].Clase!=-1&&pir[sec][k-1][i*2][j*2+1].Clase!=-1&&
                   pir[sec][k-1][i*2+1][j*2].Clase!=-1&&pir[sec][k-1][i*2+1][j*2+1].Clase!=-1){
                    if((pir[sec][k-1][i*2][j*2].Clase==pir[sec][k-1][i*2][j*2+1].Clase) &&
                        (pir[sec][k-1][i*2][j*2].Clase==pir[sec][k-1][i*2+1][j*2].Clase)&&
                        (pir[sec][k-1][i*2][j*2].Clase==pir[sec][k-1][i*2+1][j*2+1].Clase))
                     pir[sec][k][i][j].Clase=pir[sec][k-1][i*2][j*2].Clase;

                }//if
        }
     }
  }
}

if(TAMGRANDE){
        HermanamientoNivel(pir,sec,boxnivel,tam_i,tam_j,mask_sig,3,track->clases,-1);
        MarcarAbajo(pir,sec,3,track->clases,boxnivel,tam_i,tam_j,-1);
}
HermanamientoNivel(pir,sec,boxnivel,tam_i,tam_j,mask_sig,2,track->clases,-1);
MarcarAbajo(pir,sec,2,track->clases,boxnivel,tam_i,tam_j,-1);
MarcarAbajoMascara(mask_ant,mask_sig,2,-1);
HermanamientoNivel(pir,sec,boxnivel,tam_i,tam_j,mask_sig,1,track->clases,-1);
MarcarAbajo(pir,sec,1,track->clases,boxnivel,tam_i,tam_j,-1);
MarcarAbajoMascara(mask_ant,mask_sig,1,-1);
HermanamientoNivel(pir,sec,boxnivel,tam_i,tam_j,mask_sig,0,track->clases,-1);

for(k=1;k<MAXNIVEL+1;k++){
  for(i=0;i<tam_i[k];i++){
     for(j=0;j<tam_j[k];j++){
        if(pir[sec][k][i][j].Homog==1&&pir[sec][k][i][j].Clase==-1){
                if(pir[sec][k-1][i*2][j*2].Clase!=-1&&pir[sec][k-1][i*2][j*2+1].Clase!=-1&&
                   pir[sec][k-1][i*2+1][j*2].Clase!=-1&&pir[sec][k-1][i*2+1][j*2+1].Clase!=-1){
                    if((pir[sec][k-1][i*2][j*2].Clase==pir[sec][k-1][i*2][j*2+1].Clase) &&
                        (pir[sec][k-1][i*2][j*2].Clase==pir[sec][k-1][i*2+1][j*2].Clase)&&
                        (pir[sec][k-1][i*2][j*2].Clase==pir[sec][k-1][i*2+1][j*2+1].Clase))
                     pir[sec][k][i][j].Clase=pir[sec][k-1][i*2][j*2].Clase;

                }//if
        }
     }
  }
}

if(TAMGRANDE){
        HermanamientoInversoNivel(pir,sec,boxnivel,tam_i,tam_j,mask_sig,3,track->clases,-1);
        MarcarAbajo(pir,sec,3,track->clases,boxnivel,tam_i,tam_j,-1);
}
HermanamientoInversoNivel(pir,sec,boxnivel,tam_i,tam_j,mask_sig,2,track->clases,-1);
MarcarAbajo(pir,sec,2,track->clases,boxnivel,tam_i,tam_j,-1);
MarcarAbajoMascara(mask_ant,mask_sig,2,-1);
HermanamientoInversoNivel(pir,sec,boxnivel,tam_i,tam_j,mask_sig,1,track->clases,-1);
MarcarAbajo(pir,sec,1,track->clases,boxnivel,tam_i,tam_j,-1);
MarcarAbajoMascara(mask_ant,mask_sig,1,-1);
HermanamientoInversoNivel(pir,sec,boxnivel,tam_i,tam_j,mask_sig,0,track->clases,-1);


for(k=1;k<MAXNIVEL+1;k++){
  for(i=0;i<tam_i[k];i++){
     for(j=0;j<tam_j[k];j++){
        if(pir[sec][k][i][j].Homog==1&&pir[sec][k][i][j].Clase==-1){
                if(pir[sec][k-1][i*2][j*2].Clase!=-1&&pir[sec][k-1][i*2][j*2+1].Clase!=-1&&
                   pir[sec][k-1][i*2+1][j*2].Clase!=-1&&pir[sec][k-1][i*2+1][j*2+1].Clase!=-1){
                    if((pir[sec][k-1][i*2][j*2].Clase==pir[sec][k-1][i*2][j*2+1].Clase) &&
                        (pir[sec][k-1][i*2][j*2].Clase==pir[sec][k-1][i*2+1][j*2].Clase)&&
                        (pir[sec][k-1][i*2][j*2].Clase==pir[sec][k-1][i*2+1][j*2+1].Clase))
                     pir[sec][k][i][j].Clase=pir[sec][k-1][i*2][j*2].Clase;

                }//if
        }
     }
  }
}


ActualizarMascara(mask_sig, pir[sec],track->clases,tam_i,tam_j,boxnivel);

/* Recuperación de marcadores perdidos en el fotograma anterior */
for(n=0;n<NUMMARCS;n++){
        if (boxaux[n].areanivel==0) {
                #if defined __BORLANDC__
 	        //ShowMessage("Aki");
                #endif
	/* (icent, jcent) - Centroide de la zona donde puede estar el marcador */
		        icent=Rdis[0][n].icent;//(int)((Rdis[0][n].imax+Rdis[0][n].imin)/2);
			jcent=Rdis[0][n].jcent;//(int)((Rdis[0][n].jmax+Rdis[0][n].jmin)/2);
			/* clase - ClaseBusqueda[n] */
			clase=track->clases[n];//ClaseBusqueda[n];

			p2.j=-1;
			p.j =-1;

			/* Cálculo de los límites de la zona de búsqueda en función de los desplazamientos */
			for(des_v=-rango1[0][n];(des_v<=rango1[0][n] && boxnivel[0][n].encontrada==false);des_v++){
				a=(icent+des_v)-(TAMMASCARA/2-mask_ant[n][0][0][0].imin);

				if(a<0)
					a=0;
				else if (a>tam_i[0]-1)
					a=tam_i[0]-1;

				b=(icent+des_v)+(mask_ant[n][0][0][0].imax-TAMMASCARA/2);
				if(b<0)
					b=0;
				else if (b>tam_i[0]-1)
					b=tam_i[0]-1;

				aux1=icent+des_v-TAMMASCARA/2;

				for(des_h=-rango1[0][n];(des_h<=rango1[0][n] && boxnivel[0][n].encontrada==false);des_h++){
					c=(jcent+des_h)-(TAMMASCARA/2-mask_ant[n][0][0][0].jmin);
					if(c<0)
						c=0;
					else if (c>tam_j[0]-1)
						c=tam_j[0]-1;

					d=(jcent+des_h)+(mask_ant[n][0][0][0].jmax-TAMMASCARA/2);
					if(d<0)
						d=0;
					else if (d>tam_j[0]-1)
						d=tam_j[0]-1;

					aux2=jcent+des_h-TAMMASCARA/2;

					cont=0;
					if(a!=b){
						for(l=a;l<=a+((b-a)/2);l++){
							for(m=c;m<=d;m++){
                                                        //if(pir[sec][0][l][m].Homog==1&&pir[sec][0][l][m].Clase==-1) ShowMessage("si");
							   if((pir[sec][0][l][m].Homog==1)&&(pir[sec][0][l][m].Clase==-1)
							      &&((pir[sec][0][l][m].X==-1)||
								 (pir[sec][1][pir[sec][0][l][m].X][pir[sec][0][l][m].Y].Clase==-1))
							      &&(mask_ant[n][0][l-aux1][m-aux2].piel==255)){

							      cont=cont+mask_ant[n][0][l-aux1][m-aux2].p;
							   }/*if*/

							}
						}
						aux=cont*10/Rdis[0][n].areanivel;

						if(aux>90){
							busqueda[0][n][des_v+rango1[0][n]]
									[des_h+rango1[0][n]]=cont*100/Rdis[0][n].areanivel;
                                                        boxnivel[0][n].encontrada=true;
                                                        boxnivel[1][n].encontrada=true;
                                                        boxnivel[2][n].encontrada=true;
                                                     if(TAMGRANDE)
                                                        boxnivel[3][n].encontrada=true;

							p2.j=0;
							p.j=0;
							p.i=100;
						}
						else if (aux<70){
							for(l=a+((b-a)/2)+1;l<=b;l++){
								for(m=c;m<=d;m++){
                                                              //  if(pir[sec][0][l][m].Homog==1&&pir[sec][0][l][m].Clase==-1) ShowMessage("si1");
								     if((pir[sec][0][l][m].Homog==1)&&(pir[sec][0][l][m].Clase==-1)
									&&((pir[sec][0][l][m].X==-1)||
									 (pir[sec][1][pir[sec][0][l][m].X][pir[sec][0][l][m].Y].Clase==-1))
									  &&(mask_ant[n][0][l-aux1][m-aux2].piel==255)){

									  cont=cont+mask_ant[n][0][l-aux1][m-aux2].p;
									}
								}
							}
							busqueda[0][n][des_v+rango1[0][n]]
									[des_h+rango1[0][n]]=cont*100/Rdis[0][n].areanivel;
						}
						else
							busqueda[0][n][des_v+rango1[0][n]]
									[des_h+rango1[0][n]]=cont*100/Rdis[0][n].areanivel;

					} /* if (a!=b) */
					else {
						for(l=a;l<=b;l++){
							for(m=c;m<=c+((d-c)/2);m++){
                                                            //    if(pir[sec][0][l][m].Homog==1&&pir[sec][0][l][m].Clase==-1) ShowMessage("si2");
								if((pir[sec][0][l][m].Homog==1)&&(pir[sec][0][l][m].Clase==-1)
									&&((pir[sec][0][l][m].X==-1)||
									(pir[sec][1][pir[sec][0][l][m].X][pir[sec][0][l][m].Y].Clase==-1))
									&&(mask_ant[n][0][l-aux1][m-aux2].piel==255)){

									cont=cont+mask_ant[n][0][l-aux1][m-aux2].p;
								}
							}
						}
						aux=cont*10/Rdis[0][n].areanivel;
						if(aux>90){
							busqueda[0][n][des_v+rango1[0][n]]
									[des_h+rango1[0][n]]=cont*100/Rdis[0][n].areanivel;
                                                        boxnivel[0][n].encontrada=true;
                                                        boxnivel[1][n].encontrada=true;
                                                        boxnivel[2][n].encontrada=true;
                                                     if(TAMGRANDE)
                                                        boxnivel[3][n].encontrada=true;

							p2.j=0;
							p.j=0;
							p.i=100;
						}
						else if (aux<70){
							for(l=a;l<=b;l++){
								for(m=c+((d-c)/2)+1;m<=d;m++){
                                                               // if(pir[sec][0][l][m].Homog==1&&pir[sec][0][l][m].Clase==-1) ShowMessage("si3");
									if((pir[sec][0][l][m].Homog==1)&&(pir[sec][0][l][m].Clase==-1)
										&&((pir[sec][0][l][m].X==-1)||
										(pir[sec][1][pir[sec][0][l][m].X][pir[sec][0][l][m].Y].Clase==-1))
										&&(mask_ant[n][0][l-aux1][m-aux2].piel==255)){

										cont=cont+mask_ant[n][0][l-aux1][m-aux2].p;
									}
								}
							}
							busqueda[0][n][des_v+rango1[0][n]]
									[des_h+rango1[0][n]]=cont*100/Rdis[0][n].areanivel;
						}
						else
							busqueda[0][n][des_v+rango1[0][n]]
									[des_h+rango1[0][n]]=cont*100/Rdis[0][n].areanivel;
					} /* if (a==b) */
				} /* for (desh ... */
			} /* for (desv ... */
			if (p2.j==-1 && p.j==-1){
				BuscarMaximos(busqueda,n,2*rango1[0][n]+1,0,&p,&p2);
				p.j =p.j -rango1[0][n];
				p2.j=p2.j-rango1[0][n];
			}
                        if(p.i<=0) track->nivel[n]=-1;
			if (p.i>0){
                                boxnivel[0][n].encontrada=true;
                                boxnivel[1][n].encontrada=true;
                                boxnivel[2][n].encontrada=true;
                             if(TAMGRANDE)
                                boxnivel[3][n].encontrada=true;


				track->nivel[n]=0;
				track->mov_v[0][n]=(int)p.j;
				track->mov_h[0][n]=(int)p2.j;

				/*Actualización de los desplazamientos en los distintos niveles*/
				for(r=0;r<MAXNIVEL;r++){
					track->mov_h[r+1][n]=track->mov_h[r][n]/2;
					track->mov_v[r+1][n]=track->mov_v[r][n]/2;
				}

				valor[n]=p.i;

				a=(icent+(int)p2.j)-(TAMMASCARA/2-mask_ant[n][0][0][0].imin);
				if(a<0)
					a=0;
				else if (a>tam_i[0]-1)
					a=tam_i[0]-1;

				b=(icent+(int)p2.j)+(mask_ant[n][0][0][0].imax-TAMMASCARA/2);
				if(b<0)
					b=0;
				else if (b>tam_i[0]-1)
					b=tam_i[0]-1;

				c=(jcent+(int)p.j)-(TAMMASCARA/2-mask_ant[n][0][0][0].jmin);
				if(c<0)
					c=0;
				else if (c>tam_j[0]-1)
					c=tam_j[0]-1;

				d=(jcent+(int)p.j)+(mask_ant[n][0][0][0].jmax-TAMMASCARA/2);
				if(d<0)
					d=0;
				else if (d>tam_j[0]-1)
					d=tam_j[0]-1;

				aux1=icent+(int)p2.j-TAMMASCARA/2;
				aux2=jcent+(int)p.j-TAMMASCARA/2;

				for(l=a;l<=b;l++){
					for(m=c;m<=d;m++){
						if((pir[sec][0][l][m].Homog==1)&&(pir[sec][0][l][m].Clase==-1)
							&&((pir[sec][0][l][m].X==-1)||
							(pir[sec][1][pir[sec][0][l][m].X][pir[sec][0][l][m].Y].Clase==-1))
							&&(mask_ant[n][0][l-aux1][m-aux2].piel==255)){
							pir[sec][0][l][m].Clase=clase;
							mask_sig[n][0][l-aux1][m-aux2].piel=255;
							mask_sig[n][0][l-aux1][m-aux2].marcar=255;
							mask_sig[n][0][l-aux1][m-aux2].p=10;
						}
					}
				}

			} /* if (p.i>0) */
                      if (boxnivel[0][n].encontrada==true){
for(k=1;k<MAXNIVEL+1;k++){
  for(i=0;i<tam_i[k];i++){
     for(j=0;j<tam_j[k];j++){
        if(pir[sec][k][i][j].Homog==1&&pir[sec][k][i][j].Clase==-1){
                if(pir[sec][k-1][i*2][j*2].Clase==clase&&pir[sec][k-1][i*2][j*2+1].Clase==clase&&
                   pir[sec][k-1][i*2+1][j*2].Clase==clase&&pir[sec][k-1][i*2+1][j*2+1].Clase==clase){

                     pir[sec][k][i][j].Clase=clase;

                }//if
        }
     }
  }
}


HermanamientoNivel(pir,sec,boxnivel,tam_i,tam_j,mask_sig,0,track->clases,n);

for(k=1;k<MAXNIVEL+1;k++){
  for(i=0;i<tam_i[k];i++){
     for(j=0;j<tam_j[k];j++){
        if(pir[sec][k][i][j].Homog==1&&pir[sec][k][i][j].Clase==-1){
                if(pir[sec][k-1][i*2][j*2].Clase==clase&&pir[sec][k-1][i*2][j*2+1].Clase==clase&&
                   pir[sec][k-1][i*2+1][j*2].Clase==clase&&pir[sec][k-1][i*2+1][j*2+1].Clase==clase){

                     pir[sec][k][i][j].Clase=clase;

                }//if
        }
     }
  }
}



HermanamientoInversoNivel(pir,sec,boxnivel,tam_i,tam_j,mask_sig,0,track->clases,n);


for(k=1;k<MAXNIVEL+1;k++){
  for(i=0;i<tam_i[k];i++){
     for(j=0;j<tam_j[k];j++){
        if(pir[sec][k][i][j].Homog==1&&pir[sec][k][i][j].Clase==-1){
                if(pir[sec][k-1][i*2][j*2].Clase==clase&&pir[sec][k-1][i*2][j*2+1].Clase==clase&&
                   pir[sec][k-1][i*2+1][j*2].Clase==clase&&pir[sec][k-1][i*2+1][j*2+1].Clase==clase){

                     pir[sec][k][i][j].Clase=clase;

                }//if
        }
     }
  }
}


ActualizarMascara(mask_sig, pir[sec],track->clases,tam_i,tam_j,boxnivel);



                      }

		}
	}




int zona=ZONABUSQUEDA;
for(i=0;i<NUMMARCS;i++){
if (boxnivel[0][i].encontrada==true&&track->nivel[i]>0){
   rango1[0][i]=zona;
   rango1[1][i]=rango1[0][i]-1;
   rango1[2][i]=rango1[1][i]-1;
   if(TAMGRANDE)
     rango1[3][i]=rango1[2][i]-1;

}else if (boxnivel[0][i].encontrada==false){
        track->mov_v[0][i]=0;
        track->mov_h[0][i]=0;
        rango1[0][i]=rango1[0][i]+1;
        if (rango1[0][i]>MAXZONABUSQUEDA) rango1[0][i]=MAXZONABUSQUEDA;
        rango1[1][i]=rango1[0][i]-1;
        rango1[2][i]=rango1[1][i]-1;
        if(TAMGRANDE)
          rango1[3][i]=rango1[2][i]-1;

}else if (boxnivel[0][i].encontrada==true&&track->nivel[i]==0 && valor[i]>=70){
        rango1[0][i]=zona;
        rango1[1][i]=rango1[0][i]-1;
        rango1[2][i]=rango1[1][i]-1;
        if(TAMGRANDE)
         rango1[3][i]=rango1[2][i]-1;

} else if(boxnivel[0][i].encontrada==true&&track->nivel[i]==0 && valor[i]<70){
        rango1[0][i]=rango1[0][i]+1;
        if (rango1[0][i]>MAXZONABUSQUEDA) rango1[0][i]=MAXZONABUSQUEDA;
        rango1[1][i]=rango1[0][i]-1;
        rango1[2][i]=rango1[1][i]-1;
        if(TAMGRANDE)
         rango1[3][i]=rango1[2][i]-1;
}
}


}
void HermanamientoNivel(PIR ****pir, int a,BOX **boxnivel, int *tam_i,int *tam_j, MASCARA ****mask_sig,int nivel,int *clases, int marc){

int i,j,k,l,m,a1,b,c,d;
int tamano_i,tamano_j;
int clase;
int aux1,aux2;

k=nivel;

tamano_i=tam_i[k];
tamano_j=tam_j[k];
if (marc==-1){
for(m=0;m<NUMMARCS;m++){
 clase=clases[m];

a1=boxnivel[k][m].imin-rango1[k][m];
if(a1<0)a1=0;
 b=boxnivel[k][m].imax+rango1[k][m];
if(b>=tamano_i)b=tamano_i-1;

aux1=boxnivel[k][m].icent-TAMMASCARA/2;
aux2=boxnivel[k][m].jcent-TAMMASCARA/2;

c=boxnivel[k][m].jmin-rango1[k][m];
if(c<0)c=0;
d=boxnivel[k][m].jmax+rango1[k][m];
if(d>=tamano_j)d=tamano_j-1;

for (i=a1;i<=b;i++){
    for(j=c;j<=d;j++){

       if ((pir[a][k][i][j].Homog==1)&&(pir[a][k][i][j].Clase==-1)
           &&((pir[a][k][i][j].X==-1)||
           (pir[a][k+1][pir[a][k][i][j].X][pir[a][k][i][j].Y].Clase==-1)))
           {

          if(((j-1)>=c)&&(pir[a][k][i][j-1].Homog==1)
             &&(pir[a][k][i][j-1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;
              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
                mask_sig[m][k][i-aux1][j-aux2-1].p=10;
             }
          }


          else if(((j-1)>=c)&&((i-1)>=a1)&&(pir[a][k][i-1][j-1].Homog==1)
              &&(pir[a][k][i-1][j-1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
                mask_sig[m][k][i-aux1-1][j-aux2-1].p=10;
             }
          }

          else if(((i-1)>=a1)&&(pir[a][k][i-1][j].Homog==1)
             &&(pir[a][k][i-1][j].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
                mask_sig[m][k][i-aux1-1][j-aux2].p=10;
             }
          }
          else if(((i-1)>=a1)&&((j+1)<=d)&&(pir[a][k][i-1][j+1].Homog==1)
             &&(pir[a][k][i-1][j+1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2+1)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
                mask_sig[m][k][i-aux1-1][j-aux2+1].p=10;
             }
          }
          else if(((j+1)<=d)&&(pir[a][k][i][j+1].Homog==1)
            &&(pir[a][k][i][j+1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2+1)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
                mask_sig[m][k][i-aux1][j-aux2+1].p=10;
             }
          }
          else if(((j+1)<=d) &&((i+1)<=b)&&(pir[a][k][i+1][j+1].Homog==1)
             &&(pir[a][k][i+1][j+1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1+1)<TAMMASCARA && (j-aux2+1)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
                mask_sig[m][k][i-aux1+1][j-aux2+1].p=10;
             }
          }
          else if(((i+1)<=b)&&(pir[a][k][i+1][j].Homog==1)
             &&(pir[a][k][i+1][j].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1+1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
                mask_sig[m][k][i-aux1+1][j-aux2].p=10;
             }
          }
          else if(((j-1)>=c)&&((i+1)<=b)&&(pir[a][k][i+1][j-1].Homog==1)
             &&(pir[a][k][i+1][j-1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1+1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
                mask_sig[m][k][i-aux1+1][j-aux2-1].p=10;
             }
          }


    }
  }
 }
}
}else{
m=marc;
 clase=clases[m];

a1=boxnivel[k][m].imin-rango1[k][m];
if(a1<0)a1=0;
 b=boxnivel[k][m].imax+rango1[k][m];
if(b>=tamano_i)b=tamano_i-1;

aux1=boxnivel[k][m].icent-TAMMASCARA/2;
aux2=boxnivel[k][m].jcent-TAMMASCARA/2;

c=boxnivel[k][m].jmin-rango1[k][m];
if(c<0)c=0;
d=boxnivel[k][m].jmax+rango1[k][m];
if(d>=tamano_j)d=tamano_j-1;

for (i=a1;i<=b;i++){
    for(j=c;j<=d;j++){

       if ((pir[a][k][i][j].Homog==1)&&(pir[a][k][i][j].Clase==-1)
           &&((pir[a][k][i][j].X==-1)||
           (pir[a][k+1][pir[a][k][i][j].X][pir[a][k][i][j].Y].Clase==-1)))
           {

          if(((j-1)>=c)&&(pir[a][k][i][j-1].Homog==1)
             &&(pir[a][k][i][j-1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;
              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
                mask_sig[m][k][i-aux1][j-aux2-1].p=10;
             }
          }


          else if(((j-1)>=c)&&((i-1)>=a1)&&(pir[a][k][i-1][j-1].Homog==1)
              &&(pir[a][k][i-1][j-1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
                mask_sig[m][k][i-aux1-1][j-aux2-1].p=10;
             }
          }

          else if(((i-1)>=a1)&&(pir[a][k][i-1][j].Homog==1)
             &&(pir[a][k][i-1][j].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
                mask_sig[m][k][i-aux1-1][j-aux2].p=10;
             }
          }
          else if(((i-1)>=a1)&&((j+1)<=d)&&(pir[a][k][i-1][j+1].Homog==1)
             &&(pir[a][k][i-1][j+1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2+1)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
                mask_sig[m][k][i-aux1-1][j-aux2+1].p=10;
             }
          }
          else if(((j+1)<=d)&&(pir[a][k][i][j+1].Homog==1)
            &&(pir[a][k][i][j+1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2+1)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
                mask_sig[m][k][i-aux1][j-aux2+1].p=10;
             }
          }
          else if(((j+1)<=d) &&((i+1)<=b)&&(pir[a][k][i+1][j+1].Homog==1)
             &&(pir[a][k][i+1][j+1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1+1)<TAMMASCARA && (j-aux2+1)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
                mask_sig[m][k][i-aux1+1][j-aux2+1].p=10;
             }
          }
          else if(((i+1)<=b)&&(pir[a][k][i+1][j].Homog==1)
             &&(pir[a][k][i+1][j].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1+1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
                mask_sig[m][k][i-aux1+1][j-aux2].p=10;
             }
          }
          else if(((j-1)>=c)&&((i+1)<=b)&&(pir[a][k][i+1][j-1].Homog==1)
             &&(pir[a][k][i+1][j-1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1+1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
                mask_sig[m][k][i-aux1+1][j-aux2-1].p=10;
             }
          }


    }
  }
 }
}
}
void HermanamientoInversoNivel(PIR ****pir,int a, BOX **boxnivel, int *tam_i, int *tam_j, MASCARA **** mask_sig,int nivel, int *clases, int marc){

int i,j,k,l,m,a1,b,c,d;
int tamano_i,tamano_j;
int clase;
int aux1,aux2;

k=nivel;

tamano_i=tam_i[k];
tamano_j=tam_j[k];
if(marc==-1){
for(m=0;m<NUMMARCS;m++){
 clase=clases[m];

a1=boxnivel[k][m].imin-rango1[k][m];
if(a1<0)a1=0;
 b=boxnivel[k][m].imax+rango1[k][m];
if(b>=tamano_i)b=tamano_i-1;

aux1=boxnivel[k][m].icent-TAMMASCARA/2;
aux2=boxnivel[k][m].jcent-TAMMASCARA/2;

for (i=b;i>=a1;i--){
    c=boxnivel[k][m].jmin-rango1[k][m];
    if(c<0)c=0;
    d=boxnivel[k][m].jmax+rango1[k][m];
    if(d>=tamano_j)d=tamano_j-1;
    for(j=d;j>=c;j--){

        if ((pir[a][k][i][j].Homog==1)&&(pir[a][k][i][j].Clase==-1)
           &&((pir[a][k][i][j].X==-1)||
           (pir[a][k+1][pir[a][k][i][j].X][pir[a][k][i][j].Y].Clase==-1)))
           {

          if(((j-1)>=c)&&(pir[a][k][i][j-1].Homog==1)
             &&(pir[a][k][i][j-1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
             }
          }


          else if(((j-1)>=c)&&((i-1)>=a1)&&(pir[a][k][i-1][j-1].Homog==1)
              &&(pir[a][k][i-1][j-1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
             }
          }

          else if(((i-1)>=a1)&&(pir[a][k][i-1][j].Homog==1)
             &&(pir[a][k][i-1][j].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
             }
          }
          else if(((i-1)>=a1)&&((j+1)<=d)&&(pir[a][k][i-1][j+1].Homog==1)
             &&(pir[a][k][i-1][j+1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
             }
          }
          else if(((j+1)<=d)&&(pir[a][k][i][j+1].Homog==1)
            &&(pir[a][k][i][j+1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
             }
          }
          else if(((j+1)<=d) &&((i+1)<=b)&&(pir[a][k][i+1][j+1].Homog==1)
             &&(pir[a][k][i+1][j+1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
             }
          }
          else if(((i+1)<=b)&&(pir[a][k][i+1][j].Homog==1)
             &&(pir[a][k][i+1][j].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
             }
          }
          else if(((j-1)>=c)&&((i+1)<=b)&&(pir[a][k][i+1][j-1].Homog==1)
             &&(pir[a][k][i+1][j-1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
             }
          }


    }
  }
 }
}
}else{
m=marc;
 clase=clases[m];

a1=boxnivel[k][m].imin-rango1[k][m];
if(a1<0)a1=0;
 b=boxnivel[k][m].imax+rango1[k][m];
if(b>=tamano_i)b=tamano_i-1;

aux1=boxnivel[k][m].icent-TAMMASCARA/2;
aux2=boxnivel[k][m].jcent-TAMMASCARA/2;

for (i=b;i>=a1;i--){
    c=boxnivel[k][m].jmin-rango1[k][m];
    if(c<0)c=0;
    d=boxnivel[k][m].jmax+rango1[k][m];
    if(d>=tamano_j)d=tamano_j-1;
    for(j=d;j>=c;j--){

        if ((pir[a][k][i][j].Homog==1)&&(pir[a][k][i][j].Clase==-1)
           &&((pir[a][k][i][j].X==-1)||
           (pir[a][k+1][pir[a][k][i][j].X][pir[a][k][i][j].Y].Clase==-1)))
           {

          if(((j-1)>=c)&&(pir[a][k][i][j-1].Homog==1)
             &&(pir[a][k][i][j-1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
             }
          }


          else if(((j-1)>=c)&&((i-1)>=a1)&&(pir[a][k][i-1][j-1].Homog==1)
              &&(pir[a][k][i-1][j-1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
             }
          }

          else if(((i-1)>=a1)&&(pir[a][k][i-1][j].Homog==1)
             &&(pir[a][k][i-1][j].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
             }
          }
          else if(((i-1)>=a1)&&((j+1)<=d)&&(pir[a][k][i-1][j+1].Homog==1)
             &&(pir[a][k][i-1][j+1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
             }
          }
          else if(((j+1)<=d)&&(pir[a][k][i][j+1].Homog==1)
            &&(pir[a][k][i][j+1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
             }
          }
          else if(((j+1)<=d) &&((i+1)<=b)&&(pir[a][k][i+1][j+1].Homog==1)
             &&(pir[a][k][i+1][j+1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
             }
          }
          else if(((i+1)<=b)&&(pir[a][k][i+1][j].Homog==1)
             &&(pir[a][k][i+1][j].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
             }
          }
          else if(((j-1)>=c)&&((i+1)<=b)&&(pir[a][k][i+1][j-1].Homog==1)
             &&(pir[a][k][i+1][j-1].Clase==clase)){
              pir[a][k][i][j].Clase=clase;

              if((i-aux1)<TAMMASCARA && (j-aux2)<TAMMASCARA
                &&(j-aux2)>0&&(i-aux1)>0){

                mask_sig[m][k][i-aux1][j-aux2].marcar=255;
                mask_sig[m][k][i-aux1][j-aux2].piel=255;
                mask_sig[m][k][i-aux1][j-aux2].p=10;
             }
          }


    }
  }
 }
}
}



