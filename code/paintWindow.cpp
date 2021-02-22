#include "paintWindow.h"
#include <iostream>
#include "math.h"

//构造函数
paintWindow::paintWindow()
{
    //设置画板宽度
    this->setMinimumWidth(10000);
    this->setMinimumHeight(800);

    timer = new QTimer();
    time=0;
    connect(timer,SIGNAL(timeout()),this,SLOT(changeState()));
}

//从起始帧切换到终点帧
void paintWindow::change_frame()
{
    startframe=false;
    start_draw=true;
    update();
}

//结束终点帧选定
void paintWindow::finish_frame()
{
    endframe=false;
    end_draw=true;
    update();

    //计算起始帧的向量
    calc_vectors();
}

void paintWindow::calc_vectors()
{
    //起点帧和终点帧关键点数目不一致，不能进行计算。
    if(start_points.size()!=end_points.size())
    {
        cout<<"Wrong!!!Different number of points!..."<<endl;
        return;
    }
    //计算起始帧向量
    for(int i=0;i<start_points.size()-1;i++)
    {
        double x=start_points[i+1].x()-start_points[i].x();
        double y=start_points[i+1].y()-start_points[i].y();
        double _p=sqrt(pow(x,2)+pow(y,2));
        double _a=atan2(y,x);
        Vector temp;
        temp.a=_a;
        temp.p=_p;
        start_vectors.push_back(temp);
    }
    //计算终止帧向量
    for(int i=0;i<end_points.size()-1;i++)
    {
        double x=end_points[i+1].x()-end_points[i].x();
        double y=end_points[i+1].y()-end_points[i].y();
        double _p=sqrt(pow(x,2)+pow(y,2));
        double _a=atan2(y,x);
        Vector temp;
        temp.a=_a;
        temp.p=_p;
        end_vectors.push_back(temp);
    }
}

//设置插值参数
void paintWindow::set_interpolation(int _grain,int _speed, int _mode)
{
    grain=_grain;
    speed=_speed;
    mode=_mode;
    timer->start(speed);//fps
}

//鼠标回调函数，鼠标点击，加入关键点
void paintWindow::mousePressEvent(QMouseEvent *e)
{
    if(startframe)start_points.push_back(e->pos());
    else if (endframe) end_points.push_back(e->pos());
    update();
}

//绘制函数
void paintWindow::paintEvent(QPaintEvent *)
{
    if(time==0) pen_index=0;
    QPainter paint(this);
    if(start_points.size()<=0)return;  //没有关键点

    //设置笔刷样式，绘制关键点
    paint.setPen(QPen(Qt::black,5,Qt::DashDotLine,Qt::RoundCap));
    for(int i=0;i<start_points.size();i++)
    {
        paint.drawEllipse(start_points[i],1,1);
    }
    for(int i=0;i<end_points.size();i++)
    {
        paint.drawEllipse(end_points[i],1,1);
    }

    //设置笔刷样式，绘制起点帧图像
    //paint.setPen(QPen(Qt::blue,3,Qt::SolidLine,Qt::RoundCap));
    switch(pen_index)
    {
    case 0:paint.setPen(QPen(Qt::gray,3,Qt::SolidLine,Qt::RoundCap));break;
    case 1:paint.setPen(QPen(Qt::yellow,3,Qt::SolidLine,Qt::RoundCap));break;
    case 2:paint.setPen(QPen(Qt::blue,3,Qt::SolidLine,Qt::RoundCap));break;
    case 3:paint.setPen(QPen(Qt::green,3,Qt::SolidLine,Qt::RoundCap));break;
    }
    if(start_points.size()>0)
    {
        if(start_draw)
        {
            for(unsigned int i=0;i<start_points.size()-1;i++)
            {
                QPoint p1=start_points[i];
                QPoint p2=start_points[i+1];
                paint.drawLine(p1,p2);
            }
            QPoint p1=start_points[0];
            QPoint p2=start_points[start_points.size()-1];
            paint.drawLine(p1,p2);
        }

    }

    //设置笔刷样式，绘制终点帧图像
    if(end_points.size()>0)
    {
        if(end_draw)
        {
            for(unsigned int i=0;i<end_points.size()-1;i++)
            {
                QPoint p1=end_points[i];
                QPoint p2=end_points[i+1];
                paint.drawLine(p1,p2);
            }
            QPoint p1=end_points[0];
            QPoint p2=end_points[end_points.size()-1];
            paint.drawLine(p1,p2);
        }

    }

    //设置笔刷样式，绘制插值帧图像
    //paint.setPen(QPen(Qt::red,3,Qt::DotLine,Qt::RoundCap));
    switch(pen_index)
    {
    case 0:paint.setPen(QPen(Qt::red,3,Qt::DotLine,Qt::RoundCap));break;
    case 1:paint.setPen(QPen(Qt::green,3,Qt::DotLine,Qt::RoundCap));break;
    case 2:paint.setPen(QPen(Qt::yellow,3,Qt::DotLine,Qt::RoundCap));break;
    case 3:paint.setPen(QPen(Qt::blue,3,Qt::DotLine,Qt::RoundCap));break;
    }
    if(time!=0)
    {
        if(inter_points.size()>0)
        {
            for(unsigned int i=0;i<inter_points.size()-1;i++)
            {
                QPoint p1=inter_points[i];
                QPoint p2=inter_points[i+1];
                paint.drawLine(p1,p2);
            }
            QPoint p1=inter_points[0];
            QPoint p2=inter_points[inter_points.size()-1];
            paint.drawLine(p1,p2);
        }
    }
    //change_pen(paint);
}


//计时器关联槽函数，改变插值帧数据
void paintWindow::changeState()
{
    //起点帧和终点帧关键点数目不一致，不能进行插值帧操作。
    if(start_points.size()!=end_points.size())
    {
        cout<<"Wrong!!!Different number of points!..."<<endl;
        timer->stop();
        time=0;
        return;
    }
    //插值点动画结束
    if(time>grain)
    {
        timer->stop();
        time=0;
        return;
    }

    //mode==0:线性插值
    if(mode==0)
    {
        inter_points.clear();
        double t=1.0*time/grain;
        for(int i=0;i<start_points.size();i++)
        {
            QPoint temp;
            double x0=start_points[i].x();
            double y0=start_points[i].y();
            double x1=end_points[i].x();
            double y1=end_points[i].y();
            double x=(1-t)*x0+t*x1;
            double y=(1-t)*y0+t*y1;

            temp.setX(x);
            temp.setY(y);
            inter_points.push_back(temp);
        }
    }
    //mode==1:普通矢量线性插值（不规定矢量插值方向）
    else if(mode==1)
    {
        inter_points.clear();
        double t=1.0*time/grain;
        QPoint temp;
        double x0=start_points[0].x();
        double y0=start_points[0].y();
        double x1=end_points[0].x();
        double y1=end_points[0].y();
        double x=(1-t)*x0+t*x1;
        double y=(1-t)*y0+t*y1;
        temp.setX(x);
        temp.setY(y);
        inter_points.push_back(temp);
        for(int i=0;i<start_vectors.size();i++)
        {
            double vec_a0=start_vectors[i].a;
            double vec_p0=start_vectors[i].p;
            double vec_a1=end_vectors[i].a;
            double vec_p1=end_vectors[i].p;
            if(vec_a0<0)vec_a0+=2*PI;
            if(vec_a1<0)vec_a1+=2*PI;
            if(vec_a1-vec_a0>PI)vec_a0+=2*PI;
            if(vec_a1-vec_a0<-PI)vec_a1+=2*PI;

            double vec_a=(1-t)*vec_a0+t*vec_a1;
            double vec_p=(1-t)*vec_p0+t*vec_p1;
            x+=vec_p*cos(vec_a);
            y+=vec_p*sin(vec_a);
            temp.setX(x);
            temp.setY(y);
            inter_points.push_back(temp);
        }
    }
    //mode==2:顺时针矢量线性插值
    else if(mode==2)
    {
        inter_points.clear();
        double t=1.0*time/grain;
        QPoint temp;
        double x0=start_points[0].x();
        double y0=start_points[0].y();
        double x1=end_points[0].x();
        double y1=end_points[0].y();
        double x=(1-t)*x0+t*x1;
        double y=(1-t)*y0+t*y1;
        temp.setX(x);
        temp.setY(y);
        inter_points.push_back(temp);
        for(int i=0;i<start_vectors.size();i++)
        {
            double vec_a0=start_vectors[i].a;
            double vec_p0=start_vectors[i].p;
            double vec_a1=end_vectors[i].a;
            double vec_p1=end_vectors[i].p;
            if(vec_a1<vec_a0)vec_a1+=2*PI;
            double vec_a=(1-t)*vec_a0+t*vec_a1;
            double vec_p=(1-t)*vec_p0+t*vec_p1;
            x+=vec_p*cos(vec_a);
            y+=vec_p*sin(vec_a);
            temp.setX(x);
            temp.setY(y);
            inter_points.push_back(temp);
        }
    }
    //mode==3:逆时针矢量线性插值
    else if(mode==3)
    {
        inter_points.clear();
        double t=1.0*time/grain;
        QPoint temp;
        double x0=start_points[0].x();
        double y0=start_points[0].y();
        double x1=end_points[0].x();
        double y1=end_points[0].y();
        double x=(1-t)*x0+t*x1;
        double y=(1-t)*y0+t*y1;
        temp.setX(x);
        temp.setY(y);
        inter_points.push_back(temp);
        for(int i=0;i<start_vectors.size();i++)
        {
            double vec_a0=start_vectors[i].a;
            double vec_p0=start_vectors[i].p;
            double vec_a1=end_vectors[i].a;
            double vec_p1=end_vectors[i].p;
            if(vec_a0<vec_a1)vec_a0+=2*PI;
            double vec_a=(1-t)*vec_a0+t*vec_a1;
            double vec_p=(1-t)*vec_p0+t*vec_p1;
            x+=vec_p*cos(vec_a);
            y+=vec_p*sin(vec_a);
            temp.setX(x);
            temp.setY(y);
            inter_points.push_back(temp);
        }
    }
    time++;
    update();
}


//清屏
void paintWindow::clear()
{
    //储存关键点数据的vector清除
    start_points.clear();
    end_points.clear();
    start_vectors.clear();
    end_vectors.clear();
    inter_points.clear();
    start_draw=false;
    end_draw=false;
    startframe=true;
    endframe=true;
    timer->stop();
    time = 0;
    update();
}


//获取当前关键点的个数
int paintWindow::numbers()
{
    if(startframe) return start_points.size();
    else return end_points.size();
}

//更换不同样式的笔刷
void paintWindow::change_pen()
{
    pen_index=(pen_index+1)%4;
    update();
}
