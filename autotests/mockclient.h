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
#ifndef MOCK_CLIENT_H
#define MOCK_CLIENT_H

#include "../src/private/decoratedclientprivate.h"

#include <QObject>

class MockClient : public QObject, public KDecoration2::DecoratedClientPrivate
{
    Q_OBJECT
public:
    explicit MockClient(KDecoration2::DecoratedClient *client, KDecoration2::Decoration *decoration);

    Qt::Edges adjacentScreenEdges() const override;
    QString caption() const override;
    WId decorationId() const override;
    int desktop() const override;
    int height() const override;
    QIcon icon() const override;
    bool isActive() const override;
    bool isCloseable() const override;
    bool isKeepAbove() const override;
    bool isKeepBelow() const override;
    bool isMaximizeable() const override;
    bool isMaximized() const override;
    bool isMaximizedHorizontally() const override;
    bool isMaximizedVertically() const override;
    bool isMinimizeable() const override;
    bool isModal() const override;
    bool isMoveable() const override;
    bool isOnAllDesktops() const override;
    bool isResizeable() const override;
    bool isShadeable() const override;
    bool isShaded() const override;
    QPalette palette() const override;
    bool providesContextHelp() const override;
    void requestClose() override;
    void requestContextHelp() override;
    void requestToggleMaximization(Qt::MouseButtons buttons) override;
    void requestMinimize() override;
    void requestShowWindowMenu() override;
    void requestToggleKeepAbove() override;
    void requestToggleKeepBelow() override;
    void requestToggleOnAllDesktops() override;
    void requestToggleShade() override;
    int width() const override;
    WId windowId() const override;

    void setCloseable(bool set);
    void setMinimizable(bool set);
    void setProvidesContextHelp(bool set);
    void setShadeable(bool set);
    void setMaximizable(bool set);

    void setWidth(int w);
    void setHeight(int h);

Q_SIGNALS:
    void closeRequested();
    void minimizeRequested();
    void quickHelpRequested();
    void menuRequested();

private:
    bool m_closeable = false;
    bool m_minimizable = false;
    bool m_contextHelp = false;
    bool m_keepAbove = false;
    bool m_keepBelow = false;
    bool m_shadeable = false;
    bool m_shaded = false;
    bool m_maximizable = false;
    bool m_maximizedVertically = false;
    bool m_maximizedHorizontally = false;
    bool m_onAllDesktops = false;
    int m_width = 0;
    int m_height = 0;
};

#endif
