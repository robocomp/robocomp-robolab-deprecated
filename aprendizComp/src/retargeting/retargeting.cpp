// -*-c++-*-

#include "retargeting.h"

Matrixd* RotateZ(double angle)
{

 Matrixd* m = new Matrixd;
      
  m->set( cos (angle) , -sin (angle) , 0  , 0 ,
          sin (angle) ,  cos (angle) , 0  , 0 ,
              0       ,      0       , 1  , 0 ,
              0       ,      0       , 0  , 1 );
  return m;
}
Matrixd* RotateY(double angle)
{

 Matrixd* m = new Matrixd;
  m->set( cos (angle) ,  0 , sin (angle), 0 ,
             0        ,  1 ,     0      , 0 ,
         -sin (angle) ,  0 , cos (angle), 0 ,
             0        ,  0 ,     0      , 1 );
  return m;
}
Matrixd* RotateX(double angle)
{
 
 Matrixd* m = new Matrixd;
  
  m->set(    1   ,      0      ,     0       , 0 ,
             0   , cos (angle) , -sin (angle), 0 ,
             0   , sin (angle) , cos (angle) , 0 ,
             0   ,      0      ,     0       , 1 );

 
 return m;
}
Matrixd* TranslateY(double L)
{

 Matrixd* m = new Matrixd;
  m->set(    1   ,      0      ,     0       , 0 ,
             0   ,      1      ,     0       , L ,
             0   ,      0      ,     1       , 0 ,
             0   ,      0      ,     0       , 1 );
 
  return m;
}
Matrixd* TranslateX(double L)
{

 Matrixd* m = new Matrixd;
  m->set(    1   ,      0      ,     0       , L ,
             0   ,      1      ,     0       , 0 ,
             0   ,      0      ,     1       , 0 ,
             0   ,      0      ,     0       , 1 );
 
  return m;
}
Matrixd* TranslateZ(double L)
{

 Matrixd* m = new Matrixd;
  m->set(    1   ,      0      ,     0       , 0 ,
             0   ,      1      ,     0       , 0 ,
             0   ,      0      ,     1       , L ,
             0   ,      0      ,     0       , 1 );
 
  return m;
}
void normalizar(char * fichero,double max)
{
   cout<<"Maximo"<<fichero<<":"<<max<<endl;
   ofstream f1,f2;
   double aux;
   ifstream f,f3;
   f.open(fichero);
   f1.open("auxiliar.dat");
   f>>aux;
   while (!f.eof())
   {
    if (max!=0)
     f1<<fabs(aux/max)<<"\n";
    f>>aux;
   }
   f.close();
   f1.close();
   f2.open(fichero);
   f3.open("auxiliar.dat");
   while (!f3.eof())
   {
    f3>>aux;
    f2<<aux<<"\n";
   }
}


////Funcion para obtener coordenadas del extremo final del brazo con respecto al hombro a partir de los angulos de los 4 DOF
//vector<double> Retargeting::PosicionFinal(std::vector<double> Arm_joint, char * fichero,std::vector<double> max,int cont,int Persona,int Brazo)
//{
//
//double A,B,C,D,X,Y,Z,L2,L1,Xi,Yi,Zi;
//ofstream fx,fy,fz,f0,f1,f2,f3,fEx,fEy,fEz,fE0,fE1,fE2,fE3,fprueba; 
//int ki,i;
//char bufferEX[80];
//char bufferEY[80];
//char bufferEZ[80];
//char bufferE0[80];
//char bufferE1[80];
//char bufferE2[80];
//char bufferE3[80];
//if (Persona==0)
//{
//L1=mP->jBrazoIzq.dirsegmento.length();
//L2=mP->jAntebrazoIzq.dirsegmento.length();
////printf("Pos_Mano_Persona\n");
//}
//else
//{
//
//sprintf(bufferEX, "%sEX.dat", fichero);
//sprintf(bufferEY, "%sEY.dat", fichero);
//sprintf(bufferEZ, "%sEZ.dat", fichero);
//sprintf(bufferE0, "%sAngE0.dat", fichero);
//sprintf(bufferE1, "%sAngE1.dat", fichero);
//sprintf(bufferE2, "%sAngE2.dat", fichero);
//sprintf(bufferE3, "%sAngE3.dat", fichero);
//L1=mH->jBrazoIzq.dirsegmento.length();
//L2=mH->jAntebrazoIzq.dirsegmento.length();
////printf("Pos_Mano_ROBOT\n");
//}
////printf("LongitudesPers L1:%f, L2:%f\n",L1,L2);
//
//A=-Arm_joint[0];
//B=-Arm_joint[1];
//D=-Arm_joint[3];
//C=Arm_joint[2];
////printf("AngulosRobotIzq:%f,%f,%f,%f\n",A*180/PI,B*180/PI,C*180/PI,D*180/PI);
////printf("AngulosROBOT:%f,%f,%f,%f\n",LArm_joint[0]*180/PI,LArm_joint[1]*180/PI,LArm_joint[2]*180/PI,LArm_joint[3]*180/PI); 
//Matrixd* H = new Matrixd;
//
//Matrixd* TyL2 = new Matrixd;
//Matrixd* RzC = new Matrixd;
//Matrixd* RxD = new Matrixd;
//
//Matrixd* RzB = new Matrixd;
//Matrixd* TyL1 = new Matrixd;
//
//
//H=RotateX(A);
//RzB=RotateZ(B);
//TyL1=TranslateY(L1);
//RzC=RotateZ(C);
//RxD=RotateX(D);
//TyL2=TranslateY(L2);
// 
//H->mult(*H,*RzB);
//H->mult(*H,*TyL1);
//H->mult(*H,*RxD);
//H->mult(*H,*RzC);
//H->mult(*H,*TyL2);   
//
//char bufferX[80];
//char bufferY[80];
//char bufferZ[80];
//char buffer0[80];
//char buffer1[80];
//char buffer2[80];
//char buffer3[80];
//
//sprintf(bufferX, "%sX.dat", fichero);
//sprintf(bufferY, "%sY.dat", fichero);
//sprintf(bufferZ, "%sZ.dat", fichero);
//
//sprintf(buffer0, "%sAng0.dat", fichero);
//sprintf(buffer1, "%sAng1.dat", fichero);
//sprintf(buffer2, "%sAng2.dat", fichero);
//sprintf(buffer3, "%sAng3.dat", fichero);
//X=(*H)(0,3);
//Y=(*H)(1,3);
//Z=(*H)(2,3);
//if (fabs(X)>=fabs(max[0])) max[0]=fabs(X);
//if (fabs(Y)>=fabs(max[1])) max[1]=fabs(Y);
//if (fabs(Z)>=fabs(max[2])) max[2]=fabs(Z);
//if (cont==0)
//{
//fx.open(bufferX);
//fy.open(bufferY);
//fz.open(bufferZ);
//  if (Persona!=0)
//   {
//    fprueba.open("prueba");
//    fEx.open(bufferEX);
//    fEy.open(bufferEY);
//    fEz.open(bufferEZ);
//    fE0.open(bufferE0);
//    fE1.open(bufferE1);
//    fE2.open(bufferE2);
//    fE3.open(bufferE3);
//   }
//f0.open(buffer0);
//f1.open(buffer1);
//f2.open(buffer2);
//f3.open(buffer3);
//}
//else
//{
//fx.open(bufferX,ios::app);
//fy.open(bufferY,ios::app);
//fz.open(bufferZ,ios::app);
//  if (Persona!=0)
//   {
//    fprueba.open("prueba",ios::app);
//    fEx.open(bufferEX,ios::app);
//    fEy.open(bufferEY,ios::app);
//    fEz.open(bufferEZ,ios::app);
//    fE0.open(bufferE0,ios::app);
//    fE1.open(bufferE1,ios::app);
//    fE2.open(bufferE2,ios::app);
//    fE3.open(bufferE3,ios::app);
//   }
//f0.open(buffer0,ios::app);
//f1.open(buffer1,ios::app);
//f2.open(buffer2,ios::app);
//f3.open(buffer3,ios::app);
//}
//
////no funciona
//
//if (Persona==0)
//{
//  if (Brazo==0)
//   Xi=X+17;
// else
//   Xi=X-17;
//Zi=Z+57;
// X=X/(LP1+LP2);
// Y=Y/(LP1+LP2);
// Z=Z/(LP1+LP2);
// Pos_Izq_Person.x()=X;
// Pos_Izq_Person.y()=Y;
// Pos_Izq_Person.z()=Z;
//}
//else
// {
// if (Brazo==0)
//   Xi=X+90;
// else
//   Xi=X-90;
//Zi=Z+150.5;
// X=X/(LH1+LH2);
// Y=Y/(LH1+LH2);
// Z=Z/(LH1+LH2);
// Pos_Izq_ROBOT.x()=X;
// Pos_Izq_ROBOT.y()=Y;
// Pos_Izq_ROBOT.z()=Z;
//}
//fx<<X<<"\n";
//fy<<Y<<"\n";
//fz<<Z<<"\n";
//
//fx.close();
//fy.close();
//fz.close();
//if (Persona!=0)
//{
//fEx<<fabs((Pos_Izq_Person.x()-Pos_Izq_ROBOT.x()))<<"\n";
//fEy<<fabs((Pos_Izq_Person.y()-Pos_Izq_ROBOT.y()))<<"\n";
//fEz<<fabs((Pos_Izq_Person.z()-Pos_Izq_ROBOT.z()))<<"\n";
//MediaEx=MediaEx+fabs((Pos_Izq_Person.x()-Pos_Izq_ROBOT.x()));
//MediaEy=MediaEy+fabs((Pos_Izq_Person.y()-Pos_Izq_ROBOT.y()));
//MediaEz=MediaEz+fabs((Pos_Izq_Person.z()-Pos_Izq_ROBOT.z()));
//Ecuadx=Ecuadx+(pow(fabs(Pos_Izq_Person.x()-Pos_Izq_ROBOT.x()),2));
//Ecuady=Ecuady+(pow(fabs(Pos_Izq_Person.y()-Pos_Izq_ROBOT.y()),2));
//Ecuadz=Ecuadz+(pow(fabs(Pos_Izq_Person.z()-Pos_Izq_ROBOT.z()),2));
//fEx.close();
//fEy.close();
//fEz.close();
//}
//Yi=-Y;
//Vec3d Pos_Fin;
//Pos_Fin.x()=Xi;
//Pos_Fin.y()=Yi;
//Pos_Fin.z()=Zi;
//
//
//if (Persona==1) 
// {
//  if (Brazo==0)
//  {
//    
//    (mH->ang_left_arm_ROBOT)=mH->mueveBrazoIzq(Pos_Fin,Arm_joint,ang_left_pos_antROBOT);
//     for (ki=0; ki<4; ki++)
//         {
//           ang_left_pos_antROBOT[ki] = mH->ang_left_arm[ki];
//           //IncremROBOTIzq[ki]=(((mH->ang_left_arm[ki])-(ang_left_pos_antROBOT[ki])))+IncremROBOTIzq[ki];
//           //if (fabs(IncremROBOTIzq[ki])>fabs(maxAngIzq[ki])) maxAngIzq[ki]=fabs(IncremROBOTIzq[ki]);
//           //INICIO
//            IncremROBOTIzq[ki]=(mH->ang_left_arm)[ki]-IniAngIzqROBOT[ki];
//           if ((contAuxIzq==0) & (fabs(IncremROBOTIzq[ki])>fabs(maxAngIzq[1]))) 
//           {      
//              
//              maxAngIzq[1]=fabs(IncremROBOTIzq[ki]);
//           }
//             
//           //if ((fabs(IncremPersonIzq[ki])>fabs(maxAngIzq[ki]))) maxAngIzq[ki]=fabs(IncremPersonIzq[ki]);
//         }
//    if (contAuxIzq==0)
//            {
//                for (i=0; i<4; i++)
//                  {
//	           IniAngIzqROBOT[i] = (mH->ang_left_arm)[i];
//                  }
//                contAuxIzq=1;
//           
//            }  //FIN
//         
//
//          
//    f0<<IncremROBOTIzq[0]<<"\n";
//    f1<<IncremROBOTIzq[1]<<"\n";
//    f2<<IncremROBOTIzq[2]<<"\n"; 
//    f3<<IncremROBOTIzq[3]<<"\n";         
//    printf("AngulosROBOTIzqCol:%f,%f,%f,%f\n",mH->ang_left_arm[0],mH->ang_left_arm[1],mH->ang_left_arm[2],mH->ang_left_arm[3]);
//    printf("------------------------------------------------------------------\n");
//   }
// else
//  {
//    
//    (mH->ang_right_arm_ROBOT)=mH->mueveBrazoDer(Pos_Fin,Arm_joint,ang_right_pos_antROBOT);
//     for (ki=0; ki<4; ki++)
//         {
//           ang_right_pos_antROBOT[ki] = mH->ang_right_arm_ROBOT[ki];
//          // IncremROBOTDer[ki]=(((mH->ang_right_arm_ROBOT[ki])-(ang_right_pos_antROBOT[ki]))/RangoROBOTDer[ki])+IncremROBOTDer[ki];
//           IncremROBOTDer[ki]=(mH->ang_right_arm_ROBOT)[ki]-IniAngDerROBOT[ki];
//           if ((contAuxDer==0) & (fabs(IncremROBOTDer[ki])>fabs(maxAngDer[1]))) 
//           {      
//              
//              maxAngDer[1]=fabs(IncremROBOTDer[ki]);
//           }
//             
//           //if ((fabs(IncremPersonIzq[ki])>fabs(maxAngIzq[ki]))) maxAngIzq[ki]=fabs(IncremPersonIzq[ki]);
//         }
//    if (contAuxDer==0)
//            {
//                for (i=0; i<4; i++)
//                  {
//	           IniAngDerROBOT[i] = (mH->ang_right_arm_ROBOT)[i];
//                  }
//                contAuxDer=1;
//         }
//    f0<<IncremROBOTDer[0]<<"\n";
//    f1<<IncremROBOTDer[1]<<"\n";
//    f2<<IncremROBOTDer[2]<<"\n"; 
//    f3<<IncremROBOTDer[3]<<"\n";                
//    printf("AngulosROBOTDerCol:%f,%f,%f,%f\n",mH->ang_right_arm_ROBOT[0],mH->ang_right_arm_ROBOT[1],mH->ang_right_arm_ROBOT[2],mH->ang_right_arm_ROBOT[3]);
//    printf("------------------------------------------------------------------\n");
//  }
//  fprueba<<IncremPersonIzq[0]<<"       "<<IncremROBOTIzq[0]<<"\n";
//  fE0<<fabs((IncremROBOTIzq[0]-IncremPersonIzq[0]))<<"\n";
//  fE1<<fabs((IncremROBOTIzq[1]-IncremPersonIzq[1]))<<"\n";
//  fE2<<fabs((IncremROBOTIzq[2]-IncremPersonIzq[2]))<<"\n";
//  fE3<<fabs((IncremROBOTIzq[3]-IncremPersonIzq[3]))<<"\n";
//  MediaE0=MediaE0+fabs((IncremROBOTIzq[0]-IncremPersonIzq[0]));
//  MediaE1=MediaE1+fabs((IncremROBOTIzq[1]-IncremPersonIzq[1]));
//  MediaE2=MediaE2+fabs((IncremROBOTIzq[2]-IncremPersonIzq[2]));
//  MediaE3=MediaE3+fabs((IncremROBOTIzq[3]-IncremPersonIzq[3]));
//  Ecuad0=Ecuad0+(pow(fabs((IncremROBOTIzq[0]-IncremPersonIzq[0])/IncremPersonIzq[0]),2));
//  Ecuad1=Ecuad1+(pow(fabs((IncremROBOTIzq[1]-IncremPersonIzq[1])/IncremPersonIzq[1]),2));
//  Ecuad2=Ecuad2+(pow(fabs((IncremROBOTIzq[2]-IncremPersonIzq[2])/IncremPersonIzq[2]),2));
//  Ecuad3=Ecuad3+(pow(fabs((IncremROBOTIzq[3]-IncremPersonIzq[3])/IncremPersonIzq[3]),2));
//fprueba.close();
//fE0.close();
//fE1.close();
//fE2.close();
//fE3.close();
//}
//else
//{
//  if (Brazo==0)
//  {
//    for (ki=0; ki<4; ki++)
//         {
//           //IncremPersonIzq[ki]=(((Arm_joint[ki]-ang_left_arm_aux[ki])*180/PI))+IncremPersonIzq[ki];
//           //IncremPersonIzq[ki]=(((Arm_joint[ki]-ang_left_arm_aux[ki])*180/PI))*100;
//           IncremPersonIzq[ki]=Arm_joint[ki]*180/PI-IniAngIzqPerson[ki];
//           if ((contAuxIzq==0) & (fabs(IncremPersonIzq[ki])>fabs(maxAngIzq[0]))) 
//           {      
//              
//              maxAngIzq[0]=fabs(IncremPersonIzq[ki]);
//           }
//             //printf("AngulosIzq:%f\n",ang_left_arm_aux[ki]*180/PI);  
//           //if ((fabs(IncremPersonIzq[ki])>fabs(maxAngIzq[ki]))) maxAngIzq[ki]=fabs(IncremPersonIzq[ki]);
//         }
//    if (contAuxIzq==0)
//            {
//                for (i=0; i<4; i++)
//                  {
//	           IniAngIzqPerson[i] = Arm_joint[i]*180/PI;
//                  }
//                
//           //printf("AngulosIzq:%f,%f,%f,%f\n",IniAngIzq[0],IniAngIzq[1],IniAngIzq[2],IniAngIzq[3]);
//            }  
//    
//    f0<<IncremPersonIzq[0]<<"\n";
//    f1<<IncremPersonIzq[1]<<"\n";
//    f2<<IncremPersonIzq[2]<<"\n"; 
//    f3<<IncremPersonIzq[3]<<"\n";   
//  }
//  else
//  {
//     for (ki=0; ki<4; ki++)
//         {
//          // IncremPersonDer[ki]=(((Arm_joint[ki]-ang_right_arm_aux[ki])*180/PI)/RangoPersonDer[ki])+IncremPersonDer[ki];
//          IncremPersonDer[ki]=Arm_joint[ki]*180/PI-IniAngDerPerson[ki];
//           if ((contAuxDer==0) & (fabs(IncremPersonDer[ki])>fabs(maxAngDer[0]))) 
//           {      
//              
//              maxAngDer[0]=fabs(IncremPersonDer[ki]);
//           }
//             //printf("AngulosIzq:%f\n",ang_left_arm_aux[ki]*180/PI);  
//           //if ((fabs(IncremPersonIzq[ki])>fabs(maxAngIzq[ki]))) maxAngIzq[ki]=fabs(IncremPersonIzq[ki]);
//         }
//    if (contAuxDer==0)
//            {
//                for (i=0; i<4; i++)
//                  {

//	           IniAngDerPerson[i] = Arm_joint[i]*180/PI;
//                  }
//         }
//   f0<<IncremPersonDer[0]<<"\n";
//   f1<<IncremPersonDer[1]<<"\n";
//   f2<<IncremPersonDer[2]<<"\n"; 
//   f3<<IncremPersonDer[3]<<"\n";   
//  }
//}
//f0.close();
//f1.close();
//f2.close();
//f3.close();
//
//return max;
//
//}

vector<double> DegtoRad(vector<double> angD)
{
 vector<double> angR(4);
angR[0]=angD[0]*PI/180;
angR[1]=angD[1]*PI/180;
angR[2]=angD[2]*PI/180;
angR[3]=angD[3]*PI/180;
 return angR;
}
ArgFich<double,4> copia(vector<double> angulos)
{
  ArgFich<double,4> T;
  T[0]=angulos[0];
  T[1]=angulos[1];
  T[2]=angulos[2];
  T[3]=angulos[3];
 return T;
}
double maximo(double uno,double dos)
{
 if (uno>=dos)
     return uno;
 else 
     return dos;
}
double minimo(double uno,double dos)
{
 if (uno<=dos)
     return uno;
 else 
     return dos;
}
vector<double> transformLim(vector<double> Lim)
{
  std::vector<double> LimFin(2);
  double LimInf=0;
  double LimSup=0;
  bool encontradoInf=false;
  bool encontradoSup=false;
  while ((encontradoInf==false)  & (encontradoSup==false))
  {
   if (Lim[0]<=(LimInf-90))
       LimInf=LimInf-90;
   else
      encontradoInf=true;
   if (Lim[1]>=(LimSup+90))
       LimSup=LimSup+90;
   else 
      encontradoSup=true;
  }
  LimFin[0]=LimInf;
  LimFin[1]=LimSup;
  return LimFin;
}
double transfLin(double Lim,double LimROBOT,double LimPerson, double angPers,int como)
{
  double angROBOT;
  if (como==0)
    angROBOT= ((angPers-LimPerson)/(Lim-LimPerson))*(Lim-LimROBOT)+LimROBOT;
  else
    angROBOT= ((angPers-Lim)/(LimPerson-Lim))*(LimROBOT-Lim)+Lim;
  return angROBOT;
}
//Funcion para transformar angulos de persona a angulo de ROBOT
double transformang(vector<double> LimROBOT, vector<double> LimPerson, double angPers)
{
  double delta=15;//Valor arbitrario;
  std::vector<double> LimROBOTCar(2);
  std::vector<double> LimPersonCar(2);
  double LimInf,LimSup;
  //printf("LimitesROBOT:%f,%f\n",LimROBOT[0],LimROBOT[1]);
  //printf("LimitesPerson:%f,%f\n",LimPerson[0],LimPerson[1]);
  LimROBOTCar=transformLim(LimROBOT);
  //printf("LimitesROBOTSeg:%f,%f\n",LimROBOTCar[0],LimROBOTCar[1]);
  LimPersonCar=transformLim(LimPerson);
  //printf("LimitesPersonCar:%f,%f\n",LimPersonCar[0],LimPersonCar[1]);
  LimInf=maximo(LimROBOTCar[0],LimPersonCar[0]);
  //printf("LimInferior:%f\n",LimInf);
  LimSup=minimo(LimROBOTCar[1],LimPersonCar[1]);
  //printf("LimSuperior:%f\n",LimSup);
  double angROBOT;
  if ((LimInf==LimROBOT[0]) || (LimInf==LimPerson[0]))
       LimInf=LimInf+delta;
  if ((LimSup==LimROBOT[1]) || (LimSup==LimPerson[1]))
       LimSup=LimSup-delta;
  if ((angPers>=LimInf) & (angPers<=LimSup))
       return angPers;
  else
     {
      if (angPers<LimInf)

         angROBOT= (transfLin(LimInf,LimROBOT[0],LimPerson[0],angPers,0));

      if (angPers>LimSup)

         angROBOT= (transfLin(LimSup,LimROBOT[1],LimPerson[1],angPers,1));
     }
  return angROBOT;
}
//Funcion para obtener la desviacion tipica del error
/*double Desviacion(string fichero,double media, int nmuestras)
{
   
   ifstream f;
   int i;
   double desv,aux;
   desv=0;
   f.open(fichero);
   for (i=0; i<nmuestras; i++)
    {
      f>>aux;
      desv=desv+pow(aux-media,2);
    }  
   f.close();
   desv=sqrt(desv/nmuestras);
   return desv;
   
}*/
Retargeting:: Retargeting(osgDTE::modeloHOAP* mH, osgDTE::modeloVISOR* mP)
: _mP(mP),
  _mR(mH)
{
 assert(_mP.valid());
 assert(_mR.valid());

 retang_angleft.resize(4);
 retang_angright.resize(4);
 retpos_angleft.resize(4);
 retpos_angright.resize(4);
 angleft.resize(4);
 angright.resize(4);

 // Inicializa valores clave de la persona y el robot
 dist_hombro_mano_Persona = _mP->accedeJoint("jBrazoDer")->dirsegmento.length() + _mP->accedeJoint("jAntebrazoDer")->dirsegmento.length();
 pos_hombro_izq_Persona = _mP->accedeJoint("jBrazoIzq")->trf->getMatrix().getTrans() + _mP->accedeJoint("jPecho")->trf->getMatrix().getTrans(); 
 pos_hombro_der_Persona = _mP->accedeJoint("jBrazoDer")->trf->getMatrix().getTrans() + _mP->accedeJoint("jPecho")->trf->getMatrix().getTrans(); 
 pos_cintura_Persona = _mP->accedeJoint("jCintura")->trf->getMatrix().getTrans(); 

 dist_hombro_mano_ROBOT = _mR->accedeJoint("jBrazoDer")->dirsegmento.length() + _mR->accedeJoint("jAntebrazoDer")->dirsegmento.length();
 pos_hombro_izq_ROBOT = _mR->accedeJoint("jBrazoIzq")->trf->getMatrix().getTrans() + _mR->accedeJoint("jPecho")->trf->getMatrix().getTrans(); 
 pos_hombro_der_ROBOT = _mR->accedeJoint("jBrazoDer")->trf->getMatrix().getTrans() + _mR->accedeJoint("jPecho")->trf->getMatrix().getTrans(); 
 pos_cintura_ROBOT = _mR->accedeJoint("jCintura")->trf->getMatrix().getTrans(); 

}

Retargeting:: Retargeting(osgDTE::modeloNOMAD* mN, osgDTE::modeloVISOR* mP)
: _mP(mP),
  _mR(mN)
{
 assert(_mP.valid());
 assert(_mR.valid());

 retang_angleft.resize(4);
 retang_angright.resize(4);
 retpos_angleft.resize(4);
 retpos_angright.resize(4);
 angleft.resize(4);
 angright.resize(4);

 // Inicializa valores clave de la persona y el robot
 dist_hombro_mano_Persona = _mP->accedeJoint("jBrazoDer")->dirsegmento.length() + _mP->accedeJoint("jAntebrazoDer")->dirsegmento.length();
 pos_hombro_izq_Persona = _mP->accedeJoint("jBrazoIzq")->trf->getMatrix().getTrans();
 pos_hombro_der_Persona = _mP->accedeJoint("jBrazoDer")->trf->getMatrix().getTrans();
 pos_cintura_Persona = _mP->accedeJoint("jCintura")->trf->getMatrix().getTrans(); 

 dist_hombro_mano_ROBOT = _mR->accedeJoint("jBrazoDer")->dirsegmento.length() + _mR->accedeJoint("jAntebrazoDer")->dirsegmento.length();
 pos_hombro_izq_ROBOT = _mR->accedeJoint("jBrazoIzq")->trf->getMatrix().getTrans();
 pos_hombro_der_ROBOT = _mR->accedeJoint("jBrazoDer")->trf->getMatrix().getTrans();
 pos_cintura_ROBOT = _mR->accedeJoint("jCintura")->trf->getMatrix().getTrans(); 

}

Retargeting:: Retargeting(osgDTE::modelo* mR, osgDTE::modelo* mP)
: _mP(mP),
  _mR(mR)
{
 assert(_mP.valid());
 assert(_mR.valid());

 retang_angleft.resize(4);
 retang_angright.resize(4);
 retpos_angleft.resize(4);
 retpos_angright.resize(4);
 angleft.resize(4);
 angright.resize(4);

 // Inicializa valores clave de la persona y el robot
 dist_hombro_mano_Persona = _mP->accedeJoint("jBrazoDer")->dirsegmento.length() + _mP->accedeJoint("jAntebrazoDer")->dirsegmento.length();
 pos_hombro_izq_Persona = _mP->accedeJoint("jBrazoIzq")->trf->getMatrix().getTrans() + _mP->accedeJoint("jPecho")->trf->getMatrix().getTrans(); 
 pos_hombro_der_Persona = _mP->accedeJoint("jBrazoDer")->trf->getMatrix().getTrans() + _mP->accedeJoint("jPecho")->trf->getMatrix().getTrans(); 
 pos_cintura_Persona = _mP->accedeJoint("jCintura")->trf->getMatrix().getTrans(); 
 
 dist_hombro_mano_ROBOT = _mR->accedeJoint("jBrazoDer")->dirsegmento.length() + _mR->accedeJoint("jAntebrazoDer")->dirsegmento.length();
 if (_mR->accedeJoint("jPecho")==0) {
   pos_hombro_izq_ROBOT = _mR->accedeJoint("jBrazoIzq")->trf->getMatrix().getTrans();
   pos_hombro_der_ROBOT = _mR->accedeJoint("jBrazoDer")->trf->getMatrix().getTrans();
 }
 else {
   pos_hombro_izq_ROBOT = _mR->accedeJoint("jBrazoIzq")->trf->getMatrix().getTrans() + _mR->accedeJoint("jPecho")->trf->getMatrix().getTrans(); 
   pos_hombro_der_ROBOT = _mR->accedeJoint("jBrazoDer")->trf->getMatrix().getTrans() + _mR->accedeJoint("jPecho")->trf->getMatrix().getTrans(); 
 }
 pos_cintura_ROBOT = _mR->accedeJoint("jCintura")->trf->getMatrix().getTrans(); 

}

double Retargeting::Desviacion(string fich,double media, int nmuestras)
{
   
   ifstream f;
   int i;
   double desv,aux;
   desv=0;
   char fichero[20];
   strcpy(fichero,fich.c_str());
   f.open(fichero);
   for (i=0; i<nmuestras; i++)
    {
      f>>aux;
      desv=desv+pow(aux-media,2);
    }  
   f.close();
   desv=sqrt(desv/nmuestras);
   return desv;
   
}

Vec3d Retargeting::PosFin(vector<double> Arm_joint,bool Persona,bool Norm)
{
double A,B,C,D,X,Y,Z,L2,L1;

if (Persona)
{
  L1=_mP->accedeJoint("jBrazoDer")->dirsegmento.length();
  L2=_mP->accedeJoint("jAntebrazoDer")->dirsegmento.length();
}else{
  L1=_mR->accedeJoint("jBrazoDer")->dirsegmento.length();
  L2=_mR->accedeJoint("jAntebrazoDer")->dirsegmento.length();
}

A=-Arm_joint[0];
B=-Arm_joint[1];
D=-Arm_joint[3];
C=Arm_joint[2];

Matrixd* H = new Matrixd;
Matrixd* TyL2 = new Matrixd;
Matrixd* RzC = new Matrixd;
Matrixd* RxD = new Matrixd;
Matrixd* RzB = new Matrixd;
Matrixd* TyL1 = new Matrixd;

H=RotateX(A);
RzB=RotateZ(B);
TyL1=TranslateY(L1);
RzC=RotateZ(C);
RxD=RotateX(D);
TyL2=TranslateY(L2);
 
H->mult(*H,*RzB);
H->mult(*H,*TyL1);
H->mult(*H,*RxD);
H->mult(*H,*RzC);
H->mult(*H,*TyL2);   


X=(*H)(0,3);
Y=(*H)(1,3);
Z=(*H)(2,3);

Vec3d Pos_Fin;
Pos_Fin.x()=X;
Pos_Fin.y()=Y;
Pos_Fin.z()=Z;
if (Norm)
{
Pos_Fin.x()=Pos_Fin.x()/(L1+L2);
Pos_Fin.z()=Pos_Fin.z()/(L1+L2);
Pos_Fin.y()=Pos_Fin.y()/(L1+L2);
}

return Pos_Fin; 
}

// Funcion para obtener las coordenadas del extremo final del brazo con respecto al hombro usando una mezcla del retargeting de posiciones y de angulos en funcion del factor alpa (alpa = 1.0 por defecto, indicando que si no se tiene informacion alguna sobre el tipo de movimiento se debe dar prioridad al retargeting de angulos, que da por lo general menores errores)
vector<double> Retargeting::moverROBOTIzq(osg::Vec3d LArm_pos,vector<double> LArm_joint,int ini,double alpa)
{
  vector<double> Info_ang(7);
  vector<double> Info_pos(7);
  vector<double> Info(7);
  Vec3d Pos_Fin;
  vector<double> MediaErrorAngIzq(4);
  ArgFich<double,4> ErrorAngIzq;
  std::vector<double> angROBOT(4);

  vector<double> ang_ant(4);
  Vec3d pos_Fin;  

  if ( (alpa<0.0)||(alpa>1.0) )
    {
      std::cout<<"Retargeting: Valor de alpa por debajo de 0.0 o por encima de 1.0. Fijando alpa = 1.0"<<endl; 
      alpa = 1.0;
    }

  // Guarda la información de la posición anterior
  ang_ant = _mR->ang_left_arm;
  ang_ant = DegtoRad(ang_ant);
  pos_Fin=PosFin(ang_ant,false,false);   
  pos_Fin.x()=pos_Fin.x()+pos_hombro_izq_ROBOT.x();
  pos_Fin.z()=pos_Fin.z()+pos_hombro_izq_ROBOT.z()+pos_cintura_ROBOT.z();
  pos_Fin.y()=-pos_Fin.y();

  // Recupera el contexto de retarg. de posiciones para evitar discontinuidades
  (_mR->ang_left_arm)=_mR->mueveBrazoIzq(retpos_posleft,retpos_angleft,_mR->ang_left_arm);
  // Calcula la información proveniente del retargeting de posiciones (guardada en Info_pos)
  retargPosROBOTIzq(LArm_pos,LArm_joint, ini);
  // Recupera la posición anterior para el retargeting de ángulos
  (_mR->ang_left_arm)=_mR->mueveBrazoIzq(pos_Fin,ang_ant,_mR->ang_left_arm);

  // Recupera el contexto de retarg. de ángulos para evitar discontinuidades
  (_mR->ang_left_arm)=_mR->mueveBrazoIzq(retang_posleft,retang_angleft,_mR->ang_left_arm);
  // Calcula la información proveniente del retargeting de ángulos (guardada en Info_ang)
  retargAngROBOTIzq(LArm_joint, ini);
  // Recupera la posición anterior para el retargeting total
  (_mR->ang_left_arm)=_mR->mueveBrazoIzq(pos_Fin,ang_ant,_mR->ang_left_arm);

  //cout<<"Left"<<endl;
  //cout<<"Xpos: "<<retpos_posleft.x()<<" Ypos: "<<retpos_posleft.y()<<" Zpos: "<<retpos_posleft.z()<<endl;
  //cout<<"Xang: "<<retang_posleft.x()<<" Yang: "<<retang_posleft.y()<<" Zang: "<<retang_posleft.z()<<endl;
  
  // Combina ambos tipos de retargeting según el valor de alpa 
  for (int i=0;i<4;i++)
    angleft[i] = (alpa*retang_angleft[i])+((1.0-alpa)*retpos_angleft[i]);  
  posleft.x() = (alpa*retang_posleft.x())+((1.0-alpa)*retpos_posleft.x());  
  posleft.y() = (alpa*retang_posleft.y())+((1.0-alpa)*retpos_posleft.y());  
  posleft.z() = (alpa*retang_posleft.z())+((1.0-alpa)*retpos_posleft.z());
  // Mueve el brazo izquierdo a su posición definitiva
  (_mR->ang_left_arm)=_mR->mueveBrazoIzq(posleft,angleft,_mR->ang_left_arm);

  // Salva los datos de posición y ángulo (en radianes)
  angleft = DegtoRad(_mR->ang_left_arm);
  posleft = PosFin(retang_angleft,false,false);
  posleft.x()=posleft.x()+pos_hombro_izq_ROBOT.x();			 
  posleft.z()=posleft.z()+pos_hombro_izq_ROBOT.z()+pos_cintura_ROBOT.z();
  posleft.y()=-posleft.y();
  
  ArgFich<double,4> AngH;
  //AngH=copia(angROBOT);
  AngH=copia(DegtoRad(_mR->ang_left_arm));
  AngH.Infile(AngH,"ROBOTIzq",ini);
  Pos_Fin=PosFin(DegtoRad(_mR->ang_left_arm),false,true);
  //Pos_Fin=PosFin(angROBOT,false,true);
  for (int i=0;i<4;i++)
    {
      ErrorAngIzq[i]=(LArm_joint[i]-(_mR->ang_left_arm[i]*PI/180));
      MediaErrorAngIzq[i]=MediaErrorAngIzq[i]+ErrorAngIzq[i];
    }
  ErrorAngIzq.Infile(ErrorAngIzq,"ErrorIzq",ini);
  for (int i=0;i<4;i++)
    {
      if (i<3) Info[i]=Pos_Fin[i];
      Info[i+3]=MediaErrorAngIzq[i];
    }
  return Info;
}

// Funcion para obtener las coordenadas del extremo final del brazo con respecto al hombro usando una mezcla del retargeting de posiciones y de angulos en funcion del factor alpa (alpa = 1.0 por defecto, indicando que si no se tiene informacion alguna sobre el tipo de movimiento se debe dar prioridad al retargeting de angulos, que da por lo general menores errores)
vector<double> Retargeting::moverROBOTDer(osg::Vec3d RArm_pos,vector<double> RArm_joint,int ini,double alpa)
{
  
  vector<double> Info_ang(7);
  vector<double> Info_pos(7);
  vector<double> Info(7);
  Vec3d Pos_Fin;
  vector<double> MediaErrorAngDer(4);
  ArgFich<double,4> ErrorAngDer;

  vector<double> ang_ant(4);
  Vec3d pos_Fin;  

  if ( (alpa<0.0)||(alpa>1.0) )
    {
      std::cout<<"Retargeting: Valor de alpa por debajo de 0.0 o por encima de 1.0. Fijando alpa = 1.0"<<endl; 
      alpa = 1.0;
    }

  // Guarda la información de la posición anterior
  ang_ant = _mR->ang_right_arm;
  ang_ant = DegtoRad(ang_ant);
  pos_Fin=PosFin(ang_ant,false,false);   
  pos_Fin.x()=pos_Fin.x()+pos_hombro_der_ROBOT.x();			 
  pos_Fin.z()=pos_Fin.z()+pos_hombro_der_ROBOT.z()+pos_cintura_ROBOT.z();
  pos_Fin.y()=-pos_Fin.y();

  // Recupera el contexto de retarg. de posiciones para evitar discontinuidades
  (_mR->ang_right_arm)=_mR->mueveBrazoDer(retpos_posright,retpos_angright,_mR->ang_right_arm);
  // Calcula la información proveniente del retargeting de posiciones (guardada en Info_pos)
  retargPosROBOTDer(RArm_pos, RArm_joint, ini);
  // Recupera la posición anterior para el retargeting de ángulos
  (_mR->ang_right_arm)=_mR->mueveBrazoDer(pos_Fin,ang_ant,_mR->ang_right_arm);
   
  // Recupera el contexto de retarg. de ángulos para evitar discontinuidades
  (_mR->ang_right_arm)=_mR->mueveBrazoDer(retang_posright,retang_angright,_mR->ang_right_arm);
  // Calcula la información proveniente del retargeting de ángulos (guardada en Info_ang)
  retargAngROBOTDer(RArm_joint, ini);
  // Recupera la posición anterior para el retargeting total
  (_mR->ang_right_arm)=_mR->mueveBrazoDer(pos_Fin,ang_ant,_mR->ang_right_arm);


  
  //cout<<"Right"<<endl;
  //cout<<"Xpos: "<<retpos_posright.x()<<" Ypos: "<<retpos_posright.y()<<" Zpos: "<<retpos_posright.z()<<endl;
  //cout<<"Xang: "<<retang_posright.x()<<" Yang: "<<retang_posright.y()<<" Zang: "<<retang_posright.z()<<endl;

  // Combina ambos tipos de retargeting según el valor de alpa 
  for (int i=0;i<4;i++)
    angright[i] = (alpa*retang_angright[i])+((1.0-alpa)*retpos_angright[i]);  
  posright.x() = (alpa*retang_posright.x())+((1.0-alpa)*retpos_posright.x());  
  posright.y() = (alpa*retang_posright.y())+((1.0-alpa)*retpos_posright.y());  
  posright.z() = (alpa*retang_posright.z())+((1.0-alpa)*retpos_posright.z());
  // Mueve el brazo derecho a su posición definitiva
  (_mR->ang_right_arm)=_mR->mueveBrazoDer(posright,angright,_mR->ang_right_arm);
  
  // Salva los datos de posición y ángulo (en radianes)
  angright = DegtoRad(_mR->ang_right_arm);
  posright = PosFin(retang_angright,false,false);
  posright.x()=posright.x()+pos_hombro_der_ROBOT.x();			 
  posright.z()=posright.z()+pos_hombro_der_ROBOT.z()+pos_cintura_ROBOT.z();
  posright.y()=-posright.y();

  ArgFich<double,4> AngH;
  // AngH=copia(angROBOT);
  AngH=copia(DegtoRad(_mR->ang_right_arm));
  AngH.Infile(AngH,"ROBOTDer",ini);
  Pos_Fin=PosFin(DegtoRad(_mR->ang_right_arm),false,true);
  //Pos_Fin=PosFin(angROBOT,false,true);
  for (int i=0;i<4;i++)
    {
      ErrorAngDer[i]=(RArm_joint[i]-(_mR->ang_right_arm[i]*PI/180));
      MediaErrorAngDer[i]=MediaErrorAngDer[i]+ErrorAngDer[i];
    }
  ErrorAngDer.Infile(ErrorAngDer,"ErrorDer",ini);
  for (int i=0;i<4;i++)
    {
      if (i<3) Info[i]=Pos_Fin[i];
      Info[i+3]=MediaErrorAngDer[i];
    }
  
  return Info;   
}
 
//Funcion para obtener coordenadas del extremo final del brazo con respecto al hombro a partir de los angulos de los 4 DOF
void Retargeting::retargAngROBOTIzq(vector<double> LArm_joint,int ini)
{

Vec3d Pos_Fin;
//vector<double> Info(7);
//vector<double> MediaErrorAngIzq(4);
//ArgFich<double,4> ErrorAngIzq;
std::vector<double> LimROBOT0(2);
std::vector<double> LimPerson0(2);

std::vector<double> LimROBOT1(2);
std::vector<double> LimPerson1(2);

std::vector<double> LimROBOT2(2);
std::vector<double> LimPerson2(2);

std::vector<double> LimROBOT3(2);
std::vector<double> LimPerson3(2);

std::vector<double> angROBOT(4);
  
LimROBOT0[0]=(_mR->accedeJoint("jBrazoIzq")->limiteeje[0].bajo);
LimROBOT0[1]=(_mR->accedeJoint("jBrazoIzq")->limiteeje[0].alto);
LimPerson0[0]=(_mP->accedeJoint("jBrazoIzq")->limiteeje[0].bajo);
LimPerson0[1]=(_mP->accedeJoint("jBrazoIzq")->limiteeje[0].alto);

LimROBOT1[0]=_mR->accedeJoint("jBrazoIzq")->limiteeje[1].bajo;
LimROBOT1[1]=_mR->accedeJoint("jBrazoIzq")->limiteeje[1].alto;
LimPerson1[0]=_mP->accedeJoint("jBrazoIzq")->limiteeje[1].bajo;
LimPerson1[1]=_mP->accedeJoint("jBrazoIzq")->limiteeje[1].alto;

LimROBOT2[0]=_mR->accedeJoint("jAntebrazoIzq")->limiteeje[2].bajo;
LimROBOT2[1]=_mR->accedeJoint("jAntebrazoIzq")->limiteeje[2].alto;
LimPerson2[0]=_mP->accedeJoint("jAntebrazoIzq")->limiteeje[2].bajo;
LimPerson2[1]=_mP->accedeJoint("jAntebrazoIzq")->limiteeje[2].alto;

LimROBOT3[0]=_mR->accedeJoint("jAntebrazoIzq")->limiteeje[0].bajo;
LimROBOT3[1]=_mR->accedeJoint("jAntebrazoIzq")->limiteeje[0].alto;
LimPerson3[0]=_mP->accedeJoint("jAntebrazoIzq")->limiteeje[0].bajo;
LimPerson3[1]=_mP->accedeJoint("jAntebrazoIzq")->limiteeje[0].alto;

angROBOT[0]=transformang(LimROBOT0,LimPerson0,LArm_joint[0]*180/PI);
angROBOT[1]=transformang(LimROBOT1,LimPerson1,LArm_joint[1]*180/PI);
angROBOT[2]=transformang(LimROBOT2,LimPerson2,LArm_joint[2]*180/PI);
angROBOT[3]=transformang(LimROBOT3,LimPerson3,LArm_joint[3]*180/PI);

angROBOT[0]=angROBOT[0]*PI/180;
angROBOT[1]=angROBOT[1]*PI/180;
angROBOT[2]=angROBOT[2]*PI/180;
angROBOT[3]=angROBOT[3]*PI/180;

Pos_Fin=PosFin(angROBOT,false,false);
   
Pos_Fin.x()=Pos_Fin.x()+pos_hombro_izq_ROBOT.x();			 
Pos_Fin.z()=Pos_Fin.z()+pos_hombro_izq_ROBOT.z()+pos_cintura_ROBOT.z();
Pos_Fin.y()=-Pos_Fin.y();
 (_mR->ang_left_arm)=_mR->mueveBrazoIzq(Pos_Fin,angROBOT,_mR->ang_left_arm);

 // Salva los datos de posición y ángulo (en radianes)
 retang_angleft = DegtoRad(_mR->ang_left_arm);
 retang_posleft = PosFin(retang_angleft,false,false);
 retang_posleft.x()=retang_posleft.x()+pos_hombro_izq_ROBOT.x();			 
 retang_posleft.z()=retang_posleft.z()+pos_hombro_izq_ROBOT.z()+pos_cintura_ROBOT.z();
 retang_posleft.y()=-retang_posleft.y();
  
 //ArgFich<double,4> AngH;
 ////AngH=copia(angROBOT);
 //AngH=copia(DegtoRad(_mR->ang_left_arm));
 //AngH.Infile(AngH,"ROBOTIzq",ini);
 //Pos_Fin=PosFin(DegtoRad(_mR->ang_left_arm),false,true);
 ////Pos_Fin=PosFin(angROBOT,false,true);
 //for (int i=0;i<4;i++)
 //  {
 //    ErrorAngIzq[i]=(LArm_joint[i]-angROBOT[i]);
 //    MediaErrorAngIzq[i]=MediaErrorAngIzq[i]+ErrorAngIzq[i];
 //  }
 //ErrorAngIzq.Infile(ErrorAngIzq,"ErrorIzq",ini);
 //for (int i=0;i<4;i++)
 //  {
 //    if (i<3) Info[i]=Pos_Fin[i];
 //    Info[i+3]=MediaErrorAngIzq[i];
 //  }
 // return Info;
}

void Retargeting::retargAngROBOTDer(vector<double> RArm_joint,int ini)
{

  Vec3d Pos_Fin;
  //vector <double> Info(7);
  //vector<double> MediaErrorAngDer(4);
  //ArgFich<double,4> ErrorAngDer;
  std::vector<double> LimROBOT0(2);
  std::vector<double> LimPerson0(2);
  
  std::vector<double> LimROBOT1(2);
  std::vector<double> LimPerson1(2);
  
  std::vector<double> LimROBOT2(2);
  std::vector<double> LimPerson2(2);
  
  std::vector<double> LimROBOT3(2);
  std::vector<double> LimPerson3(2);
  
  std::vector<double> angROBOT(4);
  
  LimROBOT0[0]=(_mR->accedeJoint("jBrazoDer")->limiteeje[0].bajo);
  LimROBOT0[1]=(_mR->accedeJoint("jBrazoDer")->limiteeje[0].alto);
  LimPerson0[0]=(_mP->accedeJoint("jBrazoDer")->limiteeje[0].bajo);
  LimPerson0[1]=(_mP->accedeJoint("jBrazoDer")->limiteeje[0].alto);
  
  LimROBOT1[0]=_mR->accedeJoint("jBrazoDer")->limiteeje[1].bajo;
  LimROBOT1[1]=_mR->accedeJoint("jBrazoDer")->limiteeje[1].alto;
  LimPerson1[0]=_mP->accedeJoint("jBrazoDer")->limiteeje[1].bajo;
  LimPerson1[1]=_mP->accedeJoint("jBrazoDer")->limiteeje[1].alto;
  
  LimROBOT2[0]=_mR->accedeJoint("jAntebrazoDer")->limiteeje[2].bajo;
  LimROBOT2[1]=_mR->accedeJoint("jAntebrazoDer")->limiteeje[2].alto;
  LimPerson2[0]=_mP->accedeJoint("jAntebrazoDer")->limiteeje[2].bajo;
  LimPerson2[1]=_mP->accedeJoint("jAntebrazoDer")->limiteeje[2].alto;
  
  LimROBOT3[0]=_mR->accedeJoint("jAntebrazoDer")->limiteeje[0].bajo;
  LimROBOT3[1]=_mR->accedeJoint("jAntebrazoDer")->limiteeje[0].alto;
  LimPerson3[0]=_mP->accedeJoint("jAntebrazoDer")->limiteeje[0].bajo;
  LimPerson3[1]=_mP->accedeJoint("jAntebrazoDer")->limiteeje[0].alto;
  
  angROBOT[0]=transformang(LimROBOT0,LimPerson0,RArm_joint[0]*180/PI);
  angROBOT[1]=transformang(LimROBOT1,LimPerson1,RArm_joint[1]*180/PI);
  angROBOT[2]=transformang(LimROBOT2,LimPerson2,RArm_joint[2]*180/PI);
  angROBOT[3]=transformang(LimROBOT3,LimPerson3,RArm_joint[3]*180/PI);
  
  angROBOT[0]=angROBOT[0]*PI/180;
  angROBOT[1]=angROBOT[1]*PI/180;
  angROBOT[2]=angROBOT[2]*PI/180;
  angROBOT[3]=angROBOT[3]*PI/180;
  
  Pos_Fin=PosFin(angROBOT,false,false);
  
  Pos_Fin.x()=Pos_Fin.x()+pos_hombro_der_ROBOT.x();			   
  Pos_Fin.z()=Pos_Fin.z()+pos_hombro_der_ROBOT.z()+pos_cintura_ROBOT.z();
  Pos_Fin.y()=-Pos_Fin.y();
  
  (_mR->ang_right_arm)=_mR->mueveBrazoDer(Pos_Fin,angROBOT,_mR->ang_right_arm);

  // Salva los datos de posición y ángulo (en radianes)
  retang_angright = DegtoRad(_mR->ang_right_arm);
  retang_posright = PosFin(retang_angright,false,false);
  retang_posright.x()=retang_posright.x()+pos_hombro_der_ROBOT.x();			   
  retang_posright.z()=retang_posright.z()+pos_hombro_der_ROBOT.z()+pos_cintura_ROBOT.z();
  retang_posright.y()=-retang_posright.y();

  //ArgFich<double,4> AngH;
  //// AngH=copia(angROBOT);
  //AngH=copia(DegtoRad(_mR->ang_right_arm));
  //AngH.Infile(AngH,"ROBOTDer",ini);
  //Pos_Fin=PosFin(DegtoRad(_mR->ang_right_arm),false,true);
  ////Pos_Fin=PosFin(angROBOT,false,true);
  //for (int i=0;i<4;i++)
  //  {
  //    ErrorAngDer[i]=(RArm_joint[i]-angROBOT[i]);
  //    MediaErrorAngDer[i]=MediaErrorAngDer[i]+ErrorAngDer[i];
  //  }
  //ErrorAngDer.Infile(ErrorAngDer,"ErrorDer",ini);
  //for (int i=0;i<4;i++)
  //  {
  //    if (i<3) Info[i]=Pos_Fin[i];
  //    Info[i+3]=MediaErrorAngDer[i];
  //  }
  //
  //return Info;   
}

//Funcion para obtener coordenadas del extremo final del brazo con respecto al hombro a partir de las posiciones de los efectores finales. La posición de entrada es la de la mano de la persona REFERIDA a su cintura
void Retargeting::retargPosROBOTIzq(osg::Vec3d LArm_pos,vector<double> LArm_joint,int ini)
{

  osg::Vec3d manizq_ROBOT;
  double factor;
  factor = (dist_hombro_mano_ROBOT)/(dist_hombro_mano_Persona); // Brazo ROBOT (mm) / Brazo TIO (cm)

  // Lee la altura del modelo
  int alto = _mP->devuelveAltura();
  // Obtiene el factor de escalado sobre una altura base de modelo de 159,5 cm., que es la altura del modeloVISOR
  double factor2 = ((double)(alto)) / 159.5;
  // Normaliza las posiciones (relativas a cintura) según el factor de escalado
  LArm_pos *= factor2;
  
  manizq_ROBOT = LArm_pos;
  manizq_ROBOT.x() = LArm_pos.x() - pos_hombro_izq_Persona.x();
  manizq_ROBOT.z() = LArm_pos.z() - pos_hombro_izq_Persona.z();
  manizq_ROBOT = manizq_ROBOT + osg::Vec3d(0.0,0.0,pos_cintura_Persona.z());
  manizq_ROBOT *= factor; // Normalización LOCAL al hombro
  manizq_ROBOT.x() = manizq_ROBOT.x() + pos_hombro_izq_ROBOT.x();
  manizq_ROBOT.z() = manizq_ROBOT.z() + pos_hombro_izq_ROBOT.z();
  manizq_ROBOT = manizq_ROBOT + osg::Vec3d(0.0,0.0,pos_cintura_ROBOT.z()); // ajuste a la cintura del ROBOT
  
  // Mueve los brazos del ROBOT
  _mR->mueveBrazoIzq(manizq_ROBOT);
  //_mR->ang_left_arm = _mR->mueveBrazoIzq(manizq_ROBOT,LArm_joint,_mR->ang_left_arm);

  // Salva los datos de posición y ángulo (en radianes)
  retpos_angleft = DegtoRad(_mR->ang_left_arm);
  retpos_posleft = PosFin(retpos_angleft,false,false);
  retpos_posleft.x()=retpos_posleft.x()+pos_hombro_izq_ROBOT.x();		      
  retpos_posleft.z()=retpos_posleft.z()+pos_hombro_izq_ROBOT.z()+pos_cintura_ROBOT.z();
  retpos_posleft.y()=-retpos_posleft.y();

}

//Funcion para obtener coordenadas del extremo final del brazo con respecto al hombro a partir de las posiciones de los efectores finales. La posición de entrada es la de la mano de la persona REFERIDA a su cintura
void Retargeting::retargPosROBOTDer(osg::Vec3d RArm_pos,vector<double> RArm_joint,int ini)
{

  osg::Vec3d mander_ROBOT;
  double factor;
  factor = (dist_hombro_mano_ROBOT)/(dist_hombro_mano_Persona); // Brazo ROBOT (mm) / Brazo TIO (cm)

  // Lee la altura del modelo
  int alto = _mP->devuelveAltura();
  // Obtiene el factor de escalado sobre una altura base de modelo de 159,5 cm., que es la altura del modeloVISOR
  double factor2 = ((double)(alto)) / 159.5;
  // Normaliza las posiciones (relativas a cintura) según el factor de escalado
  RArm_pos *= factor2;

  mander_ROBOT = RArm_pos;
  mander_ROBOT.x() = RArm_pos.x() - pos_hombro_der_Persona.x();
  mander_ROBOT.z() = RArm_pos.z() - pos_hombro_der_Persona.z();
  mander_ROBOT = mander_ROBOT + osg::Vec3d(0.0,0.0,pos_cintura_Persona.z());
  mander_ROBOT *= factor; // Normalización LOCAL al hombro
  mander_ROBOT.x() = mander_ROBOT.x() + pos_hombro_der_ROBOT.x();
  mander_ROBOT.z() = mander_ROBOT.z() + pos_hombro_der_ROBOT.z();
  mander_ROBOT = mander_ROBOT + osg::Vec3d(0.0,0.0,pos_cintura_ROBOT.z()); // ajuste a la cintura del ROBOT

  // Mueve los brazos del ROBOT
  _mR->mueveBrazoDer(mander_ROBOT);
  //_mR->ang_right_arm = _mR->mueveBrazoDer(mander_ROBOT,RArm_joint,_mR->ang_right_arm);

  // Salva los datos de posición y ángulo (en radianes)
  retpos_angright = DegtoRad(_mR->ang_right_arm);
  retpos_posright = PosFin(retpos_angright,false,false);
  retpos_posright.x()=retpos_posright.x()+pos_hombro_der_ROBOT.x();			 
  retpos_posright.z()=retpos_posright.z()+pos_hombro_der_ROBOT.z()+pos_cintura_ROBOT.z();
  retpos_posright.y()=-retpos_posright.y();

}



