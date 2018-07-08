/* OPConfig 		Configuration program for the Open Panzer Sound Card
 * Source: 			openpanzer.org
 * Authors:    		Luke Middleton
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "mainwindow.h"
//#include <QApplication>   // We are using SingleApplication instead, which is a subclass of QApplication
#include "winsparkle.h"
#include "singleapplication.h"


int main(int argc, char *argv[])
{
    // Doesn't seem to help the high DPI/screen scaling issue
    //QApplication::setDesktopSettingsAware();

    // This was only introduced in Qt 5.6 and may help with high DPI issues
    // Untested because I am still compiling in 5.4 for other reasons (Qt Assistant doesn't work in 5.6 for one)
    //QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication::setApplicationName("OP Sound INI Creator");
    QApplication::setApplicationVersion(VER_PRODUCTVERSION_STR);
    QApplication::setOrganizationName("OPEN PANZER");

    // Use the SingleApplication version instead - this prevents multiple instances from opening
    SingleApplication app(argc, argv);
    //QApplication app(argc, argv);


    MainWindow w;
    w.show();

    // When the user tries to open a second instance of the same application, the SingleApplication showUp() signal is emitted
    // We can then use that signal to bring the focus to the instance that is already running. Of course in Windows this will
    // only actually highlight it in your taskbar, it won't actually bring it to the fore.
    QObject::connect(&app, &SingleApplication::showUp, [&]
    {
        w.show();
        w.raise();
        w.activateWindow();
    });

    return app.exec();

}

