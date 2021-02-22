#ifndef PAINTWINDOW_H
#define PAINTWINDOW_H
#include <QCoreApplication>
#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QTimer>
#include <QPainter>
#include <QPen>
#define PI 3.1415926535

using namespace std;
class Timer;
class Vector
{
public:
    double a;
    double p;
};

class paintWindow : public QWidget
{
    Q_OBJECT
private:
    int grain;  //每个曲线区间有多少个插值点（包括两端关键点）
    int speed;  //刷新速度（改为int）
    int mode=0; //插值模式
    int pen_index=0;//使用第几种笔刷
    int time;//时间
    QTimer* timer;  //计时器

    vector<QPoint> start_points;//储存起点帧点坐标
    vector<Vector> start_vectors;//储存起点帧向量
    vector<QPoint> end_points;//储存终点帧点坐标
    vector<Vector> end_vectors;//储存终点帧向量
    vector<QPoint> inter_points;//储存当前插值帧关键点

    bool start_draw=false; //是否绘制起始帧图像
    bool end_draw=false; //是否绘制终止帧图像

    bool startframe=true; //是否处于起始帧选定状态
    bool endframe=true; //是否处于终止帧选定状态

public:
    paintWindow();

    void change_frame(); //从起始帧切换到终止帧
    void finish_frame(); //结束终止帧交互
    void calc_vectors();  //计算关键帧向量

    void set_interpolation(int _grain,int _speed, int _mode);  //设置动画参数
    void paintEvent(QPaintEvent *);  //绘制函数
    void mousePressEvent(QMouseEvent *e);  //鼠标回调函数
    int numbers();  //关键点个数
    void change_pen();  //改变笔刷
    void clear(); //清屏
private slots:
    void changeState();  //连接计时器，改变小车坐标，旋转角度等信息
};


#endif // PAINTWINDOW_H
