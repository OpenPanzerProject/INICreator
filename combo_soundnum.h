#ifndef SOUNDNUMCOMBOBOX_H
#define SOUNDNUMCOMBOBOX_H

#include <QObject>
#include <QComboBox>
#include "op_defines.h"


class SoundNumComboBox : public QComboBox
{
    Q_OBJECT
public:
    SoundNumComboBox(QWidget *parent = 0);

private slots:
    void SoundNumChangedSlot(const QString &);

private:

signals:
    void SoundNumChanged(SoundNumComboBox *);

};

#endif // SOUNDNUMCOMBOBOX_H

