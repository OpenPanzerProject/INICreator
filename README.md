# INI Creator for the Open Panzer Sound Card
The [Open Panzer Sound Card](https://github.com/OpenPanzerProject/OP-Sound) is a device used to play sounds in RC models. It can be controlled in two ways - by the [Tank Control Board (TCB)](https://github.com/OpenPanzerProject/TCB) in which case all configuration is accomplished by the TCB's own configuration program called [OP Config](https://github.com/OpenPanzerProject/OP-Config). 

Alternatively the sound card can be controlled directly by standard hobby RC signals, in which case all user settings must be written to an INI file that is placed on the sound card's Micro SD memory stick. This INI Creator utility is an application that will create this INI file for you through an easy-to-use Windows interface. 

In other words, you only need this application if you intend to use the sound card in RC mode. If you are using it in combination with the TCB you can ignore this utility. 

This repository contains the source code for the INI Creator Utility, written in C++ in Qt Creator 3.4.1 using Qt 5.4.2. Qt is an open source application development software. Unless you are a developer that wants to re-compile or modify this application you probably don't need anything on this page. Instead, you can download a pre-compiled, installable version of INI Creator on the [Open Panzer Downloads](http://www.openpanzer.org/downloads#sound) page.

## Further Resources
  * For more information on the Open Panzer project, see the [OpenPanzer Wiki](http://wiki.openpanzer.org).
  * To discuss the project, feel free to join the [Open Panzer Community](http://openpanzer.org/forum/index.php?action=forum).
  * To download a compiled version of INI Creator for installation in Windows, see the [Open Panzer Downloads](http://www.openpanzer.org/downloads#sound) page. 

## License
Source files for the INI Creator are free software: you can redistribute them and/or modify them under the terms of the GNU General Public License version 3 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 

For more specific details see [http://www.gnu.org/licenses](http://www.gnu.org/licenses), the [Quick Guide to GPLv3.](http://www.gnu.org/licenses/quick-guide-gplv3.html) and the [copying.txt](https://github.com/OpenPanzerProject/OP-Config/blob/master/COPYING.txt) file in the codebase.

The GNU operating system which is under the same license has an informative [FAQ here](http://www.gnu.org/licenses/gpl-faq.html).
