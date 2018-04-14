#ifndef CHANNELPOSITIONSCOMBOBOX_H
#define CHANNELPOSITIONSCOMBOBOX_H

#include <QObject>
#include <QComboBox>

class ChannelPositionsComboBox : public QComboBox
{
    Q_OBJECT
public:
    ChannelPositionsComboBox(QWidget *parent = 0);

private slots:
  void ChannelPositionChangedSlot(const QString &);

private:

signals:
  void ChannelPositionChanged(ChannelPositionsComboBox *);

};

#endif // CHANNELPOSITIONSCOMBOBOX_H






