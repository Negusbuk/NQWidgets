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

#include <QDateTime>

#include "nqlogger.h"

NQLog::NQLog(const QString& module)
    : module_(module),
      stream_(&buffer_)
{

}

NQLog::~NQLog()
{
    NQLogger::instance()->write(module_, buffer_);
}

NQLogger* NQLogger::instance_ = NULL;

NQLogger::NQLogger(QObject *parent) :
    QObject(parent)
{

}

NQLogger* NQLogger::instance(QObject *parent)
{
    if (instance_==NULL) {
        instance_ = new NQLogger(parent);
    }

    return instance_;
}

void NQLogger::write(const QString& module, const QString& buffer)
{
    QString dateString = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

    QString message = dateString;
    message += " [";
    message += module;
    message += "] ";
    message += buffer;
    message += "\n";

    for (std::vector<QTextStream*>::iterator it = destinations_.begin();
         it!=destinations_.end();
         ++it) {
        QTextStream* stream = *it;
        stream->operator <<(message);
        stream->flush();
    }
}

void NQLogger::addDestiniation(QIODevice * device)
{
    QTextStream* stream = new QTextStream(device);
    destinations_.push_back(stream);
}

void NQLogger::addDestiniation(FILE * fileHandle)
{
    QTextStream* stream = new QTextStream(fileHandle);
    destinations_.push_back(stream);
}
