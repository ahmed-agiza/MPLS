#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

#include "simulator.h"
#include "codeeditor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_actionClose_triggered();

    void on_actionAbout_triggered();

    void on_actionAssemble_triggered();

    void on_actionStartSimulation_triggered();

    void on_actionNextCycle_triggered();

    void on_actionStopSimulation_triggered();

private:
    Ui::MainWindow *ui;
    Simulator simulator;
    CodeEditor *editor;
};

#endif // MAINWINDOW_H
