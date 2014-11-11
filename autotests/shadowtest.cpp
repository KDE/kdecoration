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
#include "../src/decorationshadow.h"

Q_DECLARE_METATYPE(QMargins)

class DecorationShadowTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testPadding_data();
    void testPadding();
    void testSizes_data();
    void testSizes();
};

void DecorationShadowTest::testPadding_data()
{
    QTest::addColumn<QByteArray>("propertyName");
    QTest::addColumn<QMargins>("padding");

    QTest::newRow("top")    << QByteArrayLiteral("paddingTop")    << QMargins(0, 10, 0, 0);
    QTest::newRow("right")  << QByteArrayLiteral("paddingRight")  << QMargins(0, 0, 10, 0);
    QTest::newRow("bottom") << QByteArrayLiteral("paddingBottom") << QMargins(0, 0, 0, 10);
    QTest::newRow("left")   << QByteArrayLiteral("paddingLeft")   << QMargins(10, 0, 0, 0);
}

void DecorationShadowTest::testPadding()
{
    using namespace KDecoration2;
    DecorationShadow shadow;

    QFETCH(QByteArray, propertyName);

    const int propertyIndex = shadow.metaObject()->indexOfProperty(propertyName.constData());
    QVERIFY(propertyIndex != -1);
    QMetaProperty metaProperty = shadow.metaObject()->property(propertyIndex);
    QCOMPARE(metaProperty.isReadable(), true);
    QCOMPARE(metaProperty.hasNotifySignal(), true);
    QCOMPARE(metaProperty.type(), QVariant::Int);
    QSignalSpy changedSpy(&shadow, SIGNAL(paddingChanged()));
    QVERIFY(changedSpy.isValid());

    QCOMPARE(shadow.property(propertyName.constData()).isValid(), true);
    QCOMPARE(shadow.property(propertyName.constData()).toInt(), 0);
    QFETCH(QMargins, padding);
    shadow.setPadding(padding);
    QCOMPARE(shadow.padding(), padding);
    QCOMPARE(shadow.property(propertyName.constData()).toInt(), 10);
    QCOMPARE(changedSpy.count(), 1);

    // trying to set to same value shouldn't emit the signal
    shadow.setPadding(padding);
    QCOMPARE(shadow.property(propertyName.constData()).toInt(), 10);
    QCOMPARE(changedSpy.count(), 1);

    // changing to different value should emit signal
    padding += 1;
    shadow.setPadding(padding);
    QCOMPARE(shadow.padding(), padding);
    QCOMPARE(shadow.property(propertyName.constData()).toInt(), 11);
    QCOMPARE(changedSpy.count(), 2);
}

void DecorationShadowTest::testSizes_data()
{
    QTest::addColumn<QByteArray>("propertyName");
    QTest::addColumn<QRect>("innerShadowRect");
    QTest::addColumn<QSize>("shadowSize");

    QTest::newRow("topLeft")     << QByteArrayLiteral("topLeft")     << QRect(1, 2, 5, 5) << QSize(6, 7);
    QTest::newRow("top")         << QByteArrayLiteral("top")         << QRect(1, 2, 1, 5) << QSize(3, 7);
    QTest::newRow("topRight")    << QByteArrayLiteral("topRight")    << QRect(0, 2, 2, 1) << QSize(3, 3);
    QTest::newRow("right")       << QByteArrayLiteral("right")       << QRect(0, 0, 1, 2) << QSize(2, 4);
    QTest::newRow("bottomRight") << QByteArrayLiteral("bottomRight") << QRect(0, 0, 1, 4) << QSize(2, 6);
    QTest::newRow("bottom")      << QByteArrayLiteral("bottom")      << QRect(0, 0, 1, 1) << QSize(1, 3);
    QTest::newRow("bottomLeft")  << QByteArrayLiteral("bottomLeft")  << QRect(1, 0, 1, 1) << QSize(2, 3);
    QTest::newRow("left")        << QByteArrayLiteral("left")        << QRect(1, 0, 1, 2) << QSize(2, 2);

}

void DecorationShadowTest::testSizes()
{
    using namespace KDecoration2;
    DecorationShadow shadow;

    QFETCH(QByteArray, propertyName);

    const int propertyIndex = shadow.metaObject()->indexOfProperty(propertyName.constData());
    QVERIFY(propertyIndex != -1);
    QMetaProperty metaProperty = shadow.metaObject()->property(propertyIndex);
    QCOMPARE(metaProperty.isReadable(), true);
    QCOMPARE(metaProperty.hasNotifySignal(), true);
    QCOMPARE(metaProperty.type(), QVariant::Size);
    QSignalSpy changedSpy(&shadow, SIGNAL(innerShadowRectChanged()));
    QVERIFY(changedSpy.isValid());

    QCOMPARE(shadow.innerShadowRect(), QRect());
    QCOMPARE(shadow.property(propertyName.constData()).isValid(), true);
    QCOMPARE(shadow.property(propertyName.constData()).toSize(), QSize());
    QFETCH(QRect, innerShadowRect);
    QFETCH(QSize, shadowSize);
    shadow.setInnerShadowRect(innerShadowRect);
    QCOMPARE(shadow.innerShadowRect(), innerShadowRect);
    // property should still be invalid as the image is not yet set
    QCOMPARE(shadow.property(propertyName.constData()).toSize(), QSize());
    shadow.setShadow(QImage(shadowSize, QImage::Format_ARGB32));
    QCOMPARE(shadow.property(propertyName.constData()).toSize(), QSize(1, 2));
    QCOMPARE(changedSpy.count(), 1);

    // trying to set to same value shouldn't emit the signal
    shadow.setInnerShadowRect(innerShadowRect);
    QCOMPARE(shadow.property(propertyName.constData()).toSize(), QSize(1, 2));
    QCOMPARE(changedSpy.count(), 1);

    // changing to different value should emit signal
    shadow.setInnerShadowRect(innerShadowRect.adjusted(1, 1, 1, 1));
    QCOMPARE(changedSpy.count(), 2);
    QCOMPARE(shadow.innerShadowRect(), innerShadowRect.adjusted(1, 1, 1, 1));
}

QTEST_MAIN(DecorationShadowTest)
#include "shadowtest.moc"
