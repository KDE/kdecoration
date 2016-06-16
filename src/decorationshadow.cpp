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
#include "decorationshadow.h"
#include "decorationshadow_p.h"

namespace KDecoration2
{

DecorationShadow::Private::Private(DecorationShadow *parent)
    : q(parent)
{
}

DecorationShadow::Private::~Private() = default;

DecorationShadow::DecorationShadow()
    : QObject()
    , d(new Private(this))
{
}

DecorationShadow::~DecorationShadow() = default;

QRect DecorationShadow::topLeftGeometry() const
{
    if (d->innerShadowRect.isNull() || d->shadow.isNull()) {
        return QRect();
    }
    return QRect(0, 0, d->innerShadowRect.left(), d->innerShadowRect.top());
}

QRect DecorationShadow::topGeometry() const
{
    if (d->innerShadowRect.isNull() || d->shadow.isNull()) {
        return QRect();
    }
    return QRect(d->innerShadowRect.left(), 0, d->innerShadowRect.width(), d->innerShadowRect.top());
}

QRect DecorationShadow::topRightGeometry() const
{
    if (d->innerShadowRect.isNull() || d->shadow.isNull()) {
        return QRect();
    }
    return QRect(d->innerShadowRect.left() + d->innerShadowRect.width(), 0,
                 d->shadow.width() - d->innerShadowRect.width() - d->innerShadowRect.left(),
                 d->innerShadowRect.top());
}

QRect DecorationShadow::rightGeometry() const
{
    if (d->innerShadowRect.isNull() || d->shadow.isNull()) {
        return QRect();
    }
    return QRect(d->innerShadowRect.left() + d->innerShadowRect.width(),
                 d->innerShadowRect.top(),
                 d->shadow.width() - d->innerShadowRect.width() - d->innerShadowRect.left(),
                 d->innerShadowRect.height());
}

QRect DecorationShadow::bottomRightGeometry() const
{
    if (d->innerShadowRect.isNull() || d->shadow.isNull()) {
        return QRect();
    }
    return QRect(d->innerShadowRect.left() + d->innerShadowRect.width(),
                 d->innerShadowRect.top() + d->innerShadowRect.height(),
                 d->shadow.width() - d->innerShadowRect.width() - d->innerShadowRect.left(),
                 d->shadow.height() - d->innerShadowRect.top() - d->innerShadowRect.height());
}

QRect DecorationShadow::bottomGeometry() const
{
    if (d->innerShadowRect.isNull() || d->shadow.isNull()) {
        return QRect();
    }
    return QRect(d->innerShadowRect.left(),
                 d->innerShadowRect.top() + d->innerShadowRect.height(),
                 d->innerShadowRect.width(),
                 d->shadow.height() - d->innerShadowRect.top() - d->innerShadowRect.height());
}

QRect DecorationShadow::bottomLeftGeometry() const
{
    if (d->innerShadowRect.isNull() || d->shadow.isNull()) {
        return QRect();
    }
    return QRect(0, d->innerShadowRect.top() + d->innerShadowRect.height(),
                 d->innerShadowRect.left(),
                 d->shadow.height() - d->innerShadowRect.top() - d->innerShadowRect.height());
}

QRect DecorationShadow::leftGeometry() const
{
    if (d->innerShadowRect.isNull() || d->shadow.isNull()) {
        return QRect();
    }
    return QRect(0, d->innerShadowRect.top(), d->innerShadowRect.left(), d->innerShadowRect.height());
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#define DELEGATE(type, name) \
    type DecorationShadow::name() const \
    { \
        return d->name; \
    }

DELEGATE(QImage, shadow)
DELEGATE(QMargins, padding)
DELEGATE(QRect, innerShadowRect)

#define I(name, Name) \
int DecorationShadow::padding##Name() const \
{ \
    return d->padding.name(); \
}
I(top, Top)
I(bottom, Bottom)
I(right, Right)
I(left, Left)
#undef I

#undef DELEGATE

#define SETTER(type, setName, name) \
    void DecorationShadow::setName(type arg) \
    { \
        if (d->name == arg) { \
            return; \
        } \
        d->name = arg; \
        emit name##Changed(d->name); \
    }

SETTER(const QImage&, setShadow, shadow)

#undef SETTER
#endif

void DecorationShadow::setPadding(const QMargins &margins)
{
    if (d->padding == margins) {
        return;
    }
    d->padding = margins;
    emit paddingChanged();
}

void DecorationShadow::setInnerShadowRect(const QRect &rect)
{
    if (d->innerShadowRect == rect) {
        return;
    }
    d->innerShadowRect = rect;
    emit innerShadowRectChanged();
}

}
