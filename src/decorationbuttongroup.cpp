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
#include "decorationbuttongroup.h"
#include "decorationbuttongroup_p.h"
#include "decoration.h"

namespace KDecoration2
{

DecorationButtonGroupPrivate::DecorationButtonGroupPrivate(Decoration *decoration, DecorationButtonGroup *parent)
    : q(parent)
    , m_decoration(decoration)
    , m_spacing(0.0)
{
}

DecorationButtonGroupPrivate::~DecorationButtonGroupPrivate() = default;

void DecorationButtonGroupPrivate::setSpacing(qreal spacing)
{
    if (m_spacing == spacing) {
        return;
    }
    m_spacing = spacing;
    emit q->spacingChanged(m_spacing);
    updateLayout();
}

void DecorationButtonGroupPrivate::setGeometry(const QRectF &geometry)
{
    if (m_geometry == geometry) {
        return;
    }
    m_geometry = geometry;
    emit q->geometryChanged(m_geometry);
}

void DecorationButtonGroupPrivate::updateLayout()
{
    const QPointF &pos = m_geometry.topLeft();
    // first calculate new size
    qreal height = 0;
    qreal width = 0;
    for (auto it = m_buttons.constBegin(); it != m_buttons.constEnd(); ++it) {
        if (!(*it)->isVisible()) {
            continue;
        }
        height = qMax(height, qreal((*it)->size().height()));
        width += (*it)->size().width();
        if (it + 1 != m_buttons.constEnd()) {
            width += m_spacing;
        }
    }
    setGeometry(QRectF(pos, QSizeF(width, height)));

    // now position all buttons
    qreal position = pos.x();
    for (auto it = m_buttons.constBegin(); it != m_buttons.constEnd(); ++it) {
        if (!(*it)->isVisible()) {
            continue;
        }
        const QSize size = (*it)->size();
        // TODO: center
        (*it)->setGeometry(QRect(QPoint(position, pos.y()), size));
        position += size.width() + m_spacing;
    }
}

DecorationButtonGroup::DecorationButtonGroup(Decoration *parent)
    : QObject(parent)
    , d(new DecorationButtonGroupPrivate(parent, this))
{
}

DecorationButtonGroup::~DecorationButtonGroup() = default;

Decoration *DecorationButtonGroup::decoration()
{
    return d->decoration();
}

Decoration *DecorationButtonGroup::decoration() const
{
    return d->decoration();
}

QRectF DecorationButtonGroup::geometry() const
{
    return d->geometry();
}

bool DecorationButtonGroup::hasButton(DecorationButtonType type) const
{
    auto it = std::find_if(d->buttons().begin(), d->buttons().end(),
        [type](DecorationButton *button) {
            return button->type() == type;
        }
    );
    return it != d->buttons().end();
}

qreal DecorationButtonGroup::spacing() const
{
    return d->spacing();
}

QPointF DecorationButtonGroup::pos() const
{
    return d->geometry().topLeft();
}

void DecorationButtonGroup::setPos(const QPointF &pos)
{
    if (d->geometry().topLeft() == pos) {
        return;
    }
    d->setGeometry(QRectF(pos, d->geometry().size()));
    d->updateLayout();
}

void DecorationButtonGroup::setSpacing(qreal spacing)
{
    d->setSpacing(spacing);
}

void DecorationButtonGroup::addButton(DecorationButton *button)
{
    connect(button, &DecorationButton::visibilityChanged, this, [this]() { d->updateLayout(); });
    d->buttons().append(button);
    d->updateLayout();
}

QList< DecorationButton* > DecorationButtonGroup::buttons() const
{
    return d->buttons();
}

void DecorationButtonGroup::removeButton(DecorationButtonType type)
{
    bool needUpdate = false;
    auto it = d->buttons().begin();
    while (it != d->buttons().end()) {
        if ((*it)->type() == type) {
            it = d->buttons().erase(it);
            needUpdate = true;
        } else {
            it++;
        }
    }
    if (needUpdate) {
        d->updateLayout();
    }
}

void DecorationButtonGroup::removeButton(DecorationButton *button)
{
    bool needUpdate = false;
    auto it = d->buttons().begin();
    while (it != d->buttons().end()) {
        if (*it == button) {
            it = d->buttons().erase(it);
            needUpdate = true;
        } else {
            it++;
        }
    }
    if (needUpdate) {
        d->updateLayout();
    }
}

void DecorationButtonGroup::paint(QPainter *painter)
{
    for (auto it = d->buttons().constBegin(); it != d->buttons().constEnd(); ++it) {
        if (!(*it)->isVisible()) {
            continue;
        }
        (*it)->paint(painter);
    }
}

} // namespace
