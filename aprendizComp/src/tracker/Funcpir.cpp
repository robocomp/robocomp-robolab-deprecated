/******************************************************************

FUNCIONES PARA REALIZAR LA SEGMENTACIÓN DE UNA IMAGEN UTILIZANDO LA BIP BINARIA

******************************************************************/

#include <tracker/Funcpir.h>

void InicPiramide (PIR ***pir, int NumNiveles, int *tam_i, int *tam_j, unsigned char **im )
{

  register int ii,i,j;
  int X,Y;
  int cont;
  int i1,i2,j1,j2;


  for (i=0;i<tam_i[0];i++){
     for(j=0;j<tam_j[0];j++){
       if (im[i][j]==255)
	pir[0][i][j].Homog=1;
       else
        pir[0][i][j].Homog=0;

        pir[0][i][j].X=-1;
        pir[0][i][j].Y=-1;
        pir[0][i][j].Clase=-1;
     }
  }


  /* Inicializacion de niveles */

  for (ii=1;ii<NumNiveles;ii++)
    {
    for (i=0;i<tam_i[ii];i++)
    {

      X=i;
      i1=X*2;
      i2=X*2+1;

      for(j=0;j<tam_j[ii];j++){

	  Y=j;
          j1=Y*2;
          j2=Y*2+1;

          pir[ii][i][j].X=-1;
          pir[ii][i][j].Y=-1;
          pir[ii][i][j].Clase=-1;

     if (pir[ii-1][i1][j1].Homog==1){

      if ((pir[ii-1][i1][j2].Homog==1&&
              pir[ii-1][i2][j1].Homog==1&&
              pir[ii-1][i2][j2].Homog==1))
	    {

             pir[ii][i][j].Homog=1;

             pir[ii-1][i1][j1].Y=Y;
             pir[ii-1][i1][j2].Y=Y;
             pir[ii-1][i2][j1].Y=Y;
             pir[ii-1][i2][j2].Y=Y;
             pir[ii-1][i1][j1].X=X;
             pir[ii-1][i1][j2].X=X;
             pir[ii-1][i2][j1].X=X;
             pir[ii-1][i2][j2].X=X;


      } else //EL NODO NO ES HOMOGENEO
	    {
         pir[ii][i][j].Homog=0;
      }


    }else
        pir[ii][i][j].Homog=0;
   }
  }
 }

}

int ContClaseI;
int ContClaseD;


void Clasificar(PIR *** pir,int num, int *tam_i, int *tam_j, int *NodosVirtuales, int ContNV, int reclasificar)
{

  register int k,i,j;
  register int ii;
  int homog,x,y;

  //Inicializo el valor de clase de la estructura piramidal

  for (i=0;i<=ContNV;i++){
      NodosVirtuales[i]=-1;
  }

  for(k=0;k<num;k++){
    for(i=0;i<tam_i[k];i++){
      for(j=0;j<tam_j[k];j++){
        pir[k][i][j].Clase=-1;
      }
    }
  }

if (reclasificar==1){//Primera clasificación después del hermanamiento
  ContClaseI=0;

  //Todos los nodos virtuales 1 son clase nueva
  for (i=0;i<=ContNV;i++)
    {
	NodosVirtuales[i]=ContClaseI;
	ContClaseI++;
    }

  for (ii=num-1;ii>=0;ii--)
    {
    for (i=0;i<tam_i[ii];i++){
       for(j=0;j<tam_j[ii];j++)
	  {
          homog=pir[ii][i][j].Homog;
          x=pir[ii][i][j].X;
          y=pir[ii][i][j].Y;
	  // CASO 1: Clase nueva
      if ((homog==1 && x==-1 && y==-1))
		{
                pir[ii][i][j].Clase=ContClaseI;
		ContClaseI++;
	    }
	  else
	    {
	  // CASO 2: Tiene padre, luego hereda Clase
        if ((homog==1) && y!=-1)
		  {
		  // 2a. Padre virtual o tutor
          if (x==-1)
		    {
            pir[ii][i][j].Clase=NodosVirtuales[y];
		    }
		  // 2b. Padre natural
          else
		    {
            pir[ii][i][j].Clase=pir[ii+1][x][y].Clase;
		    }
		  }
	  // CASO 3: Ni tiene padre ni es hijo de nadie: Clase igual a 100000
	else
		  {
		  pir[ii][i][j].Clase=-1;
		  }
	    }
	  }
    }
    }
  }else if (reclasificar==0){// clasificación antes del hermanamiento
  ContClaseI=0;

  /* Despues se estudian los niveles de la piramide:
		- Los homogeneos sin padre se marcan como clases nuevas
		- Los homogeneos con padre se marcan con el valor del padre
  */
  for (ii=num-1;ii>=0;ii--)
    {
    for (i=0;i<tam_i[ii];i++){
       for(j=0;j<tam_j[ii];j++)
	  {
           homog=pir[ii][i][j].Homog;
           x=pir[ii][i][j].X;
           y=pir[ii][i][j].Y;

	  /* CASO 1: Clase nueva */
      if ((homog==1 && x==-1 && y==-1))
		{
                pir[ii][i][j].Clase=ContClaseI;
		ContClaseI++;
	    }
	  else
	    {
	  /* CASO 2: Tiene padre, luego hereda Clase */
        if ((homog==1) && y!=-1)
		  {
		  /* 2b. Padre natural */

            pir[ii][i][j].Clase=pir[ii+1][x][y].Clase;

		  }
	  /* CASO 3: Ni tiene padre ni es hijo de nadie: Clase igual a 100000 */
		else
		  {
		  pir[ii][i][j].Clase=-1;
		  }
	    }
	  }
    }
    }
  }
//ShowMessage(IntToStr(ContClaseI));
}


int Hermanamiento(PIR ***pir, int num, int *tam_i, int *tam_j){

register int i,j,k,l;
//int tamano;
int cont;

//tamano=2;

cont=-1;

//Se recorre la estructura  desde el penúltimo nivel hasta la base

for(k=num-2;k>=0;k--){
for (i=0;i<tam_i[k];i++){
  for(j=0;j<tam_j[k];j++){

       /*Si el nodo es homogéneo y no tiene padre calculamos y almacenamos
         la distancia en color con sus vecinos homogéneos,también
         guardamos las coordenadas del vecino*/

       if ((pir[k][i][j].Homog==1)
           &&(pir[k][i][j].Y==-1)){

          l=0;

          if(((j-1)!=-1)&&
              (pir[k][i][j-1].Homog==1)&&(pir[k][i][j-1].Y!=-1)){

                pir[k][i][j].X=pir[k][i][j-1].X;
                pir[k][i][j].Y=pir[k][i][j-1].Y;
                l=1;
          }


          else if(((j-1)!=-1)&&((i-1)!=-1)
              &&(pir[k][i-1][j-1].Homog==1)&&(pir[k][i-1][j-1].Y!=-1)){

                pir[k][i][j].X=pir[k][i-1][j-1].X;
                pir[k][i][j].Y=pir[k][i-1][j-1].Y;
                l=1;
          }

          else if(((i-1)!=-1)
             &&(pir[k][i-1][j].Homog==1)&&(pir[k][i-1][j].Y!=-1)){

                pir[k][i][j].X=pir[k][i-1][j].X;
                pir[k][i][j].Y=pir[k][i-1][j].Y;
                l=1;
          }
          else if(((i-1)!=-1)&&((j+1)!=tam_j[k])
             &&(pir[k][i-1][j+1].Homog==1)&& (pir[k][i-1][j+1].Y!=-1)){

                pir[k][i][j].X=pir[k][i-1][j+1].X;
                pir[k][i][j].Y=pir[k][i-1][j+1].Y;
                l=1;
          }
          else if(((j+1)!=tam_j[k])
             &&(pir[k][i][j+1].Homog==1)&&(pir[k][i][j+1].Y!=-1)){

                pir[k][i][j].X=pir[k][i][j+1].X;
                pir[k][i][j].Y=pir[k][i][j+1].Y;
                l=1;
          }
          else if(((j+1)!=tam_j[k]) &&((i+1)!=tam_i[k])
             &&(pir[k][i+1][j+1].Homog==1)&&(pir[k][i+1][j+1].Y!=-1)){

                pir[k][i][j].X=pir[k][i+1][j+1].X;
                pir[k][i][j].Y=pir[k][i+1][j+1].Y;
                l=1;
          }
          else if(((i+1)!=tam_i[k])
             &&(pir[k][i+1][j].Homog==1)&&(pir[k][i+1][j].Y!=-1)){

                pir[k][i][j].X=pir[k][i+1][j].X;
                pir[k][i][j].Y=pir[k][i+1][j].Y;
                l=1;
          }
          else if(((j-1)!=-1)&&((i+1)!=tam_i[k])
             &&(pir[k][i+1][j-1].Homog==1)&&(pir[k][i+1][j-1].Y!=-1)){

                pir[k][i][j].X=pir[k][i+1][j-1].X;
                pir[k][i][j].Y=pir[k][i+1][j-1].Y;
                l=1;
          }

          if(l==0){

            if(((j-1)!=-1)&&
              (pir[k][i][j-1].Homog==1)&&(pir[k][i][j-1].Y==-1)){

                cont++;
                pir[k][i][j-1].X=-1;
                pir[k][i][j].X=-1;
                pir[k][i][j-1].Y=cont;
                pir[k][i][j].Y=cont;
          }


          else if(((j-1)!=-1)&&((i-1)!=-1)
              &&(pir[k][i-1][j-1].Homog==1)&&(pir[k][i-1][j-1].Y==-1)){

                cont++;
                pir[k][i-1][j-1].X=-1;
                pir[k][i][j].X=-1;
                pir[k][i-1][j-1].Y=cont;
                pir[k][i][j].Y=cont;
          }

          else if(((i-1)!=-1)
             &&(pir[k][i-1][j].Homog==1)&&(pir[k][i-1][j].Y==-1)){

                cont++;
                pir[k][i-1][j].X=-1;
                pir[k][i][j].X=-1;
                pir[k][i-1][j].Y=cont;
                pir[k][i][j].Y=cont;
          }
          else if(((i-1)!=-1)&&((j+1)!=tam_j[k])
             &&(pir[k][i-1][j+1].Homog==1)&& (pir[k][i-1][j+1].Y==-1)){

                cont++;
                pir[k][i-1][j+1].X=-1;
                pir[k][i][j].X=-1;
                pir[k][i-1][j+1].Y=cont;
                pir[k][i][j].Y=cont;
          }
          else if(((j+1)!=tam_j[k])
             &&(pir[k][i][j+1].Homog==1)&&(pir[k][i][j+1].Y==-1)){

                cont++;
                pir[k][i][j+1].X=-1;
                pir[k][i][j].X=-1;
                pir[k][i][j+1].Y=cont;
                pir[k][i][j].Y=cont;
          }
          else if(((j+1)!=tam_j[k]) &&((i+1)!=tam_i[k])
             &&(pir[k][i+1][j+1].Homog==1)&&(pir[k][i+1][j+1].Y==-1)){

                cont++;
                pir[k][i+1][j+1].X=-1;
                pir[k][i][j].X=-1;
                pir[k][i+1][j+1].Y=cont;
                pir[k][i][j].Y=cont;
          }
          else if(((i+1)!=tam_i[k])
             &&(pir[k][i+1][j].Homog==1)&&(pir[k][i+1][j].Y==-1)){

                cont++;
                pir[k][i+1][j].X=-1;
                pir[k][i][j].X=-1;
                pir[k][i+1][j].Y=cont;
                pir[k][i][j].Y=cont;
          }
          else if(((j-1)!=-1)&&((i+1)!=tam_i[k])
             &&(pir[k][i+1][j-1].Homog==1)&&(pir[k][i+1][j-1].Y==-1)){

                cont++;
                pir[k][i+1][j-1].X=-1;
                pir[k][i][j].X=-1;
                pir[k][i+1][j-1].Y=cont;
                pir[k][i][j].Y=cont;
          }


          }
                          
       }
  }
}
  //tamano=tamano*2;
}
return cont;
}

void CalcContactos(PIR ***pir, int tam_i, int tam_j,int **contactos){

register int i,j,k,l,clase;

//Inicializo a -1 todas las posiciones de la matriz
for(i=0;i<ContClaseI;i++){
  for(j=0;j<ContClaseI;j++){
      contactos[i][j]=-1;
  }
}

for(i=0;i<tam_i;i++){
   for(j=0;j<tam_j;j++){
    clase=pir[0][i][j].Clase;
    for(k=1;k<4;k++)
      for(l=1;l<4;l++){
      if((j-l>-1)&& (clase!=pir[0][i][j-l].Clase)&&
        (clase!=-1) && (pir[0][i][j-l].Clase!=-1)){

         contactos[clase][pir[0][i][j-l].Clase]=0;
         contactos[pir[0][i][j-l].Clase][clase]=1;

      }

      if((j-l>-1)&&(i-k>-1)&&(clase!=pir[0][i-k][j-l].Clase)&&
        (clase!=-1) &&(pir[0][i-k][j-l].Clase!=-1)){

         contactos[clase][pir[0][i-k][j-l].Clase]=0;
         contactos[pir[0][i-k][j-l].Clase][clase]=1;

      }

      if((i-k>-1)&& (clase!=pir[0][i-k][j].Clase)&&
        (clase!=-1) && (pir[0][i-k][j].Clase!=-1)){

         contactos[clase][pir[0][i-k][j].Clase]=1;
         contactos[pir[0][i-k][j].Clase][clase]=1;

      }

      if((j+l<tam_j)&&(i-k>-1)&&(clase!=pir[0][i-k][j+l].Clase)&&
        (clase!=-1) && (pir[0][i-k][j+l].Clase!=-1)){

         contactos[clase][pir[0][i-k][j+l].Clase]=1;
         contactos[pir[0][i-k][j+l].Clase][clase]=0;

      }
    }
   }
}

}

/*BoundingBoxIzquierda: Calcula las cajas correspondientes a las clases
en la base de la pirámide*/

void BoundingBox(PIR ***pir, int tam_i, int tam_j, BOX *box){

register int i,j,k,clase;

//Calculo de las cajas

for(i=0;i<tam_i;i++){
  for(j=0;j<tam_j;j++){

   clase=pir[0][i][j].Clase;

   if(clase!=-1){

     box[clase].area=box[clase].area+1;

     if(box[clase].imin>i)
         box[clase].imin=i;

     if(box[clase].jmin>j)
         box[clase].jmin=j;

     if(box[clase].imax<i)
         box[clase].imax=i;

     if(box[clase].jmax<j)
         box[clase].jmax=j;


   }
  }
}
}


int Fusion(PIR ***pir, int niveles,int *tam_i, int *tam_j){

register int i,j,k,ContClase;
int clase,clase1;
int cont;
int **contactos;
int *fusiones;

fusiones=new int [ContClaseI];

contactos=new int *[ContClaseI];
for(i=0;i<ContClaseI;i++){
   contactos[i]=new int [ContClaseI];
}

ContClase=0;

//Se inicializa el array de fusiones
for (i=0;i<ContClaseI;i++){
  fusiones[i]=-1;
}

//Calculo de contactos entre las clases de la base de la piramide
CalcContactos(pir,tam_i[0],tam_j[0],contactos);

//Miramos para cada clase su relación con las demás
for (i=0;i<ContClaseI;i++){
 for(j=0;j<ContClaseI; j++){

    //Si dos clases están en contacto y su color (que es el color almacenado
    // en la bounding box correspondiente) es similar
     if(i!=j && contactos[i][j]!=-1){

      //Si la primera no está fusionada con nadie

       if(fusiones[i]==-1){

      //Si la segunda tampoco está fusionada con nadie se fusionan las dos
      //asignandoles un nuevo valor de clase (el mismo a las dos) en el array
      //de fusiones

      if(fusiones[j]==-1){
         fusiones[i]=ContClase;
         fusiones[j]=ContClase;
         ContClase++;

      //Si la segunda si está fusionada, se le da a la primera el valor de clase
      // de la segunda

      }else{
          fusiones[i]=fusiones[j];

      }

   //Si la primera ya está fusionada

  }else if(fusiones[i]!=-1){

   //Si la segunda no está fusionada se le asigna a la segunda el valor de clase
   //de la primera

      if(fusiones[j]==-1){

         fusiones[j]=fusiones[i];


      //Si ambas estan fusionadas, a la que tenga clase mayor se le asigna el
      //valor de clase de la otra, y se reasignan todos los identificadores
      // de clase

      }else if(fusiones[i]!=fusiones[j]){

          if(fusiones[i]<fusiones[j]){
            clase=fusiones[j];
            clase1=fusiones[i];
            fusiones[j]=fusiones[i];
          } else{
           clase=fusiones[i];
           clase1=fusiones[j];
           fusiones[i]=fusiones[j];
          }
          ContClase--;

          for(k=0;k<ContClaseI;k++){

           if(fusiones[k]==clase){
              fusiones[k]=clase1;
           }
           else if(fusiones[k]>clase) fusiones[k]=fusiones[k]-1;

         }
      }

     }
  }

}
}

// A las clases sin fusionar se les da un nuevo identificador de clase

for (i=0;i<ContClaseI;i++){
   if(fusiones[i]==-1){
      fusiones[i]=ContClase;
      ContClase++;
   }
}


//Actualizo el valos de clase en toda la estructura

for(k=0;k<niveles;k++){
 for(i=0;i<tam_i[k];i++){
  for(j=0;j<tam_j[k];j++){
    if(pir[k][i][j].Clase!=-1) {
      cont=pir[k][i][j].Clase;
      pir[k][i][j].Clase=fusiones[cont];
    }else {
     pir[k][i][j].Clase=-1;
    }
 }
}
}

ContClaseI=ContClase;
//ShowMessage(IntToStr(ContClaseI));
return ContClase;
}

void ExtraerClases(PIR ***pir, int tam_i,int tam_j,int numClases, BOX *boxes, int *clases){

register int i;
int max=0;
int aux1, aux2,aux;

clases[2]=-1;
clases[1]=-1;
clases[0]=-1;
aux1=-1;
aux2=-1;
aux=-1;

for(i=0;i<numClases;i++){
        boxes[i].imin=tam_i;
        boxes[i].imax=0;
        boxes[i].jmin=tam_j;
        boxes[i].jmax=0;
        boxes[i].area=0;
}

BoundingBox(pir,tam_i,tam_j,boxes);

for(i=0;i<numClases;i++){

      if(boxes[i].area>max){
                max=boxes[i].area;
 //               clases[0]=i;
                aux=i;
        }
 }
max=0;
for(i=0;i<numClases; i++){

        if(boxes[i].area>max && i!=aux/*clases[0]*/){
            max=boxes[i].area;
            aux1=i;
        }
}
max=0;
for(i=0;i<numClases; i++){

        if(boxes[i].area>max && i!=aux/*clases[0]*/ && i!=aux1){
            max=boxes[i].area;
            aux2=i;
        }
}

if(boxes[aux1].jmin<boxes[aux2].jmin &&boxes[aux1].jmin<boxes[aux].jmin){
     clases[1]=aux1;
     if(boxes[aux].jmin<boxes[aux2].jmin){
        clases[0]=aux;
        clases[2]=aux2;
     }else{
        clases[2]=aux;
        clases[0]=aux2;
     }

     //clases[0/*2*/]=aux2;
}

if(boxes[aux2].jmin<boxes[aux1].jmin &&boxes[aux2].jmin<boxes[aux].jmin){
     clases[1]=aux2;
     if(boxes[aux].jmin<boxes[aux1].jmin){
        clases[0]=aux;
        clases[2]=aux1;
     }else{
        clases[2]=aux;
        clases[0]=aux1;
     }

     //clases[0/*2*/]=aux2;
}
if(boxes[aux].jmin<boxes[aux2].jmin &&boxes[aux].jmin<boxes[aux1].jmin){
     clases[1]=aux;
     if(boxes[aux1].jmin<boxes[aux2].jmin){
        clases[0]=aux1;
        clases[2]=aux2;
     }else{
        clases[2]=aux1;
        clases[0]=aux2;
     }

     //clases[0/*2*/]=aux2;
}

}



