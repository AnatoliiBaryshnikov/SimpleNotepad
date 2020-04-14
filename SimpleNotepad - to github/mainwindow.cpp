#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QTextStream>
#include <QPrinter>
#include <QPrintDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
    ui->statusbar->addPermanentWidget(ui->label_statusbar, 1);
    ui->label_statusbar->setText("New file");
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::NewFileRoutine()
{
    ui->textEdit->clear();
    default_filepath = "";
    ui->label_statusbar->setText("New file");
}

void MainWindow::openFileRoutine()
{
    QString file_path = QFileDialog::getOpenFileName(
                this, "Open a file..", QDir::homePath(), "Text file (*.txt) ;; All files (*.*)"  );
    QFile current_file (file_path);
    bool opened = current_file.open(QFile::ReadOnly | QFile::Text);
    if (!opened)
    {
        QMessageBox::warning(this, "error", "File not opened");
        return;
    }
    else
    {
        QTextStream fin (&current_file);
        ui->textEdit->setText(fin.readAll());
        current_file.close();
        default_filepath = file_path;
        ui->label_statusbar->setText(default_filepath);
    }
}

void MainWindow::saveFileRoutine()
{
    if (default_filepath == "")
    {
        saveAsFileRoutine();
        return;
    }
    else
    {
        QFile current_file (default_filepath);
        bool opened = current_file.open(QFile::WriteOnly | QFile::Text);
        if (!opened)
        {
            QMessageBox::warning(this, "Problem", "File not opened!!\nSomething terrible happened!!!");
            return;
        }
        else
        {
            QTextStream fout (&current_file);
            fout << ui->textEdit->toPlainText();
            current_file.flush();
            current_file.close();
            ui->label_statusbar->setText(default_filepath);
        }
    }
}

void MainWindow::saveAsFileRoutine()
{
    QString file_path = QFileDialog::getSaveFileName(this,
                       "Save as...", QDir::homePath(), "Text file *.txt");
    if (file_path == "")
    {
        QMessageBox::StandardButton continue_dialog = QMessageBox::question(this, "Problem", "Current file is not saved!\nContinue saving?");
        if (continue_dialog == QMessageBox::No)
        {
            return;
        }
    }
    default_filepath = file_path;
    saveFileRoutine();
}

void MainWindow::on_actionOpen_triggered()
{
    QMessageBox::StandardButton save_current =
                QMessageBox::question(this, "Save...", "Save current file?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
    if (save_current == QMessageBox::Yes)
    {
       saveFileRoutine();
       openFileRoutine();
    }
    else if (save_current == QMessageBox::Cancel)
    {
       return;
    }
    else openFileRoutine();
}

void MainWindow::on_actionNew_triggered()
{
    QMessageBox::StandardButton save_current =
                QMessageBox::question(this, "Save...", "Save current file?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
    if (save_current == QMessageBox::Yes)
    {
       saveFileRoutine();
       NewFileRoutine();
    }
    else if (save_current == QMessageBox::Cancel)
    {
       return;
    }
    else NewFileRoutine();
}

void MainWindow::on_actionSave_triggered()
{
     saveFileRoutine();
}

void MainWindow::on_actionSaveAs_triggered()
{
     saveAsFileRoutine();
}



void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}




void MainWindow::on_actionPrint_triggered()
{
    QPrinter default_printer;
    QPrintDialog pDialog(&default_printer, this);

    if (ui->textEdit->textCursor().hasSelection())
       pDialog.addEnabledOption(QAbstractPrintDialog::PrintSelection);

    int isDialogAccepted = pDialog.exec();

             if (isDialogAccepted != 1) {
                 QMessageBox::warning(this, "Print error", "File is not printed!");
                 return;
             }

    ui->textEdit->print(&default_printer);
}



void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About SimpleNotepad",
                             "SimpleNotepad for Windows\n"
                             "Version 0.1\n"
                             "Developer: Anatolii Baryshnikov\n"
                             "\n"
                             "Icons by Freepik from www.flaticon.com\n");
}



void MainWindow::on_actionExit_triggered()
{
    QMessageBox::StandardButton save_current =
                QMessageBox::question(this, "Save...", "Save current file?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
    if (save_current == QMessageBox::Yes)
    {
       saveFileRoutine();
       QApplication::quit();
    }
    else if (save_current == QMessageBox::Cancel)
    {
       return;
    }
    else QApplication::quit();
}


