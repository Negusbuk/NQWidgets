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

#include <QFile>
#include <QApplication>

#include "nqlogger.h"

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    NQLogger::instance()->addDestiniation(stdout);
    NQLog("Main") << "Test";

    QFile file(":/qss/stylesheet.qss");
    file.open(QFile::ReadOnly);
    QString stylesheet(file.readAll());
    a.setStyleSheet(stylesheet);

    MainWindow w;
    w.show();

    return a.exec();
}
