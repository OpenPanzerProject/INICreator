/* INI Creator 		INI Creator Utility for the Open Panzer Sound Card
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QLabel>
#include <QFrame>
#include <QDebug>
#include <QLineEdit>
#include <QProgressBar>
#include <QStringList>                              // Class of string lists
#include <QStringListModel>                         // String list model, for creating simple text models to populate listviews, etc.
#include <QAbstractItemView>
#include <QtSerialPort/QSerialPortInfo>             // Serial port info, used to populate the drop-down list of COM Ports
#include <QFileDialog.h>
#include <QSignalMapper>
#include <QSortFilterProxyModel>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QTimer>
#include <QProcess>
#include <QScrollBar>
#include <QFileInfo>
#include <QPixmap>
#include <QSpinBox>
#include <QCommandLineParser>
#include <QSettings>
#include <QStandardPaths>
#include <combo_baudrates.h>
#include <combo_channeltype.h>
#include <combo_channelpositions.h>
#include <combo_switchfunctions.h>
#include <combo_actionnum.h>
#include <combo_switchaction.h>
#include <helpbutton.h>                             // Custom push button for help files
#include <openpanzercomm.h>                         // OpenPanzer communication library
#include <assistant.h>
#include <downloader.h>
#include <console.h>
#include <version.h>
#include <winsparkle.h>

// We use this to more quickly create message boxes
enum ButtonCollection{vbYesNo, vbYesNoCancel, vbOkCancel, vbOkOnly};
enum IconCollection{vbCritical, vbInformation, vbQuestion, vbWarning, vbExclamation, vbNoIcon};

// Settings for the status label that we fade in and out
#define STATUS_LABEL_ON_DELAY       3000            // How long to show status messages in the bottom status bar
#define STATUS_LABEL_FADEOUT_TIME   2000            // How long does the fade out effect take
#define STATUS_LABEL_FADEIN_TIME    350             // How long does the fade in effect take
enum StatusLabelStatus{slGood, slBad, slNeutral};   // We use these to decide what color the label should have

// How often to poll the COM ports list (mS)
#define CHECK_COM_PORTS_TIME        800             // Every 0.8 seconds

// Define the index numbers of the various tabs
#define TAB_INDEX_RADIO             0
#define TAB_INDEX_SOUNDS            1
#define TAB_INDEX_FIRMWARE          2

// Struct for firmware version info
struct FirmwareVersion{
    uint8_t Major;
    uint8_t Minor;
    uint8_t Patch;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

// ------------------------------------------------------------------------------------------------------------------------------------------------>>
// PUBLIC
// ------------------------------------------------------------------------------------------------------------------------------------------------>>
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual void showEvent(QShowEvent *event);      // We need this event so we can wait until the main form loads to show WinSparkle

    // These functions will be used as callbacks for the WinSparkle dll and need to be static.
    // WinSparkle is a program we use to check for OP Config updates
    static int canShutdown();                       // Are we ready to shutdown?
    static void shutDown();                         // Shutdown

// ------------------------------------------------------------------------------------------------------------------------------------------------>>
// PUBLIC SLOTS
// ------------------------------------------------------------------------------------------------------------------------------------------------>>
public slots:


    // WinSparkle updater.
    void initWinSparkle();                          // Initialize the WinSparkle app
    void checkForUpdates();                         // The check for updates function


// -------------------------------------------------------------------------------------------------------->>
// SIGNALS
// -------------------------------------------------------------------------------------------------------->>
signals:
    void windowWasShown();                          // Has main form finished loading

    // -------------------------------------------------------------------------------------------------------->>
// PRIVATE SLOTS
// -------------------------------------------------------------------------------------------------------->>
private slots:
    // These are functions that will be called if we connect them to a signal
      void changeStackedWidget(const QModelIndex&, const QModelIndex&);

    // Slots for the Assistant (help documentation) and the About menu
    // ---------------------------------------------------------------------------------------------------->>
      void showDocumentation();
      void About();
      void showPageDocumentation(QString file);

    // Slots for the Tools menu
    // ---------------------------------------------------------------------------------------------------->>
      void ResetAllValues();
      void ShowHideHeader();    // Toggle
      void ShowHeader();
      void HideHeader();

    // Slots for the two test buttons on the Radio tab
    // ---------------------------------------------------------------------------------------------------->>
      void cmdTest1_Click();                        // These slots are in the mainwindow.cpp file under FORM-GENERAL
      void cmdTest2_Click();                        // And the connections are made in the MainWindow constructor


    // Slots for connecting/disconnecting/error handling the device
    // ---------------------------------------------------------------------------------------------------->>
      void fillPortsInfo();                         // Updates the list of detected COM ports
      void pollCOMPorts();                          // Calls fillPortsInfo but only if not already connected
      void setCOMPort();                            // Sets the current COM port
      void setBaudRate();                           // Sets the current baud rate
      void ShowSnoopStatus(boolean connected);      // If the port is open or closed for snooping, this will get called

    // Slots for reading/writing to XML file
    // ---------------------------------------------------------------------------------------------------->>
      void actionReadSettingsFromFile();            // Read all settings from a file - this slot actually only gets the file name, which
                                                    // it then passes to the real readSettingsFromFile(Qstring) private function
      void writeSettingsToFile();                   // Save all settings to a file

    // Form control slots
    // ---------------------------------------------------------------------------------------------------->>
      // Radio tab
      void SaveChannelReversed(bool checked, int);
      void SaveChannelType(ChannelTypeComboBox *, int);
      void SaveChannelPositions(ChannelPositionsComboBox *, int);
      void SaveChannelSwitchAction(SwitchFunctionComboBox *, int , int);

      // Sound tab
      void ShowHideSqueak1Settings(bool);
      void ShowHideSqueak2Settings(bool);
      void ShowHideSqueak3Settings(bool);
      void ShowHideSqueak4Settings(bool);
      void ShowHideSqueak5Settings(bool);
      void ShowHideSqueak6Settings(bool);
      void ShowHideOtherSqueakSettings();
      void UpdateEngineVolumeLabel(int);
      void UpdateOverlayVolumeLabel(int);
      void UpdateEffectsVolumeLabel(int);

      // Firmware tab - hex flashing
      void cmdFlashHex_clicked();
      void flashStarted();
      void flashFinished();
      void readyReadStandardOutput();
      void readyReadStandardError();                // Won't be using this one after all
      void getLocalHex();
      void getWebHex();
      void clearGotHex();
      void checkHexVersion();
      void SaveWebHexToLocal();
      void SaveWebHexFailed();

      // Firmware tab - console
      void ClearConsole();
      void toggleSnoop();
      void putDataToConsole(const QByteArray &data);
      void stopTeensyTimer(void);                   // Teensy flash has timed-out

      // Status bar (along the bottom)
      void StartStatusLabelOnDelay(void);           // During this delay, the status label is visible
      void FadeOutStatusLabel();                    // This slot fades out the status label


// ------------------------------------------------------------------------------------------------------------------------------------------------>>
// PRIVATE
// ------------------------------------------------------------------------------------------------------------------------------------------------>>
private:
    // Forms
    // ---------------------------------------------------------------------------------------------------->>
      Ui::MainWindow *ui;                           // This form

    // Mouse
    // ---------------------------------------------------------------------------------------------------->>
      void MouseWait();                             // Sets the mouse to hourglass
      void MouseRestore();                          // Returns the mouse to a pointer

    // Application-specific settings
    // ---------------------------------------------------------------------------------------------------->>
      QSettings ProgIni;
      QString LastPath;
      QString GetLastPath(void);                    // The app will remember the last path the user accessed
      void StoreLastPath(QString);                  // when reading/writing INI files

    // QT Assistant
    // ---------------------------------------------------------------------------------------------------->>
      Assistant *assistant;

    // Help Buttons
    // ---------------------------------------------------------------------------------------------------->>
      HelpButton *ptrHelpButton;                    // We create a pointer to one so we can pass the reference to Assistant
      QSignalMapper *signalMapper;                  // We'll use a signal mapper to map a string output from each button's released event
      void SetupHelpButtons(void);                  // This will map all the help buttons to the specific location in the help files

    // Message box helper
    // ---------------------------------------------------------------------------------------------------->>
      int msgBox(QString msg, ButtonCollection buttons, QString title, IconCollection icon);
      QMessageBox *mb;

    // Form stuff
    // ---------------------------------------------------------------------------------------------------->>
      void initActionsConnections();                // Set up abstract actions
      QLabel *serialStatusLabel;                    // References to labels and ProgressBar in the Status Bar
      QLabel *otherStatusLabel;
      QFrame *connectFrame;
      QProgressBar *statusProgressBar;
      QStringListModel *listViewWestModel;          // This is a class that provides a model that supplies strings to views (we can use views in listboxes, combo boxes, whatever)

    // Status bar
      void SetupStatusBarLabel();
      void SetStatusLabel(QString, StatusLabelStatus);      // Sets the text and background color, handles the fade-in and fade-out
      QTimer *statusLabelShowTimer;                         // Timer to show the status label

    // Serial status label and colored box
      void SerialStatus_SetAttemptConnect();
      void SerialStatus_SetConnected();
      void SerialStatus_SetNotConnected();
      void SerialStatus_SetAttemptFlash();
      void SerialStatus_Show();
      void SerialStatus_Hide();

    // COM Check timer
      QTimer *COMCheckTimer;             // This timer will be used to poll the COM ports and update the list if any new devices are found
      void startCOMChecker(void);
      void stopCOMChecker(void);

    // Tab-specific
    // ---------------------------------------------------------------------------------------------------->>
      // Radio tab
        void SetupControls_RadioTab(void);
        ChannelTypeComboBox *channelTypeCombo[CHANNELS];
        QCheckBox *reversedCheck[CHANNELS];
        ChannelPositionsComboBox *switchPosCombo[CHANNELS];
        SwitchFunctionComboBox *switchFunctionCombo[CHANNELS * MAX_SWITCH_POSITIONS];
        ActionNumComboBox *actionNumCombo[CHANNELS * MAX_SWITCH_POSITIONS];
        SwitchActionComboBox *switchActionCombo[CHANNELS * MAX_SWITCH_POSITIONS];

        channel_settings ChannelSettings[CHANNELS];

      // Sound tab
        QCheckBox *chkSqueakEnabled[NUM_SQUEAKS];
        QSpinBox *squeakMinInterval[NUM_SQUEAKS];
        QSpinBox *squeakMaxInterval[NUM_SQUEAKS];
        void SetupControls_SoundTab(void);
        void EnableMinSqueakSpeed(void);
        void DisableMinSqueakSpeed(void);

        // Firmware tab
        void SetupControls_FirmwareTab(void);
        FirmwareVersion DownloadedVersion;
        QString DownloadedVersionDate;
        void ClearFirmwareVersion(FirmwareVersion);
        FirmwareVersion DecodeVersion(QString);
        FirmwareVersion DecodeVersion(QByteArray);
        QString FirmwareVersionToString(FirmwareVersion);
        boolean isFirmwareVersionEmpty(FirmwareVersion);
        boolean FirmwareGreaterThanComparison(FirmwareVersion, FirmwareVersion);
        Downloader *VersionDownloader;
        Downloader *HexDownloader;
        boolean AttemptFlash;
        QString strTeensyLoaderOutput;
        QProcess *TeensyLoaderProcess;
        QTimer *TeensyTimeoutTimer;                 // This timer will be used to exit the Teensy flash operation if no chip is detected after a certain amount of time
        void startTeensyTimer(void);
        boolean GotWebHex;                          // If we've downloaded a hex from the web set this flag, so if the user tries again it's instantaneous
        QString WebHexFilePath;                     // If we've downloaded the hex from the web, this is the full path (including file name) where we saved it.
        boolean isCharNumeric(char);                // Is this a character from 0 to 9

    // Variables to Controls and back
    // ---------------------------------------------------------------------------------------------------->>
      void loadVariableDefaults(void);              // Initializes variables to a default value
      void Variables_to_Controls(void);             // These update all the controls with values stored in named variables
      void Controls_to_Variables(void);             // These copy all control values to named variables


    // Named vars (as opposed to numbered variables)
    // ---------------------------------------------------------------------------------------------------->>
      _device_data DeviceData;                      // This struct holds a named version of every piece of information we may want from the device


    // Physical Device Settings
    // ---------------------------------------------------------------------------------------------------->>
      OpenPanzerComm *comm;                         // Our OpenPanzer Communication object
      boolean AttemptConnect = false;               // This will only be true while the connection attempt is in process. Once we are
                                                    // connected or disconnected it will become false, and connection status can be obtained
                                                    // from comm->isConnected()

    // Reading from and writing to INI file
    // ---------------------------------------------------------------------------------------------------->>
      void readSettingsFromFile(QString, boolean);  // Give a file path, it verifies the file exists and tries to read it in. If the second arg is true, will provide confirmation message
      uint32_t switchSettingsToID(switch_function, switch_action, uint8_t);   // Pass a swich function, switch action, and action number, return a FunctionID

};

#endif // MAINWINDOW_H
