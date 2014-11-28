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
#include <QTest>
#include <QSignalSpy>
#include "mockbridge.h"
#include "mockdecoration.h"

class DecorationTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testCreate();
    void testOpaque();
};

void DecorationTest::testCreate()
{
    // just test that creating the Decoration doesn't crash
    MockBridge bridge;
    const QString bridgeKey = QStringLiteral("bridge");
    MockDecoration deco1(nullptr, QVariantList({QVariantMap({ {bridgeKey, QVariant::fromValue(5)} }),
                                                QVariant::fromValue(bridgeKey),
                                                QVariantMap(),
                                                QVariantMap({ {bridgeKey, QVariant::fromValue(&bridge)} })
                                               }));
    QVERIFY(!deco1.client().isNull());
}

void DecorationTest::testOpaque()
{
    MockBridge bridge;
    MockDecoration deco(&bridge);
    QSignalSpy opaqueChangedSpy(&deco, SIGNAL(opaqueChanged(bool)));
    QVERIFY(opaqueChangedSpy.isValid());
    QCOMPARE(deco.isOpaque(), false);
    deco.setOpaque(false);
    QVERIFY(opaqueChangedSpy.isEmpty());
    deco.setOpaque(true);
    QCOMPARE(opaqueChangedSpy.count(), 1);
    QCOMPARE(opaqueChangedSpy.first().first().toBool(), true);
    QCOMPARE(deco.isOpaque(), true);
    deco.setOpaque(true);
    QCOMPARE(opaqueChangedSpy.count(), 1);
    deco.setOpaque(false);
    QCOMPARE(opaqueChangedSpy.count(), 2);
    QCOMPARE(opaqueChangedSpy.first().first().toBool(), true);
    QCOMPARE(opaqueChangedSpy.last().first().toBool(), false);
    QCOMPARE(deco.isOpaque(), false);
}

QTEST_GUILESS_MAIN(DecorationTest)
#include "decorationtest.moc"