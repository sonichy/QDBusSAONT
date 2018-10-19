#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextEdit>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QMdiSubWindow>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    QTextEdit *textEdit = new QTextEdit;
    textEdit->setText("这是一个新文档");
    //QMdiSubWindow *subWindow =
    ui->mdiArea->addSubWindow(textEdit);
    //Q_UNUSED(subWindow);
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "打开文本", ".");
    if (!filename.isEmpty()) {
        load(filename);
    }
}

QString MainWindow::load(QString filepath)
{
    if(filepath.contains("file://")){
        filepath.remove("file://");
    }
    QFile file(filepath);
    bool ok = file.open(QIODevice::ReadOnly);
    if(ok){
        QTextStream TS(&file);
        QString s = TS.readAll();
        file.close();
        //delete file;
        QTextEdit *textEdit =  new QTextEdit;
        textEdit->setWindowTitle(filepath);
        textEdit->setPlainText(s);
        ui->mdiArea->addSubWindow(textEdit);
        activateWindow();
        return filepath + " load sucessful";
    }else{
        QMessageBox::warning(this, "错误", QString(" %1:\n%2").arg(filepath).arg(file.errorString()));
        return file.errorString();
    }
}