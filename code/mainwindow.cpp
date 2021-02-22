#include "mainWindow.h"
#include "ui_mainwindow.h"
#include "paintWindow.h"

#include <QMovie>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->start->setDisabled(false);
    ui->end->setDisabled(true);
    ui->line->setDisabled(true);
    ui->vector1->setDisabled(true);
    ui->vector2->setDisabled(true);
    ui->vector3->setDisabled(true);

    p_w=new paintWindow;
    ui->horizontalLayout_4->addWidget(p_w);
    p_w->show();

    //设置部分控件的初始值和变化范围
    ui->spinBox_grain->setMinimum(1);  //最小值
    ui->spinBox_grain->setMaximum(990);  //最大值
    ui->spinBox_grain->setSingleStep(1);  //步长
    ui->spinBox_grain->setValue(50);  //初始值

    ui->spinBox_speed->setMinimum(1);  //最小值
    ui->spinBox_speed->setMaximum(990);  //最大值
    ui->spinBox_speed->setValue(50);  //初始值

    //如果用规范的槽函数再加上connect，函数会被调用两次
    //connect(ui->generate,SIGNAL(clicked()),this,SLOT(on_generate_clicked()),Qt::uniqueconnection);
    //connect(ui->clear,SIGNAL(clicked()),this,SLOT(on_clear_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//改变笔刷
void MainWindow::on_change_clicked()
{
    p_w->change_pen();
    update();
}

//生成初始帧并绘制图像
void MainWindow::on_start_clicked()
{
    int num=p_w->numbers();
    if(num==0)return;  //没有指定关键点

    QString temp="  "+QString::number(num);
    ui->number2->setText(temp);

    ui->start->setDisabled(true);
    ui->end->setDisabled(false);

    p_w->change_frame();

    //刷新
    update();
}

//生成终止帧并绘制图像
void MainWindow::on_end_clicked()
{

    ui->end->setDisabled(true);
    ui->line->setDisabled(false);
    ui->vector1->setDisabled(false);
    ui->vector2->setDisabled(false);
    ui->vector3->setDisabled(false);

    p_w->finish_frame();

    //刷新
    update();
}

//清屏槽函数
void MainWindow::on_clear_clicked()
{
    p_w->clear();

    //清除关键点个数和曲线总长度
    int _temp=0;
    QString temp=QString::number(_temp);
    ui->number2->setText(temp);

    ui->start->setDisabled(false);
    ui->end->setDisabled(true);
    ui->line->setDisabled(true);
    ui->vector1->setDisabled(true);
    ui->vector2->setDisabled(true);
    ui->vector3->setDisabled(true);
}

//线性插值
void MainWindow::on_line_clicked()
{
    double _speed=ui->spinBox_speed->value();
    double _grain=ui->spinBox_grain->value();
    int _mode=0;
    p_w->set_interpolation(_grain, _speed, _mode);
}


//矢量线性插值
void MainWindow::on_vector1_clicked()
{
    double _speed=ui->spinBox_speed->value();
    double _grain=ui->spinBox_grain->value();
    int _mode=1;
    p_w->set_interpolation(_grain, _speed, _mode);
}

//矢量线性插值（顺时针）
void MainWindow::on_vector2_clicked()
{
    double _speed=ui->spinBox_speed->value();
    double _grain=ui->spinBox_grain->value();
    int _mode=2;
    p_w->set_interpolation(_grain, _speed, _mode);
}

//矢量线性插值（逆时针）
void MainWindow::on_vector3_clicked()
{
    double _speed=ui->spinBox_speed->value();
    double _grain=ui->spinBox_grain->value();
    int _mode=3;
    p_w->set_interpolation(_grain, _speed, _mode);
}

//更新关键点的数量
void MainWindow::update_numbers()
{
    int num=p_w->numbers();
    QString temp=QString::number(num);
    temp="   "+temp;
    ui->number2->setText(temp);
}

