/* OP_PCComm.h			Open Panzer PC Communication - a library for talking to a PC, primarily intended for use with the desktop application
 * Version 1.0   		July 2015
 * Copyright 2015		Luke Middleton	openpanzer.org
 *
 * This library provides functions for listening, responding to and executing commands received from a computer application.
 * Primarily this will involve updating all variable stored in EEPROM from settings the user has defined in the desktop app.
 *
 * We use a constructed serial protocol of "sentences" composed of up to four pieces of information, seperated by a delimiter character (we use the pipe char "|")
 * The first two values of the sentence must always be non-zero. These are the "Command" and the "ID"
 * The next two values in the sentence are the "value" to be operated upon, and finally, a "Checksum" of all or part of the sentence
 * In some cases there will be no value relevant to the command being sent, and in that case there won't be an ID either because the ID
 * identifies the value. In those cases, the command is repeated twice, first in the command slot, then in the ID slot, and in those cases
 * the Value and Checksom slots should still be sent but with 0 in them.
 *
 * Some examples in pseudo code:
 *
 * PC tells us to upate a variable in EEPROM
 * -------------------------------------------------------------------------------------------------
 * 124|2212|120|xxx newline
 * 124 		- command to update EEPROM (see definitions of commands below)
 * | 		- delimiter
 * 2212 	- ID of the value to be updated (in our Excel we see this is SmokerFastIdleSpeed)
 * | 		- delimiter
 * 120 		- the new value the EEPROM should be set to
 * | 		- delimiter
 * xxx 		- checksum (not calculated for this example)
 * newline	- newline character, tells us the sentence is over
 *
 * PC tells us to disconnect
 * -------------------------------------------------------------------------------------------------
 * 31|31|0|0 newline
 * 31		- command to tell the Tank Control Board we are done, disconnect
 * |		- delimiter
 * 31		- command repeated in the ID slot
 * |		- delimiter
 * 0		- nothing needed in the value slot, but we still include the slot
 * |		- delimiter
 * 0		- nothing needed in the checksum slot, but we still include the slot
 * newline	- end of sentence
 *
 *
 * Tank Control Board tells PC to repeat the last sentence (something didn't go right with reception)
 * -------------------------------------------------------------------------------------------------
 * 135|135|0|0 newline
 * 135		- command to tell the computer we need the last sentence resent
 * |		- delimiter
 * 135		- command repeated in the ID slot
 * |		- delimiter
 * 0		- nothing needed in the value slot, but we still include the slot
 * |		- delimiter
 * 0		- nothing needed in the checksum slot, but we still include the slot
 * newline	- end of sentence
 *
 * Tank Control Board gives the PC a value (PC must request it first, this would be the response)
 * -------------------------------------------------------------------------------------------------
 * 136|2212|120|xxx newline
 * 136		- command to tell the computer we are giving him a value
 * |		- delimiter
 * 2212		- ID of the value we are sending (should match the ID the computer just requested)
 * |		- delimiter
 * 120		- the value presently in eeprom
 * |		- delimiter
 * 0		- checksum (not calculated in this example)
 * newline	- end of sentence
 *
 *
 * Almost all communication is one sentence each device, back and forth. So PC will send a sentence and wait for TCB to send a sentence,
 * then TCB will wait for the PC to send a sentence, etc... In other words, each device should only send one sentence a time, and
 * refrain from sending any more sentences until it hears back from the other device.
 * The exception to this is if the watchdog timer expires, in which case the TCB will tell the PC it's saying goodbye.
 *
 *
 */


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

