
#include <stdio.h>
#include <tracker/IO.h>
#include <tracker/FuncTracking.h>
extern int **rango1;

void LeerImagenFicheroI(TImage *Image, int cont,IMAGEN *im){

char *nomfich;
int i,j;

nomfich=new char [50];
sprintf(nomfich,"im%03d-C.bmp",cont);

Image->Picture->LoadFromFile(nomfich);
im->alto=Image->Picture->Bitmap->Height;
im->ancho=Image->Picture->Bitmap->Width;

for(i=0;i<im->alto;i++){
        for(j=0;j<im->ancho;j++){
                im->pixel[i][j*3]=(int)(Image->Picture->Bitmap->Canvas->Pixels[j][i] & 0x0000FF);
                im->pixel[i][j*3+1]=(int)((Image->Picture->Bitmap->Canvas->Pixels[j][i] & 0x00FF00)>>8);
                im->pixel[i][j*3+2]=(int)((Image->Picture->Bitmap->Canvas->Pixels[j][i] & 0xFF0000)>>16);
        }
}

}

void LeerImagenFicheroD(TImage *Image, int cont,IMAGEN *im){

char *nomfich;
int i,j;

nomfich=new char [50];
sprintf(nomfich,"ImagenD_00%d.bmp",cont);

Image->Picture->LoadFromFile(nomfich);
im->alto=Image->Picture->Bitmap->Height;
im->ancho=Image->Picture->Bitmap->Width;

for(i=0;i<im->alto;i++){
        for(j=0;j<im->ancho;j++){
                im->pixel[i][j*3+2]=(int)(Image->Picture->Bitmap->Canvas->Pixels[j][i] & 0x0000FF);
                im->pixel[i][j*3+1]=(int)((Image->Picture->Bitmap->Canvas->Pixels[j][i] & 0x00FF00)>>8);
                im->pixel[i][j*3]=(int)((Image->Picture->Bitmap->Canvas->Pixels[j][i] & 0xFF0000)>>16);
        }
}

}
void PintarImagen(PIR ***pir, TImage *Image){
register int i,j;
int rojo,verde, azul;

for(i=0;i<ALTO;i++)
        for(j=0;j<ANCHO;j++){
           if(pir[0][i][j].Homog==1&&pir[0][i][j].Clase!=-1){
            rojo=pir[0][i][j].Clase*20+128;
            verde=0<<8;
            azul=0<<16;
           }else if (pir[0][i][j].Homog==1&&pir[0][i][j].Clase==-1){
            rojo=0;
            verde=0<<8;
            azul=255<<16;
           }else{
             rojo=0;
            verde=0<<8;
            azul=0<<16;
            }
            Image->Canvas->Pixels[j][i]=(TColor)(azul|verde|rojo);
}
}

void DibujarCajas(BOX *boxes, int *clases, TImage *Image, PIR ***pir){

register int i,j,k;
int rojo,verde,azul;

for(j=0; j<NUMMARCS; j++){

for(i=boxes[j].jmin;i<=boxes[j].jmax;i++){
            rojo=255;
            verde=0;
            azul=0;
            Image->Canvas->Pixels[i][boxes[j].imin]=(TColor)(azul|verde|rojo);
}
for(i=boxes[j].jmin;i<=boxes[j].jmax;i++){
            rojo=255;
            verde=0;
            azul=0;
            Image->Canvas->Pixels[i][boxes[j].imax]=(TColor)(azul|verde|rojo);
}
for(i=boxes[j].imin;i<=boxes[j].imax;i++){
            rojo=255;
            verde=0;
            azul=0;
            Image->Canvas->Pixels[boxes[j].jmin][i]=(TColor)(azul|verde|rojo);
}
for(i=boxes[j].imin;i<=boxes[j].imax;i++){
            rojo=255;
            verde=0;
            azul=0;
            Image->Canvas->Pixels[boxes[j].jmax][i]=(TColor)(azul|verde|rojo);
}

if(boxes[j].jcent!=-1&&boxes[j].icent!=-1)
    Image->Canvas->Pixels[boxes[j].jcent][boxes[j].icent]=(TColor)(255<<16|0|0);
}

/*for(k=0;k<NUMMARCS;k++){
        for(i=boxes[k].imin;i<=boxes[k].imax;i++){
                for(j=boxes[k].jmin;j<=boxes[k].jmax;j++){
                        if(pir[0][i][j].Clase==clases[k])
                             Image->Canvas->Pixels[j][i]=(TColor)(0|0|255);
                }
        }
} */
}

void PintarZonaBusqueda(BOX **boxnivel, int **rango, TImage *Image){

register int i,j;
int rojo,verde,azul;

for(j=0;j<NUMMARCS;j++){
for(i=boxnivel[0][j].jmin-rango[0][j];i<=boxnivel[0][j].jmax+rango[0][j];i++){
            rojo=0;
            verde=0;
            azul=255<<16;
            Image->Canvas->Pixels[i][boxnivel[0][j].imin-rango[0][j]]=(TColor)(azul|verde|rojo);
}
for(i=boxnivel[0][j].jmin-rango[0][j];i<=boxnivel[0][j].jmax+rango[0][j];i++){
            rojo=0;
            verde=0;
            azul=255<<16;
            Image->Canvas->Pixels[i][boxnivel[0][j].imax+rango[0][j]]=(TColor)(azul|verde|rojo);
}
for(i=boxnivel[0][j].imin-rango[0][j];i<=boxnivel[0][j].imax+rango[0][j];i++){
            rojo=0;
            verde=0;
            azul=255<<16;
            Image->Canvas->Pixels[boxnivel[0][j].jmin-rango[0][j]][i]=(TColor)(azul|verde|rojo);
}
for(i=boxnivel[0][j].imin-rango[0][j];i<=boxnivel[0][j].imax+rango[0][j];i++){
            rojo=0;
            verde=0;
            azul=255<<16;
            Image->Canvas->Pixels[boxnivel[0][j].jmax+rango[0][j]][i]=(TColor)(azul|verde|rojo);
}

}
}

void DibujarClasesEstereo(TRACKER *track,TImage *Image1,int sec, TImage *Image){
register int i,j,k;
int rojo,verde,azul;

for(i=0;i<ALTO; i++)
        for(j=0;j<ANCHO;j++){
            for(k=0;k<NUMMARCS;k++){
                if(track->pirder[0][i][j].Clase==track->clases[k]){
                   Image1->Canvas->Pixels[j][i]=(TColor)(0|0|255);
                }
            }
        }

/*for(i=0;i<ALTO; i++)
        for(j=0;j<ANCHO;j++){
            for(k=0;k<NUMMARCS;k++){
                if(track->pir[sec][0][i][j].Clase==track->clases[k]){
                   Image1->Canvas->Pixels[j][i]=(TColor)(0|0|255);
                }
            }
        }
*/
}

void PintarResultadoTracking(TRACKER *track, TImage *Image, int sec){
register int i,j,k;
int azul,verde,rojo;

for(i=0;i<ALTO; i++)
        for(j=0;j<ANCHO;j++){
            for(k=0;k<NUMMARCS;k++){
                if(track->pir[sec][0][i][j].Clase==track->clases[k]){
                   Image->Canvas->Pixels[j][i]=(TColor)(0|0|track->pir[sec][0][i][j].Clase);
                }
            }
        }


}

void GuardarNivelDerechoFichero(TRACKER *track,int nivel){

register int i,j,k;
FILE *f;
char *nomfich;

nomfich=new char [50];
sprintf(nomfich,"Nivel%d.raw",nivel);
f=fopen(nomfich,"wb");
for(i=0;i<track->TAMNiveles_i[nivel];i++)
        for(j=0;j<track->TAMNiveles_j[nivel];j++)
                for(k=0;k<NUMMARCS;k++){
                      if(track->pirder[nivel][i][j].Clase==track->clases[k])
                         fputc(track->pirder[nivel][i][j].Clase,f);
                      else fputc(255,f);
                }
fclose(f);
}

void GuardarResultadoTracking(TRACKER *track){

register int i,j,k;
FILE *f;

f=fopen("Resultado.raw","wb");
for(i=0;i<track->TAMNiveles_i[0];i++)
        for(j=0;j<track->TAMNiveles_j[0];j++)
                for(k=0;k<NUMMARCS;k++){
                      if(track->pir[track->a][0][i][j].Clase==track->clases[k])
                         fputc(track->pir[track->a][0][i][j].Clase,f);
                      else fputc(255,f);
                }
fclose(f);

}

