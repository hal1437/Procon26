#ifndef CREATEBLOCKDIALOG_H
#define CREATEBLOCKDIALOG_H

#include <QDialog>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include "../../Structure/Block.h"
#include "../../Structure/Field.h"
#include "../../Utility/SYNTHESIZE.hpp"

namespace Ui {
class CreateBlockDialog;
}

class CreateBlockDialog : public QDialog
{
    Q_OBJECT
    const int block_part_width  = 30;
    const int block_part_height = 30;
private:
    bool LeftButtonClicking;
    bool RightButtonClicking;
    QPoint traking_point;
    SYNTHESIZE(Block,block,Block);

protected:
    void paintEvent       (QPaintEvent* event);
    void mousePressEvent  (QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent   (QMouseEvent* event);
public:

    explicit CreateBlockDialog(QWidget *parent = 0);
    ~CreateBlockDialog();

private:
    Ui::CreateBlockDialog *ui;
};

#endif // CREATEBLOCKDIALOG_H
