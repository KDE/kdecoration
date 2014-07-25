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
#include "decorationbutton.h"
#include "decorationbutton_p.h"
#include "decoration.h"
#include "decoratedclient.h"
#include "decorationsettings.h"

#include <QElapsedTimer>
#include <QHoverEvent>
#include <QGuiApplication>
#include <QStyleHints>
#include <QTimer>

namespace KDecoration2
{

uint qHash(const DecorationButtonType &type)
{
    return static_cast<uint>(type);
}

DecorationButtonPrivate::DecorationButtonPrivate(DecorationButtonType type, Decoration *decoration, DecorationButton *parent)
    : q(parent)
    , m_decoration(decoration)
    , m_type(type)
    , m_hovered(false)
    , m_enabled(true)
    , m_checkable(false)
    , m_checked(false)
    , m_visible(true)
    , m_pressed(Qt::NoButton)
    , m_buttons(Qt::LeftButton)
    , m_doubleClickEnabled(false)
    , m_pressAndHold(false)
{
    init();
}

DecorationButtonPrivate::~DecorationButtonPrivate() = default;

void DecorationButtonPrivate::init()
{
    DecoratedClient *c = m_decoration->client();
    switch (m_type) {
    case DecorationButtonType::Menu:
        QObject::connect(q, &DecorationButton::clicked, m_decoration, &Decoration::requestShowWindowMenu, Qt::QueuedConnection);
        QObject::connect(q, &DecorationButton::doubleClicked, m_decoration, &Decoration::requestClose, Qt::QueuedConnection);
        QObject::connect(DecorationSettings::self(), &DecorationSettings::closeOnDoubleClickOnMenuChanged, q,
            [this](bool enabled) {
                setDoubleClickEnabled(enabled);
                setPressAndHold(enabled);
            }, Qt::QueuedConnection
        );
        setDoubleClickEnabled(DecorationSettings::self()->isCloseOnDoubleClickOnMenu());
        setPressAndHold(DecorationSettings::self()->isCloseOnDoubleClickOnMenu());
        setAcceptedButtons(Qt::LeftButton | Qt::RightButton);
        break;
    case DecorationButtonType::OnAllDesktops:
        setVisible(DecorationSettings::self()->isOnAllDesktopsAvailable());
        setCheckable(true);
        setChecked(c->isOnAllDesktops());
        QObject::connect(q, &DecorationButton::clicked, m_decoration, &Decoration::requestToggleOnAllDesktops, Qt::QueuedConnection);
        QObject::connect(DecorationSettings::self(), &DecorationSettings::onAllDesktopsAvailableChanged, q, &DecorationButton::setVisible);
        QObject::connect(c, &DecoratedClient::onAllDesktopsChanged, q, &DecorationButton::setChecked);
        break;
    case DecorationButtonType::Minimize:
        setEnabled(c->isMinimizable());
        QObject::connect(q, &DecorationButton::clicked, m_decoration, &Decoration::requestMinimize, Qt::QueuedConnection);
        QObject::connect(c, &DecoratedClient::minimizableChanged, q, &DecorationButton::setEnabled);
        break;
    case DecorationButtonType::Maximize:
        setEnabled(c->isMaximizable());
        setCheckable(true);
        setChecked(c->isMaximized());
        setAcceptedButtons(Qt::LeftButton | Qt::MiddleButton | Qt::RightButton);
        QObject::connect(q, &DecorationButton::clicked, m_decoration, &Decoration::requestMaximize, Qt::QueuedConnection);
        QObject::connect(c, &DecoratedClient::maximizableChanged, q, &DecorationButton::setEnabled);
        QObject::connect(c, &DecoratedClient::maximizedChanged, q, &DecorationButton::setChecked);
        break;
    case DecorationButtonType::Close:
        setEnabled(c->isCloseable());
        QObject::connect(q, &DecorationButton::clicked, m_decoration, &Decoration::requestClose, Qt::QueuedConnection);
        QObject::connect(c, &DecoratedClient::closeableChanged, q, &DecorationButton::setEnabled);
        break;
    case DecorationButtonType::QuickHelp:
        // TODO: changed signal?
        setVisible(c->providesContextHelp());
        QObject::connect(q, &DecorationButton::clicked, m_decoration, &Decoration::requestContextHelp);
        break;
    case DecorationButtonType::KeepAbove:
        setCheckable(true);
        setChecked(c->isKeepAbove());
        QObject::connect(q, &DecorationButton::clicked, m_decoration, &Decoration::requestToggleKeepAbove, Qt::QueuedConnection);
        QObject::connect(c, &DecoratedClient::keepAboveChanged, q, &DecorationButton::setChecked);
        break;
    case DecorationButtonType::KeepBelow:
        setCheckable(true);
        setChecked(c->isKeepBelow());
        QObject::connect(q, &DecorationButton::clicked, m_decoration, &Decoration::requestToggleKeepBelow, Qt::QueuedConnection);
        QObject::connect(c, &DecoratedClient::keepBelowChanged, q, &DecorationButton::setChecked);
        break;
    case DecorationButtonType::Shade:
        setEnabled(c->isShadeable());
        setCheckable(true);
        setChecked(c->isShaded());
        QObject::connect(q, &DecorationButton::clicked, m_decoration, &Decoration::requestToggleShade, Qt::QueuedConnection);
        QObject::connect(c, &DecoratedClient::shadedChanged, q, &DecorationButton::setChecked);
        QObject::connect(c, &DecoratedClient::shadeableChanged, q, &DecorationButton::setEnabled);
        break;
    default:
        // nothing
        break;
    }
}

void DecorationButtonPrivate::setGeometry(const QRect &geometry)
{
    if (m_geometry == geometry) {
        return;
    }
    m_geometry = geometry;
    emit q->geometryChanged(m_geometry);
}

void DecorationButtonPrivate::setHovered(bool hovered)
{
    if (m_hovered == hovered) {
        return;
    }
    m_hovered = hovered;
    emit q->hoveredChanged(m_hovered);
}

void DecorationButtonPrivate::setEnabled(bool enabled)
{
    if (m_enabled == enabled) {
        return;
    }
    m_enabled = enabled;
    emit q->enabledChanged(m_enabled);
    if (!m_enabled) {
        setHovered(false);
        if (isPressed()) {
            m_pressed = Qt::NoButton;
            emit q->pressedChanged(false);
        }
    }
}

void DecorationButtonPrivate::setVisible(bool visible)
{
    if (m_visible == visible) {
        return;
    }
    m_visible = visible;
    emit q->visibilityChanged(visible);
}

void DecorationButtonPrivate::setChecked(bool checked)
{
    if (!m_checkable || m_checked == checked) {
        return;
    }
    m_checked = checked;
    emit q->checkedChanged(m_checked);
}

void DecorationButtonPrivate::setCheckable(bool checkable)
{
    if (m_checkable == checkable) {
        return;
    }
    if (!checkable) {
        setChecked(false);
    }
    m_checkable = checkable;
    emit q->checkableChanged(m_checkable);
}

void DecorationButtonPrivate::setPressed(Qt::MouseButton button, bool pressed)
{
    if (pressed) {
        m_pressed = m_pressed | button;
    } else {
        m_pressed = m_pressed & ~button;
    }
    emit q->pressedChanged(isPressed());
}

void DecorationButtonPrivate::setAcceptedButtons(Qt::MouseButtons buttons)
{
    if (m_buttons == buttons) {
        return;
    }
    m_buttons = buttons;
    emit q->acceptedButtonsChanged(m_buttons);
}

void DecorationButtonPrivate::startDoubleClickTimer()
{
    if (!m_doubleClickEnabled) {
        return;
    }
    if (m_doubleClickTimer.isNull()) {
        m_doubleClickTimer.reset(new QElapsedTimer());
    }
    m_doubleClickTimer->start();
}

void DecorationButtonPrivate::invalidateDoubleClickTimer()
{
    if (m_doubleClickTimer.isNull()) {
        return;
    }
    m_doubleClickTimer->invalidate();
}

bool DecorationButtonPrivate::wasDoubleClick() const
{
    if (m_doubleClickTimer.isNull() || !m_doubleClickTimer->isValid()) {
        return false;
    }
    return !m_doubleClickTimer->hasExpired(QGuiApplication::styleHints()->mouseDoubleClickInterval());
}


void DecorationButtonPrivate::setPressAndHold(bool enable) {
    if (m_pressAndHold == enable) {
        return;
    }
    m_pressAndHold = enable;
    if (!m_pressAndHold) {
        m_pressAndHoldTimer.reset();
    }
}

void DecorationButtonPrivate::startPressAndHold()
{
    if (!m_pressAndHold) {
        return;
    }
    if (m_pressAndHoldTimer.isNull()) {
        m_pressAndHoldTimer.reset(new QTimer());
        m_pressAndHoldTimer->setSingleShot(true);
        QObject::connect(m_pressAndHoldTimer.data(), &QTimer::timeout, q, [this]() { q->clicked(Qt::LeftButton); } );
    }
    m_pressAndHoldTimer->start(QGuiApplication::styleHints()->mousePressAndHoldInterval());
}

void DecorationButtonPrivate::stopPressAndHold()
{
    if (!m_pressAndHoldTimer.isNull()) {
        m_pressAndHoldTimer->stop();
    }
}

DecorationButton::DecorationButton(DecorationButtonType type, Decoration *decoration, QObject *parent)
    : QObject(parent)
    , d(new DecorationButtonPrivate(type, decoration, this))
{
    decoration->addButton(this);
    connect(this, &DecorationButton::geometryChanged, this, &DecorationButton::update);
    auto updateSlot = [this]() { update(); };
    connect(this, &DecorationButton::hoveredChanged, this, updateSlot);
    connect(this, &DecorationButton::pressedChanged, this, updateSlot);
    connect(this, &DecorationButton::checkedChanged, this, updateSlot);
    connect(this, &DecorationButton::enabledChanged, this, updateSlot);
    connect(this, &DecorationButton::visibilityChanged, this, updateSlot);
}

DecorationButton::~DecorationButton() = default;

void DecorationButton::update(const QRect &rect)
{
    decoration()->update(rect.isNull() ? geometry() : rect);
}

QSize DecorationButton::size() const
{
    return d->geometry().size();
}

#define DELEGATE(name, type) \
type DecorationButton::name() const \
{ \
    return d->name(); \
}\

DELEGATE(isHovered, bool)
DELEGATE(isPressed, bool)
DELEGATE(geometry, QRect)
DELEGATE(decoration, const Decoration*)
DELEGATE(acceptedButtons, Qt::MouseButtons)
DELEGATE(isEnabled, bool)
DELEGATE(isChecked, bool)
DELEGATE(isCheckable, bool)
DELEGATE(isVisible, bool)
DELEGATE(type, DecorationButtonType)

#undef DELEGATE

#define DELEGATE(name, type) \
    void DecorationButton::name(type a) \
    { \
        d->name(a); \
    }

DELEGATE(setGeometry, const QRect &)
DELEGATE(setAcceptedButtons, Qt::MouseButtons)
DELEGATE(setEnabled, bool)
DELEGATE(setChecked, bool)
DELEGATE(setCheckable, bool)
DELEGATE(setVisible, bool)

#undef DELEGATE

Decoration *DecorationButton::decoration()
{
    return d->decoration();
}

bool DecorationButton::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::HoverEnter:
        hoverEnterEvent(static_cast<QHoverEvent*>(event));
        return true;
    case QEvent::HoverLeave:
        hoverLeaveEvent(static_cast<QHoverEvent*>(event));
        return true;
    case QEvent::HoverMove:
        hoverMoveEvent(static_cast<QHoverEvent*>(event));
        return true;
    case QEvent::MouseButtonPress:
        mousePressEvent(static_cast<QMouseEvent*>(event));
        return true;
    case QEvent::MouseButtonRelease:
        mouseReleaseEvent(static_cast<QMouseEvent*>(event));
        return true;
    case QEvent::MouseMove:
        mouseMoveEvent(static_cast<QMouseEvent*>(event));
        return true;
    default:
        return QObject::event(event);
    }
}

void DecorationButton::hoverEnterEvent(QHoverEvent *event)
{
    if (!d->isEnabled() || !d->isVisible() || !d->geometry().contains(event->pos())) {
        return;
    }
    d->setHovered(true);
    event->setAccepted(true);
}

void DecorationButton::hoverLeaveEvent(QHoverEvent *event)
{
    if (!d->isEnabled() || !d->isVisible() || !d->isHovered()) {
        return;
    }
    d->setHovered(false);
    event->setAccepted(true);
}

void DecorationButton::hoverMoveEvent(QHoverEvent *event)
{
    Q_UNUSED(event)
}

void DecorationButton::mouseMoveEvent(QMouseEvent *event)
{
    if (!d->isEnabled() || !d->isVisible() || !d->isHovered()) {
        return;
    }
    if (!d->geometry().contains(event->pos())) {
        d->setHovered(false);
        event->setAccepted(true);
    }
}

void DecorationButton::mousePressEvent(QMouseEvent *event)
{
    if (!d->isEnabled() || !d->isVisible() || !d->geometry().contains(event->pos())) {
        return;
    }
    d->setPressed(event->button(), true);
    event->setAccepted(true);
    if (d->isDoubleClickEnabled() && event->button() == Qt::LeftButton) {
        // check for double click
        if (d->wasDoubleClick()) {
            event->setAccepted(true);
            emit doubleClicked();
        }
        d->invalidateDoubleClickTimer();
    }
    if (d->isPressAndHold() && event->button() == Qt::LeftButton) {
        d->startPressAndHold();
    }
}

void DecorationButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (!d->isEnabled() || !d->isVisible() || !d->isPressed(event->button())) {
        return;
    }
    if (d->geometry().contains(event->pos())) {
        if (!d->isPressAndHold() || event->button() != Qt::LeftButton) {
            emit clicked(event->button());
        } else {
            d->stopPressAndHold();
        }
    }
    d->setPressed(event->button(), false);
    event->setAccepted(true);

    if (d->isDoubleClickEnabled() && event->button() == Qt::LeftButton) {
        d->startDoubleClickTimer();
    }
}

}
