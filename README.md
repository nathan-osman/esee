## esev

esev provides a UI for viewing the EXIF data in JPG files.

### Screenshot

Here's a screenshot of esev in action on Linux:

![esev window](https://i.stack.imgur.com/wgbTz.png)

### Building

To build esev, ensure that the following are installed:

- C++ compiler with C++11 support
- CMake 3.2+
- Qt 5.4+
- libexif

Open a terminal and run the following commands:

    mkdir build
    cd build
    cmake ..
    make install

If all goes well, you should be able to run `esev <filename>` to show EXIF data.
