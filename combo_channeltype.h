#ifndef CHANNELTYPECOMBOBOX_H
#define CHANNELTYPECOMBOBOX_H

#include <QObject>
#include <QComboBox>
#include "op_defines.h"

class ChannelTypeComboBox : public QComboBox
{
    Q_OBJECT
public:
    ChannelTypeComboBox(QWidget *parent = 0);
    void setType(int);
    channel_type getCurrentChannelType(void);

private slots:
  void ChannelTypeChangedSlot(const QString);

private:

signals:
  void ChannelTypeChanged(ChannelTypeComboBox *);

};

#endif // CHANNELTYPECOMBOBOX_H



