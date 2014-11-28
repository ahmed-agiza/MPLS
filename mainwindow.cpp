#include <QHBoxLayout>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    ui->twdCode->setCurrentIndex(0);
    QHBoxLayout *editorLayout = new QHBoxLayout(this);
    editor = new CodeEditor(this);
    editorLayout->addWidget(editor);
    ui->tabCodeEditor->layout()->addWidget(editor);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_actionNew_triggered(){

}

void MainWindow::on_actionOpen_triggered(){

}

void MainWindow::on_actionSave_triggered(){

}

void MainWindow::on_actionSaveAs_triggered(){

}

void MainWindow::on_actionClose_triggered(){

}

void MainWindow::on_actionAbout_triggered(){

}

void MainWindow::on_actionAssemble_triggered(){

}

void MainWindow::on_actionStartSimulation_triggered(){

}

void MainWindow::on_actionNextCycle_triggered(){

}

void MainWindow::on_actionStopSimulation_triggered(){

}
