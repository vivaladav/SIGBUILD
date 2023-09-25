![SIGBUILD](https://raw.githubusercontent.com/vivaladav/SIGBUILD/master/doc/img/SIGBUILD_logo-white_bg-128.png)


## Project
A Qt Creator plugin to get notified when your builds terminate and to track their stats.

### Project Status
Currently the plugin is usable and the project is under active development, but not ready for a proper release yet, hence no binary builds are available.

People who want to start to use SIGBUILD need to build it from scratch following the instruction in the Development section.

Because of the current status of the project, it's guaranteed only that SIGBUILD will work with the latest version of Qt Creator.
Previous versions are likely to work too, but they are not officially supported, so please do not create issues for them.

### Current features
- Systray notifications
- Audio notifications
- Latest build info
- Session log
- Full Qt Creator options

### Support this projec
You can support the development of this project becoming a [GitHub Sponsor](https://github.com/sponsors/vivaladav).

That will allow me to spend more time on it and it will allow you to influence the development roadmap.

## Development
Currently the best way to use SIGBUILD is to build Qt Creator and then build the plugin.

Ideally you want to build SIGBUILD and Qt Creator against the same version of Qt and using the same (or compatible) compiler used to build the version of Qt Creator you run.
For example, Qt Creator 11.0.2 has been build against Qt 6.4.3 using gcc 10.3.1 on Linux and you should use a similar configuration when building Qt Creator and SIGBUILD.

The current versio of SIGBUILD is tested to build against the 11.0 branch of Qt Creator.
In case you need to build SIGBUILD for an old version of Qt Creator (<= 6) checkout the "QtCreator-6" tag with git.

### Dependencies
- Qt 6 - [online installer](https://www.qt.io/download-qt-installer) / [offline installer](https://www.qt.io/offline-installers)
- Qt Creator - [online installer](https://www.qt.io/download-qt-installer) / [offline installer](https://www.qt.io/offline-installers)
- Qt Creator source code - [git repository](https://code.qt.io/cgit/qt-creator/qt-creator.git/)

Note that when installing Qt you will need at least the following 2 modules:
- Desktop binaries (ex.: Desktop gcc 64bit)
- Qt Multimedia (additional libraries)
Also when installing Qt Creator you will need to include the following package:
- Qt Creator x.y.z Plugin Development

### How to build Qt Creator
From inside the cloned qt-creator directory:
- Open CMakeLists.txt
- Make sure to set CMake variable "Qt6_DIR" to the Qt6 directory 
- Build

### How to build SIGBUILD
From inside the cloned SIGBUILD directory:
- Open CMakeLists.txt in Qt Creator
- Configure the project with a Qt6 kit
- Make sure to make the CMake variable "CMAKE_PREFIX_PATH" point to the Qt6 directory and to the Qt Creator build directory
- Run CMake
- Build

### How to run SIGBUILD
Start an instance of Qt Creator with the following command line option:
-pluginpath SIGBUILD_BUILD_DIRECTORY
