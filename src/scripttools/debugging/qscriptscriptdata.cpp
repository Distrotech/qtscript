/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtSCriptTools module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qscriptscriptdata_p.h"

#include <QtCore/qdatastream.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qshareddata.h>

QT_BEGIN_NAMESPACE

/*!
  \since 4.5
  \class QScriptScriptData
  \internal

  \brief The QScriptScriptData class holds data associated with a script.
*/

class QScriptScriptDataPrivate : public QSharedData
{
public:
    QScriptScriptDataPrivate();
    ~QScriptScriptDataPrivate();

    QString contents;
    QString fileName;
    int baseLineNumber;
    QDateTime timeStamp;
};

QScriptScriptDataPrivate::QScriptScriptDataPrivate()
{
}

QScriptScriptDataPrivate::~QScriptScriptDataPrivate()
{
}

QScriptScriptData::QScriptScriptData()
    : d_ptr(0)
{
}

QScriptScriptData::QScriptScriptData(const QString &contents, const QString &fileName,
                                     int baseLineNumber, const QDateTime &timeStamp)
    : d_ptr(new QScriptScriptDataPrivate)
{
    d_ptr->contents = contents;
    d_ptr->fileName = fileName;
    d_ptr->baseLineNumber = baseLineNumber;
    if (timeStamp.isValid())
        d_ptr->timeStamp = timeStamp;
    else
        d_ptr->timeStamp = QDateTime::currentDateTime();
    d_ptr->ref.ref();
}

QScriptScriptData::QScriptScriptData(const QScriptScriptData &other)
    : d_ptr(other.d_ptr.data())
{
    if (d_ptr)
        d_ptr->ref.ref();
}

QScriptScriptData::~QScriptScriptData()
{
}

QScriptScriptData &QScriptScriptData::operator=(const QScriptScriptData &other)
{
    d_ptr.assign(other.d_ptr.data());
    return *this;
}

QString QScriptScriptData::contents() const
{
    Q_D(const QScriptScriptData);
    if (!d)
        return QString();
    return d->contents;
}

QStringList QScriptScriptData::lines(int startLineNumber, int count) const
{
    Q_D(const QScriptScriptData);
    if (!d)
        return QStringList();
    QStringList allLines = d->contents.split(QLatin1Char('\n'));
    return allLines.mid(qMax(0, startLineNumber - d->baseLineNumber), count);
}

QString QScriptScriptData::fileName() const
{
    Q_D(const QScriptScriptData);
    if (!d)
        return QString();
    return d->fileName;
}

int QScriptScriptData::baseLineNumber() const
{
    Q_D(const QScriptScriptData);
    if (!d)
        return -1;
    return d->baseLineNumber;
}

QDateTime QScriptScriptData::timeStamp() const
{
    Q_D(const QScriptScriptData);
    if (!d)
        return QDateTime();
    return d->timeStamp;
}

bool QScriptScriptData::isValid() const
{
    Q_D(const QScriptScriptData);
    return (d != 0);
}

bool QScriptScriptData::operator==(const QScriptScriptData &other) const
{
    Q_D(const QScriptScriptData);
    const QScriptScriptDataPrivate *od = other.d_func();
    if (d == od)
        return true;
    if (!d || !od)
        return false;
    return ((d->contents == od->contents)
            && (d->fileName == od->fileName)
            && (d->baseLineNumber == od->baseLineNumber));
}

bool QScriptScriptData::operator!=(const QScriptScriptData &other) const
{
    return !(*this == other);
}

QDataStream &operator<<(QDataStream &out, const QScriptScriptData &data)
{
    const QScriptScriptDataPrivate *d = data.d_ptr.data();
    if (d) {
        out << d->contents;
        out << d->fileName;
        out << qint32(d->baseLineNumber);
    } else {
        out << QString();
        out << QString();
        out << qint32(0);
    }
    return out;
}

QDataStream &operator>>(QDataStream &in, QScriptScriptData &data)
{
    if (!data.d_ptr) {
        data.d_ptr.reset(new QScriptScriptDataPrivate());
        data.d_ptr->ref.ref();
    }
    QScriptScriptDataPrivate *d = data.d_ptr.data();
    in >> d->contents;
    in >> d->fileName;
    qint32 ln;
    in >> ln;
    d->baseLineNumber = ln;
    return in;
}

QT_END_NAMESPACE
