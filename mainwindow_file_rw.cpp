#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::loadVariableDefaults(void)
{
    // Clear/default all channel data to begin with
    for (int i = 0; i < CHANNELS; i++)
    {
        DeviceData.RCChannel[i].Digital = true;
        DeviceData.RCChannel[i].chType = CT_SWITCH;
        DeviceData.RCChannel[i].numPositions = 2;
        DeviceData.RCChannel[i].reversed = false;
        for (int j = 0; j < MAX_SWITCH_POSITIONS; j++)
        {
            DeviceData.RCChannel[i].swFunctionID[j] = SF_NULL;          // Raw ID
            DeviceData.RCChannel[i].swFunction[j] = SF_NULL;            // Actual function
            DeviceData.RCChannel[i].actionNum[j] = 0;
            DeviceData.RCChannel[i].switchAction[j] = ACTION_OFFSTOP;
        }
    }

    // Channel 1 - Engine speed
    DeviceData.RCChannel[0].Digital = false;
    DeviceData.RCChannel[0].chType = CT_ENGINE_SPEED;

    // Channel 2 - start and stop engine
    DeviceData.RCChannel[1].numPositions = 3;
    // Position 1
    DeviceData.RCChannel[1].swFunctionID[0] = 10000;            // Raw ID
    DeviceData.RCChannel[1].swFunction[0] = SF_ENGINE_START;    // Actual function
    // Position 3
    DeviceData.RCChannel[1].swFunctionID[2] = 20000;            // Raw ID
    DeviceData.RCChannel[1].swFunction[2] = SF_ENGINE_STOP;     // Actual function

    // Channel 3 - Cannon and MG
    DeviceData.RCChannel[2].numPositions = 3;
    // Position 1
    DeviceData.RCChannel[2].swFunctionID[0] = 40001;            // Raw ID
    DeviceData.RCChannel[2].swFunction[0] = SF_CANNON_FIRE;     // Actual function
    DeviceData.RCChannel[2].actionNum[0] = 1;                   // Cannon 1
    // Position 2                                            // Center position
    DeviceData.RCChannel[2].swFunctionID[1] = 50202;            // Raw ID
    DeviceData.RCChannel[2].swFunction[1] = SF_MG;              // Actual function
    DeviceData.RCChannel[2].actionNum[1] = 2;                   // MG 2
    DeviceData.RCChannel[2].switchAction[1] = ACTION_OFFSTOP;   // Stop playing the MG
    // Position 3
    DeviceData.RCChannel[2].swFunctionID[2] = 50102;            // Raw ID
    DeviceData.RCChannel[2].swFunction[2] = SF_MG;              // Actual function
    DeviceData.RCChannel[2].actionNum[2] = 2;                   // MG 2
    DeviceData.RCChannel[2].switchAction[2] = ACTION_ONSTART;   // Start playing the MG

    // Channel 4 - play user sound 1 and 2
    DeviceData.RCChannel[3].numPositions = 3;
    // Position 1
    DeviceData.RCChannel[3].swFunctionID[0] = 70101;            // Raw ID
    DeviceData.RCChannel[3].swFunction[0] = SF_USER;            // Actual function
    DeviceData.RCChannel[3].actionNum[0] = 1;
    DeviceData.RCChannel[3].switchAction[0] = ACTION_ONSTART;
    // Position 3
    DeviceData.RCChannel[3].swFunctionID[2] = 70102;            // Raw ID
    DeviceData.RCChannel[3].swFunction[2] = SF_USER;            // Actual function
    DeviceData.RCChannel[3].actionNum[2] = 2;
    DeviceData.RCChannel[3].switchAction[2] = ACTION_ONSTART;

    // Channel 5 - Volume
    DeviceData.RCChannel[4].Digital = false;
    DeviceData.RCChannel[4].chType = CT_VOLUME;

    // Squeak Defaults
    DeviceData.Squeak[0].MinInterval_mS = 1500;
    DeviceData.Squeak[0].MaxInterval_mS = 4000;
    DeviceData.Squeak[1].MinInterval_mS = 2000;
    DeviceData.Squeak[1].MaxInterval_mS = 5000;
    DeviceData.Squeak[2].MinInterval_mS = 3000;
    DeviceData.Squeak[2].MaxInterval_mS = 8000;
    DeviceData.Squeak[3].MinInterval_mS = 1000;
    DeviceData.Squeak[3].MaxInterval_mS = 2000;
    DeviceData.Squeak[4].MinInterval_mS = 3000;
    DeviceData.Squeak[4].MaxInterval_mS = 4000;
    DeviceData.Squeak[5].MinInterval_mS = 5000;
    DeviceData.Squeak[5].MaxInterval_mS = 6000;
    for (uint8_t i=0; i<NUM_SQUEAKS; i++) { DeviceData.Squeak[i].Enabled = false; }
    DeviceData.MinSqueakSpeedPct = 25;

    // Volume defaults
    DeviceData.VolumeEngine = 50;
    DeviceData.VolumeTrackOverlay = 50;
    DeviceData.VolumeEffects = 50;

    // Throttle position
    DeviceData.centerThrottle = true;
    DeviceData.idleDeadband = 5;

    // Lights
    DeviceData.Light1FlashTime_mS = 30;
    DeviceData.Light1BlinkOnTime_mS = 30;
    DeviceData.Light1BlinkOffTime_mS = 30;
    DeviceData.Light2FlashTime_mS = 30;
    DeviceData.Light2BlinkOnTime_mS = 30;
    DeviceData.Light2BlinkOffTime_mS = 30;
    DeviceData.Light3FlashTime_mS = 30;
    DeviceData.Light3BlinkOnTime_mS = 30;
    DeviceData.Light3BlinkOffTime_mS = 30;

    // Sound bank
    DeviceData.SoundBankA_Loop = false;
    DeviceData.SoundBankB_Loop = false;

    // Servo
    DeviceData.ServoReversed = false;
    DeviceData.ServoTimeToRecoil = 200;
    DeviceData.ServoTimeToReturn = 800;
    DeviceData.ServoEndPointRecoiled = 100;
    DeviceData.ServoEndPointBattery = 100;

    // Engine
    DeviceData.EngineAutoStart = false;
    DeviceData.EngineAutoStopTime_mS = 0;
}


//------------------------------------------------------------------------------------------------------------------------>>
// UTILITY - Create a function ID from switch function, switch action, and action number
//------------------------------------------------------------------------------------------------------------------------>>
uint32_t MainWindow::switchSettingsToID(switch_function sf, switch_action sa, uint8_t num)
{
    return ((multiplier_switchfunction * sf) + (multiplier_switchaction * sa) + num);
}

//------------------------------------------------------------------------------------------------------------------------>>
// FILE - READING AND WRITING
//------------------------------------------------------------------------------------------------------------------------>>
// This is just the Device menu action, and we simply get a file path/name from the user, then pass it to the actual reading function
void MainWindow::actionReadSettingsFromFile()
{
    const QString desktopFolder = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString filename = QFileDialog::getOpenFileName(this, tr("Import Settings from INI File"), desktopFolder, tr("INI files (*.ini)"));

    if (filename == "") return;     // Exit if they cancel
    else readSettingsFromFile(filename, false);

}

// This is the actual reading function, it takes a file path/name as argument.
void MainWindow::readSettingsFromFile(QString filename, boolean confirm)
{
    if (filename == "") return;

    // QFile wants separators to be "/", not "\", see http://doc.qt.io/qt-5/qfile.html
    //filename.replace(QString("\\"),QString("/"));     // Use replace, or
    filename = QDir::fromNativeSeparators(filename);    // This built-in function does it
    filename.remove("\n");
    filename = filename.trimmed();

    // Had this happening even after using trimmed sometimes when comming from command line, caused infinite headaches debugging...
    if (filename.left(1) == " ")
        filename = filename.right(filename.length()-1);

    filename = filename.trimmed();                      // Once again for damn good measure

    // Load file into QSettings object
    QSettings settings(filename, QSettings::IniFormat);
    QStringList keys;

    // Clear/default all channel data to begin with
    for (int i = 0; i < CHANNELS; i++)
    {
        DeviceData.RCChannel[i].Digital = true;
        DeviceData.RCChannel[i].chType = CT_SWITCH;
        DeviceData.RCChannel[i].numPositions = 2;
        DeviceData.RCChannel[i].reversed = false;
        for (int j = 0; j < MAX_SWITCH_POSITIONS; j++)
        {
            DeviceData.RCChannel[i].swFunctionID[j] = SF_NULL;          // Raw ID
            DeviceData.RCChannel[i].swFunction[j] = SF_NULL;            // Actual function
            DeviceData.RCChannel[i].actionNum[j] = 0;
            DeviceData.RCChannel[i].switchAction[j] = ACTION_OFFSTOP;
        }
    }

    // Channel function
    // -------------------------------------------------------------------------------------------------------------->>
    settings.beginGroup("channel_func");
    keys = settings.childKeys();
    for (int i = 0; i < keys.size(); ++i)
    {
        QString key = keys.at(i).toLocal8Bit().constData();
        int ch = -9;
        if (key == "ch1_func") ch = 0;
        if (key == "ch2_func") ch = 1;
        if (key == "ch3_func") ch = 2;
        if (key == "ch4_func") ch = 3;
        if (key == "ch5_func") ch = 4;

        if (ch >= 0 and ch < CHANNELS)
        {
            DeviceData.RCChannel[ch].Digital = !static_cast<bool>(settings.value(key, 0).toUInt());
            DeviceData.RCChannel[ch].chType = static_cast<channel_type>(settings.value(key,0).toUInt());
        }
    }
    settings.endGroup();

    // Channel reversed
    // -------------------------------------------------------------------------------------------------------------->>
    settings.beginGroup("channel_reversed");
    keys = settings.childKeys();
    for (int i = 0; i < keys.size(); ++i)
    {
        QString key = keys.at(i).toLocal8Bit().constData();
        int ch = -9;
        if (key == "ch1_reverse") ch = 0;
        if (key == "ch2_reverse") ch = 1;
        if (key == "ch3_reverse") ch = 2;
        if (key == "ch4_reverse") ch = 3;
        if (key == "ch5_reverse") ch = 4;

        if (ch >= 0 and ch < CHANNELS)
        {
            DeviceData.RCChannel[ch].reversed = static_cast<bool>(settings.value(key, 0).toUInt());
        }
    }
    settings.endGroup();

    // Switch position counts for switch channels
    // -------------------------------------------------------------------------------------------------------------->>
    settings.beginGroup("channel_pos_count");
    keys = settings.childKeys();
    for (int i = 0; i < keys.size(); ++i)
    {
        QString key = keys.at(i).toLocal8Bit().constData();
        int ch = -9;
        if (key == "ch1_pos") ch = 0;
        if (key == "ch2_pos") ch = 1;
        if (key == "ch3_pos") ch = 2;
        if (key == "ch4_pos") ch = 3;
        if (key == "ch5_pos") ch = 4;

        if (ch >= 0 and ch < CHANNELS)
        {   // Only if the channel is a switch
            if (DeviceData.RCChannel[ch].Digital)
            {   // Constrain position to valid values
                uint pos = settings.value(key, 0).toUInt();
                if (pos < MIN_SWITCH_POSITIONS) pos = MIN_SWITCH_POSITIONS;
                if (pos > MAX_SWITCH_POSITIONS) pos = MAX_SWITCH_POSITIONS;
                DeviceData.RCChannel[ch].numPositions = pos;
            }
            else
            {   // If not a switch channel, set the numPositions to 0
                DeviceData.RCChannel[ch].numPositions = 0;
            }
        }
    }
    settings.endGroup();

    // Switch functions
    // -------------------------------------------------------------------------------------------------------------->>
    settings.beginGroup("channel_pos_triggers");
    keys = settings.childKeys();
    for (int i = 0; i < keys.size(); ++i)
    {   // The key will hold the channel number and switch position data (example 24 means channel 2, switch position 4)
        QString key = keys.at(i).toLocal8Bit().constData();
        int keynum = QString(key).toInt();
        uint8_t ch = keynum / 10;
        uint8_t pos = keynum - (ch*10);
        if (ch > CHANNELS) ch = CHANNELS;
        if (ch > 0) ch -= 1;    // 0-based
        if (pos < 1) pos = 1;
        if (pos > MAX_SWITCH_POSITIONS) pos = MAX_SWITCH_POSITIONS;
        // qDebug() << "Channel " << ch << " - Switch Position " << pos;

        // Only if this channel is a switch with number of positions at least as high as this position
        if (DeviceData.RCChannel[ch].Digital && DeviceData.RCChannel[ch].numPositions >= pos)
        {   // Now unpack the function ID which will be the value associated with this key
            int32_t funcID = settings.value(key).toInt();
            // We need position to be zero-based from here on
            pos -= 1;
            // Save function ID (raw number)
            DeviceData.RCChannel[ch].swFunctionID[pos] = funcID;
            // Now unpack the function ID into its component parts (function, action, action number)
            uint16_t func = (funcID / multiplier_switchfunction);
            DeviceData.RCChannel[ch].swFunction[pos] = static_cast<switch_function>(func); // Switch function
            uint8_t act = (funcID - (func * multiplier_switchfunction)) / multiplier_switchaction;
            DeviceData.RCChannel[ch].switchAction[pos] = static_cast<switch_action>(act);
            uint8_t num = (funcID - (func * multiplier_switchfunction) - (act * multiplier_switchaction));
            DeviceData.RCChannel[ch].actionNum[pos] = num;
            // qDebug() << "Channel " << ch + 1 << " Switch Position " << pos + 1 << " Function " << func << " action " << act << " number " << num;
        }
    }
    settings.endGroup();

    // Squeak settings
    // -------------------------------------------------------------------------------------------------------------->>
    settings.beginGroup("squeak_settings");
    keys = settings.childKeys();
    for (int i = 0; i < keys.size(); ++i)
    {
        QString key = keys.at(i).toLocal8Bit().constData();
        int val = settings.value(key, 0).toInt();

        for (uint8_t i=0; i<NUM_SQUEAKS; i++)
        {
            if (key == QString("s%1min").arg(i+1)) DeviceData.Squeak[i].MinInterval_mS = val;
            if (key == QString("s%1max").arg(i+1)) DeviceData.Squeak[i].MaxInterval_mS = val;
            if (key == QString("s%1en").arg(i+1))  DeviceData.Squeak[i].Enabled = bool(val);
            if (key == "minspeed")                 DeviceData.MinSqueakSpeedPct = val;
        }
    }
    settings.endGroup();

    // Volumes
    // -------------------------------------------------------------------------------------------------------------->>
    settings.beginGroup("volumes");
    keys = settings.childKeys();
    for (int i = 0; i < keys.size(); ++i)
    {
        QString key = keys.at(i).toLocal8Bit().constData();
        if (key == "engine")    DeviceData.VolumeEngine  = settings.value(key, 0).toInt();;
        if (key == "effects")   DeviceData.VolumeEffects = settings.value(key, 0).toInt();;
        if (key == "overlay")   DeviceData.VolumeTrackOverlay = settings.value(key, 0).toInt();;
    }
    settings.endGroup();

    // Throttle
    // -------------------------------------------------------------------------------------------------------------->>
    settings.beginGroup("throttle");
    keys = settings.childKeys();
    for (int i = 0; i < keys.size(); ++i)
    {
        QString key = keys.at(i).toLocal8Bit().constData();
        if (key == "centerthrottle") DeviceData.centerThrottle = settings.value(key, 0).toBool();
        if (key == "idledeadband")   DeviceData.idleDeadband   = settings.value(key, 0).toUInt();
    }
    settings.endGroup();

    // Lights
    // -------------------------------------------------------------------------------------------------------------->>
    settings.beginGroup("lights");
    keys = settings.childKeys();
    for (int i = 0; i < keys.size(); ++i)
    {
        QString key = keys.at(i).toLocal8Bit().constData();
        if (key == "l1_flash")      DeviceData.Light1FlashTime_mS  = settings.value(key, 0).toUInt();
        if (key == "l1_blinkon")    DeviceData.Light1BlinkOnTime_mS = settings.value(key, 0).toUInt();
        if (key == "l1_blinkoff")   DeviceData.Light1BlinkOffTime_mS = settings.value(key, 0).toUInt();
        if (key == "l2_flash")      DeviceData.Light2FlashTime_mS  = settings.value(key, 0).toUInt();
        if (key == "l2_blinkon")    DeviceData.Light2BlinkOnTime_mS = settings.value(key, 0).toUInt();
        if (key == "l2_blinkoff")   DeviceData.Light2BlinkOffTime_mS = settings.value(key, 0).toUInt();
        if (key == "l3_flash")      DeviceData.Light3FlashTime_mS  = settings.value(key, 0).toUInt();
        if (key == "l3_blinkon")    DeviceData.Light3BlinkOnTime_mS = settings.value(key, 0).toUInt();
        if (key == "l3_blinkoff")   DeviceData.Light3BlinkOffTime_mS = settings.value(key, 0).toUInt();
    }
    settings.endGroup();

    // Sound Bank
    // -------------------------------------------------------------------------------------------------------------->>
    settings.beginGroup("soundbank");
    keys = settings.childKeys();
    for (int i = 0; i < keys.size(); ++i)
    {
        QString key = keys.at(i).toLocal8Bit().constData();
        if (key == "SBA_loop") DeviceData.SoundBankA_Loop = settings.value(key, 0).toBool();
        if (key == "SBB_loop") DeviceData.SoundBankB_Loop = settings.value(key, 0).toBool();
    }
    settings.endGroup();

    // Servo
    // -------------------------------------------------------------------------------------------------------------->>
    settings.beginGroup("servo");
    keys = settings.childKeys();
    for (int i = 0; i < keys.size(); ++i)
    {
        QString key = keys.at(i).toLocal8Bit().constData();
        if (key == "servo_reverse") DeviceData.ServoReversed  = settings.value(key, 0).toBool();
        if (key == "time_recoil")   DeviceData.ServoTimeToRecoil = settings.value(key, 0).toUInt();
        if (key == "time_return")   DeviceData.ServoTimeToReturn = settings.value(key, 0).toUInt();
        if (key == "ep_recoil")     DeviceData.ServoEndPointRecoiled = settings.value(key, 0).toUInt();
        if (key == "ep_battery")    DeviceData.ServoEndPointBattery = settings.value(key, 0).toUInt();
    }
    settings.endGroup();

    // Engine
    // -------------------------------------------------------------------------------------------------------------->>
    settings.beginGroup("engine");
    keys = settings.childKeys();
    for (int i = 0; i < keys.size(); ++i)
    {
        QString key = keys.at(i).toLocal8Bit().constData();
        if (key == "autostart")   DeviceData.EngineAutoStart  = settings.value(key, 0).toBool();
        if (key == "autostop")    DeviceData.EngineAutoStopTime_mS = settings.value(key, 0).toUInt();
    }
    settings.endGroup();


    // We've loaded the setting from the file into our named variables,
    // Now we need to copy the named variables to our controls
    Variables_to_Controls();

    SetStatusLabel(QString("Settings imported"),slGood);
    if (confirm)
    {
        msgBox(tr("All settings imported from file:<br><br>%1") .arg(filename), vbOkOnly, "INI File", vbInformation);
    }
}
void MainWindow::writeSettingsToFile()
{
    const QString desktopFolder = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Settings to INI File"),QString("%1/opsound.ini").arg(desktopFolder), tr("INI files (*.ini)"));

    if (filename == "") return; // Exit if they cancel

    // Create a settings object
    QSettings* settingsTo = new QSettings(filename, QSettings::IniFormat);

    // If you don't call this, the "sync" command we use later will keep prior keys, and only add new ones.
    // But of course we want to wipe the file clean first, and the only put in what has been generated this time around.
    settingsTo->clear();

    MouseWait();

    // Before we can write our settings to a file, we need to make sure VarArray is updated with the current values from the form
    Controls_to_Variables();    // This puts the control values into our named variables

    // Now save the variables to our settings object
    for (int i = 0; i < CHANNELS; i++)
    {
        // Channel function
        // -------------------------------------------------------------------------------------------------------------->>
        settingsTo->beginGroup("channel_func");
        settingsTo->setValue(QString("ch%1_func").arg(i+1), DeviceData.RCChannel[i].chType);
        settingsTo->endGroup();

        // Channel reversed
        // -------------------------------------------------------------------------------------------------------------->>
        settingsTo->beginGroup("channel_reversed");
        settingsTo->setValue(QString("ch%1_reverse").arg(i+1), static_cast<uint>(DeviceData.RCChannel[i].reversed));
        settingsTo->endGroup();

        // Switch position counts for switch channels
        // -------------------------------------------------------------------------------------------------------------->>
        settingsTo->beginGroup("channel_pos_count");
        settingsTo->setValue(QString("ch%1_pos").arg(i+1), DeviceData.RCChannel[i].numPositions);
        settingsTo->endGroup();

        // Switch functions
        // -------------------------------------------------------------------------------------------------------------->>
        settingsTo->beginGroup("channel_pos_triggers");
        if (DeviceData.RCChannel[i].Digital)
        {
            for (int j = 0; j < DeviceData.RCChannel[i].numPositions; j++)
            {
                if (DeviceData.RCChannel[i].swFunctionID[j] > 0)
                {
                    settingsTo->setValue(QString::number(i + 1) + QString::number(j + 1), DeviceData.RCChannel[i].swFunctionID[j]);
                    //qDebug() << "Channel " << i+1 << " Pos " << j + 1 << " FunctionID " << DeviceData.RCChannel[i].swFunctionID[j];
                }
            }
        }
        settingsTo->endGroup();

        // Squeak settings
        // -------------------------------------------------------------------------------------------------------------->>
        settingsTo->beginGroup("squeak_settings");
        for (uint8_t i=0; i<NUM_SQUEAKS; i++)
        {
            settingsTo->setValue(QString("s%1min").arg(i+1),DeviceData.Squeak[i].MinInterval_mS);
            settingsTo->setValue(QString("s%1max").arg(i+1),DeviceData.Squeak[i].MaxInterval_mS);
            settingsTo->setValue(QString("s%1en").arg(i+1), static_cast<uint>(DeviceData.Squeak[i].Enabled));
        }
        settingsTo->setValue(QString("minspeed"), DeviceData.MinSqueakSpeedPct);
        settingsTo->endGroup();

        // Load volumes
        // -------------------------------------------------------------------------------------------------------------->>
        settingsTo->beginGroup("volumes");
        settingsTo->setValue(QString("engine"), DeviceData.VolumeEngine);
        settingsTo->setValue(QString("effects"), DeviceData.VolumeEffects);
        settingsTo->setValue(QString("overlay"), DeviceData.VolumeTrackOverlay);
        settingsTo->endGroup();

        // Throttle
        // -------------------------------------------------------------------------------------------------------------->>
        settingsTo->beginGroup("throttle");
        settingsTo->setValue(QString("centerthrottle"), static_cast<uint>(DeviceData.centerThrottle));
        settingsTo->setValue(QString("idledeadband"), DeviceData.idleDeadband);
        settingsTo->endGroup();

        // Lights
        // -------------------------------------------------------------------------------------------------------------->>
        settingsTo->beginGroup("lights");
        settingsTo->setValue(QString("l1_flash"), DeviceData.Light1FlashTime_mS);
        settingsTo->setValue(QString("l1_blinkon"), DeviceData.Light1BlinkOnTime_mS);
        settingsTo->setValue(QString("l1_blinkoff"), DeviceData.Light1BlinkOffTime_mS);
        settingsTo->setValue(QString("l2_flash"), DeviceData.Light2FlashTime_mS);
        settingsTo->setValue(QString("l2_blinkon"), DeviceData.Light2BlinkOnTime_mS);
        settingsTo->setValue(QString("l2_blinkoff"), DeviceData.Light2BlinkOffTime_mS);
        settingsTo->setValue(QString("l3_flash"), DeviceData.Light3FlashTime_mS);
        settingsTo->setValue(QString("l3_blinkon"), DeviceData.Light3BlinkOnTime_mS);
        settingsTo->setValue(QString("l3_blinkoff"), DeviceData.Light3BlinkOffTime_mS);
        settingsTo->endGroup();

        // Sound bank
        // -------------------------------------------------------------------------------------------------------------->>
        settingsTo->beginGroup("soundbank");
        settingsTo->setValue(QString("SBA_loop"), static_cast<uint>(DeviceData.SoundBankA_Loop));
        settingsTo->setValue(QString("SBB_loop"), static_cast<uint>(DeviceData.SoundBankB_Loop));
        settingsTo->endGroup();

        // Servo
        // -------------------------------------------------------------------------------------------------------------->>
        settingsTo->beginGroup("servo");
        settingsTo->setValue(QString("servo_reverse"), static_cast<uint>(DeviceData.ServoReversed));
        settingsTo->setValue(QString("time_recoil"), DeviceData.ServoTimeToRecoil);
        settingsTo->setValue(QString("time_return"), DeviceData.ServoTimeToReturn);
        settingsTo->setValue(QString("ep_recoil"), DeviceData.ServoEndPointRecoiled);
        settingsTo->setValue(QString("ep_battery"), DeviceData.ServoEndPointBattery);
        settingsTo->endGroup();

        // Engine
        // -------------------------------------------------------------------------------------------------------------->>
        settingsTo->beginGroup("engine");
        settingsTo->setValue(QString("autostart"), static_cast<uint>(DeviceData.EngineAutoStart));
        settingsTo->setValue(QString("autostop"), DeviceData.EngineAutoStopTime_mS);
        settingsTo->endGroup();

    }

    // "sync" will force creation of the file
    settingsTo->sync();

    //pctExported = ((double)varCount/(double)totalPossibleVars)*100.0;
    SetStatusLabel(QString("File saved"), slNeutral); //.arg(QString::number(pctImported,'f',0)),slGood); -- no need, we will always write 100%

    MouseRestore();
}
