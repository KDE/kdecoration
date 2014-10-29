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
#include "decorationsettings.h"
#include "private/decorationbridge.h"
#include "private/decorationsettingsprivate.h"

#include <QFontMetrics>

namespace KDecoration2
{

DecorationSettings::DecorationSettings(DecorationBridge *bridge, QObject *parent)
    : QObject(parent)
    , d(std::move(bridge->settings(this)))
{
    auto updateUnits = [this] {
        int gridUnit = QFontMetrics(font()).boundingRect(QLatin1Char('M')).height();;
        if (gridUnit % 2 != 0) {
            gridUnit++;
        }
        if (gridUnit != d->gridUnit()) {
            d->setGridUnit(gridUnit);
            emit gridUnitChanged(gridUnit);
        }
        if (gridUnit != d->largeSpacing()) {
            d->setSmallSpacing(qMax(2, (int)(gridUnit / 4))); // 1/4 of gridUnit, at least 2
            d->setLargeSpacing(gridUnit); // msize.height
            emit spacingChanged();
        }
    };
    updateUnits();
    connect(this, &DecorationSettings::fontChanged, this, updateUnits);
}

DecorationSettings::~DecorationSettings() = default;

#define DELEGATE(type, method) \
type DecorationSettings::method() const \
{ \
    return d->method(); \
}

DELEGATE(bool, isOnAllDesktopsAvailable)
DELEGATE(bool, isAlphaChannelSupported)
DELEGATE(bool, isCloseOnDoubleClickOnMenu)
DELEGATE(QList<DecorationButtonType>, decorationButtonsLeft)
DELEGATE(QList<DecorationButtonType>, decorationButtonsRight)
DELEGATE(BorderSize, borderSize)
DELEGATE(QFont, font)
DELEGATE(int, gridUnit)
DELEGATE(int, smallSpacing)
DELEGATE(int, largeSpacing)

#undef DELEGATE

}
