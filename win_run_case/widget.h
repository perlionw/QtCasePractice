#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QProcess>
#include <QTextCodec>
#include <QSharedMemory>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void okBtnSlot();
    void cancelBtnSlot();
    void browserBtnSlot();
    void returnPressedSlot();

    void finishedEvent();
    void errorEvent();
private:
    QProcess::ProcessState StartProcess(const QString& process);

    //判断命令是否已经存在于缓冲中
    bool CommandIsExisted(const QString& process);

    //写缓冲命令
    void WriteCommandToFile(const QString& file_path, const QString& process);
    //读文件命令
    QStringList ReadCommandToFile(const QString& file_path);

private:
    Ui::Widget *ui;
    QProcess* myProcess;
};

#endif // WIDGET_H
