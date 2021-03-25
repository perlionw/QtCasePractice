#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPixmap>
#include <QPaintEvent>
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
     void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private slots:
    void playerSlot();
    void updatePixmapSlot(const QPixmap& pix);
private:
    Ui::Widget *ui;
    QTimer* timer;
    QPixmap pixmap;
};

#endif // WIDGET_H
