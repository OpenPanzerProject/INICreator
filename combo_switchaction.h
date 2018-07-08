#ifndef SWITCHACTIONCOMBOBOX_H
#define SWITCHACTIONCOMBOBOX_H

#include <QObject>
#include <QComboBox>
#include "op_defines.h"

class SwitchActionComboBox : public QComboBox
{
    Q_OBJECT
public:
    SwitchActionComboBox(QWidget *parent = 0);
    switch_action getCurrentSwitchAction(void);
    void setActions(switch_function);

private slots:
    void SwitchActionChangedSlot(const QString &);

private:

signals:
    void SwitchActionChanged(SwitchActionComboBox *);

};

#endif // SWITCHACTIONCOMBOBOX_H


