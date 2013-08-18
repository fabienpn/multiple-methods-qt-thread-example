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

#include "worker.h"
#include <QTimer>
#include <QEventLoop>

#include <QThread>
#include <QDebug>

Worker::Worker(QObject *parent) :
    QObject(parent)
{
    _abort = false;
    _interrupt = false;
}

void Worker::requestMethod(Worker::Method method)
{
    qDebug()<<"Request worker Method"<<method<<"in Thread "<<thread()->currentThreadId();
    QMutexLocker locker(&mutex);
    _interrupt = true;
    _method = method;
    condition.wakeOne();
}

void Worker::abort()
{
    qDebug()<<"Request worker aborting in Thread "<<thread()->currentThreadId();
    QMutexLocker locker(&mutex);
    _abort = true;
    condition.wakeOne();
}

void Worker::doMethod1()
{
    qDebug()<<"Starting Method1 in Thread "<<thread()->currentThreadId();

    for (int i = 0; i < 10; i ++) {

        mutex.lock();
        bool abort = _abort;
        bool interrupt = _interrupt;
        mutex.unlock();

        if (abort || interrupt) {
            qDebug()<<"Interrupting Method1 in Thread "<<thread()->currentThreadId();
            break;
        }

        QEventLoop loop;
        QTimer::singleShot(1000, &loop, SLOT(quit()));
        loop.exec();

        emit valueChanged(QString::number(i));
    }
}

void Worker::doMethod2()
{
    qDebug()<<"Starting Method2 in Thread "<<thread()->currentThreadId();

    for (int i = 0; i < 20; i ++) {

        mutex.lock();
        bool abort = _abort;
        bool interrupt = _interrupt;
        mutex.unlock();

        if (abort || interrupt) {
            qDebug()<<"Interrupting Method2 in Thread "<<thread()->currentThreadId();
            break;
        }

        QEventLoop loop;
        QTimer::singleShot(1000, &loop, SLOT(quit()));
        loop.exec();

        emit valueChanged(QString::number(i));
    }
}

void Worker::doMethod3()
{
    qDebug()<<"Starting Method3 in Thread "<<thread()->currentThreadId();

    for (int i = 0; i < 30; i ++) {

        mutex.lock();
        bool abort = _abort;
        bool interrupt = _interrupt;
        mutex.unlock();

        if (abort || interrupt) {
            qDebug()<<"Interrupting Method3 in Thread "<<thread()->currentThreadId();
            break;
        }

        QEventLoop loop;
        QTimer::singleShot(1000, &loop, SLOT(quit()));
        loop.exec();

        emit valueChanged(QString::number(i));
    }
}

void Worker::mainLoop()
{
    qDebug()<<"Starting worker mainLoop in Thread "<<thread()->currentThreadId();

    forever {

        mutex.lock();
        if (!_interrupt && !_abort) {
            condition.wait(&mutex);
        }
        _interrupt = false;

        if (_abort) {
            emit finished();
            return;
        }

        Method method = _method;
        mutex.unlock();

        switch(method) {
        case Method1:
            doMethod1();
            break;
        case Method2:
            doMethod2();
            break;
        case Method3:
            doMethod3();
            break;
        }
    }
}
