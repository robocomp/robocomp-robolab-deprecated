#include "interpolador.h"
#include <iostream>

#define DEBUGO 0
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace std; 


// -----------------------------------------------------------
//        Cáculo de los coeficientes para el CSpline
// -----------------------------------------------------------

void Interpolador::cspline(TPunto &grupeto){

// He hecho una traduccion directa desde el matlab
// Para corregir errores he cogido el fichero posiciones y pasado al matlab.

//TPos paux;

float asx,asy,asz;
int i;
Interpolador::TVector3 h; 
Interpolador::TVector4 t,alphax,alphay,alphaz,lx,ly,lz,ux,uy,uz,zx,zy,zz;
            //cout<< "Calculando CSpline"<<endl;

for(i=0;i<=3;i++){

h[i]=((grupeto[i+1].t)-(grupeto[i].t));
}

for(i=0;i<=4;i++){

t[i]=grupeto[i].t;
cdx[0][i]=grupeto[i].pos[0];
cdy[0][i]=grupeto[i].pos[1];
cdz[0][i]=grupeto[i].pos[2];

}

if (primera){ 

aex=(cdx[0][1]-cdx[0][0])/(t[1]-t[0]); 
aey=(cdy[0][1]-cdy[0][0])/(t[1]-t[0]);
aez=(cdz[0][1]-cdz[0][0])/(t[1]-t[0]);
primera=false; 
}

asx=(cdx[0][4]-cdx[0][3])/(t[4]-t[3]);
asy=(cdy[0][4]-cdy[0][3])/(t[4]-t[3]);
asz=(cdz[0][4]-cdz[0][3])/(t[4]-t[3]);
//cout<<aex<<" "<<aey<<" "<<aez<<endl;
//cout<<asx<<" "<<asy<<" "<<asz<<endl; // Correcto los angulos

alphax[0]=3*(cdx[0][1]-cdx[0][0])/h[0]-3*aex;
alphax[4]=3*asx-3*(cdx[0][4]-cdx[0][3])/h[3];

alphay[0]=3*(cdy[0][1]-cdy[0][0])/h[0]-3*aey;
alphay[4]=3*asy-3*(cdy[0][4]-cdy[0][3])/h[3];

alphaz[0]=3*(cdz[0][1]-cdz[0][0])/h[0]-3*aez;
alphaz[4]=3*asz-3*(cdz[0][4]-cdz[0][3])/h[3];

for (i=1;i<=3;i++){
    alphax[i]= 3/h[i]*(cdx[0][i + 1]-cdx[0][i])-3/h[i - 1]*(cdx[0][i] - cdx[0][i - 1]);
    alphay[i]= 3/h[i]*(cdy[0][i + 1]-cdy[0][i])-3/h[i - 1]*(cdy[0][i] - cdy[0][i - 1]);
    alphaz[i]= 3/h[i]*(cdz[0][i + 1]-cdz[0][i])-3/h[i - 1]*(cdz[0][i] - cdz[0][i - 1]);
}

lx[0] = 2 * h[0];
ux[0] = 0.5;
zx[0] = alphax[0]/lx[0];

ly[0] = 2 * h[0];
uy[0] = 0.5;
zy[0] = alphay[0]/ly[0];

lz[0] = 2 * h[0];
uz[0] = 0.5;
zz[0] = alphaz[0]/lz[0];

for (i=1;i<=3;i++){
lx[i]=2*(t[i+1]-t[i-1])-h[i-1]*ux[i-1];
ux[i]=h[i]/lx[i];
zx[i]=(alphax[i] - h[i - 1] * zx[i - 1]) / lx[i];

ly[i]=2*(t[i+1]-t[i-1])-h[i-1]*uy[i-1];
uy[i]=h[i]/ly[i];
zy[i]=(alphay[i] - h[i - 1] * zy[i - 1]) / ly[i];

lz[i]=2*(t[i+1]-t[i-1])-h[i-1]*uz[i-1];
uz[i]=h[i]/lz[i];
zz[i]=(alphaz[i] - h[i - 1] * zz[i - 1]) / lz[i];
}

lx[4] = h[3] * (2 - ux[3]);
zx[4] = (alphax[4] - h[3] * zx[3]) / lx[4];
cdx[2][4] = zx[4];

ly[4] = h[3] * (2 - uy[3]);
zy[4] = (alphay[4] - h[3] * zy[3]) / ly[4];
cdy[2][4] = zy[4];

lz[4] = h[3] * (2 - uz[3]);
zz[4] = (alphaz[4] - h[3] * zz[3]) / lz[4];
cdz[2][4] = zz[4];

for (i=3;i>=0;i--){
    cdx[2][i] = zx[i] - ux[i] * cdx[2][i+1];//cdx[2]
    cdx[1][i] = (cdx[0][i + 1] - cdx[0][i]) / h[i] - h[i] * (cdx[2][i + 1] + 2 * cdx[2][i]) / 3;//bx
    cdx[3][i] = (cdx[2][i + 1] - cdx[2][i]) / 3 / h[i];//dx
   
    cdy[2][i] = zy[i] - uy[i] * cdy[2][i + 1];
    cdy[1][i] = (cdy[0][i + 1] - cdy[0][i]) / h[i] - h[i] * (cdy[2][i + 1] + 2 * cdy[2][i]) / 3;
    cdy[3][i] = (cdy[2][i + 1] - cdy[2][i]) / 3 / h[i];
    
    cdz[2][i] = zz[i] - uz[i] * cdz[2][i + 1];
    cdz[1][i] = (cdz[0][i + 1] - cdz[0][i]) / h[i] - h[i] * (cdz[2][i + 1] + 2 * cdz[2][i]) / 3;
    cdz[3][i] = (cdz[2][i + 1] - cdz[2][i]) / 3 / h[i];
}


 
aex=(cdx[0][1]-cdx[0][0])/(t[1]-t[0]); 
aey=(cdy[0][1]-cdy[0][0])/(t[1]-t[0]);
aez=(cdz[0][1]-cdz[0][0])/(t[1]-t[0]); // Calculo el ángulo de entrada para la siguiente interpolación, manteniendo el angulo anterior
                               // creando así una continuidad artificial en el ángulo de inicio del primer par de puntos del siguiente cuarteto

listo=true;// estamos listos para movernos
tac=ta=grupeto[2].t; // inicio tiempos para movernos
tri = microsec_clock::local_time();// tiempo actual     
}

// --------------------------------------------
//        Potencia de un número
// --------------------------------------------
float Interpolador::potencia(float n,int x){

float acum;
int i;
     acum=1.0; 
     i=1;
     
     while(i<=x){
     acum=n*acum;
     i++;
     }
return acum;
}



// Entrada de una nueva posición en el interpolador
// COMPRABADO QUE COGE TODO PERFECTAMENTE
void Interpolador::updateInterpolador(osg::Vec3d posicion)
{
     
     //Comprobado, los puntos los pasa bien asi;
     double intermedio=0.0;
     alcanzada= true;
     listo=false;
     
     
         tii = microsec_clock::local_time();         
         incremento= tii-ti;// intervalo ti+1 -ti

         intermedio=(incremento.total_microseconds());

         intermedio= intermedio/1000000;

         eje_t=eje_t+intermedio;
         
         if(primera){
         
         grupeto[n_puntos].pos=posicion;
         grupeto[n_puntos].t=eje_t;  
         if(n_puntos==2)tac=eje_t;
       /*  cout<<grupeto[n_puntos].pos[0]<<" "<<grupeto[n_puntos].pos[1]<<" "<<grupeto[n_puntos].pos[2]<<" t=";      
         cout<<grupeto[n_puntos].t<<endl<<endl;    */ 
         }
         
         else{
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
           grupeto[0]=grupeto[1];
           grupeto[1]=grupeto[2];
           grupeto[2].pos=pos;                              // Posición donde se quedo el brazo por última vez
           //grupeto[2].t=grupeto[1].t+(eje_t-grupeto[1].t)/2;// trato de asegurarme que no pase nada raro con el tiempo calculando
                                                              // el valor medio entre el tiempo de ahora y el tiempo de antes de la posicion
                                                              // que mantengo
           grupeto[2].t=grupeto[3].t;
           grupeto[3]=grupeto[4];                                               
           grupeto[4].pos=posicion;
           grupeto[4].t=eje_t;
           
          // grupeto[3].t=2*(grupeto[2].t-grupeto[1].t)+grupeto[1].t;
           n_puntos=4;                                      // Lo preparo para salir inteporlar
           
      /*   cout<<grupeto[0].pos[0]<<" "<<grupeto[0].pos[1]<<" "<<grupeto[0].pos[2]<<" t=";      
         cout<<grupeto[0].t<<endl;         
         cout<<grupeto[1].pos[0]<<" "<<grupeto[1].pos[1]<<" "<<grupeto[1].pos[2]<<" t=";      
         cout<<grupeto[1].t<<endl;         
         cout<<grupeto[2].pos[0]<<" "<<grupeto[2].pos[1]<<" "<<grupeto[2].pos[2]<<" t=";      
         cout<<grupeto[2].t<<endl;         
         cout<<grupeto[3].pos[0]<<" "<<grupeto[3].pos[1]<<" "<<grupeto[3].pos[2]<<" t=";     
         cout<<grupeto[3].t<<endl; */
         
         }
     
                
      
         ti=tii;// para el siguiente punto
         n_puntos++;
       
         if (n_puntos>4){// Ya tengo los puntos, indico que se puede interpolar, y preparo n_puntos para la siguiente interpolación
            Interpolador::cspline(grupeto);
            
         }

}


// Función que devuelve la posición interpolada,
// actualizándola además en la variable posicion
osg::Vec3d Interpolador::posicionInterpolada()
{
  double x,y,z;// coordenadas interpoladas
  double tinc;

  time_duration inc;
  

   if (tac>=grupeto[3].t){
   listo=false;  
  
}
if (listo){ 

 
              x=cdx[0][2]+cdx[1][2]*(tac-ta)+cdx[2][2]*potencia(tac-ta,2)+cdx[3][2]*potencia(tac-ta,3);
              y=cdy[0][2]+cdy[1][2]*(tac-ta)+cdy[2][2]*potencia(tac-ta,2)+cdy[3][2]*potencia(tac-ta,3);
              z=cdz[0][2]+cdz[1][2]*(tac-ta)+cdz[2][2]*potencia(tac-ta,2)+cdz[3][2]*potencia(tac-ta,3);

              
              //if(tac>=grupeto[3].t) listo=false;

              pos=osg::Vec3(x,y,z);          
              //cout<<"x "<<x<<" y "<<y<<" z "<<z<<" tac "<<tac<<endl;
     } 
       
     trii = microsec_clock::local_time(); 
     inc= trii-tri;
     tinc=(inc.total_microseconds());
     tinc=tinc/1000000;
     tac=tac+tinc;     
     tri=trii;
     
     return pos;
            

  
}
