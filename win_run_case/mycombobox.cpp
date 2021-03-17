#include "mycombobox.h"

MyComboBox::MyComboBox(QWidget* parent):QComboBox(parent)
{

}

void MyComboBox::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
    {
        emit returnPressed();
    }
}

