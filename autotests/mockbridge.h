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
#ifndef MOCK_BRIDGE_H
#define MOCK_BRIDGE_H

#include "../src/private/decorationbridge.h"
#include <QObject>

class MockClient;
class MockSettings;

class MockBridge : public KDecoration2::DecorationBridge
{
    Q_OBJECT
public:
    std::unique_ptr<KDecoration2::DecoratedClientPrivate> createClient(KDecoration2::DecoratedClient *client, KDecoration2::Decoration *decoration) override;
    std::unique_ptr<KDecoration2::DecorationSettingsPrivate> settings(KDecoration2::DecorationSettings *parent)  override;
    void update(KDecoration2::Decoration *decoration, const QRect &geometry)  override;

    MockClient * lastCreatedClient() const {
        return m_lastCreatedClient;
    }
    MockSettings *lastCreatedSettings() const {
        return m_lastCreatedSettings;
    }

private:
    MockClient *m_lastCreatedClient = nullptr;
    MockSettings *m_lastCreatedSettings = nullptr;
};

#endif
