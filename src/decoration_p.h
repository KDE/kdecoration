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
#ifndef KDECORATION2_DECORATION_P_H
#define KDECORATION2_DECORATION_P_H
#include "decoration.h"

#include <QElapsedTimer>

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
class DecorationButton;
class DecoratedClient;
class DecorationSettings;
class DecorationShadow;

class Decoration::Private
{
public:
    Private(Decoration *decoration);
    void requestClose();
    void requestMaximize(Qt::MouseButtons buttons);
    void requestMinimize();
    void requestContextHelp();
    void requestToggleOnAllDesktops();
    void requestToggleShade();
    void requestToggleKeepAbove();
    void requestToggleKeepBelow();
    void requestShowWindowMenu();

    DecoratedClient *client() {
        return m_client;
    }

    const DecoratedClient *client() const {
        return m_client;
    }

    int borderLeft() const {
        return m_borderLeft;
    }
    int borderRight() const {
        return m_borderRight;
    }
    int borderTop() const {
        return m_borderTop;
    }
    int borderBottom() const {
        return m_borderBottom;
    }
    void setBorders(int left, int right, int top, int bottom);

    int extendedBorderLeft() const {
        return m_extendedBorderLeft;
    }
    int extendedBorderRight() const {
        return m_extendedBorderRight;
    }
    int extendedBorderTop() const {
        return m_extendedBorderTop;
    }
    int extendedBorderBottom() const {
        return m_extendedBorderBottom;
    }
    void setExtendedBorders(int left, int right, int top, int bottom);

    Qt::WindowFrameSection windowFrameSection() const {
        return m_windowFrameSection;
    }
    void setWindowFrameSection(Qt::WindowFrameSection section);
    void updateWindowFrameSection(const QPoint &mousePosition);

    const QRect &titleRect() const {
        return m_titleRect;
    }
    void setTitleRect(const QRect &rect);

    void addButton(DecorationButton *button);

    const QList<DecorationButton*> &buttons() const {
        return m_buttons;
    }

    bool isOpaque() const {
        return m_opaque;
    }
    void setOpaque(bool opaque);

    QPointer<DecorationShadow> shadow() const {
        return m_shadow;
    }
    void setShadow(QPointer<DecorationShadow> shadow);

    void startDoubleClickTimer() {
        m_doubleClickTimer.start();
    }
    void invalidateDoubleClickTimer() {
        m_doubleClickTimer.invalidate();
    }
    bool wasDoubleClick() const;

    QSharedPointer<DecorationSettings> settings;

private:
    Decoration *q;
    DecoratedClient *m_client;
    int m_borderLeft;
    int m_borderRight;
    int m_borderTop;
    int m_borderBottom;
    int m_extendedBorderLeft;
    int m_extendedBorderRight;
    int m_extendedBorderTop;
    int m_extendedBorderBottom;
    QList<DecorationButton*> m_buttons;
    Qt::WindowFrameSection m_windowFrameSection;
    QRect m_titleRect;
    bool m_opaque;
    QPointer<DecorationShadow> m_shadow;
    QElapsedTimer m_doubleClickTimer;
};

} // namespace

#endif
