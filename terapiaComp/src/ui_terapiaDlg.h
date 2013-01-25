/********************************************************************************
** Form generated from reading UI file 'terapiaDlg.ui'
**
** Created: Mon Jan 17 10:37:16 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEVILLADLG_H
#define UI_SEVILLADLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>
#include <phonon/seekslider.h>
#include <phonon/videoplayer.h>
#include <phonon/volumeslider.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *speechPB;
    QTextEdit *textEdit;
    QListWidget *phrasesLW;
    Phonon::VideoPlayer *videoPlayer;
    QPushButton *playPB;
    QPushButton *stopPB;
    Phonon::SeekSlider *seekSlider;
    Phonon::VolumeSlider *volumeSlider;
    QListWidget *videosLW;
    QLabel *label;
    QLabel *label_2;
    QPushButton *openVidPB;
    QPushButton *openTextPB;
    QPushButton *switchPB;
    QListWidget *juegosLW;
    QLabel *label_4;
    QFrame *frame;
    QLabel *label_3;
    QFrame *fPanel;
    QRadioButton *rbHola;
    QRadioButton *rbAdios;
    QPushButton *pbRec;
    QLCDNumber *lcdHombro;
    QLCDNumber *lcdCodo;
    QLCDNumber *lcdPulso;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1024, 768);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(1024, 768));
        MainWindow->setMaximumSize(QSize(1024, 768));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        speechPB = new QPushButton(centralwidget);
        speechPB->setObjectName(QString::fromUtf8("speechPB"));
        speechPB->setGeometry(QRect(910, 470, 90, 27));
        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(20, 400, 981, 61));
        phrasesLW = new QListWidget(centralwidget);
        phrasesLW->setObjectName(QString::fromUtf8("phrasesLW"));
        phrasesLW->setGeometry(QRect(760, 40, 251, 331));
        videoPlayer = new Phonon::VideoPlayer(centralwidget);
        videoPlayer->setObjectName(QString::fromUtf8("videoPlayer"));
        videoPlayer->setGeometry(QRect(290, 40, 441, 291));
        playPB = new QPushButton(centralwidget);
        playPB->setObjectName(QString::fromUtf8("playPB"));
        playPB->setGeometry(QRect(310, 340, 51, 27));
        stopPB = new QPushButton(centralwidget);
        stopPB->setObjectName(QString::fromUtf8("stopPB"));
        stopPB->setGeometry(QRect(670, 340, 61, 27));
        seekSlider = new Phonon::SeekSlider(centralwidget);
        seekSlider->setObjectName(QString::fromUtf8("seekSlider"));
        seekSlider->setEnabled(false);
        seekSlider->setGeometry(QRect(400, 340, 241, 25));
        seekSlider->setProperty("iconVisible", QVariant(true));
        seekSlider->setProperty("tracking", QVariant(true));
        volumeSlider = new Phonon::VolumeSlider(centralwidget);
        volumeSlider->setObjectName(QString::fromUtf8("volumeSlider"));
        volumeSlider->setGeometry(QRect(260, 60, 21, 261));
        videosLW = new QListWidget(centralwidget);
        videosLW->setObjectName(QString::fromUtf8("videosLW"));
        videosLW->setGeometry(QRect(20, 40, 231, 331));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 59, 17));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(760, 10, 59, 17));
        label_2->setFont(font);
        openVidPB = new QPushButton(centralwidget);
        openVidPB->setObjectName(QString::fromUtf8("openVidPB"));
        openVidPB->setGeometry(QRect(20, 370, 51, 27));
        openTextPB = new QPushButton(centralwidget);
        openTextPB->setObjectName(QString::fromUtf8("openTextPB"));
        openTextPB->setGeometry(QRect(760, 370, 51, 27));
        switchPB = new QPushButton(centralwidget);
        switchPB->setObjectName(QString::fromUtf8("switchPB"));
        switchPB->setGeometry(QRect(440, 370, 121, 27));
        switchPB->setCheckable(false);
        juegosLW = new QListWidget(centralwidget);
        new QListWidgetItem(juegosLW);
        new QListWidgetItem(juegosLW);
        new QListWidgetItem(juegosLW);
        juegosLW->setObjectName(QString::fromUtf8("juegosLW"));
        juegosLW->setGeometry(QRect(20, 500, 231, 221));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 470, 60, 17));
        label_4->setFont(font);
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(360, 480, 320, 240));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(700, 480, 211, 17));
        label_3->setFont(font);
        fPanel = new QFrame(centralwidget);
        fPanel->setObjectName(QString::fromUtf8("fPanel"));
        fPanel->setGeometry(QRect(700, 500, 301, 221));
        fPanel->setFrameShape(QFrame::StyledPanel);
        fPanel->setFrameShadow(QFrame::Raised);
        rbHola = new QRadioButton(fPanel);
        rbHola->setObjectName(QString::fromUtf8("rbHola"));
        rbHola->setGeometry(QRect(30, 30, 106, 22));
        rbAdios = new QRadioButton(fPanel);
        rbAdios->setObjectName(QString::fromUtf8("rbAdios"));
        rbAdios->setGeometry(QRect(30, 60, 61, 22));
        pbRec = new QPushButton(centralwidget);
        pbRec->setObjectName(QString::fromUtf8("pbRec"));
        pbRec->setGeometry(QRect(300, 610, 51, 27));
        lcdHombro = new QLCDNumber(centralwidget);
        lcdHombro->setObjectName(QString::fromUtf8("lcdHombro"));
        lcdHombro->setGeometry(QRect(310, 510, 41, 23));
        lcdHombro->setAutoFillBackground(false);
        lcdHombro->setSegmentStyle(QLCDNumber::Flat);
        lcdCodo = new QLCDNumber(centralwidget);
        lcdCodo->setObjectName(QString::fromUtf8("lcdCodo"));
        lcdCodo->setGeometry(QRect(310, 540, 41, 23));
        lcdPulso = new QLCDNumber(centralwidget);
        lcdPulso->setObjectName(QString::fromUtf8("lcdPulso"));
        lcdPulso->setGeometry(QRect(310, 570, 41, 23));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(260, 510, 51, 19));
        QFont font1;
        font1.setFamily(QString::fromUtf8("AlArabiya"));
        font1.setPointSize(10);
        label_5->setFont(font1);
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(260, 540, 41, 19));
        label_6->setFont(font1);
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(260, 570, 41, 20));
        label_7->setFont(font1);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1024, 29));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(stopPB, SIGNAL(clicked()), videoPlayer, SLOT(stop()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        speechPB->setText(QApplication::translate("MainWindow", "Speech", 0, QApplication::UnicodeUTF8));
        playPB->setText(QString());
        stopPB->setText(QString());
        label->setText(QApplication::translate("MainWindow", "Videos", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Phrases", 0, QApplication::UnicodeUTF8));
        openVidPB->setText(QString());
        openTextPB->setText(QString());
        switchPB->setText(QApplication::translate("MainWindow", "Change to", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = juegosLW->isSortingEnabled();
        juegosLW->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = juegosLW->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("MainWindow", "Bienvenida / Despedida.", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem1 = juegosLW->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("MainWindow", "Flexi\303\263n hombro.", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem2 = juegosLW->item(2);
        ___qlistwidgetitem2->setText(QApplication::translate("MainWindow", "Flexi\303\263n y extensi\303\263n de codo.", 0, QApplication::UnicodeUTF8));
        juegosLW->setSortingEnabled(__sortingEnabled);

        label_4->setText(QApplication::translate("MainWindow", "Juegos", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Panel de Control.", 0, QApplication::UnicodeUTF8));
        rbHola->setText(QApplication::translate("MainWindow", "Hola", 0, QApplication::UnicodeUTF8));
        rbAdios->setText(QApplication::translate("MainWindow", "Adi\303\263s", 0, QApplication::UnicodeUTF8));
        pbRec->setText(QApplication::translate("MainWindow", "Rec", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Hombro", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Codo", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Pulso", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEVILLADLG_H
