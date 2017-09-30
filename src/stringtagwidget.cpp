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

#include <cstring>

#include <libexif/exif-content.h>
#include <libexif/exif-format.h>
#include <libexif/exif-mem.h>

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

#include "stringtagwidget.h"

StringTagWidget::StringTagWidget(ExifIfd ifd, const QString &name, QWidget *parent)
    : AbstractTagWidget(ifd, name, parent),
      mLineEdit(new QLineEdit)
{
    connect(mLineEdit, &QLineEdit::textChanged, this, &StringTagWidget::changed);

    QLabel *label = new QLabel(tr("%1:").arg(exif_tag_get_title(tag())));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(mLineEdit);
    setLayout(layout);
}

void StringTagWidget::reset()
{
    mLineEdit->clear();
}

void StringTagWidget::readTag(ExifEntry *entry)
{
    char data[256];
    exif_entry_get_value(entry, data, sizeof(data) / sizeof(char));
    mLineEdit->setText(data);
}

void StringTagWidget::writeTag(ExifData *data)
{
    ExifMem *mem = nullptr;
    ExifEntry *entry = nullptr;

    do {

        // Create a memory allocator
        mem = exif_mem_new_default();
        if (!mem) {
            break;
        }

        // Create a new entry
        entry = exif_entry_new_mem(mem);
        if (!entry) {
            break;
        }

        // Retrieve the value of the widget
        QByteArray value = mLineEdit->text().toUtf8();

        // Allocate memory and copy data to the buffer
        void *buffer = exif_mem_alloc(mem, value.length());
        if (!buffer) {
            break;
        }
        memcpy(buffer, value.constData(), value.length());

        // Initialize the entry
        entry->tag = tag();
        entry->format = EXIF_FORMAT_UNDEFINED;
        entry->components = value.length();
        entry->data = reinterpret_cast<unsigned char*>(buffer);
        entry->size = value.length();

        // Add the entry to the correct IFD
        exif_content_add_entry(data->ifd[ifd()], entry);

    } while (false);

    // Unref the entry and allocator if non-null
    if (entry) {
        exif_entry_unref(entry);
    }
    if (mem) {
        exif_mem_unref(mem);
    }
}
