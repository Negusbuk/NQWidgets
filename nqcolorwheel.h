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

#ifndef NQCOLORWHEEL_H
#define NQCOLORWHEEL_H

#include <QWidget>
#include <QPainter>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QConicalGradient>

class NQColorWheel : public QWidget
{
    Q_OBJECT
public:
    explicit NQColorWheel(QWidget *parent = 0);

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
    const QColor& color() const { return currentColor_; }

signals:

    void colorChange(const QColor &color);

public slots:

    void setColor(const QColor &color);

protected:

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *e);

    void initializeWheel(const QSize& size);
    void initializeSquare(int hue);
    void drawIndicator(int hue);
    void drawPicker(const QColor &color);

    QColor colorFromPosition(const QPoint &point);

    void hueChanged(int hue);
    void svChanged(const QColor &newcolor);

    QSize size_;
    int margin_;
    int wheelWidth_;
    QImage imageWheel_;
    QPixmap pixmapWheel_;
    QRegion regionWheel_;
    QImage imageSquare_;
    QRegion regionSquare_;

    QColor currentColor_;

    bool mousePressed_;
    bool insideWheel_;
    bool insideSquare_;
    QPoint lastPosition_;
};

#endif // NQCOLORWHEEL_H
