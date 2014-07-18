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

#include <QCoreApplication>
#include <QHoverEvent>

namespace KDecoration2
{

DecorationPrivate::DecorationPrivate(Decoration *deco)
    : q(deco)
    , m_client(new DecoratedClient(deco))
    , m_borderLeft(0)
    , m_borderRight(0)
    , m_borderTop(0)
    , m_borderBottom(0)
    , m_windowFrameSection(Qt::NoSection)
{
}

void DecorationPrivate::setBorders(int left, int right, int top, int bottom)
{
    m_borderLeft   = left;
    m_borderRight  = right;
    m_borderTop    = top;
    m_borderBottom = bottom;
    emit q->bordersChanged();
}

void DecorationPrivate::setWindowFrameSection(Qt::WindowFrameSection section)
{
    if (m_windowFrameSection == section) {
        return;
    }
    m_windowFrameSection = section;
    emit q->windowFrameSectionChanged(m_windowFrameSection);
}

void DecorationPrivate::updateWindowFrameSection(const QPoint &mousePosition)
{
    if (m_titleRect.contains(mousePosition)) {
        setWindowFrameSection(Qt::TitleBarArea);
        return;
    }
    const QSize size = q->size();
    const bool left   = mousePosition.x() < m_borderLeft;
    const bool top    = mousePosition.y() < m_borderTop;
    const bool bottom = size.height() - mousePosition.y() < m_borderBottom;
    const bool right  = size.width() - mousePosition.x() < m_borderRight;
    if (left) {
        if (top && mousePosition.y() < m_titleRect.y()) {
            setWindowFrameSection(Qt::TopLeftSection);
        } else if (bottom && (mousePosition.y() > m_titleRect.y() + m_titleRect.height())) {
            setWindowFrameSection(Qt::BottomLeftSection);
        } else {
            setWindowFrameSection(Qt::LeftSection);
        }
        return;
    }
    if (right) {
        if (top && mousePosition.y() < m_titleRect.y()) {
            setWindowFrameSection(Qt::TopRightSection);
        } else if (bottom && (mousePosition.y() > m_titleRect.y() + m_titleRect.height())) {
            setWindowFrameSection(Qt::BottomRightSection);
        } else {
            setWindowFrameSection(Qt::RightSection);
        }
        return;
    }
    if (bottom) {
        if (mousePosition.y() > m_titleRect.y() + m_titleRect.height()) {
            setWindowFrameSection(Qt::BottomSection);
        } else {
            setWindowFrameSection(Qt::TitleBarArea);
        }
        return;
    }
    if (top) {
        if (mousePosition.y() < m_titleRect.y()) {
            setWindowFrameSection(Qt::TopSection);
        } else {
            setWindowFrameSection(Qt::TitleBarArea);
        }
        return;
    }
    setWindowFrameSection(Qt::NoSection);
}

void DecorationPrivate::setTitleRect(const QRect &rect)
{
    if (m_titleRect == rect) {
        return;
    }
    m_titleRect = rect;
    emit q->titleRectChanged(rect);
    // TODO: updateWindowFrameSection?
}

void DecorationPrivate::addButton(DecorationButton *button)
{
    Q_ASSERT(!m_buttons.contains(button));
    m_buttons << button;
    QObject::connect(button, &QObject::destroyed, q,
        [this](QObject *o) {
            m_buttons.removeAll(static_cast<DecorationButton*>(o));
        }
    );
}

#define DELEGATE(name) \
void DecorationPrivate::name() \
{ \
    m_client->d->name(); \
}

DELEGATE(requestClose)
DELEGATE(requestMinimize)
DELEGATE(requestContextHelp)
DELEGATE(requestToggleOnAllDesktops)
DELEGATE(requestToggleShade)
DELEGATE(requestToggleKeepAbove)
DELEGATE(requestToggleKeepBelow)
DELEGATE(requestShowWindowMenu)
#undef DELEGATE

#define DELEGATE(name, type) \
void DecorationPrivate::name(type a) \
{ \
    m_client->d->name(a); \
}

DELEGATE(requestMaximize, Qt::MouseButtons)
#undef DELEGATE



Decoration::Decoration(QObject *parent)
    : QObject(parent)
    , d(new DecorationPrivate(this))
{
}

Decoration::~Decoration() = default;

DecoratedClient *Decoration::client()
{
    return d->client();
}

const DecoratedClient *Decoration::client() const
{
    return d->client();
}

void Decoration::setBorders(int left, int right, int top, int bottom)
{
    d->setBorders(left, right, top, bottom);
}

#define DELEGATE(name) \
void Decoration::name() \
{ \
    d->name(); \
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

#define DELEGATE(name, type) \
void Decoration::name(type a) \
{ \
    d->name(a); \
}

DELEGATE(requestMaximize, Qt::MouseButtons)
DELEGATE(addButton, DecorationButton*)
DELEGATE(setTitleRect, const QRect&)

#undef DELEGATE

#define DELEGATE(name, type) \
type Decoration::name() const \
{ \
    return d->name(); \
}\

DELEGATE(borderLeft, int)
DELEGATE(borderRight, int)
DELEGATE(borderTop, int)
DELEGATE(borderBottom, int)
DELEGATE(windowFrameSection, Qt::WindowFrameSection)
DELEGATE(titleRect, QRect)

#undef DELEGATE

QSize Decoration::size() const
{
    return QSize(client()->width() + borderLeft() + borderRight(),
                 client()->height() + borderTop() + borderBottom());
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
    default:
        return QObject::event(event);
    }
}

void Decoration::hoverEnterEvent(QHoverEvent *event)
{
    for (DecorationButton *button : d->buttons()) {
        QCoreApplication::instance()->sendEvent(button, event);
    }
    d->updateWindowFrameSection(event->pos());
}

void Decoration::hoverLeaveEvent(QHoverEvent *event)
{
    for (DecorationButton *button : d->buttons()) {
        QCoreApplication::instance()->sendEvent(button, event);
    }
    d->setWindowFrameSection(Qt::NoSection);
}

void Decoration::hoverMoveEvent(QHoverEvent *event)
{
    for (DecorationButton *button : d->buttons()) {
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
    d->updateWindowFrameSection(event->pos());
}

void Decoration::mouseMoveEvent(QMouseEvent *event)
{
    for (DecorationButton *button : d->buttons()) {
        if (button->isPressed()) {
            QCoreApplication::instance()->sendEvent(button, event);
            return;
        }
    }
    // not handled, take care ourselves
}

void Decoration::mousePressEvent(QMouseEvent *event)
{
    for (DecorationButton *button : d->buttons()) {
        if (button->isHovered() && button->acceptedButtons().testFlag(event->button())) {
            QCoreApplication::instance()->sendEvent(button, event);
            return;
        }
    }
    // not handled, take care ourselves
}

void Decoration::mouseReleaseEvent(QMouseEvent *event)
{
    for (DecorationButton *button : d->buttons()) {
        if (button->isPressed() && button->acceptedButtons().testFlag(event->button())) {
            QCoreApplication::instance()->sendEvent(button, event);
            return;
        }
    }
    // not handled, take care ourselves
    d->updateWindowFrameSection(event->pos());
}

void Decoration::update(const QRect &r)
{
    DecorationBridge::self()->update(this, r.isNull() ? rect() : r);
}

} // namespace
