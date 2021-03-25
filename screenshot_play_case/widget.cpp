#include "widget.h"
#include "ui_widget.h"
#include <QClipboard>
#include "fetchimage.h"
#include <QThread>
#include <QPainter>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

//    timer = new QTimer(this);

//    connect(timer, &QTimer::timeout, this, &Widget::playerSlot);

//    timer->start(200);

    FetchImage* ob = new FetchImage(this);

//    QThread* thread = new QThread;
//    ob->moveToThread(thread);
//    thread->start();
    connect(ob, &FetchImage::updatePixmap, this, &Widget::updatePixmapSlot);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.save();
    QPixmap temp = pixmap.scaled(this->size());
    painter.drawPixmap(0, 0, temp.width(), temp.height(), temp);
    painter.restore();
}

void Widget::playerSlot()
{
    QClipboard* clipboard = QApplication::clipboard();
    this->pixmap = clipboard->pixmap();
    //ui->playerLabel->setPixmap(this->pixmap.scaled(ui->playerLabel->size()));
}

void Widget::updatePixmapSlot(const QPixmap& pix)
{
    this->pixmap = pix;
    update();
}
