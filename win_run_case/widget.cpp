#include "widget.h"
#include "ui_widget.h"

#include <QString>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QBuffer>
#include <QTextStream>
#define FIXWIDTH 368
#define FIXHEIGHT 175
#define FILEPATH "cache.txt"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);


    //读取缓冲命令，添加至ComboxBox
    QStringList list = ReadCommandToFile(FILEPATH);
    if(list.size() >= 0)
    {
        ui->processComboBox->addItems(list);
    }


    //为空时设置按钮为不可点击
    if(ui->processComboBox->currentText().isEmpty())
        ui->okPushButton->setDisabled(true);

    //设置程序标题
    this->setWindowTitle("运行");

    //设置固定大小的窗口
    this->setMinimumSize(FIXWIDTH, FIXHEIGHT);
    this->setMaximumSize(FIXWIDTH, FIXHEIGHT);

    //设置部件在布局中的比例
    ui->infoHorizontalLayout->setStretchFactor(ui->imageLabel, 1);
    ui->infoHorizontalLayout->setStretchFactor(ui->infoLabel, 9);
    ui->openHorizontalLayout->setStretchFactor(ui->openLabel, 1);
    ui->openHorizontalLayout->setStretchFactor(ui->processComboBox, 9);

    //连接槽函数
    connect(ui->okPushButton, &QPushButton::clicked, this, &Widget::okBtnSlot);
    connect(ui->browserPushButton, &QPushButton::clicked, this, &Widget::browserBtnSlot);
    connect(ui->cancelPushButton, &QPushButton::clicked, this, &Widget::cancelBtnSlot);
    connect(ui->processComboBox, &MyComboBox::returnPressed, this, &Widget::returnPressedSlot);
    connect(ui->processComboBox, &MyComboBox::currentTextChanged, this, [&](){
        //检测到有内容改变时，若有内容则设置按钮为可点击，无内容设置为不可点击
        if(ui->processComboBox->currentText().isEmpty())
            ui->okPushButton->setDisabled(true);
        else
            ui->okPushButton->setDisabled(false);
    });

}

Widget::~Widget()
{
    delete ui;
}

void Widget::okBtnSlot()
{
    QString process = ui->processComboBox->currentText();
    if(process.isEmpty())
        return;

    if(StartProcess(process) == QProcess::NotRunning)
    {
        QMessageBox::critical(this, "process", "windows not find ,please comfirm file is true, try again");
        return;
    }

    //判断命令是否已经存在于缓冲中
    if(!CommandIsExisted(process))
    {
        //写入缓冲，程序下次运行读取
        WriteCommandToFile(FILEPATH, process);
    }


    this->close();
}

void Widget::cancelBtnSlot()
{
    this->close();
}

void Widget::browserBtnSlot()
{
    QString process_name = QFileDialog::getOpenFileName(this, tr("浏览"), QDir::currentPath(), tr("程序 (*.exe;*.pif;*.com;*.bat;*.cmd)"));

    if(process_name.isEmpty())
    {
        return;
    }

    ui->processComboBox->setCurrentText(process_name);
}

void Widget::returnPressedSlot()
{
    okBtnSlot();
}

QProcess::ProcessState Widget::StartProcess(const QString &process)
{
    QProcess* myProcess= new QProcess;
    myProcess->start(process.trimmed());
    return myProcess->state();
}

bool Widget::CommandIsExisted(const QString &process)
{
    for(int i = 0; i < ui->processComboBox->count(); ++i)
    {
        if(process == ui->processComboBox->itemText(i))
            return true;
    }
    return false;
}

void Widget::WriteCommandToFile(const QString &file_path, const QString &process)
{
    QFile file(file_path);
    file.open(QFile::Append);
    QTextStream stream(&file);
    stream << process << '\n';
    file.close();
}

QStringList Widget::ReadCommandToFile(const QString &file_path)
{
    QStringList list;
    QFile file(file_path);
    bool ok = file.open(QFile::ReadOnly);
    if(!ok)
        return list;
    QTextStream stream(&file);

    //按行读取的两种方式
    //1
    while(!stream.atEnd())
        list << stream.readLine();

    //2
//    QString line;
//    while (stream.readLineInto(&line)) {
//        list << line;
//    }
    file.close();
    return list;
}
