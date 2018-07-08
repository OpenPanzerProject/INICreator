#include "combo_switchfunctions.h"

SwitchFunctionComboBox::SwitchFunctionComboBox(QWidget *parent) : QComboBox(parent)
{
    // Fill on creation
    this->insertItem(0, "", SF_NULL);
    this->insertItem(1, "Start Engine", SF_ENGINE_START);
    this->insertItem(2, "Stop Engine", SF_ENGINE_STOP);
    this->insertItem(3, "Toggle Engine", SF_ENGINE_TOGGLE);
    this->insertItem(4, "Cannon Fire", SF_CANNON_FIRE);
    this->insertItem(5, "Machine Gun", SF_MG);
    this->insertItem(9, "User Sound", SF_USER);
    this->insertItem(10,"Light", SF_LIGHT);


    // Connect to my custom signal
    connect(this, SIGNAL(editTextChanged(QString)), this, SLOT(SwitchFunctionChangedSlot(QString)));
    connect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(SwitchFunctionChangedSlot(QString)));
}

void SwitchFunctionComboBox::SwitchFunctionChangedSlot(const QString &)
{
    // Send out custom signal with a pointer to this box
    emit SwitchFunctionChanged(this);
}

// This returns the current actual switch function
switch_function SwitchFunctionComboBox::getCurrentSwitchFunction(void)
{
    return switch_function(this->currentData().toInt());
}
