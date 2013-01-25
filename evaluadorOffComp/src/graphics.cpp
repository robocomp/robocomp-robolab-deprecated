#include "graphics.h"

#define fieldsNumber 4
#define headerFields 9
#define nameJoint 0
#define rx 1
#define ry 2
#define rz 3

graphics::graphics(QString filePath)
{
  setupUi(this);
//   this->waistBox->setEnabled(false);
//   this->torsoBox->setEnabled(false);
//   this->rightshoulderBox->setEnabled(false);
//   this->leftshoulderBox->setEnabled(false);
//   this->rightkneeBox->setEnabled(false);
//   this->leftkneeBox->setEnabled(false);
//   this->righthipBox->setEnabled(false);
//   this->lefthipBox->setEnabled(false);
//   this->rightelbowBox->setEnabled(false);
//   this->leftelbowBox->setEnabled(false);
//   this->neckBox->setEnabled(false);
//   
//   colorWaistx->setEnabled(false);
//   colorWaisty->setEnabled(false);
//   colorWaistz->setEnabled(false);
//   this->colorTorsox->setEnabled(false);
//   this->colorTorsoy->setEnabled(false);
//   this->colorTorsoz->setEnabled(false);
//   this->colorRShoulderx->setEnabled(false);
//   this->colorRShouldery->setEnabled(false);
//   this->colorRShoulderz->setEnabled(false);
//   this->colorLShoulderx->setEnabled(false);
//   this->colorLShouldery->setEnabled(false);
//   this->colorLShoulderz->setEnabled(false);
//   this->colorRKneex->setEnabled(false);
//   this->colorRKneey->setEnabled(false);
//   this->colorRKneez->setEnabled(false);
//   this->colorLKneex->setEnabled(false);
//   this->colorLKneey->setEnabled(false);
//   this->colorLKneez->setEnabled(false);
//   this->colorRHipx->setEnabled(false);
//   this->colorRHipy->setEnabled(false);
//   this->colorRHipz->setEnabled(false);
//   this->colorLHipx->setEnabled(false);
//   this->colorLHipy->setEnabled(false);
//   this->colorLHipz->setEnabled(false);
//   this->colorRElbowx->setEnabled(false);
//   this->colorRElbowy->setEnabled(false);
// //   this->colorRElbowz->setEnabled(false);
//   this->colorLElbowx->setEnabled(false);
//   this->colorLElbowy->setEnabled(false);
// //   this->colorLElbowz->setEnabled(false);
//   this->colorNeckx->setEnabled(false);
//   this->colorNecky->setEnabled(false);
//   this->colorNeckz->setEnabled(false);

	connect(lshoulderx, SIGNAL(clicked(bool)), 		this, SLOT(lsrx_HMarker()));
	connect(lshouldery, SIGNAL(clicked(bool)), 		this, SLOT(lsry_HMarker()));
	connect(lshoulderz, SIGNAL(clicked(bool)), 		this, SLOT(lsrz_HMarker()));
	
	connect(rshoulderx, SIGNAL(clicked(bool)), 		this, SLOT(rsrx_HMarker()));
	connect(rshouldery, SIGNAL(clicked(bool)), 		this, SLOT(rsry_HMarker()));
	connect(rshoulderz, SIGNAL(clicked(bool)), 		this, SLOT(rsrz_HMarker()));
	
	connect(leftelbowrx, SIGNAL(clicked(bool)), 	this, SLOT(lerx_HMarker()));
	connect(leftelbowry, SIGNAL(clicked(bool)), 	this, SLOT(lery_HMarker()));
	
	connect(rightelbowrx, SIGNAL(clicked(bool)), 	this, SLOT(rerx_HMarker()));
	connect(rightelbowry, SIGNAL(clicked(bool)), 	this, SLOT(rery_HMarker()));
	
	connect(lefthiprx, SIGNAL(clicked(bool)), 		this, SLOT(lhrx_HMarker()));
	connect(lefthipry, SIGNAL(clicked(bool)), 		this, SLOT(lhry_HMarker()));
	connect(lefthiprz, SIGNAL(clicked(bool)), 		this, SLOT(lhrz_HMarker()));
	
	connect(righthiprx, SIGNAL(clicked(bool)), 		this, SLOT(rhrx_HMarker()));
	connect(righthipry, SIGNAL(clicked(bool)), 		this, SLOT(rhry_HMarker()));
	connect(righthiprz, SIGNAL(clicked(bool)), 		this, SLOT(rhrz_HMarker()));
	
	connect(lkneerx, SIGNAL(clicked(bool)), 		this, SLOT(lkrx_HMarker()));
	connect(lkneery, SIGNAL(clicked(bool)), 		this, SLOT(lkry_HMarker()));
	connect(lkneerz, SIGNAL(clicked(bool)), 		this, SLOT(lkrz_HMarker()));
	
	connect(rkneerx, SIGNAL(clicked(bool)), 		this, SLOT(rkrx_HMarker()));
	connect(rkneery, SIGNAL(clicked(bool)), 		this, SLOT(rkry_HMarker()));
	connect(rkneerz, SIGNAL(clicked(bool)), 		this, SLOT(rkrz_HMarker()));
	
	connect(torsorx, SIGNAL(clicked(bool)), 		this, SLOT(torsorx_HMarker()));
	connect(torsory, SIGNAL(clicked(bool)), 		this, SLOT(torsory_HMarker()));
	connect(torsorz, SIGNAL(clicked(bool)), 		this, SLOT(torsorz_HMarker()));
	
	connect(waistrx, SIGNAL(clicked(bool)), 		this, SLOT(waistrx_HMarker()));
	connect(waistry, SIGNAL(clicked(bool)), 		this, SLOT(waistry_HMarker()));
	connect(waistrz, SIGNAL(clicked(bool)), 		this, SLOT(waistrz_HMarker()));
	 
	connect(neckrx, SIGNAL(clicked(bool)), 			this, SLOT(nrx_HMarker()));
	connect(neckry, SIGNAL(clicked(bool)), 			this, SLOT(nry_HMarker()));
	connect(neckrz, SIGNAL(clicked(bool)), 			this, SLOT(nrz_HMarker()));
	
	connect(synth_wave_check, 	SIGNAL(clicked(bool)), 	this, SLOT(initSynthWave()));
	connect(synthGain_slider, 	SIGNAL(valueChanged(int)), 	this, SLOT(setSynthGain(int)));
	connect(synthOffset_slider, SIGNAL(valueChanged(int)), 	this, SLOT(setSynthOffset(int)));
	connect(synthFreq_slider, 	SIGNAL(valueChanged(int)), 	this, SLOT(setSynthFreq(int)));
	connect(synthPhase_slider, 	SIGNAL(valueChanged(int)), 	this, SLOT(setSynthPhase(int)));
	
	connect(uncheckAllJoints_pb, SIGNAL(pressed()), 				this, SLOT(disableAllJoints()));
	connect(uncheckPreviousJoints_pb, SIGNAL(pressed()), 			this, SLOT(disablePreviousJoints()));
	
  this->show();
  mX = new QwtPlotMarker();
  mY = new QwtPlotMarker();
  some_zoomer = new QwtPlotZoomer(plot->canvas());
  some_zoomer->setRubberBandPen(QPen(Qt::black, 2, Qt::DotLine));
  some_zoomer->setTrackerPen(QPen(Qt::black));
  
  getDataFromFile(filePath);
}
void graphics::getDataFromFile(QString filePath)
{
  bool headerReaded;
  int fieldsReaded;
  QFile doc(filePath);
  QString line;
  QStringList lineFields;
  QString name;
  float valueX,valueY,valueZ;
  
  if(!doc.open(QIODevice::ReadOnly | QIODevice::Text))
    return;
  
  QTextStream in(&doc);
  synthSize=0;
  waistX.clear();
  waistY.clear();
  waistZ.clear();
  torsoX.clear();
  torsoY.clear();
  torsoZ.clear();
  rightShoulderX.clear();
  rightShoulderY.clear();
  rightShoulderZ.clear();
  leftShoulderX.clear();
  leftShoulderY.clear();
  leftShoulderZ.clear();
  rightKneeX.clear();
  rightKneeY.clear();
  rightKneeZ.clear();
  leftKneeX.clear();
  leftKneeY.clear();
  leftKneeZ.clear();
  rightHipX.clear();
  rightHipY.clear();
  rightHipZ.clear();
  leftHipX.clear();
  leftHipY.clear();
  leftHipZ.clear();
  rightElbowX.clear();
  rightElbowY.clear();
  rightElbowZ.clear();
  leftElbowX.clear();
  leftElbowY.clear();
  leftElbowZ.clear();
  neckX.clear();
  neckY.clear();
  neckZ.clear();
  
  tiempo.clear();
  
  fieldsReaded = 0;
  headerReaded = false;
  
  period = 33;
  patientName = "";  
  
  while(!in.atEnd()){
      line = in.readLine();
      lineFields = line.split(' ');
      
      if(!headerReaded){
		if(lineFields.size() > 1){
		  storeHeaderParam(lineFields);
		  fieldsReaded++;
		  if(fieldsReaded==headerFields)
			headerReaded=true;
		}
      }
      else{
		if(lineFields.size() == fieldsNumber){
			name = lineFields.at(nameJoint);
			valueX = lineFields.at(rx).toFloat();
			valueY = lineFields.at(ry).toFloat();
			valueZ = lineFields.at(rz).toFloat();
			addData(name,valueX,valueY,valueZ);
			synthSize++;
		}
      }
  }
  this->periodLabel->setText("Periodo(ms): "+QString::number(period));
  this->patientLabel->setText("Paciente:		"+patientName);
  this->dateLabel->setText("Fecha: "+dateSesion);
  this->responsibleLabel->setText("Responsable:	"+responsableName);

  ///Periodo en segundos. Hay que leerlo de la cabecera cuando esté preparada.
  int size = waistX.size();
  for(double i=0; i< size; i++){
    tiempo.push_back(period*i);
  }
  plot->setAxisScale(QwtPlot::yLeft, -190.0, 185.0);
  plot->setAxisScale(QwtPlot::xBottom, 0.0, tiempo.last()+1000);
  some_zoomer->setZoomBase();
}
void graphics::addData(QString jointName, float rotationX, float rotationY, float rotationZ)
{
  //Conversion de radianes a grados
  rotationX = (rotationX*360.0)/(2*M_PI);
  rotationY = (rotationY*360.0)/(2*M_PI);
  rotationZ = (rotationZ*360.0)/(2*M_PI);
  
  if(jointName.compare("XN_SKEL_WAIST")==0)
  {
    waistBox->setEnabled(true);
    colorWaistx->setEnabled(true);
    colorWaisty->setEnabled(true);
    colorWaistz->setEnabled(true);
	//jointsNames.insert(jointName);
    waistX.push_back(rotationX);
    waistY.push_back(rotationY);
    waistY.push_back(rotationZ);
  }
  else{
      if(jointName.compare("XN_SKEL_TORSO")==0){
	torsoBox->setEnabled(true);
	colorTorsox->setEnabled(true);
 	colorTorsoy->setEnabled(true);
	colorTorsoz->setEnabled(true);
	torsoX.push_back(rotationX);
	torsoY.push_back(rotationY);
	torsoZ.push_back(rotationZ);
      }
      else{
	  if(jointName.compare("XN_SKEL_RIGHT_SHOULDER")==0){
	    rightshoulderBox->setEnabled(true);
	    colorRShoulderx->setEnabled(true);
	    colorRShouldery->setEnabled(true);
	    colorRShoulderz->setEnabled(true);
	    rightShoulderX.push_back(rotationX);
	    rightShoulderY.push_back(rotationY);
	    rightShoulderZ.push_back(rotationZ);
	  }
	  else{
	      if(jointName.compare("XN_SKEL_LEFT_SHOULDER")==0){
		leftshoulderBox->setEnabled(true);
		colorLShoulderx->setEnabled(true);
		colorLShouldery->setEnabled(true);	
		colorLShoulderz->setEnabled(true);
		leftShoulderX.push_back(rotationX);
		leftShoulderY.push_back(rotationY);
		leftShoulderZ.push_back(rotationZ);
	      }
	      else{
		  if(jointName.compare("XN_SKEL_RIGHT_KNEE")==0){
		    rightkneeBox->setEnabled(true);
		    colorRKneex->setEnabled(true);
		    colorRKneey->setEnabled(true);
		    colorRKneez->setEnabled(true);
		    rightKneeX.push_back(rotationX);
		    rightKneeY.push_back(rotationY);
		    rightKneeZ.push_back(rotationZ);
		  }
		  else{
		      if(jointName.compare("XN_SKEL_LEFT_KNEE")==0){
			leftkneeBox->setEnabled(true);
			colorLKneex->setEnabled(true);
			colorLKneey->setEnabled(true);
			colorLKneez->setEnabled(true);
			leftKneeX.push_back(rotationX);
			leftKneeY.push_back(rotationY);
			leftKneeZ.push_back(rotationZ);
		      }
		      else{
			  if(jointName.compare("XN_SKEL_RIGHT_HIP")==0){
			    righthipBox->setEnabled(true);
			    colorRHipx->setEnabled(true);
			    colorRHipy->setEnabled(true);
			    colorRHipz->setEnabled(true);
			    rightHipX.push_back(rotationX);
			    rightHipY.push_back(rotationY);
			    rightHipZ.push_back(rotationZ);
			  }
			  else{
			      if(jointName.compare("XN_SKEL_LEFT_HIP")==0){
				lefthipBox->setEnabled(true);
				colorLHipx->setEnabled(true);
				colorLHipy->setEnabled(true);
				colorLHipz->setEnabled(true);
				leftHipX.push_back(rotationX);
				leftHipY.push_back(rotationY);
				leftHipZ.push_back(rotationZ);
			      }
			      else{
				  if(jointName.compare("XN_SKEL_RIGHT_ELBOW")==0){
				    rightelbowBox->setEnabled(true);
				    colorRElbowx->setEnabled(true);
				    colorRElbowy->setEnabled(true);
// 				    colorRElbowz->setEnabled(true);
				    rightElbowX.push_back(rotationX);
				    rightElbowY.push_back(rotationY);
				    rightElbowZ.push_back(rotationZ);
				  }
				  else{
				      if(jointName.compare("XN_SKEL_LEFT_ELBOW")==0){
					leftelbowBox->setEnabled(true);
					colorLElbowx->setEnabled(true);
					colorLElbowy->setEnabled(true);
// 					colorLElbowz->setEnabled(true);
					leftElbowX.push_back(rotationX);
					leftElbowY.push_back(rotationY);
					leftElbowZ.push_back(rotationZ);
				      }
				      else{
					  if(jointName.compare("XN_SKEL_NECK")==0){
					    neckBox->setEnabled(true);
					    colorNeckx->setEnabled(true);
					    colorNecky->setEnabled(true);
					    colorNeckz->setEnabled(true);
					    neckX.push_back(rotationX);
					    neckY.push_back(rotationY);
					    neckZ.push_back(rotationZ);    
					  }
					  else
					      qDebug()<<"Articulación no encontrada";
				      }
				  }
			      }
			  }
		      }
		  }
	      }
	  }
      }
  }
  
}
QPen graphics::getColor(int Index)
{
    switch(Index){
      case 0:
	return QPen(Qt::white);
      case 1:
	return QPen(Qt::black);
      case 2:
	return QPen(Qt::red);
      case 3:
	return QPen(Qt::darkRed);
      case 4:
	return QPen(Qt::green);
      case 5:
	return QPen(Qt::darkGreen);
      case 6:
	return QPen(Qt::blue);
      case 7:
	return QPen(Qt::darkBlue);
      case 8:
	return QPen(Qt::cyan);
      case 9:
	return QPen(Qt::darkCyan);
      case 10:
	return QPen(Qt::magenta);
      case 11:
	return QPen(Qt::darkMagenta);
      case 12:
	return QPen(Qt::yellow);
      case 13:
	return QPen(Qt::darkYellow);
      case 14:
	return QPen(Qt::gray);
      case 15:
	return QPen(Qt::darkGray);
      case 16:
	return QPen(Qt::lightGray);
	  default:
	return QPen(Qt::red);
    };
}
void graphics::drawWaist()
{
  if(waistBox->isEnabled())
  {
	if(waistrx->isChecked())
	{
	  waistrxC->setPen(getColor(this->colorWaistx->currentIndex()));
	  waistrxC->setData(tiempo,waistX);
	  waistrxC->attach(this->plot);
	}
	if(waistry->isChecked())
	{
	  waistryC->setPen(getColor(this->colorWaisty->currentIndex()));
	  waistryC->setData(tiempo,waistY);
	  waistryC->attach(this->plot);
	}
	if(waistrz->isChecked())
	{
	  waistrzC->setPen(getColor(this->colorWaistz->currentIndex()));
	  waistrzC->setData(tiempo,waistZ);
	  waistrzC->attach(this->plot);
	}
  }
  this->plot->replot();
}
void graphics::drawTorso()
{
  if(torsoBox->isEnabled()){
      if(torsorx->isChecked()){
	
	TorsorxC->setPen(getColor(this->colorTorsox->currentIndex()));
	TorsorxC->setData(tiempo,torsoX);
	TorsorxC->attach(this->plot);
      }
      if(torsory->isChecked()){
	
	TorsoryC->setPen(getColor(this->colorTorsoy->currentIndex()));
	TorsoryC->setData(tiempo,torsoY);
	TorsoryC->attach(this->plot);
      }
      if(torsorz->isChecked()){
	TorsorzC->setPen(getColor(this->colorTorsoz->currentIndex()));
	TorsorzC->setData(tiempo,torsoZ);
	TorsorzC->attach(this->plot);
      }
  }
  this->plot->replot();
}
void graphics::drawShoulders(int timePosition)
{
  if(rightshoulderBox->isEnabled()){
      if(rshoulderx->isChecked()){
	
	rsrxC->setPen(colorRShoulderx->color());
	rsrxC->setData(tiempo,rightShoulderX);
// 	qDebug()<<"Right Shoulder X: "<<rightShoulderX;
	rsrxC->attach(this->plot);
      }
      if(rshouldery->isChecked()){
	
	rsryC->setPen(colorRShouldery->color());
	rsryC->setData(tiempo,rightShoulderY);
	rsryC->attach(this->plot);
      }
      if(rshoulderz->isChecked()){
	rsrzC->setPen(colorRShoulderz->color());
	rsrzC->setData(tiempo,rightShoulderZ);
	rsrzC->attach(this->plot);
      }
  }
  
  if(leftshoulderBox->isEnabled()){
      if(lshoulderx->isChecked()){
	
	lsrxC->setPen(colorLShoulderx->color());
	lsrxC->setData(tiempo,leftShoulderX);
	lsrxC->attach(this->plot);
      }
      if(lshouldery->isChecked()){
	
	lsryC->setPen(colorLShouldery->color());
	lsryC->setData(tiempo,leftShoulderY);
	lsryC->attach(this->plot);
      }
      if(lshoulderz->isChecked()){
	lsrzC->setPen(colorLShoulderz->color());
	lsrzC->setData(tiempo,leftShoulderZ);
	lsrzC->attach(this->plot);
      }
  }
  
  this->plot->replot();
  
}
void graphics::drawKnees()
{ 
  
  if(rightkneeBox->isEnabled()){
      if(rkneerx->isChecked()){
	
	rkrxC->setPen(colorRKneex->color());
	rkrxC->setData(tiempo,rightKneeX);
	rkrxC->attach(this->plot);
      }
      if(rkneery->isChecked()){
	
	rkryC->setPen(colorRKneey->color());
	rkryC->setData(tiempo,rightKneeY);
	rkryC->attach(this->plot);
      }
      if(rkneery->isChecked()){
	rkrzC->setPen(colorRKneez->color());
	rkrzC->setData(tiempo,rightKneeZ);
	rkrzC->attach(this->plot);
      }
  }
  
  if(leftkneeBox->isEnabled()){
      if(lkneerx->isChecked()){
	
	lkrxC->setPen(colorLKneex->color());
	lkrxC->setData(tiempo,leftKneeX);
	lkrxC->attach(this->plot);
      }
      if(lkneery->isChecked()){
	
	lkryC->setPen(colorLKneey->color());
	lkryC->setData(tiempo,leftKneeY);
	lkryC->attach(this->plot);
      }
      if(lkneerz->isChecked()){
	lkrzC->setPen(colorLKneez->color());
	lkrzC->setData(tiempo,leftKneeZ);
	lkrzC->attach(this->plot);
      }
  }
  
  this->plot->replot();
 
}
void graphics::drawHips()
{
  
  if(righthipBox->isEnabled()){
      if(righthiprx->isChecked()){
	
	rhrxC->setPen(colorRHipx->color());
	rhrxC->setData(tiempo,rightHipX);
	rhrxC->attach(this->plot);
      }
      if(righthipry->isChecked()){
	
	rhryC->setPen(colorRHipy->color());
	rhryC->setData(tiempo,rightHipY);
	rhryC->attach(this->plot);
      }
      if(righthiprz->isChecked()){
	rhrzC->setPen(colorRHipz->color());
	rhrzC->setData(tiempo,rightHipZ);
	rhrzC->attach(this->plot);
      }
  }
  
  if(lefthipBox->isEnabled()){
      if(lefthiprx->isChecked()){
	
	lhrxC->setPen(colorLHipx->color());
	lhrxC->setData(tiempo,leftHipX);
	lhrxC->attach(this->plot);
      }
      if(lefthipry->isChecked()){
	
	lhryC->setPen(colorLHipy->color());
	lhryC->setData(tiempo,leftHipY);
	lhryC->attach(this->plot);
      }
      if(lefthiprz->isChecked()){
	lhrzC->setPen(colorLHipz->color());
	lhrzC->setData(tiempo,leftHipZ);
	lhrzC->attach(this->plot);
      }
  }
  
  this->plot->replot();

}
void graphics::drawElbows()
{
  if(rightelbowBox->isEnabled()){
      if(rightelbowrx->isChecked()){
	
	rerxC->setPen(colorRElbowx->color());
	rerxC->setData(tiempo,rightElbowX);
	rerxC->attach(this->plot);
      }
      if(rightelbowry->isChecked()){
	
	reryC->setPen(colorRElbowy->color());
	reryC->setData(tiempo,rightElbowY);
	reryC->attach(this->plot);
      }
// //       if(rightelbowrz->isChecked()){
// 	rerzC->setPen(getColor(this->colorRElbowz->currentIndex()));
// 	rerzC->setData(tiempo,rightElbowZ);
// 	rerzC->attach(this->plot);
//       }
  }
  
  if(leftelbowBox->isEnabled()){
      if(leftelbowrx->isChecked()){
	
	lerxC->setPen(colorLElbowx->color());
	lerxC->setData(tiempo,leftElbowX);
	lerxC->attach(this->plot);
      }
      if(leftelbowry->isChecked()){
	
	leryC->setPen(colorLElbowy->color());
	leryC->setData(tiempo,leftElbowY);
	leryC->attach(this->plot);
      }
//       if(leftelbowrz->isChecked()){
// 	lerzC->setPen(getColor(this->colorLElbowz->currentIndex()));
// 	lerzC->setData(tiempo,leftElbowZ);
// 	lerzC->attach(this->plot);
//       }
  }
  
  this->plot->replot();

}
void graphics::drawNeck()
{
  if(neckBox->isEnabled()){
      if(neckrx->isChecked()){
	
	nrxC->setPen(colorNeckx->color());
	nrxC->setData(tiempo,neckX);
	nrxC->attach(this->plot);
      }
      if(neckry->isChecked()){
	
	nryC->setPen(colorNecky->color());
	nryC->setData(tiempo,neckY);
	nryC->attach(this->plot);
      }
      if(neckrz->isChecked()){
	nrzC->setPen(colorNeckz->color());
	nrzC->setData(tiempo,neckZ);
	nrzC->attach(this->plot);
      }
  }
  this->plot->replot();
}
void graphics::storeHeaderParam(QStringList headerParam)
{
  if(headerParam.at(0).compare("HEADER_VERSION")==0){
      headerVersion = "";
      for(int i=1;i<headerParam.size();i++){
	headerVersion = headerVersion + headerParam.at(i) + " "; 
      }
  }
  else{
    if(headerParam.at(0).compare("CENTER_NAME")==0){
      centerName = headerParam.at(1);
    }
    else{
      if(headerParam.at(0).compare("RESPONSIBLE_NAME")==0){
	responsableName = "";
	 for(int i=1;i<headerParam.size();i++){
	    responsableName = responsableName + headerParam.at(i);
	    if(i==headerParam.size()-2)
	      responsableName=responsableName+", ";
	    else
	      responsableName=responsableName+" ";
	 }
      }
      else{
	if(headerParam.at(0).compare("PATIENT_NAME")==0){
	  patientName = "";
	  for(int i=1;i<headerParam.size();i++){
	    patientName = patientName + headerParam.at(i);
	    if(i==headerParam.size()-2)
	      patientName = patientName+", ";
	    else
	      patientName = patientName+" ";
	  }
	}
	else{
	  if(headerParam.at(0).compare("TIME_INITIAL")==0){
	    timeInitial = "";
	    for(int i=1;i<headerParam.size();i++){
	      timeInitial = timeInitial + headerParam.at(i) + " "; 
	    }
	  }
	  else{
	    if(headerParam.at(0).compare("TIME_FINAL")==0){
		timeFinal = "";
		for(int i=1;i<headerParam.size();i++){
		  timeFinal = timeFinal + headerParam.at(i) + " "; 
		}
	    }
	    else{
	      if(headerParam.at(0).compare("PERIOD")==0){
		period = headerParam.at(1).toFloat();
	      }
	      else{
		if(headerParam.at(0).compare("JOINTS")==0){
		  joints = "";
		  for(int i=1;i<headerParam.size();i++){
		    joints = joints + headerParam.at(i) + ", "; 
		  }
		}
		else{
		  if(headerParam.at(0).compare("DATE_SESSION")==0){
		    dateSesion = "";
		    for(int i=1;i<headerParam.size();i++){
		      dateSesion = dateSesion + headerParam.at(i);
		      if(i<headerParam.size()-1)
			dateSesion = dateSesion+"/";
		    }
		  }
		  else
		    qDebug()<<"Parametro de cabecera no reconocido por el sistema";
		}
	      }
	    }
	  }
	}
      }
    }
  }
    
}
void graphics::drawMarkers(float mseconds)
{
  int x = (int)mseconds/period;
  if(x>=tiempo.size())
   return;
  
  mX->setLabel(QString::fromUtf8("x(ms) = ")+QString::number(tiempo.at(x)));
  mX->setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
  mX->setLabelOrientation(Qt::Horizontal);
  mX->setLineStyle(QwtPlotMarker::VLine);
  mX->setLinePen(QPen(Qt::black, 0, Qt::DashDotLine));
  mX->setXValue(tiempo.at(x));
  mX->attach(this->plot);
  
  if(leftshoulderBox->isEnabled())
  {
      if(jointHorizontalMarker == "lshoulderx")
	  {
		mY->setLabel(QString::fromUtf8("y(º) = ")+QString::number(leftShoulderX.at(x)));
		mY->setYValue(leftShoulderX.at(x));
      }
      if(jointHorizontalMarker == "lshouldery")
	  {
		mY->setLabel(QString::fromUtf8("y(º) = ")+QString::number(leftShoulderY.at(x)));
		mY->setYValue(leftShoulderY.at(x));
      }
      if(jointHorizontalMarker == "lshoulderz")
	  {
		mY->setLabel(QString::fromUtf8("y(º) = ")+QString::number(leftShoulderZ.at(x)));
		mY->setYValue(leftShoulderZ.at(x));
      }
  }
  if(rightshoulderBox->isEnabled())
  {
      if(jointHorizontalMarker == "rshoulderx")
	  {
		mY->setLabel(QString::fromUtf8("y(º) = ")+QString::number(rightShoulderX.at(x)));
		mY->setYValue(rightShoulderX.at(x));
      }
      if(jointHorizontalMarker == "rshouldery")
	  {
		mY->setLabel(QString::fromUtf8("y(º) = ")+QString::number(rightShoulderY.at(x)));
		mY->setYValue(rightShoulderY.at(x));
      }
      if(jointHorizontalMarker == "rshoulderz")
	  {
		mY->setLabel(QString::fromUtf8("y(º) = ")+QString::number(rightShoulderZ.at(x)));
		mY->setYValue(rightShoulderZ.at(x));
      }
  }
  if(leftelbowBox->isEnabled())
  {
      if(jointHorizontalMarker == "leftelbowrx")
	  {
		mY->setLabel(QString::fromUtf8("y(º) = ")+QString::number(leftElbowX.at(x)));
		mY->setYValue(leftElbowX.at(x));
      }
      if(jointHorizontalMarker == "leftelbowry")
	  {
		mY->setLabel(QString::fromUtf8("y(º) = ")+QString::number(leftElbowY.at(x)));
		mY->setYValue(leftElbowY.at(x));
      }
//       if(leftelbowrz->isChecked())
// 	  {
// 
//       }
  }
  if(rightelbowBox->isEnabled())
  {
      if(jointHorizontalMarker == "rightelbowrx")
	  {

      }
      if(jointHorizontalMarker == "rightelbowry")
	  {

      }
// //       if(rightelbowrz->isChecked())
// // 	  {
// // 
// //       }
  }
  if(waistBox->isEnabled())
  {
      if(jointHorizontalMarker == "waistrx")
	  {

      }
      if(jointHorizontalMarker == "waistry")
	  {

      }
      if(jointHorizontalMarker == "waistrz")
	  {

      }
  }
  if(torsoBox->isEnabled())
  {
      if(jointHorizontalMarker == "torsorx")
	  {

      }
      if(jointHorizontalMarker == "torsory")
	  {

      }
      if(jointHorizontalMarker == "torsorz")
	  {

      }
  }
  if(leftkneeBox->isEnabled())
  {
      if(jointHorizontalMarker == "lkneerx")
	  {

      }
      if(jointHorizontalMarker == "lkneery")
	  {

      }
      if(jointHorizontalMarker == "lkneerz")
	  {

      }
  }
  if(rightkneeBox->isEnabled())
  {
      if(jointHorizontalMarker == "rkneerx")
	  {

      }
      if(jointHorizontalMarker == "rkneery")
	  {

      }
      if(jointHorizontalMarker == "rkneerz")
	  {

      }
  }
  if(lefthipBox->isEnabled())
  {
      if(jointHorizontalMarker == "lefthiprx")
	  {

      }
      if(jointHorizontalMarker == "lefthipry")
	  {

      }
      if(jointHorizontalMarker == "lefthiprz")
	  {

      }
  }
  if(righthipBox->isEnabled())
  {
      if(jointHorizontalMarker == "righthiprx")
	  {

      }
      if(jointHorizontalMarker == "righthipry")
	  {

      }
      if(jointHorizontalMarker == "righthiprz")
	  {

      }
  }
  if(neckBox->isEnabled())
  {
      if(jointHorizontalMarker == "neckrx")
	  {

      }
      if(jointHorizontalMarker == "neckry")
	  {

      }
      if(jointHorizontalMarker == "neckrz")
	  {

      }
  }
  mY->setLabelAlignment(Qt::AlignLeft | Qt::AlignBottom);
  mY->setLabelOrientation(Qt::Horizontal);
  mY->setLineStyle(QwtPlotMarker::HLine);
  mY->setLinePen(QPen(Qt::black, 0, Qt::DashDotLine));
  mY->attach(this->plot);
  
  refreshPlot(); 
}

///SLOTS
// void graphics::openFile()
// {
//   QString filePath = "";
//   filePath = QFileDialog::getOpenFileName(this,tr("Abrir fichero..."),tr("."),tr("Texto (*.txt)"));
//   
//   //Se ha pulsado cancelar
//   if(filePath.compare("")==0)
//     return;
//   
//   this->waistBox->setEnabled(false);
//   this->torsoBox->setEnabled(false);
//   this->rightshoulderBox->setEnabled(false);
//   this->leftshoulderBox->setEnabled(false);
//   this->rightkneeBox->setEnabled(false);
//   this->leftkneeBox->setEnabled(false);
//   this->righthipBox->setEnabled(false);
//   this->lefthipBox->setEnabled(false);
//   this->rightelbowBox->setEnabled(false);
//   this->leftelbowBox->setEnabled(false);
//   this->neckBox->setEnabled(false);
// 
// //   colorWaistx->setEnabled(false);
// //   colorWaisty->setEnabled(false);
// //   colorWaistz->setEnabled(false);
// //   this->colorTorsox->setEnabled(false);
// //   this->colorTorsoy->setEnabled(false);
// //   this->colorTorsoz->setEnabled(false);
// //   this->colorRShoulderx->setEnabled(false);
// //   this->colorRShouldery->setEnabled(false);
// //   this->colorRShoulderz->setEnabled(false);
// //   this->colorLShoulderx->setEnabled(false);
// //   this->colorLShouldery->setEnabled(false);
// //   this->colorLShoulderz->setEnabled(false);
// //   this->colorRKneex->setEnabled(false);
// //   this->colorRKneey->setEnabled(false);
// //   this->colorRKneez->setEnabled(false);
// //   this->colorLKneex->setEnabled(false);
// //   this->colorLKneey->setEnabled(false);
// //   this->colorLKneez->setEnabled(false);
// //   this->colorRHipx->setEnabled(false);
// //   this->colorRHipy->setEnabled(false);
// //   this->colorRHipz->setEnabled(false);
// //   this->colorLHipx->setEnabled(false);
// //   this->colorLHipy->setEnabled(false);
// //   this->colorLHipz->setEnabled(false);
// //   this->colorRElbowx->setEnabled(false);
// //   this->colorRElbowy->setEnabled(false);
// // //   this->colorRElbowz->setEnabled(false);
// //   this->colorLElbowx->setEnabled(false);
// //   this->colorLElbowy->setEnabled(false);
// // //   this->colorLElbowz->setEnabled(false);
// //   this->colorNeckx->setEnabled(false);
// //   this->colorNecky->setEnabled(false);
// //   this->colorNeckz->setEnabled(false);
//   
//   this->getDataFromFile(filePath);
// }
void graphics::refreshPlot()
{
    
  TorsorxC = new QwtPlotCurve("Torso rx");
  TorsoryC = new QwtPlotCurve("Torso ry");
  TorsorzC = new QwtPlotCurve("Torso rz");
  
  waistrxC = new QwtPlotCurve("Waist rx");
  waistryC = new QwtPlotCurve("Waist ry");
  waistrzC = new QwtPlotCurve("Waist rz");
  
  rsrxC = new QwtPlotCurve("Right Shoulder rx");
  rsryC = new QwtPlotCurve("Right Shoulder ry");
  rsrzC = new QwtPlotCurve("Right Shoulder rz");
  
  lsrxC = new QwtPlotCurve("Left Shoulder rx");
  lsryC = new QwtPlotCurve("Left Shoulder ry");
  lsrzC = new QwtPlotCurve("Left Shoulder rz");
  
  rkrxC = new QwtPlotCurve("Right Knee rx");
  rkryC = new QwtPlotCurve("Right Knee ry");
  rkrzC = new QwtPlotCurve("Right Knee rz");
  
  lkrxC = new QwtPlotCurve("Left Knee rx");
  lkryC = new QwtPlotCurve("Left Knee ry");
  lkrzC = new QwtPlotCurve("Left Knee rz");
 
  rhrxC = new QwtPlotCurve("Right Hip rx");
  rhryC = new QwtPlotCurve("Right Hip ry");
  rhrzC = new QwtPlotCurve("Right Hip rz");
  
  lhrxC = new QwtPlotCurve("Left Hip rx");
  lhryC = new QwtPlotCurve("Left Hip ry");
  lhrzC = new QwtPlotCurve("Left Hip rz");

  rerxC = new QwtPlotCurve("Right Elbow rx");
  reryC = new QwtPlotCurve("Right Elbow ry");
  rerzC = new QwtPlotCurve("Right Elbow rz");
  
  lerxC = new QwtPlotCurve("Left Elbow rx");
  leryC = new QwtPlotCurve("Left Elbow ry");
  lerzC = new QwtPlotCurve("Left Elbow rz");
 
  nrxC = new QwtPlotCurve("Neck rx");
  nryC = new QwtPlotCurve("Neck ry");
  nrzC = new QwtPlotCurve("Neck rz");
  
  synthCurve = new QwtPlotCurve("Synthtetic Wave");
   
  drawWaist();
  drawTorso();
  drawShoulders(0);
  drawKnees();
  drawHips();
  drawElbows();
  drawNeck();
  drawSynth();
  
  plot->replot();
  
  delete (waistrxC);
  delete (waistryC);
  delete (waistrzC);
  
  delete (TorsorxC);
  delete (TorsoryC);
  delete (TorsorzC);
  
  delete (rsrxC);
  delete (rsryC);
  delete (rsrzC);
  delete (lsrxC);
  delete (lsryC);
  delete (lsrzC);
  
  delete (rkrxC);
  delete (rkryC);
  delete (rkrzC);
  delete (lkrxC);
  delete (lkryC);
  delete (lkrzC);
  
  delete (rhrxC);
  delete (rhryC);
  delete (rhrzC);
  delete (lhrxC);
  delete (lhryC);
  delete (lhrzC);
  
  delete (rerxC);
  delete (reryC);
  delete (rerzC);
  delete (lerxC);
  delete (leryC);
  delete (lerzC);
  
  delete (nrxC);
  delete (nryC);
  delete (nrzC);
  
  delete (synthCurve);
}
void graphics::drawAction()
{
  drawMarkers(0);
}
void graphics::lerx_HMarker()
{
  jointHorizontalMarker="leftelbowrx";
}
void graphics::lery_HMarker()
{
  jointHorizontalMarker="leftelbowry";
}
void graphics::lerz_HMarker()
{
  jointHorizontalMarker="leftelbowrz";
}
void graphics::lhrx_HMarker()
{
  jointHorizontalMarker="lefthiprx";
}
void graphics::lhry_HMarker()
{
  jointHorizontalMarker="lefthipry";
}
void graphics::lhrz_HMarker()
{
  jointHorizontalMarker="lefthiprz";
}
void graphics::lkrx_HMarker()
{
  jointHorizontalMarker="lkneerx";
}
void graphics::lkry_HMarker()
{
  jointHorizontalMarker="lkneery";
}
void graphics::lkrz_HMarker()
{
  jointHorizontalMarker="lkneerz";

}
void graphics::lsrx_HMarker()
{
  jointHorizontalMarker="lshoulderx";

}
void graphics::lsry_HMarker()
{
  jointHorizontalMarker="lshouldery";

}
void graphics::lsrz_HMarker()
{
  jointHorizontalMarker="lshoulderz";

}
void graphics::nrx_HMarker()
{
  jointHorizontalMarker="neckrx";
}
void graphics::nry_HMarker()
{
  jointHorizontalMarker="neckry";
}
void graphics::nrz_HMarker()
{
  jointHorizontalMarker="neckrz";
}
void graphics::rerx_HMarker()
{
  jointHorizontalMarker="rightelbowrx";
}
void graphics::rery_HMarker()
{
  jointHorizontalMarker="rightelbowry";
}
void graphics::rerz_HMarker()
{
  jointHorizontalMarker="rightelbowrz";
}
void graphics::rhrx_HMarker()
{
  jointHorizontalMarker="righthiprx";
}
void graphics::rhry_HMarker()
{
  jointHorizontalMarker="righthiprx";
}
void graphics::rhrz_HMarker()
{
  jointHorizontalMarker="righthiprz";
}
void graphics::rkrx_HMarker()
{
  jointHorizontalMarker="rkneerx";
}
void graphics::rkry_HMarker()
{
  jointHorizontalMarker="rkneery";
}
void graphics::rkrz_HMarker()
{
  jointHorizontalMarker="rkneerz";
}
void graphics::rsrx_HMarker()
{
  jointHorizontalMarker="rshoulderx";
}
void graphics::rsry_HMarker()
{
  jointHorizontalMarker="rshouldery";
}
void graphics::rsrz_HMarker()
{
  jointHorizontalMarker="rshoulderz";
}
void graphics::torsorx_HMarker()
{
  jointHorizontalMarker="torsorx";
}
void graphics::torsory_HMarker()
{
  jointHorizontalMarker="torsory";
}
void graphics::torsorz_HMarker()
{
  jointHorizontalMarker="torsorz";
}
void graphics::waistrx_HMarker()
{
  jointHorizontalMarker="waistrx";
}
void graphics::waistry_HMarker()
{
  jointHorizontalMarker="waistry";
}
void graphics::waistrz_HMarker()
{
  jointHorizontalMarker="waistrz";
}
void graphics::updateSynth()
{
  synthWave.clear();
  for (int time=0;time<tiempo.size();time++)
  {
	synthWave.push_back(synthGain*sin(2*3.1416*synthFreq*time+synthPhase)+synthOffset);
  }
  refreshPlot();
}
void graphics::setSynthGain(int sliderValue)
{
  synthGain=sliderValue;
  updateSynth();
  synthGain_textedit->setText(QString().setNum(synthGain));
}
void graphics::setSynthOffset(int sliderValue)
{
  synthOffset=sliderValue;
  updateSynth();
  synthOffset_textedit->setText(QString().setNum(synthOffset));
}
void graphics::setSynthFreq(int sliderValue)
{
  synthFreq=sliderValue;
  updateSynth();
  synthFreq_textedit->setText(QString().setNum(synthFreq));
}
void graphics::setSynthPhase(int sliderValue)
{
  synthPhase=(float)sliderValue/1000;
  updateSynth();
  synthPhase_textedit->setText(QString().setNum(((float)sliderValue/1000)*360/6.283, 'f',2));
}
void graphics::initSynthWave()
{
  setSynthGain(90);
  setSynthOffset(0);
  setSynthFreq(6500);
  setSynthPhase(0);
}
void graphics::drawSynth()
{
    if(synth_wave_check->isChecked())
  {
	synthCurve->setPen(synth_wave_color->color());
	synthCurve->setData(tiempo,synthWave);
	synthCurve->attach(this->plot);
  }
}
void graphics::disableAllJoints()
{
	lshoulderx->setCheckState(Qt::Unchecked);
	lshouldery->setCheckState(Qt::Unchecked);
	lshoulderz->setCheckState(Qt::Unchecked);
	rshoulderx->setCheckState(Qt::Unchecked);
	rshouldery->setCheckState(Qt::Unchecked);
	rshoulderz->setCheckState(Qt::Unchecked);
	leftelbowrx->setCheckState(Qt::Unchecked);
	leftelbowry->setCheckState(Qt::Unchecked);
	rightelbowrx->setCheckState(Qt::Unchecked);
	rightelbowry->setCheckState(Qt::Unchecked);
	lefthiprx->setCheckState(Qt::Unchecked);
	lefthipry->setCheckState(Qt::Unchecked);
	lefthiprz->setCheckState(Qt::Unchecked);
	righthiprx->setCheckState(Qt::Unchecked);
	righthipry->setCheckState(Qt::Unchecked);
	righthiprz->setCheckState(Qt::Unchecked);
	lkneerx->setCheckState(Qt::Unchecked);
	lkneery->setCheckState(Qt::Unchecked);
	lkneerz->setCheckState(Qt::Unchecked);
	rkneerx->setCheckState(Qt::Unchecked);
	rkneery->setCheckState(Qt::Unchecked);
	rkneerz->setCheckState(Qt::Unchecked);
	torsorx->setCheckState(Qt::Unchecked);
	torsory->setCheckState(Qt::Unchecked);
	torsorz->setCheckState(Qt::Unchecked);
	waistrx->setCheckState(Qt::Unchecked);
	waistry->setCheckState(Qt::Unchecked);
	waistrz->setCheckState(Qt::Unchecked);
	neckrx->setCheckState(Qt::Unchecked);
	neckry->setCheckState(Qt::Unchecked);
	neckrz->setCheckState(Qt::Unchecked);
}
void graphics::disablePreviousJoints()
{
  disableAllJoints();
      if(jointHorizontalMarker == "lshoulderx")
	  {lshoulderx->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "lshouldery")
	  {lshouldery->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "lshoulderz")
	  {lshoulderz->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "rshoulderx")
	  {rshoulderx->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "rshouldery")
	  {rshouldery->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "rshoulderz")
	  {rshoulderz->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "leftelbowrx")
	  {leftelbowrx->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "leftelbowry")
	  {leftelbowry->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "rightelbowrx")
	  {rightelbowrx->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "rightelbowry")
	  {rightelbowry->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "waistrx")
	  {waistrx->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "waistry")
	  {waistry->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "waistrz")
	  {waistrz->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "torsorx")
	  {torsorx->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "torsory")
	  {torsory->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "torsorz")
	  {torsorz->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "lkneerx")
	  {lkneerx->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "lkneery")
	  {lkneery->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "lkneerz")
	  {lkneerz->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "rkneerx")
	  {rkneerx->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "rkneery")
	  {rkneery->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "rkneerz")
	  {rkneerz->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "lefthiprx")
	  {lefthiprx->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "lefthipry")
	  {lefthipry->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "lefthiprz")
	  {lefthiprz->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "righthiprx")
	  {righthiprx->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "righthipry")
	  {righthipry->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "righthiprz")
	  {righthipry->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "neckrx")
	  {neckrx->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "neckry")
	  {neckry->setCheckState(Qt::Checked);
      }
      if(jointHorizontalMarker == "neckrz")
	  {neckrz->setCheckState(Qt::Checked);
      }
}
graphics::~graphics()
{
  delete (mX);
  delete (mY);
}

