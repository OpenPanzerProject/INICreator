#ifndef ACTIONNUMCOMBOBOX_H
#define ACTIONNUMCOMBOBOX_H

#include <QObject>
#include <QComboBox>
#include "op_defines.h"


class ActionNumComboBox : public QComboBox
{
    Q_OBJECT
public:
    ActionNumComboBox(QWidget *parent = 0);
    void setActionNums(switch_function);

private slots:
    void ActionNumChangedSlot(const QString &);

private:

signals:
    void ActionNumChanged(ActionNumComboBox *);

};

#endif // ACTIONNUMCOMBOBOX_H

