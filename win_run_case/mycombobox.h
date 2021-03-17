#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H
#include <QComboBox>
#include <QKeyEvent>
class MyComboBox : public QComboBox
{
    Q_OBJECT
public:
    MyComboBox(QWidget* parent = nullptr);

    virtual void keyReleaseEvent(QKeyEvent * e);

signals:
    void returnPressed();
};

#endif // MYCOMBOBOX_H
