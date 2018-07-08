#ifndef OPENPANZERCOMM_H
#define OPENPANZERCOMM_H

#include <QtCore/QtGlobal>

#include <QMessageBox>
#include <QtSerialPort/QSerialPort> // Serial port
#include <QString.h>
#include <QDebug.h>
#include <QTimer.h>
#include <arduino_compat.h>         // Gives us Arduino-like data type names
#include <op_devicedata_struct.h>

// We've decided to allow the user to modify the baud rate because we also want to implement a general-purpose
// console. This gives them one extra thing to think about though when connecting to the TCB, so set this to
// the USB_BAUD_RATE setting in OP_Config.h and we will default to it.
#define USB_DEFAULT_BAUD_RATE    QSerialPort::Baud115200

// Communication Errors
#define MAX_ERRORS              10      // How many communication errors allowed before we just disconnect

// Safety
#define SERIAL_COMM_TIMEOUT 	7000	// How many milliseconds of inactivity do we wait before we just disconnect from the computer (1000 ms = 1 second)
#define MAX_COMM_ERRORCOUNT		10		// How many communication errors do we permit before simply disconnecting

// If we also want to include data sent or received in communication mode to the console, set these to true
#define APPEND_SENT_TO_CONSOLE  true
#define APPEND_RECEIVED_TO_CONSOLE  true

// Debugging
#define DEBUG_MSGS              false    // Set to true to have serial comms printed to the debug panel


struct PortSettings {
    QString name;
    QString stringBaudRate;
    //qint32 baudRate;
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QString stringDataBits;
    QSerialPort::Parity parity;
    QString stringParity;
    QSerialPort::StopBits stopBits;
    QString stringStopBits;
    QSerialPort::FlowControl flowControl;
    QString stringFlowControl;
};


// Class OP_PCComm
class OpenPanzerComm : public QObject // By inheriting from QObject, the class can use signal and slot mechanism
{
    Q_OBJECT    // need this macro for QObject to work

    // -------------------------------------------------------------------------------------------------------->>
    // PUBLIC
    // -------------------------------------------------------------------------------------------------------->>
    public:
         explicit OpenPanzerComm();		// Constructor

        // Functions
        void begin();           // Init
        void updatePortSettings(QString comName, QSerialPort::BaudRate);   // For now just updates the COM port and Baud rate (other settings are hard-coded)
        void openSerial_forSnoop(void);  // Open serial port, and that's it.
        void closeSerial(void); // Close serial port and tell device Goodbye
        void Snoop(void);       // This opens the serial port but doesn't attempt to establish communication with the device

        boolean isConnected(void);  // returns connection status
        boolean isSnooping(void);   // returns snooping status

        // Vars
        PortSettings currentPortSettings;


        //public slots:

    // -------------------------------------------------------------------------------------------------------->>
    // SIGNALS
    // -------------------------------------------------------------------------------------------------------->>
    signals:
        void ConnectionChanged(boolean connected);      // Signal for anytime we connect or disconnect
        void SnoopingChanged(boolean snooping);         // Signal for anytime we open or close port for the purpose of snooping
        void CommError(QString, QSerialPort::SerialPortError);                        // Signal for errors that need to be displayer to the user
        void IncrementError(void);                      // Non-critical error handling

    // -------------------------------------------------------------------------------------------------------->>
    // PRIVATE
    // -------------------------------------------------------------------------------------------------------->>
    private:

        // Vars
        // ---------------------------------------------------------------------------------------------------->>
          QSerialPort  *serial;
          int           numErrors;
          boolean       Connected;  // Connected is serial port open and continous comms established with device
          boolean       Snooping;   // Snopping is serial port open and we are just listening (console mode)
          uint8_t       NumErrors;        // How many communication errors have we accumulated since connecting

        // Connection
        // ---------------------------------------------------------------------------------------------------->>
          void setSerialSettings(void);

     // -------------------------------------------------------------------------------------------------------->>
     // PRIVATE SLOTS
     // -------------------------------------------------------------------------------------------------------->>
     private slots:
         // Serial Slots
         // ---------------------------------------------------------------------------------------------------->>
            void handleError(QSerialPort::SerialPortError error);
            void handleNonCriticalError(void);

         // Timer slots
         // ---------------------------------------------------------------------------------------------------->>
           void ProcessConnectionChange(boolean connected);  // If true we start the watchdog timer, if false we clear it.
           void ProcessSnoopingChange(boolean snooping);     //
           void connectionFailed(void);
};


#endif // OPENPANZERCOMM_H

