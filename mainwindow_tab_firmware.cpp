#include "mainwindow.h"
#include "ui_mainwindow.h"


//------------------------------------------------------------------------------------------------------------------------>>
// FORM CONTROLS - FIRMWARE TAB (AVRDUDE)
//------------------------------------------------------------------------------------------------------------------------>>

void MainWindow::SetupControls_FirmwareTab(void)
{
    GotWebHex = false;          // We haven't gotten a hex from the internet yet
    WebHexFilePath = "";        // We don't have a local path to it either
    ClearFirmwareVersion(DownloadedVersion);    // We don't know what version is out there
    DownloadedVersionDate = ""; // Don't know the date of the downloaded version either
    AttemptFlash = false;       // We aren't attempting to flash yet
    ui->lblHexVersion->hide();  // Don't show this label yet
    ui->ltxtHexPath->clear();   // Clear hex path textbox
    ui->cmdWebHexPicker->setChecked(false);

    ui->chkAutoscroll->setChecked(true);    // Default to autoscroll the console window

    // Downloaders
    VersionDownloader = new Downloader(this);
    connect(VersionDownloader, SIGNAL(downloadSuccess()), this, SLOT(checkHexVersion()));
    connect(VersionDownloader, SIGNAL(downloadFailed()), this, SLOT(SaveWebHexFailed()));
    HexDownloader = new Downloader(this);
    connect(HexDownloader, SIGNAL(downloadSuccess()), this, SLOT(SaveWebHexToLocal()));
    connect(HexDownloader, SIGNAL(downloadFailed()), this, SLOT(SaveWebHexFailed()));

    // Button signals and slots - flashing
    connect(ui->cmdFlashHex, SIGNAL(clicked(bool)), this, SLOT(cmdFlashHex_clicked()));
    connect(ui->cmdLocalHexPicker, SIGNAL(clicked(bool)), this, SLOT(getLocalHex()));
    connect(ui->cmdWebHexPicker, SIGNAL(clicked(bool)), this, SLOT(getWebHex()));

    // Teensy Loader process and signals
    TeensyLoaderProcess = new QProcess(this);
    connect(TeensyLoaderProcess, SIGNAL(started()), this, SLOT(flashStarted()));
    connect(TeensyLoaderProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(flashFinished()));
    connect(TeensyLoaderProcess,SIGNAL(readyReadStandardOutput()),this,SLOT(readyReadStandardOutput()));
    connect(TeensyLoaderProcess, SIGNAL(readyReadStandardError()), this, SLOT(readyReadStandardError()));
    // We also use a timer to exit the Teensy flash operation if no chip is detected after a certain amount of time,
    // otherwise it can hang forever (there is no intrinsic timeout feature the way there is with AVRDUDE)
    TeensyTimeoutTimer = new QTimer(this);
    connect(TeensyTimeoutTimer, SIGNAL(timeout()), this, SLOT(stopTeensyTimer()));

    // Console slots and signals
    connect(ui->cmdSnoop, SIGNAL(clicked(bool)), this, SLOT(toggleSnoop()));
    connect(ui->cmdClearConsole, SIGNAL(clicked(bool)), this, SLOT(ClearConsole()));
    connect(comm, SIGNAL(SnoopingChanged(boolean)), this, SLOT(ShowSnoopStatus(boolean)));
}
void MainWindow::clearGotHex()
{
    GotWebHex = false;
    ClearConsole();                             // Remove text from the console window
    ui->ltxtHexPath->clear();                   // Clear the path text box
    ui->lblHexVersion->clear();                 // Clear version label
    WebHexFilePath = "";                        // We don't know the final path
    ClearFirmwareVersion(DownloadedVersion);    // We don't know the version
    DownloadedVersionDate = "";                 // Or this
}
void MainWindow::getWebHex()
{
QString versionfile;
    // This attempts to download a hex file from one of the Open Panzer firmware folders on the web
    // depending on the device selected. Inside the firmware folder should always be two files,
    // version.txt and a hex file. URLs to these files are defined at the top of MainWindow.h

    // We use this to skip downloading the file if we've already downloaded it this session
    // It is not likely the release changed in the last few minutes. However for testing when
    // the release is changing every five seconds, this can get to be annoying...
    if (GotWebHex)
    {
        // We already downloaded the hex file
        ui->cmdWebHexPicker->setChecked(false); // Uncheck button
        ui->ltxtHexPath->setText(WebHexFilePath);
        ui->ltxtHexPath->setCursorPosition(0);
        ui->lblHexVersion->show();
        if (!isFirmwareVersionEmpty(DownloadedVersion))
        {
            QString version = "Version: ";
            version.append(FirmwareVersionToString(DownloadedVersion));
            if (DownloadedVersionDate != "") version.append("       Date: " + DownloadedVersionDate);
            ui->lblHexVersion->setText(version);
            // Show the special message again
            QString stat = "Release ";
            stat.append(FirmwareVersionToString(DownloadedVersion));
            stat.append(" downloaded");
            SetStatusLabel(stat, slGood);
        }
        else
        {
            ui->lblHexVersion->setText("Version Unavailable!");
            SetStatusLabel("Latest release downloaded", slGood); // Well, it's not entirely good since we don't know the version...
        }
        return;
    }
    else
    {
        // Ok, we are going to try to download the hex file from the web
        // First, init some stuff
        clearGotHex();
        ui->cmdWebHexPicker->setChecked(true);      // Check our button
        ui->lblHexVersion->setText("Connecting to OpenPanzer.org...");
        ui->cmdWebHexPicker->setEnabled(false);     // And prevent user from clicking it
        MouseWait();

        // Now we kick off the download. It is two part, and we start off by trying to get the version file.
        // Release version URLs are defined at the top of op_defines.h
        versionfile = LATEST_RELEASE_VERSION_URL_TEENSYSOUND;
        VersionDownloader->startDownload(versionfile);
    }
}
void MainWindow::checkHexVersion()
{   // If we make it to here, we successfully retrieved the version.txt file from the web.
    // The contents are now held in a byte array member of the VersionDownloader
    QString strIncoming = QString(VersionDownloader->downloadedData());

    // The file should be two lines long
    QStringList strList = strIncoming.split(QRegExp("\n|\r\n|\r"), QString::SkipEmptyParts);
    QString strVer;
    QString strDate;

    // The version should be on the first line, and the release date on the second line
    if (strList.size() > 0) strVer = strList.at(0);
    if (strList.size() > 1) strDate = strList.at(1);

    DownloadedVersion= DecodeVersion(strVer);
    if (!isFirmwareVersionEmpty(DownloadedVersion))
    {
        QString version = "Version: ";
        version.append(strVer);

        // Check if date is present as well
        if (strDate != "")
        {
            DownloadedVersionDate = strDate;
            version.append("       Date: " + strDate);
        }

        // Now show the version/date
        ui->lblHexVersion->setText(version);
        ui->lblHexVersion->show();

        // We could also show release notes in the console textbox if we add them to the file after line 2
        if (strList.size() > 2)
        {
            ui->txtConsole->clear();
            ui->txtConsole->setText(strIncoming);
            // If Autoscroll is checked, keep the text scrolled down
            if (ui->chkAutoscroll->isChecked()) ui->txtConsole->verticalScrollBar()->setValue(ui->txtConsole->verticalScrollBar()->maximum());
        }
    }
    else
    {
        // If this happens, we should actually probably abort the remainder of the download...
        // But we don't because we're crazy like that.
        ui->lblHexVersion->setText("Version Unavailable!");
        ui->lblHexVersion->show();
    }

    // Now we have the version, let's download the actual hex file
    //Hex URL is defined in MainWindow.h
    QString hexfile;
    hexfile = LATEST_RELEASE_HEX_URL_TEENSYSOUND;
    HexDownloader->startDownload(hexfile);
}
void MainWindow::SaveWebHexToLocal()
{   // If we make it to here, we successfully downloaded the device hex file from the web.
    // The contents are now held in a byte array member of the HexDownloader.
    // We will save it to a file on the disk

    // This is the path and file name that we want to save the file to
    QString formattedVersion = FirmwareVersionToString(DownloadedVersion);
    formattedVersion.replace(".","-");  // When saving the file name, we changed "." to "-"
    QString hexFileFolder = QString("%1/firmware/").arg(QCoreApplication::applicationDirPath());
    QString hexFilePath;
    hexFilePath = hexFileFolder + QString("OPSOUND_%1.hex").arg(formattedVersion);

    //QString hexFilePath = QString("%1/firmware/OPSOUND_%2.hex").arg(QCoreApplication::applicationDirPath()).arg(formattedVersion);

    // Make sure the firmware folder exists as a sub-directory of the application directory path
    if (!QDir(hexFileFolder).exists())
    {
        QDir().mkdir(hexFileFolder);
    }

    // If it already exists, delete it
    if (QFile::exists(hexFilePath))
    {
         QFile::remove(hexFilePath);
         while(QFile::exists(hexFilePath)) { /* wait */ }
    }

    // Now create a new file and save our hex data to it
    QFile *file = new QFile(hexFilePath);
    if(file->open(QFile::Append))
    {
        file->write(HexDownloader->downloadedData());
        file->flush();
        file->close();

        // Ok, we successfully saved the file
        delete file;            // We are not deleting a file, we are deleting this variable that we don't need
        GotWebHex = true;       // Set this flag to true so we don't have to download it again so long as the program remains open and device unchanged
        WebHexFilePath = hexFilePath;
        ui->ltxtHexPath->setText(hexFilePath);  // Show the path.
        ui->ltxtHexPath->setCursorPosition(0);

        if (isFirmwareVersionEmpty(DownloadedVersion))
        {
            SetStatusLabel("Latest release downloaded", slGood); // Well, it's not entirely good since we don't know the version...
        }
        else
        {
            QString stat = "Release ";
            stat.append(FirmwareVersionToString(DownloadedVersion));
            stat.append(" downloaded");
            SetStatusLabel(stat, slGood);
        }
        MouseRestore();
    }
    else
    {
        MouseRestore();
        msgBox("Hex file downloaded, but unable to save it. Please try again.",vbOkOnly, "Unable to Save",vbExclamation);
        // Hide the version label, it's useless now
        ui->lblHexVersion->hide();
        ui->ltxtHexPath->clear();
        GotWebHex = false;
    }
    ui->cmdWebHexPicker->setText("Get Latest Release"); // Restore button text
    ui->cmdWebHexPicker->setChecked(false);             // Uncheck
    ui->cmdWebHexPicker->setEnabled(true);              // Re-enable
}
void MainWindow::SaveWebHexFailed()
{   // If we make it here, either the version or the hex download failed. .
    MouseRestore();
    msgBox("The latest release was unable to be fetched. Please try again later.",vbOkOnly, "Failed Web Retrieve", vbExclamation);
    ui->lblHexVersion->hide();      // Hide the version label, it's useless now
    ui->ltxtHexPath->clear();       // Clear the path text
    GotWebHex = false;              // We do NOT have the file
    WebHexFilePath = "";            // There is no path to the non-existent file
    ClearFirmwareVersion(DownloadedVersion);    // Clear the version
    DownloadedVersionDate = "";     // And its date
    ui->cmdWebHexPicker->setText("Get Latest Release"); // Restore button text
    ui->cmdWebHexPicker->setChecked(false);             // Uncheck
    ui->cmdWebHexPicker->setEnabled(true);              // Re-enable
}
void MainWindow::ClearFirmwareVersion(FirmwareVersion fv)
{
    fv.Major = 0;
    fv.Minor = 0;
    fv.Patch = 0;
    (void)fv; // To shut up compiler warnings
}
boolean MainWindow::isFirmwareVersionEmpty(FirmwareVersion fv)
{
    if (fv.Major > 0 || fv.Minor > 0 || fv.Patch > 0) return false;
    else return true;
}
boolean MainWindow::FirmwareGreaterThanComparison(FirmwareVersion FVCheck, FirmwareVersion FVCompare)
{
    // Returns true if check version is greater than compare version
    if      (FVCheck.Major > FVCompare.Major) return true;
    else if (FVCheck.Major < FVCompare.Major) return false;
    else // Same major version
    {
        if      (FVCheck.Minor > FVCompare.Minor) return true;
        else if (FVCheck.Minor < FVCompare.Minor) return false;
        else // Same major and minor version
        {
            if   (FVCheck.Patch > FVCompare.Patch) return true;
            else                               return false;
        }
    }
}
FirmwareVersion MainWindow::DecodeVersion(QString strVersion)
{
    QByteArray qba;
    qba = strVersion.toUtf8();
    return DecodeVersion(qba);
}
FirmwareVersion MainWindow::DecodeVersion(QByteArray qbav)
{
    // String should come in format of xx.yy.zz where
    // xx is the Major revision number,
    // yy is the Minor revision number,
    // zz is the Patch number
    FirmwareVersion fv;
    uint8_t num;

    ClearFirmwareVersion(fv);   // Clear to start

    if (qbav.size() == 8 && qbav[2] == '.' && qbav[5] == '.')           // Format xx.xx.xx
    {
        if (isCharNumeric(qbav[0])) // Double digit major
        {
            num = qbav[0] - '0';
            fv.Major = num * 10;
        }
        if (isCharNumeric(qbav[1])) fv.Major += qbav[1] - '0';

        if (isCharNumeric(qbav[3]))
        {
            num = qbav[3] - '0';
            fv.Minor = num * 10;
        }
        if (isCharNumeric(qbav[4])) fv.Minor += qbav[4] - '0';

        if (isCharNumeric(qbav[6]))
        {
            num = qbav[6] - '0';
            fv.Patch = num * 10;
        }
        if (isCharNumeric(qbav[7])) fv.Patch += qbav[7] - '0';
    }
    else if (qbav.size() == 7 && qbav[1] == '.' && qbav[4] == '.')      // Format x.xx.xx
    {
        if (isCharNumeric(qbav[0])) // Single digit major
        {
            num = qbav[0] - '0';
            fv.Major = num;
        }

        if (isCharNumeric(qbav[2]))
        {
            num = qbav[2] - '0';
            fv.Minor = num * 10;
        }
        if (isCharNumeric(qbav[3])) fv.Minor += qbav[3] - '0';

        if (isCharNumeric(qbav[5]))
        {
            num = qbav[5] - '0';
            fv.Patch = num * 10;
        }
        if (isCharNumeric(qbav[6])) fv.Patch += qbav[6] - '0';
    }
    if (fv.Major > 99) fv.Major = 99;
    if (fv.Minor > 99) fv.Minor = 99;
    if (fv.Patch > 99) fv.Patch = 99;

    return fv;
}
boolean MainWindow::isCharNumeric(char c)
{
    if (c >= '0' && c <= '9') return true;
    else return false;
}
QString MainWindow::FirmwareVersionToString(FirmwareVersion fv)
{   // This will return a string of the firmware version with periods in the correct place
    // and numbers padded with zero if they are less than 10 (except for major)

    QString sfv;
    ClearFirmwareVersion(fv);

    sfv.append(QString::number(fv.Major));
    sfv.append(".");
    if (fv.Minor < 10)  sfv.append("0");
    sfv.append(QString::number(fv.Minor));
    sfv.append(".");
    if (fv.Patch < 10)  sfv.append("0");
    sfv.append(QString::number(fv.Patch));

    return sfv;
}
void MainWindow::getLocalHex()
{   // This lets the user select a hex file on their local system
    QString fileName;
    QString directory = ".";

    // If a hex file is already specified in the text box, we will try to open the file dialog chooser in the same folder.
    // This makes it easy for us to hit the "Get Latest Release", have it download, then click the Manual button to view
    // that folder.
    QString existingHex = ui->ltxtHexPath->text();
    if (!existingHex.isEmpty())
    {
        QFileInfo fi(existingHex);
        directory = fi.absolutePath();
    }

    fileName = QFileDialog::getOpenFileName(this, tr("Select Hex File"),directory,tr("Hex files (*.hex)"));
    if (!fileName.isEmpty())
    {
        ui->ltxtHexPath->setText(fileName);
        ui->ltxtHexPath->setCursorPosition(0);
        ui->lblHexVersion->hide();  // We won't know what version this is, so hide the label
    }
    else ui->ltxtHexPath->clear();
}
void MainWindow::cmdFlashHex_clicked()
{
QString program;
QStringList arguments;
QString flagVerbose;
QString flagProgrammer;
QString flagBaud;
QString hex;

    // If we are already in the middle of flashing, exit. This shouldn't actually occur.
    // Also if we are in the middle of a connection attempt, exit then too. That can occur if the
    // user clicks enough buttons.
    if (AttemptFlash || AttemptConnect)
    {
        ui->cmdFlashHex->setChecked(false);     // Don't let the button check
        return;                                 // Exit
    }

    // Make sure the user has selected a hex file. Whether they selected it locally or downloaded it first,
    // either way it is a local file now. The path should be in the plain text box:
    QString HexFile = ui->ltxtHexPath->text();
    //HexFile = "D:/UAV/QT/Projects/build-OpenPanzerConfig-Desktop_Qt_5_4_2_MSVC2013_64bit-Release/release/firmware/TCBMK1_00-91-01.hex";
    if(HexFile.isEmpty())
    {
        msgBox("No file selected",vbOkOnly,"Select File First",vbInformation);
        ui->cmdFlashHex->setChecked(false);     // Uncheck button
        return;
    }

    // Make sure the file actually exists
    //qDebug() << "Hex file check " << HexFile;
    //qDebug() << "QFile::exists(HexFile) = " << QFile::exists(HexFile);
    if (!QFile::exists(HexFile))
    {
        msgBox("File doesn't exist! Please re-check.",vbOkOnly,"No File",vbExclamation);
        ui->cmdFlashHex->setChecked(false);     // Uncheck button
        return;
    }

    // Ok, let's roll.

    MouseWait();        // Hourglass the cursor

    // Before we start the flash, we need to take care of some things.
    AttemptFlash = true;       // Set the attempt flag

    // If are presently connected, we need to disconnect because if Qt has the COM port, TeensyLoader won't be able to access it
    if (comm->isSnooping()) comm->closeSerial();

    // Set the serial status label
    SerialStatus_SetAttemptFlash();

    // Don't let the user click Flash again until we are done
    ui->cmdFlashHex->setChecked(true);  // "Check" (depress) the button
    ui->cmdFlashHex->setText("wait");   // Set text to "Wait"
    ui->cmdFlashHex->setEnabled(false); // Disable (can't click)

    // Also disable the Hex/Snoop/ClearScreen/COM/Baud/Connect controls
    ui->cmdLocalHexPicker->setEnabled(false);
    ui->cmdWebHexPicker->setEnabled(false);
    ui->cmdSnoop->setEnabled(false);
    ui->cmdClearConsole->setEnabled(false);
    ui->cboCOMPorts->setEnabled(false);
    ui->cboConsoleBaud->setEnabled(false);

    // .arg(QCoreApplication::applicationDirPath()); returns the release dev folder:
    // "path/QT/Projects/build-OpenPanzerSound_Qt_5_4_2_MinGW_32bit-Release/release/
    // So for testing copy the Teensy loader files there

    // Now construct our command line arguments
    // We will use PJRC's command-line version of Teensy Loader to flash the Teensy 3.2 chip on the Sound Card.
    // No baud or COM port settings are required.
    // Construct our Teensy Loader executable and list of arguments.
    // Don't put any spaces in the argument list, or it won't work.
    program = QString("%1/teensyloader/teensy_loader_cli ").arg(QCoreApplication::applicationDirPath());       // Teensy loader exe file

    //teensy_loader_cli -mmcu=mk20dx256 -w blink_slow_Teensy32.hex
    QString flagMCU =       "-mmcu=mk20dx256";  // -mmcu - Teensy 3.2 (mk20dx256)
    QString flagWait =      "-w";               // -w wait - Wait for device to appear. When the pushbuttons has not been pressed and
                                                //           HalfKay may not be running yet, this option makes teensy_loader_cli wait.
                                                //           It is safe to use this when HalfKay is already running. The hex file is
                                                //           read before waiting to verify it exists, and again immediately after the device is detected.
    flagVerbose =           "-v";               // -v Verbose output

    // Assemble
    arguments.clear();
    arguments << flagMCU << flagWait << flagVerbose << HexFile;

    // Clear text box and our output string
    ui->txtConsole->clear();
    strTeensyLoaderOutput.clear();
    // qDebug() << "Program:" << program << " Args: " << arguments;

    startTeensyTimer(); // Start the timeout timer
    TeensyLoaderProcess->setProcessChannelMode(QProcess::MergedChannels);
    TeensyLoaderProcess->start(program, arguments);
}
void MainWindow::startTeensyTimer()
{
    TeensyTimeoutTimer->setSingleShot(true);
    TeensyTimeoutTimer->start(10000);   // Is 10 seconds too long?
}
void MainWindow::stopTeensyTimer()
{
    TeensyLoaderProcess->kill();
}
void MainWindow::flashStarted()
{
    //    qDebug() << "Flash Started";
}
void MainWindow::flashFinished()
{
    MouseRestore();

    //qDebug() << TeensyLoaderProcess->exitCode() << " - " << TeensyLoaderProcess->exitStatus();
    if (TeensyLoaderProcess->exitCode() == 0)
    {
        // Flash successful
        SetStatusLabel("Firmware update successful",slGood);
        ui->txtConsole->append("SUCCESS!"); // The console message is not very helpful, add one.
    }
    else
    {
        // Flash failed
        // timeout (killed) will return code 62097
        // Typically this will be because the user didn't press the button and the operation timed out

        // In this case, the last words in the console will still be "Hint - press the button" or some such, not very helpful
        // now. Add a failure message
        ui->txtConsole->append("FLASH FAILED.");

        msgBox("Flash operation failed. Remember to push the button on your Sound Card after the flash operation starts!",vbOkOnly,"Flash Failed", vbExclamation);
        SetStatusLabel("Firmware update failed!",slBad);
    }

    // Kill that bitch, make sure it's really dead.
    TeensyLoaderProcess->kill();

    // Set the button back
    ui->cmdFlashHex->setChecked(false);  // Un-check the button
    ui->cmdFlashHex->setText("Flash");   // Revert text
    ui->cmdFlashHex->setEnabled(true);  // Enable

    // We can also re-enable these
    ui->cmdLocalHexPicker->setEnabled(true);
    ui->cmdWebHexPicker->setEnabled(true);
    ui->cmdSnoop->setEnabled(true);
    ui->cmdClearConsole->setEnabled(true);
    ui->cboCOMPorts->setEnabled(true);
    ui->cboConsoleBaud->setEnabled(true);

    // Clear the flag
    AttemptFlash = false;

    // We always end the flash operation disconnected (because we started it disconnected)
    SerialStatus_SetNotConnected();
}
void MainWindow::readyReadStandardOutput()
{
    // Append output to our string
    strTeensyLoaderOutput.append(TeensyLoaderProcess->readAllStandardOutput());

    // Put string in textbox
    ui->txtConsole->setText(strTeensyLoaderOutput);
    // If Autoscroll is checked, keep the text scrolled down
    if (ui->chkAutoscroll->isChecked()) ui->txtConsole->verticalScrollBar()->setValue(ui->txtConsole->verticalScrollBar()->maximum());

    // Now some output is bad news. But we haven't done enough testing yet to know what it would be.
    // It's hard to screw this one up!
//    if (strTeensyLoaderOutput.contains("SOME BAD NEWS", Qt::CaseInsensitive))
//    {
//        TeensyLoaderProcess->kill();
//    }
}
void MainWindow::readyReadStandardError()
{
    // Nothing will come here if we are using setProcessChannelMode(QProcess::MergedChannels)
    // Merged channels means error and standard output are combined into one stream, read by readyReadStandardOutput
}


//------------------------------------------------------------------------------------------------------------------------>>
// FORM CONTROLS - FIRMWARE TAB - CONSOLE FUNCTIONALITY
//------------------------------------------------------------------------------------------------------------------------>>
void MainWindow::toggleSnoop()
{
    // First, we ignore any clicks if we are in the middle of flashing
    if (AttemptFlash)
    {
        ui->cmdSnoop->setChecked(false);    // Don't let the button check
        return;                             // Exit
    }

    if (comm->isSnooping())
    {
        comm->closeSerial();
    }
    else
    {
        ui->cmdSnoop->setChecked(true);     // "Check" (depress) the button, but disable it and set the text to wait
        ui->cmdSnoop->setText("wait");
        ui->cmdSnoop->setEnabled(false);
        MouseWait();
        ClearConsole();                     // Clear the console
        AttemptConnect = true;              // Set a flag to indicate a connection attempt is underway
        if (!comm->isConnected()) SerialStatus_SetAttemptConnect();  // Set the serial status label if we aren't already connected
        comm->Snoop();  // This will determine if the port is already open or not, and take the correct action
    }
}
void MainWindow::ClearConsole()
{
    // Clear the console text box
    ui->txtConsole->clear();
}
void MainWindow::ShowSnoopStatus(boolean snooping)
{
    AttemptConnect = false; // The attempt is over, we know now whether we are actually connected or not.
    MouseRestore();

    if (snooping)
    {
        QString qs = "Snooping ";
        qs.append(ui->cboCOMPorts->currentText());
        SetStatusLabel(qs,slNeutral);
        ui->cmdSnoop->setChecked(true);                 // "Check" Snoop button
            ui->cmdSnoop->setText("Stop Snooping");   // Change text to "Stop Snooping"
                ui->cmdSnoop->setEnabled(true);

        // We also don't want the user changing the COM port and Baud rate while snooping, although it wouldn't hurt if they did
        ui->cboCOMPorts->setEnabled(false);
        ui->cboConsoleBaud->setEnabled(false);

        // Update serial status Label
        SerialStatus_SetConnected();
    }
    else
    {
        // Snooping is over, or connection failed. When snooping ends, by definition we are done snooping, and we are also disconnected
        if (!AttemptFlash)  SetStatusLabel("COM port closed",slNeutral);   // If we quit snooping to try flashing, we don't need a warning
        ui->cmdSnoop->setChecked(false);            // Uncheck Snoop button
            ui->cmdSnoop->setText("Snoop");         // Change text back to "Snoop"
                ui->cmdSnoop->setEnabled(true);

        ui->cboCOMPorts->setEnabled(true);
        ui->cboConsoleBaud->setEnabled(true);

        // Update the serial status Label - but not if we quit snooping to try flashing, in that case
        // the flashing routine will set its own message.
        if (!AttemptFlash) SerialStatus_SetNotConnected();
    }
}
void MainWindow::putDataToConsole(const QByteArray &data)
{
    ui->txtConsole->insertPlainText(QString(data));
    // If Autoscroll is checked, keep the text scrolled down
    if (ui->chkAutoscroll->isChecked()) ui->txtConsole->verticalScrollBar()->setValue(ui->txtConsole->verticalScrollBar()->maximum());
}
