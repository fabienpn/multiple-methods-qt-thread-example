/*
   Copyright 2013 Fabien Pierre-Nicolas.
      - Primarily authored by Fabien Pierre-Nicolas

   This file is part of multiple-methods-qt-thread-example, a simple example to demonstrate how to use a thread running multiple methods.
   This example is explained on http://fabienpn.wordpress.com/qt-thread-multiple-methods-with-sources/

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This progra is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // The thread and the worker are created in the constructor so it is always safe to delete them.
    thread = new QThread();
    worker = new Worker();

    worker->moveToThread(thread);
    connect(thread, SIGNAL(started()), worker, SLOT(mainLoop()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
    qDebug()<<"Starting thread in Thread "<<this->QObject::thread()->currentThreadId();
    thread->start();

    connect(worker, SIGNAL(valueChanged(QString)), ui->label, SLOT(setText(QString)));
}

MainWindow::~MainWindow()
{
    worker->abort();
    thread->wait();
    qDebug()<<"Deleting thread and worker in Thread "<<this->QObject::thread()->currentThreadId();
    delete thread;
    delete worker;

    delete ui;
}

void MainWindow::on_method1Button_clicked()
{
    worker->requestMethod(Worker::Method1);
}

void MainWindow::on_method2Button_clicked()
{
    worker->requestMethod(Worker::Method2);
}

void MainWindow::on_method3Button_clicked()
{
    worker->requestMethod(Worker::Method3);
}
