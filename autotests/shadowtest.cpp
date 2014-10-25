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

    QTest::newRow("top")    << QByteArrayLiteral("paddingTop");
    QTest::newRow("right")  << QByteArrayLiteral("paddingRight");
    QTest::newRow("bottom") << QByteArrayLiteral("paddingBottom");
    QTest::newRow("left")   << QByteArrayLiteral("paddingLeft");
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
    QMetaMethod notifySignal = metaProperty.notifySignal();
    QVERIFY(notifySignal.methodSignature().startsWith(propertyName));
    QCOMPARE(notifySignal.parameterCount(), 1);
    QCOMPARE(notifySignal.parameterType(0), int(QMetaType::Int));
    QByteArray signalName = QByteArrayLiteral("2") + notifySignal.methodSignature();
    QSignalSpy changedSpy(&shadow, signalName.constData());
    QVERIFY(changedSpy.isValid());

    QCOMPARE(shadow.property(propertyName.constData()).isValid(), true);
    QCOMPARE(shadow.property(propertyName.constData()).toInt(), 0);
    shadow.setProperty(propertyName.constData(), 10);
    QCOMPARE(shadow.property(propertyName.constData()).toInt(), 10);
    QCOMPARE(changedSpy.count(), 1);
    QCOMPARE(changedSpy.first().first().toInt(), 10);

    // trying to set to same value shouldn't emit the signal
    shadow.setProperty(propertyName.constData(), 10);
    QCOMPARE(shadow.property(propertyName.constData()).toInt(), 10);
    QCOMPARE(changedSpy.count(), 1);

    // changing to different value should emit signal
    shadow.setProperty(propertyName.constData(), 11);
    QCOMPARE(shadow.property(propertyName.constData()).toInt(), 11);
    QCOMPARE(changedSpy.count(), 2);
    QCOMPARE(changedSpy.first().first().toInt(), 10);
    QCOMPARE(changedSpy.last().first().toInt(), 11);
}

void DecorationShadowTest::testSizes_data()
{
    QTest::addColumn<QByteArray>("propertyName");

    QTest::newRow("topLeft")     << QByteArrayLiteral("topLeft");
    QTest::newRow("top")         << QByteArrayLiteral("top");
    QTest::newRow("topRight")    << QByteArrayLiteral("topRight");
    QTest::newRow("right")       << QByteArrayLiteral("right");
    QTest::newRow("bottomRight") << QByteArrayLiteral("bottomRight");
    QTest::newRow("bottom")      << QByteArrayLiteral("bottom");
    QTest::newRow("bottomLeft")  << QByteArrayLiteral("bottomLeft");
    QTest::newRow("left")        << QByteArrayLiteral("left");

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
    QMetaMethod notifySignal = metaProperty.notifySignal();
    QVERIFY(notifySignal.methodSignature().startsWith(propertyName));
    QCOMPARE(notifySignal.parameterCount(), 1);
    QCOMPARE(notifySignal.parameterType(0), int(QMetaType::QSize));
    QByteArray signalName = QByteArrayLiteral("2") + notifySignal.methodSignature();
    QSignalSpy changedSpy(&shadow, signalName.constData());
    QVERIFY(changedSpy.isValid());

    QCOMPARE(shadow.property(propertyName.constData()).isValid(), true);
    QCOMPARE(shadow.property(propertyName.constData()).toSize(), QSize());
    shadow.setProperty(propertyName.constData(), QSize(1, 2));
    QCOMPARE(shadow.property(propertyName.constData()).toSize(), QSize(1, 2));
    QCOMPARE(changedSpy.count(), 1);
    QCOMPARE(changedSpy.first().first().toSize(), QSize(1, 2));

    // trying to set to same value shouldn't emit the signal
    shadow.setProperty(propertyName.constData(), QSize(1, 2));
    QCOMPARE(shadow.property(propertyName.constData()).toSize(), QSize(1, 2));
    QCOMPARE(changedSpy.count(), 1);

    // changing to different value should emit signal
    shadow.setProperty(propertyName.constData(), QSize(3, 4));
    QCOMPARE(shadow.property(propertyName.constData()).toSize(), QSize(3, 4));
    QCOMPARE(changedSpy.count(), 2);
    QCOMPARE(changedSpy.first().first().toSize(), QSize(1, 2));
    QCOMPARE(changedSpy.last().first().toSize(), QSize(3, 4));
}

QTEST_MAIN(DecorationShadowTest)
#include "shadowtest.moc"
