#ifndef FETCHIMAGE_H
#define FETCHIMAGE_H

#include <QObject>
#include <QPixmap>
#include <QTimer>
class FetchImage : public QObject
{
    Q_OBJECT
public:
    explicit FetchImage(QObject *parent = 0);

signals:

    void updatePixmap(const QPixmap& pixmap);

public slots:
    void fetchPixmap();

private:
    QTimer* timer;
};

#endif // FETCHIMAGE_H
