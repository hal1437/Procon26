/********************************************************************************
** Form generated from reading UI file 'CreateBlockDialog_UI.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEBLOCKDIALOG_UI_H
#define UI_CREATEBLOCKDIALOG_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CreateBlockDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QLabel *FieldLabel;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CreateBlockDialog)
    {
        if (CreateBlockDialog->objectName().isEmpty())
            CreateBlockDialog->setObjectName(QStringLiteral("CreateBlockDialog"));
        CreateBlockDialog->resize(197, 204);
        verticalLayout_2 = new QVBoxLayout(CreateBlockDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 6, 0, 0);
        frame = new QFrame(CreateBlockDialog);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        FieldLabel = new QLabel(frame);
        FieldLabel->setObjectName(QStringLiteral("FieldLabel"));

        verticalLayout->addWidget(FieldLabel);


        verticalLayout_2->addWidget(frame);

        buttonBox = new QDialogButtonBox(CreateBlockDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(CreateBlockDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), CreateBlockDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CreateBlockDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(CreateBlockDialog);
    } // setupUi

    void retranslateUi(QDialog *CreateBlockDialog)
    {
        CreateBlockDialog->setWindowTitle(QApplication::translate("CreateBlockDialog", "Dialog", 0));
        FieldLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CreateBlockDialog: public Ui_CreateBlockDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEBLOCKDIALOG_UI_H
