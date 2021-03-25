#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QContextMenuEvent>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void contextMenuEvent(QContextMenuEvent * event);

private slots:
    void newImagePushButtonSlot();
    void shotScreenSlot();
    void saveImagePushButtonSlot();
private:
    Ui::Widget *ui;
    QTimer* timer;
    QPixmap pix;
};

#endif // WIDGET_H
