#ifndef SWITCHFUNCTIONCOMBOBOX_H
#define SWITCHFUNCTIONCOMBOBOX_H

#include <QObject>
#include <QComboBox>
#include "op_defines.h"

class SwitchFunctionComboBox : public QComboBox
{
    Q_OBJECT
public:
    SwitchFunctionComboBox(QWidget *parent = 0);
    switch_function getCurrentSwitchFunction(void);

private slots:
  void SwitchFunctionChangedSlot(const QString &);

private:

signals:
  void SwitchFunctionChanged(SwitchFunctionComboBox *);

};

#endif // SWITCHFUNCTIONCOMBOBOX_H
