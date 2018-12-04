LibreTuner-Qt
=============

A graphical frontend for LibreTuner using Qt.

## Dependencies
* [Qt5](https://www.qt.io/) - On Windows, be sure to install the MinGW version. If you do not already have MinGW installed, you can install it through Qt.
* [CMake](https://cmake.org/)


## Building
Clone the repo
`git clone https://github.com/LibreTuner/LibreTuner-Qt.git`
`cd LibreTuner-Qt`
Make a build directory
`mkdir build`
`cd build`

The rest build process is currently platform-dependent. In the future, Cargo should handle the build.

### Linux
Run CMake and start the build
`cmake .. && make`

### Windows
#### MSVC
TODO
#### MinGW
Locate your Qt directory. This will probably be similar to C:\Qt\5.11.2\mingw53_32. Next, run CMake. Replace "QT_DIRECTORY" with the path you just found.
`cmake -G "MinGW Makefiles" -DQt_DIR="QT_DIRECTORY\lib\cmake\Qt5"`
Run mingw32-make
`mingw32-make.exe`
If you get a "command not found" error, you need to add MinGW to your PATH. You can read how in the "Environment Settings" section of [this page](http://www.mingw.org/wiki/Getting_Started). If you installed MinGW through Qt, your installation directory should be in `C:\Qt\Tools` depending on where you installed Qt.



Run LibreTuner with
`gui\gui.exe`