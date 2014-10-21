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
    : paddingTop(0)
    , paddingRight(0)
    , paddingBottom(0)
    , paddingLeft(0)
    , q(parent)
{
}

DecorationShadow::Private::~Private() = default;

DecorationShadow::DecorationShadow(QObject *parent)
    : QObject(parent)
    , d(new Private(this))
{
}

DecorationShadow::~DecorationShadow() = default;

#define DELEGATE(type, name) \
    type DecorationShadow::name() const \
    { \
        return d->name; \
    }

DELEGATE(QImage, shadow)
#define S(name) DELEGATE(QSize, name)
S(topLeft)
S(top)
S(topRight)
S(right)
S(bottomRight)
S(bottom)
S(bottomLeft)
S(left)
#undef S

#define I(name) DELEGATE(int, name)
I(paddingTop)
I(paddingBottom)
I(paddingRight)
I(paddingLeft)
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

#define S(setName, name) SETTER(const QSize&, setName, name)
S(setTopLeft,     topLeft)
S(setTop,         top)
S(setTopRight,    topRight)
S(setRight,       right)
S(setBottomRight, bottomRight)
S(setBottom,      bottom)
S(setBottomLeft,  bottomLeft)
S(setLeft,        left)
#undef S

#define I(setName, name) SETTER(int, setName, name)
I(setPaddingTop,    paddingTop)
I(setPaddingRight,  paddingBottom)
I(setPaddingBottom, paddingRight)
I(setPaddingLeft,   paddingLeft)
#undef I

#undef SETTER

}
