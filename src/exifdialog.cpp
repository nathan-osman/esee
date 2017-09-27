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

#include <QFileInfo>
#include <QFont>
#include <QGridLayout>
#include <QImage>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QVBoxLayout>

#include "exifdialog.h"
#include "exifreader.h"

ExifDialog::ExifDialog(const QString &filename)
    : mFilename(filename),
      mThumbnailImage(new QLabel),
      mGridLayout(new QGridLayout),
      mReader(new ExifReader(this))
{
    setupUi();
    loadMedia();
}

void ExifDialog::setupUi()
{
    setWindowTitle(tr("Extremely Simple EXIF Viewer - %1").arg(QFileInfo(mFilename).baseName()));

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout->addWidget(mThumbnailImage);
    vboxLayout->addLayout(mGridLayout);
    setLayout(vboxLayout);
}

void ExifDialog::loadMedia()
{
    if (mReader->load(mFilename)) {
        showThumbnail();
        addTag("Make");
        addTag("Model");
        addTag("DateTimeOriginal");
        addTag("DateTimeDigitized");
        addTag("ApertureValue");
        addTag("ShutterSpeedValue");
        addTag("Flash");
    } else {
        QMessageBox::critical(
            this,
            tr("Error"),
            tr("Unable to read %1.").arg(QFileInfo(mFilename).fileName())
        );
    }
}

void ExifDialog::showThumbnail()
{
    mThumbnailImage->setPixmap(QPixmap::fromImage(mReader->thumbnail()));
}

void ExifDialog::addTag(const QString &key)
{
    QLabel *tagKey = new QLabel(tr("%1:").arg(mReader->title(key)));
    QLabel *tagValue = new QLabel(mReader->value(key));

    QFont font = tagKey->font();
    font.setBold(true);
    tagKey->setFont(font);

    int newRow = mGridLayout->rowCount();
    mGridLayout->addWidget(tagKey, newRow, 0);
    mGridLayout->addWidget(tagValue, newRow, 1);
}
