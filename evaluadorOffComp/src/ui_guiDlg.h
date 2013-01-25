/********************************************************************************
** Form generated from reading UI file 'guiDlg.ui'
**
** Created: Thu Nov 17 15:07:23 2011
**      by: Qt User Interface Compiler version 4.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUIDLG_H
#define UI_GUIDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QTimeEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_guiDlg
{
public:
    QFrame *frameOSG;
    QLabel *Visor_label;
    QPushButton *getFile_pb;
    QWidget *gridLayoutWidget;
    QGridLayout *PlayerButtons;
    QPushButton *reset_pb;
    QPushButton *setPause_pb;
    QPushButton *setPlay_pb;
    QPushButton *setLoop_pb;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridLayout_3;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *nameOfFile_label;
    QLabel *monthOfSession_label;
    QLabel *dayOfSession_label;
    QLabel *yearOfSession_label;
    QLabel *label_7;
    QLabel *initialTime_label;
    QLabel *label_9;
    QLabel *nameOfPatient_label;
    QLabel *label_11;
    QLabel *doctorName_label;
    QLabel *label_13;
    QLabel *nameOfCenter_label;
    QWidget *angulos_tab;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout;
    QLabel *waist_label;
    QLabel *torso_label;
    QLCDNumber *waist_x_lcd;
    QLCDNumber *waist_y_lcd;
    QLabel *neck_label;
    QLabel *shoulder_r_label;
    QLabel *shoulder_l_label;
    QLabel *knee_r_label;
    QLabel *knee_l_label;
    QLabel *hip_r_label;
    QLabel *hip_l_label;
    QLCDNumber *waist_z_lcd;
    QLCDNumber *torso_x_lcd;
    QLCDNumber *torso_y_lcd;
    QLCDNumber *torso_z_lcd;
    QLCDNumber *neck_x_lcd;
    QLCDNumber *neck_y_lcd;
    QLCDNumber *neck_z_lcd;
    QLCDNumber *shoulder_r_x_lcd;
    QLCDNumber *shoulder_r_y_lcd;
    QLCDNumber *shoulder_r_z_lcd;
    QLCDNumber *shoulder_l_x_lcd;
    QLCDNumber *shoulder_l_y_lcd;
    QLCDNumber *shoulder_l_z_lcd;
    QLCDNumber *knee_r_x_lcd;
    QLCDNumber *knee_r_y_lcd;
    QLCDNumber *knee_r_z_lcd;
    QLCDNumber *knee_l_x_lcd;
    QLCDNumber *knee_l_y_lcd;
    QLCDNumber *knee_l_z_lcd;
    QLCDNumber *hip_r_x_lcd;
    QLCDNumber *hip_r_y_lcd;
    QLCDNumber *hip_r_z_lcd;
    QLCDNumber *hip_l_x_lcd;
    QLCDNumber *hip_l_y_lcd;
    QLCDNumber *hip_l_z_lcd;
    QLabel *elbow_r_label;
    QLCDNumber *elbow_r_x_lcd;
    QLCDNumber *elbow_r_y_lcd;
    QLCDNumber *elbow_r_z_lcd;
    QLabel *elbow_l_label;
    QLCDNumber *elbow_l_x_lcd;
    QLCDNumber *elbow_l_y_lcd;
    QLCDNumber *elbow_l_z_lcd;
    QWidget *Graph;
    QPushButton *showGraph_pb;
    QWidget *layoutWidget;
    QFormLayout *formLayout;
    QPushButton *decreaseSpeed_pb;
    QPushButton *resetSpeed_pb;
    QPushButton *increaseSpeed_pb;
    QLabel *label_6;
    QWidget *gridLayoutWidget_4;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QTimeEdit *timeEdit;
    QGridLayout *gridLayout_4;
    QProgressBar *timeBar;

    void setupUi(QWidget *guiDlg)
    {
        if (guiDlg->objectName().isEmpty())
            guiDlg->setObjectName(QString::fromUtf8("guiDlg"));
        guiDlg->resize(957, 570);
        frameOSG = new QFrame(guiDlg);
        frameOSG->setObjectName(QString::fromUtf8("frameOSG"));
        frameOSG->setGeometry(QRect(10, 50, 640, 480));
        frameOSG->setFrameShape(QFrame::StyledPanel);
        frameOSG->setFrameShadow(QFrame::Raised);
        Visor_label = new QLabel(guiDlg);
        Visor_label->setObjectName(QString::fromUtf8("Visor_label"));
        Visor_label->setGeometry(QRect(10, 20, 641, 17));
        getFile_pb = new QPushButton(guiDlg);
        getFile_pb->setObjectName(QString::fromUtf8("getFile_pb"));
        getFile_pb->setGeometry(QRect(660, 10, 291, 27));
        gridLayoutWidget = new QWidget(guiDlg);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(660, 50, 111, 145));
        PlayerButtons = new QGridLayout(gridLayoutWidget);
        PlayerButtons->setObjectName(QString::fromUtf8("PlayerButtons"));
        PlayerButtons->setContentsMargins(0, 0, 0, 0);
        reset_pb = new QPushButton(gridLayoutWidget);
        reset_pb->setObjectName(QString::fromUtf8("reset_pb"));

        PlayerButtons->addWidget(reset_pb, 2, 0, 1, 1);

        setPause_pb = new QPushButton(gridLayoutWidget);
        setPause_pb->setObjectName(QString::fromUtf8("setPause_pb"));

        PlayerButtons->addWidget(setPause_pb, 1, 0, 1, 1);

        setPlay_pb = new QPushButton(gridLayoutWidget);
        setPlay_pb->setObjectName(QString::fromUtf8("setPlay_pb"));

        PlayerButtons->addWidget(setPlay_pb, 0, 0, 1, 1);

        setLoop_pb = new QPushButton(gridLayoutWidget);
        setLoop_pb->setObjectName(QString::fromUtf8("setLoop_pb"));
        setLoop_pb->setCheckable(true);

        PlayerButtons->addWidget(setLoop_pb, 3, 0, 1, 1);

        tabWidget = new QTabWidget(guiDlg);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(660, 210, 291, 351));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayoutWidget_3 = new QWidget(tab);
        gridLayoutWidget_3->setObjectName(QString::fromUtf8("gridLayoutWidget_3"));
        gridLayoutWidget_3->setGeometry(QRect(0, 0, 281, 311));
        gridLayout_3 = new QGridLayout(gridLayoutWidget_3);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(gridLayoutWidget_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_3->addWidget(label_2, 0, 0, 1, 1);

        label_3 = new QLabel(gridLayoutWidget_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_3->addWidget(label_3, 2, 0, 1, 1);

        label_4 = new QLabel(gridLayoutWidget_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_3->addWidget(label_4, 3, 0, 1, 1);

        label_5 = new QLabel(gridLayoutWidget_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_3->addWidget(label_5, 4, 0, 1, 1);

        nameOfFile_label = new QLabel(gridLayoutWidget_3);
        nameOfFile_label->setObjectName(QString::fromUtf8("nameOfFile_label"));

        gridLayout_3->addWidget(nameOfFile_label, 0, 1, 1, 1);

        monthOfSession_label = new QLabel(gridLayoutWidget_3);
        monthOfSession_label->setObjectName(QString::fromUtf8("monthOfSession_label"));

        gridLayout_3->addWidget(monthOfSession_label, 3, 1, 1, 1);

        dayOfSession_label = new QLabel(gridLayoutWidget_3);
        dayOfSession_label->setObjectName(QString::fromUtf8("dayOfSession_label"));

        gridLayout_3->addWidget(dayOfSession_label, 2, 1, 1, 1);

        yearOfSession_label = new QLabel(gridLayoutWidget_3);
        yearOfSession_label->setObjectName(QString::fromUtf8("yearOfSession_label"));

        gridLayout_3->addWidget(yearOfSession_label, 4, 1, 1, 1);

        label_7 = new QLabel(gridLayoutWidget_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_3->addWidget(label_7, 5, 0, 1, 1);

        initialTime_label = new QLabel(gridLayoutWidget_3);
        initialTime_label->setObjectName(QString::fromUtf8("initialTime_label"));

        gridLayout_3->addWidget(initialTime_label, 5, 1, 1, 1);

        label_9 = new QLabel(gridLayoutWidget_3);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_3->addWidget(label_9, 1, 0, 1, 1);

        nameOfPatient_label = new QLabel(gridLayoutWidget_3);
        nameOfPatient_label->setObjectName(QString::fromUtf8("nameOfPatient_label"));

        gridLayout_3->addWidget(nameOfPatient_label, 1, 1, 1, 1);

        label_11 = new QLabel(gridLayoutWidget_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_3->addWidget(label_11, 6, 0, 1, 1);

        doctorName_label = new QLabel(gridLayoutWidget_3);
        doctorName_label->setObjectName(QString::fromUtf8("doctorName_label"));

        gridLayout_3->addWidget(doctorName_label, 6, 1, 1, 1);

        label_13 = new QLabel(gridLayoutWidget_3);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_3->addWidget(label_13, 7, 0, 1, 1);

        nameOfCenter_label = new QLabel(gridLayoutWidget_3);
        nameOfCenter_label->setObjectName(QString::fromUtf8("nameOfCenter_label"));

        gridLayout_3->addWidget(nameOfCenter_label, 7, 1, 1, 1);

        tabWidget->addTab(tab, QString());
        angulos_tab = new QWidget();
        angulos_tab->setObjectName(QString::fromUtf8("angulos_tab"));
        gridLayoutWidget_2 = new QWidget(angulos_tab);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(0, 0, 281, 315));
        gridLayout = new QGridLayout(gridLayoutWidget_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        waist_label = new QLabel(gridLayoutWidget_2);
        waist_label->setObjectName(QString::fromUtf8("waist_label"));
        waist_label->setEnabled(false);

        gridLayout->addWidget(waist_label, 0, 0, 1, 1);

        torso_label = new QLabel(gridLayoutWidget_2);
        torso_label->setObjectName(QString::fromUtf8("torso_label"));
        torso_label->setEnabled(false);

        gridLayout->addWidget(torso_label, 1, 0, 1, 1);

        waist_x_lcd = new QLCDNumber(gridLayoutWidget_2);
        waist_x_lcd->setObjectName(QString::fromUtf8("waist_x_lcd"));
        waist_x_lcd->setEnabled(false);
        waist_x_lcd->setFrameShape(QFrame::Box);
        waist_x_lcd->setFrameShadow(QFrame::Plain);
        waist_x_lcd->setLineWidth(1);
        waist_x_lcd->setMidLineWidth(0);
        waist_x_lcd->setSmallDecimalPoint(true);
        waist_x_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(waist_x_lcd, 0, 1, 1, 1);

        waist_y_lcd = new QLCDNumber(gridLayoutWidget_2);
        waist_y_lcd->setObjectName(QString::fromUtf8("waist_y_lcd"));
        waist_y_lcd->setEnabled(false);
        waist_y_lcd->setFrameShape(QFrame::Box);
        waist_y_lcd->setFrameShadow(QFrame::Plain);
        waist_y_lcd->setLineWidth(1);
        waist_y_lcd->setMidLineWidth(0);
        waist_y_lcd->setSmallDecimalPoint(true);
        waist_y_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(waist_y_lcd, 0, 2, 1, 1);

        neck_label = new QLabel(gridLayoutWidget_2);
        neck_label->setObjectName(QString::fromUtf8("neck_label"));
        neck_label->setEnabled(false);

        gridLayout->addWidget(neck_label, 2, 0, 1, 1);

        shoulder_r_label = new QLabel(gridLayoutWidget_2);
        shoulder_r_label->setObjectName(QString::fromUtf8("shoulder_r_label"));

        gridLayout->addWidget(shoulder_r_label, 3, 0, 1, 1);

        shoulder_l_label = new QLabel(gridLayoutWidget_2);
        shoulder_l_label->setObjectName(QString::fromUtf8("shoulder_l_label"));

        gridLayout->addWidget(shoulder_l_label, 4, 0, 1, 1);

        knee_r_label = new QLabel(gridLayoutWidget_2);
        knee_r_label->setObjectName(QString::fromUtf8("knee_r_label"));
        knee_r_label->setEnabled(false);

        gridLayout->addWidget(knee_r_label, 7, 0, 1, 1);

        knee_l_label = new QLabel(gridLayoutWidget_2);
        knee_l_label->setObjectName(QString::fromUtf8("knee_l_label"));
        knee_l_label->setEnabled(false);

        gridLayout->addWidget(knee_l_label, 8, 0, 1, 1);

        hip_r_label = new QLabel(gridLayoutWidget_2);
        hip_r_label->setObjectName(QString::fromUtf8("hip_r_label"));
        hip_r_label->setEnabled(false);

        gridLayout->addWidget(hip_r_label, 9, 0, 1, 1);

        hip_l_label = new QLabel(gridLayoutWidget_2);
        hip_l_label->setObjectName(QString::fromUtf8("hip_l_label"));
        hip_l_label->setEnabled(false);

        gridLayout->addWidget(hip_l_label, 10, 0, 1, 1);

        waist_z_lcd = new QLCDNumber(gridLayoutWidget_2);
        waist_z_lcd->setObjectName(QString::fromUtf8("waist_z_lcd"));
        waist_z_lcd->setEnabled(false);
        waist_z_lcd->setFrameShape(QFrame::Box);
        waist_z_lcd->setFrameShadow(QFrame::Plain);
        waist_z_lcd->setLineWidth(1);
        waist_z_lcd->setMidLineWidth(0);
        waist_z_lcd->setSmallDecimalPoint(true);
        waist_z_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(waist_z_lcd, 0, 3, 1, 1);

        torso_x_lcd = new QLCDNumber(gridLayoutWidget_2);
        torso_x_lcd->setObjectName(QString::fromUtf8("torso_x_lcd"));
        torso_x_lcd->setEnabled(false);
        torso_x_lcd->setFrameShape(QFrame::Box);
        torso_x_lcd->setFrameShadow(QFrame::Plain);
        torso_x_lcd->setLineWidth(1);
        torso_x_lcd->setMidLineWidth(0);
        torso_x_lcd->setSmallDecimalPoint(true);
        torso_x_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(torso_x_lcd, 1, 1, 1, 1);

        torso_y_lcd = new QLCDNumber(gridLayoutWidget_2);
        torso_y_lcd->setObjectName(QString::fromUtf8("torso_y_lcd"));
        torso_y_lcd->setEnabled(false);
        torso_y_lcd->setFrameShape(QFrame::Box);
        torso_y_lcd->setFrameShadow(QFrame::Plain);
        torso_y_lcd->setLineWidth(1);
        torso_y_lcd->setMidLineWidth(0);
        torso_y_lcd->setSmallDecimalPoint(true);
        torso_y_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(torso_y_lcd, 1, 2, 1, 1);

        torso_z_lcd = new QLCDNumber(gridLayoutWidget_2);
        torso_z_lcd->setObjectName(QString::fromUtf8("torso_z_lcd"));
        torso_z_lcd->setEnabled(false);
        torso_z_lcd->setFrameShape(QFrame::Box);
        torso_z_lcd->setFrameShadow(QFrame::Plain);
        torso_z_lcd->setLineWidth(1);
        torso_z_lcd->setMidLineWidth(0);
        torso_z_lcd->setSmallDecimalPoint(true);
        torso_z_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(torso_z_lcd, 1, 3, 1, 1);

        neck_x_lcd = new QLCDNumber(gridLayoutWidget_2);
        neck_x_lcd->setObjectName(QString::fromUtf8("neck_x_lcd"));
        neck_x_lcd->setEnabled(false);
        neck_x_lcd->setFrameShape(QFrame::Box);
        neck_x_lcd->setFrameShadow(QFrame::Plain);
        neck_x_lcd->setLineWidth(1);
        neck_x_lcd->setMidLineWidth(0);
        neck_x_lcd->setSmallDecimalPoint(true);
        neck_x_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(neck_x_lcd, 2, 1, 1, 1);

        neck_y_lcd = new QLCDNumber(gridLayoutWidget_2);
        neck_y_lcd->setObjectName(QString::fromUtf8("neck_y_lcd"));
        neck_y_lcd->setEnabled(false);
        neck_y_lcd->setFrameShape(QFrame::Box);
        neck_y_lcd->setFrameShadow(QFrame::Plain);
        neck_y_lcd->setLineWidth(1);
        neck_y_lcd->setMidLineWidth(0);
        neck_y_lcd->setSmallDecimalPoint(true);
        neck_y_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(neck_y_lcd, 2, 2, 1, 1);

        neck_z_lcd = new QLCDNumber(gridLayoutWidget_2);
        neck_z_lcd->setObjectName(QString::fromUtf8("neck_z_lcd"));
        neck_z_lcd->setEnabled(false);
        neck_z_lcd->setFrameShape(QFrame::Box);
        neck_z_lcd->setFrameShadow(QFrame::Plain);
        neck_z_lcd->setLineWidth(1);
        neck_z_lcd->setMidLineWidth(0);
        neck_z_lcd->setSmallDecimalPoint(true);
        neck_z_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(neck_z_lcd, 2, 3, 1, 1);

        shoulder_r_x_lcd = new QLCDNumber(gridLayoutWidget_2);
        shoulder_r_x_lcd->setObjectName(QString::fromUtf8("shoulder_r_x_lcd"));
        shoulder_r_x_lcd->setFrameShape(QFrame::Box);
        shoulder_r_x_lcd->setFrameShadow(QFrame::Plain);
        shoulder_r_x_lcd->setLineWidth(1);
        shoulder_r_x_lcd->setMidLineWidth(0);
        shoulder_r_x_lcd->setSmallDecimalPoint(true);
        shoulder_r_x_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(shoulder_r_x_lcd, 3, 1, 1, 1);

        shoulder_r_y_lcd = new QLCDNumber(gridLayoutWidget_2);
        shoulder_r_y_lcd->setObjectName(QString::fromUtf8("shoulder_r_y_lcd"));
        shoulder_r_y_lcd->setFrameShape(QFrame::Box);
        shoulder_r_y_lcd->setFrameShadow(QFrame::Plain);
        shoulder_r_y_lcd->setLineWidth(1);
        shoulder_r_y_lcd->setMidLineWidth(0);
        shoulder_r_y_lcd->setSmallDecimalPoint(true);
        shoulder_r_y_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(shoulder_r_y_lcd, 3, 2, 1, 1);

        shoulder_r_z_lcd = new QLCDNumber(gridLayoutWidget_2);
        shoulder_r_z_lcd->setObjectName(QString::fromUtf8("shoulder_r_z_lcd"));
        shoulder_r_z_lcd->setFrameShape(QFrame::Box);
        shoulder_r_z_lcd->setFrameShadow(QFrame::Plain);
        shoulder_r_z_lcd->setLineWidth(1);
        shoulder_r_z_lcd->setMidLineWidth(0);
        shoulder_r_z_lcd->setSmallDecimalPoint(true);
        shoulder_r_z_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(shoulder_r_z_lcd, 3, 3, 1, 1);

        shoulder_l_x_lcd = new QLCDNumber(gridLayoutWidget_2);
        shoulder_l_x_lcd->setObjectName(QString::fromUtf8("shoulder_l_x_lcd"));
        shoulder_l_x_lcd->setFrameShape(QFrame::Box);
        shoulder_l_x_lcd->setFrameShadow(QFrame::Plain);
        shoulder_l_x_lcd->setLineWidth(1);
        shoulder_l_x_lcd->setMidLineWidth(0);
        shoulder_l_x_lcd->setSmallDecimalPoint(true);
        shoulder_l_x_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(shoulder_l_x_lcd, 4, 1, 1, 1);

        shoulder_l_y_lcd = new QLCDNumber(gridLayoutWidget_2);
        shoulder_l_y_lcd->setObjectName(QString::fromUtf8("shoulder_l_y_lcd"));
        shoulder_l_y_lcd->setFrameShape(QFrame::Box);
        shoulder_l_y_lcd->setFrameShadow(QFrame::Plain);
        shoulder_l_y_lcd->setLineWidth(1);
        shoulder_l_y_lcd->setMidLineWidth(0);
        shoulder_l_y_lcd->setSmallDecimalPoint(true);
        shoulder_l_y_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(shoulder_l_y_lcd, 4, 2, 1, 1);

        shoulder_l_z_lcd = new QLCDNumber(gridLayoutWidget_2);
        shoulder_l_z_lcd->setObjectName(QString::fromUtf8("shoulder_l_z_lcd"));
        shoulder_l_z_lcd->setFrameShape(QFrame::Box);
        shoulder_l_z_lcd->setFrameShadow(QFrame::Plain);
        shoulder_l_z_lcd->setLineWidth(1);
        shoulder_l_z_lcd->setMidLineWidth(0);
        shoulder_l_z_lcd->setSmallDecimalPoint(true);
        shoulder_l_z_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(shoulder_l_z_lcd, 4, 3, 1, 1);

        knee_r_x_lcd = new QLCDNumber(gridLayoutWidget_2);
        knee_r_x_lcd->setObjectName(QString::fromUtf8("knee_r_x_lcd"));
        knee_r_x_lcd->setEnabled(false);
        knee_r_x_lcd->setFrameShape(QFrame::Box);
        knee_r_x_lcd->setFrameShadow(QFrame::Plain);
        knee_r_x_lcd->setLineWidth(1);
        knee_r_x_lcd->setMidLineWidth(0);
        knee_r_x_lcd->setSmallDecimalPoint(true);
        knee_r_x_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(knee_r_x_lcd, 7, 1, 1, 1);

        knee_r_y_lcd = new QLCDNumber(gridLayoutWidget_2);
        knee_r_y_lcd->setObjectName(QString::fromUtf8("knee_r_y_lcd"));
        knee_r_y_lcd->setEnabled(false);
        knee_r_y_lcd->setFrameShape(QFrame::Box);
        knee_r_y_lcd->setFrameShadow(QFrame::Plain);
        knee_r_y_lcd->setLineWidth(1);
        knee_r_y_lcd->setMidLineWidth(0);
        knee_r_y_lcd->setSmallDecimalPoint(true);
        knee_r_y_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(knee_r_y_lcd, 7, 2, 1, 1);

        knee_r_z_lcd = new QLCDNumber(gridLayoutWidget_2);
        knee_r_z_lcd->setObjectName(QString::fromUtf8("knee_r_z_lcd"));
        knee_r_z_lcd->setEnabled(false);
        knee_r_z_lcd->setFrameShape(QFrame::Box);
        knee_r_z_lcd->setFrameShadow(QFrame::Plain);
        knee_r_z_lcd->setLineWidth(1);
        knee_r_z_lcd->setMidLineWidth(0);
        knee_r_z_lcd->setSmallDecimalPoint(true);
        knee_r_z_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(knee_r_z_lcd, 7, 3, 1, 1);

        knee_l_x_lcd = new QLCDNumber(gridLayoutWidget_2);
        knee_l_x_lcd->setObjectName(QString::fromUtf8("knee_l_x_lcd"));
        knee_l_x_lcd->setEnabled(false);
        knee_l_x_lcd->setFrameShape(QFrame::Box);
        knee_l_x_lcd->setFrameShadow(QFrame::Plain);
        knee_l_x_lcd->setLineWidth(1);
        knee_l_x_lcd->setMidLineWidth(0);
        knee_l_x_lcd->setSmallDecimalPoint(true);
        knee_l_x_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(knee_l_x_lcd, 8, 1, 1, 1);

        knee_l_y_lcd = new QLCDNumber(gridLayoutWidget_2);
        knee_l_y_lcd->setObjectName(QString::fromUtf8("knee_l_y_lcd"));
        knee_l_y_lcd->setEnabled(false);
        knee_l_y_lcd->setFrameShape(QFrame::Box);
        knee_l_y_lcd->setFrameShadow(QFrame::Plain);
        knee_l_y_lcd->setLineWidth(1);
        knee_l_y_lcd->setMidLineWidth(0);
        knee_l_y_lcd->setSmallDecimalPoint(true);
        knee_l_y_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(knee_l_y_lcd, 8, 2, 1, 1);

        knee_l_z_lcd = new QLCDNumber(gridLayoutWidget_2);
        knee_l_z_lcd->setObjectName(QString::fromUtf8("knee_l_z_lcd"));
        knee_l_z_lcd->setEnabled(false);
        knee_l_z_lcd->setFrameShape(QFrame::Box);
        knee_l_z_lcd->setFrameShadow(QFrame::Plain);
        knee_l_z_lcd->setLineWidth(1);
        knee_l_z_lcd->setMidLineWidth(0);
        knee_l_z_lcd->setSmallDecimalPoint(true);
        knee_l_z_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(knee_l_z_lcd, 8, 3, 1, 1);

        hip_r_x_lcd = new QLCDNumber(gridLayoutWidget_2);
        hip_r_x_lcd->setObjectName(QString::fromUtf8("hip_r_x_lcd"));
        hip_r_x_lcd->setEnabled(false);
        hip_r_x_lcd->setFrameShape(QFrame::Box);
        hip_r_x_lcd->setFrameShadow(QFrame::Plain);
        hip_r_x_lcd->setLineWidth(1);
        hip_r_x_lcd->setMidLineWidth(0);
        hip_r_x_lcd->setSmallDecimalPoint(true);
        hip_r_x_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(hip_r_x_lcd, 9, 1, 1, 1);

        hip_r_y_lcd = new QLCDNumber(gridLayoutWidget_2);
        hip_r_y_lcd->setObjectName(QString::fromUtf8("hip_r_y_lcd"));
        hip_r_y_lcd->setEnabled(false);
        hip_r_y_lcd->setFrameShape(QFrame::Box);
        hip_r_y_lcd->setFrameShadow(QFrame::Plain);
        hip_r_y_lcd->setLineWidth(1);
        hip_r_y_lcd->setMidLineWidth(0);
        hip_r_y_lcd->setSmallDecimalPoint(true);
        hip_r_y_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(hip_r_y_lcd, 9, 2, 1, 1);

        hip_r_z_lcd = new QLCDNumber(gridLayoutWidget_2);
        hip_r_z_lcd->setObjectName(QString::fromUtf8("hip_r_z_lcd"));
        hip_r_z_lcd->setEnabled(false);
        hip_r_z_lcd->setFrameShape(QFrame::Box);
        hip_r_z_lcd->setFrameShadow(QFrame::Plain);
        hip_r_z_lcd->setLineWidth(1);
        hip_r_z_lcd->setMidLineWidth(0);
        hip_r_z_lcd->setSmallDecimalPoint(true);
        hip_r_z_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(hip_r_z_lcd, 9, 3, 1, 1);

        hip_l_x_lcd = new QLCDNumber(gridLayoutWidget_2);
        hip_l_x_lcd->setObjectName(QString::fromUtf8("hip_l_x_lcd"));
        hip_l_x_lcd->setEnabled(false);
        hip_l_x_lcd->setFrameShape(QFrame::Box);
        hip_l_x_lcd->setFrameShadow(QFrame::Plain);
        hip_l_x_lcd->setLineWidth(1);
        hip_l_x_lcd->setMidLineWidth(0);
        hip_l_x_lcd->setSmallDecimalPoint(true);
        hip_l_x_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(hip_l_x_lcd, 10, 1, 1, 1);

        hip_l_y_lcd = new QLCDNumber(gridLayoutWidget_2);
        hip_l_y_lcd->setObjectName(QString::fromUtf8("hip_l_y_lcd"));
        hip_l_y_lcd->setEnabled(false);
        hip_l_y_lcd->setFrameShape(QFrame::Box);
        hip_l_y_lcd->setFrameShadow(QFrame::Plain);
        hip_l_y_lcd->setLineWidth(1);
        hip_l_y_lcd->setMidLineWidth(0);
        hip_l_y_lcd->setSmallDecimalPoint(true);
        hip_l_y_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(hip_l_y_lcd, 10, 2, 1, 1);

        hip_l_z_lcd = new QLCDNumber(gridLayoutWidget_2);
        hip_l_z_lcd->setObjectName(QString::fromUtf8("hip_l_z_lcd"));
        hip_l_z_lcd->setEnabled(false);
        hip_l_z_lcd->setFrameShape(QFrame::Box);
        hip_l_z_lcd->setFrameShadow(QFrame::Plain);
        hip_l_z_lcd->setLineWidth(1);
        hip_l_z_lcd->setMidLineWidth(0);
        hip_l_z_lcd->setSmallDecimalPoint(true);
        hip_l_z_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(hip_l_z_lcd, 10, 3, 1, 1);

        elbow_r_label = new QLabel(gridLayoutWidget_2);
        elbow_r_label->setObjectName(QString::fromUtf8("elbow_r_label"));

        gridLayout->addWidget(elbow_r_label, 5, 0, 1, 1);

        elbow_r_x_lcd = new QLCDNumber(gridLayoutWidget_2);
        elbow_r_x_lcd->setObjectName(QString::fromUtf8("elbow_r_x_lcd"));
        elbow_r_x_lcd->setFrameShape(QFrame::Box);
        elbow_r_x_lcd->setFrameShadow(QFrame::Plain);
        elbow_r_x_lcd->setLineWidth(1);
        elbow_r_x_lcd->setMidLineWidth(0);
        elbow_r_x_lcd->setSmallDecimalPoint(true);
        elbow_r_x_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(elbow_r_x_lcd, 5, 1, 1, 1);

        elbow_r_y_lcd = new QLCDNumber(gridLayoutWidget_2);
        elbow_r_y_lcd->setObjectName(QString::fromUtf8("elbow_r_y_lcd"));
        elbow_r_y_lcd->setFrameShape(QFrame::Box);
        elbow_r_y_lcd->setFrameShadow(QFrame::Plain);
        elbow_r_y_lcd->setLineWidth(1);
        elbow_r_y_lcd->setMidLineWidth(0);
        elbow_r_y_lcd->setSmallDecimalPoint(true);
        elbow_r_y_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(elbow_r_y_lcd, 5, 2, 1, 1);

        elbow_r_z_lcd = new QLCDNumber(gridLayoutWidget_2);
        elbow_r_z_lcd->setObjectName(QString::fromUtf8("elbow_r_z_lcd"));
        elbow_r_z_lcd->setFrameShape(QFrame::Box);
        elbow_r_z_lcd->setFrameShadow(QFrame::Plain);
        elbow_r_z_lcd->setLineWidth(1);
        elbow_r_z_lcd->setMidLineWidth(0);
        elbow_r_z_lcd->setSmallDecimalPoint(true);
        elbow_r_z_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(elbow_r_z_lcd, 5, 3, 1, 1);

        elbow_l_label = new QLabel(gridLayoutWidget_2);
        elbow_l_label->setObjectName(QString::fromUtf8("elbow_l_label"));

        gridLayout->addWidget(elbow_l_label, 6, 0, 1, 1);

        elbow_l_x_lcd = new QLCDNumber(gridLayoutWidget_2);
        elbow_l_x_lcd->setObjectName(QString::fromUtf8("elbow_l_x_lcd"));
        elbow_l_x_lcd->setFrameShape(QFrame::Box);
        elbow_l_x_lcd->setFrameShadow(QFrame::Plain);
        elbow_l_x_lcd->setLineWidth(1);
        elbow_l_x_lcd->setMidLineWidth(0);
        elbow_l_x_lcd->setSmallDecimalPoint(true);
        elbow_l_x_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(elbow_l_x_lcd, 6, 1, 1, 1);

        elbow_l_y_lcd = new QLCDNumber(gridLayoutWidget_2);
        elbow_l_y_lcd->setObjectName(QString::fromUtf8("elbow_l_y_lcd"));
        elbow_l_y_lcd->setFrameShape(QFrame::Box);
        elbow_l_y_lcd->setFrameShadow(QFrame::Plain);
        elbow_l_y_lcd->setLineWidth(1);
        elbow_l_y_lcd->setMidLineWidth(0);
        elbow_l_y_lcd->setSmallDecimalPoint(true);
        elbow_l_y_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(elbow_l_y_lcd, 6, 2, 1, 1);

        elbow_l_z_lcd = new QLCDNumber(gridLayoutWidget_2);
        elbow_l_z_lcd->setObjectName(QString::fromUtf8("elbow_l_z_lcd"));
        elbow_l_z_lcd->setFrameShape(QFrame::Box);
        elbow_l_z_lcd->setFrameShadow(QFrame::Plain);
        elbow_l_z_lcd->setLineWidth(1);
        elbow_l_z_lcd->setMidLineWidth(0);
        elbow_l_z_lcd->setSmallDecimalPoint(true);
        elbow_l_z_lcd->setSegmentStyle(QLCDNumber::Flat);

        gridLayout->addWidget(elbow_l_z_lcd, 6, 3, 1, 1);

        tabWidget->addTab(angulos_tab, QString());
        Graph = new QWidget();
        Graph->setObjectName(QString::fromUtf8("Graph"));
        showGraph_pb = new QPushButton(Graph);
        showGraph_pb->setObjectName(QString::fromUtf8("showGraph_pb"));
        showGraph_pb->setGeometry(QRect(10, 10, 271, 61));
        tabWidget->addTab(Graph, QString());
        layoutWidget = new QWidget(guiDlg);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(842, 50, 111, 151));
        formLayout = new QFormLayout(layoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout->setContentsMargins(0, 0, 0, 0);
        decreaseSpeed_pb = new QPushButton(layoutWidget);
        decreaseSpeed_pb->setObjectName(QString::fromUtf8("decreaseSpeed_pb"));

        formLayout->setWidget(2, QFormLayout::FieldRole, decreaseSpeed_pb);

        resetSpeed_pb = new QPushButton(layoutWidget);
        resetSpeed_pb->setObjectName(QString::fromUtf8("resetSpeed_pb"));

        formLayout->setWidget(3, QFormLayout::FieldRole, resetSpeed_pb);

        increaseSpeed_pb = new QPushButton(layoutWidget);
        increaseSpeed_pb->setObjectName(QString::fromUtf8("increaseSpeed_pb"));

        formLayout->setWidget(4, QFormLayout::FieldRole, increaseSpeed_pb);

        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout->setWidget(1, QFormLayout::FieldRole, label_6);

        gridLayoutWidget_4 = new QWidget(guiDlg);
        gridLayoutWidget_4->setObjectName(QString::fromUtf8("gridLayoutWidget_4"));
        gridLayoutWidget_4->setGeometry(QRect(10, 530, 641, 31));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_4);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(gridLayoutWidget_4);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        timeEdit = new QTimeEdit(gridLayoutWidget_4);
        timeEdit->setObjectName(QString::fromUtf8("timeEdit"));
        timeEdit->setReadOnly(true);
        timeEdit->setCalendarPopup(true);

        gridLayout_2->addWidget(timeEdit, 0, 1, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        timeBar = new QProgressBar(gridLayoutWidget_4);
        timeBar->setObjectName(QString::fromUtf8("timeBar"));
        timeBar->setFocusPolicy(Qt::NoFocus);
        timeBar->setValue(0);

        gridLayout_4->addWidget(timeBar, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout_4, 0, 2, 1, 1);


        retranslateUi(guiDlg);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(guiDlg);
    } // setupUi

    void retranslateUi(QWidget *guiDlg)
    {
        guiDlg->setWindowTitle(QApplication::translate("guiDlg", "Revisor de Sesiones de Rehabilitaci\303\263n", 0, QApplication::UnicodeUTF8));
        Visor_label->setText(QApplication::translate("guiDlg", "Visor Osg: ", 0, QApplication::UnicodeUTF8));
        getFile_pb->setText(QApplication::translate("guiDlg", "Abrir Archivo de Sesi\303\263n", 0, QApplication::UnicodeUTF8));
        reset_pb->setText(QApplication::translate("guiDlg", "Stop", 0, QApplication::UnicodeUTF8));
        setPause_pb->setText(QApplication::translate("guiDlg", "Pause", 0, QApplication::UnicodeUTF8));
        setPlay_pb->setText(QApplication::translate("guiDlg", "Play", 0, QApplication::UnicodeUTF8));
        setLoop_pb->setText(QApplication::translate("guiDlg", "Reproducir\n"
" siempre", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("guiDlg", "Nombre del Fichero:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("guiDlg", "D\303\255a:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("guiDlg", "Mes:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("guiDlg", "A\303\261o:", 0, QApplication::UnicodeUTF8));
        nameOfFile_label->setText(QApplication::translate("guiDlg", "nameOfFile", 0, QApplication::UnicodeUTF8));
        monthOfSession_label->setText(QApplication::translate("guiDlg", "monthOfSession", 0, QApplication::UnicodeUTF8));
        dayOfSession_label->setText(QApplication::translate("guiDlg", "dayOfSession", 0, QApplication::UnicodeUTF8));
        yearOfSession_label->setText(QApplication::translate("guiDlg", "yearOfSession", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("guiDlg", "Hora de Inicio:", 0, QApplication::UnicodeUTF8));
        initialTime_label->setText(QApplication::translate("guiDlg", "initialTime", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("guiDlg", "Nombre:", 0, QApplication::UnicodeUTF8));
        nameOfPatient_label->setText(QApplication::translate("guiDlg", "nameOfPatient", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("guiDlg", "M\303\251dico Responsable:", 0, QApplication::UnicodeUTF8));
        doctorName_label->setText(QApplication::translate("guiDlg", "doctorName", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("guiDlg", "Centro de Salud:", 0, QApplication::UnicodeUTF8));
        nameOfCenter_label->setText(QApplication::translate("guiDlg", "centerName", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("guiDlg", "Informaci\303\263n", 0, QApplication::UnicodeUTF8));
        waist_label->setText(QApplication::translate("guiDlg", "Cintura:", 0, QApplication::UnicodeUTF8));
        torso_label->setText(QApplication::translate("guiDlg", "Torso:", 0, QApplication::UnicodeUTF8));
        neck_label->setText(QApplication::translate("guiDlg", "Cuello:", 0, QApplication::UnicodeUTF8));
        shoulder_r_label->setText(QApplication::translate("guiDlg", "Hombro (R):", 0, QApplication::UnicodeUTF8));
        shoulder_l_label->setText(QApplication::translate("guiDlg", "Hombro (L):", 0, QApplication::UnicodeUTF8));
        knee_r_label->setText(QApplication::translate("guiDlg", "Rodilla (R):", 0, QApplication::UnicodeUTF8));
        knee_l_label->setText(QApplication::translate("guiDlg", "Rodilla (L):", 0, QApplication::UnicodeUTF8));
        hip_r_label->setText(QApplication::translate("guiDlg", "Cadera (R):", 0, QApplication::UnicodeUTF8));
        hip_l_label->setText(QApplication::translate("guiDlg", "Cadera (L):", 0, QApplication::UnicodeUTF8));
        elbow_r_label->setText(QApplication::translate("guiDlg", "Codo (R):", 0, QApplication::UnicodeUTF8));
        elbow_l_label->setText(QApplication::translate("guiDlg", "Codo (L):", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(angulos_tab), QApplication::translate("guiDlg", "\303\201ngulos", 0, QApplication::UnicodeUTF8));
        showGraph_pb->setText(QApplication::translate("guiDlg", "Abrir ventana de Gr\303\241ficas", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(Graph), QApplication::translate("guiDlg", "Gr\303\241ficas", 0, QApplication::UnicodeUTF8));
        decreaseSpeed_pb->setText(QApplication::translate("guiDlg", "M\303\241s Lento", 0, QApplication::UnicodeUTF8));
        resetSpeed_pb->setText(QApplication::translate("guiDlg", "Normal", 0, QApplication::UnicodeUTF8));
        increaseSpeed_pb->setText(QApplication::translate("guiDlg", "M\303\241s R\303\241pido", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("guiDlg", "Velocidad:   ", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("guiDlg", "Tiempo:", 0, QApplication::UnicodeUTF8));
        timeEdit->setDisplayFormat(QApplication::translate("guiDlg", "HH:mm:ss", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class guiDlg: public Ui_guiDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUIDLG_H
