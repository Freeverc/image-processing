/********************************************************************************
** Form generated from reading UI file 'ImageSegmentationUI.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGESEGMENTATIONUI_H
#define UI_IMAGESEGMENTATIONUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageSegmentationUIClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ImageSegmentationUIClass)
    {
        if (ImageSegmentationUIClass->objectName().isEmpty())
            ImageSegmentationUIClass->setObjectName(QString::fromUtf8("ImageSegmentationUIClass"));
        ImageSegmentationUIClass->resize(600, 400);
        menuBar = new QMenuBar(ImageSegmentationUIClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        ImageSegmentationUIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ImageSegmentationUIClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        ImageSegmentationUIClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(ImageSegmentationUIClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        ImageSegmentationUIClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ImageSegmentationUIClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ImageSegmentationUIClass->setStatusBar(statusBar);

        retranslateUi(ImageSegmentationUIClass);

        QMetaObject::connectSlotsByName(ImageSegmentationUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *ImageSegmentationUIClass)
    {
        ImageSegmentationUIClass->setWindowTitle(QApplication::translate("ImageSegmentationUIClass", "ImageSegmentationUI", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ImageSegmentationUIClass: public Ui_ImageSegmentationUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGESEGMENTATIONUI_H
