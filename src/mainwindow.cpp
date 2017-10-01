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

#include <libexif/exif-ifd.h>

#include <QAction>
#include <QCloseEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QWidget>

#include "jpegfile.h"
#include "mainwindow.h"
#include "stringtagwidget.h"

MainWindow::MainWindow()
    : mSave(new QAction(tr("&Save"), this)),
      mSaveAs(new QAction(tr("&Save &As..."), this)),
      mFile(nullptr),
      mDirty(false)
{
    connect(mSave, &QAction::triggered, this, &MainWindow::onSave);
    connect(mSaveAs, &QAction::triggered, this, &MainWindow::onSaveAs);

    mSave->setEnabled(false);
    mSaveAs->setEnabled(false);

    QMenu *file = menuBar()->addMenu(tr("&File"));
    file->addAction(tr("&Open..."), this, &MainWindow::onOpen);
    file->addAction(mSave);
    file->addAction(mSaveAs);
    file->addSeparator();
    file->addAction(tr("&Quit"), this, &MainWindow::close);

    // Create all of the widgets
    mWidgets.append(new StringTagWidget(EXIF_IFD_0, "Make"));
    mWidgets.append(new StringTagWidget(EXIF_IFD_0, "Model"));
    mWidgets.append(new StringTagWidget(EXIF_IFD_EXIF, "DateTimeOriginal"));
    mWidgets.append(new StringTagWidget(EXIF_IFD_EXIF, "DateTimeDigitized"));

    // Create a layout and add all of the widgets
    QVBoxLayout *layout = new QVBoxLayout;
    foreach (AbstractTagWidget *widget, mWidgets) {
        connect(widget, &AbstractTagWidget::changed, this, &MainWindow::onChanged);
        widget->setEnabled(false);
        layout->addWidget(widget);
    }

    // Add the spacer
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // Create the main widget and add the layout
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);

    updateTitle();
}

MainWindow::~MainWindow()
{
    if (mFile) {
        delete mFile;
    }
}

void MainWindow::openImage(const QString &filename)
{
    // Attempt to open the file
    JpegFile *file = new JpegFile(filename);
    if (!file->open()) {
        QMessageBox::critical(
            this,
            tr("Error"),
            tr("Unable to read EXIF data from %1.").arg(QFileInfo(filename).fileName())
        );
        delete file;
        return;
    }

    // Free an existing image
    if (mFile) {
        delete mFile;
    }
    mFile = file;

    // Update each of the widgets
    foreach (AbstractTagWidget *widget, mWidgets) {
        widget->setEnabled(true);
        widget->read(file->data());
    }

    // Update the rest of the UI
    mSave->setEnabled(true);
    mSaveAs->setEnabled(true);
    mFilename = filename;
    mDirty = false;
    updateTitle();
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
        tr("Open File"),
        QString(),
        tr("JPEG images (*.jpg *.jpeg)")
    );
    if (!filename.isNull()) {
        openImage(filename);
    }
}

void MainWindow::onSave()
{
    // Update data for each of the widgets
    foreach (AbstractTagWidget *widget, mWidgets) {
        widget->write(mFile->data());
    }

    if (!mFile->save()) {
        QMessageBox::critical(
            this,
            tr("Error"),
            tr("Unable to save %1.").arg(QFileInfo(mFilename).fileName())
        );
        return;
    }

    mDirty = false;
    updateTitle();
}

void MainWindow::onSaveAs()
{
    QString filename = QFileDialog::getSaveFileName(
        this,
        tr("Save File As"),
        QString(),
        tr("JPEG images (*.jpg)")
    );
    if (!filename.isNull()) {
        mFilename = filename;
        onSave();
    }
}

void MainWindow::onChanged()
{
    if (!mDirty) {
        mDirty = true;
        updateTitle();
    }
}

void MainWindow::updateTitle()
{
    QString title = tr("Extremely Simple EXIF Editor");
    if (!mFilename.isNull()) {
        title = tr("%1 - %2").arg(title).arg(QFileInfo(mFilename).fileName());
        if (mDirty) {
            title = tr("%1 (*)").arg(title);
        }
    }
    setWindowTitle(title);
}
