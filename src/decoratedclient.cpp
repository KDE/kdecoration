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
#include "decoratedclient.h"
#include "private/decoratedclientprivate.h"
#include "private/decorationbridge.h"
#include "decoration.h"

namespace KDecoration2
{

DecoratedClient::DecoratedClient(Decoration *parent)
    : QObject(parent)
    , d(DecorationBridge::self()->createClient(this, parent))
{
}

DecoratedClient::~DecoratedClient() = default;

#define DELEGATE(type, method) \
type DecoratedClient::method() const \
{ \
    return d->method(); \
}

DELEGATE(bool, isActive)
DELEGATE(QString, caption)
DELEGATE(int, desktop)
DELEGATE(bool, isOnAllDesktops)
DELEGATE(bool, isShaded)
DELEGATE(QIcon, icon)
DELEGATE(bool, isMaximized)
DELEGATE(bool, isMaximizedHorizontally)
DELEGATE(bool, isMaximizedVertically)
DELEGATE(bool, isKeepAbove)
DELEGATE(bool, isKeepBelow)
DELEGATE(bool, isCloseable)
DELEGATE(bool, isMaximizable)
DELEGATE(bool, isMinimizable)
DELEGATE(bool, providesContextHelp)
DELEGATE(bool, isModal)
DELEGATE(bool, isShadeable)
DELEGATE(bool, isMovable)
DELEGATE(bool, isResizable)
DELEGATE(WId, windowId)
DELEGATE(WId, decorationId)
DELEGATE(int, width)
DELEGATE(int, height)
DELEGATE(Decoration *, decoration)
DELEGATE(QPalette, palette)
DELEGATE(Qt::Edges, borderingScreenEdges)

#undef DELEGATE

const DecoratedClientPrivate *DecoratedClient::handle() const
{
    return d.data();
}

DecoratedClientPrivate *DecoratedClient::handle()
{
    return d.data();
}

} // namespace
