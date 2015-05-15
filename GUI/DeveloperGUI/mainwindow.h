#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "../../Structure/Block.h"
#include "../../Structure/Field.h"
#include "../../Structure/Problem.h"
#include "../../Utility/SYNTHESIZE.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    const float field_part_width  = 15;
    const float field_part_height = 15;

protected:
    void keyPressEvent(QKeyEvent * event);
public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Problem* prob;
    Ui::MainWindow *ui;

private slots:
    void AddBlock();
    void EditBlock();
    void EditField();
};

#endif // MAINWINDOW_H
