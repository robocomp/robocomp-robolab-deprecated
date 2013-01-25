#include <tracker/IORaw.h>

#include <stdio.h>
#include <jpeglib.h> habilitar en linux

#define FIXEDQUALITY	75 	// that should be enough to most uses...

void LeerImagenFichero(IMAGEN *im, int cont){
register int i,j;
FILE *f;
char nomfich[50];

//nomfich=new char [50];
sprintf(nomfich,"vg000%d.raw",cont);
f=fopen(nomfich,"rb");
for(i=0;i<im->alto;i++)
        for(j=0;j<im->ancho*3; j++){

             im->pixel[i][j]=fgetc(f);
        }

fclose(f);
}


void GuardarResultadoTracking(TRACKER *track, int cont){

register int i,j,m;
FILE *f;
char nomfich[50];

//nomfich=new char [50];
sprintf(nomfich,"Tracking%d.raw",cont);
f=fopen(nomfich, "wb");

for(i=0;i<track->im.alto;i++)
        for(j=0;j<track->im.ancho;j++){
           for(m=0;m<NUMMARCS;m++){	     
                if(track->pir[track->a][0][i][j].Clase==track->clases[m]){
                        fputc(255,f);
                }else   fputc(0,f);
	   }
        }

fclose(f);
}

void GuardarImagenEntrada(TRACKER *track, int cont){

register int i,j,m;
FILE *f;
char nomfich[50];

//nomfich=new char [50];
sprintf(nomfich,"Imagen%d.raw",cont);
f=fopen(nomfich, "wb");

for(i=0;i<track->im.alto;i++)
        for(j=0;j<track->im.ancho*3;j++){
	  fputc(track->im.pixel[i][j],f);
	}
fclose(f);
}

static void PrepararJpeg(FILE* fp,
			 jpeg_compress_struct& cinfo, 
			 jpeg_error_mgr& jerr,
			 int ancho,
			 int alto,
			 int ncanales)
{
  
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  jpeg_stdio_dest(&cinfo, fp);
  cinfo.image_width = ancho;
  cinfo.image_height = alto;
  cinfo.input_components = ncanales;
  if (ncanales == 1)
    cinfo.in_color_space = JCS_GRAYSCALE;
  else if (ncanales == 3)
    cinfo.in_color_space = JCS_RGB;
  else
    {
      printf("No se guardar en jpeg imagenes de %d canales",ncanales);
      return;
    }
  
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, FIXEDQUALITY, TRUE);
  jpeg_start_compress(&cinfo, TRUE);

}

static void RecogerJpeg( jpeg_compress_struct& cinfo)
{
  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);
}



void GuardarImagenColorPiel(TRACKER *track, int cont, formato fmt){

  register int i,j,m;
  FILE *fp;
  char nomfich[50];

  if (fmt == jpeg) 
    {
      sprintf(nomfich,"Piel%04d.jpg",cont);
      if ( (fp=fopen(nomfich, "w")) == NULL) {
	printf("Error al intentar abrir: %s\n", nomfich);
	return;
      }
      jpeg_compress_struct cinfo;
      jpeg_error_mgr jerr;
      PrepararJpeg(fp, cinfo, jerr, track->im.ancho, track->im.alto, 1);
      for (int i = 0; i < track->im.alto; i++) {
	jpeg_write_scanlines(&cinfo, &(track->seg.pixel[i]), 1);
      }
      RecogerJpeg(cinfo);
		
      fclose(fp);
    }
  else if (fmt == raw) {
    sprintf(nomfich,"Piel%04d.raw",cont);
    fp=fopen(nomfich, "wb");
    for(i=0;i<track->im.alto;i++)
      for(j=0;j<track->im.ancho;j++){
	fputc(track->seg.pixel[i][j],fp);
      }
    fclose(fp);
  }
}

void GuardarImagenDisparidad(TRACKER *track, int cont, formato fmt)
{
  register int i,j,m;
  FILE *fp;
  char nomfich[50];

  if (fmt == jpeg) 
    {
      sprintf(nomfich,"Disparidad%04d.jpg",cont);
      if ( (fp=fopen(nomfich, "w")) == NULL) {
	printf("Error al intentar abrir: %s\n", nomfich);
	return;
      }
      jpeg_compress_struct cinfo;
      jpeg_error_mgr jerr;
      PrepararJpeg(fp, cinfo, jerr, track->imdispar.ancho, track->imdispar.alto, 1);
      for (int i = 0; i < track->imdispar.alto; i++) {
	jpeg_write_scanlines(&cinfo, &(track->imdispar.pixel[i]), 1);
      }
      RecogerJpeg(cinfo);
		
      fclose(fp);
    }
  else if (fmt == raw) {
    sprintf(nomfich,"Disparidad%04d.raw",cont);
    fp=fopen(nomfich, "wb");
    for(i=0;i<track->imdispar.alto;i++)
      for(j=0;j<track->imdispar.ancho;j++){
	fputc(track->imdispar.pixel[i][j],fp);
      }
    fclose(fp);
  }
}

void GuardarSilueta(TRACKER *track, int cont, formato fmt)
{
  register int i,j,m;
  FILE *fp;
  char nomfich[50];

  if (fmt == jpeg) 
    {
      sprintf(nomfich,"Silueta%04d.jpg",cont);
      if ( (fp=fopen(nomfich, "w")) == NULL) {
	printf("Error al intentar abrir: %s\n", nomfich);
	return;
      }
      jpeg_compress_struct cinfo;
      jpeg_error_mgr jerr;
      PrepararJpeg(fp, cinfo, jerr, track->silueta.ancho, track->silueta.alto, 3);
      for (int i = 0; i < track->silueta.alto; i++) {
	jpeg_write_scanlines(&cinfo, &(track->silueta.pixel[i]), 1);
      }
      RecogerJpeg(cinfo);
		
      fclose(fp);
    }
  else if (fmt == raw) {
    sprintf(nomfich,"Silueta%04d.raw",cont);
    fp=fopen(nomfich, "wb");
    for(i=0;i<track->silueta.alto;i++)
      for(j=0;j<track->silueta.ancho*3;j++){
	fputc(track->silueta.pixel[i][j],fp);
      }
    fclose(fp);
  }
}

void GuardarResultadoEstereo(TRACKER *track, int cont){

register int i,j,m;
FILE *f;
char nomfich[50];

//nomfich=new char [50];
sprintf(nomfich,"Estereo%d.raw",cont);
f=fopen(nomfich, "wb");

for(i=0;i<track->im.alto;i++)
        for(j=0;j<track->im.ancho;j++){
           for(m=0;m<NUMMARCS;m++){
                if(track->pirder[0][i][j].Clase==track->clases[m]){
                        fputc(255,f);
                }else   fputc(0,f);
           }
        }
fclose(f);

}

void GuardarMapaDisparidad(TRACKER *track, int cont) 
{
  register int i,j,m;
  FILE *f;
  char nomfich[50];
  int aux=-1;
  //nomfich=new char [50];
  sprintf(nomfich,"Disparidad%04d.raw",cont);
  f=fopen(nomfich, "wb");


  
  for(i=0;i<track->im.alto;i++)
    for(j=0;j<track->im.ancho;j++){
      aux=-1;
      for(m=0;m<NUMMARCS;m++){	
	if(track->pir[track->a][0][i][j].Clase==track->clases[m]){
	  aux=m;
	}
      }
      if(aux!=-1)
	fputc( (int)(( track->disparidad[aux] / 16.0 ) * 255.0), f);
      else   fputc(0,f);
    }
    
  
  fclose(f);
  
}

void GuardarImagenColor(unsigned char **imagen, int alto, int ancho, int cont){
  register int i,j;
  FILE *f;
  char nomfich[50];

  sprintf(nomfich,"Imagen%04d.raw",cont);
  f=fopen(nomfich,"wb");

  for(i=0;i<alto;i++)
    for(j=0;j<ancho;j++){
      fputc(imagen[i][j*3],f);
      fputc(imagen[i][j*3+1],f);
      fputc(imagen[i][j*3+2],f);
    }
  fclose(f);
}

