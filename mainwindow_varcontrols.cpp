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
        channelTypeCombo[i]->setCurrentIndex(channelTypeCombo[i]->findData(DeviceData.RCChannel[i].chFunction));
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

            // If user sound, set number and action
            if (DeviceData.RCChannel[i].swFunction[j] == SF_USER)
            {
                soundNumCombo[start_interval + j]->setCurrentText(QString::number(DeviceData.RCChannel[i].userSoundNum[j]));
                soundActionCombo[start_interval + j]->setCurrentIndex(soundActionCombo[start_interval + j]->findData(DeviceData.RCChannel[i].userSoundAction[j]));
            }
            // DeviceData.RCChannel[ChannelNum].swFunction[PosNum] = static_cast<switch_function>(switchFunctionCombo[start_interval + PosNum]->currentData().toUInt());

            SaveChannelSwitchAction(switchFunctionCombo[i + j], i, j);
        }
    }

    // Sound tab
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
}


void MainWindow::Controls_to_Variables(void)
{
    // Radio tab
    // ---------------------------------------------------------------------------------------------------------->>
    // CHANNELS
    for (int i=0; i<CHANNELS; i++)
    {
        // Channel type (switch or some other)
        DeviceData.RCChannel[i].chFunction = channelTypeCombo[i]->getCurrentChannelFunction();
        if (static_cast<channel_function>(channelTypeCombo[i]->itemData(channelTypeCombo[i]->currentIndex()).toUInt()) == CF_SWITCH)
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
            // If switch, set function
            if (DeviceData.RCChannel[i].Digital)    // We can refer to DeviceData because Digital was already set above
            {
                DeviceData.RCChannel[i].swFunction[j] = switchFunctionCombo[start_interval + j]->getCurrentSwitchFunction();
            }

            // If user sound, set number and action
            if (DeviceData.RCChannel[i].swFunction[j] == SF_USER)   // We can refer to DeviceData because switch function was already set above
            {
                DeviceData.RCChannel[i].userSoundNum[j] = soundNumCombo[start_interval + j]->currentIndex();
                DeviceData.RCChannel[i].userSoundAction[j] = soundActionCombo[start_interval + j]->getCurrentSoundAction();
            }
        }
    }

    // Sound tab
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
}



