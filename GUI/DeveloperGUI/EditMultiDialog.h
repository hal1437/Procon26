#ifndef EDITMULTIDIALOG_H
#define EDITMULTIDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include "../../Structure/Block.h"
#include "../../Structure/Field.h"
#include "../../Utility/SYNTHESIZE.hpp"

template<size_t MATRIX_WIDTH,size_t MATRIX_HEIGHT>
class EditMultiDialog : public QDialog
{
private:
    typedef Multi<Constants::ZUKU,MATRIX_WIDTH,MATRIX_HEIGHT> matrix;
    const int block_part_width  = 30;
    const int block_part_height = 30;

    QVBoxLayout* layout;
    QLabel* label;
    QDialogButtonBox * button;
    bool LeftButtonClicking;
    bool RightButtonClicking;
    QPoint traking_point;
    SYNTHESIZE(matrix,mat,Matrix);

protected:
    void paintEvent(QPaintEvent* event){
        //フィールドの描画
        QPixmap pixmap(block_part_width * MATRIX_WIDTH+1,block_part_height * MATRIX_HEIGHT+1);
        QPainter painter(&pixmap);

        for(int i=0;i<MATRIX_HEIGHT;i++){
            for(int j=0;j<MATRIX_WIDTH;j++){
                QImage image;
                QRect r(j*block_part_width, i*block_part_height,block_part_width, block_part_height);
                if(mat[i][j]==Constants::FILL)painter.fillRect(r,QBrush(QColor::fromRgb(100,100,100)));
                else                            painter.fillRect(r,QBrush(QColor::fromRgb(255,255,255)));
                painter.drawImage(j * block_part_width,i * block_part_height,image);
            }
        }
        painter.setPen(QPen(QColor::fromRgb(0,0,0), 1));
        for(int i=0;i<MATRIX_HEIGHT+1;i++)painter.drawLine(0,i*block_part_height,block_part_width * FIELD_WIDTH,i*block_part_height);
        for(int j=0;j<FIELD_WIDTH +1;j++)painter.drawLine(j*block_part_width,0,j*block_part_width,block_part_height * FIELD_HEIGHT);

        label->setPixmap(pixmap);
        QDialog::paintEvent(event);
    }
    void mousePressEvent(QMouseEvent* event){
        if(event->button() == Qt::LeftButton )LeftButtonClicking  = true;
        if(event->button() == Qt::RightButton)RightButtonClicking = true;
        mouseMoveEvent(event);
    }
    void mouseReleaseEvent(QMouseEvent* event){
        if(event->button() == Qt::LeftButton )LeftButtonClicking  = false;
        if(event->button() == Qt::RightButton)RightButtonClicking = false;
        mouseMoveEvent(event);
    }
    void mouseMoveEvent(QMouseEvent* event){
        QPoint selectpoint(event->pos().x() / block_part_width,event->pos().y() / block_part_height);
        if(selectpoint.x() < 0 || selectpoint.x() >= MATRIX_WIDTH ||
           selectpoint.y() < 0 || selectpoint.y() >= MATRIX_HEIGHT )return;
        if(LeftButtonClicking){
            mat[selectpoint.y()][selectpoint.x()] = Constants::FILL;
        }
        if(RightButtonClicking){
            mat[selectpoint.y()][selectpoint.x()] = Constants::EMPTY;
        }
    }

public:
    EditMultiDialog(QWidget *parent=0) :
        QDialog(parent),
        block_part_width (500 / MATRIX_WIDTH),
        block_part_height(500 / MATRIX_HEIGHT)
    {
        LeftButtonClicking  = false;
        RightButtonClicking = false;

        layout = new QVBoxLayout(this);
        label  = new QLabel(this);
        button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,this);
        layout->addWidget(label);
        layout->addWidget(button);
        layout->setContentsMargins(0,0,0,0);
        setLayout(layout);

        connect(button,SIGNAL(accepted()),this,SLOT(accept()));
        connect(button,SIGNAL(rejected()),this,SLOT(reject()));
        setMouseTracking(true);
        label->resize(block_part_width * MATRIX_WIDTH+1,block_part_height * MATRIX_HEIGHT+1);
    }
    ~EditMultiDialog(){}

};

#endif // EDITMULTIDIALOG_H
