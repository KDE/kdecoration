/*
 * SPDX-FileCopyrightText: 2014 Martin Gräßlin <mgraesslin@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */
#include "decorationbuttongroup.h"
#include "decoration.h"
#include "decorationbuttongroup_p.h"
#include "decorationsettings.h"

#include <QDebug>

namespace KDecoration2
{
DecorationButtonGroup::Private::Private(Decoration *decoration, DecorationButtonGroup *parent)
    : decoration(decoration)
    , spacing(0.0)
    , q(parent)
{
}

DecorationButtonGroup::Private::~Private() = default;

void DecorationButtonGroup::Private::setGeometry(const QRectF &geo)
{
    if (geometry == geo) {
        return;
    }
    geometry = geo;
    Q_EMIT q->geometryChanged(geometry);
}

namespace
{
static bool s_layoutRecursion = false;
}

void DecorationButtonGroup::Private::updateLayout()
{
    if (s_layoutRecursion) {
        return;
    }
    s_layoutRecursion = true;
    const QPointF &pos = geometry.topLeft();
    // first calculate new size
    qreal height = 0;
    qreal width = 0;
    for (auto it = buttons.constBegin(); it != buttons.constEnd(); ++it) {
        if (!(*it)->isVisible()) {
            continue;
        }
        height = qMax(height, qreal((*it)->size().height()));
        width += (*it)->size().width();
        if (it + 1 != buttons.constEnd()) {
            width += spacing;
        }
    }
    setGeometry(QRectF(pos, QSizeF(width, height)));

    // now position all buttons
    qreal position = pos.x();
    const auto &constButtons = buttons;
    for (auto button : constButtons) {
        if (!button->isVisible()) {
            continue;
        }
        const QSizeF size = button->size();
        // TODO: center
        button->setGeometry(QRectF(QPointF(position, pos.y()), size));
        position += size.width() + spacing;
    }
    s_layoutRecursion = false;
}

DecorationButtonGroup::DecorationButtonGroup(Decoration *parent)
    : QObject(parent)
    , d(new Private(parent, this))
{
}

DecorationButtonGroup::DecorationButtonGroup(DecorationButtonGroup::Position type,
                                             Decoration *parent,
                                             std::function<DecorationButton *(DecorationButtonType, Decoration *, QObject *)> buttonCreator)
    : QObject(parent)
    , d(new Private(parent, this))
{
    auto settings = parent->settings();
    auto createButtons = [=] {
        const auto &buttons = (type == Position::Left) ? settings->decorationButtonsLeft() : settings->decorationButtonsRight();
        for (DecorationButtonType type : buttons) {
            if (DecorationButton *b = buttonCreator(type, parent, this)) {
                addButton(QPointer<DecorationButton>(b));
            }
        }
    };
    createButtons();
    auto changed = type == Position::Left ? &DecorationSettings::decorationButtonsLeftChanged : &DecorationSettings::decorationButtonsRightChanged;
    connect(settings.data(), changed, this, [this, createButtons] {
        qDeleteAll(d->buttons);
        d->buttons.clear();
        createButtons();
    });
}

DecorationButtonGroup::~DecorationButtonGroup() = default;

QPointer<Decoration> DecorationButtonGroup::decoration() const
{
    return QPointer<Decoration>(d->decoration);
}

QRectF DecorationButtonGroup::geometry() const
{
    return d->geometry;
}

bool DecorationButtonGroup::hasButton(DecorationButtonType type) const
{
    // TODO: check for deletion of button
    auto it = std::find_if(d->buttons.begin(), d->buttons.end(), [type](const QPointer<DecorationButton> &button) {
        return button->type() == type;
    });
    return it != d->buttons.end();
}

qreal DecorationButtonGroup::spacing() const
{
    return d->spacing;
}

QPointF DecorationButtonGroup::pos() const
{
    return d->geometry.topLeft();
}

void DecorationButtonGroup::setPos(const QPointF &pos)
{
    if (d->geometry.topLeft() == pos) {
        return;
    }
    d->setGeometry(QRectF(pos, d->geometry.size()));
    d->updateLayout();
}

void DecorationButtonGroup::setSpacing(qreal spacing)
{
    if (d->spacing == spacing) {
        return;
    }
    d->spacing = spacing;
    Q_EMIT spacingChanged(d->spacing);
    d->updateLayout();
}

void DecorationButtonGroup::addButton(const QPointer<DecorationButton> &button)
{
    Q_ASSERT(!button.isNull());
    connect(button.data(), &DecorationButton::visibilityChanged, this, [this]() {
        d->updateLayout();
    });
    connect(button.data(), &DecorationButton::geometryChanged, this, [this]() {
        d->updateLayout();
    });
    d->buttons.append(button);
    d->updateLayout();
}

QVector<QPointer<DecorationButton>> DecorationButtonGroup::buttons() const
{
    return d->buttons;
}

void DecorationButtonGroup::removeButton(DecorationButtonType type)
{
    bool needUpdate = false;
    auto it = d->buttons.begin();
    while (it != d->buttons.end()) {
        if ((*it)->type() == type) {
            it = d->buttons.erase(it);
            needUpdate = true;
        } else {
            it++;
        }
    }
    if (needUpdate) {
        d->updateLayout();
    }
}

void DecorationButtonGroup::removeButton(const QPointer<DecorationButton> &button)
{
    bool needUpdate = false;
    auto it = d->buttons.begin();
    while (it != d->buttons.end()) {
        if (*it == button) {
            it = d->buttons.erase(it);
            needUpdate = true;
        } else {
            it++;
        }
    }
    if (needUpdate) {
        d->updateLayout();
    }
}

void DecorationButtonGroup::paint(QPainter *painter, const QRect &repaintArea)
{
    const auto &buttons = d->buttons;
    for (auto button : buttons) {
        if (!button->isVisible()) {
            continue;
        }
        button->paint(painter, repaintArea);
    }
}

} // namespace
