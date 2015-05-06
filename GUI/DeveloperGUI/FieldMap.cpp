#include "FieldMap.h"
#include <QPainter>
#include <QDebug>
#include <QIcon>


void FieldMap::mousePressEvent(QMouseEvent* event){
    QPoint select_point(event->x() / field_part_width,event->y() / field_part_height);
    qDebug() << select_point;

    //持ちコマの変更
    for(int i=0;i<blocks.size();i++){
        QPoint length(select_point - blocks[i].pos);
        //範囲外
        if(length.x() < 0 || length.x() > BLOCK_WIDTH ||
           length.y() < 0 || length.y() > BLOCK_HEIGHT )continue;
        //選択
        if(blocks[i].block[length.y()][length.x()] == Constants::FILL){
            index = i;
            break;
        }
    }

}

void FieldMap::paintEvent(QPaintEvent* event){
    //フィールドの描画
    QPixmap pixmap(field_part_width * FIELD_WIDTH,field_part_height * FIELD_HEIGHT );
    QPainter painter(&pixmap);

    Field draw_field = field;
    for(auto block : blocks)draw_field.Projection(block.pos,block.block);
    for(int i=0;i<FIELD_HEIGHT;i++){
        for(int j=0;j<FIELD_WIDTH;j++){
            QImage image;
            QRect r(j*field_part_width, i*field_part_height,field_part_width, field_part_height);
            if(draw_field[i][j]==Constants::FILL)painter.fillRect(r,QBrush(QColor::fromRgb(100,100,100)));
            else                                 painter.fillRect(r,QBrush(QColor::fromRgb(255,255,255)));
            painter.drawImage(j * field_part_width,i * field_part_height,image);
        }
    }
    painter.setPen(QPen(QColor::fromRgb(0,0,0), 1));
    for(int i=0;i<FIELD_HEIGHT+1;i++)painter.drawLine(0,i*field_part_height,field_part_width * FIELD_WIDTH,i*field_part_height);
    for(int j=0;j<FIELD_WIDTH +1;j++)painter.drawLine(j*field_part_width,0,j*field_part_width,field_part_height * FIELD_HEIGHT);

    //選択中のブロックの描画
    if(index >= 0){
        painter.setPen(QPen(QColor::fromRgb(255,0,0), 2));
        painter.drawRect(QRect(blocks[index].pos.x()*field_part_width,
                               blocks[index].pos.y()*field_part_height,
                               8*field_part_width,
                               8*field_part_height));
    }
    this->setPixmap(pixmap);
    QLabel::paintEvent(event);
}

QIcon FieldMap::MakeIcon(Block block){
    QPixmap pix(41,41);
    QPainter painter(&pix);
    const int block_part_width = 5;
    const int block_part_height = 5;

    for(int i=0;i<BLOCK_HEIGHT;i++){
        for(int j=0;j<BLOCK_WIDTH;j++){
            QImage image;
            QRect r(j*block_part_width, i*block_part_height,block_part_width, block_part_height);
            if(block[i][j]==Constants::FILL)painter.fillRect(r,QBrush(QColor::fromRgb(150,150,150)));
            else                            painter.fillRect(r,QBrush(QColor::fromRgb(255,255,255)));
            painter.drawImage(j * block_part_width,i * block_part_height,image);
        }
    }
    painter.setPen(QPen(QColor::fromRgb(0,0,0), 1));
    for(int i=0;i<BLOCK_HEIGHT+1;i++)painter.drawLine(0,i*block_part_height,block_part_width * FIELD_WIDTH,i*block_part_height);
    for(int j=0;j<BLOCK_WIDTH +1;j++)painter.drawLine(j*block_part_width,0,j*block_part_width,block_part_height * FIELD_HEIGHT);

    return QIcon(pix);
}
void FieldMap::AddBlock(QPoint pos,Block block){
    blocks.append(BlockParam{block,pos,false});
    listing->addItem(new QListWidgetItem(QString("(") + QString::number(pos.x()) + "," + QString::number(pos.y()) + ")"));
    listing->item(listing->count()-1)->setIcon(MakeIcon(block));
}

QPoint FieldMap::GetPoint(int index)const{
    if(index >= 0){
        return blocks[index].pos;
    }
    return QPoint(0,0);
}

FieldMap::FieldMap(QWidget *parent):
    QLabel(parent)
{
    listing = new QListWidget();
    listing->setIconSize(QSize(41,41));
    listing->setSelectionMode(QAbstractItemView::NoSelection);
    index = 0;
    this->setMaximumSize(field_part_width * FIELD_WIDTH+2,field_part_height * FIELD_HEIGHT+2);
}

FieldMap::~FieldMap()
{

}

void FieldMap::SetRotate(Constants::ANGLE angle){
    if(index >= 0){
        blocks[index].block = blocks[index].block.GetRotate(angle);
        listing->item(index)->setIcon(MakeIcon(blocks[index].block));
    }
}

void FieldMap::SetPoint (QPoint pos){
    const int around_size = 8;
    if(index >= 0){
        pos.setX(std::min(std::max(-around_size + 1,pos.x()),FIELD_WIDTH -1));
        pos.setY(std::min(std::max(-around_size + 1,pos.y()),FIELD_HEIGHT-1));
        blocks[index].pos = pos;
        listing->item(index)->setText(QString("(") + QString::number(pos.x()) + "," + QString::number(pos.y()) + ")");
    }
}
void FieldMap::Reverse(){
    if(index >= 0){
        blocks[index].block = blocks[index].block.GetReverse();
    }
}


