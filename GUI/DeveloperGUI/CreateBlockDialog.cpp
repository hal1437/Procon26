#include "CreateBlockDialog.h"
#include "ui_CreateBlockDialog_UI.h"
#include <QPainter>
#include <QDebug>

void CreateBlockDialog::paintEvent(QPaintEvent* event){    
    //フィールドの描画
    QPixmap pixmap(block_part_width * BLOCK_WIDTH+1,block_part_height * BLOCK_HEIGHT+1);
    QPainter painter(&pixmap);

    for(int i=0;i<BLOCK_HEIGHT;i++){
        for(int j=0;j<BLOCK_WIDTH;j++){
            QImage image;
            QRect r(j*block_part_width, i*block_part_height,block_part_width, block_part_height);
            if(block[i][j]==Constants::FILL)painter.fillRect(r,QBrush(QColor::fromRgb(100,100,100)));
            else                            painter.fillRect(r,QBrush(QColor::fromRgb(255,255,255)));
            painter.drawImage(j * block_part_width,i * block_part_height,image);
        }
    }
    painter.setPen(QPen(QColor::fromRgb(0,0,0), 1));
    for(int i=0;i<FIELD_HEIGHT+1;i++)painter.drawLine(0,i*block_part_height,block_part_width * FIELD_WIDTH,i*block_part_height);
    for(int j=0;j<FIELD_WIDTH +1;j++)painter.drawLine(j*block_part_width,0,j*block_part_width,block_part_height * FIELD_HEIGHT);

    ui->FieldLabel->setPixmap(pixmap);
    QDialog::paintEvent(event);
}

void CreateBlockDialog::mousePressEvent(QMouseEvent* event){
    if(event->button() == Qt::LeftButton )LeftButtonClicking  = true;
    if(event->button() == Qt::RightButton)RightButtonClicking = true;
    mouseMoveEvent(event);
}
void CreateBlockDialog::mouseReleaseEvent(QMouseEvent* event){
    if(event->button() == Qt::LeftButton )LeftButtonClicking  = false;
    if(event->button() == Qt::RightButton)RightButtonClicking = false;
    mouseMoveEvent(event);
}
void CreateBlockDialog::mouseMoveEvent(QMouseEvent* event){
    QPoint selectpoint(event->pos().x() / block_part_width,event->pos().y() / block_part_height);
    if(selectpoint.x() < 0 || selectpoint.x() >= BLOCK_WIDTH ||
       selectpoint.y() < 0 || selectpoint.y() >= BLOCK_HEIGHT )return;
    if(LeftButtonClicking){
        block[selectpoint.y()][selectpoint.x()] = Constants::FILL;
    }
    if(RightButtonClicking){
        block[selectpoint.y()][selectpoint.x()] = Constants::EMPTY;
    }
}

CreateBlockDialog::CreateBlockDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateBlockDialog)
{
    LeftButtonClicking  = false;
    RightButtonClicking = false;
    ui->setupUi(this);
    setMouseTracking(true);
    ui->FieldLabel->resize(ui->FieldLabel->sizeHint());
}

CreateBlockDialog::~CreateBlockDialog()
{
    delete ui;
}
