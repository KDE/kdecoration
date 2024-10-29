/*
 * SPDX-FileCopyrightText: 2014 Martin Gräßlin <mgraesslin@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */
#include "mocksettings.h"
#include "../src/decorationsettings.h"

MockSettings::MockSettings(KDecoration3::DecorationSettings *parent)
    : DecorationSettingsPrivate(parent)
{
}

KDecoration3::BorderSize MockSettings::borderSize() const
{
    return KDecoration3::BorderSize::Normal;
}

QList<KDecoration3::DecorationButtonType> MockSettings::decorationButtonsLeft() const
{
    return QList<KDecoration3::DecorationButtonType>();
}

QList<KDecoration3::DecorationButtonType> MockSettings::decorationButtonsRight() const
{
    return QList<KDecoration3::DecorationButtonType>();
}

bool MockSettings::isAlphaChannelSupported() const
{
    return true;
}

bool MockSettings::isCloseOnDoubleClickOnMenu() const
{
    return m_closeDoubleClickOnMenu;
}

bool MockSettings::isOnAllDesktopsAvailable() const
{
    return m_onAllDesktopsAvailable;
}

void MockSettings::setOnAllDesktopsAvailabe(bool set)
{
    if (m_onAllDesktopsAvailable == set) {
        return;
    }
    m_onAllDesktopsAvailable = set;
    Q_EMIT decorationSettings()->onAllDesktopsAvailableChanged(m_onAllDesktopsAvailable);
}

void MockSettings::setCloseOnDoubleClickOnMenu(bool set)
{
    if (m_closeDoubleClickOnMenu == set) {
        return;
    }
    m_closeDoubleClickOnMenu = set;
    Q_EMIT decorationSettings()->closeOnDoubleClickOnMenuChanged(m_closeDoubleClickOnMenu);
}
