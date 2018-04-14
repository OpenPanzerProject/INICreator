#include "combo_soundaction.h"

SoundActionComboBox::SoundActionComboBox(QWidget *parent) : QComboBox(parent)
{
    // Fill on creation
    this->insertItem(0, "Play", SOUND_PLAY);
    this->insertItem(1, "Repeat", SOUND_REPEAT);
    this->insertItem(2, "Stop", SOUND_STOP);

    // Connect to my custom signal
    connect(this, SIGNAL(editTextChanged(QString)), this, SLOT(SoundActionChangedSlot(QString)));
    connect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(SoundActionChangedSlot(QString)));
}

void SoundActionComboBox::SoundActionChangedSlot(const QString &)
{
    // Send out custom signal with a pointer to this box
    emit SoundActionChanged(this);
}

// This returns the current actual sound action
sound_action SoundActionComboBox::getCurrentSoundAction(void)
{
    return sound_action(this->currentData().toInt());
}
