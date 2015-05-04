#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include "../../Structure/Block.h"
#include "../../Structure/Field.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    const float field_part_width  = 15;
    const float field_part_height = 15;
private:
    Field field;
    QList<std::pair<QPoint,Block>> blocks;

protected:
    void paintEvent(QPaintEvent* event);
public:
    static QPixmap MakeIcon(QColor col,int width,int height);

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
