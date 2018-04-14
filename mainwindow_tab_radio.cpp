#include "mainwindow.h"
#include "ui_mainwindow.h"


//------------------------------------------------------------------------------------------------------------------------>>
// FORM CONTROLS - RADIO TAB - SETUP
//------------------------------------------------------------------------------------------------------------------------>>
void MainWindow::SetupControls_RadioTab(void)
{
    ui->cboChannel1Type->setType(1);
    ui->cboChannel2Type->setType(2);
    ui->cboChannel3Type->setType(3);
    ui->cboChannel4Type->setType(4);
    ui->cboChannel5Type->setType(5);

    ui->cboChannel1Type->setCurrentIndex(1);    // First has the option of being engine speed, select that by default
    ui->cboChannel5Type->setCurrentIndex(1);    // Fifth has the option of being volume, select that by default

    // Array of channel types
    channelTypeCombo[0] = ui->cboChannel1Type;
    channelTypeCombo[1] = ui->cboChannel2Type;
    channelTypeCombo[2] = ui->cboChannel3Type;
    channelTypeCombo[3] = ui->cboChannel4Type;
    channelTypeCombo[4] = ui->cboChannel5Type;

    // Array of channel switch positions
    switchPosCombo[0] = ui->cboCh1NumSwitchPos;
    switchPosCombo[1] = ui->cboCh2NumSwitchPos;
    switchPosCombo[2] = ui->cboCh3NumSwitchPos;
    switchPosCombo[3] = ui->cboCh4NumSwitchPos;
    switchPosCombo[4] = ui->cboCh5NumSwitchPos;

    // Array of channel reversed settings
    reversedCheck[0] = ui->chkReversed1;
    reversedCheck[1] = ui->chkReversed2;
    reversedCheck[2] = ui->chkReversed3;
    reversedCheck[3] = ui->chkReversed4;
    reversedCheck[4] = ui->chkReversed5;

    // Array of switch functions by channel/position
    switchFunctionCombo[0] = ui->cboCh1Pos1;
    switchFunctionCombo[1] = ui->cboCh1Pos2;
    switchFunctionCombo[2] = ui->cboCh1Pos3;
    switchFunctionCombo[3] = ui->cboCh1Pos4;
    switchFunctionCombo[4] = ui->cboCh1Pos5;
    switchFunctionCombo[5] = ui->cboCh1Pos6;
    switchFunctionCombo[6] = ui->cboCh2Pos1;
    switchFunctionCombo[7] = ui->cboCh2Pos2;
    switchFunctionCombo[8] = ui->cboCh2Pos3;
    switchFunctionCombo[9] = ui->cboCh2Pos4;
    switchFunctionCombo[10] = ui->cboCh2Pos5;
    switchFunctionCombo[11] = ui->cboCh2Pos6;
    switchFunctionCombo[12] = ui->cboCh3Pos1;
    switchFunctionCombo[13] = ui->cboCh3Pos2;
    switchFunctionCombo[14] = ui->cboCh3Pos3;
    switchFunctionCombo[15] = ui->cboCh3Pos4;
    switchFunctionCombo[16] = ui->cboCh3Pos5;
    switchFunctionCombo[17] = ui->cboCh3Pos6;
    switchFunctionCombo[18] = ui->cboCh4Pos1;
    switchFunctionCombo[19] = ui->cboCh4Pos2;
    switchFunctionCombo[20] = ui->cboCh4Pos3;
    switchFunctionCombo[21] = ui->cboCh4Pos4;
    switchFunctionCombo[22] = ui->cboCh4Pos5;
    switchFunctionCombo[23] = ui->cboCh4Pos6;
    switchFunctionCombo[24] = ui->cboCh5Pos1;
    switchFunctionCombo[25] = ui->cboCh5Pos2;
    switchFunctionCombo[26] = ui->cboCh5Pos3;
    switchFunctionCombo[27] = ui->cboCh5Pos4;
    switchFunctionCombo[28] = ui->cboCh5Pos5;
    switchFunctionCombo[29] = ui->cboCh5Pos6;

    // Array of user sound number by channel/position
    soundNumCombo[0] = ui->cboCh1Pos1_SoundNum;
    soundNumCombo[1] = ui->cboCh1Pos2_SoundNum;
    soundNumCombo[2] = ui->cboCh1Pos3_SoundNum;
    soundNumCombo[3] = ui->cboCh1Pos4_SoundNum;
    soundNumCombo[4] = ui->cboCh1Pos5_SoundNum;
    soundNumCombo[5] = ui->cboCh1Pos6_SoundNum;
    soundNumCombo[6] = ui->cboCh2Pos1_SoundNum;
    soundNumCombo[7] = ui->cboCh2Pos2_SoundNum;
    soundNumCombo[8] = ui->cboCh2Pos3_SoundNum;
    soundNumCombo[9] = ui->cboCh2Pos4_SoundNum;
    soundNumCombo[10] = ui->cboCh2Pos5_SoundNum;
    soundNumCombo[11] = ui->cboCh2Pos6_SoundNum;
    soundNumCombo[12] = ui->cboCh3Pos1_SoundNum;
    soundNumCombo[13] = ui->cboCh3Pos2_SoundNum;
    soundNumCombo[14] = ui->cboCh3Pos3_SoundNum;
    soundNumCombo[15] = ui->cboCh3Pos4_SoundNum;
    soundNumCombo[16] = ui->cboCh3Pos5_SoundNum;
    soundNumCombo[17] = ui->cboCh3Pos6_SoundNum;
    soundNumCombo[18] = ui->cboCh4Pos1_SoundNum;
    soundNumCombo[19] = ui->cboCh4Pos2_SoundNum;
    soundNumCombo[20] = ui->cboCh4Pos3_SoundNum;
    soundNumCombo[21] = ui->cboCh4Pos4_SoundNum;
    soundNumCombo[22] = ui->cboCh4Pos5_SoundNum;
    soundNumCombo[23] = ui->cboCh4Pos6_SoundNum;
    soundNumCombo[24] = ui->cboCh5Pos1_SoundNum;
    soundNumCombo[25] = ui->cboCh5Pos2_SoundNum;
    soundNumCombo[26] = ui->cboCh5Pos3_SoundNum;
    soundNumCombo[27] = ui->cboCh5Pos4_SoundNum;
    soundNumCombo[28] = ui->cboCh5Pos5_SoundNum;
    soundNumCombo[29] = ui->cboCh5Pos6_SoundNum;

    // Array of user sound action by channel/position
    soundActionCombo[0] = ui->cboCh1Pos1_Play;
    soundActionCombo[1] = ui->cboCh1Pos2_Play;
    soundActionCombo[2] = ui->cboCh1Pos3_Play;
    soundActionCombo[3] = ui->cboCh1Pos4_Play;
    soundActionCombo[4] = ui->cboCh1Pos5_Play;
    soundActionCombo[5] = ui->cboCh1Pos6_Play;
    soundActionCombo[6] = ui->cboCh2Pos1_Play;
    soundActionCombo[7] = ui->cboCh2Pos2_Play;
    soundActionCombo[8] = ui->cboCh2Pos3_Play;
    soundActionCombo[9] = ui->cboCh2Pos4_Play;
    soundActionCombo[10] = ui->cboCh2Pos5_Play;
    soundActionCombo[11] = ui->cboCh2Pos6_Play;
    soundActionCombo[12] = ui->cboCh3Pos1_Play;
    soundActionCombo[13] = ui->cboCh3Pos2_Play;
    soundActionCombo[14] = ui->cboCh3Pos3_Play;
    soundActionCombo[15] = ui->cboCh3Pos4_Play;
    soundActionCombo[16] = ui->cboCh3Pos5_Play;
    soundActionCombo[17] = ui->cboCh3Pos6_Play;
    soundActionCombo[18] = ui->cboCh4Pos1_Play;
    soundActionCombo[19] = ui->cboCh4Pos2_Play;
    soundActionCombo[20] = ui->cboCh4Pos3_Play;
    soundActionCombo[21] = ui->cboCh4Pos4_Play;
    soundActionCombo[22] = ui->cboCh4Pos5_Play;
    soundActionCombo[23] = ui->cboCh4Pos6_Play;
    soundActionCombo[24] = ui->cboCh5Pos1_Play;
    soundActionCombo[25] = ui->cboCh5Pos2_Play;
    soundActionCombo[26] = ui->cboCh5Pos3_Play;
    soundActionCombo[27] = ui->cboCh5Pos4_Play;
    soundActionCombo[28] = ui->cboCh5Pos5_Play;
    soundActionCombo[29] = ui->cboCh5Pos6_Play;

    // Force initalization of function/sound num/sound action combos
    for (int i=0; i<CHANNELS; i++)
    {
        SaveChannelType(channelTypeCombo[i],i);
        SaveChannelPositions(switchPosCombo[i], i);
        for (int j=0; j<MAX_SWITCH_POSITIONS; j++)
        {
            SaveChannelSwitchAction(switchFunctionCombo[(i * MAX_SWITCH_POSITIONS) + i], i, j);
        }
    }

    // Save information when any channel type is changed, and enable/disable the position-count combo as required
    connect(ui->cboChannel1Type, &ChannelTypeComboBox::ChannelTypeChanged, [this](ChannelTypeComboBox* changedCombo) {SaveChannelType(changedCombo, 0);});
    connect(ui->cboChannel2Type, &ChannelTypeComboBox::ChannelTypeChanged, [this](ChannelTypeComboBox* changedCombo) {SaveChannelType(changedCombo, 1);});
    connect(ui->cboChannel3Type, &ChannelTypeComboBox::ChannelTypeChanged, [this](ChannelTypeComboBox* changedCombo) {SaveChannelType(changedCombo, 2);});
    connect(ui->cboChannel4Type, &ChannelTypeComboBox::ChannelTypeChanged, [this](ChannelTypeComboBox* changedCombo) {SaveChannelType(changedCombo, 3);});
    connect(ui->cboChannel5Type, &ChannelTypeComboBox::ChannelTypeChanged, [this](ChannelTypeComboBox* changedCombo) {SaveChannelType(changedCombo, 4);});

    // Save information abou reversed status
    connect(ui->chkReversed1, &QCheckBox::toggled, [this](bool isChecked) {SaveChannelReversed(isChecked, 0);});
    connect(ui->chkReversed2, &QCheckBox::toggled, [this](bool isChecked) {SaveChannelReversed(isChecked, 1);});
    connect(ui->chkReversed3, &QCheckBox::toggled, [this](bool isChecked) {SaveChannelReversed(isChecked, 2);});
    connect(ui->chkReversed4, &QCheckBox::toggled, [this](bool isChecked) {SaveChannelReversed(isChecked, 3);});
    connect(ui->chkReversed5, &QCheckBox::toggled, [this](bool isChecked) {SaveChannelReversed(isChecked, 4);});

    // Save information when any channel position-counts are changed
    connect(ui->cboCh1NumSwitchPos, &ChannelPositionsComboBox::ChannelPositionChanged, [this](ChannelPositionsComboBox* changedCombo) {SaveChannelPositions(changedCombo, 0);});
    connect(ui->cboCh2NumSwitchPos, &ChannelPositionsComboBox::ChannelPositionChanged, [this](ChannelPositionsComboBox* changedCombo) {SaveChannelPositions(changedCombo, 1);});
    connect(ui->cboCh3NumSwitchPos, &ChannelPositionsComboBox::ChannelPositionChanged, [this](ChannelPositionsComboBox* changedCombo) {SaveChannelPositions(changedCombo, 2);});
    connect(ui->cboCh4NumSwitchPos, &ChannelPositionsComboBox::ChannelPositionChanged, [this](ChannelPositionsComboBox* changedCombo) {SaveChannelPositions(changedCombo, 3);});
    connect(ui->cboCh5NumSwitchPos, &ChannelPositionsComboBox::ChannelPositionChanged, [this](ChannelPositionsComboBox* changedCombo) {SaveChannelPositions(changedCombo, 4);});

    connect(ui->cboCh1Pos1, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 0, 0);});
    connect(ui->cboCh1Pos2, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 0, 1);});
    connect(ui->cboCh1Pos3, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 0, 2);});
    connect(ui->cboCh1Pos4, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 0, 3);});
    connect(ui->cboCh1Pos5, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 0, 4);});
    connect(ui->cboCh1Pos6, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 0, 5);});
    connect(ui->cboCh2Pos1, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 1, 0);});
    connect(ui->cboCh2Pos2, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 1, 1);});
    connect(ui->cboCh2Pos3, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 1, 2);});
    connect(ui->cboCh2Pos4, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 1, 3);});
    connect(ui->cboCh2Pos5, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 1, 4);});
    connect(ui->cboCh2Pos6, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 1, 5);});
    connect(ui->cboCh3Pos1, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 2, 0);});
    connect(ui->cboCh3Pos2, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 2, 1);});
    connect(ui->cboCh3Pos3, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 2, 2);});
    connect(ui->cboCh3Pos4, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 2, 3);});
    connect(ui->cboCh3Pos5, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 2, 4);});
    connect(ui->cboCh3Pos6, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 2, 5);});
    connect(ui->cboCh4Pos1, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 3, 0);});
    connect(ui->cboCh4Pos2, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 3, 1);});
    connect(ui->cboCh4Pos3, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 3, 2);});
    connect(ui->cboCh4Pos4, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 3, 3);});
    connect(ui->cboCh4Pos5, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 3, 4);});
    connect(ui->cboCh4Pos6, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 3, 5);});
    connect(ui->cboCh5Pos1, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 4, 0);});
    connect(ui->cboCh5Pos2, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 4, 1);});
    connect(ui->cboCh5Pos3, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 4, 2);});
    connect(ui->cboCh5Pos4, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 4, 3);});
    connect(ui->cboCh5Pos5, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 4, 4);});
    connect(ui->cboCh5Pos6, &SwitchFunctionComboBox::SwitchFunctionChanged, [this](SwitchFunctionComboBox* changedCombo) {SaveChannelSwitchAction(changedCombo, 4, 5);});
}


//------------------------------------------------------------------------------------------------------------------------>>
// FORM CONTROLS - RADIO TAB - SAVE AND VALIDATE VALUE CHANGES
//------------------------------------------------------------------------------------------------------------------------>>
void MainWindow::SaveChannelReversed(bool checked, int WhatChannel)
{   // Any change in the reversed status of a channel, save to our variable
    ChannelSettings[WhatChannel].reversed = checked;
}

void MainWindow::SaveChannelType(ChannelTypeComboBox *, int ChannelNum)
{
    // Any change, save to our variable
    // "Switch" is data = 0, so !0 = digital flag
    DeviceData.RCChannel[ChannelNum].Digital = !static_cast<boolean>(channelTypeCombo[ChannelNum]->currentData().toUInt());
    DeviceData.RCChannel[ChannelNum].chFunction = static_cast<channel_function>(channelTypeCombo[ChannelNum]->currentData().toUInt());
    if (!DeviceData.RCChannel[ChannelNum].Digital)
    {
        // Type is set to some other function like volume or engine speed,
        // re-set and disable the count-of-switch positions and all the other combos related to switch positions
        if (switchPosCombo[ChannelNum]->findText("") == -1)   // If empty item doesn't already exist,
            switchPosCombo[ChannelNum]->addItem("");          // insert an empty item.
        switchPosCombo[ChannelNum]->setCurrentText("");       // Set the combo to it
        switchPosCombo[ChannelNum]->setEnabled(false);        // Now disable the combo
        DeviceData.RCChannel[ChannelNum].numPositions = 2;    // But default to 2 positions in our variable just so it's a number
    }
    else
    {
        // Type is set to digital, show the count-of-switch positions combo with however many positions we have saved
        switchPosCombo[ChannelNum]->setCurrentText(QString::number(DeviceData.RCChannel[ChannelNum].numPositions));
        // Now we're off the empty selection, remove it
        switchPosCombo[ChannelNum]->removeItem(switchPosCombo[ChannelNum]->findText(""));
        // Now re-enable
        switchPosCombo[ChannelNum]->setEnabled(true);
    }
}
void MainWindow::SaveChannelPositions(ChannelPositionsComboBox *, int ChannelNum)
{
    // Any change, save to our variable
    int poss = switchPosCombo[ChannelNum]->currentData().toInt();
    if (poss < MIN_SWITCH_POSITIONS || poss > MAX_SWITCH_POSITIONS) poss = 0;
    DeviceData.RCChannel[ChannelNum].numPositions = poss;

    // Now show the actions just for the number of positions we've selected
    int start_interval = (MAX_SWITCH_POSITIONS * ChannelNum);
    if (DeviceData.RCChannel[ChannelNum].numPositions >= MIN_SWITCH_POSITIONS)
    {
        for (int i=0; i<MAX_SWITCH_POSITIONS; i++)
        {
            if (DeviceData.RCChannel[ChannelNum].numPositions > i)
            {
                //switchFunctionCombo[start_interval+i]->show();
                switchFunctionCombo[start_interval+i]->setEnabled(true);
                // Only show the num/play combos if the switch function selection is SF_USER
                if (static_cast<switch_function>(switchFunctionCombo[start_interval+i]->currentData().toUInt()) == SF_USER)
                {
                    soundNumCombo[start_interval + i]->show();
                    soundActionCombo[start_interval + i]->show();
                }
                else
                {
                    soundNumCombo[start_interval + i]->hide();
                    soundActionCombo[start_interval + i]->hide();
                }
            }
            else
            {
                //switchFunctionCombo[start_interval+i]->hide();
                soundNumCombo[start_interval + i]->hide();
                soundActionCombo[start_interval + i]->hide();
                // Clear any functions from this position
                switchFunctionCombo[start_interval+i]->setCurrentIndex(switchFunctionCombo[start_interval + i]->findData(SF_NULL));
                switchFunctionCombo[start_interval+i]->setEnabled(false);
            }
        }
    }
    else
    {
        // If we have 0 switch positions we want to hide all
        for (int i=0; i<MAX_SWITCH_POSITIONS; i++)
        {
            //switchFunctionCombo[start_interval+i]->hide();
            soundNumCombo[start_interval + i]->hide();
            soundActionCombo[start_interval + i]->hide();
            // Clear any functions from this position
            switchFunctionCombo[start_interval+i]->setCurrentIndex(switchFunctionCombo[start_interval + i]->findData(SF_NULL));
            switchFunctionCombo[start_interval+i]->setEnabled(false);
        }
    }
}
void MainWindow::SaveChannelSwitchAction(SwitchFunctionComboBox *, int ChannelNum, int PosNum)
{
    // Any change, save to our variable
    int start_interval = (MAX_SWITCH_POSITIONS * ChannelNum);
    DeviceData.RCChannel[ChannelNum].swFunction[PosNum] = static_cast<switch_function>(switchFunctionCombo[start_interval + PosNum]->currentData().toUInt());

    // Now show the actions just for the number of positions we've selected
    if (DeviceData.RCChannel[ChannelNum].swFunction[PosNum] == SF_USER)
    {
        soundNumCombo[start_interval + PosNum]->show();
        soundActionCombo[start_interval + PosNum]->show();
    }
    else
    {
        soundNumCombo[start_interval + PosNum]->hide();
        soundActionCombo[start_interval + PosNum]->hide();
    }
}


