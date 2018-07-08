#include "mainwindow.h"
#include "ui_mainwindow.h"


// Here we have two routines for setting all controls to the current value of our named variables, or
// setting all named variables to the values specified in the controls.

//------------------------------------------------------------------------------------------------------------------------>>
// VARIABLES TO CONTROLS & CONTROLS TO VARIABLES
//------------------------------------------------------------------------------------------------------------------------>>
void MainWindow::Variables_to_Controls(void)
{

    // Radio tab
    // ---------------------------------------------------------------------------------------------------------->>
    // CHANNELS
    for (int i=0; i<CHANNELS; i++)
    {
        // Number of channel switch positions (if switch)
        switchPosCombo[i]->setCurrentText(QString::number(DeviceData.RCChannel[i].numPositions));
        // Channel type (switch or some other)
        channelTypeCombo[i]->setCurrentIndex(channelTypeCombo[i]->findData(DeviceData.RCChannel[i].chType));
        // Channel reversed settings
        reversedCheck[i]->setChecked(DeviceData.RCChannel[i].reversed);
        SaveChannelType(channelTypeCombo[i],i);
        SaveChannelPositions(switchPosCombo[i],i);

        int start_interval = (MAX_SWITCH_POSITIONS * i);
        for (int j=0; j<MAX_SWITCH_POSITIONS; j++)
        {
            // If switch, set function
            if (DeviceData.RCChannel[i].Digital)
            {
                switchFunctionCombo[start_interval + j]->setCurrentIndex(switchFunctionCombo[start_interval + j]->findData(DeviceData.RCChannel[i].swFunction[j]));
            }

            // Some functions requires numbers and actions
            SaveChannelSwitchAction(switchFunctionCombo[i + j], i, j);     // First make sure all options are shown/hidden correctly
            if (DeviceData.RCChannel[i].swFunction[j] == SF_USER ||
                DeviceData.RCChannel[i].swFunction[j] == SF_MG   ||
                DeviceData.RCChannel[i].swFunction[j] == SF_LIGHT||
                DeviceData.RCChannel[i].swFunction[j] == SF_CANNON_FIRE)
            {
                actionNumCombo[start_interval + j]->setCurrentText(QString::number(DeviceData.RCChannel[i].actionNum[j]));
                switchActionCombo[start_interval + j]->setCurrentIndex(switchActionCombo[start_interval + j]->findData(DeviceData.RCChannel[i].switchAction[j]));
            }

        }
    }

    // Settings tab
    // ---------------------------------------------------------------------------------------------------------->>
    // Squeaks
    for (uint8_t i=0; i<NUM_SQUEAKS; i++)
    {
        chkSqueakEnabled[i]->setChecked(DeviceData.Squeak[i].Enabled);
        squeakMinInterval[i]->setValue(DeviceData.Squeak[i].MinInterval_mS);
        squeakMaxInterval[i]->setValue(DeviceData.Squeak[i].MaxInterval_mS);
    }
    ui->spinMinSqueakSpeed->setValue(DeviceData.MinSqueakSpeedPct);

    ShowHideOtherSqueakSettings();
    ShowHideSqueak1Settings(ui->chkEnableSqueak1->isChecked());
    ShowHideSqueak2Settings(ui->chkEnableSqueak2->isChecked());
    ShowHideSqueak3Settings(ui->chkEnableSqueak3->isChecked());
    ShowHideSqueak4Settings(ui->chkEnableSqueak4->isChecked());
    ShowHideSqueak5Settings(ui->chkEnableSqueak5->isChecked());
    ShowHideSqueak6Settings(ui->chkEnableSqueak6->isChecked());

    // Volumes
    ui->sliderEngineVolume->setValue(DeviceData.VolumeEngine);
    ui->sliderOverlayVolume->setValue(DeviceData.VolumeTrackOverlay);
    ui->sliderEffectsVolume->setValue(DeviceData.VolumeEffects);

    // Throttle position
    ui->radioCenterIdle->setChecked(DeviceData.centerThrottle);
    ui->radioLowIdle->setChecked(!DeviceData.centerThrottle);

    // Lights
    ui->spinLight1Flash_mS->setValue(DeviceData.Light1FlashTime_mS);
    ui->spinLight1BlinkOn_mS->setValue(DeviceData.Light1BlinkOnTime_mS);
    ui->spinLight1BlinkOff_mS->setValue(DeviceData.Light1BlinkOffTime_mS);
    ui->spinLight2Flash_mS->setValue(DeviceData.Light2FlashTime_mS);
    ui->spinLight2BlinkOn_mS->setValue(DeviceData.Light2BlinkOnTime_mS);
    ui->spinLight2BlinkOff_mS->setValue(DeviceData.Light2BlinkOffTime_mS);
    ui->spinLight3Flash_mS->setValue(DeviceData.Light3FlashTime_mS);
    ui->spinLight3BlinkOn_mS->setValue(DeviceData.Light3BlinkOnTime_mS);
    ui->spinLight3BlinkOff_mS->setValue(DeviceData.Light3BlinkOffTime_mS);

    // Servo
    ui->spinRecoilTimeToRecoil->setValue(DeviceData.ServoTimeToRecoil);
    ui->spinRecoilTimeToReturn->setValue(DeviceData.ServoTimeToReturn);
    ui->chkRecoilServoReversed->setChecked(DeviceData.ServoReversed);
    ui->spinEndPointRecoil->setValue(DeviceData.ServoEndPointRecoiled);
    ui->spinEndPointBattery->setValue(DeviceData.ServoEndPointBattery);

    // Engine
    ui->chkEngineAutoStart->setChecked(DeviceData.EngineAutoStart);
    double autostop = DeviceData.EngineAutoStopTime_mS;  // We need to convert mS to Minutes with 1-decimal precision
    autostop /= 60000;
    ui->dspinEngineAutoStop->setValue(autostop);
}


void MainWindow::Controls_to_Variables(void)
{
    // Radio tab
    // ---------------------------------------------------------------------------------------------------------->>
    // CHANNELS
    for (int i=0; i<CHANNELS; i++)
    {
        // Channel type (switch or some other)
        DeviceData.RCChannel[i].chType = channelTypeCombo[i]->getCurrentChannelType();
        if (static_cast<channel_type>(channelTypeCombo[i]->itemData(channelTypeCombo[i]->currentIndex()).toUInt()) == CT_SWITCH)
        {
             DeviceData.RCChannel[i].Digital = true;
        }
        else DeviceData.RCChannel[i].Digital = false;
        // Number of channel switch positions (if switch)
        DeviceData.RCChannel[i].numPositions = switchPosCombo[i]->currentText().toUInt();
        // Channel reversed settings
        DeviceData.RCChannel[i].reversed = reversedCheck[i]->isChecked();

        int start_interval = (MAX_SWITCH_POSITIONS * i);
        for (int j=0; j<MAX_SWITCH_POSITIONS; j++)
        {
            // If switch, set function, action, and action number
            if (DeviceData.RCChannel[i].Digital && switchFunctionCombo[start_interval + j]->getCurrentSwitchFunction() != SF_NULL)
            {
                DeviceData.RCChannel[i].swFunction[j] = switchFunctionCombo[start_interval + j]->getCurrentSwitchFunction();
                DeviceData.RCChannel[i].switchAction[j] = switchActionCombo[start_interval + j]->getCurrentSwitchAction();
                DeviceData.RCChannel[i].actionNum[j] = actionNumCombo[start_interval + j]->currentText().toInt();
                DeviceData.RCChannel[i].swFunctionID[j] = switchSettingsToID(DeviceData.RCChannel[i].swFunction[j], DeviceData.RCChannel[i].switchAction[j], DeviceData.RCChannel[i].actionNum[j]);
                // qDebug() << "Channel " << i + 1 << " Position " << j + 1 << "Function " << DeviceData.RCChannel[i].swFunction[j] << " Action " << DeviceData.RCChannel[i].switchAction[j] << " Num " << DeviceData.RCChannel[i].actionNum[j] << " ID " << DeviceData.RCChannel[i].swFunctionID[j];
            }
            else // Default these to something benign
            {
                DeviceData.RCChannel[i].swFunctionID[j] = 0;
                DeviceData.RCChannel[i].swFunction[j] = SF_NULL;
                DeviceData.RCChannel[i].switchAction[j] = ACTION_NULL;
                DeviceData.RCChannel[i].actionNum[j] = 1;
            }
        }
    }

    // Settings tab
    // ---------------------------------------------------------------------------------------------------------->>
    // Squeaks
    for (uint8_t i=0; i<NUM_SQUEAKS; i++)
    {

        DeviceData.Squeak[i].Enabled = chkSqueakEnabled[i]->isChecked();
        DeviceData.Squeak[i].MinInterval_mS = squeakMinInterval[i]->value();
        DeviceData.Squeak[i].MaxInterval_mS = squeakMaxInterval[i]->value();
    }
    DeviceData.MinSqueakSpeedPct = ui->spinMinSqueakSpeed->value();

    // Volumes
    DeviceData.VolumeEngine = ui->sliderEngineVolume->value();
    DeviceData.VolumeTrackOverlay = ui->sliderOverlayVolume->value();
    DeviceData.VolumeEffects = ui->sliderEffectsVolume->value();

    // Throttle position
    if (ui->radioCenterIdle->isChecked()) DeviceData.centerThrottle = true;
    else if (ui->radioLowIdle->isChecked()) DeviceData.centerThrottle = false;

    // Lights
    DeviceData.Light1FlashTime_mS = ui->spinLight1Flash_mS->value();
    DeviceData.Light1BlinkOnTime_mS = ui->spinLight1BlinkOn_mS->value();
    DeviceData.Light1BlinkOffTime_mS = ui->spinLight1BlinkOff_mS->value();
    DeviceData.Light2FlashTime_mS = ui->spinLight2Flash_mS->value();
    DeviceData.Light2BlinkOnTime_mS = ui->spinLight2BlinkOn_mS->value();
    DeviceData.Light2BlinkOffTime_mS = ui->spinLight2BlinkOff_mS->value();
    DeviceData.Light3FlashTime_mS = ui->spinLight3Flash_mS->value();
    DeviceData.Light3BlinkOnTime_mS = ui->spinLight3BlinkOn_mS->value();
    DeviceData.Light3BlinkOffTime_mS = ui->spinLight3BlinkOff_mS->value();

    // Servo
    DeviceData.ServoReversed = ui->chkRecoilServoReversed->isChecked();
    DeviceData.ServoTimeToRecoil = ui->spinRecoilTimeToRecoil->value();
    DeviceData.ServoTimeToReturn = ui->spinRecoilTimeToReturn->value();
    DeviceData.ServoEndPointRecoiled = ui->spinEndPointRecoil->value();
    DeviceData.ServoEndPointBattery = ui->spinEndPointBattery->value();

    // Engine
    DeviceData.EngineAutoStart = ui->chkEngineAutoStart->isChecked();
    DeviceData.EngineAutoStopTime_mS = static_cast<int32_t>(ui->dspinEngineAutoStop->value() * 60000.0); // We need to convert minutes to milli-seconds

}



