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
#include "decoration.h"
#include "decoration_p.h"
#include "decoratedclient.h"
#include "private/decoratedclientprivate.h"
#include "private/decorationbridge.h"
#include "decorationbutton.h"
#include "decorationshadow.h"

#include <QGuiApplication>
#include <QHoverEvent>
#include <QStyleHints>

namespace KDecoration2
{

namespace {
DecorationBridge *findBridge(const QVariantList &args)
{
    for (const auto &arg: args) {
        if (auto bridge = arg.toMap().value(QStringLiteral("bridge")).value<DecorationBridge*>()) {
            return bridge;
        }
    }
    Q_UNREACHABLE();
}
}

Decoration::Private::Private(Decoration *deco, const QVariantList &args)
    : sectionUnderMouse(Qt::NoSection)
    , bridge(findBridge(args))
    , client(new DecoratedClient(deco, bridge))
    , opaque(false)
    , q(deco)
{
    Q_UNUSED(args)
}

void Decoration::Private::setSectionUnderMouse(Qt::WindowFrameSection section)
{
    if (sectionUnderMouse == section) {
        return;
    }
    sectionUnderMouse = section;
    emit q->sectionUnderMouseChanged(sectionUnderMouse);
}

void Decoration::Private::updateSectionUnderMouse(const QPoint &mousePosition)
{
    if (titleBar.contains(mousePosition)) {
        setSectionUnderMouse(Qt::TitleBarArea);
        return;
    }
    const QSize size = q->size();
    const bool left   = mousePosition.x() < borders.left();
    const bool top    = mousePosition.y() < borders.top();
    const bool bottom = size.height() - mousePosition.y() < borders.bottom();
    const bool right  = size.width() - mousePosition.x() < borders.right();
    if (left) {
        if (top && mousePosition.y() < titleBar.y()) {
            setSectionUnderMouse(Qt::TopLeftSection);
        } else if (bottom && (mousePosition.y() > titleBar.y() + titleBar.height())) {
            setSectionUnderMouse(Qt::BottomLeftSection);
        } else {
            setSectionUnderMouse(Qt::LeftSection);
        }
        return;
    }
    if (right) {
        if (top && mousePosition.y() < titleBar.y()) {
            setSectionUnderMouse(Qt::TopRightSection);
        } else if (bottom && (mousePosition.y() > titleBar.y() + titleBar.height())) {
            setSectionUnderMouse(Qt::BottomRightSection);
        } else {
            setSectionUnderMouse(Qt::RightSection);
        }
        return;
    }
    if (bottom) {
        if (mousePosition.y() > titleBar.y() + titleBar.height()) {
            setSectionUnderMouse(Qt::BottomSection);
        } else {
            setSectionUnderMouse(Qt::TitleBarArea);
        }
        return;
    }
    if (top) {
        if (mousePosition.y() < titleBar.y()) {
            setSectionUnderMouse(Qt::TopSection);
        } else {
            setSectionUnderMouse(Qt::TitleBarArea);
        }
        return;
    }
    setSectionUnderMouse(Qt::NoSection);
}

void Decoration::Private::addButton(DecorationButton *button)
{
    Q_ASSERT(!buttons.contains(button));
    buttons << button;
    QObject::connect(button, &QObject::destroyed, q,
        [this](QObject *o) {
            buttons.removeAll(static_cast<DecorationButton*>(o));
        }
    );
}

bool Decoration::Private::wasDoubleClick() const
{
    if (!m_doubleClickTimer.isValid()) {
        return false;
    }
    return !m_doubleClickTimer.hasExpired(QGuiApplication::styleHints()->mouseDoubleClickInterval());
}

Decoration::Decoration(QObject *parent, const QVariantList &args)
    : QObject(parent)
    , d(new Private(this, args))
{
    connect(this, &Decoration::bordersChanged, this, [this]{ update(); });
}

Decoration::~Decoration() = default;

void Decoration::init()
{
    Q_ASSERT(!d->settings.isNull());
}

QPointer<DecoratedClient> Decoration::client() const
{
    return QPointer<DecoratedClient>(d->client);
}

#define DELEGATE(name) \
void Decoration::name() \
{ \
    d->client->d->name(); \
}

DELEGATE(requestClose)
DELEGATE(requestContextHelp)
DELEGATE(requestMinimize)
DELEGATE(requestToggleOnAllDesktops)
DELEGATE(requestToggleShade)
DELEGATE(requestToggleKeepAbove)
DELEGATE(requestToggleKeepBelow)
DELEGATE(requestShowWindowMenu)

#undef DELEGATE

void Decoration::requestToggleMaximization(Qt::MouseButtons buttons)
{
    d->client->d->requestToggleMaximization(buttons);
}

#define DELEGATE(name, variableName, type, emitValue) \
void Decoration::name(type a) \
{ \
    if (d->variableName == a) { \
        return; \
    } \
    d->variableName = a; \
    emit variableName##Changed(emitValue); \
}

DELEGATE(setBorders, borders, const QMargins&, )
DELEGATE(setResizeOnlyBorders, resizeOnlyBorders, const QMargins&, )
DELEGATE(setTitleBar, titleBar, const QRect&, )
DELEGATE(setOpaque, opaque, bool, d->opaque)
DELEGATE(setShadow, shadow, const QPointer<DecorationShadow> &, d->shadow)

#undef DELEGATE

#define DELEGATE(name, type) \
type Decoration::name() const \
{ \
    return d->name; \
}

DELEGATE(borders, QMargins)
DELEGATE(resizeOnlyBorders, QMargins)
DELEGATE(titleBar, QRect)
DELEGATE(sectionUnderMouse, Qt::WindowFrameSection)
DELEGATE(shadow, QPointer<DecorationShadow>)

#undef DELEGATE

bool Decoration::isOpaque() const
{
    return d->opaque;
}

#define BORDER(name, Name) \
int Decoration::border##Name() const \
{ \
    return d->borders.name(); \
} \
int Decoration::resizeOnlyBorder##Name() const \
{ \
    return d->resizeOnlyBorders.name(); \
}

BORDER(left, Left)
BORDER(right, Right)
BORDER(top, Top)
BORDER(bottom, Bottom)
#undef BORDER

QSize Decoration::size() const
{
    const QMargins &b = d->borders;
    return QSize(client()->width() + b.left() + b.right(),
                 client()->height() + b.top() + b.bottom());
}

QRect Decoration::rect() const
{
    return QRect(QPoint(0, 0), size());
}

bool Decoration::event(QEvent *event)
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

void Decoration::hoverEnterEvent(QHoverEvent *event)
{
    for (DecorationButton *button : d->buttons) {
        QCoreApplication::instance()->sendEvent(button, event);
    }
    d->updateSectionUnderMouse(event->pos());
}

void Decoration::hoverLeaveEvent(QHoverEvent *event)
{
    for (DecorationButton *button : d->buttons) {
        QCoreApplication::instance()->sendEvent(button, event);
    }
    d->setSectionUnderMouse(Qt::NoSection);
}

void Decoration::hoverMoveEvent(QHoverEvent *event)
{
    for (DecorationButton *button : d->buttons) {
        if (!button->isEnabled() || !button->isVisible()) {
            continue;
        }
        const bool hovered = button->isHovered();
        const bool contains = button->geometry().contains(event->pos());
        if (!hovered && contains) {
            QHoverEvent e(QEvent::HoverEnter, event->posF(), event->oldPosF(), event->modifiers());
            QCoreApplication::instance()->sendEvent(button, &e);
        } else if (hovered && !contains) {
            QHoverEvent e(QEvent::HoverLeave, event->posF(), event->oldPosF(), event->modifiers());
            QCoreApplication::instance()->sendEvent(button, &e);
        } else if (hovered && contains) {
            QCoreApplication::instance()->sendEvent(button, event);
        }
    }
    d->updateSectionUnderMouse(event->pos());
}

void Decoration::mouseMoveEvent(QMouseEvent *event)
{
    for (DecorationButton *button : d->buttons) {
        if (button->isPressed()) {
            QCoreApplication::instance()->sendEvent(button, event);
            return;
        }
    }
    // not handled, take care ourselves
}

void Decoration::mousePressEvent(QMouseEvent *event)
{
    for (DecorationButton *button : d->buttons) {
        if (button->isHovered()) {
            if (button->acceptedButtons().testFlag(event->button())) {
                QCoreApplication::instance()->sendEvent(button, event);
            }
            event->setAccepted(true);
            return;
        }
    }
    // not handled, take care ourselves
    if (event->button() == Qt::LeftButton) {
        if (d->titleBar.contains(event->pos())) {
            // check for double click
            if (d->wasDoubleClick()) {
                event->setAccepted(true);
                // emit signal
                emit titleBarDoubleClicked();
            }
        }
        d->invalidateDoubleClickTimer();
    }
}

void Decoration::mouseReleaseEvent(QMouseEvent *event)
{
    for (DecorationButton *button : d->buttons) {
        if (button->isPressed() && button->acceptedButtons().testFlag(event->button())) {
            QCoreApplication::instance()->sendEvent(button, event);
            return;
        }
    }
    // not handled, take care ourselves
    d->updateSectionUnderMouse(event->pos());
    if (event->button() == Qt::LeftButton && d->titleBar.contains(event->pos())) {
        d->startDoubleClickTimer();
    }
}

void Decoration::wheelEvent(QWheelEvent *event)
{
    if (d->titleBar.contains(event->pos())) {
        event->setAccepted(true);
        for (DecorationButton *button : d->buttons) {
            // check if a button contains the point
            if (button->geometry().contains(event->pos())) {
                return;
            }
        }
        emit titleBarWheelEvent(event->angleDelta());
    }
}

void Decoration::update(const QRect &r)
{
    d->bridge->update(this, r.isNull() ? rect() : r);
}

void Decoration::update()
{
    update(QRect());
}

void Decoration::setSettings(const QSharedPointer< DecorationSettings > &settings)
{
    d->settings = settings;
}

QSharedPointer< DecorationSettings > Decoration::settings() const
{
    return d->settings;
}

} // namespace
