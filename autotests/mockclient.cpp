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
#include "mockclient.h"
#include <decoratedclient.h>

#include <QPalette>

MockClient::MockClient(KDecoration2::DecoratedClient *client, KDecoration2::Decoration *decoration)
    : QObject()
    , DecoratedClientPrivate(client, decoration)
{
}

Qt::Edges MockClient::borderingScreenEdges() const
{
    return Qt::Edges();
}

QString MockClient::caption() const
{
    return QString();
}

WId MockClient::decorationId() const
{
    return 0;
}

int MockClient::desktop() const
{
    return 1;
}

int MockClient::height() const
{
    return 0;
}

QIcon MockClient::icon() const
{
    return QIcon();
}

bool MockClient::isActive() const
{
    return false;
}

bool MockClient::isCloseable() const
{
    return m_closeable;
}

bool MockClient::isKeepAbove() const
{
    return m_keepAbove;
}

bool MockClient::isKeepBelow() const
{
    return m_keepBelow;
}

bool MockClient::isMaximizable() const
{
    return false;
}

bool MockClient::isMaximized() const
{
    return false;
}

bool MockClient::isMaximizedHorizontally() const
{
    return false;
}

bool MockClient::isMaximizedVertically() const
{
    return false;
}

bool MockClient::isMinimizable() const
{
    return m_minimizable;
}

bool MockClient::isModal() const
{
    return false;
}

bool MockClient::isMovable() const
{
    return false;
}

bool MockClient::isOnAllDesktops() const
{
    return false;
}

bool MockClient::isResizable() const
{
    return false;
}

bool MockClient::isShadeable() const
{
    return m_shadeable;
}

bool MockClient::isShaded() const
{
    return m_shaded;
}

QPalette MockClient::palette() const
{
    return QPalette();
}

bool MockClient::providesContextHelp() const
{
    return m_contextHelp;
}

void MockClient::requestClose()
{
    emit closeRequested();
}

void MockClient::requestContextHelp()
{
    emit quickHelpRequested();
}

void MockClient::requestMaximize(Qt::MouseButtons buttons)
{
    Q_UNUSED(buttons)
}

void MockClient::requestMinimize()
{
    emit minimizeRequested();
}

void MockClient::requestShowWindowMenu()
{
}

void MockClient::requestToggleKeepAbove()
{
    m_keepAbove = !m_keepAbove;
    emit client()->keepAboveChanged(m_keepAbove);
}

void MockClient::requestToggleKeepBelow()
{
    m_keepBelow = !m_keepBelow;
    emit client()->keepBelowChanged(m_keepBelow);
}

void MockClient::requestToggleOnAllDesktops()
{
}

void MockClient::requestToggleShade()
{
    m_shaded = !m_shaded;
    emit client()->shadedChanged(m_shaded);
}

int MockClient::width() const
{
    return 0;
}

WId MockClient::windowId() const
{
    return 0;
}

void MockClient::setCloseable(bool set)
{
    m_closeable = set;
    emit client()->closeableChanged(set);
}

void MockClient::setMinimizable(bool set)
{
    m_minimizable = set;
    emit client()->minimizableChanged(set);
}

void MockClient::setProvidesContextHelp(bool set)
{
    m_contextHelp = set;
    emit client()->providesContextHelpChanged(set);
}

void MockClient::setShadeable(bool set)
{
    m_shadeable = set;
    emit client()->shadeableChanged(set);
}
