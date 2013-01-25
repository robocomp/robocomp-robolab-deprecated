#include "sesiondata.h"
#include <ctime>

sesionData::sesionData()
{
  
  clickedSave = false;
  setupUi(this);
  
  connect(saveButton,SIGNAL(clicked(bool)),this,SLOT(saveButtonAction()));
  
  this->show();
}


void sesionData::saveButtonAction()
{
  
  QTime curTime;
  QDate curDate;
  
  curTime = QTime::currentTime();
  curDate = QDate::currentDate();
  
  header = "HEADER_VERSION "+versionLabel->text()+"\nCENTER_NAME "+centerLabel->text()+"\nRESPONSIBLE_NAME "+responsibleName->text()+"\nPATIENT_NAME "+patientLabel->text()
	   +"\nDATE_SESSION "+QString::number(curDate.year())+" "+QString::number(curDate.month())+" "+QString::number(curDate.day())+"\nTIME_INITIAL "+QString::number(curTime.hour())+" "+
	   QString::number(curTime.minute())+" "+QString::number(curTime.second())+"\nTIME_FINAL "+"-- -- --"+"\nPERIOD 33.3"+"\nJOINTS waist torso right-shoulder right-knee right-hip right-elbow neck left-shoulder left-knee left-hip left-elbow\n\n";

  header =header.toUtf8();
  emit done();
}

QString sesionData::getHeader()
{
  return header;
}

sesionData::~sesionData()
{
 
}

