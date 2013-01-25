#ifndef SESIONDATA_H
#define SESIONDATA_H
#include "ui_sesionDataWindowDlg.h"
#include <QObject>
#include <QtGui>

class sesionData: public QWidget, public Ui_SesionDataWindow 
{
Q_OBJECT
private:
  
  bool clickedSave;
  QString header;
  QMutex mutex;
  
public:
  sesionData();
  
  QString getHeader();
  
  ~sesionData();
  
public slots:
  
  void saveButtonAction();
signals:
  void done();
};

#endif // SESIONDATA_H
