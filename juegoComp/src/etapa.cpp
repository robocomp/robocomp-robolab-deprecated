#include "etapa.h"

Etapa::Etapa()
{
  fraseInicio = "";
  fraseFin = "";
  fraseTiempo = "";
  posFin = QVec::vec3(0.,0.,0.);
  rangoError = 0;
  tiempoDisparoFrase = 0;
  nombreObjetivo = "";
  seguirObjetivo = false;
}

Etapa::Etapa(QString _id, QString _fraseInicio, QString _fraseFin, QString _fraseTiempo, QVec _posFin, int _rangoError, int _tiempoDisparoFrase, QString _nombreObjetivo, bool _seguirObjetivo)
{
  fraseInicio = _fraseInicio;
  fraseFin = _fraseFin;
  fraseTiempo = _fraseTiempo;
  posFin = _posFin;
  rangoError = _rangoError;
  tiempoDisparoFrase = _tiempoDisparoFrase;
  nombreObjetivo = _nombreObjetivo;
  seguirObjetivo = _seguirObjetivo;
  id =_id;
}

Etapa::~Etapa()
{

}

QString Etapa::getId()
{
	return id;
}
void Etapa::setId(QString name)
{
	id=name;
}

 
void Etapa::setFraseInicio(QString frase)
{
  fraseInicio = frase;
}

QString Etapa::getFraseInicio()
{
  return fraseInicio;
}

void Etapa::setFraseFin(QString frase)
{
  fraseFin = frase;
}
 
QString Etapa::getFraseFin()
{
  return fraseFin;
}

void Etapa::setFraseTiempo(QString frase)
{
  fraseTiempo = frase;
}
 
QString Etapa::getFraseTiempo()
{
  return fraseTiempo;
}

void Etapa::setPosFin(QVec pFin)
{
  posFin = pFin;
}

QVec Etapa::getPosFin()
{
  return posFin;
}

void Etapa::setRangoError(int rango)
{
  rangoError = rango;
}

int Etapa::getRangoError()
{
  return rangoError;
}

void Etapa::setSeguirObjetivo(bool _seguirObjetivo)
{
  seguirObjetivo = _seguirObjetivo;
}
  
bool Etapa::getSeguirObjetivo()
{
  return seguirObjetivo;
}

void Etapa::setNombreObjetivo(QString _nombreObjetivo)
{
  nombreObjetivo = _nombreObjetivo;
}

QString Etapa::getNombreObjetivo()
{
  return nombreObjetivo;
}


bool Etapa::finEtapa(const QVec posActual)
{   
	
	QVec aux = posActual - posFin;
// 	posActual.print("posActual mano");
// 	posFin.print("posFin de la etapa");
// 	aux.print("aux");
// 	qDebug()<<"distancia al objetivo"<<aux.norm2();
	if ( aux.norm2() > rangoError)
		return false;
	else
		return true;
	
}