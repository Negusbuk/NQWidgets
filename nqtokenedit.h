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

#ifndef NQTOKENEDIT_H
#define NQTOKENEDIT_H

#include <QString>
#include <QScrollArea>

#include <flowlayout.h>

class NQTokenEditItem;

class NQTokenEdit : public QScrollArea
{
    Q_OBJECT
    Q_PROPERTY(QColor itemColor READ itemColor WRITE setItemColor)
    Q_PROPERTY(int horizontalMargin READ horizontalMargin WRITE setHorizontalMargin)
    Q_PROPERTY(int verticalMargin READ verticalMargin WRITE setVerticalMargin)
    Q_PROPERTY(int horizontalSpacing READ horizontalSpacing WRITE setHorizontalSpacing)
    Q_PROPERTY(int verticalSpacing READ verticalSpacing WRITE setVerticalSpacing)

public:
    explicit NQTokenEdit(QWidget *parent = 0);

    const QStringList& tokens() { return tokens_; }

    const QColor& itemColor() const { return itemColor_; }
    int horizontalMargin() const { return horizontalMargin_; }
    int verticalMargin() const { return verticalMargin_; }
    int horizontalSpacing() const { return layout_->horizontalSpacing(); }
    int verticalSpacing() const { return layout_->verticalSpacing(); }

public slots:

    void clear();
    void setTokens(const QStringList& l);
    void appendTokens(const QStringList& l);

    void setItemColor(const QColor& color);
    void setHorizontalMargin(int margin);
    void setVerticalMargin(int margin);
    void setHorizontalSpacing(int spacing);
    void setVerticalSpacing(int spacing);

protected:

    virtual void mousePressEvent(QMouseEvent *ev);

    QStringList tokens_;
    FlowLayout* layout_;
    QList<NQTokenEditItem*> itemList_;

    QColor itemColor_;
    int horizontalMargin_;
    int verticalMargin_;

protected slots:

    void itemChanged(NQTokenEditItem*);

signals:

    void tokensChanged(const QStringList&);
};

#endif // NQTOKENEDIT_H
