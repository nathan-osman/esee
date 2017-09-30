/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Nathan Osman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <QAction>
#include <QCloseEvent>
#include <QFileDialog>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

#include "mainwindow.h"

MainWindow::MainWindow()
    : mOpen(new QAction(tr("&Open..."), this)),
      mSave(new QAction(tr("&Save"), this)),
      mDirty(false)
{
    connect(mOpen, &QAction::triggered, this, &MainWindow::onOpen);
    connect(mSave, &QAction::triggered, this, &MainWindow::onSave);

    mSave->setEnabled(false);

    QMenu *file = menuBar()->addMenu(tr("&File"));
    file->addAction(mOpen);
    file->addAction(mSave);
    file->addSeparator();
    file->addAction(tr("&Quit"), this, &MainWindow::close);
}

void MainWindow::openImage(const QString &filename)
{
    //...
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (mDirty) {
        QMessageBox::StandardButton button = QMessageBox::warning(
            this,
            tr("Warning"),
            tr("This image has unsaved changes. Are you sure you want to quit?"),
            QMessageBox::Yes | QMessageBox::No
        );
        if (button == QMessageBox::No) {
            event->ignore();
        }
    }
}

void MainWindow::onOpen()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        tr("Open File")
    );
    if (!filename.isNull()) {
        openImage(filename);
    }
}

void MainWindow::onSave()
{
    //...
}
