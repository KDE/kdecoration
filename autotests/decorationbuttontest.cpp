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
#include <QStyleHints>
#include "../src/decoratedclient.h"
#include "../src/decorationsettings.h"
#include "mockdecoration.h"
#include "mockbridge.h"
#include "mockbutton.h"
#include "mockclient.h"
#include "mocksettings.h"

Q_DECLARE_METATYPE(Qt::MouseButton)

class DecorationButtonTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testButton();
    void testChecked();
    void testEnabled();
    void testPressIgnore_data();
    void testPressIgnore();
    void testReleaseIgnore_data();
    void testReleaseIgnore();
    void testHoverEnterIgnore_data();
    void testHoverEnterIgnore();
    void testHoverLeaveIgnore_data();
    void testHoverLeaveIgnore();
    void testHover();
    void testMouseMove_data();
    void testMouseMove();
    void testClose();
    void testMinimize();
    void testQuickHelp();
    void testKeepAbove();
    void testKeepBelow();
    void testShade();
    void testMaximize();
    void testOnAllDesktops();
    void testMenu();
    void testMenuDoubleClick();
    void testMenuPressAndHold();
};

void DecorationButtonTest::testButton()
{
    MockBridge bridge;
    MockDecoration mockDecoration(&bridge);
    // create a custom button and verify the base settings
    MockButton button(KDecoration2::DecorationButtonType::Custom, &mockDecoration);
    QCOMPARE(button.decoration().data(), &mockDecoration);
    const MockButton &constRef(button);
    QCOMPARE(constRef.decoration().data(), &mockDecoration);
    QCOMPARE(button.type(), KDecoration2::DecorationButtonType::Custom);
    QCOMPARE(button.acceptedButtons(), Qt::MouseButtons(Qt::LeftButton));
    QCOMPARE(button.isCheckable(), false);
    QCOMPARE(button.isChecked(), false);
    QCOMPARE(button.isEnabled(), true);
    QCOMPARE(button.isHovered(), false);
    QCOMPARE(button.isPressed(), false);
    QCOMPARE(button.isVisible(), true);
    QCOMPARE(button.size(), QSize(0, 0));
    QCOMPARE(button.geometry(), QRect());

    // test setting the geometry
    QSignalSpy geometryChangedSpy(&button, SIGNAL(geometryChanged(QRect)));
    QVERIFY(geometryChangedSpy.isValid());
    // setting to default geometry shouldn't change
    button.setGeometry(QRect());
    QCOMPARE(button.geometry(), QRect());
    QCOMPARE(geometryChangedSpy.count(), 0);
    // setting to a proper geometry should change
    const QRect testGeometry = QRect(0, 0, 10, 20);
    button.setGeometry(testGeometry);
    QCOMPARE(button.geometry(), testGeometry);
    QCOMPARE(button.size(), testGeometry.size());
    QCOMPARE(geometryChangedSpy.count(), 1);
    QCOMPARE(geometryChangedSpy.first().first().toRect(), testGeometry);

    // test changing visibility
    QSignalSpy visibilityChangedSpy(&button, SIGNAL(visibilityChanged(bool)));
    QVERIFY(visibilityChangedSpy.isValid());
    button.setVisible(true);
    QCOMPARE(visibilityChangedSpy.count(), 0);
    button.setVisible(false);
    QCOMPARE(button.isVisible(), false);
    QCOMPARE(visibilityChangedSpy.count(), 1);
    QCOMPARE(visibilityChangedSpy.first().first().toBool(), false);
}

void DecorationButtonTest::testChecked()
{
    MockBridge bridge;
    MockDecoration mockDecoration(&bridge);
    // create a custom button and verify the base settings
    MockButton button(KDecoration2::DecorationButtonType::Custom, &mockDecoration);
    button.setGeometry(QRect(0, 0, 10, 10));

    // without being checkable it should not get checked
    QSignalSpy checkedChangedSpy(&button, SIGNAL(checkedChanged(bool)));
    QVERIFY(checkedChangedSpy.isValid());
    button.setChecked(true);
    QCOMPARE(button.isChecked(), false);
    QCOMPARE(checkedChangedSpy.count(), 0);

    // now let's set the checkable state
    QSignalSpy checkableChangedSpy(&button, SIGNAL(checkableChanged(bool)));
    QVERIFY(checkableChangedSpy.isValid());
    // setting to same should not emit
    button.setCheckable(false);
    QCOMPARE(checkableChangedSpy.count(), 0);
    button.setCheckable(true);
    QCOMPARE(button.isCheckable(), true);
    QCOMPARE(checkableChangedSpy.count(), 1);
    QCOMPARE(checkableChangedSpy.first().first().toBool(), true);

    // now it should be possible to check the button
    button.setChecked(true);
    QCOMPARE(button.isChecked(), true);
    QCOMPARE(checkedChangedSpy.count(), 1);
    // setting again should not change
    button.setChecked(true);
    QCOMPARE(button.isChecked(), true);
    QCOMPARE(checkedChangedSpy.count(), 1);
    // and disable
    button.setChecked(false);
    QCOMPARE(button.isChecked(), false);
    QCOMPARE(checkedChangedSpy.count(), 2);
    QCOMPARE(checkedChangedSpy.first().first().toBool(), true);
    QCOMPARE(checkedChangedSpy.last().first().toBool(), false);

    // last but not least let's disable the checkable again, this should disable a checked button
    button.setChecked(true);
    QCOMPARE(button.isChecked(), true);
    checkedChangedSpy.clear();
    QCOMPARE(checkedChangedSpy.count(), 0);
    button.setCheckable(false);
    QCOMPARE(button.isCheckable(), false);
    QCOMPARE(checkableChangedSpy.count(), 2);
    QCOMPARE(checkableChangedSpy.last().first().toBool(), false);
    QCOMPARE(button.isChecked(), false);
    QCOMPARE(checkedChangedSpy.count(), 1);
}

void DecorationButtonTest::testEnabled()
{
    MockBridge bridge;
    MockDecoration mockDecoration(&bridge);
    // create a custom button and verify the base settings
    MockButton button(KDecoration2::DecorationButtonType::Custom, &mockDecoration);
    button.setGeometry(QRect(0, 0, 10, 10));

    // enabling has influence on whether the button accepts events, so we need to fake events
    QSignalSpy enabledChangedSpy(&button, SIGNAL(enabledChanged(bool)));
    QVERIFY(enabledChangedSpy.isValid());
    // setting to same shouldn't change
    button.setEnabled(true);
    QCOMPARE(enabledChangedSpy.count(), 0);
    button.setEnabled(false);
    QCOMPARE(button.isEnabled(), false);
    QCOMPARE(enabledChangedSpy.count(), 1);
    QCOMPARE(enabledChangedSpy.first().first().toBool(), false);

    // now let's send it a hover entered event
    QSignalSpy hoveredChangedSpy(&button, SIGNAL(hoveredChanged(bool)));
    QVERIFY(hoveredChangedSpy.isValid());
    QHoverEvent event(QEvent::HoverEnter, QPointF(1, 1), QPointF(-1, -1));
    event.setAccepted(false);
    button.event(&event);
    QCOMPARE(event.isAccepted(), false);
    QCOMPARE(hoveredChangedSpy.count(), 0);

    // if we enable the button again we should get a hovered changed signal
    button.setEnabled(true);
    QCOMPARE(enabledChangedSpy.count(), 2);
    QCOMPARE(enabledChangedSpy.last().first().toBool(), true);
    button.event(&event);
    QCOMPARE(event.isAccepted(), true);
    QCOMPARE(hoveredChangedSpy.count(), 1);
    QCOMPARE(hoveredChangedSpy.first().first().toBool(), true);

    // if we disable the button now we get a hovered disabled signal
    button.setEnabled(false);
    QCOMPARE(hoveredChangedSpy.count(), 2);
    QCOMPARE(hoveredChangedSpy.last().first().toBool(), false);
}

void DecorationButtonTest::testPressIgnore_data()
{
    QTest::addColumn<bool>("enabled");
    QTest::addColumn<bool>("visible");
    QTest::addColumn<QPoint>("clickPos");
    QTest::addColumn<Qt::MouseButton>("mouseButton");
    QTest::addColumn<bool>("expectedAccepted");

    QTest::newRow("all-disabled") << false << false << QPoint(0, 0) << Qt::LeftButton << false;
    QTest::newRow("enabled") << true << false << QPoint(0, 0) << Qt::LeftButton << false;
    QTest::newRow("visible") << false << true << QPoint(0, 0) << Qt::LeftButton << false;
    QTest::newRow("outside") << true << true << QPoint(20, 20) << Qt::LeftButton << false;
    QTest::newRow("wrong-button") << true << true << QPoint(0, 0) << Qt::RightButton << false;
}

void DecorationButtonTest::testPressIgnore()
{
    MockBridge bridge;
    MockDecoration mockDecoration(&bridge);
    // create a custom button and verify the base settings
    MockButton button(KDecoration2::DecorationButtonType::Custom, &mockDecoration);
    button.setGeometry(QRect(0, 0, 10, 10));
    button.setAcceptedButtons(Qt::LeftButton);
    QSignalSpy pressedSpy(&button, SIGNAL(pressed()));
    QVERIFY(pressedSpy.isValid());
    QSignalSpy pressedChangedSpy(&button, SIGNAL(pressedChanged(bool)));
    QVERIFY(pressedChangedSpy.isValid());

    QFETCH(bool, enabled);
    QFETCH(bool, visible);
    button.setEnabled(enabled);
    button.setVisible(visible);

    QFETCH(QPoint, clickPos);
    QFETCH(Qt::MouseButton, mouseButton);
    QMouseEvent pressEvent(QEvent::MouseButtonPress, clickPos, mouseButton, mouseButton, Qt::NoModifier);
    pressEvent.setAccepted(false);
    button.event(&pressEvent);
    QTEST(pressEvent.isAccepted(), "expectedAccepted");
    QCOMPARE(button.isPressed(), false);
    QVERIFY(pressedSpy.isEmpty());
    QVERIFY(pressedChangedSpy.isEmpty());
}

void DecorationButtonTest::testReleaseIgnore_data()
{
    QTest::addColumn<bool>("enabled");
    QTest::addColumn<bool>("visible");
    QTest::addColumn<QPoint>("clickPos");
    QTest::addColumn<Qt::MouseButton>("mouseButton");
    QTest::addColumn<bool>("expectedAccepted");
    QTest::addColumn<bool>("expectedPressed");
    QTest::addColumn<int>("expectedPressedChanged");

    QTest::newRow("all-disabled") << false << false << QPoint(0, 0) << Qt::LeftButton << false << false << 2;
    QTest::newRow("enabled") << true << false << QPoint(0, 0) << Qt::LeftButton << false << false << 2;
    QTest::newRow("visible") << false << true << QPoint(0, 0) << Qt::LeftButton << false << false << 2;
    QTest::newRow("outside") << true << true << QPoint(20, 20) << Qt::LeftButton << true << false << 2;
    QTest::newRow("wrong-button") << true << true << QPoint(0, 0) << Qt::RightButton << false << true << 1;
}

void DecorationButtonTest::testReleaseIgnore()
{
    MockBridge bridge;
    MockDecoration mockDecoration(&bridge);
    // create a custom button and verify the base settings
    MockButton button(KDecoration2::DecorationButtonType::Custom, &mockDecoration);
    button.setGeometry(QRect(0, 0, 10, 10));
    button.setAcceptedButtons(Qt::LeftButton);
    button.setEnabled(true);
    button.setVisible(true);
    QSignalSpy pressedSpy(&button, SIGNAL(pressed()));
    QVERIFY(pressedSpy.isValid());
    QSignalSpy pressedChangedSpy(&button, SIGNAL(pressedChanged(bool)));
    QVERIFY(pressedChangedSpy.isValid());
    QSignalSpy clickedSpy(&button, SIGNAL(clicked(Qt::MouseButton)));
    QVERIFY(clickedSpy.isValid());

    QMouseEvent pressEvent(QEvent::MouseButtonPress, QPoint(0, 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    pressEvent.setAccepted(false);
    button.event(&pressEvent);
    QCOMPARE(pressEvent.isAccepted(), true);
    QCOMPARE(button.isPressed(), true);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(pressedChangedSpy.count(), 1);
    QCOMPARE(pressedChangedSpy.last().first().toBool(), true);

    QFETCH(bool, enabled);
    QFETCH(bool, visible);
    button.setEnabled(enabled);
    button.setVisible(visible);

    QFETCH(QPoint, clickPos);
    QFETCH(Qt::MouseButton, mouseButton);
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, clickPos, mouseButton, Qt::NoButton, Qt::NoModifier);
    releaseEvent.setAccepted(false);
    button.event(&releaseEvent);
    QTEST(releaseEvent.isAccepted(), "expectedAccepted");
    QTEST(button.isPressed(), "expectedPressed");
    QCOMPARE(pressedSpy.count(), 1);
    QTEST(pressedChangedSpy.count(), "expectedPressedChanged");
    QCOMPARE(pressedChangedSpy.last().first().toBool(), button.isPressed());
    QCOMPARE(clickedSpy.count(), 0);
}

void DecorationButtonTest::testHoverEnterIgnore_data()
{
    QTest::addColumn<bool>("enabled");
    QTest::addColumn<bool>("visible");
    QTest::addColumn<QPoint>("enterPos");

    QTest::newRow("all-disabled") << false << false << QPoint(0, 0);
    QTest::newRow("enabled") << true << false << QPoint(0, 0);
    QTest::newRow("visible") << false << true << QPoint(0, 0);
    QTest::newRow("outside") << true << true << QPoint(20, 20);
}

void DecorationButtonTest::testHoverEnterIgnore()
{
    MockBridge bridge;
    MockDecoration mockDecoration(&bridge);
    // create a custom button and verify the base settings
    MockButton button(KDecoration2::DecorationButtonType::Custom, &mockDecoration);
    button.setGeometry(QRect(0, 0, 10, 10));
    QSignalSpy pointerEnteredSpy(&button, SIGNAL(pointerEntered()));
    QVERIFY(pointerEnteredSpy.isValid());
    QSignalSpy hoveredChangedSpy(&button, SIGNAL(hoveredChanged(bool)));
    QVERIFY(hoveredChangedSpy.isValid());

    QFETCH(bool, enabled);
    QFETCH(bool, visible);
    button.setEnabled(enabled);
    button.setVisible(visible);

    QFETCH(QPoint, enterPos);
    QHoverEvent enterEvent(QEvent::HoverEnter, enterPos, QPoint());
    enterEvent.setAccepted(false);
    button.event(&enterEvent);
    QCOMPARE(enterEvent.isAccepted(), false);
    QCOMPARE(button.isHovered(), false);
    QCOMPARE(pointerEnteredSpy.count(), 0);
    QCOMPARE(hoveredChangedSpy.count(), 0);

    // send a HoverLeft event should not be processed
    button.setEnabled(true);
    button.setVisible(true);
    QHoverEvent leftEvent(QEvent::HoverLeave, QPoint(20, 20), enterPos);
    leftEvent.setAccepted(false);
    button.event(&leftEvent);
    QCOMPARE(leftEvent.isAccepted(), false);
}

void DecorationButtonTest::testHoverLeaveIgnore_data()
{
    QTest::addColumn<bool>("enabled");
    QTest::addColumn<bool>("visible");
    QTest::addColumn<QPoint>("leavePos");
    QTest::addColumn<int>("expectedLeaveCount");
    QTest::addColumn<int>("expectedHoverChangedCount");

    QTest::newRow("all-disabled") << false << false << QPoint(20, 20) << 1 << 2;
    QTest::newRow("enabled") << true << false << QPoint(20, 20) << 1 << 2;
    QTest::newRow("visible") << false << true << QPoint(20, 20) << 1 << 2;
    QTest::newRow("inside") << true << true << QPoint(5, 5) << 0 << 1;
}

void DecorationButtonTest::testHoverLeaveIgnore()
{
    MockBridge bridge;
    MockDecoration mockDecoration(&bridge);
    // create a custom button and verify the base settings
    MockButton button(KDecoration2::DecorationButtonType::Custom, &mockDecoration);
    button.setGeometry(QRect(0, 0, 10, 10));
    button.setEnabled(true);
    button.setVisible(true);
    QSignalSpy pointerEnteredSpy(&button, SIGNAL(pointerEntered()));
    QVERIFY(pointerEnteredSpy.isValid());
    QSignalSpy hoveredChangedSpy(&button, SIGNAL(hoveredChanged(bool)));
    QVERIFY(hoveredChangedSpy.isValid());
    QSignalSpy pointerLeavedSpy(&button, SIGNAL(pointerLeft()));
    QVERIFY(pointerLeavedSpy.isValid());

    QHoverEvent enterEvent(QEvent::HoverEnter, QPoint(0, 0), QPoint());
    enterEvent.setAccepted(false);
    button.event(&enterEvent);
    QCOMPARE(enterEvent.isAccepted(), true);
    QCOMPARE(button.isHovered(), true);
    QCOMPARE(pointerEnteredSpy.count(), 1);
    QCOMPARE(hoveredChangedSpy.count(), 1);
    QCOMPARE(hoveredChangedSpy.last().first().toBool(), true);

    QFETCH(bool, enabled);
    QFETCH(bool, visible);
    button.setEnabled(enabled);
    button.setVisible(visible);

    QFETCH(QPoint, leavePos);
    QHoverEvent leftEvent(QEvent::HoverLeave, leavePos, QPoint(0, 0));
    leftEvent.setAccepted(false);
    button.event(&leftEvent);
    QCOMPARE(leftEvent.isAccepted(), false);
    QCOMPARE(pointerEnteredSpy.count(), 1);
    QTEST(pointerLeavedSpy.count(), "expectedLeaveCount");
    QTEST(hoveredChangedSpy.count(), "expectedHoverChangedCount");
    QCOMPARE(hoveredChangedSpy.last().first().toBool(), button.isHovered());
}

void DecorationButtonTest::testHover()
{
    MockBridge bridge;
    MockDecoration mockDecoration(&bridge);
    // create a custom button and verify the base settings
    MockButton button(KDecoration2::DecorationButtonType::Custom, &mockDecoration);
    button.setGeometry(QRect(0, 0, 10, 10));
    button.setEnabled(true);
    button.setVisible(true);
    QSignalSpy pointerEnteredSpy(&button, SIGNAL(pointerEntered()));
    QVERIFY(pointerEnteredSpy.isValid());
    QSignalSpy hoveredChangedSpy(&button, SIGNAL(hoveredChanged(bool)));
    QVERIFY(hoveredChangedSpy.isValid());
    QSignalSpy pointerLeavedSpy(&button, SIGNAL(pointerLeft()));
    QVERIFY(pointerLeavedSpy.isValid());

    QHoverEvent enterEvent(QEvent::HoverEnter, QPoint(0, 0), QPoint());
    enterEvent.setAccepted(false);
    button.event(&enterEvent);
    QCOMPARE(enterEvent.isAccepted(), true);
    QCOMPARE(button.isHovered(), true);
    QCOMPARE(pointerEnteredSpy.count(), 1);
    QCOMPARE(hoveredChangedSpy.count(), 1);
    QCOMPARE(hoveredChangedSpy.last().first().toBool(), true);

    // send in a hovermove event - it's passed through, but not used
    QHoverEvent moveEvent(QEvent::HoverMove, QPoint(5, 0), QPoint(0, 0));
    moveEvent.setAccepted(false);
    button.event(&moveEvent);
    QCOMPARE(moveEvent.isAccepted(), false);

    QHoverEvent leftEvent(QEvent::HoverLeave, QPoint(10, 0), QPoint(0, 0));
    leftEvent.setAccepted(false);
    button.event(&leftEvent);
    QCOMPARE(leftEvent.isAccepted(), true);
    QCOMPARE(pointerEnteredSpy.count(), 1);
    QCOMPARE(pointerLeavedSpy.count(), 1);
    QCOMPARE(hoveredChangedSpy.count(), 2);
    QCOMPARE(hoveredChangedSpy.last().first().toBool(), false);
}

void DecorationButtonTest::testMouseMove_data()
{
    QTest::addColumn<bool>("enabled");
    QTest::addColumn<bool>("visible");
    QTest::addColumn<QPoint>("movePos");
    QTest::addColumn<bool>("expectedAccepted");
    QTest::addColumn<bool>("expectedHovered");
    QTest::addColumn<int>("expectedChangedCount");

    QTest::newRow("outside") << true << true << QPoint(10, 10) << true << false << 2;
    QTest::newRow("inside") << true << true << QPoint(5, 5) << false << true << 1;
    QTest::newRow("disabled") << false << true << QPoint(10, 10) << false << false << 2;
    QTest::newRow("invisible") << true << false << QPoint(10, 10) << false << false << 2;
}

void DecorationButtonTest::testMouseMove()
{
    MockBridge bridge;
    MockDecoration mockDecoration(&bridge);
    // create a custom button and verify the base settings
    MockButton button(KDecoration2::DecorationButtonType::Custom, &mockDecoration);
    button.setGeometry(QRect(0, 0, 10, 10));
    button.setEnabled(true);
    button.setVisible(true);
    QSignalSpy hoveredChangedSpy(&button, SIGNAL(hoveredChanged(bool)));
    QVERIFY(hoveredChangedSpy.isValid());
    QSignalSpy pointerLeavedSpy(&button, SIGNAL(pointerLeft()));
    QVERIFY(pointerLeavedSpy.isValid());

    QHoverEvent enterEvent(QEvent::HoverEnter, QPoint(0, 0), QPoint());
    enterEvent.setAccepted(false);
    button.event(&enterEvent);
    QCOMPARE(enterEvent.isAccepted(), true);
    QCOMPARE(button.isHovered(), true);
    QCOMPARE(hoveredChangedSpy.count(), 1);
    QCOMPARE(hoveredChangedSpy.last().first().toBool(), true);

    QFETCH(bool, enabled);
    button.setEnabled(enabled);
    QFETCH(bool, visible);
    button.setVisible(visible);

    QFETCH(QPoint, movePos);
    QMouseEvent mouseMoveEvent(QEvent::MouseMove, movePos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    mouseMoveEvent.setAccepted(false);
    button.event(&mouseMoveEvent);
    QTEST(mouseMoveEvent.isAccepted(), "expectedAccepted");
    QTEST(button.isHovered(), "expectedHovered");
    QTEST(hoveredChangedSpy.count(), "expectedChangedCount");
    QCOMPARE(hoveredChangedSpy.last().first().toBool(), button.isHovered());

    // explicit further move event outside of button
    QMouseEvent mouseMoveEvent2(QEvent::MouseMove, QPoint(50, 50), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    mouseMoveEvent2.setAccepted(false);
    button.event(&mouseMoveEvent2);
    QTEST(mouseMoveEvent2.isAccepted(), "expectedHovered");
    QCOMPARE(button.isHovered(), false);
}

void DecorationButtonTest::testClose()
{
    MockBridge bridge;
    MockDecoration mockDecoration(&bridge);
    MockClient *client = bridge.lastCreatedClient();
    MockButton button(KDecoration2::DecorationButtonType::Close, &mockDecoration);
    button.setGeometry(QRect(0, 0, 10, 10));

    QCOMPARE(button.isEnabled(), false);
    QCOMPARE(button.isCheckable(), false);
    QCOMPARE(button.isChecked(), false);
    QCOMPARE(button.isVisible(), true);
    QCOMPARE(button.acceptedButtons(), Qt::LeftButton);

    // if the client is closeable the button should get enabled
    QSignalSpy closeableChangedSpy(mockDecoration.client(), SIGNAL(closeableChanged(bool)));
    QVERIFY(closeableChangedSpy.isValid());
    client->setCloseable(true);
    QCOMPARE(button.isEnabled(), true);
    QCOMPARE(closeableChangedSpy.count(), 1);
    QCOMPARE(closeableChangedSpy.first().first().toBool(), true);

    // clicking the button should trigger a request for close
    QSignalSpy clickedSpy(&button, SIGNAL(clicked(Qt::MouseButton)));
    QVERIFY(clickedSpy.isValid());
    QSignalSpy pressedSpy(&button, SIGNAL(pressed()));
    QVERIFY(pressedSpy.isValid());
    QSignalSpy releasedSpy(&button, SIGNAL(released()));
    QVERIFY(releasedSpy.isValid());
    QSignalSpy closeRequestedSpy(client, SIGNAL(closeRequested()));
    QVERIFY(closeRequestedSpy.isValid());
    QSignalSpy pressedChangedSpy(&button, SIGNAL(pressedChanged(bool)));
    QVERIFY(pressedChangedSpy.isValid());

    QMouseEvent pressEvent(QEvent::MouseButtonPress, QPointF(5, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    pressEvent.setAccepted(false);
    button.event(&pressEvent);
    QCOMPARE(pressEvent.isAccepted(), true);
    QCOMPARE(button.isPressed(), true);
    QCOMPARE(clickedSpy.count(), 0);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(releasedSpy.count(), 0);
    QCOMPARE(closeRequestedSpy.count(), 0);
    QCOMPARE(pressedChangedSpy.count(), 1);
    QCOMPARE(pressedChangedSpy.first().first().toBool(), true);

    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPointF(5, 5), Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    releaseEvent.setAccepted(false);
    button.event(&releaseEvent);
    QCOMPARE(releaseEvent.isAccepted(), true);
    QCOMPARE(button.isPressed(), false);
    QCOMPARE(clickedSpy.count(), 1);
    QCOMPARE(clickedSpy.first().first().value<Qt::MouseButton>(), Qt::LeftButton);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(releasedSpy.count(), 1);
    QVERIFY(closeRequestedSpy.wait());
    QCOMPARE(closeRequestedSpy.count(), 1);
    QCOMPARE(pressedChangedSpy.count(), 2);
    QCOMPARE(pressedChangedSpy.last().first().toBool(), false);
}

void DecorationButtonTest::testMinimize()
{
    MockBridge bridge;
    MockDecoration mockDecoration(&bridge);
    MockClient *client = bridge.lastCreatedClient();
    MockButton button(KDecoration2::DecorationButtonType::Minimize, &mockDecoration);
    button.setGeometry(QRect(0, 0, 10, 10));

    QCOMPARE(button.isEnabled(), false);
    QCOMPARE(button.isCheckable(), false);
    QCOMPARE(button.isChecked(), false);
    QCOMPARE(button.isVisible(), true);
    QCOMPARE(button.acceptedButtons(), Qt::LeftButton);

    // if the client is minimizeable the button should get enabled
    QSignalSpy minimizableChangedSpy(mockDecoration.client(), SIGNAL(minimizeableChanged(bool)));
    QVERIFY(minimizableChangedSpy.isValid());
    client->setMinimizable(true);
    QCOMPARE(button.isEnabled(), true);
    QCOMPARE(minimizableChangedSpy.count(), 1);
    QCOMPARE(minimizableChangedSpy.first().first().toBool(), true);

    // clicking the button should trigger a request for minimize
    QSignalSpy clickedSpy(&button, SIGNAL(clicked(Qt::MouseButton)));
    QVERIFY(clickedSpy.isValid());
    QSignalSpy pressedSpy(&button, SIGNAL(pressed()));
    QVERIFY(pressedSpy.isValid());
    QSignalSpy releasedSpy(&button, SIGNAL(released()));
    QVERIFY(releasedSpy.isValid());
    QSignalSpy minimizeRequestedSpy(client, SIGNAL(minimizeRequested()));
    QVERIFY(minimizeRequestedSpy.isValid());
    QSignalSpy pressedChangedSpy(&button, SIGNAL(pressedChanged(bool)));
    QVERIFY(pressedChangedSpy.isValid());

    QMouseEvent pressEvent(QEvent::MouseButtonPress, QPointF(5, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    pressEvent.setAccepted(false);
    button.event(&pressEvent);
    QCOMPARE(pressEvent.isAccepted(), true);
    QCOMPARE(button.isPressed(), true);
    QCOMPARE(clickedSpy.count(), 0);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(releasedSpy.count(), 0);
    QCOMPARE(minimizeRequestedSpy.count(), 0);
    QCOMPARE(pressedChangedSpy.count(), 1);
    QCOMPARE(pressedChangedSpy.first().first().toBool(), true);

    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPointF(5, 5), Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    releaseEvent.setAccepted(false);
    button.event(&releaseEvent);
    QCOMPARE(releaseEvent.isAccepted(), true);
    QCOMPARE(button.isPressed(), false);
    QCOMPARE(clickedSpy.count(), 1);
    QCOMPARE(clickedSpy.first().first().value<Qt::MouseButton>(), Qt::LeftButton);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(releasedSpy.count(), 1);
    QVERIFY(minimizeRequestedSpy.wait());
    QCOMPARE(minimizeRequestedSpy.count(), 1);
    QCOMPARE(pressedChangedSpy.count(), 2);
    QCOMPARE(pressedChangedSpy.last().first().toBool(), false);
}

void DecorationButtonTest::testQuickHelp()
{
    MockBridge bridge;
    MockDecoration mockDecoration(&bridge);
    MockClient *client = bridge.lastCreatedClient();
    MockButton button(KDecoration2::DecorationButtonType::ContextHelp, &mockDecoration);
    button.setGeometry(QRect(0, 0, 10, 10));

    QCOMPARE(button.isEnabled(), true);
    QCOMPARE(button.isCheckable(), false);
    QCOMPARE(button.isChecked(), false);
    QCOMPARE(button.isVisible(), false);
    QCOMPARE(button.acceptedButtons(), Qt::LeftButton);

    // if the client provides quickhelp the button should get enabled
    QSignalSpy providesContextHelpChangedSpy(mockDecoration.client(), SIGNAL(providesContextHelpChanged(bool)));
    QVERIFY(providesContextHelpChangedSpy.isValid());
    client->setProvidesContextHelp(true);
    QCOMPARE(button.isVisible(), true);
    QCOMPARE(providesContextHelpChangedSpy.count(), 1);
    QCOMPARE(providesContextHelpChangedSpy.first().first().toBool(), true);

    // clicking the button should trigger a request for minimize
    QSignalSpy clickedSpy(&button, SIGNAL(clicked(Qt::MouseButton)));
    QVERIFY(clickedSpy.isValid());
    QSignalSpy pressedSpy(&button, SIGNAL(pressed()));
    QVERIFY(pressedSpy.isValid());
    QSignalSpy releasedSpy(&button, SIGNAL(released()));
    QVERIFY(releasedSpy.isValid());
    QSignalSpy quickhelpRequestedSpy(client, SIGNAL(quickHelpRequested()));
    QVERIFY(quickhelpRequestedSpy.isValid());
    QSignalSpy pressedChangedSpy(&button, SIGNAL(pressedChanged(bool)));
    QVERIFY(pressedChangedSpy.isValid());

    QMouseEvent pressEvent(QEvent::MouseButtonPress, QPointF(5, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    pressEvent.setAccepted(false);
    button.event(&pressEvent);
    QCOMPARE(pressEvent.isAccepted(), true);
    QCOMPARE(button.isPressed(), true);
    QCOMPARE(clickedSpy.count(), 0);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(releasedSpy.count(), 0);
    QCOMPARE(quickhelpRequestedSpy.count(), 0);
    QCOMPARE(pressedChangedSpy.count(), 1);
    QCOMPARE(pressedChangedSpy.first().first().toBool(), true);

    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPointF(5, 5), Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    releaseEvent.setAccepted(false);
    button.event(&releaseEvent);
    QCOMPARE(releaseEvent.isAccepted(), true);
    QCOMPARE(button.isPressed(), false);
    QCOMPARE(clickedSpy.count(), 1);
    QCOMPARE(clickedSpy.first().first().value<Qt::MouseButton>(), Qt::LeftButton);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(releasedSpy.count(), 1);
    QVERIFY(quickhelpRequestedSpy.wait());
    QCOMPARE(quickhelpRequestedSpy.count(), 1);
    QCOMPARE(pressedChangedSpy.count(), 2);
    QCOMPARE(pressedChangedSpy.last().first().toBool(), false);
}

void DecorationButtonTest::testKeepAbove()
{
    MockBridge bridge;
    MockDecoration mockDecoration(&bridge);
    MockButton button(KDecoration2::DecorationButtonType::KeepAbove, &mockDecoration);
    button.setGeometry(QRect(0, 0, 10, 10));

    QCOMPARE(button.isEnabled(), true);
    QCOMPARE(button.isCheckable(), true);
    QCOMPARE(button.isChecked(), false);
    QCOMPARE(button.isVisible(), true);
    QCOMPARE(button.acceptedButtons(), Qt::LeftButton);

    // clicking the button should trigger a request for keep above changed
    QSignalSpy clickedSpy(&button, SIGNAL(clicked(Qt::MouseButton)));
    QVERIFY(clickedSpy.isValid());
    QSignalSpy pressedSpy(&button, SIGNAL(pressed()));
    QVERIFY(pressedSpy.isValid());
    QSignalSpy releasedSpy(&button, SIGNAL(released()));
    QVERIFY(releasedSpy.isValid());
    QSignalSpy keepAboveChangedSpy(mockDecoration.client(), SIGNAL(keepAboveChanged(bool)));
    QVERIFY(keepAboveChangedSpy.isValid());
    QSignalSpy pressedChangedSpy(&button, SIGNAL(pressedChanged(bool)));
    QVERIFY(pressedChangedSpy.isValid());

    QMouseEvent pressEvent(QEvent::MouseButtonPress, QPointF(5, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    pressEvent.setAccepted(false);
    button.event(&pressEvent);
    QCOMPARE(pressEvent.isAccepted(), true);
    QCOMPARE(button.isPressed(), true);
    QCOMPARE(clickedSpy.count(), 0);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(releasedSpy.count(), 0);
    QCOMPARE(keepAboveChangedSpy.count(), 0);
    QCOMPARE(pressedChangedSpy.count(), 1);
    QCOMPARE(pressedChangedSpy.first().first().toBool(), true);

    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPointF(5, 5), Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    releaseEvent.setAccepted(false);
    button.event(&releaseEvent);
    QCOMPARE(releaseEvent.isAccepted(), true);
    QCOMPARE(button.isPressed(), false);
    QCOMPARE(clickedSpy.count(), 1);
    QCOMPARE(clickedSpy.first().first().value<Qt::MouseButton>(), Qt::LeftButton);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(releasedSpy.count(), 1);
    QVERIFY(keepAboveChangedSpy.wait());
    QCOMPARE(keepAboveChangedSpy.count(), 1);
    QCOMPARE(keepAboveChangedSpy.first().first().toBool(), true);
    QCOMPARE(pressedChangedSpy.count(), 2);
    QCOMPARE(pressedChangedSpy.last().first().toBool(), false);
    QCOMPARE(button.isChecked(), true);

    // click once more should change again
    button.event(&pressEvent);
    button.event(&releaseEvent);
    QVERIFY(keepAboveChangedSpy.wait());
    QCOMPARE(keepAboveChangedSpy.count(), 2);
    QCOMPARE(keepAboveChangedSpy.first().first().toBool(), true);
    QCOMPARE(keepAboveChangedSpy.last().first().toBool(), false);
    QCOMPARE(button.isChecked(), false);
}

void DecorationButtonTest::testKeepBelow()
{
    MockBridge bridge;
    MockDecoration mockDecoration(&bridge);
    MockButton button(KDecoration2::DecorationButtonType::KeepBelow, &mockDecoration);
    button.setGeometry(QRect(0, 0, 10, 10));

    QCOMPARE(button.isEnabled(), true);
    QCOMPARE(button.isCheckable(), true);
    QCOMPARE(button.isChecked(), false);
    QCOMPARE(button.isVisible(), true);
    QCOMPARE(button.acceptedButtons(), Qt::LeftButton);

    // clicking the button should trigger a request for keep above changed
    QSignalSpy clickedSpy(&button, SIGNAL(clicked(Qt::MouseButton)));
    QVERIFY(clickedSpy.isValid());
    QSignalSpy pressedSpy(&button, SIGNAL(pressed()));
    QVERIFY(pressedSpy.isValid());
    QSignalSpy releasedSpy(&button, SIGNAL(released()));
    QVERIFY(releasedSpy.isValid());
    QSignalSpy keepBelowChangedSpy(mockDecoration.client(), SIGNAL(keepBelowChanged(bool)));
    QVERIFY(keepBelowChangedSpy.isValid());
    QSignalSpy pressedChangedSpy(&button, SIGNAL(pressedChanged(bool)));
    QVERIFY(pressedChangedSpy.isValid());

    QMouseEvent pressEvent(QEvent::MouseButtonPress, QPointF(5, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    pressEvent.setAccepted(false);
    button.event(&pressEvent);
    QCOMPARE(pressEvent.isAccepted(), true);
    QCOMPARE(button.isPressed(), true);
    QCOMPARE(clickedSpy.count(), 0);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(releasedSpy.count(), 0);
    QCOMPARE(keepBelowChangedSpy.count(), 0);
    QCOMPARE(pressedChangedSpy.count(), 1);
    QCOMPARE(pressedChangedSpy.first().first().toBool(), true);

    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPointF(5, 5), Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    releaseEvent.setAccepted(false);
    button.event(&releaseEvent);
    QCOMPARE(releaseEvent.isAccepted(), true);
    QCOMPARE(button.isPressed(), false);
    QCOMPARE(clickedSpy.count(), 1);
    QCOMPARE(clickedSpy.first().first().value<Qt::MouseButton>(), Qt::LeftButton);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(releasedSpy.count(), 1);
    QVERIFY(keepBelowChangedSpy.wait());
    QCOMPARE(keepBelowChangedSpy.count(), 1);
    QCOMPARE(keepBelowChangedSpy.first().first().toBool(), true);
    QCOMPARE(pressedChangedSpy.count(), 2);
    QCOMPARE(pressedChangedSpy.last().first().toBool(), false);
    QCOMPARE(button.isChecked(), true);

    // click once more should change again
    button.event(&pressEvent);
    button.event(&releaseEvent);
    QVERIFY(keepBelowChangedSpy.wait());
    QCOMPARE(keepBelowChangedSpy.count(), 2);
    QCOMPARE(keepBelowChangedSpy.first().first().toBool(), true);
    QCOMPARE(keepBelowChangedSpy.last().first().toBool(), false);
    QCOMPARE(button.isChecked(), false);
}

void DecorationButtonTest::testShade()
{
    MockBridge bridge;
    MockDecoration mockDecoration(&bridge);
    MockClient *client = bridge.lastCreatedClient();
    MockButton button(KDecoration2::DecorationButtonType::Shade, &mockDecoration);
    button.setGeometry(QRect(0, 0, 10, 10));

    QCOMPARE(button.isEnabled(), false);
    QCOMPARE(button.isCheckable(), true);
    QCOMPARE(button.isChecked(), false);
    QCOMPARE(button.isVisible(), true);
    QCOMPARE(button.acceptedButtons(), Qt::LeftButton);

    // if the client is shadeable the button should get enabled
    QSignalSpy shadeableChangedSpy(mockDecoration.client(), SIGNAL(shadeableChanged(bool)));
    QVERIFY(shadeableChangedSpy.isValid());
    client->setShadeable(true);
    QCOMPARE(button.isEnabled(), true);
    QCOMPARE(shadeableChangedSpy.count(), 1);
    QCOMPARE(shadeableChangedSpy.first().first().toBool(), true);

    // clicking the button should trigger a request for keep above changed
    QSignalSpy clickedSpy(&button, SIGNAL(clicked(Qt::MouseButton)));
    QVERIFY(clickedSpy.isValid());
    QSignalSpy pressedSpy(&button, SIGNAL(pressed()));
    QVERIFY(pressedSpy.isValid());
    QSignalSpy releasedSpy(&button, SIGNAL(released()));
    QVERIFY(releasedSpy.isValid());
    QSignalSpy shadedChangedSpy(mockDecoration.client(), SIGNAL(shadedChanged(bool)));
    QVERIFY(shadedChangedSpy.isValid());
    QSignalSpy pressedChangedSpy(&button, SIGNAL(pressedChanged(bool)));
    QVERIFY(pressedChangedSpy.isValid());

    QMouseEvent pressEvent(QEvent::MouseButtonPress, QPointF(5, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    pressEvent.setAccepted(false);
    button.event(&pressEvent);
    QCOMPARE(pressEvent.isAccepted(), true);
    QCOMPARE(button.isPressed(), true);
    QCOMPARE(clickedSpy.count(), 0);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(releasedSpy.count(), 0);
    QCOMPARE(shadedChangedSpy.count(), 0);
    QCOMPARE(pressedChangedSpy.count(), 1);
    QCOMPARE(pressedChangedSpy.first().first().toBool(), true);

    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPointF(5, 5), Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    releaseEvent.setAccepted(false);
    button.event(&releaseEvent);
    QCOMPARE(releaseEvent.isAccepted(), true);
    QCOMPARE(button.isPressed(), false);
    QCOMPARE(clickedSpy.count(), 1);
    QCOMPARE(clickedSpy.first().first().value<Qt::MouseButton>(), Qt::LeftButton);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(releasedSpy.count(), 1);
    QVERIFY(shadedChangedSpy.wait());
    QCOMPARE(shadedChangedSpy.count(), 1);
    QCOMPARE(shadedChangedSpy.first().first().toBool(), true);
    QCOMPARE(pressedChangedSpy.count(), 2);
    QCOMPARE(pressedChangedSpy.last().first().toBool(), false);
    QCOMPARE(button.isChecked(), true);

    // click once more should change again
    button.event(&pressEvent);
    button.event(&releaseEvent);
    QVERIFY(shadedChangedSpy.wait());
    QCOMPARE(shadedChangedSpy.count(), 2);
    QCOMPARE(shadedChangedSpy.first().first().toBool(), true);
    QCOMPARE(shadedChangedSpy.last().first().toBool(), false);
    QCOMPARE(button.isChecked(), false);
}

void DecorationButtonTest::testMaximize()
{
    MockBridge bridge;
    MockDecoration mockDecoration(&bridge);
    MockClient *client = bridge.lastCreatedClient();
    MockButton button(KDecoration2::DecorationButtonType::Maximize, &mockDecoration);
    button.setGeometry(QRect(0, 0, 10, 10));

    QCOMPARE(button.isEnabled(), false);
    QCOMPARE(button.isCheckable(), true);
    QCOMPARE(button.isChecked(), false);
    QCOMPARE(button.isVisible(), true);
    QCOMPARE(button.acceptedButtons(), Qt::LeftButton | Qt::MiddleButton | Qt::RightButton);

    // if the client is maximizable the button should get enabled
    QSignalSpy maximizableChangedSpy(mockDecoration.client(), SIGNAL(maximizeableChanged(bool)));
    QVERIFY(maximizableChangedSpy.isValid());
    client->setMaximizable(true);
    QCOMPARE(button.isEnabled(), true);
    QCOMPARE(maximizableChangedSpy.count(), 1);
    QCOMPARE(maximizableChangedSpy.first().first().toBool(), true);

    // clicking the button should trigger a request for keep above changed
    QSignalSpy clickedSpy(&button, SIGNAL(clicked(Qt::MouseButton)));
    QVERIFY(clickedSpy.isValid());
    QSignalSpy pressedSpy(&button, SIGNAL(pressed()));
    QVERIFY(pressedSpy.isValid());
    QSignalSpy releasedSpy(&button, SIGNAL(released()));
    QVERIFY(releasedSpy.isValid());
    QSignalSpy maximizedChangedSpy(mockDecoration.client(), SIGNAL(maximizedChanged(bool)));
    QVERIFY(maximizedChangedSpy.isValid());
    QSignalSpy maximizedVerticallyChangedSpy(mockDecoration.client(), SIGNAL(maximizedVerticallyChanged(bool)));
    QVERIFY(maximizedVerticallyChangedSpy.isValid());
    QSignalSpy maximizedHorizontallyChangedSpy(mockDecoration.client(), SIGNAL(maximizedHorizontallyChanged(bool)));
    QVERIFY(maximizedHorizontallyChangedSpy.isValid());
    QSignalSpy pressedChangedSpy(&button, SIGNAL(pressedChanged(bool)));
    QVERIFY(pressedChangedSpy.isValid());

    QMouseEvent leftPressEvent(QEvent::MouseButtonPress, QPointF(5, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    leftPressEvent.setAccepted(false);
    button.event(&leftPressEvent);
    QCOMPARE(leftPressEvent.isAccepted(), true);
    QCOMPARE(button.isPressed(), true);
    QCOMPARE(clickedSpy.count(), 0);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(releasedSpy.count(), 0);
    QCOMPARE(maximizedChangedSpy.count(), 0);
    QCOMPARE(pressedChangedSpy.count(), 1);
    QCOMPARE(pressedChangedSpy.first().first().toBool(), true);

    QMouseEvent leftReleaseEvent(QEvent::MouseButtonRelease, QPointF(5, 5), Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    leftReleaseEvent.setAccepted(false);
    button.event(&leftReleaseEvent);
    QCOMPARE(leftReleaseEvent.isAccepted(), true);
    QCOMPARE(button.isPressed(), false);
    QCOMPARE(clickedSpy.count(), 1);
    QCOMPARE(clickedSpy.first().first().value<Qt::MouseButton>(), Qt::LeftButton);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(releasedSpy.count(), 1);
    QVERIFY(maximizedChangedSpy.wait());
    QCOMPARE(maximizedChangedSpy.count(), 1);
    QCOMPARE(maximizedChangedSpy.first().first().toBool(), true);
    QCOMPARE(pressedChangedSpy.count(), 2);
    QCOMPARE(pressedChangedSpy.last().first().toBool(), false);
    QCOMPARE(button.isChecked(), true);

    // clicking again should set to restored
    button.event(&leftPressEvent);
    button.event(&leftReleaseEvent);
    QVERIFY(maximizedChangedSpy.wait());
    QCOMPARE(maximizedChangedSpy.count(), 2);
    QCOMPARE(maximizedChangedSpy.first().first().toBool(), true);
    QCOMPARE(maximizedChangedSpy.last().first().toBool(), false);
    QCOMPARE(button.isChecked(), false);

    // test the other buttons
    QMouseEvent rightPressEvent(QEvent::MouseButtonPress, QPointF(5, 5), Qt::RightButton, Qt::RightButton, Qt::NoModifier);
    rightPressEvent.setAccepted(false);
    button.event(&rightPressEvent);
    QCOMPARE(rightPressEvent.isAccepted(), true);
    QCOMPARE(button.isPressed(), true);

    QMouseEvent middlePressEvent(QEvent::MouseButtonPress, QPointF(5, 5), Qt::MiddleButton, Qt::MiddleButton, Qt::NoModifier);
    middlePressEvent.setAccepted(false);
    button.event(&middlePressEvent);
    QCOMPARE(middlePressEvent.isAccepted(), true);
    QCOMPARE(button.isPressed(), true);

    QMouseEvent middleReleaseEvent(QEvent::MouseButtonRelease, QPointF(5, 5), Qt::MiddleButton, Qt::NoButton, Qt::NoModifier);
    middleReleaseEvent.setAccepted(false);
    button.event(&middleReleaseEvent);
    QCOMPARE(middleReleaseEvent.isAccepted(), true);
    QVERIFY(maximizedHorizontallyChangedSpy.wait());
    QCOMPARE(button.isPressed(), true);
    QCOMPARE(clickedSpy.count(), 3);
    QCOMPARE(button.isChecked(), false);
    QCOMPARE(client->isMaximizedHorizontally(), true);
    QCOMPARE(client->isMaximizedVertically(), false);

    QMouseEvent rightReleaseEvent(QEvent::QEvent::MouseButtonRelease, QPointF(5, 5), Qt::RightButton, Qt::NoButton, Qt::NoModifier);
    rightReleaseEvent.setAccepted(false);
    button.event(&rightReleaseEvent);
    QCOMPARE(rightReleaseEvent.isAccepted(), true);
    QVERIFY(maximizedVerticallyChangedSpy.wait());
    QCOMPARE(button.isPressed(), false);
    QCOMPARE(clickedSpy.count(), 4);
    QCOMPARE(client->isMaximizedHorizontally(), true);
    QCOMPARE(client->isMaximizedVertically(), true);
    QCOMPARE(button.isChecked(), true);
}

void DecorationButtonTest::testOnAllDesktops()
{
    MockBridge bridge;
    auto decoSettings = QSharedPointer<KDecoration2::DecorationSettings>::create(&bridge);
    MockDecoration mockDecoration(&bridge);
    mockDecoration.setSettings(decoSettings);
    MockButton button(KDecoration2::DecorationButtonType::OnAllDesktops, &mockDecoration);
    button.setGeometry(QRect(0, 0, 10, 10));

    QCOMPARE(button.isEnabled(), true);
    QCOMPARE(button.isCheckable(), true);
    QCOMPARE(button.isChecked(), false);
    QCOMPARE(button.isVisible(), false);
    QCOMPARE(button.acceptedButtons(), Qt::LeftButton);
    QCOMPARE(mockDecoration.client()->isOnAllDesktops(), false);

    MockSettings *settings = bridge.lastCreatedSettings();
    QVERIFY(settings);

    QSignalSpy onAllDesktopsAvailableChangedSpy(decoSettings.data(), SIGNAL(onAllDesktopsAvailableChanged(bool)));
    QVERIFY(onAllDesktopsAvailableChangedSpy.isValid());
    QSignalSpy visibleChangedSpy(&button, SIGNAL(visibilityChanged(bool)));
    QVERIFY(visibleChangedSpy.isValid());

    settings->setOnAllDesktopsAvailabe(true);
    QCOMPARE(onAllDesktopsAvailableChangedSpy.count(), 1);
    QCOMPARE(onAllDesktopsAvailableChangedSpy.last().first().toBool(), true);
    QCOMPARE(visibleChangedSpy.count(), 1);
    QCOMPARE(visibleChangedSpy.last().first().toBool(), true);

    // clicking the button should trigger a request for on all desktops
    QSignalSpy clickedSpy(&button, SIGNAL(clicked(Qt::MouseButton)));
    QVERIFY(clickedSpy.isValid());
    QSignalSpy pressedSpy(&button, SIGNAL(pressed()));
    QVERIFY(pressedSpy.isValid());
    QSignalSpy releasedSpy(&button, SIGNAL(released()));
    QVERIFY(releasedSpy.isValid());
    QSignalSpy onAllDesktopsChangedSpy(mockDecoration.client(), SIGNAL(onAllDesktopsChanged(bool)));
    QVERIFY(onAllDesktopsChangedSpy.isValid());
    QSignalSpy pressedChangedSpy(&button, SIGNAL(pressedChanged(bool)));
    QVERIFY(pressedChangedSpy.isValid());

    QMouseEvent pressEvent(QEvent::MouseButtonPress, QPointF(5, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    pressEvent.setAccepted(false);
    button.event(&pressEvent);
    QCOMPARE(pressEvent.isAccepted(), true);
    QCOMPARE(button.isPressed(), true);
    QCOMPARE(clickedSpy.count(), 0);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(releasedSpy.count(), 0);
    QCOMPARE(onAllDesktopsChangedSpy.count(), 0);
    QCOMPARE(pressedChangedSpy.count(), 1);
    QCOMPARE(pressedChangedSpy.first().first().toBool(), true);

    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPointF(5, 5), Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    releaseEvent.setAccepted(false);
    button.event(&releaseEvent);
    QCOMPARE(releaseEvent.isAccepted(), true);
    QCOMPARE(button.isPressed(), false);
    QCOMPARE(clickedSpy.count(), 1);
    QCOMPARE(clickedSpy.first().first().value<Qt::MouseButton>(), Qt::LeftButton);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(releasedSpy.count(), 1);
    QVERIFY(onAllDesktopsChangedSpy.wait());
    QCOMPARE(onAllDesktopsChangedSpy.count(), 1);
    QCOMPARE(onAllDesktopsChangedSpy.first().first().toBool(), true);
    QCOMPARE(pressedChangedSpy.count(), 2);
    QCOMPARE(pressedChangedSpy.last().first().toBool(), false);
    QCOMPARE(button.isChecked(), true);
}

void DecorationButtonTest::testMenu()
{
    MockBridge bridge;
    auto decoSettings = QSharedPointer<KDecoration2::DecorationSettings>::create(&bridge);
    MockDecoration mockDecoration(&bridge);
    mockDecoration.setSettings(decoSettings);
    MockClient *client = bridge.lastCreatedClient();
    MockButton button(KDecoration2::DecorationButtonType::Menu, &mockDecoration);
    button.setGeometry(QRect(0, 0, 10, 10));

    QCOMPARE(button.isEnabled(), true);
    QCOMPARE(button.isCheckable(), false);
    QCOMPARE(button.isChecked(), false);
    QCOMPARE(button.isVisible(), true);
    QCOMPARE(button.acceptedButtons(), Qt::LeftButton | Qt::RightButton);

    // clicking the button should trigger a request for menu button
    QSignalSpy clickedSpy(&button, SIGNAL(clicked(Qt::MouseButton)));
    QVERIFY(clickedSpy.isValid());
    QSignalSpy pressedSpy(&button, SIGNAL(pressed()));
    QVERIFY(pressedSpy.isValid());
    QSignalSpy releasedSpy(&button, SIGNAL(released()));
    QVERIFY(releasedSpy.isValid());
    QSignalSpy pressedChangedSpy(&button, SIGNAL(pressedChanged(bool)));
    QVERIFY(pressedChangedSpy.isValid());
    QSignalSpy menuRequestedSpy(client, SIGNAL(menuRequested()));
    QVERIFY(menuRequestedSpy.isValid());

    QMouseEvent pressEvent(QEvent::MouseButtonPress, QPointF(5, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    pressEvent.setAccepted(false);
    button.event(&pressEvent);
    QCOMPARE(pressEvent.isAccepted(), true);
    QCOMPARE(button.isPressed(), true);
    QCOMPARE(clickedSpy.count(), 0);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(releasedSpy.count(), 0);
    QCOMPARE(menuRequestedSpy.count(), 0);
    QCOMPARE(pressedChangedSpy.count(), 1);
    QCOMPARE(pressedChangedSpy.first().first().toBool(), true);

    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPointF(5, 5), Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    releaseEvent.setAccepted(false);
    button.event(&releaseEvent);
    QCOMPARE(releaseEvent.isAccepted(), true);
    QCOMPARE(button.isPressed(), false);
    QCOMPARE(clickedSpy.count(), 1);
    QCOMPARE(clickedSpy.first().first().value<Qt::MouseButton>(), Qt::LeftButton);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(releasedSpy.count(), 1);
    QVERIFY(menuRequestedSpy.wait());
    QCOMPARE(menuRequestedSpy.count(), 1);
    QCOMPARE(pressedChangedSpy.count(), 2);
    QCOMPARE(pressedChangedSpy.last().first().toBool(), false);
}

void DecorationButtonTest::testMenuDoubleClick()
{
    MockBridge bridge;
    auto decoSettings = QSharedPointer<KDecoration2::DecorationSettings>::create(&bridge);
    MockDecoration mockDecoration(&bridge);
    mockDecoration.setSettings(decoSettings);
    MockClient *client = bridge.lastCreatedClient();
    MockButton button(KDecoration2::DecorationButtonType::Menu, &mockDecoration);
    button.setGeometry(QRect(0, 0, 10, 10));

    MockSettings *settings = bridge.lastCreatedSettings();
    QVERIFY(settings);
    QSignalSpy closeOnDoubleClickOnMenuChangedSpy(decoSettings.data(), SIGNAL(closeOnDoubleClickOnMenuChanged(bool)));
    QVERIFY(closeOnDoubleClickOnMenuChangedSpy.isValid());
    settings->setCloseOnDoubleClickOnMenu(true);
    QCOMPARE(closeOnDoubleClickOnMenuChangedSpy.count(), 1);
    QCOMPARE(closeOnDoubleClickOnMenuChangedSpy.last().first().toBool(), true);

    // button used a queued connection, so we need to run event loop
    QCoreApplication::processEvents();

    QSignalSpy clickedSpy(&button, SIGNAL(clicked(Qt::MouseButton)));
    QVERIFY(clickedSpy.isValid());
    QSignalSpy doubleClickedSpy(&button, SIGNAL(doubleClicked()));
    QVERIFY(doubleClickedSpy.isValid());
    QSignalSpy closeRequestedSpy(client, SIGNAL(closeRequested()));
    QVERIFY(closeRequestedSpy.isValid());
    QSignalSpy menuRequestedSpy(client, SIGNAL(menuRequested()));
    QVERIFY(menuRequestedSpy.isValid());

    QMouseEvent pressEvent(QEvent::MouseButtonPress, QPointF(5, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    pressEvent.setAccepted(false);
    button.event(&pressEvent);
    QCOMPARE(pressEvent.isAccepted(), true);
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPointF(5, 5), Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    releaseEvent.setAccepted(false);
    button.event(&releaseEvent);
    QCOMPARE(releaseEvent.isAccepted(), true);
    // should not have emitted a clicked
    QCOMPARE(clickedSpy.count(), 0);
    QCOMPARE(doubleClickedSpy.count(), 0);

    // another press should trigger the double click event
    pressEvent.setAccepted(false);
    button.event(&pressEvent);
    QCOMPARE(pressEvent.isAccepted(), true);
    QVERIFY(closeRequestedSpy.wait());
    QCOMPARE(doubleClickedSpy.count(), 1);
    QCOMPARE(closeRequestedSpy.count(), 1);
    QCOMPARE(menuRequestedSpy.count(), 0);

    releaseEvent.setAccepted(false);
    button.event(&releaseEvent);
    QCOMPARE(releaseEvent.isAccepted(), true);
    QCOMPARE(clickedSpy.count(), 0);
    // run events
    QCoreApplication::processEvents();
    QCOMPARE(closeRequestedSpy.count(), 1);
    QCOMPARE(menuRequestedSpy.count(), 0);

    // a double click of right button shouldn't trigger the double click event
    QMouseEvent rightPressEvent(QEvent::MouseButtonPress, QPointF(5, 5), Qt::RightButton, Qt::RightButton, Qt::NoModifier);
    rightPressEvent.setAccepted(false);
    button.event(&rightPressEvent);
    QCOMPARE(rightPressEvent.isAccepted(), true);
    QMouseEvent rightReleaseEvent(QEvent::MouseButtonRelease, QPointF(5, 5), Qt::RightButton, Qt::NoButton, Qt::NoModifier);
    rightReleaseEvent.setAccepted(false);
    button.event(&rightReleaseEvent);
    QCOMPARE(rightReleaseEvent.isAccepted(), true);
    QCOMPARE(clickedSpy.count(), 1);
    QVERIFY(menuRequestedSpy.wait());
    QCOMPARE(menuRequestedSpy.count(), 1);
    // second click
    rightPressEvent.setAccepted(false);
    button.event(&rightPressEvent);
    QCOMPARE(rightPressEvent.isAccepted(), true);
    rightReleaseEvent.setAccepted(false);
    button.event(&rightReleaseEvent);
    QCOMPARE(rightReleaseEvent.isAccepted(), true);
    QCOMPARE(clickedSpy.count(), 2);
    QVERIFY(menuRequestedSpy.wait());
    QCOMPARE(menuRequestedSpy.count(), 2);
}

void DecorationButtonTest::testMenuPressAndHold()
{
    MockBridge bridge;
    auto decoSettings = QSharedPointer<KDecoration2::DecorationSettings>::create(&bridge);
    MockDecoration mockDecoration(&bridge);
    mockDecoration.setSettings(decoSettings);
    MockClient *client = bridge.lastCreatedClient();
    MockButton button(KDecoration2::DecorationButtonType::Menu, &mockDecoration);
    button.setGeometry(QRect(0, 0, 10, 10));

    MockSettings *settings = bridge.lastCreatedSettings();
    QVERIFY(settings);
    QSignalSpy closeOnDoubleClickOnMenuChangedSpy(decoSettings.data(), SIGNAL(closeOnDoubleClickOnMenuChanged(bool)));
    QVERIFY(closeOnDoubleClickOnMenuChangedSpy.isValid());
    settings->setCloseOnDoubleClickOnMenu(true);
    QCOMPARE(closeOnDoubleClickOnMenuChangedSpy.count(), 1);
    QCOMPARE(closeOnDoubleClickOnMenuChangedSpy.last().first().toBool(), true);

    // button used a queued connection, so we need to run event loop
    QCoreApplication::processEvents();

    QSignalSpy menuRequestedSpy(client, SIGNAL(menuRequested()));
    QVERIFY(menuRequestedSpy.isValid());
    QSignalSpy doubleClickedSpy(&button, SIGNAL(doubleClicked()));
    QVERIFY(doubleClickedSpy.isValid());
    QSignalSpy closeRequestedSpy(client, SIGNAL(closeRequested()));
    QVERIFY(closeRequestedSpy.isValid());
    QSignalSpy clickedSpy(&button, SIGNAL(clicked(Qt::MouseButton)));
    QVERIFY(clickedSpy.isValid());

    // send a press event
    QMouseEvent pressEvent(QEvent::MouseButtonPress, QPointF(5, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    pressEvent.setAccepted(false);
    button.event(&pressEvent);
    QCOMPARE(pressEvent.isAccepted(), true);
    QCOMPARE(clickedSpy.count(), 0);

    // and wait
    QVERIFY(menuRequestedSpy.wait());
    QCOMPARE(menuRequestedSpy.count(), 1);
    QCOMPARE(clickedSpy.count(), 1);

    // send release event
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPointF(5, 5), Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    releaseEvent.setAccepted(false);
    button.event(&releaseEvent);
    QCOMPARE(releaseEvent.isAccepted(), true);
    QCOMPARE(clickedSpy.count(), 1);

    QTest::qWait(QGuiApplication::styleHints()->mouseDoubleClickInterval());

    // and it shouldn't be a double click
    pressEvent.setAccepted(false);
    button.event(&pressEvent);
    QCOMPARE(pressEvent.isAccepted(), true);

    // while waiting we disable click and hold
    settings->setCloseOnDoubleClickOnMenu(false);
    QCOMPARE(closeOnDoubleClickOnMenuChangedSpy.count(), 2);
    QCOMPARE(closeOnDoubleClickOnMenuChangedSpy.last().first().toBool(), false);
    // button used a queued connection, so we need to run event loop
    QCoreApplication::processEvents();
    // and releasing should emit the menu signal
    releaseEvent.setAccepted(false);
    button.event(&releaseEvent);
    QCOMPARE(releaseEvent.isAccepted(), true);
    QCOMPARE(clickedSpy.count(), 2);
    QVERIFY(menuRequestedSpy.wait());
    QCOMPARE(menuRequestedSpy.count(), 2);
    // never got a dobule click
    QCOMPARE(closeRequestedSpy.count(), 0);
}

QTEST_MAIN(DecorationButtonTest)
#include "decorationbuttontest.moc"
