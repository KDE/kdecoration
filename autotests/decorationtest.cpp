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
#include "mockclient.h"
#include "mockdecoration.h"

class DecorationTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testCreate();
    void testOpaque();
    void testSection_data();
    void testSection();
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

Q_DECLARE_METATYPE(QMargins)
Q_DECLARE_METATYPE(Qt::WindowFrameSection)

void DecorationTest::testSection_data()
{
    QTest::addColumn<QRect>("titleBar");
    QTest::addColumn<QMargins>("margins");
    QTest::addColumn<QPoint>("pos");
    QTest::addColumn<Qt::WindowFrameSection>("expected");

    QRect r(1, 1, 98, 8);
    QMargins m(1, 10, 1, 1);
    QTest::newRow("topLeft")      << r << m << QPoint(0, 0)     << Qt::TopLeftSection;
    QTest::newRow("top@Left")     << r << m << QPoint(1, 0)     << Qt::TopSection;
    QTest::newRow("top@Right")    << r << m << QPoint(100, 0)   << Qt::TopSection;
    QTest::newRow("topRight")     << r << m << QPoint(101, 0)   << Qt::TopRightSection;
    QTest::newRow("right@top")    << r << m << QPoint(101, 1)   << Qt::RightSection;
    QTest::newRow("right@bottom") << r << m << QPoint(101, 109) << Qt::RightSection;
    QTest::newRow("bottomRight")  << r << m << QPoint(101, 110) << Qt::BottomRightSection;
    QTest::newRow("bottom@right") << r << m << QPoint(100, 110) << Qt::BottomSection;
    QTest::newRow("bottom@left")  << r << m << QPoint(1, 110)   << Qt::BottomSection;
    QTest::newRow("bottomLeft")   << r << m << QPoint(0, 110)   << Qt::BottomLeftSection;
    QTest::newRow("left@Top")     << r << m << QPoint(0, 1)     << Qt::LeftSection;
    QTest::newRow("left@Bottom")  << r << m << QPoint(0, 109)   << Qt::LeftSection;
    QTest::newRow("title")        << r << m << QPoint(1, 1)     << Qt::TitleBarArea;
}

void DecorationTest::testSection()
{
    MockBridge bridge;
    MockDecoration deco(&bridge);
    MockClient *client = bridge.lastCreatedClient();
    client->setWidth(100);
    client->setHeight(100);
    QCOMPARE(deco.size(), QSize(100, 100));
    QCOMPARE(deco.borderLeft(), 0);
    QCOMPARE(deco.borderTop(), 0);
    QCOMPARE(deco.borderRight(), 0);
    QCOMPARE(deco.borderBottom(), 0);
    QCOMPARE(deco.titleBar(), QRect());
    QCOMPARE(deco.sectionUnderMouse(), Qt::NoSection);

    QFETCH(QRect, titleBar);
    QFETCH(QMargins, margins);
    deco.setBorders(margins);
    QCOMPARE(deco.borderLeft(), margins.left());
    QCOMPARE(deco.borderTop(), margins.top());
    QCOMPARE(deco.borderRight(), margins.right());
    QCOMPARE(deco.borderBottom(), margins.bottom());
    deco.setTitleBar(titleBar);
    QCOMPARE(deco.titleBar(), titleBar);
    QCOMPARE(deco.size(), QSize(100 + deco.borderLeft() + deco.borderRight(), 100 + deco.borderTop() + deco.borderBottom()));

    QSignalSpy spy(&deco, SIGNAL(sectionUnderMouseChanged(Qt::WindowFrameSection)));
    QVERIFY(spy.isValid());
    QFETCH(QPoint, pos);
    QHoverEvent event(QEvent::HoverMove, QPointF(pos), QPointF(pos));
    QCoreApplication::sendEvent(&deco, &event);
    QFETCH(Qt::WindowFrameSection, expected);
    QCOMPARE(deco.sectionUnderMouse(), expected);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.first().first().value<Qt::WindowFrameSection>(), expected);

    QHoverEvent event2(QEvent::HoverMove, QPointF(50, 50), QPointF(50, 50));
    QCoreApplication::sendEvent(&deco, &event2);
    QCOMPARE(deco.sectionUnderMouse(), Qt::NoSection);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.first().first().value<Qt::WindowFrameSection>(), expected);
    QCOMPARE(spy.last().first().value<Qt::WindowFrameSection>(), Qt::NoSection);
}

QTEST_GUILESS_MAIN(DecorationTest)
#include "decorationtest.moc"
