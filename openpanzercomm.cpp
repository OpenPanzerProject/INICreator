#include "openpanzercomm.h"

OpenPanzerComm::OpenPanzerComm()
{
    // Objects
    // ---------------------------------------------------------------------------------------------------------------------------------->>
        serial = new QSerialPort(this);
        serial->setSettingsRestoredOnClose(false);


    // ---------------------------------------------------------------------------------------------------------------------------------->>
    // Signals and Slots
    // ---------------------------------------------------------------------------------------------------------------------------------->>
    // Open Panzer Connection connections
        connect(this, SIGNAL(ConnectionChanged(boolean)), this, SLOT(ProcessConnectionChange(boolean)));

    // Serial connections
        connect(this, SIGNAL(IncrementError()), this, SLOT(handleNonCriticalError()));
        connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
}

void OpenPanzerComm::begin()
{
    Connected = false;
    Snooping = false;
    NumErrors = 0;
}



//------------------------------------------------------------------------------------------------------------------------>>
// PORT: OPEN, CLOSE, ETC
//------------------------------------------------------------------------------------------------------------------------>>
void OpenPanzerComm::updatePortSettings(QString comName, QSerialPort::BaudRate baud)
{
    currentPortSettings.name = comName;
    currentPortSettings.baudRate = baud;
        currentPortSettings.stringBaudRate = QString::number(currentPortSettings.baudRate);
    currentPortSettings.dataBits = QSerialPort::Data8;
        currentPortSettings.stringDataBits = QStringLiteral("8");
    currentPortSettings.parity = QSerialPort::NoParity;
        currentPortSettings.stringParity = tr("None");
    currentPortSettings.stopBits = QSerialPort::OneStop;
        currentPortSettings.stringStopBits = QStringLiteral("1");
    currentPortSettings.flowControl = QSerialPort::NoFlowControl;   // We don't want hardware flow control
        currentPortSettings.stringFlowControl = tr("None");
}

void OpenPanzerComm::setSerialSettings(void)
{
    serial->setPortName(currentPortSettings.name);
    serial->setBaudRate(currentPortSettings.baudRate);
    serial->setDataBits(currentPortSettings.dataBits);
    serial->setParity(currentPortSettings.parity);
    serial->setStopBits(currentPortSettings.stopBits);
    serial->setFlowControl(currentPortSettings.flowControl);
}

void OpenPanzerComm::openSerial_forSnoop(void)
{
    setSerialSettings();    // Make sure these are updated

    // Close before opening, this actually shouldn't happen though.
    // But if it does, we also throw in a "set DTR pin High" just for good measure. (High means don't reset the device.)
    if (serial->isOpen()) { serial->setDataTerminalReady(true); serial->close(); }

    if (serial->open(QIODevice::ReadWrite)) // This statement can take a while to complete
    {

        // Note, this can only be set after the serial port is open.
//        serial->setRequestToSend(true);      // Don't need to worry about RTS
        serial->setDataTerminalReady(true); // True here means "set DTR pin High", ie don't reset device

        // If we are just snooping, all we need to do is open the serial port: which we've already done if we're at this statement.
        Snooping = true;
    }
    else
    {
        Snooping = false;
    }
    // Emit the signal to let us know what happened
    emit SnoopingChanged(Snooping);
}


void OpenPanzerComm::connectionFailed()
{   // We come here if initTimer expires. That means we were unable to connect.

    closeSerial();   // We no longer close the serial port here
}

void OpenPanzerComm::closeSerial()
{
    // Close the port
    if (serial->isOpen())
    {   // Make sure DTR is high at the end, in case it makes any difference next time
        serial->setDataTerminalReady(true); // True here means "set DTR pin High"
        serial->close();
    }

    Connected = false;
    emit ConnectionChanged(Connected);

    if (Snooping)
    {
        Snooping = false;
        emit SnoopingChanged(Snooping);
    }
}

void OpenPanzerComm::ProcessConnectionChange(boolean connected)
{   // This slot gets called any time the connection status changes

    if (connected)
    {
        // Clear the error count
        NumErrors = 0;
    }
}

void OpenPanzerComm::Snoop()
{
   // Here we want to start snooping
   if (Connected)
   {
       // If we are already connected to the device, then the serial port is already open. All we need to do is
       // tell the device to exit communication mode and go into normal operation mode, but we leave the serial port
       // open so we can listen to whatever it says in normal operation mode.

       // Now update flags and emit signals
       Connected = false;               // In this case, Connected = false doesn't mean the port is closed,
       emit ConnectionChanged(false);   // it just means we aren't in communication mode anymore.

       Snooping = true;
       emit SnoopingChanged(true);      // This emits the Snooping true signal
   }
   else
   {   // In this case we want to snoop and the serial port isn't open yet, so we will open it.
       openSerial_forSnoop();           // openSerial_forSnoop() will take care of emitting the correct signals
   }
}

void OpenPanzerComm::ProcessSnoopingChange(boolean) //  snooping)
{
    //
}



//------------------------------------------------------------------------------------------------------------------------>>
// UTILITIES
//------------------------------------------------------------------------------------------------------------------------>>
boolean OpenPanzerComm::isConnected(void)
{
    return Connected;
}

boolean OpenPanzerComm::isSnooping(void)
{
    return Snooping;
}


//------------------------------------------------------------------------------------------------------------------------>>
// ERROR HANDLING
//------------------------------------------------------------------------------------------------------------------------>>

void OpenPanzerComm::handleError(QSerialPort::SerialPortError error)
{
//    if (error == QSerialPort::ResourceError)
//    {
//        qDebug() << serial->errorString();
//    }

    if (error != 0)
    {
        emit CommError(serial->errorString(), serial->error());
        serial->clearError();
    }
}

void OpenPanzerComm::handleNonCriticalError(void)
{
    if (NumErrors > MAX_COMM_ERRORCOUNT)
    {
        closeSerial();
        emit CommError("Too many communication errors occured. Device disconnected.", QSerialPort::NoError);
    }
}


