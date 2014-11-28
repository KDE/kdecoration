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

DecorationSettingsPrivate::DecorationSettingsPrivate(DecorationSettings *parent)
    : q(parent)
    , m_gridUnit(-1)
    , m_smallSpacing(-1)
    , m_largeSpacing(-1)
{
}

DecorationSettingsPrivate::~DecorationSettingsPrivate()
{
}

DecorationSettings *DecorationSettingsPrivate::decorationSettings()
{
    return q;
}

const DecorationSettings *DecorationSettingsPrivate::decorationSettings() const
{
    return q;
}

QFont DecorationSettingsPrivate::font() const
{
    return QFontDatabase::systemFont(QFontDatabase::TitleFont);
}

QFontMetricsF DecorationSettingsPrivate::fontMetrics() const
{
    return QFontMetricsF(font());
}

}
