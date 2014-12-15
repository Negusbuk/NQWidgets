/****************************************************************************
 **
 **  Copyright (C) 2013 Andreas Mussgiller
 **
 **  This program is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **
 **
 ****************************************************************************/

#include <iostream>
#include <cmath>

#include <QPainter>
#include <QResizeEvent>
#include <QStyleOption>
#include <QtCore/qmath.h>
#include <QDebug>

#include "nqcolorwheel.h"

NQColorWheel::NQColorWheel(QWidget *parent) :
    QWidget(parent),
    size_(200, 200),
    margin_(5),
    wheelWidth_(20),
    minimumS_(0.0),
    currentColor_(Qt::white),
    mousePressed_(false),
    insideWheel_(false),
    insideSquare_(false)
{
    setFixedSize(size_);

    currentColor_ = currentColor_.toHsv();

    emit colorChange(currentColor_);
}

QSize NQColorWheel::sizeHint() const
{
    return QSize(width(), height());
}

QSize NQColorWheel::minimumSizeHint() const
{
    return size_;
}

void NQColorWheel::setColor(const QColor &color)
{
    int h = color.hue();
    int s = color.saturation();
    int v = color.value();

    currentColor_.setHsv(h, s, v);

    if (!isVisible()) return;

    initializeSquare(h);

    repaint();
}

void NQColorWheel::initializeWheel(const QSize& size)
{
    int r = qMin(size.width(), size.height());

    QStyleOption option;
    option.initFrom(this);

    QBrush background = option.palette.window();

    imageWheel_ = QImage(size, QImage::Format_ARGB32_Premultiplied);
    imageWheel_.fill(background.color());
    QPainter painter(&imageWheel_);
    painter.setRenderHint(QPainter::Antialiasing);

    QConicalGradient conicalGradient(0, 0, 0);
    conicalGradient.setColorAt(0.0, Qt::red);
    conicalGradient.setColorAt(60.0/360.0, Qt::yellow);
    conicalGradient.setColorAt(120.0/360.0, Qt::green);
    conicalGradient.setColorAt(180.0/360.0, Qt::cyan);
    conicalGradient.setColorAt(240.0/360.0, Qt::blue);
    conicalGradient.setColorAt(300.0/360.0, Qt::magenta);
    conicalGradient.setColorAt(1.0, Qt::red);

    painter.translate(r/2, r/2);

    QBrush brush(conicalGradient);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);
    painter.drawEllipse(QPoint(0,0), r/2-margin_,r/2-margin_);
    /* inner circle */
    painter.setBrush(background);
    painter.drawEllipse(QPoint(0,0), r/2-margin_-wheelWidth_, r/2-margin_-wheelWidth_);

    //caculate wheel region
    regionWheel_ = QRegion(r/2, r/2, r-2*margin_, r-2*margin_, QRegion::Ellipse);
    regionWheel_.translate(-(r-2*margin_)/2, -(r-2*margin_)/2);

    int tmp = 2*(margin_+wheelWidth_);
    QRegion subRe(r/2, r/2, r-tmp, r-tmp, QRegion::Ellipse);
    subRe.translate(-(r-tmp)/2, -(r-tmp)/2);
    regionWheel_ -= subRe;
}

void NQColorWheel::initializeSquare(int hue)
{
//    QPainter painter(&squarePixmap);
//    painter.setRenderHint(QPainter::Antialiasing);

    // region of the widget
    int w = qMin(width(), height());
    // radius of outer circle
    qreal r = w/2-margin_;
    // radius of inner circle
    qreal ir = r-wheelWidth_;
    // left corner of square
    qreal m = w/2.0-ir/qSqrt(2);
    //painter.translate(m, m);
    //painter.setPen(Qt::NoPen);
    QImage square(255, 255, QImage::Format_ARGB32_Premultiplied);
    QColor color;
    QRgb vv;
    for(int i=0;i<255;++i){
        for(int j=0;j<255;++j){
            color = QColor::fromHsv(hue, i * (1.0-minimumS_) + minimumS_*255, j);
            vv = qRgb(color.red(),color.green(),color.blue());
            square.setPixel(i,j,vv);
        }
    }
    qreal SquareWidth = 2*ir/qSqrt(2);
    imageSquare_ = square.scaled(SquareWidth, SquareWidth);

    regionSquare_ = QRegion(m, m, SquareWidth, SquareWidth);
}

void NQColorWheel::drawIndicator(int hue)
{
    QPainter painter(&pixmapWheel_);
    painter.setRenderHint(QPainter::Antialiasing);
    if(hue > 20 && hue < 200){
        painter.setPen(Qt::black);
    }else{
        painter.setPen(Qt::white);
    }
    painter.setBrush(Qt::NoBrush);

    QPen pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);
    qreal r = qMin(height(), width()) / 2.0;
    painter.translate(r, r);
    painter.rotate( -hue );
    r = qMin(height(), width()) / 2.0 - margin_ - wheelWidth_;
    painter.drawLine(QPointF(r+2,0.0), QPointF(r+wheelWidth_-2,0.0));
}

void NQColorWheel::drawPicker(const QColor &color)
{
    QPainter painter(&pixmapWheel_);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;

    // region of the widget
    int w = qMin(width(), height());
    // radius of outer circle
    qreal r = w/2-margin_;
    // radius of inner circle
    qreal ir = r-wheelWidth_;
    // left corner of square
    qreal m = w/2.0-ir/qSqrt(2);
    painter.translate(m-5, m-5);
    qreal SquareWidth = 2*ir/qSqrt(2);
    qreal S = (color.saturationF()-minimumS_)/(1.0-minimumS_)*SquareWidth;
    qreal V = color.valueF()*SquareWidth;

    if (color.saturation() > 30 || color.value() < 50){
        //pen.setColor(Qt::white);
    }

    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawEllipse(S, V, 8, 8);
}

void NQColorWheel::mousePressEvent(QMouseEvent *event)
{
    lastPosition_ = event->pos();

    if (regionWheel_.contains(lastPosition_)) {
        insideWheel_ = true;
        insideSquare_ = false;
        QColor color = colorFromPosition(lastPosition_);
        hueChanged(color.hue());
    } else if (regionSquare_.contains(lastPosition_)) {
        insideWheel_ = false;
        insideSquare_ = true;
        QColor color = colorFromPosition(lastPosition_);
        svChanged(color);
    }

    mousePressed_ = true;
}

void NQColorWheel::mouseMoveEvent(QMouseEvent *event)
{
    lastPosition_ = event->pos();

    if (!mousePressed_) return;

    if (insideWheel_) {
        QColor color = colorFromPosition(lastPosition_);
        if (color.isValid()) hueChanged(color.hue());
    } else if (insideSquare_){
        QColor color = colorFromPosition(lastPosition_);
        if (color.isValid()) svChanged(color);
    } else {
        // TODO: due with cursor out of region after press
        //        int length = qMin(width(), height());
        //        QPoint center(length/2, length/2);
        //        int R = qSqrt(qPow(qAbs(lastPos.x()), 2)
        //                      + qPow(qAbs(lastPos.y()), 2));
        //        if(inWheel){
        //            int r =  length / 2;
        //            r += qSqrt(qPow(center.x(), 2) + qPow(center.y(), 2));
        //            int x0 = r/R * qAbs(lastPos.x());
        //            int y0 = r/R * qAbs(lastPos.y());
        //            QColor color = posColor(QPoint(x0, y0));
        //            hueChanged(color.hue());
        //        }else if(inSquare){
        //            //
        //        }
    }
}

void NQColorWheel::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed_ = false;
    insideWheel_ = false;
    insideSquare_ = false;
}

void NQColorWheel::resizeEvent(QResizeEvent *event)
{
    pixmapWheel_ = QPixmap(event->size());
    pixmapWheel_.fill(palette().background().color());

    initializeWheel(event->size());
    initializeSquare(currentColor_.hue());

    update();
}

void NQColorWheel::paintEvent(QPaintEvent * /* e */)
{
    QPainter painter(this);

    QStyleOption opt;
    opt.initFrom(this);

    QPainter composePainter(&pixmapWheel_);
    composePainter.drawImage(0, 0, imageWheel_);
    composePainter.drawImage(regionSquare_.boundingRect().topLeft(), imageSquare_);
    composePainter.end();
    drawIndicator(currentColor_.hue());
    drawPicker(currentColor_);

    painter.drawPixmap(0, 0, pixmapWheel_);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

QColor NQColorWheel::colorFromPosition(const QPoint &point)
{
    if (!pixmapWheel_.rect().contains(point)) return QColor();

    if (insideWheel_) {
        qreal hue = 0;
        int r = qMin(width(), height()) / 2;
        if (point.x() > r) {
            if (point.y() < r) {
                //1
                hue = 90 - (qAtan2((point.x() - r) , (r - point.y()) )  / 3.14 / 2 * 360);
            } else {
                //4
                hue = 270 + (qAtan2( (point.x() - r) , (point.y() - r ) )  / 3.14 / 2 * 360);
            }
        } else {
            if (point.y() < r) {
                //2
                hue =  90 + (qAtan2( (r - point.x()) , (r - point.y()) )  / 3.14 / 2 * 360);
            } else {
                //3
                hue =  270 - (qAtan2( (r - point.x()) , (point.y() - r ))  / 3.14 / 2 * 360);
            }
        }
        hue = hue>359?359:hue;
        hue = hue<0?0:hue;
        return QColor::fromHsv(hue,
                               currentColor_.saturation(),
                               currentColor_.value());
    }

    if (insideSquare_) {
        // region of the widget
        int w = qMin(width(), height());
        // radius of outer circle
        qreal r = w/2-margin_;
        // radius of inner circle
        qreal ir = r-wheelWidth_;
        // left corner of square
        qreal m = w/2.0-ir/qSqrt(2);
        QPoint p = point - QPoint(m, m);
        qreal SquareWidth = 2*ir/qSqrt(2);
        qreal s = p.x()/SquareWidth;
        if (s<0) s = 0.0;
        if (s>1) s = 1.0;
        //if (s<minimumS_) s = minimumS_;
        qreal v = p.y()/SquareWidth;
        if (v<0) v = 0.0;
        if (v>1) v = 1.0;

        return QColor::fromHsvF(currentColor_.hueF(), minimumS_ + s*(1.0-minimumS_), v);
    }

    return QColor();
}

void NQColorWheel::hueChanged(int hue)
{
    if (hue<0 || hue>359) return;
    int s = currentColor_.saturation();
    int v = currentColor_.value();
    currentColor_.setHsv(hue, s, v);
    if (!isVisible()) return;
    //drawWheel(size());
    initializeSquare(hue);
    //drawIndicator(hue);
    //drawPicker(current);
    repaint();
    emit colorChange(currentColor_);
}

void NQColorWheel::svChanged(const QColor &newcolor)
{
    int hue = currentColor_.hue();
    currentColor_.setHsv(hue,
                         newcolor.saturation(),
                         newcolor.value());
    if (!isVisible()) return;
    //drawWheel(size());
    //drawSquare(hue);
    //drawIndicator(hue);
    //drawPicker(newcolor);
    repaint();
    emit colorChange(currentColor_);
}
