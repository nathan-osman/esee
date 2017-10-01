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

#ifndef JPEGFILE_H
#define JPEGFILE_H

#include <libexif/exif-data.h>

#include <QByteArray>
#include <QList>

class QFile;

/**
 * @brief JPEG file loaded in memory
 *
 * In order to write a JPEG file with updated EXIF data, each segment in the
 * file must be preserved for later reassembly.
 */
class JpegFile
{
public:

    explicit JpegFile(const QString &filename);
    virtual ~JpegFile();

    bool open();
    bool save();

    ExifData *data() const;

private:

    bool readQuint16(unsigned char *&p, const unsigned char *end, quint16 &value);
    bool findNextSegment(unsigned char *&p, const unsigned char *end);

    void writeQuint16(QByteArray &buffer, quint16 value);

    QString mFilename;
    QList<QByteArray> mSegments;

    ExifData *mData;
};

#endif // JPEGFILE_H
