#include <tracker/BIP.h>
#include <math.h>
#include <stdio.h>
#include <algorithm>
//#include <vcl.h>


BIP::BIP(int _altoim, int _anchoim, int _nivelesI, double umbral){

register int i,j,k;
int lado;

altoim=_altoim;
anchoim=_anchoim;
if (altoim>anchoim)
        lado=altoim;
else
        lado=anchoim;

double seis = log10((double)lado)/log10(2.0);
niveles=static_cast<int>(seis+1.0);

nivelesI=_nivelesI;

TamNivelesAlto=new int [niveles];
TamNivelesAncho=new int [niveles];
TamNivelesAlto[0]=altoim;
TamNivelesAncho[0]=anchoim;
for (i=1;i<niveles;i++)
        TamNivelesAlto[i]=TamNivelesAlto[i-1]/2;
TamNivelesAncho[0]=anchoim;
for (i=1;i<niveles;i++)
        TamNivelesAncho[i]=TamNivelesAncho[i-1]/2;


NodosV=new std::vector<NV> [nivelesI];
for(i=0;i<niveles;i++){
  NodosV[i].reserve(TamNivelesAlto[i]*TamNivelesAncho[i]/2);

}
pir=new PIRGRIS**[niveles];
for(i=0;i<niveles;i++){
        pir[i]=new PIRGRIS*[TamNivelesAlto[i]];
        for(j=0;j<TamNivelesAlto[i];j++)
                pir[i][j]=new PIRGRIS[TamNivelesAncho[i]];
}

for(i=0;i<altoim;i++){
        for(j=0;j<anchoim;j++){
                pir[0][i][j].X=-1;
                pir[0][i][j].Y=-1;
                pir[0][i][j].Homog=1;
                pir[0][i][j].Area=1;
                pir[0][i][j].Clase=-1;

        }
}
for(k=1;k<niveles;k++)
        for(i=0;i<TamNivelesAlto[k];i++)
                for(j=0;j<TamNivelesAncho[k];j++){
                        pir[k][i][j].X=-1;
                        pir[k][i][j].Y=-1;
                        pir[k][i][j].Homog=-1;
                        pir[k][i][j].Area=0;
                        pir[k][i][j].Clase=-1;
                }


umbralColor=new double[9];
umbralColor[0]=umbral;
umbralColor[1]=umbral;
umbralColor[2]=umbral;
umbralColor[3]=umbral;
umbralColor[4]=umbral;
umbralColor[5]=umbral;
umbralColor[6]=umbral;
umbralColor[7]=umbral;
umbralColor[8]=umbral;
}

BIP::~BIP(){
register int i,j,k;

delete(TamNivelesAlto);
delete(TamNivelesAncho);

for(i=0;i<niveles;i++){
        for(j=0;j<TamNivelesAlto[i];j++)
                delete(pir[i][j]);
        delete(pir[i]);
}
delete(pir);
delete(umbralColor);
}
/***********************************************************
GENERAR PARTE REGULAR: Genera la estructura 4 a 1 de la parte regular de la BIP
**********************************************************************/
int BIP::GenerarParteRegular(int nivel){

register int i,j,i1,j1,i2,j2,k;
int generado=-1;

k=nivel+1;

for(i=0;i<TamNivelesAlto[k];i++){
        i1=i*2;
        i2=i1+1;
        for(j=0;j<TamNivelesAncho[k];j++){
                j1=j*2;
                j2=j1+1;
                if(pir[nivel][i1][j1].Homog==1 && pir[nivel][i2][j1].Homog==1
                   &&pir[nivel][i1][j2].Homog==1 &&pir[nivel][i2][j2].Homog==1){

                        if(DisColor(pir[nivel][i1][j1],pir[nivel][i2][j1])<umbralColor[nivel]&&
                          DisColor(pir[nivel][i1][j1],pir[nivel][i1][j2])<umbralColor[nivel]&&
                           DisColor(pir[nivel][i1][j1],pir[nivel][i2][j2])<umbralColor[nivel]){

                                generado=1;

                                pir[k][i][j].Homog=1;
                                pir[k][i][j].Area=pir[nivel][i1][j1].Area+pir[nivel][i1][j2].Area
                                +pir[nivel][i2][j1].Area+pir[nivel][i2][j2].Area;
                                pir[k][i][j].I=((pir[nivel][i1][j1].I*pir[nivel][i1][j1].Area)+(pir[nivel][i1][j2].I*pir[nivel][i1][j2].Area)+
                                        (pir[nivel][i2][j1].I*pir[nivel][i2][j1].Area)+(pir[nivel][i2][j2].I*pir[nivel][i2][j2].Area))/pir[k][i][j].Area;

                                
             //Actualizo la posición del nodo padre en los 4 nodos del nivel
             //inferior
			
                                pir[nivel][i1][j1].Y=j;
		                pir[nivel][i1][j2].Y=j;
		                pir[nivel][i2][j1].Y=j;
                                pir[nivel][i2][j2].Y=j;
                                pir[nivel][i1][j1].X=i;
		                pir[nivel][i1][j2].X=i;
		                pir[nivel][i2][j1].X=i;
                                pir[nivel][i2][j2].X=i;

                        }

                   }
        }
}
return generado;
}

/**************************************************************************++++
CLASIFICAR: Asigna valores de clase a los nodos de la estructura sin padre y
distribuye estos valores entre sus hijos. Además almacena los colores de estos
padres raíz para generar la imagen segmentada.
************************************************************************************/
void BIP::Clasificar(){
register int i,j,k,l;
int homog,x,y;
ContClase=0;
Padre papa;

for(k=nivelesI-1;k>=niveles;k--){
 for(i=0;i<NodosV[k].size();i++){
   if(NodosV[k][i].padre==-1){

                NodosV[k][i].Clase=ContClase;
                ContClase++;
                papa.Area=NodosV[k][i].Area;
                papa.posi=-1;
                papa.posj=i;
                papa.Niv=k;
                papa.NodoV=1;
                papa.I=NodosV[k][i].I;
                padres.push_back(papa);

            } else if (NodosV[k][i].padre!=-1){

                NodosV[k][i].Clase=NodosV[k+1][NodosV[k][i].padre].Clase;
            }

    }
    }
  for (k=niveles-1;k>=0;k--)
    {
    for(i=0;i<NodosV[k].size();i++){
          if (NodosV[k][i].padre!=-1){

                NodosV[k][i].Clase=NodosV[k+1][NodosV[k][i].padre].Clase;
            }
          else if(NodosV[k][i].padre==-1){

                NodosV[k][i].Clase=ContClase;
                ContClase++;
                papa.Area=NodosV[k][i].Area;
                papa.posi=-1;
                papa.posj=i;
                papa.Niv=k;
                papa.NodoV=1;
                papa.I=NodosV[k][i].I;
                padres.push_back(papa);
            }
    }

    for (i=0;i<TamNivelesAlto[k];i++){
       for(j=0;j<TamNivelesAncho[k];j++)
	  {
          homog=pir[k][i][j].Homog;
          x=pir[k][i][j].X;
          y=pir[k][i][j].Y;

      if ((homog==1)&& x!=-1 && y!=-1)
		  {
		  /* Padre natural */

            pir[k][i][j].Clase=pir[k+1][x][y].Clase;

		  }
      else if (homog==1 && x==-1 && y!=-1){
                  pir[k][i][j].Clase=NodosV[k+1][y].Clase;

	  /* CASO 1: Clase nueva */
      }else if ((homog==1 && x==-1 && y==-1))
		{
                pir[k][i][j].Clase=ContClase;
                papa.Area=pir[k][i][j].Area;
                papa.posi=i;
                papa.posj=j;
                papa.Niv=k;
                papa.NodoV=-1;
                papa.I=pir[k][i][j].I;
                padres.push_back(papa);
	 	ContClase++;
        }

    }//for
    }//for
  }//for
}

/************************************************************
LLevarColoresArriba: Para propagar los colores de las regiones
generadas en la segmentación a traves de la estructura.

*************************************************************/

void BIP::LLevarColoresArriba(){

  register int i,j,k;

 //for(k=0;k<3;k++)
    for(i=0;i<TamNivelesAlto[0];i++)
      for(j=0;j<TamNivelesAncho[0];j++){

        if (pir[0][i][j].Homog==1)
	        pir[0][i][j].I=padres[pir[0][i][j].Clase].I;
        else pir[0][i][j].I=0;
      }


}
/************************************************************************************
CALCULAR ADYACENCIA REGULAR: Si dos nodos regulares están en contacto en un nivel
sus padres estarán en contacto en el nivel superior.
***************************************************************************************/
void CalcularAdyacenciaRegular(std::vector<NV> &nodos, PIRGRIS ***pir, int tamAlto, int tamAncho, int nivel){

register int i,j;
PUNTO coor,coor1;


for(i=0;i<tamAlto;i++){
   for(j=0;j<tamAncho;j++){
   
   //Si dos nodos regulares están en contacto y sus padres son uno un nodo virtual y el otro un nodo regular
  //entonces estos dos nodos padres están en contacto en el nivel superior
  if (pir[nivel][i][j].X!=-1 && pir[nivel][i][j].Y!=-1){ // nodo 1 padre regular nodo2 padre regular o virtual
    coor.i= pir[nivel][i][j].X;
    coor.j= pir[nivel][i][j].Y;

    if((j-1!=-1)&&pir[nivel][i][j-1].Y!=-1 &&pir[nivel][i][j-1].X==-1){
        //Para comprobar si el contacto estaba ya incluido
        if(std::find(pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosV.begin(),pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosV.end(),
          pir[nivel][i][j-1].Y)==pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosV.end()){
                pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosV.push_back(pir[nivel][i][j-1].Y);
          }
       if(std::find_if(nodos[pir[nivel][i][j-1].Y].contactosR.begin(),nodos[pir[nivel][i][j-1].Y].contactosR.end(),
          CmpContactos(coor))==nodos[pir[nivel][i][j-1].Y].contactosR.end()){
                nodos[pir[nivel][i][j-1].Y].contactosR.push_back(coor);
          }
      } else if (((j-1!=-1)&&pir[nivel][i][j-1].Y!=-1 &&pir[nivel][i][j-1].X!=-1)&&
                ((pir[nivel][i][j-1].X!=pir[nivel][i][j].X||pir[nivel][i][j-1].Y!=pir[nivel][i][j].Y)&&
                (pir[nivel][i][j-1].X!=pir[nivel][i][j].X-1||pir[nivel][i][j-1].Y!=pir[nivel][i][j].Y-1)&&
                (pir[nivel][i][j-1].X!=pir[nivel][i][j].X-1||pir[nivel][i][j-1].Y!=pir[nivel][i][j].Y)&&
                (pir[nivel][i][j-1].X!=pir[nivel][i][j].X-1||pir[nivel][i][j-1].Y!=pir[nivel][i][j].Y+1)&&
                (pir[nivel][i][j-1].X!=pir[nivel][i][j].X||pir[nivel][i][j-1].Y!=pir[nivel][i][j].Y-1)&&
                (pir[nivel][i][j-1].X!=pir[nivel][i][j].X||pir[nivel][i][j-1].Y!=pir[nivel][i][j].Y+1)&&
                (pir[nivel][i][j-1].X!=pir[nivel][i][j].X+1||pir[nivel][i][j-1].Y!=pir[nivel][i][j].Y-1)&&
                (pir[nivel][i][j-1].X!=pir[nivel][i][j].X+1||pir[nivel][i][j-1].Y!=pir[nivel][i][j].Y)&&
                (pir[nivel][i][j-1].X!=pir[nivel][i][j].X+1||pir[nivel][i][j-1].Y!=pir[nivel][i][j].Y+1))){

                coor1.i=pir[nivel][i][j-1].X;
                coor1.j=pir[nivel][i][j-1].Y;

                 if(std::find_if(pir[nivel+1][pir[nivel][i][j-1].X][pir[nivel][i][j-1].Y].contactosR.begin(),pir[nivel+1][pir[nivel][i][j-1].X][pir[nivel][i][j-1].Y].contactosR.end(),
                 CmpContactos(coor))==pir[nivel+1][pir[nivel][i][j-1].X][pir[nivel][i][j-1].Y].contactosR.end()){
                 pir[nivel+1][pir[nivel][i][j-1].X][pir[nivel][i][j-1].Y].contactosR.push_back(coor);
                 }
                 if(std::find_if(pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosR.begin(),pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosR.end(),
                 CmpContactos(coor1))==pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosR.end()){
                 pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosR.push_back(coor1);
                }
      }

      if((j-1!=-1)&&(i-1!=-1)&&pir[nivel][i-1][j-1].Y!=-1 &&pir[nivel][i-1][j-1].X==-1){
        //Para comprobar si el contacto estaba ya incluido
        if(std::find(pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosV.begin(),pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosV.end(),
          pir[nivel][i-1][j-1].Y)==pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosV.end()){
                pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosV.push_back(pir[nivel][i-1][j-1].Y);
          }
       if(std::find_if(nodos[pir[nivel][i-1][j-1].Y].contactosR.begin(),nodos[pir[nivel][i-1][j-1].Y].contactosR.end(),
          CmpContactos(coor))==nodos[pir[nivel][i-1][j-1].Y].contactosR.end()){
                nodos[pir[nivel][i-1][j-1].Y].contactosR.push_back(coor);
          }
      } else if (((j-1!=-1)&&(i-1!=-1)&&pir[nivel][i-1][j-1].Y!=-1 &&pir[nivel][i-1][j-1].X!=-1)&&
                ((pir[nivel][i-1][j-1].X!=pir[nivel][i][j].X||pir[nivel][i-1][j-1].Y!=pir[nivel][i][j].Y)&&
                (pir[nivel][i-1][j-1].X!=pir[nivel][i][j].X-1||pir[nivel][i-1][j-1].Y!=pir[nivel][i][j].Y-1)&&
                (pir[nivel][i-1][j-1].X!=pir[nivel][i][j].X-1||pir[nivel][i-1][j-1].Y!=pir[nivel][i][j].Y)&&
                (pir[nivel][i-1][j-1].X!=pir[nivel][i][j].X-1||pir[nivel][i-1][j-1].Y!=pir[nivel][i][j].Y+1)&&
                (pir[nivel][i-1][j-1].X!=pir[nivel][i][j].X||pir[nivel][i-1][j-1].Y!=pir[nivel][i][j].Y-1)&&
                (pir[nivel][i-1][j-1].X!=pir[nivel][i][j].X||pir[nivel][i-1][j-1].Y!=pir[nivel][i][j].Y+1)&&
                (pir[nivel][i-1][j-1].X!=pir[nivel][i][j].X+1||pir[nivel][i-1][j-1].Y!=pir[nivel][i][j].Y-1)&&
                (pir[nivel][i-1][j-1].X!=pir[nivel][i][j].X+1||pir[nivel][i-1][j-1].Y!=pir[nivel][i][j].Y)&&
                (pir[nivel][i-1][j-1].X!=pir[nivel][i][j].X+1||pir[nivel][i-1][j-1].Y!=pir[nivel][i][j].Y+1))){

                coor1.i=pir[nivel][i-1][j-1].X;
                coor1.j=pir[nivel][i-1][j-1].Y;

                 if(std::find_if(pir[nivel+1][pir[nivel][i-1][j-1].X][pir[nivel][i-1][j-1].Y].contactosR.begin(),pir[nivel+1][pir[nivel][i-1][j-1].X][pir[nivel][i-1][j-1].Y].contactosR.end(),
                 CmpContactos(coor))==pir[nivel+1][pir[nivel][i-1][j-1].X][pir[nivel][i-1][j-1].Y].contactosR.end()){
                 pir[nivel+1][pir[nivel][i-1][j-1].X][pir[nivel][i-1][j-1].Y].contactosR.push_back(coor);
                 }
                 if(std::find_if(pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosR.begin(),pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosR.end(),
                 CmpContactos(coor1))==pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosR.end()){
                 pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosR.push_back(coor1);
                }
      }


      if((i-1!=-1)&&pir[nivel][i-1][j].Y!=-1 &&pir[nivel][i-1][j].X==-1){
        //Para comprobar si el contacto estaba ya incluido
        if(std::find(pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosV.begin(),pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosV.end(),
          pir[nivel][i-1][j].Y)==pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosV.end()){
                pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosV.push_back(pir[nivel][i-1][j].Y);
          }
       if(std::find_if(nodos[pir[nivel][i-1][j].Y].contactosR.begin(),nodos[pir[nivel][i-1][j].Y].contactosR.end(),
          CmpContactos(coor))==nodos[pir[nivel][i-1][j].Y].contactosR.end()){
                nodos[pir[nivel][i-1][j].Y].contactosR.push_back(coor);
          }
      } else if (((i-1!=-1)&&pir[nivel][i-1][j].Y!=-1 &&pir[nivel][i-1][j].X!=-1)&&
                ((pir[nivel][i-1][j].X!=pir[nivel][i][j].X||pir[nivel][i-1][j].Y!=pir[nivel][i][j].Y)&&
                (pir[nivel][i-1][j].X!=pir[nivel][i][j].X-1||pir[nivel][i-1][j].Y!=pir[nivel][i][j].Y-1)&&
                (pir[nivel][i-1][j].X!=pir[nivel][i][j].X-1||pir[nivel][i-1][j].Y!=pir[nivel][i][j].Y)&&
                (pir[nivel][i-1][j].X!=pir[nivel][i][j].X-1||pir[nivel][i-1][j].Y!=pir[nivel][i][j].Y+1)&&
                (pir[nivel][i-1][j].X!=pir[nivel][i][j].X||pir[nivel][i-1][j].Y!=pir[nivel][i][j].Y-1)&&
                (pir[nivel][i-1][j].X!=pir[nivel][i][j].X||pir[nivel][i-1][j].Y!=pir[nivel][i][j].Y+1)&&
                (pir[nivel][i-1][j].X!=pir[nivel][i][j].X+1||pir[nivel][i-1][j].Y!=pir[nivel][i][j].Y-1)&&
                (pir[nivel][i-1][j].X!=pir[nivel][i][j].X+1||pir[nivel][i-1][j].Y!=pir[nivel][i][j].Y)&&
                (pir[nivel][i-1][j].X!=pir[nivel][i][j].X+1||pir[nivel][i-1][j].Y!=pir[nivel][i][j].Y+1))){

                coor1.i=pir[nivel][i-1][j].X;
                coor1.j=pir[nivel][i-1][j].Y;

                 if(std::find_if(pir[nivel+1][pir[nivel][i-1][j].X][pir[nivel][i-1][j].Y].contactosR.begin(),pir[nivel+1][pir[nivel][i-1][j].X][pir[nivel][i-1][j].Y].contactosR.end(),
                 CmpContactos(coor))==pir[nivel+1][pir[nivel][i-1][j].X][pir[nivel][i-1][j].Y].contactosR.end()){
                 pir[nivel+1][pir[nivel][i-1][j].X][pir[nivel][i-1][j].Y].contactosR.push_back(coor);
                 }
                 if(std::find_if(pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosR.begin(),pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosR.end(),
                 CmpContactos(coor1))==pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosR.end()){
                 pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosR.push_back(coor1);
                }
      }

      if((j+1!=tamAncho)&&(i-1!=-1)&&pir[nivel][i-1][j+1].Y!=-1 &&pir[nivel][i-1][j+1].X==-1){
        //Para comprobar si el contacto estaba ya incluido
        if(std::find(pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosV.begin(),pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosV.end(),
          pir[nivel][i-1][j+1].Y)==pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosV.end()){
                pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosV.push_back(pir[nivel][i-1][j+1].Y);
          }
       if(std::find_if(nodos[pir[nivel][i-1][j+1].Y].contactosR.begin(),nodos[pir[nivel][i-1][j+1].Y].contactosR.end(),
          CmpContactos(coor))==nodos[pir[nivel][i-1][j+1].Y].contactosR.end()){
                nodos[pir[nivel][i-1][j+1].Y].contactosR.push_back(coor);
          }
      }else if (((j+1!=tamAncho)&&(i-1!=-1)&&pir[nivel][i-1][j+1].Y!=-1 &&pir[nivel][i-1][j+1].X!=-1)&&
                ((pir[nivel][i-1][j+1].X!=pir[nivel][i][j].X||pir[nivel][i-1][j+1].Y!=pir[nivel][i][j].Y)&&
                (pir[nivel][i-1][j+1].X!=pir[nivel][i][j].X-1||pir[nivel][i-1][j+1].Y!=pir[nivel][i][j].Y-1)&&
                (pir[nivel][i-1][j+1].X!=pir[nivel][i][j].X-1||pir[nivel][i-1][j+1].Y!=pir[nivel][i][j].Y)&&
                (pir[nivel][i-1][j+1].X!=pir[nivel][i][j].X-1||pir[nivel][i-1][j+1].Y!=pir[nivel][i][j].Y+1)&&
                (pir[nivel][i-1][j+1].X!=pir[nivel][i][j].X||pir[nivel][i-1][j+1].Y!=pir[nivel][i][j].Y-1)&&
                (pir[nivel][i-1][j+1].X!=pir[nivel][i][j].X||pir[nivel][i-1][j+1].Y!=pir[nivel][i][j].Y+1)&&
                (pir[nivel][i-1][j+1].X!=pir[nivel][i][j].X+1||pir[nivel][i-1][j+1].Y!=pir[nivel][i][j].Y-1)&&
                (pir[nivel][i-1][j+1].X!=pir[nivel][i][j].X+1||pir[nivel][i-1][j+1].Y!=pir[nivel][i][j].Y)&&
                (pir[nivel][i-1][j+1].X!=pir[nivel][i][j].X+1||pir[nivel][i-1][j+1].Y!=pir[nivel][i][j].Y+1))){

                coor1.i=pir[nivel][i-1][j+1].X;
                coor1.j=pir[nivel][i-1][j+1].Y;

                 if(std::find_if(pir[nivel+1][pir[nivel][i-1][j+1].X][pir[nivel][i-1][j+1].Y].contactosR.begin(),pir[nivel+1][pir[nivel][i-1][j+1].X][pir[nivel][i-1][j+1].Y].contactosR.end(),
                 CmpContactos(coor))==pir[nivel+1][pir[nivel][i-1][j+1].X][pir[nivel][i-1][j+1].Y].contactosR.end()){
                 pir[nivel+1][pir[nivel][i-1][j+1].X][pir[nivel][i-1][j+1].Y].contactosR.push_back(coor);
                 }
                 if(std::find_if(pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosR.begin(),pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosR.end(),
                 CmpContactos(coor1))==pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosR.end()){
                 pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].contactosR.push_back(coor1);
                }

    }
   }
   else if(pir[nivel][i][j].X==-1&&pir[nivel][i][j].Y!=-1){ //nodo 1 padre virtual  nodo2 padre regular o virtual
        if((j-1!=-1)&&pir[nivel][i][j-1].Y!=-1 &&pir[nivel][i][j-1].X==-1 && pir[nivel][i][j].Y!=pir[nivel][i][j-1].Y){
        //Para comprobar si el contacto estaba ya incluido
        if(std::find(nodos[pir[nivel][i][j].Y].contactos.begin(),nodos[pir[nivel][i][j].Y].contactos.end(),
          pir[nivel][i][j-1].Y)==nodos[pir[nivel][i][j].Y].contactos.end()){
                nodos[pir[nivel][i][j].Y].contactos.push_back(pir[nivel][i][j-1].Y);
          }
       if(std::find(nodos[pir[nivel][i][j-1].Y].contactos.begin(),nodos[pir[nivel][i][j-1].Y].contactos.end(),
          pir[nivel][i][j].Y)==nodos[pir[nivel][i][j-1].Y].contactos.end()){
                nodos[pir[nivel][i][j-1].Y].contactos.push_back(pir[nivel][i][j].Y);
          }
      } else if((j-1!=-1)&&pir[nivel][i][j-1].Y!=-1 &&pir[nivel][i][j-1].X!=-1){
        coor.i=pir[nivel][i][j-1].X;
        coor.j=pir[nivel][i][j-1].Y;
        if(std::find(pir[nivel+1][pir[nivel][i][j-1].X][pir[nivel][i][j-1].Y].contactosV.begin(),pir[nivel+1][pir[nivel][i][j-1].X][pir[nivel][i][j-1].Y].contactosV.end(),
          pir[nivel][i][j].Y)==pir[nivel+1][pir[nivel][i][j-1].X][pir[nivel][i][j-1].Y].contactosV.end()){
                pir[nivel+1][pir[nivel][i][j-1].X][pir[nivel][i][j-1].Y].contactosV.push_back(pir[nivel][i][j].Y);
          }
       if(std::find_if(nodos[pir[nivel][i][j].Y].contactosR.begin(),nodos[pir[nivel][i][j].Y].contactosR.end(),
          CmpContactos(coor))==nodos[pir[nivel][i][j].Y].contactosR.end()){
                nodos[pir[nivel][i][j].Y].contactosR.push_back(coor);
          }
      }

        if((j-1!=-1)&&(i-1!=-1)&&
        pir[nivel][i-1][j-1].Y!=-1 &&pir[nivel][i-1][j-1].X==-1 && pir[nivel][i][j].Y!=pir[nivel][i-1][j-1].Y){
           if(std::find(nodos[pir[nivel][i][j].Y].contactos.begin(),nodos[pir[nivel][i][j].Y].contactos.end(),
                pir[nivel][i-1][j-1].Y)==nodos[pir[nivel][i][j].Y].contactos.end()){
                nodos[pir[nivel][i][j].Y].contactos.push_back(pir[nivel][i-1][j-1].Y);
          }
         if(std::find(nodos[pir[nivel][i-1][j-1].Y].contactos.begin(),nodos[pir[nivel][i-1][j-1].Y].contactos.end(),
          pir[nivel][i][j].Y)==nodos[pir[nivel][i-1][j-1].Y].contactos.end()){
                nodos[pir[nivel][i-1][j-1].Y].contactos.push_back(pir[nivel][i][j].Y);
          }
      } else if((j-1!=-1)&&(i-1!=-1)&&pir[nivel][i-1][j-1].Y!=-1 &&pir[nivel][i-1][j-1].X!=-1){
        coor.i=pir[nivel][i-1][j-1].X;
        coor.j=pir[nivel][i-1][j-1].Y;
        if(std::find(pir[nivel+1][pir[nivel][i-1][j-1].X][pir[nivel][i-1][j-1].Y].contactosV.begin(),pir[nivel+1][pir[nivel][i-1][j-1].X][pir[nivel][i-1][j-1].Y].contactosV.end(),
          pir[nivel][i][j].Y)==pir[nivel+1][pir[nivel][i-1][j-1].X][pir[nivel][i-1][j-1].Y].contactosV.end()){
                pir[nivel+1][pir[nivel][i-1][j-1].X][pir[nivel][i-1][j-1].Y].contactosV.push_back(pir[nivel][i][j].Y);
          }
       if(std::find_if(nodos[pir[nivel][i][j].Y].contactosR.begin(),nodos[pir[nivel][i][j].Y].contactosR.end(),
          CmpContactos(coor))==nodos[pir[nivel][i][j].Y].contactosR.end()){
                nodos[pir[nivel][i][j].Y].contactosR.push_back(coor);
          }
      }


      if((i-1!=-1)&&pir[nivel][i-1][j].Y!=-1 &&pir[nivel][i-1][j].X==-1 && pir[nivel][i][j].Y!=pir[nivel][i-1][j].Y){
        if(std::find(nodos[pir[nivel][i][j].Y].contactos.begin(),nodos[pir[nivel][i][j].Y].contactos.end(),
          pir[nivel][i-1][j].Y)==nodos[pir[nivel][i][j].Y].contactos.end()){
                nodos[pir[nivel][i][j].Y].contactos.push_back(pir[nivel][i-1][j].Y);
           }
          if(std::find(nodos[pir[nivel][i-1][j].Y].contactos.begin(),nodos[pir[nivel][i-1][j].Y].contactos.end(),
          pir[nivel][i][j].Y)==nodos[pir[nivel][i-1][j].Y].contactos.end()){
                nodos[pir[nivel][i-1][j].Y].contactos.push_back(pir[nivel][i][j].Y);
          }
      } else if((i-1!=-1)&&pir[nivel][i-1][j].Y!=-1 &&pir[nivel][i-1][j].X!=-1){
        coor.i=pir[nivel][i-1][j].X;
        coor.j=pir[nivel][i-1][j].Y;
        if(std::find(pir[nivel+1][pir[nivel][i-1][j].X][pir[nivel][i-1][j].Y].contactosV.begin(),pir[nivel+1][pir[nivel][i-1][j].X][pir[nivel][i-1][j].Y].contactosV.end(),
          pir[nivel][i][j].Y)==pir[nivel+1][pir[nivel][i-1][j].X][pir[nivel][i-1][j].Y].contactosV.end()){
                pir[nivel+1][pir[nivel][i-1][j].X][pir[nivel][i-1][j].Y].contactosV.push_back(pir[nivel][i][j].Y);
          }
       if(std::find_if(nodos[pir[nivel][i][j].Y].contactosR.begin(),nodos[pir[nivel][i][j].Y].contactosR.end(),
          CmpContactos(coor))==nodos[pir[nivel][i][j].Y].contactosR.end()){
                nodos[pir[nivel][i][j].Y].contactosR.push_back(coor);
          }
      }

      if((j+1!=tamAncho)&&(i-1!=-1)&&
        pir[nivel][i-1][j+1].Y!=-1 &&pir[nivel][i-1][j+1].X==-1 && pir[nivel][i][j].Y!=pir[nivel][i-1][j+1].Y){
        if(std::find(nodos[pir[nivel][i][j].Y].contactos.begin(),nodos[pir[nivel][i][j].Y].contactos.end(),
          pir[nivel][i-1][j+1].Y)==nodos[pir[nivel][i][j].Y].contactos.end()){
                nodos[pir[nivel][i][j].Y].contactos.push_back(pir[nivel][i-1][j+1].Y);
          }
        if(std::find(nodos[pir[nivel][i-1][j+1].Y].contactos.begin(),nodos[pir[nivel][i-1][j+1].Y].contactos.end(),
          pir[nivel][i][j].Y)==nodos[pir[nivel][i-1][j+1].Y].contactos.end()){
                nodos[pir[nivel][i-1][j+1].Y].contactos.push_back(pir[nivel][i][j].Y);
          }
      } else if((j+1!=tamAncho)&&(i-1!=-1)&&pir[nivel][i-1][j+1].Y!=-1 &&pir[nivel][i-1][j+1].X!=-1){
        coor.i=pir[nivel][i-1][j+1].X;
        coor.j=pir[nivel][i-1][j+1].Y;
        if(std::find(pir[nivel+1][pir[nivel][i-1][j+1].X][pir[nivel][i-1][j+1].Y].contactosV.begin(),pir[nivel+1][pir[nivel][i-1][j+1].X][pir[nivel][i-1][j+1].Y].contactosV.end(),
          pir[nivel][i][j].Y)==pir[nivel+1][pir[nivel][i-1][j+1].X][pir[nivel][i-1][j+1].Y].contactosV.end()){
                pir[nivel+1][pir[nivel][i-1][j+1].X][pir[nivel][i-1][j+1].Y].contactosV.push_back(pir[nivel][i][j].Y);
          }
       if(std::find_if(nodos[pir[nivel][i][j].Y].contactosR.begin(),nodos[pir[nivel][i][j].Y].contactosR.end(),
          CmpContactos(coor))==nodos[pir[nivel][i][j].Y].contactosR.end()){
                nodos[pir[nivel][i][j].Y].contactosR.push_back(coor);
          }
      } //else if
      } //else if
   } //for
  } //for
}

/******************************************************************************************

********************************************************************************************************/
void CalcularAdyacenciaVirtual(PIRGRIS ***pir, std::vector<NV> &nodos, std::vector<NV> nodosinf, int nivel, int solovirtual,int alto,int ancho){

register int i,j;
PUNTO coor;

for(i=0;i<nodosinf.size();i++){
//Si dos nodos virtuales están en contacto en un nivel sus padres estarán en contacto
//en el nivel superior
   if(nodosinf[i].padre!=-1){
      for(j=0;j<nodosinf[i].contactos.size();j++){
         if(nodosinf[nodosinf[i].contactos[j]].padre!=-1&&nodosinf[i].padre!=nodosinf[nodosinf[i].contactos[j]].padre){
         if(std::find(nodos[nodosinf[i].padre].contactos.begin(),nodos[nodosinf[i].padre].contactos.end(),
          nodosinf[nodosinf[i].contactos[j]].padre)==nodos[nodosinf[i].padre].contactos.end())
            nodos[nodosinf[i].padre].contactos.push_back(nodosinf[nodosinf[i].contactos[j]].padre);

         if(std::find(nodos[nodosinf[nodosinf[i].contactos[j]].padre].contactos.begin(),nodos[nodosinf[nodosinf[i].contactos[j]].padre].contactos.end(),
          nodosinf[i].padre)==nodos[nodosinf[nodosinf[i].contactos[j]].padre].contactos.end())
            nodos[nodosinf[nodosinf[i].contactos[j]].padre].contactos.push_back(nodosinf[i].padre);
         }//if
      } //for
      if(!solovirtual){
       for(j=0;j<nodosinf[i].contactosR.size();j++){
          if(pir[nivel][(int)nodosinf[i].contactosR[j].i][(int)nodosinf[i].contactosR[j].j].X==-1&&
                pir[nivel][(int)nodosinf[i].contactosR[j].i][(int)nodosinf[i].contactosR[j].j].Y!=-1&&
                pir[nivel][(int)nodosinf[i].contactosR[j].i][(int)nodosinf[i].contactosR[j].j].Y!=nodosinf[i].padre){

          if(std::find(nodos[nodosinf[i].padre].contactos.begin(),nodos[nodosinf[i].padre].contactos.end(),
          pir[nivel][(int)nodosinf[i].contactosR[j].i][(int)nodosinf[i].contactosR[j].j].Y)==nodos[nodosinf[i].padre].contactos.end())
            nodos[nodosinf[i].padre].contactos.push_back(pir[nivel][(int)nodosinf[i].contactosR[j].i][(int)nodosinf[i].contactosR[j].j].Y);

         if(std::find(nodos[pir[nivel][(int)nodosinf[i].contactosR[j].i][(int)nodosinf[i].contactosR[j].j].Y].contactos.begin(),nodos[pir[nivel][(int)nodosinf[i].contactosR[j].i][(int)nodosinf[i].contactosR[j].j].Y].contactos.end(),
          nodosinf[i].padre)==nodos[pir[nivel][(int)nodosinf[i].contactosR[j].i][(int)nodosinf[i].contactosR[j].j].Y].contactos.end())
            nodos[pir[nivel][(int)nodosinf[i].contactosR[j].i][(int)nodosinf[i].contactosR[j].j].Y].contactos.push_back(nodosinf[i].padre);
         }//if
         else if(pir[nivel][(int)nodosinf[i].contactosR[j].i][(int)nodosinf[i].contactosR[j].j].X!=-1&&
                pir[nivel][(int)nodosinf[i].contactosR[j].i][(int)nodosinf[i].contactosR[j].j].Y!=-1){

                coor.i= pir[nivel][(int)nodosinf[i].contactosR[j].i][(int)nodosinf[i].contactosR[j].j].X;
                coor.j=pir[nivel][(int)nodosinf[i].contactosR[j].i][(int)nodosinf[i].contactosR[j].j].Y;
                
          if(std::find(pir[nivel+1][(int)coor.i][(int)coor.j].contactosV.begin(),
          pir[nivel+1][(int)coor.i][(int)coor.j].contactosV.end(),
          nodosinf[i].padre)==pir[nivel+1][(int)coor.i][(int)coor.j].contactosV.end())
            pir[nivel+1][(int)coor.i][(int)coor.j].contactosV.push_back(nodosinf[i].padre);

          if(std::find_if(nodos[nodosinf[i].padre].contactosR.begin(),nodos[nodosinf[i].padre].contactosR.end(),
          CmpContactos(coor))==nodos[nodosinf[i].padre].contactosR.end()){
                nodos[nodosinf[i].padre].contactosR.push_back(coor);
          }//if
         }//else if
       }//for
       }
   }//if
}//for
}
/***************************************************************************************************
GENERAR PARTE IRREGULAR:  Realiza la etapa de parent search e intralevel twinning entre nodos regulares.
También realiza las etapas de virtual parent search and virtual vertices linking  entre nodos virtuales.
*******************************************************************************************************/

void BIP::GenerarParteIrregular(int nivel, double umbral){
register int i,j,l;
std::vector<DIST> dist;
DIST aux_dist;
int lmin,lminV,distmin,distminV;
long int cont=0;
NV nodo;
std::vector<double> distV;
std::vector<int> pos;
std::vector<double> distR;
std::vector<PUNTO> posR;
PUNTO aux;
PUNTO lminV1;
int tamanoAlto;
int tamanoAncho;

//Se empieza con la parte de la jerarquía que contiene nodos regulares y virtuales
if(nivel<niveles-1){
tamanoAlto=TamNivelesAlto[nivel];
tamanoAncho=TamNivelesAncho[nivel];

if(nivel>0){
 //Lo primero es ver que nodos virtuales están en contacto

         CalcularAdyacenciaRegular(NodosV[nivel],pir,TamNivelesAlto[nivel-1],TamNivelesAncho[nivel-1],nivel-1);

        if (nivel>1)

         CalcularAdyacenciaVirtual(pir,NodosV[nivel],NodosV[nivel-1],nivel-1,0,TamNivelesAlto[nivel],TamNivelesAncho[nivel]);
}

//Un nodo regular sin padre busca entre sus vecinos regulares o irregulares el mas parecido a el y que cumpla el umbral
//y que tenga padre para unirse a el. Si no lo encuentra buscara entre sus vecinos regulares o irregulares sin padre
//para generar un nodo virtual

//Primero busco entre los regulares
//Primero en la vecindad 8
 for(i=0;i<TamNivelesAlto[nivel];i++)
        for(j=0;j<TamNivelesAncho[nivel];j++){

        dist.clear();
        
          if(pir[nivel][i][j].Homog==1&&pir[nivel][i][j].X==-1 && pir[nivel][i][j].Y==-1){
                if(((j-1)!=-1)&&
                (pir[nivel][i][j-1].Homog==1)){
                        aux_dist.dif=DisColor(pir[nivel][i][j],pir[nivel][i][j-1]);
                        aux_dist.x=i;
                        aux_dist.y=j-1;
                        dist.push_back(aux_dist);
                }

                if(((j-1)!=-1)&&((i-1)!=-1)
                &&(pir[nivel][i-1][j-1].Homog==1)){
                        aux_dist.dif=DisColor(pir[nivel][i][j],pir[nivel][i-1][j-1]);
                        aux_dist.x=i-1;
                        aux_dist.y=j-1;
                        dist.push_back(aux_dist);
                }

                if(((i-1)!=-1)
                &&(pir[nivel][i-1][j].Homog==1)){
                        aux_dist.dif=DisColor(pir[nivel][i][j],pir[nivel][i-1][j]);
                        aux_dist.x=i-1;
                        aux_dist.y=j;
                        dist.push_back(aux_dist);
                }
                if(((i-1)!=-1)&&((j+1)!=tamanoAncho)
                &&(pir[nivel][i-1][j+1].Homog==1)){
                        aux_dist.dif=DisColor(pir[nivel][i][j],pir[nivel][i-1][j+1]);
                        aux_dist.x=i-1;
                        aux_dist.y=j+1;
                        dist.push_back(aux_dist);
                }
                if(((j+1)!=tamanoAncho)
                &&(pir[nivel][i][j+1].Homog==1)){
                        aux_dist.dif=DisColor(pir[nivel][i][j],pir[nivel][i][j+1]);
                        aux_dist.x=i;
                        aux_dist.y=j+1;
                        dist.push_back(aux_dist);
                }
                if(((j+1)!=tamanoAncho) &&((i+1)!=tamanoAlto)
                &&(pir[nivel][i+1][j+1].Homog==1)){
                        aux_dist.dif=DisColor(pir[nivel][i][j],pir[nivel][i+1][j+1]);
                        aux_dist.x=i+1;
                        aux_dist.y=j+1;
                        dist.push_back(aux_dist);
                }
                if(((i+1)!=tamanoAlto)
                &&(pir[nivel][i+1][j].Homog==1)){
                        aux_dist.dif=DisColor(pir[nivel][i][j],pir[nivel][i+1][j]);
                        aux_dist.x=i+1;
                        aux_dist.y=j;
                        dist.push_back(aux_dist);
                }
                if(((j-1)!=-1)&&((i+1)!=tamanoAlto)
                &&(pir[nivel][i+1][j-1].Homog==1)){
                        aux_dist.dif=DisColor(pir[nivel][i][j],pir[nivel][i+1][j-1]);
                        aux_dist.x=i+1;
                        aux_dist.y=j-1;
                        dist.push_back(aux_dist);
                }
 //Ahora entre sus vecinos regulares que no estan en vecindad 8

       for (l=0;l<pir[nivel][i][j].contactosR.size();l++){
                aux_dist.dif=DisColor(pir[nivel][i][j],
                        pir[nivel][pir[nivel][i][j].contactosR[l].i][pir[nivel][i][j].contactosR[l].j]);
                aux_dist.x=pir[nivel][i][j].contactosR[l].i;
                aux_dist.y=pir[nivel][i][j].contactosR[l].j;
                dist.push_back(aux_dist);
        }


          distmin=10000.0;
          lmin=-1;
          lminV=-1;


       /*Entre los vecinos con padre buscamos el que tiene distancia
       minima en nivel de color con el nodo y cumple que esta distancia
       es menor que el umbral, y pegamos el nodo al padre del vecino*/


          for (l=0;l<dist.size();l++){

         // ShowMessage(FloatToStr(dist[l].dif));
             //Aqui busca los regulares con padre
               if(pir[nivel][dist[l].x][dist[l].y].Y!=-1 && dist[l].dif<distmin &&
                   dist[l].dif<umbral){
                   distmin=dist[l].dif;
                   lmin=l;

               }
          }


          //Ahora voy a buscar entre los irregulares

          lminV=-1;
          distminV=10000.0;
         for(l=0; l<pir[nivel][i][j].contactosV.size();l++){
                   if((NodosV[nivel][pir[nivel][i][j].contactosV[l]].padre!=-1&&(DisColor(pir[nivel][i][j],NodosV[nivel][pir[nivel][i][j].contactosV[l]]))<distminV)&&
                   (DisColor(pir[nivel][i][j],NodosV[nivel][pir[nivel][i][j].contactosV[l]]))<umbral){
                        distminV=DisColor(pir[nivel][i][j],NodosV[nivel][pir[nivel][i][j].contactosV[l]]);
                        lminV=pir[nivel][i][j].contactosV[l];
                   }
          }

          /*Se pega al padre del vecino*/
           if (lmin!=-1&&lminV!=-1){
                if (distmin<=distminV){

                      pir[nivel][i][j].X=pir[nivel][dist[lmin].x][dist[lmin].y].X;
                      pir[nivel][i][j].Y=pir[nivel][dist[lmin].x][dist[lmin].y].Y;


                      if(pir[nivel][i][j].X==-1 && pir[nivel][i][j].Y!=-1){//El padre es nodo virtual

                        nodo=CrearNodoVirtual(pir[nivel][i][j],NodosV[nivel+1][pir[nivel][i][j].Y]);
                        NodosV[nivel+1][pir[nivel][i][j].Y].Area= nodo.Area;
                        NodosV[nivel+1][pir[nivel][i][j].Y].I=nodo.I;

                      }else if (pir[nivel][i][j].X!=-1 &&pir[nivel][i][j].Y!=-1){  //el padre es un nodo de la estructura

                        pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].I=
                        ((pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].I
                        *pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].Area)
                        +(pir[nivel][i][j].I*pir[nivel][i][j].Area))/(pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].Area
                        +pir[nivel][i][j].Area);

                        pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].Area=
                        pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].Area
                        +pir[nivel][i][j].Area;



                      }
                }else {
                        pir[nivel][i][j].X=-1;
                        pir[nivel][i][j].Y=NodosV[nivel][lminV].padre;
                        nodo=CrearNodoVirtual(pir[nivel][i][j],NodosV[nivel+1][pir[nivel][i][j].Y]);
                        NodosV[nivel+1][pir[nivel][i][j].Y].Area= nodo.Area;
                        NodosV[nivel+1][pir[nivel][i][j].Y].I=nodo.I;
                }
           }else if(lminV==-1 &&lmin!=-1){

                      pir[nivel][i][j].X=pir[nivel][dist[lmin].x][dist[lmin].y].X;
                      pir[nivel][i][j].Y=pir[nivel][dist[lmin].x][dist[lmin].y].Y;


                      if(pir[nivel][i][j].X==-1 && pir[nivel][i][j].Y!=-1){//El padre es nodo virtual

                        nodo=CrearNodoVirtual(pir[nivel][i][j],NodosV[nivel+1][pir[nivel][i][j].Y]);
                       NodosV[nivel+1][pir[nivel][i][j].Y].Area= nodo.Area;
                        NodosV[nivel+1][pir[nivel][i][j].Y].I=nodo.I;

                      }else if (pir[nivel][i][j].X!=-1 &&pir[nivel][i][j].Y!=-1){  //el padre es un nodo de la estructura

                        pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].I=
                        ((pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].I*pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].Area)
                        +(pir[nivel][i][j].I*pir[nivel][i][j].Area))/(pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].Area
                        +pir[nivel][i][j].Area);

                        pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].Area=
                        pir[nivel+1][pir[nivel][i][j].X][pir[nivel][i][j].Y].Area
                        +pir[nivel][i][j].Area;

                      }
           }else if(lmin==-1&&lminV!=-1){
                        pir[nivel][i][j].X=-1;
                        pir[nivel][i][j].Y=NodosV[nivel][lminV].padre;
                        nodo=CrearNodoVirtual(pir[nivel][i][j],NodosV[nivel+1][pir[nivel][i][j].Y]);
                        NodosV[nivel+1][pir[nivel][i][j].Y].Area= nodo.Area;
                        NodosV[nivel+1][pir[nivel][i][j].Y].I=nodo.I;

           } else if (lmin==-1 && lminV==-1){

             distmin=10000.0;
             lmin=-1;

             for (l=0;l<dist.size();l++){
                //Aqui como no encontro nodo con padre, busca uno sin padre
                 if(pir[nivel][dist[l].x][dist[l].y].Y==-1 && dist[l].dif<distmin &&
                    dist[l].dif<umbral){
                     distmin=dist[l].dif;
                     lmin=l;
                 }
             }

          lminV=-1;
          distminV=10000.0;
         for(l=0; l<pir[nivel][i][j].contactosV.size();l++){
                   if((NodosV[nivel][pir[nivel][i][j].contactosV[l]].padre==-1&&(DisColor(pir[nivel][i][j],NodosV[nivel][pir[nivel][i][j].contactosV[l]]))<distminV)&&
                   (DisColor(pir[nivel][i][j],NodosV[nivel][pir[nivel][i][j].contactosV[l]]))<umbral){
                        distminV=DisColor(pir[nivel][i][j],NodosV[nivel][pir[nivel][i][j].contactosV[l]]);
                        lminV=pir[nivel][i][j].contactosV[l];
                   }
          }

          if (lmin!=-1 && lminV!=-1){
                if (distmin<=distminV){
                   nodo=CrearNodoVirtual(pir[nivel][i][j],
                               pir[nivel][dist[lmin].x][dist[lmin].y]);
                   NodosV[nivel+1].push_back(nodo);
                   pir[nivel][i][j].X=-1;
                   pir[nivel][i][j].Y=cont;
                   pir[nivel][dist[lmin].x][dist[lmin].y].X=-1;
                   pir[nivel][dist[lmin].x][dist[lmin].y].Y=cont;
                   cont++;

                }else {
                   nodo=CrearNodoVirtual(pir[nivel][i][j],
                               NodosV[nivel][lminV]);
                   NodosV[nivel+1].push_back(nodo);
                   pir[nivel][i][j].X=-1;
                   pir[nivel][i][j].Y=cont;
                   NodosV[nivel][lminV].padre=cont;
                   cont++;
                }

          } else if(lmin!=-1&&lminV==-1){
                   nodo=CrearNodoVirtual(pir[nivel][i][j],
                               pir[nivel][dist[lmin].x][dist[lmin].y]);
                   NodosV[nivel+1].push_back(nodo);
                   pir[nivel][i][j].X=-1;
                   pir[nivel][i][j].Y=cont;
                   pir[nivel][dist[lmin].x][dist[lmin].y].X=-1;
                   pir[nivel][dist[lmin].x][dist[lmin].y].Y=cont;
                   cont++;
          } else if(lmin==-1&&lminV!=-1){
                   nodo=CrearNodoVirtual(pir[nivel][i][j],
                               NodosV[nivel][lminV]);
                   NodosV[nivel+1].push_back(nodo);
                   pir[nivel][i][j].X=-1;
                   pir[nivel][i][j].Y=cont;
                   NodosV[nivel][lminV].padre=cont;
                   cont++;
       }
     }
  }
 }

//Un nodo virtual sin padre busca entre sus vecinos virtuales el mas parecido a el y que cumpla el umbral
//y que tenga padre para unirse a el. Si no lo encuentra buscara entre sus vecinos virtuales sin padre
//para generar un nodo virtual


 if(nivel>0){
 //Lo primero es ver que nodos virtuales están en contacto

        for(i=0;i<NodosV[nivel].size();i++){
                if (NodosV[nivel][i].padre==-1){

                distV.clear();
                pos.clear();
                    for(j=0;j<NodosV[nivel][i].contactos.size();j++){
                        distV.push_back(DisColor(NodosV[nivel][i],NodosV[nivel][NodosV[nivel][i].contactos[j]]));
                        pos.push_back(NodosV[nivel][i].contactos[j]);
                    }

                distR.clear();
                posR.clear();

                     for(j=0;j<NodosV[nivel][i].contactosR.size();j++){

                        distR.push_back(DisColor(NodosV[nivel][i],pir[nivel][NodosV[nivel][i].contactosR[j].i][NodosV[nivel][i].contactosR[j].j]));
                        aux.i=NodosV[nivel][i].contactosR[j].i;
                        aux.j=NodosV[nivel][i].contactosR[j].j;
                        posR.push_back(aux);
                     }

                distmin=10000.0;
                lmin=-1;
                distminV=10000.0;
                lminV1.i=-1;
                lminV1.j=-1;
                       for(l=0;l<distV.size();l++){
                        //Busca entre los no-huérfanos
                                if(distV[l]<distmin && NodosV[nivel][pos[l]].padre!=-1 &&
                                distV[l]<umbral){
                                     distmin=distV[l];
                                     lmin=pos[l];
                                }
                        }

                       for(l=0;l<distR.size();l++){

                                if(distR[l]<distmin && pir[nivel][posR[l].i][posR[l].j].X==-1
                                &&pir[nivel][posR[l].i][posR[l].j].Y!=-1 &&
                                distR[l]<umbral){
                                     distminV=distR[l];
                                     lminV1.i=posR[l].i;
                                     lminV1.j=posR[l].j;
                                }
                        }

                        if (lmin!=-1 && lminV1.i!=-1){
                                if (distmin<=distminV){
                                    NodosV[nivel][i].padre=NodosV[nivel][lmin].padre;
                                    //Esto es para la media de color
                                    nodo=CrearNodoVirtual(NodosV[nivel][i],NodosV[nivel+1][NodosV[nivel][i].padre]);
                                    NodosV[nivel+1][NodosV[nivel][i].padre].Area= nodo.Area;
                                    NodosV[nivel+1][NodosV[nivel][i].padre].I=nodo.I;

                                }else{
                                    NodosV[nivel][i].padre=pir[nivel][lminV1.i][lminV1.j].Y;
                                    nodo=CrearNodoVirtual(NodosV[nivel][i],NodosV[nivel+1][NodosV[nivel][i].padre]);
                                    NodosV[nivel+1][NodosV[nivel][i].padre].Area=nodo.Area;
                                    NodosV[nivel+1][NodosV[nivel][i].padre].I=nodo.I;
                                }
                        }else if(lmin!=-1&&lminV1.i==-1){
                                    NodosV[nivel][i].padre=NodosV[nivel][lmin].padre;
                                    //Esto es para la media de color
                                    nodo=CrearNodoVirtual(NodosV[nivel][i],NodosV[nivel+1][NodosV[nivel][i].padre]);
                                    NodosV[nivel+1][NodosV[nivel][i].padre].Area= nodo.Area;
                                    NodosV[nivel+1][NodosV[nivel][i].padre].I=nodo.I;
                        } else if(lmin==-1&&lminV1.i!=-1){
                                    NodosV[nivel][i].padre=pir[nivel][lminV1.i][lminV1.j].Y;
                                    nodo=CrearNodoVirtual(NodosV[nivel][i],NodosV[nivel+1][NodosV[nivel][i].padre]);
                                    NodosV[nivel+1][NodosV[nivel][i].padre].Area=nodo.Area;
                                    NodosV[nivel+1][NodosV[nivel][i].padre].I=nodo.I;

                        }else if (lmin==-1&&lminV1.i==-1){
                               distmin=10000.0;
                               distminV=10000.0;
                               lminV1.i=-1;


                        for(l=0;l<distV.size();l++){
                        //Busca entre los huérfanos
                                 if(distV[l]<distmin && NodosV[nivel][pos[l]].padre==-1 &&
                                 distV[l]<umbral){
                                        distmin=distV[l];
                                        lmin=pos[l];
                                 }
                                }

                                for(l=0;l<distR.size();l++){
                                        if(distR[l]<distmin && pir[nivel][posR[l].i][posR[l].j].X==-1
                                        &&pir[nivel][posR[l].i][posR[l].j].Y==-1 &&
                                        distR[l]<umbral){
                                                distminV=distR[l];
                                                lminV1.i=posR[l].i;
                                                lminV1.j=posR[l].j;
                                        }
                                }

                                if (lmin!=-1&&lminV1.i!=-1){
                                        if(distmin<=distminV){
                                              nodo=CrearNodoVirtual(NodosV[nivel][i],NodosV[nivel][lmin]);
                                              NodosV[nivel+1].push_back(nodo);
                                              NodosV[nivel][i].padre=cont;
                                              NodosV[nivel][lmin].padre=cont;
                                              cont++;
                                        } else{
                                              nodo=CrearNodoVirtual(NodosV[nivel][i],pir[nivel][lminV1.i][lminV1.j]);
                                              NodosV[nivel+1].push_back(nodo);
                                              NodosV[nivel][i].padre=cont;
                                              pir[nivel][lminV1.i][lminV1.j].X=-1;
                                              pir[nivel][lminV1.i][lminV1.j].Y=cont;
                                              cont++;
                                        }
                                }else if(lmin!=-1&&lminV1.i==-1){
                                       nodo=CrearNodoVirtual(NodosV[nivel][i],NodosV[nivel][lmin]);
                                       NodosV[nivel+1].push_back(nodo);
                                       NodosV[nivel][i].padre=cont;
                                       NodosV[nivel][lmin].padre=cont;
                                       cont++;
                                }else if(lmin==-1&&lminV1.i!=-1){
                                       nodo=CrearNodoVirtual(NodosV[nivel][i],pir[nivel][lminV1.i][lminV1.j]);
                                       NodosV[nivel+1].push_back(nodo);
                                       NodosV[nivel][i].padre=cont;
                                       pir[nivel][lminV1.i][lminV1.j].X=-1;
                                       pir[nivel][lminV1.i][lminV1.j].Y=cont++;
                                }else if (lmin==-1&&lminV1.i==-1){
                                        nodo.I=NodosV[nivel][i].I;
                                        nodo.Area=NodosV[nivel][i].Area;
                                        nodo.Clase=-1;
                                        nodo.padre=-1;
                                        NodosV[nivel+1].push_back(nodo);
                                        NodosV[nivel][i].padre=cont;
                                        cont++;
                                }

                        }


                }
        }
 }

 //A partir de aqui se esta teniendo en cuenta solo la parte alta de la jerarquía
 //donde solo hay nodos virtuales

 }else{

        CalcularAdyacenciaVirtual(pir,NodosV[nivel],NodosV[nivel-1],nivel-1,1,TamNivelesAlto[nivel],TamNivelesAncho[nivel]);


        for(i=0;i<NodosV[nivel].size();i++){
                if (NodosV[nivel][i].padre==-1){
                distV.clear();
                pos.clear();
                    for(j=0;j<NodosV[nivel][i].contactos.size();j++){
                        distV.push_back(DisColor(NodosV[nivel][i],NodosV[nivel][NodosV[nivel][i].contactos[j]]));
                        pos.push_back(NodosV[nivel][i].contactos[j]);

                    }
                        distmin=10000.0;
                        lmin=-1;
                        for(l=0;l<distV.size();l++){
                                if(distV[l]<distmin && NodosV[nivel][pos[l]].padre!=-1 &&
                                DisColor(NodosV[nivel][i],NodosV[nivel][pos[l]])<umbral){
                                     distmin=distV[l];
                                     lmin=pos[l];
                                }
                        }
                        if(lmin!=-1){
                                NodosV[nivel][i].padre=NodosV[nivel][lmin].padre;
                                nodo=CrearNodoVirtual(NodosV[nivel][i],NodosV[nivel+1][NodosV[nivel][i].padre]);
                                NodosV[nivel+1][NodosV[nivel][i].padre].Area= nodo.Area;
                                NodosV[nivel+1][NodosV[nivel][i].padre].I=nodo.I;
                        }
                        else{
                          distmin=10000.0;
                          lmin=-1;
                        for(l=0;l<distV.size();l++){
                                if(distV[l]<distmin && NodosV[nivel][pos[l]].padre==-1 &&
                                DisColor(NodosV[nivel][i],NodosV[nivel][pos[l]])<umbral){
                                     distmin=distV[l];
                                     lmin=pos[l];
                                }
                        }
                                if(lmin!=-1){
                                    nodo=CrearNodoVirtual(NodosV[nivel][i],
                                        NodosV[nivel][lmin]);
                                    NodosV[nivel+1].push_back(nodo);
                                    NodosV[nivel][i].padre=cont;
                                    NodosV[nivel][lmin].padre=cont;
                                    cont++;
                                }
                                else{
                                        nodo.I=NodosV[nivel][i].I;
                                        nodo.Area=NodosV[nivel][i].Area;
                                        nodo.Clase=-1;
                                        nodo.padre=-1;
                                        NodosV[nivel+1].push_back(nodo);
                                        NodosV[nivel][i].padre=cont;
                                        cont++;
                                }

                        }

                }
        }
  }
}

void BoundingBox(PIRGRIS ***pir,BOXGRIS *cajas, int clases, int alto, int ancho){

register int i,j,clase;
Padre padre;

//Inicialización de las cajas

for(i=0;i<clases;i++){
    cajas[i].imin=alto;
    cajas[i].jmin=ancho;
    cajas[i].imax=-1;
    cajas[i].jmax=-1;
    cajas[i].I=-1;
    cajas[i].area=0;
    cajas[i].clase=i;
    cajas[i].sumi=0;
    cajas[i].sumj=0;
}

//Calculo de las cajas

for(i=0;i<alto;i++){
  for(j=0;j<ancho;j++){
    clase=pir[0][i][j].Clase;
   if(clase!=-1){

     cajas[clase].sumi=cajas[clase].sumi+i;
     cajas[clase].sumj=cajas[clase].sumj+j;
     cajas[clase].area=cajas[clase].area+1;
     cajas[clase].I=pir[0][i][j].I;

     if(cajas[clase].imin>i)
         cajas[clase].imin=i;

     if(cajas[clase].jmin>j)
         cajas[clase].jmin=j;

     if(cajas[clase].imax<i)
         cajas[clase].imax=i;

     if(cajas[clase].jmax<j)
         cajas[clase].jmax=j;
   }
  }
}
for(i=0;i<clases;i++){
        if (cajas[i].area!=0) cajas[i].icent=cajas[i].sumi/cajas[i].area;
        if (cajas[i].area!=0) cajas[i].jcent=cajas[i].sumj/cajas[i].area;
}
}


void CalcContactos(int **contactos,PIRGRIS ***pir, int altoim, int anchoim){

register int i,j;

//Se recorre la base de la pirámide, y cada píxel pregunta a sus vecinos
//si tiene una clase diferente a la suya

for(i=0;i<altoim;i++){
   for(j=0;j<anchoim;j++){

      if((j-1!=-1)&& (pir[0][i][j].Clase!=pir[0][i][j-1].Clase)&&
        pir[0][i][j].Clase!=-1 &&pir[0][i][j-1].Clase!=-1){

         contactos[pir[0][i][j].Clase][pir[0][i][j-1].Clase]=1;
         contactos[pir[0][i][j-1].Clase][pir[0][i][j].Clase]=1;

      }

      if((j-1!=-1)&&(i-1!=-1)&&(pir[0][i][j].Clase!=pir[0][i-1][j-1].Clase)&&
        pir[0][i][j].Clase!=-1 &&pir[0][i-1][j-1].Clase!=-1){

         contactos[pir[0][i][j].Clase][pir[0][i-1][j-1].Clase]=1;
         contactos[pir[0][i-1][j-1].Clase][pir[0][i][j].Clase]=1;

      }

      if((i-1!=-1)&& (pir[0][i][j].Clase!=pir[0][i-1][j].Clase)&&
        pir[0][i][j].Clase!=-1 &&pir[0][i-1][j].Clase!=-1){

         contactos[pir[0][i][j].Clase][pir[0][i-1][j].Clase]=1;
         contactos[pir[0][i-1][j].Clase][pir[0][i][j].Clase]=1;

      }

      if((j+1!=anchoim)&&(i-1!=-1)&&(pir[0][i][j].Clase!=pir[0][i-1][j+1].Clase)&&
        pir[0][i][j].Clase!=-1 &&pir[0][i-1][j+1].Clase!=-1){

         contactos[pir[0][i][j].Clase][pir[0][i-1][j+1].Clase]=1;
         contactos[pir[0][i-1][j+1].Clase][pir[0][i][j].Clase]=1;

      }
   }
}

}


void Fusionar(FUS *fus,int i, int j, BOXGRIS *cajas){

//Para las clases de area <100 (se les asigna el color de la clase grande)

    fus[i].I=fus[j].I;

 //Actualiza el valor de color de las cajas de las clases que se fusionan

 cajas[i].I=fus[i].I;
 cajas[j].I=fus[i].I;
}



void BIP::FusionAreasPequenas(){

FUS *fusiones; //Array para almacenar las fusiones

BOXGRIS *cajas;

cajas=new BOXGRIS [ContClase];

register int i,j;
int cont1;

register int cont;

int **contactos;


double *colores;  //Array para almacenar la diferencia de color de una clase
                  // con las clases con las que tiene contacto

int colormin, posmin;

//Reserva de memoria para el array de colores

colores= new double[ContClase];

//Reserva de memoria para el array de fusiones

fusiones=new FUS [ContClase];

//Reserva de memoria para la matriz de contactos entre clases

contactos= new int* [ContClase];
for(i=0;i<ContClase;i++)
   contactos[i]= new int[ContClase];


for(i=0;i<ContClase;i++){
  for(j=0;j<ContClase;j++){
      contactos[i][j]=-1;
  }
}


BoundingBox(pir,cajas,ContClase,altoim,anchoim);

//Se inicializa el array de fusiones

for (i=0;i<ContClase;i++){
  fusiones[i].clase=-1;
  fusiones[i].I=cajas[i].I;
  fusiones[i].Area=cajas[i].area;
}

//Calculo de contactos entre las clases de la base de la piramide

CalcContactos(contactos,pir,altoim,anchoim);

//Miramos para cada clase con área <AREAMIN su relación con las demás

cont=0;

for (i=0;i<ContClase;i++){

 if(fusiones[i].Area<AREAMIN){

 //Inicialización del array de colores
   for(j=0;j<ContClase;j++){

       colores[j]=10000.0;

   }

   for(j=0;j<ContClase; j++){

    //Si dos clases están en contacto calculamos su diferencia de color

     if(i!=j && contactos[i][j]==1 && fusiones[j].Area>=AREAMIN){

        colores[j]=DisColor(fusiones[i],fusiones[j]);
     }
    }

    colormin=10000.0;
    posmin=-1;

    //Buscamos la clase con menor diferencia de color
    for(j=0;j<ContClase;j++){

       if(colores[j]<colormin){

         colormin=colores[j];
         posmin=j;

       }
    }

    if(colormin<10000.0){
      //Si la primera no está fusionada con nadie

       //if(fusiones[i].clase==-1){

      //Si la segunda tampoco está fusionada con nadie se fusionan las dos
      //asignandoles un nuevo valor de clase (el mismo a las dos) en el array
      //de fusiones

      if(fusiones[posmin].clase==-1){
         fusiones[i].clase=cont;
         fusiones[i].Area=AREAMIN;
         fusiones[posmin].clase=cont;
         Fusionar(fusiones,i,posmin,cajas);
         cont++;

      //Si la segunda si está fusionada, se le da a la primera el valor de clase
      // de la segunda y también su color

      }else{
          fusiones[i].clase=fusiones[posmin].clase;
          fusiones[i].Area=AREAMIN;
          fusiones[i].I=fusiones[posmin].I;

      }//if else
     //if
 }//if color
 }//if area<AREAMIN
} //for

// A las clases sin fusionar se les da un nuevo identificador de clase y
// se mantiene su color

for (i=0;i<ContClase;i++){
   if(fusiones[i].clase==-1){
      fusiones[i].clase=cont;
      fusiones[i].I=cajas[i].I;
      cont++;
   }
}


//Se actualizan los valores de clase y el color en la base de la piramide

for(i=0;i<altoim;i++){
  for(j=0;j<anchoim;j++){
    if(pir[0][i][j].Clase!=-1) {
       cont1=pir[0][i][j].Clase;
       pir[0][i][j].Clase=fusiones[cont1].clase;
       pir[0][i][j].I=fusiones[cont1].I;
   }
}
}

ContClase=cont;

}



void BIP::Segmentar(IMAGENGRIS im, double umbral){


register int i,j,k;
int regular;

for(k=1;k<niveles;k++)
        for(i=0;i<TamNivelesAlto[k];i++)
                for(j=0;j<TamNivelesAncho[k];j++){
                        pir[k][i][j].X=-1;
                        pir[k][i][j].Y=-1;
                        pir[k][i][j].Homog=-1;
                        pir[k][i][j].Area=0;
                        pir[k][i][j].Clase=-1;
                }



for(i=0;i<altoim;i++)
        for(j=0;j<anchoim;j++){
	  if (im.pixel[i][j]!=0){
	    pir[0][i][j].I=im.pixel[i][j];
	    pir[0][i][j].X=-1;
	    pir[0][i][j].Y=-1;
	    pir[0][i][j].Homog=1;
	    pir[0][i][j].Area=1;
	    pir[0][i][j].Clase=-1;
	  }else{
	    pir[0][i][j].Homog=0;
	  }

        }


 for(i=0;i<nivelesI;i++)
   NodosV[i].clear();

 padres.clear();

for(i=0;i<niveles-1;i++) {
        regular=GenerarParteRegular(i);
        GenerarParteIrregular(i,umbral);
        if (NodosV[i].size()==NodosV[i+1].size() && regular==-1){
         numNiveles=i+1;
         break;
         }
}


for(i=niveles-1; i<nivelesI-1&&NodosV[i].size()!=NodosV[i-1].size(); i++){

       GenerarParteIrregular(i,umbral);
        numNiveles=i+1;
} 

Clasificar();
LLevarColoresArriba();

int areas=0;

for(i=0;i<ContClase;i++)
        areas=areas+padres[i].Area;
FusionAreasPequenas();

//Esto en realidad sirve para la extraccion de caracteristicas
cajas=new BOXGRIS[ContClase];
BoundingBox(pir,cajas,ContClase, altoim, anchoim);

}

void BIP::Reiniciar()
{
register int i,j,k;
int lado; 

//for(i=0;i<niveles;i++){
//  NodosV[i].reserve(TamNivelesAlto[i]*TamNivelesAncho[i]/2);
//}

for(i=0;i<altoim;i++){
        for(j=0;j<anchoim;j++){
                pir[0][i][j].X=-1;
                pir[0][i][j].Y=-1;
                pir[0][i][j].Homog=1;
                pir[0][i][j].Area=1;
                pir[0][i][j].Clase=-1;

        }
}
for(k=1;k<niveles;k++)
        for(i=0;i<TamNivelesAlto[k];i++)
                for(j=0;j<TamNivelesAncho[k];j++){
                        pir[k][i][j].X=-1;
                        pir[k][i][j].Y=-1;
                        pir[k][i][j].Homog=-1;
                        pir[k][i][j].Area=0;
                        pir[k][i][j].Clase=-1;
                }

}

