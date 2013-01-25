/********************************************************************************
** Form generated from reading UI file 'guiDlg.ui'
**
** Created: Tue Sep 27 17:09:25 2011
**      by: Qt User Interface Compiler version 4.7.0
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
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_guiDlg
{
public:
    QFrame *frame;

    void setupUi(QWidget *guiDlg)
    {
        if (guiDlg->objectName().isEmpty())
            guiDlg->setObjectName(QString::fromUtf8("guiDlg"));
        guiDlg->resize(667, 506);
        frame = new QFrame(guiDlg);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(10, 10, 640, 480));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        retranslateUi(guiDlg);

        QMetaObject::connectSlotsByName(guiDlg);
    } // setupUi

    void retranslateUi(QWidget *guiDlg)
    {
        guiDlg->setWindowTitle(QApplication::translate("guiDlg", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class guiDlg: public Ui_guiDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUIDLG_H
