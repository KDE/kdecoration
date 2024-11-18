/*
 * SPDX-FileCopyrightText: 2014 Martin Gräßlin <mgraesslin@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */
#include "decoratedwindow.h"
#include "private/decoratedwindowprivate.h"
#include "private/decorationbridge.h"

#include <QColor>

namespace KDecoration3
{
DecoratedWindow::DecoratedWindow(Decoration *parent, DecorationBridge *bridge)
    : QObject()
    , d(bridge->createClient(this, parent))
{
}

DecoratedWindow::~DecoratedWindow() = default;

bool DecoratedWindow::isActive() const
{
    return d->isActive();
}

QString DecoratedWindow::caption() const
{
    return d->caption();
}

bool DecoratedWindow::isOnAllDesktops() const
{
    return d->isOnAllDesktops();
}

bool DecoratedWindow::isShaded() const
{
    return d->isShaded();
}

QIcon DecoratedWindow::icon() const
{
    return d->icon();
}

bool DecoratedWindow::isMaximized() const
{
    return d->isMaximized();
}

bool DecoratedWindow::isMaximizedHorizontally() const
{
    return d->isMaximizedHorizontally();
}

bool DecoratedWindow::isMaximizedVertically() const
{
    return d->isMaximizedVertically();
}

bool DecoratedWindow::isKeepAbove() const
{
    return d->isKeepAbove();
}

bool DecoratedWindow::isKeepBelow() const
{
    return d->isKeepBelow();
}

bool DecoratedWindow::isCloseable() const
{
    return d->isCloseable();
}

bool DecoratedWindow::isMaximizeable() const
{
    return d->isMaximizeable();
}

bool DecoratedWindow::isMinimizeable() const
{
    return d->isMinimizeable();
}

bool DecoratedWindow::providesContextHelp() const
{
    return d->providesContextHelp();
}

bool DecoratedWindow::isModal() const
{
    return d->isModal();
}

bool DecoratedWindow::isShadeable() const
{
    return d->isShadeable();
}

bool DecoratedWindow::isMoveable() const
{
    return d->isMoveable();
}

bool DecoratedWindow::isResizeable() const
{
    return d->isResizeable();
}

int DecoratedWindow::width() const
{
    return d->width();
}

int DecoratedWindow::height() const
{
    return d->height();
}

QSize DecoratedWindow::size() const
{
    return d->size();
}

QPalette DecoratedWindow::palette() const
{
    return d->palette();
}

Qt::Edges DecoratedWindow::adjacentScreenEdges() const
{
    return d->adjacentScreenEdges();
}

QString DecoratedWindow::windowClass() const
{
    return d->windowClass();
}

bool DecoratedWindow::hasApplicationMenu() const
{
    if (const auto *appMenuEnabledPrivate = dynamic_cast<ApplicationMenuEnabledDecoratedWindowPrivate *>(d.get())) {
        return appMenuEnabledPrivate->hasApplicationMenu();
    }
    return false;
}

bool DecoratedWindow::isApplicationMenuActive() const
{
    if (const auto *appMenuEnabledPrivate = dynamic_cast<ApplicationMenuEnabledDecoratedWindowPrivate *>(d.get())) {
        return appMenuEnabledPrivate->isApplicationMenuActive();
    }
    return false;
}

Decoration *DecoratedWindow::decoration() const
{
    return d->decoration();
}

QColor DecoratedWindow::color(QPalette::ColorGroup group, QPalette::ColorRole role) const
{
    return d->palette().color(group, role);
}

QColor DecoratedWindow::color(ColorGroup group, ColorRole role) const
{
    return d->color(group, role);
}

void DecoratedWindow::showApplicationMenu(int actionId)
{
    if (auto *appMenuEnabledPrivate = dynamic_cast<ApplicationMenuEnabledDecoratedWindowPrivate *>(d.get())) {
        appMenuEnabledPrivate->showApplicationMenu(actionId);
    }
}

} // namespace

#include "moc_decoratedwindow.cpp"
