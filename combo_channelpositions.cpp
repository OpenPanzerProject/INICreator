#include "combo_channelpositions.h"

ChannelPositionsComboBox::ChannelPositionsComboBox(QWidget *parent) : QComboBox(parent)
{
    // Fill on creation
    this->insertItem(0, "2", 2);
    this->insertItem(1, "3", 3);
    this->insertItem(2, "4", 4);
    this->insertItem(3, "5", 5);
    this->insertItem(4, "6", 6);

    // Connect to my custom signal
    connect(this, SIGNAL(editTextChanged(QString)), this, SLOT(ChannelPositionChangedSlot(QString)));
    connect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(ChannelPositionChangedSlot(QString)));
}
void ChannelPositionsComboBox::ChannelPositionChangedSlot(const QString &)
{
    // Send out custom signal with a pointer to this box
    emit ChannelPositionChanged(this);
}

