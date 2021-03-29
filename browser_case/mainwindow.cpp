#include "mainwindow.h"
#include "ui_mainwindow.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    progressBar =NULL;
    progressBar = new QProgressBar;
    ui->statusBar->addWidget(progressBar);
    connect(ui->webView, &QWebView::loadProgress, progressBar, &QProgressBar::setValue);
    connect(ui->urlLineEdit, &QLineEdit::returnPressed, [&]()
    {
        progressBar = new QProgressBar;
        ui->statusBar->addWidget(progressBar);
        connect(ui->webView, &QWebView::loadProgress, progressBar, &QProgressBar::setValue);
        ui->webView->load(QUrl(ui->urlLineEdit->text()));
        qDebug() << "清理前：" << ui->statusBar->children().count();
    });


    connect(ui->webView, &QWebView::loadFinished, [&](bool ok)
    {
        if(ok)
        {
            if(progressBar)
            {
                delete progressBar;
                progressBar = NULL;
            }
            qDebug() << "清理后: " << ui->statusBar->children().count();
            ui->statusBar->showMessage("加载完成", 5*1000);
        }
    });

    connect(ui->forwardPushButton, &QPushButton::clicked, ui->webView, &QWebView::forward);
    connect(ui->backPushButton, &QPushButton::clicked, ui->webView, &QWebView::back);
    connect(ui->reloadPushButton, &QPushButton::clicked, ui->webView, &QWebView::reload);

    connect(ui->webView, &QWebView::urlChanged, [&](const QUrl& url){
        ui->urlLineEdit->setText(url.toString());
    });

    connect(ui->webView, &QWebView::titleChanged, [&](const QString& title)
    {
        this->setWindowTitle(title);
    });

    ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(ui->webView, &QWebView::linkClicked, [&](const QUrl& url)
    {
        progressBar = new QProgressBar;
        ui->statusBar->addWidget(progressBar);
        connect(ui->webView, &QWebView::loadProgress, progressBar, &QProgressBar::setValue);
        ui->webView->load(url);
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}
