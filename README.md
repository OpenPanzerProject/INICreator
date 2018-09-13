# INI Creator for the Open Panzer Sound Card
The [Open Panzer Sound Card](https://github.com/OpenPanzerProject/OP-Sound) is a device used to play sounds in RC models. It can be controlled in two ways - by the [Tank Control Board (TCB)](https://github.com/OpenPanzerProject/TCB) in which case all configuration is accomplished by the TCB's own configuration program called [OP Config](https://github.com/OpenPanzerProject/OP-Config). Alternatively the sound card can be controlled directly by standard hobby RC signals, in which case all user settings must be written to an INI file that is placed on the sound card's Micro SD memory stick. This INI Creator utility is an application that will create this INI file for you through an easy-to-use Windows interface. 

In other words, you only need this application if you intend to use the sound card in RC mode. If you are using it in combination with the TCB you can ignore this utility. 

This repository contains the source code for the INI Creator Utility, written in C++ in Qt Creator 3.4.1 using Qt 5.4.2. Qt is an open source application development software. Unless you are a developer that wants to re-compile or modify this application you probably don't need anything on this page. Instead, you can download a pre-compiled, installable version of INI Creator on the [Open Panzer Downloads page](http://www.openpanzer.org/downloads#sound).

## Resources
  * For more information on the Open Panzer project, see the [OpenPanzer Wiki](http://wiki.openpanzer.org).
  * To discuss the project, feel free to join the [Open Panzer Community](http://openpanzer.org/forum/index.php?action=forum).
  * To download a compiled version of OP Config for installation in Windows, see the [Open Panzer Downloads page](http://www.openpanzer.org/downloads). 
