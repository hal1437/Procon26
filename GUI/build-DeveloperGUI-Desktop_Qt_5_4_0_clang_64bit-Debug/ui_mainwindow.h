/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "FieldMap.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionBlockEdit;
    QAction *actionAddBlock;
    QAction *actionFieldEdit;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QFrame *Field;
    QVBoxLayout *verticalLayout;
    FieldMap *FieldLabel;
    QMenuBar *menuBar;
    QMenu *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(547, 413);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        actionBlockEdit = new QAction(MainWindow);
        actionBlockEdit->setObjectName(QStringLiteral("actionBlockEdit"));
        actionAddBlock = new QAction(MainWindow);
        actionAddBlock->setObjectName(QStringLiteral("actionAddBlock"));
        actionFieldEdit = new QAction(MainWindow);
        actionFieldEdit->setObjectName(QStringLiteral("actionFieldEdit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        Field = new QFrame(centralWidget);
        Field->setObjectName(QStringLiteral("Field"));
        Field->setFrameShape(QFrame::StyledPanel);
        Field->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(Field);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        FieldLabel = new FieldMap(Field);
        FieldLabel->setObjectName(QStringLiteral("FieldLabel"));
        FieldLabel->setScaledContents(false);

        verticalLayout->addWidget(FieldLabel);


        horizontalLayout->addWidget(Field);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 547, 22));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menu->addAction(actionBlockEdit);
        mainToolBar->addAction(actionAddBlock);
        mainToolBar->addAction(actionBlockEdit);
        mainToolBar->addAction(actionFieldEdit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionBlockEdit->setText(QApplication::translate("MainWindow", "\343\203\226\343\203\255\343\203\203\343\202\257\347\267\250\351\233\206", 0));
#ifndef QT_NO_TOOLTIP
        actionBlockEdit->setToolTip(QApplication::translate("MainWindow", "BlockEdit", 0));
#endif // QT_NO_TOOLTIP
        actionBlockEdit->setShortcut(QApplication::translate("MainWindow", "Ctrl+B", 0));
        actionAddBlock->setText(QApplication::translate("MainWindow", "\343\203\226\343\203\255\343\203\203\343\202\257\350\277\275\345\212\240", 0));
        actionAddBlock->setShortcut(QApplication::translate("MainWindow", "Ctrl+A", 0));
        actionFieldEdit->setText(QApplication::translate("MainWindow", "\343\203\225\343\202\243\343\203\274\343\203\253\343\203\211\347\267\250\351\233\206", 0));
        actionFieldEdit->setShortcut(QApplication::translate("MainWindow", "Ctrl+F", 0));
        FieldLabel->setText(QString());
        menu->setTitle(QApplication::translate("MainWindow", "\347\267\250\351\233\206", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
