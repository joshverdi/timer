#include <QPainter>
#include <QPolygonF>
#include <QTextDocument>
#include <QMediaPlayer>
#include <iostream>
#include <math.h>

#include "gettimeofday.h"
#include "graphicsview.h"

#define PI 3.14159265

GraphicsView::GraphicsView(std::shared_ptr<NTouchClock> _ntclock, QWidget* _parent)
    :QGraphicsView(_parent), ntclock(_ntclock)
{
    setBackgroundBrush(QBrush(QColor(Qt::black)));
    scene = new QGraphicsScene(this);
    setScene(scene);

    chrono_on = false;
    countdown_on = false;
    quartz = new QTimer(this);
    quartz->setInterval(50);
    quartz->start();

    qgray = new QColor(192,192,192);
    qgreen = new QColor(0,255,0);
    needle_color = new QColor(255,0,0);

    font = new QFont();
    font->setPixelSize(width()*0.40);
    font->setBold(true);
    font->setFamily("Arial");

    millifont = new QFont();
    millifont->setPixelSize(font->pixelSize()*32/40);
    millifont->setBold(true);
    millifont->setFamily("Arial");

    minifont = new QFont();
    minifont->setPixelSize(font->pixelSize()*32*0.3/40);
    minifont->setFamily("Arial");

    outline_pen = new QPen(*qgray);
    outline_pen->setWidth(2);

    highlight_pen = new QPen(*qgreen);
    highlight_pen->setWidth(2);

    needle_pen = new QPen(*needle_color);
    needle_pen->setWidth(2);


    ray = height()*3;
    disk_diameter = ray/2;
    time_offset = 0;
    time_countdown = 0;

    ltextitem  = new QGraphicsTextItem(QString(ntclock->hourstr().c_str())+QString(":")+QString(ntclock->minutestr().c_str()));
    ltextitem->setFont(*font);
    ltextitem->setDefaultTextColor(*qgray);
    ltextitem->setPos(-ray*2.5,-ray);

    rtextitem  = new QGraphicsTextItem(QString(ntclock->secstr().c_str())+QString("."));
    rtextitem->setFont(*font);
    rtextitem->setDefaultTextColor(*qgreen);
    rtextitem->setPos(ray+0.25f*ray,-ray);

    rsmalltextitem  = new QGraphicsTextItem(QString(ntclock->centisecstr().c_str()));
    rsmalltextitem->setFont(*millifont);
    rsmalltextitem->setDefaultTextColor(*qgreen);
    rsmalltextitem->setPos(ray+0.9f*ray,-ray*0.9f);

    setRenderHints(QPainter::Antialiasing);

    drawclock();
    drawclocknum();
    drawstart();
    drawdigits();
    drawneedle();

    connect(quartz, SIGNAL(timeout()), this, SLOT(timerTimeout()));
}

void GraphicsView::setnclock(int _hour, int _minute, int _sec, int _centisec)
{
    ntclock->sethour(_hour);
    ntclock->setminute(_minute);
    ntclock->setsec(_sec);
    ntclock->setcentisec(_centisec);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent* e)
{
    bool handled = false;

    // Compute norm from the disk center and see if it is less than the ray (0.50*diameter + arbitrary margin):
    if ((int)sqrt(pow(e->x()-(width()/2),2)+pow(e->y()-(height()/2),2)) < (disk_diameter*0.55))
    {
        update_state();
    }
    if (handled == false)
        QGraphicsView::mouseReleaseEvent(e);
}

void GraphicsView::resizeEvent(QResizeEvent* e)
{
    scene->removeItem(ltextitem);
    scene->removeItem(rtextitem);
    scene->removeItem(rsmalltextitem);
    delete ltextitem;
    delete rtextitem;
    delete rsmalltextitem;

    for (auto item: glitem_smallgraduation)
    {
        scene->removeItem(item);
    }
    for (auto item: glitem_mediumgraduation)
    {
        scene->removeItem(item);
    }
    scene->removeItem(ellipse);
    scene->removeItem(needle_gitem);

    scene->clear();

    delete scene;
    scene = new QGraphicsScene(this);
    setScene(scene);

    glitem_smallgraduation.clear();
    glitem_mediumgraduation.clear();
    ray = width()/5;
    disk_diameter = ray/2;

    ltextitem  = new QGraphicsTextItem(QString(ntclock->hourstr().c_str())+QString(":")+QString(ntclock->minutestr().c_str()));
    ltextitem->setFont(*font);
    ltextitem->setDefaultTextColor(*qgray);
    font->setPixelSize(width()*50/512);
    ltextitem->setPos(-width()/2,-ray);
    millifont->setPixelSize(font->pixelSize()*32/40);
    minifont->setPixelSize(font->pixelSize()*32*0.3/40);
    rtextitem  = new QGraphicsTextItem(QString(ntclock->secstr().c_str())+QString("."));
    rtextitem->setFont(*font);
    rtextitem->setDefaultTextColor(*qgreen);
    rtextitem->setPos(width()/4,-ray);
    rsmalltextitem  = new QGraphicsTextItem(QString(ntclock->centisecstr().c_str()));
    rsmalltextitem->setFont(*millifont);
    rsmalltextitem->setDefaultTextColor(*qgreen);
    rsmalltextitem->setPos(width()/4+rtextitem->document()->size().width()*0.9,-ray*0.9f);

    drawclock();
    drawclocknum();
    drawneedle();
    drawdigits();

    if (chrono_on==true)
    {
        if (scene->items().indexOf(start) != -1)
        {
            scene->removeItem(start);
            delete start;
        }
        drawstop();
    }
    else
    {
        if (scene->items().indexOf(stop) != -1)
        {
            scene->removeItem(stop);
            delete stop;
        }
        drawstart();
    }

    update();
    QGraphicsView::resizeEvent(e);
}

void GraphicsView::timerTimeout()
{
    if (countdown_on == false)
    {
        if (chrono_on == true)
        {
            gettimeofday(&tv_current, NULL);
            long current_amount = (tv_current.tv_usec + tv_current.tv_sec*1000000) - (tv_start.tv_usec + tv_start.tv_sec*1000000);
            ntclock->compute_time(current_amount+time_offset);
            scene->removeItem(needle_gitem);
            drawneedle();
            update();
            ltextitem->setPlainText(QString(ntclock->hourstr().c_str())+QString(":")+QString(ntclock->minutestr().c_str()));
            rtextitem->setPlainText(QString(ntclock->secstr().c_str())+QString("."));
            rsmalltextitem->setPlainText(QString(ntclock->centisecstr().c_str()));
            update();
        }
    }
    else
    {
        if (chrono_on == true)
        {
            gettimeofday(&tv_current, NULL);
            long current_amount = (tv_current.tv_usec + tv_current.tv_sec*1000000) - (tv_start.tv_usec + tv_start.tv_sec*1000000);
            if (current_amount+time_offset < time_countdown)
            {
                ntclock->compute_time(time_countdown-current_amount-time_offset);
                scene->removeItem(needle_gitem);
                drawneedle();
                update();
                ltextitem->setPlainText(QString(ntclock->hourstr().c_str())+QString(":")+QString(ntclock->minutestr().c_str()));
                rtextitem->setPlainText(QString(ntclock->secstr().c_str())+QString("."));
                rsmalltextitem->setPlainText(QString(ntclock->centisecstr().c_str()));
                update();
            }
            else
            {
                chrono_on = false;
                countdown_on = false;
                countdown_alert();
                // Setup Alert Here
            }
        }
    }
}

void GraphicsView::reset()
{
    chrono_on = false;
    time_offset = 0;
    ntclock->reset();

    scene->removeItem(ltextitem);
    scene->removeItem(rtextitem);
    scene->removeItem(rsmalltextitem);
    delete ltextitem;
    delete rtextitem;
    delete rsmalltextitem;

    for (auto item: glitem_smallgraduation)
    {
        scene->removeItem(item);
    }
    for (auto item: glitem_mediumgraduation)
    {
        scene->removeItem(item);
    }
    scene->removeItem(ellipse);
    scene->removeItem(needle_gitem);

    scene->clear();

    delete scene;
    scene = new QGraphicsScene(this);
    setScene(scene);

    glitem_smallgraduation.clear();
    glitem_mediumgraduation.clear();
    ray = width()/5;
    disk_diameter = ray/2;

    ltextitem  = new QGraphicsTextItem(QString(ntclock->hourstr().c_str())+QString(":")+QString(ntclock->minutestr().c_str()));
    ltextitem->setFont(*font);
    ltextitem->setDefaultTextColor(*qgray);
    font->setPixelSize(width()*50/512);
    ltextitem->setPos(-width()/2,-ray);
    millifont->setPixelSize(font->pixelSize()*32/40);
    minifont->setPixelSize(font->pixelSize()*32*0.3/40);
    rtextitem  = new QGraphicsTextItem(QString(ntclock->secstr().c_str())+QString("."));
    rtextitem->setFont(*font);
    rtextitem->setDefaultTextColor(*qgreen);
    rtextitem->setPos(width()/4,-ray);
    rsmalltextitem  = new QGraphicsTextItem(QString(ntclock->centisecstr().c_str()));
    rsmalltextitem->setFont(*millifont);
    rsmalltextitem->setDefaultTextColor(*qgreen);
    rsmalltextitem->setPos(width()/4+rtextitem->document()->size().width()*0.9,-ray*0.9f);

    drawclock();
    drawclocknum();
    drawneedle();
    drawdigits();

    if (scene->items().indexOf(stop) != -1)
    {
        scene->removeItem(stop);
        delete stop;
    }
    drawstart();
}

void GraphicsView::drawclock()
{
    QBrush graduation_brush(QColor(64,64,64));

    small_graduation_length = 0.05f*ray;
    ellipse = scene->addEllipse(-0.5f*disk_diameter, -0.5f*disk_diameter, disk_diameter, disk_diameter, *outline_pen, graduation_brush);
    for (int i = 0; i<60; i++)
    {
        float x1 = (float)(cos((90-6*i)*PI/180.0)*ray);
        float y1 = (float)(sin((90-6*i)*PI/180.0)*ray);
        float x2 = cos((90-6*i)*PI/180.0)*(ray+small_graduation_length);
        float y2 = sin((90-6*i)*PI/180.0)*(ray+small_graduation_length);

        glitem_smallgraduation.push_back(scene->addLine(    (int)x1,
                                                            (int)y1,
                                                            (int)x2,
                                                            (int)y2,
                                                            *outline_pen));
    }

    medium_graduation_length = 0.1f*ray;
    for (int i = 0; i<12; i++)
    {
        float x1 = (float)(cos((90-30*i)*PI/180.0)*ray);
        float y1 = (float)(sin((90-30*i)*PI/180.0)*ray);
        float x2 = cos((90-30*i)*PI/180.0)*(ray+medium_graduation_length);
        float y2 = sin((90-30*i)*PI/180.0)*(ray+medium_graduation_length);

        glitem_mediumgraduation.push_back(scene->addLine(   (int)x1,
                                                            (int)y1,
                                                            (int)x2,
                                                            (int)y2,
                                                            *outline_pen));
    }
}

void GraphicsView::drawclocknum()
{
    for (int i = 0; i<12; i++)
    {
        float x = (float)(cos((90-30*i)*PI/180.0)*ray*0.85f);
        float y = (float)(sin((90-30*i)*PI/180.0)*ray*0.85f);
        x=x-ray*0.08f;
        y=y+ray*0.1f;
        clocknumtextitem = new QGraphicsTextItem(std::to_string(5*i).c_str());
        clocknumtextitem->setFont(*minifont);
        clocknumtextitem->setDefaultTextColor(*qgray);
        clocknumtextitem->setPos(x,-y);
        scene->addItem(clocknumtextitem);
        qtitem_clocknum.push_back(clocknumtextitem);
    }
}

void GraphicsView::drawstart()
{
    float x = ray/10.0;
    float y = ray/10.0;
    QPolygonF triangle;
    triangle.append(QPointF(-x,+y));
    triangle.append(QPointF(+x,+0));
    triangle.append(QPointF(-x,-y));
    triangle.append(QPointF(-x,y));

    start = scene->addPolygon(triangle, *outline_pen/*, QBrush(QColor(255,0,0))*/);
}

void GraphicsView::drawstop()
{
    float x = ray/10.0;
    float y = ray/10.0;
    QPolygonF rect;
    rect.append(QPointF(-x,-y));
    rect.append(QPointF(-x,+y));
    rect.append(QPointF(+x,+y));
    rect.append(QPointF(+x,-y));
    rect.append(QPointF(-x,-y));

    stop = scene->addPolygon(rect, *needle_pen, QBrush(QColor(255,0,0)));
}

void GraphicsView::drawdigits()
{
    scene->addItem(ltextitem);
    scene->addItem(rtextitem);
    scene->addItem(rsmalltextitem);
}

void GraphicsView::drawneedle()
{
    float alpha = (90-6*ntclock->getsec()-6*ntclock->getcentisec()/100)*PI/(-180.0);
    float x1 = (float)(cos(alpha)*disk_diameter/(-1.75));
    float y1 = (float)(sin(alpha)*disk_diameter/(-1.75));
    float x2 = cos(alpha)*(-ray);
    float y2 = sin(alpha)*(-ray);
    needle_gitem = scene->addLine( (int)-x1,
                                   (int)-y1,
                                   (int)-x2,
                                   (int)-y2,
                                   *needle_pen);
}

void GraphicsView::update_state()
{
    long time_amount = 0;
    chrono_on = !chrono_on;
    ltextitem->setDefaultTextColor(*qgray);
    rtextitem->setDefaultTextColor(*qgreen);
    rsmalltextitem->setDefaultTextColor(*qgreen);
    if (countdown_on == false)
    {
        if (chrono_on == true)
        {
            gettimeofday(&tv_start, NULL);
            gettimeofday(&tv_current, NULL);
            time_amount = (tv_current.tv_usec + tv_current.tv_sec*1000000) - (tv_start.tv_usec + tv_start.tv_sec*1000000);
            ntclock->compute_time(time_amount+time_offset);
            if (scene->items().indexOf(start) != -1)
            {
                scene->removeItem(start);
                delete start;
            }
            drawstop();
        }
        else
        {
            gettimeofday(&tv_current, NULL);
            time_offset += (tv_current.tv_usec + tv_current.tv_sec*1000000) - (tv_start.tv_usec + tv_start.tv_sec*1000000);
            if (scene->items().indexOf(stop) != -1)
            {
                scene->removeItem(stop);
                delete stop;
            }
            drawstart();
        }
    }
    else
    {
        if (chrono_on == true)
        {
            gettimeofday(&tv_start, NULL);
            if (scene->items().indexOf(start) != -1)
            {
                scene->removeItem(start);
                delete start;
            }
            drawstop();
        }
        else
        {
            gettimeofday(&tv_current, NULL);
            time_offset += (tv_current.tv_usec + tv_current.tv_sec*1000000) - (tv_start.tv_usec + tv_start.tv_sec*1000000);
            if (scene->items().indexOf(stop) != -1)
            {
                scene->removeItem(stop);
                delete stop;
            }
            drawstart();
        }
    }
}

std::string GraphicsView::lap()
{
    std::string value;
    long time_amount;

    time_amount = (tv_current.tv_usec + tv_current.tv_sec*1000000) - (tv_start.tv_usec + tv_start.tv_sec*1000000);
    gettimeofday(&tv_current, NULL);
    if (countdown_on == false)
    {
        if (chrono_on == true)
            value = NTouchClock::compute_timestr(time_amount+time_offset);
    }
    else
    {
        if (chrono_on == true)
            value = NTouchClock::compute_timestr(time_countdown-time_amount-time_offset);
    }
    return value;
}

void GraphicsView::init_countdown(int _hour, int _minute, int _sec, int _centisec)
{
    reset();
    setnclock(_hour, _minute, _sec, _centisec);
    ltextitem->setPlainText(QString(ntclock->hourstr().c_str())+QString(":")+QString(ntclock->minutestr().c_str()));
    rtextitem->setPlainText(QString(ntclock->secstr().c_str())+QString("."));
    rsmalltextitem->setPlainText(QString(ntclock->centisecstr().c_str()));
    scene->removeItem(needle_gitem);
    drawneedle();
    time_countdown = (_centisec*10000)+(_sec*1000000)+(_minute*60*1000000)+(_hour*3600*1000000);
    update();
}

void GraphicsView::countdown_alert()
{
    QMediaPlayer* qmplayer;
    chrono_on = false;
    time_offset = 0;
    ntclock->reset();

    qmplayer = new QMediaPlayer();
    qmplayer->setMedia(QUrl::fromLocalFile(":/sound/arrived.mp3"));
    qmplayer->setVolume(50);
    qmplayer->play();
    delete qmplayer;

    scene->removeItem(ltextitem);
    scene->removeItem(rtextitem);
    scene->removeItem(rsmalltextitem);
    delete ltextitem;
    delete rtextitem;
    delete rsmalltextitem;

    for (auto item: glitem_smallgraduation)
    {
        scene->removeItem(item);
    }
    for (auto item: glitem_mediumgraduation)
    {
        scene->removeItem(item);
    }
    scene->removeItem(ellipse);
    scene->removeItem(needle_gitem);
    scene->clear();

    glitem_smallgraduation.clear();
    glitem_mediumgraduation.clear();
    ray = width()/5;
    disk_diameter = ray/2;

    ltextitem  = new QGraphicsTextItem(QString(ntclock->hourstr().c_str())+QString(":")+QString(ntclock->minutestr().c_str()));
    ltextitem->setFont(*font);
    ltextitem->setDefaultTextColor(Qt::red);
    font->setPixelSize(width()*50/512);
    ltextitem->setPos(-width()/2,-ray);
    millifont->setPixelSize(font->pixelSize()*32/40);
    minifont->setPixelSize(font->pixelSize()*32*0.3/40);
    rtextitem  = new QGraphicsTextItem(QString(ntclock->secstr().c_str())+QString("."));
    rtextitem->setFont(*font);
    rtextitem->setDefaultTextColor(Qt::red);
    rtextitem->setPos(width()/4,-ray);
    rsmalltextitem  = new QGraphicsTextItem(QString(ntclock->centisecstr().c_str()));
    rsmalltextitem->setFont(*millifont);
    rsmalltextitem->setDefaultTextColor(Qt::red);
    rsmalltextitem->setPos(width()/4+rtextitem->document()->size().width()*0.9,-ray*0.9f);

    drawclock();
    drawclocknum();
    drawneedle();
    drawdigits();

    if (scene->items().indexOf(stop) != -1)
    {
        scene->removeItem(stop);
        delete stop;
    }
    drawstart();
}

