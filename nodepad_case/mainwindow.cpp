#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QTextStream>
#include <QFile>
#include <QCloseEvent>
#include <QDateTime>
#include <QFont>
#include <QFontDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QLabel>
#include <QHBoxLayout>
#include "about.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#define DEFALUTCAPTION "无标题-记事本"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(DEFALUTCAPTION);
    ui->mainToolBar->setHidden(true);

    //设置状态栏的信息
    QLabel* codeLabel = new QLabel("Windows(CRLF)");
    codeLabel->setAlignment(Qt::AlignCenter);

    //设置label字体颜色
    codeLabel->setStyleSheet("color:black;");
    QLabel* blockColNumLabel = new QLabel("第1行，第1列");
    blockColNumLabel->setObjectName("blockColNumLabel");
    blockColNumLabel->setStyleSheet("color:black;");
    blockColNumLabel->setAlignment(Qt::AlignCenter);

    QLabel* label3 = new QLabel("100%");
    label3->setStyleSheet("color:black;");
    ui->statusBar->addPermanentWidget(new QLabel, 7);
    ui->statusBar->addPermanentWidget(codeLabel, 2);
    ui->statusBar->addPermanentWidget(blockColNumLabel, 2);
    ui->statusBar->addPermanentWidget(label3, 2);

    connect(ui->textEdit, &QTextEdit::textChanged, [&]()
    {
        ui->undoAction->setEnabled(true);
    });

    //动态设置快捷键示例
//    QAction* temp = this->findChild<QAction*>("newAction");
//    if(temp)
//    {
//        temp->setShortcut(Qt::CTRL | Qt::Key_N);
//        temp->setShortcutContext(Qt::ApplicationShortcut);
//    }

    //文件类动作
    connect(ui->newAction, &QAction::triggered, this, &MainWindow::newActionSlot);
    connect(ui->openAction, &QAction::triggered, this, &MainWindow::openActionSlot);
    connect(ui->saveAction, &QAction::triggered, this, &MainWindow::saveActionSlot);
    connect(ui->saveAsAction, &QAction::triggered, this, &MainWindow::saveAsActionSlot);
    connect(ui->exitAction, &QAction::triggered, this, [&](){
        //触发关闭事件
        this->close();
    });

    //编辑类动作
    connect(ui->dateTimeAction, &QAction::triggered, this, &MainWindow::dateTimeActionSlot);
    connect(ui->undoAction, &QAction::triggered, ui->textEdit, &QTextEdit::undo);
    connect(ui->cutAction, &QAction::triggered, ui->textEdit, &QTextEdit::cut);
    connect(ui->copyAction, &QAction::triggered, ui->textEdit, &QTextEdit::copy);
    connect(ui->pasteAction, &QAction::triggered, ui->textEdit, &QTextEdit::paste);



    connect(ui->delAction, &QAction::triggered, [&](){
        ui->textEdit->textCursor().removeSelectedText();

    });
    connect(ui->checkAllAction, &QAction::triggered, ui->textEdit, &QTextEdit::selectAll);

    //格式类动作
    connect(ui->wordWrapAction, &QAction::triggered, [&]()
    {
        if(ui->wordWrapAction->isChecked())
            ui->textEdit->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        else
            ui->textEdit->setWordWrapMode(QTextOption::NoWrap);

    });

    connect(ui->fontAction, &QAction::triggered, this, &MainWindow::fontActionSlot);

    //查看类动作
    connect( ui->zoomInAction, &QAction::triggered, [&](){
        ui->textEdit->zoomIn();
        zoom += 1;
    });

    connect( ui->zoomOutAction, &QAction::triggered, [&](){
        ui->textEdit->zoomOut();
        zoom -= 1;
    });

    connect( ui->restoreZoomAction, &QAction::triggered, [&](){
            ui->textEdit->zoomOut(zoom);
            zoom = 0;
    });

    connect( ui->statusBarAction, &QAction::triggered, [&](){
        if(ui->statusBarAction->isChecked()) //默认选中，再次点击为未选中
            ui->statusBar->setHidden(false);
        else
            ui->statusBar->setHidden(true);
    });

    //帮助类动作
    connect(ui->viewHelpAction, &QAction::triggered, [&]()
    {
        QDesktopServices::openUrl(QUrl("www.baidu.com"));
    });

    connect(ui->aboutNotepadaction, &QAction::triggered, [&]()
    {
        About* about = new About(this);
        about->exec();
    });

    connect(ui->aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);

    //游标位置改变时，获取文本游标的列和块的数值
    connect(ui->textEdit, &QTextEdit::cursorPositionChanged, [&]()
    {
        //获得列号
        int colNum = ui->textEdit->textCursor().columnNumber() + 1;
        //获得行号
        int blockNum = ui->textEdit->textCursor().blockNumber() + 1;

        QLabel* blockColNumLabel = ui->statusBar->findChild<QLabel*>("blockColNumLabel");
        if(blockColNumLabel)
            blockColNumLabel->setText(QString("第%1行，第%2列").arg(blockNum).arg(colNum));

    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << GetSavedFlag() << "&&" << ui->textEdit->document()->isModified();
    CloseLogicProcess(event);
}

void MainWindow::newActionSlot()
{
    //1、有打开或另存文件时， 检测到修改后，提示是否保存
    //2、无打开或另存文件时，检测到修改后，提示是否保存
    //3、无打开或另存文件时，未检测到修改，不提示保存

    //如果当前内容已经保存至文件或则编辑未作修改，则不做提示, 恢复初始状态
    if(GetSavedFlag() || !ui->textEdit->document()->isModified())
    {
        SetSavedFlag(false);
        RestoreInitState();
        return;
    }

    //提示逻辑处理
    TipLogicProcess();

}

void MainWindow::openActionSlot()
{
    //1、有打开或另存文件时， 检测到修改后，提示是否保存
    //2、无打开或另存文件时，检测到修改后，提示是否保存
    //3、无打开或另存文件时，未检测到修改，不提示保存
    //如果当前内容已经保存至文件或则编辑未作修改，则不做提示, 恢复初始状态
    if(GetSavedFlag() || !ui->textEdit->document()->isModified())
    {
        SetSavedFlag(false);
        QString file_path = QFileDialog::getOpenFileName(this, "打开", QDir::currentPath(), "文本文档(*.txt);;所有文档(*.*)");
        if(file_path.isEmpty())
            return;
        RestoreInitState();
        QString text;
        if(ReadFile(file_path, text))
            ModifyInitState(text, file_path);
        return;
    }

    //提示逻辑处理
    TipLogicProcess();

    QString file_path = QFileDialog::getOpenFileName(this, "打开", QDir::currentPath(), "文本文档(*.txt);;所有文档(*.*)");
    if(file_path.isEmpty())
        return;
    QString text;
    if(ReadFile(file_path, text))
        ModifyInitState(text, file_path);
}

void MainWindow::saveActionSlot()
{

    //1、如果当前有文件打开或另存，则将内容写入文件，否则另存为新文件
    if(m_current_file_path.isEmpty())
     {
        saveAsActionSlot();
        return;
     }

     SetSavedFlag(true);

     //判断编辑区域文件是否修改，若未修改则不写入文件
     if(!ui->textEdit->document()->isModified())
         return;

     if(WriteFile(m_current_file_path, ui->textEdit->toPlainText()))
         ModifyInitState(ui->textEdit->toPlainText(), m_current_file_path);
}

bool MainWindow::saveAsActionSlot()
{
    QString file_path = QFileDialog::getSaveFileName(this, "另存为", QDir::currentPath() + "/*.txt", "文本文档(*.txt);;所有文档(*.*)");
    if(file_path.isEmpty())
        return false;
    if(WriteFile(file_path, ui->textEdit->toPlainText()))
         ModifyInitState(ui->textEdit->toPlainText(), file_path);

    return true;
}

void MainWindow::dateTimeActionSlot()
{
    QDateTime date_time = QDateTime::currentDateTime();
    QString time = date_time.toString("yyyy-MM-dd hh:mm:ss");
    ui->textEdit->append(time);
}

void MainWindow::fontActionSlot()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, QFont("Helvetica [Cronyx]", 10), this);
    if (!ok) return;
    ui->textEdit->setFont(font);
}

bool MainWindow::WriteFile(const QString &file_path, const QString &text)
{
    QFile file(file_path);
    if(!file.open(QFile::WriteOnly))
    {
        qDebug() << "open file" << file_path << " error";
        return false;
    }

    QTextStream stream(&file);
    stream << text;
    file.close();
    return true;
}

bool MainWindow::ReadFile(const QString &file_path, QString &text)
{
    QFile file(file_path);
    if(!file.open(QFile::ReadOnly))
    {
        qDebug() << "open file" << file_path << " error";
        return false;
    }
    QTextStream stream(&file);
    text = stream.readAll();
    return true;
}

QString MainWindow::GetFileName(const QString &file_path)
{
    if(file_path.isEmpty())
        return "";
    int start_index = file_path.toStdString().rfind("/");
    QString file_name = file_path.mid(start_index + 1);
    return file_name;
}

void MainWindow::RestoreInitState()
{
    m_current_file_path.clear();
    ui->textEdit->clear();
    this->setWindowTitle(DEFALUTCAPTION);
}

void MainWindow::ModifyInitState(const QString& text, const QString& file_path)
{
    QString file_name = GetFileName(file_path);
    ui->textEdit->setText(text);
    this->setWindowTitle(file_name + "-记事本");
    m_current_file_path = file_path;
}

void MainWindow::TipLogicProcess()
{
    //显示中文的两种方式
//        QMessageBox msgBox;
//        msgBox.setWindowTitle("记事本");
//        msgBox.setText("是否将更改保存到无标题？");
//        msgBox.addButton("保存", QMessageBox::NoRole)->setDefault(true);
//        msgBox.addButton("不保存",QMessageBox::NoRole );
//        msgBox.addButton("取消", QMessageBox::NoRole);
//        int btn_ret = msgBox.exec();
    int btn_ret = QMessageBox::information(NULL, "记事本", "是否将更改保存到无标题？", "保存", "不保存", "取消", 0);
    switch (btn_ret) {
    case 0:
        if(!m_current_file_path.isEmpty())
        {
            //保存内容至文件成功，恢复初始状态
            if(WriteFile(m_current_file_path, ui->textEdit->toPlainText()))
                RestoreInitState();
        }
        else
        {
            //无文件打开为另存为成功
            if(saveAsActionSlot())
                RestoreInitState();
        }

        break;
    case 1: //恢复初始状态
        RestoreInitState();
        break;
    case 2:
        qDebug() << "cancel";
        break;
    default:
        break;
    }
}

void MainWindow::CloseLogicProcess(QCloseEvent* event)
{
    //已经保存或者未修改则关闭窗口
    if(GetSavedFlag() || !ui->textEdit->document()->isModified())
    {
        //accept后需要return否则程序往下运行
        event->accept();
        return;
    }

    int btn_ret = QMessageBox::information(NULL, "记事本", "是否将更改保存到无标题？", "保存", "不保存", "取消", 0);
    switch (btn_ret) {
    case 0:
        qDebug() << "save";
        if(!m_current_file_path.isEmpty())
        {
            qDebug() << "1";
            //保存内容至文件成功
            WriteFile(m_current_file_path, ui->textEdit->toPlainText());
            event->accept();
        }
        else
        {
            qDebug() << "2";
            //无文件打开为另存为成功
            saveAsActionSlot();
            event->ignore();
        }

        break;
    case 1: //恢复初始状态
        qDebug() << "nosave";
        RestoreInitState();
        event->accept();
        break;
    case 2:
        qDebug() << "cancel";
        event->ignore();
        break;
    default:
        break;
    }
}

void MainWindow::SetSavedFlag(bool flag)
{
    this->is_saved_flag = flag;
}

bool MainWindow::GetSavedFlag()
{
    return this->is_saved_flag;
}
