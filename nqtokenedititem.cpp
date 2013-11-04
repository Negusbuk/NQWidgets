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

#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <QLayout>

#include "nqtokenedititem.h"

NQTokenEditItem::NQTokenEditItem(int idx,
                                 const QColor& color, int hmargin, int vmargin,
                                 QWidget *parent)
    : QLabel(parent),
      index_(idx),
      currentCursorPos_(0),
      itemColor_(color),
      horizontalMargin_(hmargin),
      verticalMargin_(vmargin)
{
    setTokenText("");
    setFocus();
}

NQTokenEditItem::NQTokenEditItem(QString label, int idx,
                                 const QColor& color, int hmargin, int vmargin,
                                 QWidget *parent)
    : QLabel(label, parent),
      index_(idx),
      currentCursorPos_(-1),
      itemColor_(color),
      horizontalMargin_(hmargin),
      verticalMargin_(vmargin)
{
    setTokenText(label);
}

void NQTokenEditItem::setTokenText(const QString& text)
{
    layout_.setText(text);
    layout_.beginLayout();
    layout_.createLine();
    layout_.endLayout();
    int width = layout_.lineAt(0).naturalTextWidth();
    int height = layout_.lineAt(0).height();
    lineHeight_ = height;
    setFixedWidth(width + horizontalMargin_*2);
    setFixedHeight(height + verticalMargin_*4);
}

void NQTokenEditItem::paintEvent(QPaintEvent *)
{
    QStyle *style = QWidget::style();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect fr = frameRect();

    QStyleOption opt;
    opt.initFrom(this);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(itemColor_.darker()));
    painter.drawRoundedRect(fr, fr.height()/2, fr.height()/2);

    QRect fr2;
    fr2.setX(1);
    fr2.setY(1);
    fr2.setWidth(fr.width()-2);
    fr2.setHeight(fr.height()-2);
    painter.setBrush(QBrush(itemColor_));
    painter.drawRoundedRect(fr2, fr2.height()/2, fr2.height()/2);

    fr2.setY(verticalMargin_*2);
    fr2.setHeight(lineHeight_);
    style->drawItemText(&painter, fr2, Qt::AlignHCenter,
                        opt.palette, true, layout_.text(), foregroundRole());

    if (currentCursorPos_!=-1) {
        QPointF topLeft = fr2.topLeft();
        topLeft.setX(topLeft.x() + horizontalMargin_ - 2);
        topLeft.setY(topLeft.y());
        layout_.drawCursor(&painter, topLeft, currentCursorPos_, 1);
    }
}

void NQTokenEditItem::mousePressEvent(QMouseEvent *ev)
{
    setFocus();

    const QTextLine& line = layout_.lineAt(0);
    int x = ev->pos().x();
    currentCursorPos_ = line.xToCursor(x - horizontalMargin_);
}

void NQTokenEditItem::keyPressEvent(QKeyEvent * ev)
{
    int key = ev->key();

    if (key==Qt::Key_Enter || key==Qt::Key_Return) {
        clearFocus();
        return;
    } else if (key==Qt::Key_Left) {
        if (currentCursorPos_>0) {
            currentCursorPos_--;
            update();
        }
    } else if (key==Qt::Key_Right) {
        if (currentCursorPos_<layout_.text().length()) {
            currentCursorPos_++;
            update();
        }
    } else if (key==Qt::Key_Backspace) {
        if (currentCursorPos_>0) {
            QString text = layout_.text().remove(currentCursorPos_-1, 1);
            setTokenText(text);
            currentCursorPos_--;
        }
    } else if (key==Qt::Key_Delete) {
        QString text = layout_.text();
        if (currentCursorPos_<text.length()) {
            text = text.remove(currentCursorPos_, 1);
            setTokenText(text);
        }
    } else {
        QString text = ev->text();
        if (text.length()!=1) return;
        QChar c = text[0];
        if (c.isLetterOrNumber() || c.isSpace()) {
            text = layout_.text().insert(currentCursorPos_, c);
            setTokenText(text);
            currentCursorPos_++;
        }
    }
}

void NQTokenEditItem::focusOutEvent(QFocusEvent * /* ev */)
{
    currentCursorPos_ = -1;
    update();
    emit itemChanged(this);
}

void NQTokenEditItem::changeEvent(QEvent * event)
{
    if (event->type()!=QEvent::FontChange) return;

    layout_.setFont(font());
    setTokenText(layout_.text());
}

void NQTokenEditItem::setItemColor(const QColor& color)
{
    itemColor_ = color;
    repaint();
}

void NQTokenEditItem::setHorizontalMargin(int margin)
{
    horizontalMargin_ = margin;
    setTokenText(layout_.text());
    repaint();
}

void NQTokenEditItem::setVerticalMargin(int margin)
{
    verticalMargin_ = margin;
    setTokenText(layout_.text());
    repaint();
}
