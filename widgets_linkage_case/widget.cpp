#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->dial, &QDial::valueChanged, ui->horizontalSlider, &QSlider::setValue);
    connect(ui->horizontalSlider, &QSlider::valueChanged, ui->dial, &QDial::setValue);
    connect(ui->spinBox, SIGNAL(valueChanged(int)), ui->dial, SLOT(setValue(int)));
    connect(ui->dial, &QDial::valueChanged, ui->spinBox, &QSpinBox::setValue);
    connect(ui->spinBox, SIGNAL(valueChanged(int)), ui->horizontalSlider, SLOT(setValue(int)));
    connect(ui->horizontalSlider, &QSlider::valueChanged, ui->spinBox, &QSpinBox::setValue);
}

Widget::~Widget()
{
    delete ui;
}
