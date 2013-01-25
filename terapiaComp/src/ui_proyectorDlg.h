/********************************************************************************
** Form generated from reading UI file 'proyectorDlg.ui'
**
** Created: Thu Jan 13 11:43:58 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROYECTORDLG_H
#define UI_PROYECTORDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_proyectorDlg
{
public:
    QGridLayout *gridLayout;
    QWidget *widget;

    void setupUi(QDialog *proyectorDlg)
    {
        if (proyectorDlg->objectName().isEmpty())
            proyectorDlg->setObjectName(QString::fromUtf8("proyectorDlg"));
        proyectorDlg->resize(400, 300);
        gridLayout = new QGridLayout(proyectorDlg);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        widget = new QWidget(proyectorDlg);
        widget->setObjectName(QString::fromUtf8("widget"));

        gridLayout->addWidget(widget, 0, 0, 1, 1);


        retranslateUi(proyectorDlg);

        QMetaObject::connectSlotsByName(proyectorDlg);
    } // setupUi

    void retranslateUi(QDialog *proyectorDlg)
    {
        proyectorDlg->setWindowTitle(QApplication::translate("proyectorDlg", "Dialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class proyectorDlg: public Ui_proyectorDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROYECTORDLG_H
