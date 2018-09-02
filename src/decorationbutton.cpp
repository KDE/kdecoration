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
#include "decoration_p.h"
#include "decoratedclient.h"
#include "decorationsettings.h"

#include <KLocalizedString>

#include <QDebug>
#include <QElapsedTimer>
#include <QHoverEvent>
#include <QGuiApplication>
#include <QStyleHints>
#include <QTimer>

namespace KDecoration2
{

#ifndef DOXYGEN_SHOULD_SKIP_THIS
uint qHash(const DecorationButtonType &type)
{
    return static_cast<uint>(type);
}
#endif

DecorationButton::Private::Private(DecorationButtonType type, const QPointer<Decoration> &decoration, DecorationButton *parent)
    : decoration(decoration)
    , type(type)
    , hovered(false)
    , enabled(true)
    , checkable(false)
    , checked(false)
    , visible(true)
    , acceptedButtons(Qt::LeftButton)
    , doubleClickEnabled(false)
    , pressAndHold(false)
    , q(parent)
    , m_pressed(Qt::NoButton)
{
    init();
}

DecorationButton::Private::~Private() = default;

void DecorationButton::Private::init()
{
    auto c = decoration->client().data();
    auto settings = decoration->settings();
    switch (type) {
    case DecorationButtonType::Menu:
        QObject::connect(q, &DecorationButton::clicked, decoration.data(), &Decoration::requestShowWindowMenu, Qt::QueuedConnection);
        QObject::connect(q, &DecorationButton::doubleClicked, decoration.data(), &Decoration::requestClose, Qt::QueuedConnection);
        QObject::connect(settings.data(), &DecorationSettings::closeOnDoubleClickOnMenuChanged, q,
            [this](bool enabled) {
                doubleClickEnabled = enabled;
                setPressAndHold(enabled);
            }, Qt::QueuedConnection
        );
        doubleClickEnabled = settings->isCloseOnDoubleClickOnMenu();
        setPressAndHold(settings->isCloseOnDoubleClickOnMenu());
        setAcceptedButtons(Qt::LeftButton | Qt::RightButton);
        break;
    case DecorationButtonType::ApplicationMenu:
        setVisible(c->hasApplicationMenu());
        setCheckable(true); // will be "checked" whilst the menu is opened
        // FIXME TODO connect directly and figure out the button geometry/offset stuff
        QObject::connect(q, &DecorationButton::clicked, decoration.data(), [this] {
            decoration->requestShowApplicationMenu(q->geometry().toRect(), 0 /* actionId */);
        }, Qt::QueuedConnection); //&Decoration::requestShowApplicationMenu, Qt::QueuedConnection);
        QObject::connect(c, &DecoratedClient::hasApplicationMenuChanged, q, &DecorationButton::setVisible);
        QObject::connect(c, &DecoratedClient::applicationMenuActiveChanged, q, &DecorationButton::setChecked);
        break;
    case DecorationButtonType::OnAllDesktops:
        setVisible(settings->isOnAllDesktopsAvailable());
        setCheckable(true);
        setChecked(c->isOnAllDesktops());
        QObject::connect(q, &DecorationButton::clicked, decoration.data(), &Decoration::requestToggleOnAllDesktops, Qt::QueuedConnection);
        QObject::connect(settings.data(), &DecorationSettings::onAllDesktopsAvailableChanged, q, &DecorationButton::setVisible);
        QObject::connect(c, &DecoratedClient::onAllDesktopsChanged, q, &DecorationButton::setChecked);
        break;
    case DecorationButtonType::Minimize:
        setEnabled(c->isMinimizeable());
        QObject::connect(q, &DecorationButton::clicked, decoration.data(), &Decoration::requestMinimize, Qt::QueuedConnection);
        QObject::connect(c, &DecoratedClient::minimizeableChanged, q, &DecorationButton::setEnabled);
        break;
    case DecorationButtonType::Maximize:
        setEnabled(c->isMaximizeable());
        setCheckable(true);
        setChecked(c->isMaximized());
        setAcceptedButtons(Qt::LeftButton | Qt::MiddleButton | Qt::RightButton);
        QObject::connect(q, &DecorationButton::clicked, decoration.data(), &Decoration::requestToggleMaximization, Qt::QueuedConnection);
        QObject::connect(c, &DecoratedClient::maximizeableChanged, q, &DecorationButton::setEnabled);
        QObject::connect(c, &DecoratedClient::maximizedChanged, q, &DecorationButton::setChecked);
        break;
    case DecorationButtonType::Close:
        setEnabled(c->isCloseable());
        QObject::connect(q, &DecorationButton::clicked, decoration.data(), &Decoration::requestClose, Qt::QueuedConnection);
        QObject::connect(c, &DecoratedClient::closeableChanged, q, &DecorationButton::setEnabled);
        break;
    case DecorationButtonType::ContextHelp:
        setVisible(c->providesContextHelp());
        QObject::connect(q, &DecorationButton::clicked, decoration.data(), &Decoration::requestContextHelp, Qt::QueuedConnection);
        QObject::connect(c, &DecoratedClient::providesContextHelpChanged, q, &DecorationButton::setVisible);
        break;
    case DecorationButtonType::KeepAbove:
        setCheckable(true);
        setChecked(c->isKeepAbove());
        QObject::connect(q, &DecorationButton::clicked, decoration.data(), &Decoration::requestToggleKeepAbove, Qt::QueuedConnection);
        QObject::connect(c, &DecoratedClient::keepAboveChanged, q, &DecorationButton::setChecked);
        break;
    case DecorationButtonType::KeepBelow:
        setCheckable(true);
        setChecked(c->isKeepBelow());
        QObject::connect(q, &DecorationButton::clicked, decoration.data(), &Decoration::requestToggleKeepBelow, Qt::QueuedConnection);
        QObject::connect(c, &DecoratedClient::keepBelowChanged, q, &DecorationButton::setChecked);
        break;
    case DecorationButtonType::Shade:
        setEnabled(c->isShadeable());
        setCheckable(true);
        setChecked(c->isShaded());
        QObject::connect(q, &DecorationButton::clicked, decoration.data(), &Decoration::requestToggleShade, Qt::QueuedConnection);
        QObject::connect(c, &DecoratedClient::shadedChanged, q, &DecorationButton::setChecked);
        QObject::connect(c, &DecoratedClient::shadeableChanged, q, &DecorationButton::setEnabled);
        break;
    default:
        // nothing
        break;
    }
}

void DecorationButton::Private::setHovered(bool set)
{
    if (hovered == set) {
        return;
    }
    hovered = set;
    emit q->hoveredChanged(hovered);
}

void DecorationButton::Private::setEnabled(bool set)
{
    if (enabled == set) {
        return;
    }
    enabled = set;
    emit q->enabledChanged(enabled);
    if (!enabled) {
        setHovered(false);
        if (isPressed()) {
            m_pressed = Qt::NoButton;
            emit q->pressedChanged(false);
        }
    }
}

void DecorationButton::Private::setVisible(bool set)
{
    if (visible == set) {
        return;
    }
    visible = set;
    emit q->visibilityChanged(set);
    if (!visible) {
        setHovered(false);
        if (isPressed()) {
            m_pressed = Qt::NoButton;
            emit q->pressedChanged(false);
        }
    }
}

void DecorationButton::Private::setChecked(bool set)
{
    if (!checkable || checked == set) {
        return;
    }
    checked = set;
    emit q->checkedChanged(checked);
}

void DecorationButton::Private::setCheckable(bool set)
{
    if (checkable == set) {
        return;
    }
    if (!set) {
        setChecked(false);
    }
    checkable = set;
    emit q->checkableChanged(checkable);
}

void DecorationButton::Private::setPressed(Qt::MouseButton button, bool pressed)
{
    if (pressed) {
        m_pressed = m_pressed | button;
    } else {
        m_pressed = m_pressed & ~button;
    }
    emit q->pressedChanged(isPressed());
}

void DecorationButton::Private::setAcceptedButtons(Qt::MouseButtons buttons)
{
    if (acceptedButtons == buttons) {
        return;
    }
    acceptedButtons = buttons;
    emit q->acceptedButtonsChanged(acceptedButtons);
}

void DecorationButton::Private::startDoubleClickTimer()
{
    if (!doubleClickEnabled) {
        return;
    }
    if (m_doubleClickTimer.isNull()) {
        m_doubleClickTimer.reset(new QElapsedTimer());
    }
    m_doubleClickTimer->start();
}

void DecorationButton::Private::invalidateDoubleClickTimer()
{
    if (m_doubleClickTimer.isNull()) {
        return;
    }
    m_doubleClickTimer->invalidate();
}

bool DecorationButton::Private::wasDoubleClick() const
{
    if (m_doubleClickTimer.isNull() || !m_doubleClickTimer->isValid()) {
        return false;
    }
    return !m_doubleClickTimer->hasExpired(QGuiApplication::styleHints()->mouseDoubleClickInterval());
}


void DecorationButton::Private::setPressAndHold(bool enable) {
    if (pressAndHold == enable) {
        return;
    }
    pressAndHold = enable;
    if (!pressAndHold) {
        m_pressAndHoldTimer.reset();
    }
}

void DecorationButton::Private::startPressAndHold()
{
    if (!pressAndHold) {
        return;
    }
    if (m_pressAndHoldTimer.isNull()) {
        m_pressAndHoldTimer.reset(new QTimer());
        m_pressAndHoldTimer->setSingleShot(true);
        QObject::connect(m_pressAndHoldTimer.data(), &QTimer::timeout, q, [this]() { q->clicked(Qt::LeftButton); } );
    }
    m_pressAndHoldTimer->start(QGuiApplication::styleHints()->mousePressAndHoldInterval());
}

void DecorationButton::Private::stopPressAndHold()
{
    if (!m_pressAndHoldTimer.isNull()) {
        m_pressAndHoldTimer->stop();
    }
}

QString DecorationButton::Private::typeToString(DecorationButtonType type)
{
    switch (type) {
    case DecorationButtonType::Menu:
        return i18n("Menu");
    case DecorationButtonType::ApplicationMenu:
        return i18n("Application menu");
    case DecorationButtonType::OnAllDesktops:
        if ( this->q->isChecked() )
            return i18n("On one desktop");
        else
            return i18n("On all desktops");
    case DecorationButtonType::Minimize:
        return i18n("Minimize");
    case DecorationButtonType::Maximize:
        if ( this->q->isChecked() )
            return i18n("Restore");
        else
            return i18n("Maximize");
    case DecorationButtonType::Close:
        return i18n("Close");
    case DecorationButtonType::ContextHelp:
        return i18n("Context help");
    case DecorationButtonType::Shade:
        if ( this->q->isChecked() )
            return i18n("Unshade");
        else
            return i18n("Shade");
    case DecorationButtonType::KeepBelow:
        if ( this->q->isChecked() )
            return i18n("Don't keep below");
        else
            return i18n("Keep below");
    case DecorationButtonType::KeepAbove:
        if ( this->q->isChecked() )
            return i18n("Don't keep above");
        else
            return i18n("Keep above");
    default:
        return QString();
    }
}

DecorationButton::DecorationButton(DecorationButtonType type, const QPointer<Decoration> &decoration, QObject *parent)
    : QObject(parent)
    , d(new Private(type, decoration, this))
{
    decoration->d->addButton(this);
    connect(this, &DecorationButton::geometryChanged,
            this, static_cast<void (DecorationButton::*)(const QRectF&)>(&DecorationButton::update));
    auto updateSlot = static_cast<void (DecorationButton::*)()>(&DecorationButton::update);
    connect(this, &DecorationButton::hoveredChanged, this, updateSlot);
    connect(this, &DecorationButton::hoveredChanged, this,
        [this](bool hovered) {
            if (hovered) {
                //TODO: show tooltip if hovered and hide if not
                const QString type = this->d->typeToString(this->type());
                this->decoration()->requestShowToolTip(type);
            } else {
                this->decoration()->requestHideToolTip();
            }
        }
    );
    connect(this, &DecorationButton::pressedChanged, this, updateSlot);
    connect(this, &DecorationButton::checkedChanged, this, updateSlot);
    connect(this, &DecorationButton::enabledChanged, this, updateSlot);
    connect(this, &DecorationButton::visibilityChanged, this, updateSlot);
    connect(this, &DecorationButton::hoveredChanged, this,
        [this](bool hovered) {
            if (hovered) {
                emit pointerEntered();
            } else {
                emit pointerLeft();
            }
        }
    );
    connect(this, &DecorationButton::pressedChanged, this,
        [this](bool p) {
            if (p) {
                emit pressed();
            } else {
                emit released();
            }
        }
    );
}

DecorationButton::~DecorationButton() = default;

void DecorationButton::update(const QRectF &rect)
{
    decoration()->update(rect.isNull() ? geometry().toRect() : rect.toRect());
}

void DecorationButton::update()
{
    update(QRectF());
}

QSizeF DecorationButton::size() const
{
    return d->geometry.size();
}

bool DecorationButton::isPressed() const
{
    return d->isPressed();
}

#define DELEGATE(name, variableName, type) \
type DecorationButton::name() const \
{ \
    return d->variableName; \
}

DELEGATE(isHovered, hovered, bool)
DELEGATE(isEnabled, enabled, bool)
DELEGATE(isChecked, checked, bool)
DELEGATE(isCheckable, checkable, bool)
DELEGATE(isVisible, visible, bool)

#define DELEGATE2(name, type) DELEGATE(name, name, type)
DELEGATE2(geometry, QRectF)
DELEGATE2(decoration, QPointer<Decoration>)
DELEGATE2(acceptedButtons, Qt::MouseButtons)
DELEGATE2(type, DecorationButtonType)

#undef DELEGATE2
#undef DELEGATE

#define DELEGATE(name, type) \
    void DecorationButton::name(type a) \
    { \
        d->name(a); \
    }

DELEGATE(setAcceptedButtons, Qt::MouseButtons)
DELEGATE(setEnabled, bool)
DELEGATE(setChecked, bool)
DELEGATE(setCheckable, bool)
DELEGATE(setVisible, bool)

#undef DELEGATE

#define DELEGATE(name, variableName, type) \
void DecorationButton::name(type a) \
{ \
    if (d->variableName == a) { \
        return; \
    } \
    d->variableName = a; \
    emit variableName##Changed(d->variableName); \
}

DELEGATE(setGeometry, geometry, const QRectF &)

#undef DELEGATE

bool DecorationButton::contains(const QPointF &pos) const
{
    return d->geometry.toRect().contains(pos.toPoint());
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
    case QEvent::Wheel:
        wheelEvent(static_cast<QWheelEvent*>(event));
        return true;
    default:
        return QObject::event(event);
    }
}

void DecorationButton::hoverEnterEvent(QHoverEvent *event)
{
    if (!d->enabled || !d->visible || !contains(event->posF())) {
        return;
    }
    d->setHovered(true);
    event->setAccepted(true);
}

void DecorationButton::hoverLeaveEvent(QHoverEvent *event)
{
    if (!d->enabled || !d->visible || !d->hovered || contains(event->posF())) {
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
    if (!d->enabled || !d->visible || !d->hovered) {
        return;
    }
    if (!contains(event->localPos())) {
        d->setHovered(false);
        event->setAccepted(true);
    }
}

void DecorationButton::mousePressEvent(QMouseEvent *event)
{
    if (!d->enabled || !d->visible || !contains(event->localPos()) || !d->acceptedButtons.testFlag(event->button())) {
        return;
    }
    d->setPressed(event->button(), true);
    event->setAccepted(true);
    if (d->doubleClickEnabled && event->button() == Qt::LeftButton) {
        // check for double click
        if (d->wasDoubleClick()) {
            event->setAccepted(true);
            emit doubleClicked();
        }
        d->invalidateDoubleClickTimer();
    }
    if (d->pressAndHold && event->button() == Qt::LeftButton) {
        d->startPressAndHold();
    }
}

void DecorationButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (!d->enabled || !d->visible || !d->isPressed(event->button())) {
        return;
    }
    if (contains(event->localPos())) {
        if (!d->pressAndHold || event->button() != Qt::LeftButton) {
            emit clicked(event->button());
        } else {
            d->stopPressAndHold();
        }
    }
    d->setPressed(event->button(), false);
    event->setAccepted(true);

    if (d->doubleClickEnabled && event->button() == Qt::LeftButton) {
        d->startDoubleClickTimer();
    }
}

void DecorationButton::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event)
}

}
