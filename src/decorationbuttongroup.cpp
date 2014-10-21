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
#include "decorationsettings.h"

#include <QDebug>

namespace KDecoration2
{

DecorationButtonGroup::Private::Private(Decoration *decoration, DecorationButtonGroup *parent)
    : q(parent)
    , m_decoration(decoration)
    , m_spacing(0.0)
{
}

DecorationButtonGroup::Private::~Private() = default;

void DecorationButtonGroup::Private::setSpacing(qreal spacing)
{
    if (m_spacing == spacing) {
        return;
    }
    m_spacing = spacing;
    emit q->spacingChanged(m_spacing);
    updateLayout();
}

void DecorationButtonGroup::Private::setGeometry(const QRectF &geometry)
{
    if (m_geometry == geometry) {
        return;
    }
    m_geometry = geometry;
    emit q->geometryChanged(m_geometry);
}

static bool s_layoutRecursion = false;

void DecorationButtonGroup::Private::updateLayout()
{
    if (s_layoutRecursion) {
        return;
    }
    s_layoutRecursion = true;
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
    s_layoutRecursion = false;
}

DecorationButtonGroup::DecorationButtonGroup(Decoration *parent)
    : QObject(parent)
    , d(new Private(parent, this))
{
}

DecorationButtonGroup::DecorationButtonGroup(DecorationButtonGroup::Position type, Decoration *parent, std::function<DecorationButton*(DecorationButtonType, Decoration*, QObject*)> buttonCreator)
    : QObject(parent)
    , d(new Private(parent, this))
{
    auto createButtons = [=] {
        const QList<DecorationButtonType> buttons = (type == Position::Left) ?
            DecorationSettings::self()->decorationButtonsLeft() :
            DecorationSettings::self()->decorationButtonsRight();
        for (DecorationButtonType type : buttons) {
            if (DecorationButton *b = buttonCreator(type, parent, this)) {
                addButton(QPointer<DecorationButton>(b));
            }
        }
    };
    createButtons();
    auto changed = type == Position::Left ? &DecorationSettings::decorationButtonsLeftChanged : &DecorationSettings::decorationButtonsRightChanged;
    connect(DecorationSettings::self(), changed, this,
        [this, createButtons] {
            while (!d->buttons().isEmpty()) {
                delete d->buttons().takeFirst();
            }
            createButtons();
        }
    );
}

DecorationButtonGroup::~DecorationButtonGroup() = default;

QPointer<Decoration> DecorationButtonGroup::decoration() const
{
    return QPointer<Decoration>(d->decoration());
}

QRectF DecorationButtonGroup::geometry() const
{
    return d->geometry();
}

bool DecorationButtonGroup::hasButton(DecorationButtonType type) const
{
    auto it = std::find_if(d->buttons().begin(), d->buttons().end(),
        [type](const QPointer<DecorationButton> &button) {
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

void DecorationButtonGroup::addButton(QPointer<DecorationButton> button)
{
    Q_ASSERT(!button.isNull());
    connect(button.data(), &DecorationButton::visibilityChanged, this, [this]() { d->updateLayout(); });
    connect(button.data(), &DecorationButton::geometryChanged, this, [this]() { d->updateLayout(); });
    d->buttons().append(button);
    d->updateLayout();
}

QList< QPointer<DecorationButton> > DecorationButtonGroup::buttons() const
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

void DecorationButtonGroup::removeButton(QPointer<DecorationButton> button)
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
