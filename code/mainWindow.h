#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "paintWindow.h"
#include <QWidget>
#include <QMainWindow>
#include <QLayout>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    paintWindow* p_w;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_start_clicked();//选定起始帧
    void on_end_clicked();//选定终点帧
    void on_clear_clicked();//清屏

    void on_line_clicked();//线性插值
    void on_vector1_clicked();//矢量线性差值
    void on_vector2_clicked();//矢量线性插值（矢量顺时针变换）
    void on_vector3_clicked();//矢量线性插值（矢量逆时针变换）

    void update_numbers();//更新关键点数目
    void on_change_clicked(); //改变画笔颜色

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
