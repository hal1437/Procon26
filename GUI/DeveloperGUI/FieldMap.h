#ifndef FIELDMAP_H
#define FIELDMAP_H

#include <QLabel>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QListWidget>
#include "../../Structure/Block.h"
#include "../../Structure/Field.h"
#include "../../Utility/SYNTHESIZE.hpp"

class FieldMap : public QLabel
{
public:
    //ブロック状態型
    struct BlockParam{
        Block block;
        QPoint pos;
        bool Using;
    };

private:
    const int field_part_width = 15;
    const int field_part_height = 15;

    SYNTHESIZE(Field       ,field  ,Field);
    SYNTHESIZE(int         ,index  ,Index);
    SYNTHESIZE(QListWidget*,listing,Listing);
    QList<BlockParam> blocks;

protected:
    void mousePressEvent(QMouseEvent* event);
    void paintEvent     (QPaintEvent* event);

public:
    static QIcon MakeIcon(Block block);
    void AddBlock(QPoint pos,Block block);

    QPoint GetPoint(int index)const;

    FieldMap(QWidget *parent = 0);
    ~FieldMap();

public slots:
    void SetRotate(Constants::ANGLE angle); //回転
    void SetPoint (QPoint pos);             //移動
    void Reverse  ();                       //反転


};

#endif // FIELDMAP_H
