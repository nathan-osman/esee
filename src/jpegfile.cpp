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

#include <QFile>
#include <QtEndian>

#include "jpegfile.h"

JpegFile::JpegFile(const QString &filename)
    : mFilename(filename),
      mData(nullptr)
{
}

JpegFile::~JpegFile()
{
    if (mData) {
        exif_data_unref(mData);
    }
}

bool JpegFile::open()
{
    // Open the file for reading
    QFile file(mFilename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    // Alocate a buffer the size of the file and read it in
    QByteArray buffer = file.readAll();
    file.close();

    // Ensure the entire file was read
    if (buffer.length() != file.size()) {
        return false;
    }

    const unsigned char *start = reinterpret_cast<const unsigned char*>(buffer.constData());
    const unsigned char *end = start + buffer.length();
    unsigned char *p = reinterpret_cast<unsigned char*>(buffer.data());

    // Read segments until EOF is reached
    while (true) {

        // Remember the beginning of the segment
        const unsigned char *segmentStart = p;

        // Read the marker
        quint16 marker;
        if (!readQuint16(p, end, marker)) {
            return false;
        }

        // If the marker is start of image, then continue
        if (marker == 0xffd8) {
            continue;
        }

        // If the marker is end-of-image, then quit
        if (marker == 0xffd9) {
            break;
        }

        // Two different types of segments exist - start-of-scan which
        // requires searching content for the end of the segment and all other
        // types which include a 16-bit unsigned int indicating size
        int segmentSize;
        if (marker == 0xffda) {
            if (!findNextSegment(p, end)) {
                return false;
            }
            segmentSize = p - segmentStart;
        } else {
            quint16 dataSize;
            if (!readQuint16(p, end, dataSize) || p + dataSize >= end) {
                return false;
            }
            p += dataSize - 2;
            segmentSize = sizeof(quint16) + dataSize;
        }

        // For the APP1 segment (EXIF), initialize mData; for all other
        // segments, create a QByteArray and append it to the list
        if (marker == 0xffe1) {
            mData = exif_data_new_from_data(
                segmentStart,
                segmentSize
            );
        } else {
            mSegments.append(QByteArray(reinterpret_cast<const char*>(segmentStart), segmentSize));
        }
    }

    return true;
}

bool JpegFile::save()
{
    // Create a buffer to store the file content
    QByteArray buffer;

    // Write the start-of-image segment
    writeQuint16(buffer, 0xffd8);

    // Write the EXIF segment
    unsigned char *data;
    unsigned int dataSize;
    exif_data_save_data(mData, &data, &dataSize);
    if (!dataSize) {
        return false;
    }
    writeQuint16(buffer, 0xffe1);
    writeQuint16(buffer, dataSize + sizeof(quint16));
    buffer.append(reinterpret_cast<const char*>(data), dataSize);
    free(data);

    // Write the other segments
    foreach (const QByteArray &segment, mSegments) {
        buffer.append(segment);
    }

    // Write the end-of-image segment
    writeQuint16(buffer, 0xffd9);

    // Open the file for writing
    QFile file(mFilename);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    // Write the buffer
    qint64 bytesWritten = file.write(buffer);
    file.close();

    return bytesWritten == buffer.length();
}

ExifData *JpegFile::data() const
{
    return mData;
}

bool JpegFile::readQuint16(unsigned char *&p, const unsigned char *end, quint16 &value)
{
    if (p + sizeof(quint16) > end) {
        return false;
    }
    value = qToBigEndian(*reinterpret_cast<quint16*>(p));
    p += sizeof(quint16);
    return true;
}

bool JpegFile::findNextSegment(unsigned char *&p, const unsigned char *end)
{
    while (true) {
        while (p < end && *p != 0xff) {
            ++p;
        }
        if (p + 1 >= end) {
            return false;
        }
        unsigned char q = *(p + 1);
        if (q == 0x00 || q & 0xf8 == 0xd0) {
            p += 2;
            continue;
        } else {
            return true;
        }
    }
}

void JpegFile::writeQuint16(QByteArray &buffer, quint16 value)
{
    value = qToBigEndian(value);
    buffer.append(reinterpret_cast<const char*>(&value), sizeof(quint16));
}
