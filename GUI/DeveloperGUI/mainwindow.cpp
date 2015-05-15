#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>
#include <QDir>
#include "FieldMap.h"
#include "EditMultiDialog.h"

void MainWindow::keyPressEvent(QKeyEvent * event){
    FieldMap* map = ui->FieldLabel;
    if(event->key() == Qt::Key_Up)   map->SetPoint(map->GetPoint(map->getIndex())+QPoint( 0,-1));
    if(event->key() == Qt::Key_Right)map->SetPoint(map->GetPoint(map->getIndex())+QPoint( 1, 0));
    if(event->key() == Qt::Key_Down) map->SetPoint(map->GetPoint(map->getIndex())+QPoint( 0, 1));
    if(event->key() == Qt::Key_Left) map->SetPoint(map->GetPoint(map->getIndex())+QPoint(-1, 0));
    if(event->key() == Qt::Key_Shift)map->SetRotate(Constants::ANGLE90);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->layout()->addWidget(ui->FieldLabel->getListing());
    ui->Field     ->setMaximumSize(field_part_width * FIELD_WIDTH+2,field_part_height * FIELD_HEIGHT+2);
    ui->FieldLabel->setMaximumSize(field_part_width * FIELD_WIDTH+2,field_part_height * FIELD_HEIGHT+2);
    //Users/hal1437/Programs/Procon26/Problem/Problem.txt
    prob = new Problem("");

    ui->FieldLabel->setField(prob->GetField());
    for(int i=0;i<prob->Count();i++)ui->FieldLabel->AddBlock(QPoint(0,0),prob->GetBlock(i));

    connect(ui->actionAddBlock ,SIGNAL(triggered()),this,SLOT(AddBlock()));
    connect(ui->actionBlockEdit,SIGNAL(triggered()),this,SLOT(EditBlock()));
    connect(ui->actionFieldEdit,SIGNAL(triggered()),this,SLOT(EditField()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddBlock(){
    EditMultiDialog<8,8> diag;
    diag.setMatrix(Block());
    if(diag.exec()){
        ui->FieldLabel->AddBlock(QPoint(0,0),diag.getMatrix());
    }
}
void MainWindow::EditBlock(){
    if(this->ui->FieldLabel->getIndex() >= 0){
        EditMultiDialog<BLOCK_WIDTH,BLOCK_HEIGHT> diag;
        diag.setMatrix(ui->FieldLabel->GetSelecting() );
        if(diag.exec()){
            ui->FieldLabel->GetSelecting() = diag.getMatrix();
        }
    }
}
void MainWindow::EditField(){
    EditMultiDialog<FIELD_WIDTH,FIELD_HEIGHT> diag;
    diag.setMatrix(ui->FieldLabel->getField());
    if(diag.exec()){
        ui->FieldLabel->setField(diag.getMatrix());
    }
}
