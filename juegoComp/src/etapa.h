#ifndef ETAPA_H
#define ETAPA_H

#include <qstring.h>
#include <QMat/QMatAll>


class Etapa
{
  
private:
  QString fraseInicio;
  QString fraseFin;
  QString fraseTiempo;
  QString nombreObjetivo;
  QString id;
  QVec posFin;
  int rangoError;
  int tiempoDisparoFrase;
  bool seguirObjetivo;
  
public:
  Etapa();
  /// frase a decir por etapas 
  Etapa(QString _id, QString _fraseInicio, QString _fraseFin, QString _fraseTiempo, QVec _posFin, int _rangoError, int _tiempoDisparoFrase, QString _nombreObjetivo, bool _seguirObjetivo);

  ~Etapa();
  
  void setId(QString name);
  
  QString getId();
  
  void setFraseInicio(QString frase);

  QString getFraseInicio();
  
  void setFraseFin(QString frase);

  QString getFraseFin();  

  void setFraseTiempo(QString frase);

  QString getFraseTiempo();    
  
  void setPosFin(QVec pFin);

  QVec getPosFin();  
  
  void setRangoError(int rango);
  
  int getRangoError();
  
  void setSeguirObjetivo(bool _seguirObjetivo);
  
  bool getSeguirObjetivo();

  void setNombreObjetivo(QString _nombreObjetivo);

  QString getNombreObjetivo();   
  
  bool finEtapa(const QVec posActual);
};

#endif // ETAPA_H
