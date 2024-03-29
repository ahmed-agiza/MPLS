#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#define TEST_MODE
#undef TEST_MODE

QList<Instruction *> MainWindow::emptyInsList;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), simulator(new Simulator(this)), editor(new CodeEditor(this)), simulationRunning(false), isFileModified(false){
    ui->setupUi(this);
    qDebug() << "";

    ui->twdCode->setCurrentIndex(0);
    ui->tabCodeEditor->layout()->addWidget(editor);
    QObject::connect(editor, SIGNAL(textChanged()), this, SLOT(fileModified()));

    installEventFilter(this);

    ui->tblAssembled->horizontalHeader()->setStretchLastSection(true);
    ui->tblRegisters->horizontalHeader()->setStretchLastSection(true);
    ui->tblMemory->horizontalHeader()->setStretchLastSection(true);

    ui->tblPpl->setColumnCount(0);
    ui->tblPpl->setRowCount(0);
    pplHeader.clear();
    ui->tblPpl->setHorizontalHeaderLabels(pplHeader);

#ifdef TEST_MODE

    //**************Dummy value for testing**************************
    QList<Instruction *> *temp = new QList<Instruction *>;
   /* temp->push_back(new Instruction(this, InstructionName::ADD, 2, 3, 4, 5));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 10, 11, 21, 15));
    temp->push_back(new Instruction(this, InstructionName::BLE, 14, 13, 14, 3));
    temp->push_back(new Instruction(this, InstructionName::J, 5, 6, 7, 4));
    temp->push_back(new Instruction(this, InstructionName::JAL, 1, 3, 7, 5));
    temp->push_back(new Instruction(this, InstructionName::JR, 9, 3, 0, 5));
    temp->push_back(new Instruction(this, InstructionName::LW, 0, 3, 5, 5));
    temp->push_back(new Instruction(this, InstructionName::SW, 6, 3, 4, 8));
    temp->push_back(new Instruction(this, InstructionName::SLT, 1, 3, 4, 5));
 int rs, int rt, int rd, int immediate, ExecState state)*/
   // temp->push_back(new Instruction(this, InstructionName::ADDI, 0, 5, 0, 5));
   // temp->push_back(new Instruction(this, InstructionName::ADDI, 5, 6, 0, 4));
   // temp->push_back(new Instruction(this, InstructionName::JAL, 5, 6, 0, 6));
   // temp->push_back(new Instruction(this, InstructionName::ADDI, 11, 11, 0, 5));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 0, 3, 0, 5));
    temp->push_back(new Instruction(this, InstructionName::SW, 0, 3, 0, 4));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 11, 11, 0, 5));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 11, 11, 0, 5));
    temp->push_back(new Instruction(this, InstructionName::LW, 0, 7, 0, 4));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 7, 11, 0, 5));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 11, 11, 0, 5));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 11, 11, 0, 6));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 11, 11, 0, 6));
    //temp->push_back(new Instruction(this, InstructionName::ADD, 6, 5, 7, 0));
   // temp->push_back(new Instruction(this, InstructionName::BLE, 5, 6, 0, 6));
    temp->push_back(new Instruction(this, InstructionName::SW, 6, 5, 4, 4));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 11, 11, 0, 5));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 11, 11, 0, 6));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 11, 11, 0, 6));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 0, 5, 0, 1));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 11, 11, 0, 5));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 11, 11, 0, 6));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 11, 11, 0, 6));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 11, 11, 0, 6));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 11, 11, 0, 6));
    temp->push_back(new Instruction(this, InstructionName::LW, 6, 5, 4, 4));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 11, 11, 0, 6));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 11, 11, 0, 5));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 11, 11, 0, 6));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 11, 11, 0, 6));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 11, 11, 0, 6));
    temp->push_back(new Instruction(this, InstructionName::ADDI, 11, 11, 0, 6));

    InstructionModel *insModel = new InstructionModel(this, temp);

    ui->tblAssembled->setModel(insModel);
    //RegisterFile *regFile = new RegisterFile(this, 0, 32);
    //ProgramCounter *pc = new ProgramCounter(this, 0);
    //pc->setValue(10);
    //RegisterModel *regModel = new RegisterModel(this, regFile, pc);
    //ui->tblRegisters->setModel(regModel);

    //DataMemory *memory = new DataMemory(this, 0, 35, 7);
    //MemoryModel *memModel = new MemoryModel(this, memory);
    //ui->tblMemory->setModel(memModel);

    core = new Core(this, *temp);
    connect(core, SIGNAL(stalled()), this, SLOT(simulationStalled()));
    connect(core, SIGNAL(forwarded(QString)), this, SLOT(simulationForwarded(QString)));
    connect(core, SIGNAL(currentStage(QString,int,int,Instruction*)), this, SLOT(addStage(QString,int,int,Instruction*)));
    ui->tblRegisters->setModel(new RegisterModel(this, core->getRegisterFile(), core->getProgramCounter()));
    ui->tblMemory->setModel(new MemoryModel(this, core->getDataMemory()));
    ui->tblPipieline->setModel(new BuffersModel(this, core->getIFID(), core->getIDEX(), core->getEXMEM(), core->getMEMWB()));
    //**************End test**************************
#else
    core = new Core(this);
    ui->tblAssembled->setModel(new InstructionModel(this, &emptyInsList));
    ui->tblRegisters->setModel(new RegisterModel(this, core->getRegisterFile(), core->getProgramCounter()));
    ui->tblMemory->setModel(new MemoryModel(this, core->getDataMemory()));
    ui->tblPipieline->setModel(new BuffersModel(this, core->getIFID(), core->getIDEX(), core->getEXMEM(), core->getMEMWB()));
    ui->tblAssembled->resizeColumnsToContents();
    ui->tblQueue->setModel(new InstructioQueueModel(this, &emptyInsList));
#endif


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

    ui->tblPpl->setColumnCount(0);
    ui->tblPpl->setRowCount(0);
    pplHeader.clear();
    ui->tblPpl->setHorizontalHeaderLabels(pplHeader);

    simulator = new Simulator(this, instructions);
    bool parsed = simulator->parseInstructions();
    ui->actionNextCycle->setEnabled(parsed);
    statusBar()->clearMessage();
    if (parsed){
        ui->txtConsole->clear();
        core = simulator->getCore();
        ui->tblAssembled->setModel(new InstructionModel(this, &(simulator->_instructions)));
        ui->tblRegisters->setModel(new RegisterModel(this, core->getRegisterFile(), core->getProgramCounter()));
        ui->tblMemory->setModel(new MemoryModel(this, core->getDataMemory()));
        ui->tblPipieline->setModel(new BuffersModel(this, core->getIFID(), core->getIDEX(), core->getEXMEM(), core->getMEMWB()));
        ui->tblAssembled->resizeColumnsToContents();
        ui->tblQueue->setModel(new InstructioQueueModel(this, &(core->_instrQueue)));
        connect(core, SIGNAL(stalled()), this, SLOT(simulationStalled()));
        connect(core, SIGNAL(forwarded(QString)), this, SLOT(simulationForwarded(QString)));
        connect(core, SIGNAL(simulationComplete()), this, SLOT(simulationComplete()));
        connect(core, SIGNAL(currentStage(QString,int,int,Instruction*)), this, SLOT(addStage(QString,int,int,Instruction*)));
        ui->twdCode->setTabText(1, "Assembly - Cycle " + QString::number(simulator->getCurrentCycle()));
    }else{
         qDebug() << "Parsing error";
         QStringList errors = simulator->getErrors();
        ui->txtConsole->clear();
        foreach(const QString err, errors){
            appendErrorMessage(err);
        }

    }
     ui->twdCode->setCurrentIndex(1);

}

void MainWindow::on_actionStartSimulation_triggered(){
    simulationRunning = true;
    refreshSimulationActions();
    ui->twdCode->setTabText(1, "Assembly - Cycle " + QString::number(simulator->getCurrentCycle()));
}

void MainWindow::on_actionNextCycle_triggered(){
    //simulator->nextCycle();
    //ui->twdCode->setTabText(1, "Assembly - Cycle " + QString::number(simulator->getCurrentCycle()));
    //core->executeCycle();
    statusBar()->clearMessage();
    auto temp = ui->tblQueue->model();
    ui->tblQueue->setModel(new InstructioQueueModel(this, &(core->_instrQueue)));
    if (temp)
        delete temp;
    simulator->nextCycle();
    ui->twdCode->setTabText(1, "Assembly - Cycle " + QString::number(/*simulator->getCurrentCycle()*/simulator->getCurrentCycle()));
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

void MainWindow::simulationStalled(){
    qDebug() << "Stall!!";
    statusBar()->clearMessage();
    statusBar()->showMessage("Pipeline Stalled", 3000);

}

void MainWindow::simulationForwarded(QString msg){
    qDebug() << "Forwarded: " << msg;
    statusBar()->clearMessage();
    statusBar()->showMessage(msg);
}

void MainWindow::simulationComplete(){
    statusBar()->showMessage("Simulation Complete", 3000);
}

void MainWindow::appendErrorMessage(QString msg){
    qDebug() << msg;
    ui->txtConsole->setText(ui->txtConsole->toPlainText() + "\n" + msg);
    //ui->txtConsole->append(msg + "\n");
}

void MainWindow::addStage(QString stage, int cycle, int instructionNumber, Instruction *instr){
    qDebug() <<"ADD STAGE: " << stage << "  -  " << cycle << "  -  " << instructionNumber << "  -   " << instr->toString();
    if(cycle >= ui->tblPpl->columnCount()){
        qDebug() << "Adding column";
        ui->tblPpl->insertColumn(ui->tblPpl->columnCount());
        pplHeader << "Cycle " + QString::number(cycle);
        ui->tblPpl->setHorizontalHeaderLabels(pplHeader);
    }
    if (instructionNumber >= ui->tblPpl->rowCount()){
        qDebug() << "Adding row";
        ui->tblPpl->insertRow(ui->tblPpl->rowCount());
    }
    qDebug() << ui->tblPpl->rowCount();
    qDebug() << ui->tblPpl->columnCount();

    ui->tblPpl->setItem(instructionNumber, cycle, new QTableWidgetItem(stage + ": " + instr->toString()));
    ui->tblPpl->resizeColumnsToContents();

}

void MainWindow::enableSimulation(){
    ui->actionStartSimulation->setEnabled(!simulationRunning && simulator->isReady());
}

MainWindow::~MainWindow(){
    delete ui;
}
