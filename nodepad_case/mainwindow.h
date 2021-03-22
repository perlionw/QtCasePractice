#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //重写窗口关闭事件
    virtual void closeEvent(QCloseEvent *);

private slots:
    void newActionSlot();
    void openActionSlot();
    void saveActionSlot();
    bool saveAsActionSlot();
    void dateTimeActionSlot();
    void fontActionSlot();

private:
    //写入文件
    bool WriteFile(const QString& file_path, const QString& text);

    //读取文件
    bool ReadFile(const QString& file_path, QString& text);

    //根据文件路径获取文件名
    QString GetFileName(const QString& file_path);


    //恢复初始状态
    void RestoreInitState();

    //修改初始状态
    void ModifyInitState(const QString& text, const QString& file_path);

    //提示逻辑处理
    void TipLogicProcess();



    //推出逻辑处理
    void CloseLogicProcess(QCloseEvent*);

    void SetSavedFlag(bool flag);
    bool GetSavedFlag();

private:
    Ui::MainWindow *ui;

    //当前打开的路径
    QString m_current_file_path;

    //标记有文件打开时的保存标识
    bool is_saved_flag = false;

    //当前缩放值, 0 代表初始值
    int zoom = 0;

};

#endif // MAINWINDOW_H
