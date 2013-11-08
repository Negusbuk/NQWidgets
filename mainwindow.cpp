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

#include <QTabWidget>

#include "nqcolorwheel.h"

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QTabWidget *central = new QTabWidget(this);
    setCentralWidget(central);

    QWidget* content;
    QBoxLayout* layout;

    content = new QWidget(this);
    layout = new QVBoxLayout();
    content->setLayout(layout);

    tokenedit_ = new NQTokenEdit(this);
    tokenedit_->setMinimumWidth(300);
    tokenedit_->setMinimumHeight(200);
    tokenedit_->setHorizontalMargin(10);

    layout->addWidget(tokenedit_);

    tokenlist_ = new QListWidget(this);
    tokenlist_->setMinimumHeight(200);
    layout->addWidget(tokenlist_);

    connect(tokenedit_, SIGNAL(tokensChanged(QStringList)),
            this, SLOT(tokensChanged(QStringList)));

    QStringList l;
    l << "One" << "Two" << "Three";
    l << "Four" << "Five" << "Six";
    l << "Seven" << "Eight" << "Nine";
    tokenedit_->setTokens(l);

    central->addTab(content, "NQTokenEdit");

    content = new QWidget(this);
    layout = new QVBoxLayout();
    content->setLayout(layout);

    colorwheel_ = new NQColorWheel(content);
    layout->addWidget(colorwheel_);

    colordisplay_ = new QWidget(content);
    layout->addWidget(colordisplay_);

    connect(colorwheel_, SIGNAL(colorChange(QColor)),
            this, SLOT(colorChanged(QColor)));

    central->addTab(content, "NQColorWheel");
}

MainWindow::~MainWindow()
{

}

void MainWindow::tokensChanged(const QStringList& list)
{
    tokenlist_->clear();

    for (QStringList::ConstIterator it = list.begin();
         it!=list.end();
         ++it) {
        tokenlist_->addItem(*it);
    }
}

void MainWindow::colorChanged(const QColor& color)
{
    QPalette Pal(colordisplay_->palette());
    Pal.setColor(QPalette::Background, color);
    colordisplay_->setAutoFillBackground(true);
    colordisplay_->setPalette(Pal);
}
