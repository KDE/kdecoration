/*
 * Copyright 2014  Martin Gräßlin <mgraesslin@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "mockdecoration.h"
#include "mockbridge.h"

#include <utility>
#include <QMap>
#include <QVariantMap>

MockDecoration::MockDecoration(QObject *parent, const QVariantList &args)
    : Decoration(parent, args)
{
}

#ifdef _MSC_VER
QMap<QString, QVariant> makeMap(const QString& key, const QVariant &value) {
    QMap<QString, QVariant> ret;
    ret.insert(key, value);
    return ret;
}
MockDecoration::MockDecoration(MockBridge *bridge)
    : MockDecoration(nullptr, QVariantList({makeMap(QStringLiteral("bridge"), QVariant::fromValue(bridge))}))
#else
MockDecoration::MockDecoration(MockBridge *bridge)
    : MockDecoration(nullptr, QVariantList({QVariantMap({{QStringLiteral("bridge"), QVariant::fromValue(bridge)}})}))
#endif
{
}

void MockDecoration::paint(QPainter *painter, const QRect &repaintRegion)
{
    Q_UNUSED(painter)
    Q_UNUSED(repaintRegion)
}

void MockDecoration::setOpaque(bool set)
{
    Decoration::setOpaque(set);
}

void MockDecoration::setBorders(const QMargins &m)
{
    Decoration::setBorders(m);
}

void MockDecoration::setTitleBar(const QRect &rect)
{
    Decoration::setTitleBar(rect);
}
