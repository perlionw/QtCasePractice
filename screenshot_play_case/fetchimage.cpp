#include "fetchimage.h"
#include <QClipboard>
#include <QApplication>
FetchImage::FetchImage(QObject *parent) : QObject(parent)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FetchImage::fetchPixmap);
    timer->start(200);
}

void FetchImage::fetchPixmap()
{
    QClipboard* clip_board = QApplication::clipboard();
    emit updatePixmap(clip_board->pixmap());
}

