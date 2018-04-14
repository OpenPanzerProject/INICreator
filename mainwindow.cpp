/* OPConfig 		Configuration program for the Open Panzer TCB (Tank Control Board)
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
#include "ui_mainwindow.h"
#include "winsparkle.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Window icon
    setWindowIcon(QIcon(":/icons/images/OP_Icon_32.png"));

    // Create our assistant for help documentation
    assistant = new Assistant;

    // This fills our combo box with the list of detected COM ports
    fillPortsInfo();
    // We also create a timer to periodically update the list so as new devices are plugged in they automatically get added to the list (or removed)
    COMCheckTimer = new QTimer(this);
    connect(COMCheckTimer, SIGNAL(timeout()), this, SLOT(pollCOMPorts())); // Every time the COMCheckTimer pings, update the list of COM ports
    startCOMChecker(); // This starts the timer

    // We also initialize the baud rate combo. Note this baud rate is what the desktop app uses to communicate with the device,
    // but changing this will NOT change the device's baud rate.
    ui->cboConsoleBaud->setCategory(bcConsole);
    ui->cboConsoleBaud->setCurrentIndex(ui->cboConsoleBaud->findData(USB_DEFAULT_BAUD_RATE));  // See OpenPanzerComm.h for the value of default

    // Begin the OpenPanzer Communication object
    comm = new OpenPanzerComm();
    comm->begin();

    // Pass the OpenPanzer Communication object the selected com port
    setCOMPort();
    setBaudRate();

    // Message Box
    // ---------------------------------------------------------------------------------------------------------------------------------->>
    // We want a messagebox formatted with our specific CSS (qss) so we create one here to use throughout MainWindow
    mb = new QMessageBox(this);
    QFile file(":/css/msgbox.qss");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        mb->setStyleSheet(file.readAll());
        file.close();
    }

    // LIST OF TABS
    // ---------------------------------------------------------------------------------------------------------------------------------->>
    // List views are only representations of data models. In the h file we created a pointer to the QStringListModel class
    // which we now instantiate
    listViewWestModel = new QStringListModel(this);
    // Make data list
    QStringList strList;
    strList << "Radio" << "Sounds" << "Firmware";
    // Populate our model
    listViewWestModel->setStringList(strList);
    // Glue model and view together
    ui->listViewWest->setModel(listViewWestModel);
    // Select the first item. WOW! No chance of using a simple integer here. We must pass it a const QModelIndex type
    // We just pull it from our model, row 0, column 0
    ui->listViewWest->setCurrentIndex(listViewWestModel->index(0,0));

    // Help Buttons
    // ---------------------------------------------------------------------------------------------------------------------------------->>
    SetupHelpButtons();


    // ABSTRACT ACTIONS
    // ---------------------------------------------------------------------------------------------------------------------------------->>
    // These can show up in  menus, toolbars, etc...
    initActionsConnections();


    // CONNECTIONS
    // ---------------------------------------------------------------------------------------------------------------------------------->>
    // Let's connect the listview and stacked widget. What a convoluted nightmare this is, about three hours of work for these two lines.
    // QListView really doesn't have any signals that are worth a damn. Nor can we use a signal from QStringListModel object. Instead
    // we must use a signal from the more generic QItemSelectionModel which we point to our list view selection model
    // Now this signal works and passes both current and previous index numbers to our created function called changeStackedWidget.
    // We only need to use the new "current" index and set the stacked widget to that same index.
    QItemSelectionModel *selectionModel = ui->listViewWest->selectionModel();
    connect(selectionModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(changeStackedWidget(QModelIndex,QModelIndex)));
    // Uncomment the below if compiling with Qt 5.6
    // In Qt 5.6 whenever we click on the list view, it would select two items - the one you wanted to select, and the next one too.
    // This doesn't look right. If we use this second signal to re-assert the listView selected item, we can sort of work around it.
    // You get some flickering but it does work.
    //connect(ui->listViewWest, SIGNAL(clicked(QModelIndex)), this, SLOT(changeStackedWidget56Fix(QModelIndex)));

    // When we change items in the COM port or Baud rate drop-downs, update our port settings
    connect(ui->cboCOMPorts, SIGNAL(currentIndexChanged(int)), this, SLOT(setCOMPort()));
    connect(ui->cboConsoleBaud, SIGNAL(currentIndexChanged(int)), this, SLOT(setBaudRate()));

    // Connect cmdTest1 and cmdTest2 buttons
    connect(ui->cmdTest1, SIGNAL(clicked(bool)), this, SLOT(cmdTest1_Click()));
    connect(ui->cmdTest2, SIGNAL(clicked(bool)), this, SLOT(cmdTest2_Click()));
    // Hide them if we're not using them for testing
    ui->cmdTest1->hide();
    ui->cmdTest2->hide();

    // Connect HideHeader button
    connect(ui->cmdHideHeader, SIGNAL(clicked(bool)), this, SLOT(ShowHideHeader()));

    // WinSparkle init. WinSparkle is an application we use to update the program. But we don't want it to start until
    // after the main window has shown
    connect(this, SIGNAL(windowWasShown()), this, SLOT(initWinSparkle()), Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));


    // STATUS BAR   see: http://www.bogotobogo.com/Qt/Qt5_QStatusBar.php
    // ---------------------------------------------------------------------------------------------------------------------------------->>
    // We can't align individual widgets on the status bar, so instead, put them all in a single widget in the order we want, then add the
    // single widget to the status bar.
    QHBoxLayout *layout = new QHBoxLayout;
    // Left - Index 0: Serial status label
    serialStatusLabel = new QLabel(this);
    serialStatusLabel->setText("Not connected");
    serialStatusLabel->setMinimumWidth(260);
    serialStatusLabel->setMaximumWidth(260);
    layout->addWidget(serialStatusLabel,0,Qt::AlignLeft);       // Index 0 - Serial status label
    layout->insertSpacing(1,20);                                // Index 1 - spacing
    // Next - Index 2: Other status label
    otherStatusLabel = new QLabel(this);
    otherStatusLabel->setMinimumWidth(200);
    otherStatusLabel->setMaximumWidth(200);
    otherStatusLabel->setMinimumHeight(20);
    otherStatusLabel->setMaximumHeight(20);
    otherStatusLabel->setAlignment(Qt::AlignVCenter);
    otherStatusLabel->setAlignment(Qt::AlignHCenter);
    otherStatusLabel->setText("");
    layout->addWidget(otherStatusLabel,0,Qt::AlignLeft);        // Index 2 - Other status label
    layout->insertSpacing(3,20);                                // Index 3 - spacing
    layout->insertStretch(-1,1);                                // Index 5 - spacing
    // Next - Index 4: Progress bar
    statusProgressBar = new QProgressBar(this);
    statusProgressBar->setTextVisible(false);
    statusProgressBar->setMaximumHeight(20);
    layout->addWidget(statusProgressBar,9);                     // Index 4 - Progress bar
    //layout->insertStretch(-1,1);                                // Index 5 - spacing
    layout->addStrut(30);                                       // Sets the perpendicular dimension to a minimum size
    // This will be a little red/green box to visually represent connection status
    connectFrame = new QFrame(this);
    connectFrame->setStyleSheet("QFrame { background-color: #FF0632; border: 1px solid white; border-radius: 2px; }");
    connectFrame->setMinimumWidth(25);
    connectFrame->setMaximumWidth(25);
    connectFrame->setMinimumHeight(15);
    connectFrame->setMaximumHeight(15);
    layout->insertWidget(0,connectFrame,0,Qt::AlignLeft);
    statusProgressBar->hide();  // Don't show for now
        qApp->processEvents();  // Equivalent of VB DoEvents()
    layout->setContentsMargins(10,0,0,0);   // Left, top, right, bottom - NOT the same as CSS! This puts a bit of padding to the left
    QWidget *container = new QWidget;
    container->setLayout(layout);
    ui->statusBar->addPermanentWidget(container,1);


    // SETUP THE TABS
    // ---------------------------------------------------------------------------------------------------------------------------------->>
      SetupStatusBarLabel();
      SetupControls_RadioTab();
      SetupControls_SoundTab();
      SetupControls_FirmwareTab();


    // Var Array - do this after all setups are done
    // ---------------------------------------------------------------------------------------------------------------------------------->>
      loadVariableDefaults();       // Load defaults into our VarArray
      Variables_to_Controls();      // Now assign the named variables to our controls


    // Go back to first tab of listview in case we got off it during setup
    // ---------------------------------------------------------------------------------------------------------------------------------->>
    // You actually have to set two things - the listview along the side (why I called it West), but programmatically that won't automatically
    // trigger the stacked widget change the way a click will. So we also set the stacked widget explicitly.
      ui->listViewWest->setCurrentIndex(listViewWestModel->index(0,0));
      ui->stackedWidgetMain->setCurrentIndex(0);
      qApp->processEvents();  // Equivalent of VB DoEvents()


    // Finally, now we're all loaded and cozy-comfy: parse any command line arguments
    // ---------------------------------------------------------------------------------------------------------------------------------->>
      // But wait! Don't process them until the window has loaded.
      // "A QTimer with a timeout of 0 will time out as soon as all the events in the window system's event queue have been processed"
      QTimer::singleShot(0, this, SLOT(ProcessCommandLineArgs()));

}


//------------------------------------------------------------------------------------------------------------------------>>
// COMMAND LINE ARGUMENTS
//------------------------------------------------------------------------------------------------------------------------>>
void MainWindow::ProcessCommandLineArgs()
{
    // At present we don't have much in the way of command line options configured, and there really isn't much need.
    // But our Windows installer will associate ".opz" files with the OPConfig application. Opz files are XML settings files
    // used to save TCB settings to a computer as backup. What we have done in the installer (Inno Setup) is to cause any
    // double-click on an .opz file in Windows to execute opconfig.exe with a command line option -f followed by the path and
    // name of the opz file. For example, clicking on C:\mysettings.opz would execute:
    // opconfig -f C:\mysettings.opz

    // So we do want to parse the -f option, get the file name, and pass it to our readSettingsFromFile routine (mainwindow_file_rw.cpp)

    // If future command line arguments are created in the future, we would check for their presence here and create whatever logic
    // is needed to handle them.

    // The QCommandLineParser adds some convenient functionality for checking command line flags and values. We will only be using
    // a very small portion of what it can actually do, which includes command line help, etc...
    // QCommandLineParser: http://doc.qt.io/qt-5/qcommandlineparser.html
    // QCommandLineOption: http://doc.qt.io/qt-5/qcommandlineoption.html
    QCommandLineParser parser;

    // Create an -f option with a value (filename) and call it loadSettingsFileOption. They can use syntax "-f" or "--file"
    // - First argument of this version of QCommandLineOption is a string list of the various ways they can write the option (f or file)
    // - Second is the description ("Read settings from file.")
    // - Third is the name we give to the value the user will pass for this option (filename)
    QCommandLineOption loadSettingsFileOption(QStringList() << "f" << "file",
                                              "Read settings from file.",
                                              "filename");

    // Now add this option to our parser
    parser.addOption(loadSettingsFileOption);

    // Process the actual command line arguments given by the user. Here we could pass the app (if we were in main.cpp),
    // or it also allows us to simply pass a QStringList of arguments.
    parser.process(QCoreApplication::arguments());

    // Check if the -f flag was even used (we named it loadSettingsFileOption)
    if (parser.isSet(loadSettingsFileOption))
    {
        // It was, now retrieve the value that came with it.
        QString filename = (parser.value(loadSettingsFileOption));
        filename.simplified(); // trim white space

        // Ok - if they passed a file name, send it to the readSettingsFromFile function (mainwindow_file_rw.cpp)
        if (filename != "")
        {
            readSettingsFromFile(filename, true);
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------>>
// FORM - WINSPARKLE AUTO UPDATER
//------------------------------------------------------------------------------------------------------------------------>>
// WinSparkle is an application used to check for and install updates
// https://github.com/vslavik/winsparkle
void MainWindow::initWinSparkle()
{
    // Setup updates feed. This must be done before win_sparkle_init(), but
    // could also be accomplished more conveniently using a Windows resource file (.rc)
    // We use resources.rc (look for it in the project directory, or in the headers section of Qt Creator)

    //If these functions aren't called by the app, the URL is obtained from
    //Windows resource named "FeedURL" of type "APPCAST".
//    win_sparkle_set_appcast_url("http://openpanzer.org/downloads/opconfig/latestrelease/appcast.xml");
//    win_sparkle_set_app_details(L"openpanzer.org", L"OP Config Update Example", L"1.0");

    // This specifies whether automatic checks should occur. We do not want automatic checks because it
    // is a nuisance to the user. Instead they can check manually using the option in the help menu.
    win_sparkle_set_automatic_check_for_updates(0);

    // Give it the callback function that lets it know if it's safe to shutdown the application
    win_sparkle_set_can_shutdown_callback(&canShutdown);

    // Give it the callback function that *will* shutdown the app
    win_sparkle_set_shutdown_request_callback(&shutDown);

    // Initialize the updater and possibly show some UI
    win_sparkle_init();
}
void MainWindow::checkForUpdates()
{   // This does the actual update check.
    // We use the "without" version. It differs from the "with" option only in that
    // it doesn't give the user to "skip this update." Of course they can always cancel it
    // and do it later. EDIT: actually doesn't seem to make any difference, the option is still
    // there. I think it's confusing but whatever.
    win_sparkle_check_update_with_ui();
}
void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    // showEvent() is called right *before* the window is shown, but WinSparkle
    // requires that the main UI of the application is already shown when
    // calling win_sparkle_init() (otherwise it could show its updates UI
    // behind the app instead of at top). By using a helper signal, we delay
    // calling initWinSparkle() until after the window was shown.

    // The windowWasShown signal is connected to the initWinSparkle() function in the constructor of MainWindow
    emit windowWasShown();
}
int MainWindow::canShutdown()
{
    // We always return true because there's nothing to wait on.
    return 1;
}
void MainWindow::shutDown()
{
    QApplication::quit();
}


//------------------------------------------------------------------------------------------------------------------------>>
// FORM - GENERAL
//------------------------------------------------------------------------------------------------------------------------>>
MainWindow::~MainWindow()
{   // Destructor. Cleanup WinSparkle too.
    win_sparkle_cleanup();
    delete ui;
}
void MainWindow::changeStackedWidget(const QModelIndex& current, const QModelIndex& )
{
    // This function changes the current tab when the user clicks on an item on the left

    // EDIT: Although we can prevent the user from leaving the tab, due to the CSS on the listView
    // I can't figure out how to prevent the other tabs from being highlighted, which results in a
    // confusing condition where the tab we are on is not the tab highlighted...
    // I guess I will leave it and let the user change tabs during flashing. There isn't too much they can get in
    // trouble with, and the flashing doesn't care what tab you're on.
/*    // If we are in the midst of a flashing operation, do not change
    if (AttemptFlash && current.row() != TAB_INDEX_FIRMWARE)
    {
        ui->listViewWest->setCurrentIndex(listViewWestModel->index(TAB_INDEX_FIRMWARE,0));
        // Now we simply exit without changing the stackedWidget
        return;
    }
*/
    // Set the stacked widget to the page passed by current index
    ui->stackedWidgetMain->setCurrentIndex(current.row());
    // Set focus to any old thing so there isn't an ugly looking square around the text we just clicked
    ui->stackedWidgetMain->setFocus();

    // Likewise if wer'e moving off the Firmware tab and we were snooping, we stop the snooping because it's of no use
    // if we can't see it, and while snooping is enabled we can't connect, which is annoying if you are on another tab
    // and want to connect
    if (ui->stackedWidgetMain->currentIndex() != TAB_INDEX_FIRMWARE && comm->isSnooping())
    {
        comm->closeSerial();
    }
}
// Uncomment the below if compiling in Qt 5.6
/*void MainWindow::changeStackedWidget56Fix(const QModelIndex& current)
{
    // In Qt 5.6, there is a bug that will highlight both the current listView item and the one after it, whenever
    // you click on it. I was able to work around this by adding a second signal from listViewWest clicked that
    // calls this slot. It causes a flicker because the second item is still selected, but then this one de-selects it.
    ui->listViewWest->setCurrentIndex(current);
    qApp->processEvents();  // Equivalent of VB DoEvents()
}*/
void MainWindow::ShowHideHeader()
{
    if (ui->frmHeader->isHidden())
    {
        ui->frmHeader->show();
        setMinimumHeight(788);
        resize(width(), 788);
    }
    else
    {
        // Hide the "OPEN PANZER OP CONFIG" header bar along the top of the application window. This can be useful
        // for those with small screens
        ui->frmHeader->hide();
        setMinimumHeight(733);
        resize(width(), 733);
    }
}
void MainWindow::cmdTest1_Click()
{
    qDebug() << "Test 1 button clicked";
}
void MainWindow::cmdTest2_Click()
{
    qDebug() << "Test 2 button clicked";
}


//------------------------------------------------------------------------------------------------------------------------>>
// FORM CONTROLS - TOP SECTION - COM, BAUD
//------------------------------------------------------------------------------------------------------------------------>>
void MainWindow::startCOMChecker(void)
{   // This is a repetitive timer that will keep triggering over and over until stopped
    // Each time it trips it will run the pollCOMPorts() function below (setup by signals and slots
    // in MainWindow:MainWindow constructor)
    if (COMCheckTimer->isActive() == false)
    {
        COMCheckTimer->start(CHECK_COM_PORTS_TIME);
    }
}
void MainWindow::stopCOMChecker(void)
{
    COMCheckTimer->stop();
}
void MainWindow::pollCOMPorts()
{
    // We only update the COM ports list while not connected
    if (!comm->isConnected()) fillPortsInfo();
}
void MainWindow::fillPortsInfo()
{
    // This updates the COM port dropdown with all the detected COM ports. The user can refresh the list by clicking the
    // button next to the COM port box.
    QString currentCOM = ui->cboCOMPorts->currentText();

    ui->cboCOMPorts->clear();
    ui->cboCOMPorts->setInsertPolicy(QComboBox::InsertAlphabetically);

    QStringList list;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        list.append(info.portName());
    }
    list.sort();    // Keep them in order
    for (int i = 0; i < list.size(); ++i)
        ui->cboCOMPorts->addItem(list.at(i));

    // Set the selection back to the previous entry if it still exists
    if (currentCOM != "" && ui->cboCOMPorts->findText(currentCOM) != -1)
    {
        ui->cboCOMPorts->setCurrentIndex(ui->cboCOMPorts->findText(currentCOM));
    }
}
void MainWindow::setCOMPort()
{   // Whenever the COM port changes, update the comm object
    // We also include the baud rate
    QSerialPort::BaudRate br = static_cast<QSerialPort::BaudRate>(ui->cboConsoleBaud->itemData(ui->cboConsoleBaud->currentIndex()).toInt());
    comm->updatePortSettings(ui->cboCOMPorts->currentText(),  br);
}
void MainWindow::setBaudRate()
{   // Whenever the Baud rate changes, update the comm object
    // We also include the COM port
    QSerialPort::BaudRate br = static_cast<QSerialPort::BaudRate>(ui->cboConsoleBaud->itemData(ui->cboConsoleBaud->currentIndex()).toInt());
    comm->updatePortSettings(ui->cboCOMPorts->currentText(), br);
}



//------------------------------------------------------------------------------------------------------------------------>>
// FORM CONTROLS - SERIAL STATUS LABEL (In bottom status bar), INITIAL CONNECTION FIRMWARE CHECKS
//------------------------------------------------------------------------------------------------------------------------>>

void MainWindow::SerialStatus_SetAttemptConnect()
{
    serialStatusLabel->setText(tr("Attempting to connect to %1 (%2)...") .arg(comm->currentPortSettings.name).arg(comm->currentPortSettings.stringBaudRate));
        qApp->processEvents();  // Equivalent of VB DoEvents()
}
void MainWindow::SerialStatus_SetNotConnected()
{
    serialStatusLabel->setText(tr("Not connected"));
    // Red box
    connectFrame->setStyleSheet("QFrame { background-color: #FF0632; border: 1px solid white; border-radius: 2px; }");
}
void MainWindow::SerialStatus_SetConnected()
{
    serialStatusLabel->setText(tr("Connected to %1 (%2)") .arg(comm->currentPortSettings.name).arg(comm->currentPortSettings.stringBaudRate));
    // Green box
    connectFrame->setStyleSheet("QFrame { background-color: #01D826; border: 1px solid white; border-radius: 2px; }");
}
void MainWindow::SerialStatus_SetAttemptFlash()
{
    serialStatusLabel->setText(tr("Attempting to flash..."));
    // Neutral box - we don't know yet if successful or not
    connectFrame->setStyleSheet("QFrame { background-color: #E1E3E3; border: 1px solid white; border-radius: 2px; }");
            qApp->processEvents();  // Equivalent of VB DoEvents()
}



//------------------------------------------------------------------------------------------------------------------------>>
// FORM - "OTHER" STATUS LABEL (in bottom status bar)
//------------------------------------------------------------------------------------------------------------------------>>
void MainWindow::SetupStatusBarLabel()
{
    // We need a timer for the status label
    statusLabelShowTimer = new QTimer(this);

    // When the timer expires, fade out the status label
    connect(statusLabelShowTimer, SIGNAL(timeout()), this, SLOT(FadeOutStatusLabel()));
}
void MainWindow::SetStatusLabel(QString t, StatusLabelStatus status=slNeutral)
{
    // This function sets the text of the label and the background color (green for good, red for bad), then
    // it fades in the label, shows it for some length of time, and finally fades it back out.

    // Set the text of the status label
    otherStatusLabel->setText(t);
        qApp->processEvents();

    // Set the background color
    switch (status)
    {
        case slGood:    // Green
            otherStatusLabel->setStyleSheet("background: #006600; border: 1px solid #000; color: #FFF; padding-top: 2px;");
            break;

        case slBad:     // Red
            otherStatusLabel->setStyleSheet("background: #990000; border: 1px solid #000; color: #FFF; padding-top: 2px;");
            break;

        case slNeutral: // No background
        default:
            otherStatusLabel->setStyleSheet("background: transparent; border: 1px solid #FFF; color: #FFF; padding-top: 2px;");
    }

    // Fade it in slowly
    QWidget *w = otherStatusLabel;
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    w->setGraphicsEffect(eff);
    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
    a->setDuration(STATUS_LABEL_FADEIN_TIME);
    a->setStartValue(0);
    a->setEndValue(1);
    a->setEasingCurve(QEasingCurve::InBack);
    w->show();
    a->start(QPropertyAnimation::DeleteWhenStopped);
    connect(a,SIGNAL(finished()),this,SLOT(StartStatusLabelOnDelay()));
}
void MainWindow::StartStatusLabelOnDelay()
{
    statusLabelShowTimer->setSingleShot(true);
    statusLabelShowTimer->start(STATUS_LABEL_ON_DELAY);
}
void MainWindow::FadeOutStatusLabel()
{
    // Fade out the label
    QWidget *w = otherStatusLabel;
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(this);
    w->setGraphicsEffect(eff);
    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
    a->setDuration(STATUS_LABEL_FADEOUT_TIME);
    a->setStartValue(1);
    a->setEndValue(0);
    a->setEasingCurve(QEasingCurve::OutBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);
}



//------------------------------------------------------------------------------------------------------------------------>>
// ABSTRACT ACTIONS
//------------------------------------------------------------------------------------------------------------------------>>
void MainWindow::initActionsConnections()
{
    // File Menu
    ui->actionOpenFile->setEnabled(true);
    ui->actionSaveFile->setEnabled(true);
    ui->actionExit->setEnabled(true);
    connect(ui->actionOpenFile, SIGNAL(triggered()), this, SLOT(actionReadSettingsFromFile()));
    connect(ui->actionSaveFile, SIGNAL(triggered()), this, SLOT(writeSettingsToFile()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

    // Tools Menu
    ui->actionResetAllVals->setEnabled(true);
    connect(ui->actionResetAllVals, SIGNAL(triggered()), this, SLOT(ResetAllValues()));
    ui->actionShowHeader->setEnabled(true);
    connect(ui->actionShowHeader, SIGNAL(triggered()), this, SLOT(ShowHideHeader()));

    // Help Menu
    ui->actionHelp_Contents->setEnabled(true);
    ui->actionHelp_Contents->setShortcut(QKeySequence::HelpContents);   // F1 is the shortcut key
    connect(ui->actionHelp_Contents, SIGNAL(triggered()), this, SLOT(showDocumentation()));

    ui->actionCheck_for_Updates->setEnabled(true);
    connect(ui->actionCheck_for_Updates, SIGNAL(triggered(bool)), this, SLOT(checkForUpdates()));

    ui->actionAbout_OP_Config->setEnabled(true);
    connect(ui->actionAbout_OP_Config, SIGNAL(triggered()), this, SLOT(AboutOP()));

}



//------------------------------------------------------------------------------------------------------------------------>>
// ABSTRACT ACTIONS
//------------------------------------------------------------------------------------------------------------------------>>
void MainWindow::ResetAllValues()
{
    // This sets all controls to the same defaults the program is initalized with on open.
    if (msgBox("Are you sure you want to reset every setting to default?", vbYesNo, "Reset all Settings?", vbQuestion) == QMessageBox::Yes)
    {
        loadVariableDefaults();       // Load defaults into our VarArray
        Variables_to_Controls();      // Now assign the named variables to our controls
    }
}



//------------------------------------------------------------------------------------------------------------------------>>
// ASSISTANT - HELP DOCUMENTATION
//------------------------------------------------------------------------------------------------------------------------>>
void MainWindow::showDocumentation()
{   // This opens the assistant to the start page
    assistant->showDocumentation("index.html");
}
void MainWindow::showPageDocumentation(QString file)
{   // This opens the assistant to the specified link
    assistant->showDocumentation(file);
}
// This function will map all our help buttons to specific locations in the help files
void MainWindow::SetupHelpButtons()
{
    ptrHelpButton = ui->hpbRadio;               // Point to any one of our help buttons
    ptrHelpButton->PassAssistant(assistant);    // Pass a pointer to the assistant object to our ptrHelpButton, only needs to be done once.

    // Create a signal mapper so we can map the released() signal of each button to another signal that includes a string value
    // This mapped signal will be connected to the showPageDocumentation function which takes a string address value to open
    // the documentation to a specific location
    QSignalMapper *signalMapper = new QSignalMapper(this);

    // Connect all the help buttons to the signal mapper
    // Radio tab
        connect(ui->hpbRadio, SIGNAL(released()), signalMapper, SLOT(map()));               // Radio setup
    // Sounds tab
        connect(ui->hpbGeneralSound, SIGNAL(released()), signalMapper, SLOT(map()));        // General sound options
        connect(ui->hpbSoundSqueaks, SIGNAL(released()), signalMapper, SLOT(map()));        // Squeak section
        connect(ui->hpbSoundVolumes, SIGNAL(released()), signalMapper, SLOT(map()));        // Relative volumes
    // Firmware tab
        connect(ui->hpbFirmware, SIGNAL(released()), signalMapper, SLOT(map()));            // Firmware
        connect(ui->hpbConsole, SIGNAL(released()), signalMapper, SLOT(map()));             // Console


    // Now set the individual string values (html pages) to map each button to
    // Radio tab
        signalMapper->setMapping(ui->hpbRadio, "radio.html");                               // Radio setup
    // Sounds
        signalMapper->setMapping(ui->hpbGeneralSound, "sounds.html");                       // Sound tab
        signalMapper->setMapping(ui->hpbSoundSqueaks, "sounds.html#squeaks");               // Squeak settings
        signalMapper->setMapping(ui->hpbSoundVolumes, "sounds.html#volumes");               // Relative volumes
    // Firmware
        signalMapper->setMapping(ui->hpbFirmware, "firmware.html#flash");                   // Firmware
        signalMapper->setMapping(ui->hpbConsole, "firmware.html#console");                  // Console

    // Now connect the mapped signals to the showPageDocumentation() function
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(showPageDocumentation(QString)));
}



//------------------------------------------------------------------------------------------------------------------------>>
// FORM - MESSAGE BOX
//------------------------------------------------------------------------------------------------------------------------>>
// The typing required to display a simple message box is ridiculous. This function along with the enums defined at the top
// of mainwindow.h make it much quicker for most options
int MainWindow::msgBox(QString msg, ButtonCollection buttons=vbOkOnly, QString title="Title", IconCollection icon=vbNoIcon)
{
    switch (buttons)
    {
        case vbYesNo:
            mb->setStandardButtons(QMessageBox::Yes|QMessageBox::No);
            break;
        case vbYesNoCancel:
            mb->setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
            break;
        case vbOkCancel:
            mb->setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
            break;
        default:
            mb->setStandardButtons(QMessageBox::Ok);
    }

    switch (icon)
    {
        case vbInformation:
            mb->setIcon(QMessageBox::Information);
            break;
        case vbCritical:
            mb->setIcon(QMessageBox::Critical);
            break;
        case vbWarning:
        case vbExclamation:
            mb->setIcon(QMessageBox::Warning);
            break;
        case vbQuestion:
            mb->setIcon(QMessageBox::Question);
            break;
        default:
            mb->setIcon(QMessageBox::NoIcon);
    }
    QString finaltitle = " ";   // we add a space at the beginning of the title to move it away a bit from the icon
    finaltitle.append(title);
    mb->setWindowTitle(finaltitle);
    // For some ridiculous reason, you can not style message box text using CSS as you can with the other properties.
    // We have to create an HTML element around the text to set font color.
    QString mText;
    mText = "<p style=""color:#000;"">";
        mText.append(msg);
    mText.append("</p>");
    mb->setText(mText);
    return mb->exec();
}


//------------------------------------------------------------------------------------------------------------------------>>
// ABOUT DIALOG
//------------------------------------------------------------------------------------------------------------------------>>
void MainWindow::AboutOP()
{
    QMessageBox about;

    QFile file(":/css/msgbox.qss");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        about.setStyleSheet(file.readAll());
        file.close();
    }

    // Even though I apply a css stylesheet, it seems to have limited effect, so I also use inline-styles below.
    // The version info comes from version.h
    QString infoText = "<span style='font-size: 12px;'>Version ";
    infoText.append(VER_PRODUCTVERSION_STR);
    infoText.append("<br /><br />&#169; ");
    infoText.append(VER_COPYRIGHT_YEAR_STR);
    infoText.append(" <a href='http://www.openpanzer.org/' style='color: #330055; border-bottom: 1px solid #330055; background: #E7E0EB;'>OpenPanzer.org</a></span>");

    about.setWindowTitle("  OP Sound");
    about.setTextFormat(Qt::RichText);   //this is what makes the links clickable
    about.setText("An ini-file creator utility for the&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<br />Open Panzer Sound Card.");
    //about.setInformativeText("<span style='font-size: 12px;'>Version 1.0<br /><br />&#169; 2016 <a href='http://www.openpanzer.org/' style='color: #330055; border-bottom: 1px solid #330055; background: #E7E0EB;'>OpenPanzer.org</a></span>");
    about.setInformativeText(infoText);
    about.setStandardButtons(QMessageBox::Ok);
    about.setIconPixmap(QPixmap(":/images/AboutIcon.png"));
    about.setDefaultButton(QMessageBox::Ok);
    about.show();
    about.exec();
}



//------------------------------------------------------------------------------------------------------------------------>>
// MOUSE ICON
//------------------------------------------------------------------------------------------------------------------------>>
void MainWindow::MouseWait(void)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
}

void MainWindow::MouseRestore(void)
{
    QApplication::restoreOverrideCursor();
}



