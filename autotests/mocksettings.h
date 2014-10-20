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
#ifndef MOCK_SETTINGS_H
#define MOCK_SETTINGS_H

#include "../src/private/decorationsettingsprivate.h"

class MockSettings : public KDecoration2::DecorationSettingsPrivate
{
public:
    explicit MockSettings(KDecoration2::DecorationSettings *parent);

    KDecoration2::BorderSize borderSize() const override;
    QList< KDecoration2::DecorationButtonType > decorationButtonsLeft() const override;
    QList< KDecoration2::DecorationButtonType > decorationButtonsRight() const override;
    bool isAlphaChannelSupported() const override;
    bool isCloseOnDoubleClickOnMenu() const override;
    bool isOnAllDesktopsAvailable() const override;

    void setOnAllDesktopsAvailabe(bool set);
    void setCloseOnDoubleClickOnMenu(bool set);

private:
    bool m_onAllDesktopsAvailable = false;
    bool m_closeDoubleClickOnMenu = false;
};

#endif
