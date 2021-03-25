#include "widget.h"
#include "ui_widget.h"
#include <QDesktopWidget>
#include <QPixmap>
#include <QDebug>
#include <QFileDialog>
#include <QDesktopServices>
#include <QClipboard>
#include <QMenu>
#include <QAction>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->delayTimeSpinBox->setValue(3);
    connect(ui->newImagePushButton, &QPushButton::clicked, this, &Widget::newImagePushButtonSlot);
    connect(ui->saveImagePushButton, &QPushButton::clicked, this, &Widget::saveImagePushButtonSlot);
    connect(ui->exitPushButton, &QPushButton::clicked, this, &Widget::close);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Widget::shotScreenSlot);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu;
    QAction* saveAction = new QAction("保存图片", menu);
    connect(saveAction, &QAction::triggered, this, &Widget::saveImagePushButtonSlot);
    menu->addAction(saveAction);
    menu->exec(event->globalPos());
    //清理自己
    menu->deleteLater();
}

void Widget::newImagePushButtonSlot()
{
    //检测是否需要隐藏
    if(ui->hiddenCheckBox->isChecked())
    {
        //this->setHidden(true);
        int delaytime = ui->delayTimeSpinBox->value();
        qDebug() << delaytime;
//        timer->start(delaytime*1000);
        timer->start(1000/24);
    }
    else
    {
        qApp->beep();
    }
}

void Widget::shotScreenSlot()
{
    //抓取主屏幕
    pix = QPixmap::grabWindow(QApplication::desktop()->winId());
    ui->imageLabel->setPixmap(pix.scaled(ui->imageLabel->size()));

    QClipboard* cliboard = QApplication::clipboard();

    cliboard->setPixmap(pix);

    //this->show();
    //timer->stop();
}

void Widget::saveImagePushButtonSlot()
{
    qDebug() << "adafsa";
    QString file_name = QFileDialog::getSaveFileName(this, "保存", QDesktopServices::storageLocation(QDesktopServices::PicturesLocation) + "/*.jpg"
                              , "Image(*.jpg;*.png;*.jpeg)");
    this->pix.save(file_name);
}
