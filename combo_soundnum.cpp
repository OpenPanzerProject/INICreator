#include "combo_soundnum.h"

SoundNumComboBox::SoundNumComboBox(QWidget *parent) : QComboBox(parent)
{
    // Fill on creation
    for (int i=0; i<MAX_NUM_USER_SOUNDS; i++)
    {
        this->insertItem(i+1, QString::number(i+1));
    }

    // Connect to my custom signal
    connect(this, SIGNAL(editTextChanged(QString)), this, SLOT(SoundNumChangedSlot(QString)));
    connect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(SoundNumChangedSlot(QString)));
}

void SoundNumComboBox::SoundNumChangedSlot(const QString &)
{
    // Send out custom signal with a pointer to this box
    emit SoundNumChanged(this);
}
