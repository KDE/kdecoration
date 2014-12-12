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
#ifndef KDECORATION2_DECORATED_CLIENT_PRIVATE_H
#define KDECORATION2_DECORATED_CLIENT_PRIVATE_H

#include <kdecoration2/private/kdecoration2_private_export.h>

#include <QString>
#include <QIcon>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the KDecoration2 API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

namespace KDecoration2
{

class Decoration;
class DecoratedClient;

class KDECORATIONS_PRIVATE_EXPORT DecoratedClientPrivate
{
public:
    virtual ~DecoratedClientPrivate();
    virtual bool isActive() const = 0;
    virtual QString caption() const = 0;
    virtual int desktop() const = 0;
    virtual bool isOnAllDesktops() const = 0;
    virtual bool isShaded() const = 0;
    virtual QIcon icon() const = 0;
    virtual bool isMaximized() const = 0;
    virtual bool isMaximizedHorizontally() const = 0;
    virtual bool isMaximizedVertically() const = 0;
    virtual bool isKeepAbove() const = 0;
    virtual bool isKeepBelow() const = 0;

    virtual bool isCloseable() const = 0;
    virtual bool isMaximizeable() const = 0;
    virtual bool isMinimizeable() const = 0;
    virtual bool providesContextHelp() const = 0;
    virtual bool isModal() const = 0;
    virtual bool isShadeable() const = 0;
    virtual bool isMoveable() const = 0;
    virtual bool isResizeable() const = 0;

    virtual WId windowId() const = 0;
    virtual WId decorationId() const = 0;

    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual QPalette palette() const = 0;
    virtual Qt::Edges adjacentScreenEdges() const = 0;

    virtual void requestClose() = 0;
    virtual void requestToggleMaximization(Qt::MouseButtons buttons) = 0;
    virtual void requestMinimize() = 0;
    virtual void requestContextHelp() = 0;
    virtual void requestToggleOnAllDesktops() = 0;
    virtual void requestToggleShade() = 0;
    virtual void requestToggleKeepAbove() = 0;
    virtual void requestToggleKeepBelow() = 0;
    virtual void requestShowWindowMenu() = 0;

    Decoration *decoration();
    Decoration *decoration() const;

protected:
    explicit DecoratedClientPrivate(DecoratedClient *client, Decoration *decoration);
    DecoratedClient *client();

private:
    class Private;
    const QScopedPointer<Private> d;
};

} // namespace

#endif
