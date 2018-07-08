#include "combo_switchaction.h"

SwitchActionComboBox::SwitchActionComboBox(QWidget *parent) : QComboBox(parent)
{
    // Connect to my custom signal
    connect(this, SIGNAL(editTextChanged(QString)), this, SLOT(SwitchActionChangedSlot(QString)));
    connect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(SwitchActionChangedSlot(QString)));
}

void SwitchActionComboBox::setActions(switch_function sf)
{
    // First clear all prior items
    this->clear();

    // Different actions are available for different switch functions
    switch (sf)
    {
        case SF_USER:
            this->insertItem(0, "Play", ACTION_ONSTART);
            this->insertItem(1, "Repeat", ACTION_REPEATTOGGLE);
            this->insertItem(2, "Stop", ACTION_OFFSTOP);
            break;

        case SF_MG:
            this->insertItem(0, "Start", ACTION_ONSTART);
            this->insertItem(1, "Stop", ACTION_OFFSTOP);
            this->insertItem(2, "Toggle", ACTION_REPEATTOGGLE);
            break;

        case SF_LIGHT:
            this->insertItem(0, "On", ACTION_ONSTART);
            this->insertItem(1, "Off", ACTION_OFFSTOP);
            this->insertItem(2, "Toggle", ACTION_REPEATTOGGLE);
            this->insertItem(3, "Start Blink", ACTION_STARTBLINK);
            this->insertItem(4, "Toggle Blink", ACTION_TOGGLEBLINK);
            this->insertItem(5, "Flash", ACTION_FLASH);
            break;

        default:
            return;
    }
}

void SwitchActionComboBox::SwitchActionChangedSlot(const QString &)
{
    // Send out custom signal with a pointer to this box
    emit SwitchActionChanged(this);
}

// This returns the current actual sound action
switch_action SwitchActionComboBox::getCurrentSwitchAction(void)
{
    return switch_action(this->currentData().toInt());
}
