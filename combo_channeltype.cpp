#include "combo_channeltype.h"

ChannelTypeComboBox::ChannelTypeComboBox(QWidget *parent) : QComboBox(parent) {}

void ChannelTypeComboBox::setType(int channel)
{
    // Generate the default options with associated values
    // We use a different list depending on the channel, but they all get at least the switch option
    switch (channel)
    {
        case 1:
        this->insertItem(0, "Switch", CF_SWITCH);
        this->insertItem(1, "Engine Speed", CF_ENGINE_SPEED);
        break;

        case 5:
        this->insertItem(0, "Switch", CF_SWITCH);
        this->insertItem(1, "Volume", CF_VOLUME);
        break;

        default:
        this->insertItem(0, "Switch", CF_SWITCH);
    }


    // Connect to my custom signal
    connect(this, SIGNAL(editTextChanged(QString)), this, SLOT(ChannelTypeChangedSlot(QString)));
    connect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(ChannelTypeChangedSlot(QString)));
}

void ChannelTypeComboBox::ChannelTypeChangedSlot(const QString )
{
    // Send out custom signal with a pointer to this box
    emit ChannelTypeChanged(this);
}

// This returns the current actual channel_function
channel_function ChannelTypeComboBox::getCurrentChannelFunction(void)
{
    return channel_function(this->currentData().toInt());
}
