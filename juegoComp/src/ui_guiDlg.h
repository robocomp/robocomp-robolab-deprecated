/********************************************************************************
** Form generated from reading UI file 'guiDlg.ui'
**
** Created: Tue Nov 29 17:01:33 2011
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
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QStackedWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <phonon/videoplayer.h>

QT_BEGIN_NAMESPACE

class Ui_guiDlg
{
public:
    QVBoxLayout *verticalLayout_3;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QVBoxLayout *verticalLayout;
    QFrame *frameOSG;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_2;
    Phonon::VideoPlayer *videoPlayer;

    void setupUi(QWidget *guiDlg)
    {
        if (guiDlg->objectName().isEmpty())
            guiDlg->setObjectName(QString::fromUtf8("guiDlg"));
        guiDlg->resize(1024, 768);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(guiDlg->sizePolicy().hasHeightForWidth());
        guiDlg->setSizePolicy(sizePolicy);
        verticalLayout_3 = new QVBoxLayout(guiDlg);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        stackedWidget = new QStackedWidget(guiDlg);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        sizePolicy.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy);
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        sizePolicy.setHeightForWidth(page->sizePolicy().hasHeightForWidth());
        page->setSizePolicy(sizePolicy);
        page->setMinimumSize(QSize(800, 600));
        verticalLayout = new QVBoxLayout(page);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frameOSG = new QFrame(page);
        frameOSG->setObjectName(QString::fromUtf8("frameOSG"));
        sizePolicy.setHeightForWidth(frameOSG->sizePolicy().hasHeightForWidth());
        frameOSG->setSizePolicy(sizePolicy);
        frameOSG->setMinimumSize(QSize(800, 600));
        frameOSG->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        frameOSG->setFrameShape(QFrame::StyledPanel);
        frameOSG->setFrameShadow(QFrame::Raised);

        verticalLayout->addWidget(frameOSG);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        sizePolicy.setHeightForWidth(page_2->sizePolicy().hasHeightForWidth());
        page_2->setSizePolicy(sizePolicy);
        page_2->setMinimumSize(QSize(800, 600));
        verticalLayout_2 = new QVBoxLayout(page_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        videoPlayer = new Phonon::VideoPlayer(page_2);
        videoPlayer->setObjectName(QString::fromUtf8("videoPlayer"));
        sizePolicy.setHeightForWidth(videoPlayer->sizePolicy().hasHeightForWidth());
        videoPlayer->setSizePolicy(sizePolicy);
        videoPlayer->setMinimumSize(QSize(800, 600));

        verticalLayout_2->addWidget(videoPlayer);

        stackedWidget->addWidget(page_2);

        verticalLayout_3->addWidget(stackedWidget);


        retranslateUi(guiDlg);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(guiDlg);
    } // setupUi

    void retranslateUi(QWidget *guiDlg)
    {
        guiDlg->setWindowTitle(QApplication::translate("guiDlg", "Juego Kinect y Augmented Reality", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class guiDlg: public Ui_guiDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUIDLG_H
