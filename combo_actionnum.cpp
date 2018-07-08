#include "combo_actionnum.h"

ActionNumComboBox::ActionNumComboBox(QWidget *parent) : QComboBox(parent)
{
    // Connect to my custom signal
    connect(this, SIGNAL(editTextChanged(QString)), this, SLOT(ActionNumChangedSlot(QString)));
    connect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(ActionNumChangedSlot(QString)));
}

void ActionNumComboBox::setActionNums(switch_function sf)
{
    // First clear all prior items
    this->clear();

    // Different actions are available for different switch functions
    switch (sf)
    {
        case SF_USER:
            for (int i=0; i<MAX_NUM_USER_SOUNDS; i++)
            {
                this->insertItem(i+1, QString::number(i+1));
            }
            break;

        case SF_MG:                 // Same as number of lights
        case SF_CANNON_FIRE:        // Same as number of lights
        case SF_LIGHT:
            for (int i=0; i<NUM_LIGHTS; i++)
            {
                this->insertItem(i+1, QString::number(i+1));
            }
            break;

        default:
            return;
    }
}

void ActionNumComboBox::ActionNumChangedSlot(const QString &)
{
    // Send out custom signal with a pointer to this box
    emit ActionNumChanged(this);
}

