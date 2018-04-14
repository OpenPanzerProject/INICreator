#ifndef SOUNDACTIONCOMBOBOX_H
#define SOUNDACTIONCOMBOBOX_H

#include <QObject>
#include <QComboBox>
#include "op_defines.h"

class SoundActionComboBox : public QComboBox
{
    Q_OBJECT
public:
    SoundActionComboBox(QWidget *parent = 0);
    sound_action getCurrentSoundAction(void);

private slots:
    void SoundActionChangedSlot(const QString &);

private:

signals:
    void SoundActionChanged(SoundActionComboBox *);

};

#endif // SOUNDACTIONCOMBOBOX_H


