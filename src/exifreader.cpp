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

#include <libexif/exif-entry.h>
#include <libexif/exif-tag.h>

#include "exifreader.h"

ExifReader::ExifReader(QObject *parent)
    : QObject(parent),
      mData(nullptr)
{
}

ExifReader::~ExifReader()
{
    if (mData) {
        exif_data_unref(mData);
    }
}

bool ExifReader::load(const QString &filename)
{
    mData = exif_data_new_from_file(filename.toUtf8().constData());
    return mData != nullptr;
}

QImage ExifReader::thumbnail() const
{
    QImage image;
    image.loadFromData(mData->data, mData->size);
    return image;
}

QString ExifReader::title(const QString &key) const
{
    // Convert the name to an ExifTag
    ExifTag tag = exif_tag_from_name(key.toUtf8().constData());
    if (!tag) {
        return QString();
    }

    return exif_tag_get_title(tag);
}

QString ExifReader::value(const QString &key) const
{
    // Convert the name to an ExifTag
    ExifTag tag = exif_tag_from_name(key.toUtf8().constData());
    if (!tag) {
        return QString();
    }

    // Attempt to find the tag in the data
    ExifEntry *entry = exif_data_get_entry(mData, tag);
    if (!entry) {
        return QString();
    }

    char data[256];
    return exif_entry_get_value(entry, data, sizeof(data) / sizeof(char));
}
