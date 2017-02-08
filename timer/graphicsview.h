#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsPolygonItem>
#include <QTimer>
#include <QMouseEvent>
#include <memory>

#include "ntouchclock.h"

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    GraphicsView(std::shared_ptr<NTouchClock> _ntclock, QWidget* _parent = 0);
    void setcountdown(bool _oniftrue){countdown_on = _oniftrue;}
    void setchrono(bool _oniftrue){chrono_on = _oniftrue;}
    bool getcountdown() const {return countdown_on;}
    bool getchrono() const {return chrono_on;}
    void reset();
    std::string lap();
    void init_countdown(int _hour, int _minute, int _sec, int _centisec);
    void countdown_alert();

protected:
    void setnclock(int _hour, int _minute, int _sec, int _centisec);
    void drawclock();
    void drawclocknum();
    void drawstart();
    void drawstop();
    void drawneedle();
    void drawdigits();
    void update_state();

private slots:
    void mouseReleaseEvent(QMouseEvent* e);
    void resizeEvent(QResizeEvent* e);
    void timerTimeout();

private:
    QGraphicsScene* scene;
    QGraphicsEllipseItem* ellipse;
    QGraphicsPolygonItem* start;
    QGraphicsPolygonItem* stop;
    QGraphicsTextItem* ltextitem;
    QGraphicsTextItem* rtextitem;
    QGraphicsTextItem* rsmalltextitem;
    QGraphicsTextItem* clocknumtextitem;
    QGraphicsLineItem* needle_gitem;
    QList<QGraphicsLineItem*> glitem_smallgraduation;
    QList<QGraphicsLineItem*> glitem_mediumgraduation;
    QList<QGraphicsTextItem*> qtitem_clocknum;
    QPen* outline_pen;
    QPen* highlight_pen;
    QPen* needle_pen;
    QFont* font;
    QFont* millifont;
    QFont* minifont;
    QColor* qgray;
    QColor* qgreen;
    QColor* needle_color;
    QTimer* quartz;

    std::shared_ptr<NTouchClock> ntclock;
    struct timeval tv_start, tv_current;
    long time_offset; // When the timer is paused
    long time_countdown;

    float ray, disk_diameter;
    float small_graduation_length, medium_graduation_length;
    bool chrono_on, countdown_on;
};

#endif // GRAPHICSVIEW_H
