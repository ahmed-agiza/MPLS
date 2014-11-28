#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), simulator(0), editor(new CodeEditor(this)), simulationRunning(false), isFileModified(false){
    ui->setupUi(this);
    ui->twdCode->setCurrentIndex(0);
    ui->tabCodeEditor->layout()->addWidget(editor);
    QObject::connect(editor, SIGNAL(textChanged()), this, SLOT(fileModified()));

    installEventFilter(this);
}

bool MainWindow::saveActiveFile(){
    if (openFilePath.trimmed() == "")
        return saveActiveFileAs();
    QFile file(openFilePath);
    if (file.open(QFile::ReadWrite | QFile::Text | QFile::Truncate)){
        QTextStream saveStream(&file);
        QStringList content = editor->toPlainText().split("\n");
        for (int i = 0; i < content.size(); i++){
            saveStream << content.at(i);
            if (i < content.size() - 1)
                saveStream << "\n";
        }
        file.close();
        isFileModified = false;
        return true;
    }else{
        return saveActiveFileAs();
    }
}

bool MainWindow::saveActiveFileAs(){
    QString fileName = QFileDialog::getSaveFileName(this, "Save File As..", QDir::currentPath(), "*.asm");
    if (fileName.trimmed() == "")
        return false;
    QFile file(fileName);
    if (file.open(QFile::ReadWrite | QFile::Text | QFile::Truncate)){
        QTextStream saveStream(&file);
        QStringList content = editor->toPlainText().split("\n");
        for (int i = 0; i < content.size(); i++){
            saveStream << content.at(i);
            if (i < content.size() - 1)
                saveStream << "\n";
        }
        file.close();
        isFileModified = false;
        ui->twdCode->setTabText(0, "Code Editor - " + QFileInfo(file).fileName());
        openFilePath = fileName;
        return true;
    }else
        return false;


}


bool MainWindow::eventFilter(QObject *o, QEvent *e){
    return QMainWindow::eventFilter(o, e);
}

void MainWindow::closeEvent(QCloseEvent *e){
    if (!isFileModified){
        e->accept();
        return;
    }
    QMessageBox::StandardButton response = QMessageBox::question(this, "Close", "Do you want to save the modified file before closing?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
    if (response == QMessageBox::Yes){
        if (saveActiveFile()){
            e->accept();
            return;
        }
    }
    if (response == QMessageBox::No){
        e->accept();
        return;
    }
    e->ignore();

}

void MainWindow::on_actionNew_triggered(){
    if (isFileModified){
        QMessageBox::StandardButton response = QMessageBox::question(this, "Open", "Do you want to save the modified file before opening a new one?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
        if (response == QMessageBox::Yes)
            if (!saveActiveFile())
                return;
        if (response == QMessageBox::Cancel)
            return;
    }

    isFileModified = false;
    ui->twdCode->setTabText(0, "Code Editor - Untitled");
    openFilePath = "";
}

void MainWindow::on_actionOpen_triggered(){
    if (isFileModified){
        QMessageBox::StandardButton response = QMessageBox::question(this, "Open", "Do you want to save the modified file before opening a new one?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
        if (response == QMessageBox::Yes)
            if (!saveActiveFile())
                return;
        if (response == QMessageBox::Cancel)
            return;
    }

    QString fileName = QFileDialog::getOpenFileName(this, "Open File", QDir::currentPath(), "*.asm *.txt");
    if (fileName.trimmed() == "")
        return;

    QFile file(fileName);
    if (file.open(QFile::ReadOnly | QFile::Text)){
        editor->setText(file.readAll());
        isFileModified = false;
        qDebug() << ui->tabCodeEditor->windowTitle();
        ui->twdCode->setTabText(0, "Code Editor - " + QFileInfo(file).fileName());
        openFilePath = fileName;
        file.close();
    }else
        QMessageBox::critical(this, "Error", "Cannot open the specified file", QMessageBox::Yes);
}

void MainWindow::on_actionSave_triggered(){
    saveActiveFile();
}

void MainWindow::on_actionSaveAs_triggered(){
    saveActiveFileAs();
}

void MainWindow::on_actionClose_triggered(){
    if (!isFileModified){
        hide();
        exit(0);
    }
    QMessageBox::StandardButton response = QMessageBox::question(this, "Close", "Do you want to save the modified file before closing?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
    if (response == QMessageBox::Yes){
        if (saveActiveFile()){
            hide();
            exit(0);
        }
    }
    if (response == QMessageBox::No){
        hide();
        exit(0);
    }

}

void MainWindow::on_actionAbout_triggered(){

}

void MainWindow::on_actionAssemble_triggered(){
    QStringList instructions = editor->toPlainText().remove(QRegExp("#[^\n]*")).split("\n");
    for(QString &instr : instructions)
        instr = instr.trimmed();
    if (simulator)
        delete simulator;
    simulator = new Simulator(this, instructions);
}

void MainWindow::on_actionStartSimulation_triggered(){
    simulationRunning = true;
    refreshSimulationActions();
    ui->twdCode->setTabText(1, "Assembly - Cycle " + QString::number(simulator->getCurrentCycle()));
}

void MainWindow::on_actionNextCycle_triggered(){
    simulator->nextCycle();
    ui->twdCode->setTabText(1, "Assembly - Cycle " + QString::number(simulator->getCurrentCycle()));
}

void MainWindow::on_actionStopSimulation_triggered(){

}

void MainWindow::refreshSimulationActions(){
    ui->actionNextCycle->setEnabled(simulationRunning);
    ui->actionStopSimulation->setEnabled(simulationRunning);
}

void MainWindow::fileModified(){
    isFileModified = true;
    if(!ui->twdCode->tabText(0).trimmed().endsWith("*"))
        ui->twdCode->setTabText(0, ui->twdCode->tabText(0) + "*");
}

void MainWindow::enableSimulation(){
    ui->actionStartSimulation->setEnabled(!simulationRunning && simulator->isReady());
}

MainWindow::~MainWindow(){
    delete ui;
}
