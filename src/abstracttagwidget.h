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

#ifndef ABSTRACTTAGWIDGET_H
#define ABSTRACTTAGWIDGET_H

#include <libexif/exif-data.h>
#include <libexif/exif-entry.h>

#include <QWidget>

/**
 * @brief Base class for all tag widgets
 *
 * Each derived class must implement the two methods used for reading and
 * writing the tag value from an ExifData instance.
 */
class AbstractTagWidget : public QWidget
{
    Q_OBJECT

public:

    AbstractTagWidget(const QString &name, QWidget *parent = nullptr);

    void read(ExifData *data);
    void write(ExifData *data);

signals:

    void changed();

protected:

    QString title() const;

    virtual void readTag(ExifEntry *entry) = 0;
    virtual void writeTag(ExifEntry *entry) = 0;

private:

    QString mName;
};

#endif // ABSTRACTTAGWIDGET_H
