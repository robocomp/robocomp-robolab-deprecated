/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef CAMERADLGCONTROL_H
#define CAMERADLGCONTROL_H

#include <QtGui>
#include <qworld/qworld.h>
#include <Camera.h>


class CameraDlgControl : public QWidget
{
  Q_OBJECT
  private:
	RoboCompCamera::CameraPrx camera_prx;
	RoboCompCamera::TCamParams camParams;
	RoboCompCamera::imgType imgV;
	RoboCompDifferentialRobot::TBaseState bState;
	RoboCompCommonHead::THeadState hState;
	QImage *imgI, *imgD;
	QFrame *frameI,*frameD;
	int imgSize;
	QRadioButton *rbOnoff;
  public:
	
	qWorld *qvisorI, *qvisorD;
    
  public:
	CameraDlgControl(RoboCompCamera::CameraPrx _camera_prx);
	~CameraDlgControl();

	void showMessage(const QString &message);
  public slots:
	void update();

  signals:
	void outputMessage(QString message);
};

#endif
