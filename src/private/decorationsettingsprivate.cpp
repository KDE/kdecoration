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
#include "decorationsettingsprivate.h"
#include <QFontDatabase>

namespace KDecoration2
{

class Q_DECL_HIDDEN DecorationSettingsPrivate::Private
{
public:
    explicit Private(DecorationSettings *settings);
    DecorationSettings *settings;
    int gridUnit = -1;
    int smallSpacing = -1;
    int largeSpacing = -1;
};

DecorationSettingsPrivate::Private::Private(DecorationSettings *settings)
    : settings(settings)
{
}

DecorationSettingsPrivate::DecorationSettingsPrivate(DecorationSettings *parent)
    : d(new Private(parent))
{
}

DecorationSettingsPrivate::~DecorationSettingsPrivate()
{
}

DecorationSettings *DecorationSettingsPrivate::decorationSettings()
{
    return d->settings;
}

const DecorationSettings *DecorationSettingsPrivate::decorationSettings() const
{
    return d->settings;
}

QFont DecorationSettingsPrivate::font() const
{
    return QFontDatabase::systemFont(QFontDatabase::TitleFont);
}

QFontMetricsF DecorationSettingsPrivate::fontMetrics() const
{
    return QFontMetricsF(font());
}

int DecorationSettingsPrivate::gridUnit() const
{
    return d->gridUnit;
}

int DecorationSettingsPrivate::smallSpacing() const
{
    return d->smallSpacing;
}

int DecorationSettingsPrivate::largeSpacing() const
{
    return d->largeSpacing;
}

void DecorationSettingsPrivate::setGridUnit(int unit)
{
    d->gridUnit = unit;
}

void DecorationSettingsPrivate::setLargeSpacing(int spacing)
{
    d->largeSpacing = spacing;
}

void DecorationSettingsPrivate::setSmallSpacing(int spacing)
{
    d->smallSpacing = spacing;
}

}
