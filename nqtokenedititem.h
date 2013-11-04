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

#ifndef NQTOKENEDITITEM_H
#define NQTOKENEDITITEM_H

#include <QLabel>
#include <QTextLayout>

class NQTokenEditItem : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QColor itemColor READ itemColor WRITE setItemColor)
    Q_PROPERTY(int horizontalMargin READ horizontalMargin WRITE setHorizontalMargin)
    Q_PROPERTY(int verticalMargin READ verticalMargin WRITE setVerticalMargin)

public:
    explicit NQTokenEditItem(int idx,
                             const QColor& color, int hmargin, int vmargin,
                             QWidget *parent = 0);
    explicit NQTokenEditItem(QString label, int idx,
                             const QColor& color, int hmargin, int vmargin,
                             QWidget *parent = 0);

    const QString tokenText() { return layout_.text(); }
    int index() const { return index_; }

    const QColor& itemColor() const { return itemColor_; }
    int horizontalMargin() const { return horizontalMargin_; }
    int verticalMargin() const { return verticalMargin_; }

public slots:

    void setTokenText(const QString& text);

    void setItemColor(const QColor& color);
    void setHorizontalMargin(int margin);
    void setVerticalMargin(int margin);

protected:

    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void keyPressEvent(QKeyEvent * e);
    virtual void focusOutEvent(QFocusEvent * ev);
    virtual void paintEvent(QPaintEvent *ev);
    virtual void changeEvent(QEvent * event);

    int index_;
    QTextLayout layout_;
    int lineHeight_;
    int currentCursorPos_;

    QColor itemColor_;
    int horizontalMargin_;
    int verticalMargin_;

signals:

    void itemChanged(NQTokenEditItem*);
};

#endif // NQTOKENEDITITEM_H
