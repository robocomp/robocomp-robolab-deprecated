#ifndef BIP_H
#define BIP_H
#include <vector>
#include <tracker/Config.h>
//#include <tracker/emd.h>



class CmpContactos{
PUNTO p;
public:
CmpContactos(PUNTO &_p):p(_p){}
bool operator()(const PUNTO p1){
                return ((p1.i==p.i)&&(p1.j==p.j));
                }
};

/****************************************************************************

                DEFINICIÓN DE LA CLASE BIP

****************************************************************************/



class BIP{
public:
        int niveles;
        int *TamNivelesAncho;
        int *TamNivelesAlto;
        std::vector<NV> *NodosV;
        PIRGRIS ***pir;
        int altoim;
        int anchoim;
        int ContClase;
        std::vector<Padre> padres;
        double *umbralColor;
        int nivelesI;
        int numNiveles;
	BOXGRIS *cajas;

        BIP(int _altoim, int _anchoim, int nivelesI,double umbral);
        ~BIP();
        void Segmentar(IMAGENGRIS im, double umbral);

	void Reiniciar();

private:
        int GenerarParteRegular(int nivel);
        void GenerarParteIrregular(int nivel,double umbral);
        void Clasificar();
	void LLevarColoresArriba();
        void FusionAreasPequenas();
};


template<typename color, typename color1> double DisColor(color nodo1, color1 nodo2){

  double d;
  double di;

di= (double)abs(nodo1.I-nodo2.I);

return di;
}

template <typename celda, typename celda1> NV CrearNodoVirtual(celda nodo1, celda1 nodo2){

PUNTO p1,p2,medio;
int area;
NV nodoV;


//Asigna la media de color entre los dos nodos al nodo virtual padre

area=nodo1.Area+nodo2.Area;

nodoV.I=(unsigned char)(((nodo1.I*nodo1.Area)+(nodo2.I*nodo2.Area))/(area));

nodoV.Area=area;
nodoV.Clase=-1;
nodoV.padre=-1;

return nodoV;

}


#endif
