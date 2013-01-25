// -*-c++-*-
#ifndef RETARG_H
#define RETARG_H

#include <osg/Geode>
#include <osg/TexGen>
#include <osg/Texture1D>
#include <osg/Texture2D>
#include <osg/Matrix>
#include <osg/MatrixTransform>

#include <osgDB/ReadFile>

#include <osgViewer/Viewer>

#include <osg/Drawable>

#include <cassert>
#include <unistd.h>
#include <cstring>
#include <modelo-osg/utils.h>
#include <modelo-osg/modelo.h>
#include <modelo-osg/modeloVISOR.h>
#include <modelo-osg/modeloHOAP.h>
#include <modelo-osg/modeloNOMAD.h>

#include <obs/observadormodelo.h>
#include <obs/observadormodelocoli.h>

#include <set>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
using namespace osgDTE;

template<typename T, int tamanio> class ArgFich {
public:
  //inline int size() { return size; }
  inline void Infile(ArgFich datos,char * fichero, int ini);
  
  inline T& operator[] (int i) { return v[i]; }
  inline const T& operator[] (int i) const { return v[i]; }
  T v[tamanio];

};
template <typename T,int tamanio> void ArgFich<T,tamanio>::Infile(ArgFich datos,char * fichero, int ini)
{

ofstream fx,fy,fz;
ofstream f0,f1,f2,f3;

char bufferX[80];
char bufferY[80];
char bufferZ[80];

sprintf(bufferX, "%sX.dat", fichero);
sprintf(bufferY, "%sY.dat", fichero);
sprintf(bufferZ, "%sZ.dat", fichero);

char buffer0[80];
char buffer1[80];
char buffer2[80];
char buffer3[80];
sprintf(buffer0, "%sAng0.dat", fichero);
sprintf(buffer1, "%sAng1.dat", fichero);
sprintf(buffer2, "%sAng2.dat", fichero);
sprintf(buffer3, "%sAng3.dat", fichero);

if (ini==0)
{
  if (tamanio==3)
  {
   fx.open(bufferX);
   fy.open(bufferY);
   fz.open(bufferZ);
  }else{
  
  f0.open(buffer0);
  f1.open(buffer1);
  f2.open(buffer2);
  f3.open(buffer3);
  }

}else
{

  if (tamanio==3)
  {
   fx.open(bufferX,ios::app);
   fy.open(bufferY,ios::app);
   fz.open(bufferZ,ios::app);
  }else{
  f0.open(buffer0,ios::app);
  f1.open(buffer1,ios::app);
  f2.open(buffer2,ios::app);
  f3.open(buffer3,ios::app);
  }
}
if (tamanio==3)
{
fx<<datos[0]<<"\n";
fy<<datos[1]<<"\n";
fz<<datos[2]<<"\n";
fx.close();
fy.close();
fz.close();
}else{
f0<<datos[0]*180/PI<<"\n";
f1<<datos[1]*180/PI<<"\n";
f2<<datos[2]*180/PI<<"\n"; 
f3<<datos[3]*180/PI<<"\n"; 
f0.close();
f1.close();
f2.close();
f3.close(); 

}

}
// Clase que contiene las rutinas de aprendizaje
class Retargeting
{
 public:

  // Constructor
  Retargeting(osgDTE::modeloHOAP* mH, osgDTE::modeloVISOR* mP) ;
  Retargeting(osgDTE::modeloNOMAD* mN, osgDTE::modeloVISOR* mP) ;
  Retargeting(osgDTE::modelo* mR, osgDTE::modelo* mP) ;

  // Destructor
  ~Retargeting() {}

  // Funcion para obtener las coordenadas del extremo final del brazo con respecto al hombro usando una mezcla del retargeting de posiciones y de angulos en funcion del factor alpa (alpa = 1.0 por defecto, indicando que si no se tiene informacion alguna sobre el tipo de movimiento se debe dar prioridad al retargeting de angulos, que da por lo general menores errores) 
  vector<double> moverROBOTIzq(osg::Vec3d LArm_pos,vector<double> LArm_joint,int ini,double alpa = 1.0);
  vector<double> moverROBOTDer(osg::Vec3d RArm_pos,vector<double> RArm_joint,int ini,double alpa = 1.0);  

  //Funcion para obtener coordenadas del extremo final del brazo con respecto al hombro a partir de los angulos de los 4 DOF
  void retargAngROBOTIzq(vector<double> LArm_joint,int ini);
  void retargAngROBOTDer(vector<double> RArm_joint,int ini);
  Vec3d PosFin(vector<double> Arm_joint,bool Persona,bool Norm);
  //void InFile(ArgFich< vector<double> > datos,char * fichero,int ini);
  double Desviacion(string fichero,double media, int nmuestras);

  //Funcion para obtener coordenadas del extremo final del brazo con respecto al hombro a partir de las posiciones de los efectores finales
  void retargPosROBOTIzq(osg::Vec3d LArm_pos,vector<double> LArm_joint,int ini);
  void retargPosROBOTDer(osg::Vec3d RArm_pos,vector<double> RArm_joint,int ini);

  std::vector<double> retang_angleft;
  osg::Vec3d retang_posleft;
  std::vector<double> retang_angright;
  osg::Vec3d retang_posright;
  std::vector<double> retpos_angleft;
  osg::Vec3d retpos_posleft;
  std::vector<double> retpos_angright;
  osg::Vec3d retpos_posright;
  std::vector<double> angleft;
  osg::Vec3d posleft;
  std::vector<double> angright;
  osg::Vec3d posright;

 protected:
  /*static char Ficheros[34][20] = {"PosPersonIzqX.dat","PosPersonIzqY.dat","PosPersonIzqZ.dat","PosPersonDerX.dat","PosPersonDerY.dat","PosPersonDerZ.dat", "PosHOAPIzqX.dat","PosHOAPIzqY.dat","PosHOAPIzqZ.dat","PosHOAPDerX.dat","PosHOAPDerY.dat","PosHOAPDerZ.dat","PersIzqAng0.dat","PersIzqAng1.dat",
"PersIzqAng2.dat","PersIzqAng3.dat","PersDerAng0.dat","PersDerAng1.dat","PersDerAng2.dat","PersDerAng3.dat","ErrorPosIzqX.dat","ErrorPosIzqY.dat",
"ErrorPosIzqZ.dat","ErrorPosDerX.dat","ErrorPosDerY.dat","ErrorPosDerZ.dat","ErrorIzqAng0.dat","ErrorIzqAng1.dat","ErrorIzqAng2.dat","ErrorIzqAng3.dat",
"ErrorDerAng0.dat","ErrorDerAng1.dat","ErrorDerAng2.dat","ErrorDerAng3.dat"};*/
  osg::ref_ptr<osgDTE::modelo> _mP;
  osg::ref_ptr<osgDTE::modelo> _mR; // modelo de ROBOT (HOAP o NOMAD)
  
  double dist_hombro_mano_Persona;
  double dist_hombro_mano_ROBOT;
  osg::Vec3d pos_hombro_izq_Persona;
  osg::Vec3d pos_hombro_der_Persona;
  osg::Vec3d pos_cintura_Persona;
  osg::Vec3d pos_hombro_izq_ROBOT;
  osg::Vec3d pos_hombro_der_ROBOT;
  osg::Vec3d pos_cintura_ROBOT;

};

#endif
