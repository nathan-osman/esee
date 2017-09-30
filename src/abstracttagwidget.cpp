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

#include "abstracttagwidget.h"

AbstractTagWidget::AbstractTagWidget(ExifIfd ifd, const QString &name, QWidget *parent)
    : QWidget(parent),
      mIfd(ifd),
      mTag(exif_tag_from_name(name.toUtf8().constData()))
{
}

void AbstractTagWidget::read(ExifData *data)
{
    reset();
    ExifEntry *entry = exif_data_get_entry(data, mTag);
    if (!entry) {
        return;
    }
    readTag(entry);
}

void AbstractTagWidget::write(ExifData *data)
{
    writeTag(data);
}

QSize AbstractTagWidget::sizeHint() const
{
    return QSize(400, QWidget::sizeHint().height());
}

ExifIfd AbstractTagWidget::ifd() const
{
    return mIfd;
}

ExifTag AbstractTagWidget::tag() const
{
    return mTag;
}
