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
#ifndef KDECORATION2_DECORATIONBUTTON_P_H
#define KDECORATION2_DECORATIONBUTTON_P_H

#include "decorationbutton.h"

class QElapsedTimer;
class QTimer;

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

class Q_DECL_HIDDEN DecorationButton::Private
{
public:
    explicit Private(DecorationButtonType type, const QPointer<Decoration> &decoration, DecorationButton *parent);
    ~Private();

    bool isPressed() const {
        return m_pressed != Qt::NoButton;
    }
    bool isPressed(Qt::MouseButton button) const {
        return m_pressed.testFlag(button);
    }

    void setHovered(bool hovered);
    void setPressed(Qt::MouseButton, bool pressed);
    void setAcceptedButtons(Qt::MouseButtons buttons);
    void setEnabled(bool enabled);
    void setChecked(bool checked);
    void setCheckable(bool checkable);
    void setVisible(bool visible);
    void startDoubleClickTimer();
    void invalidateDoubleClickTimer();
    bool wasDoubleClick() const;
    void setPressAndHold(bool enable);
    void startPressAndHold();
    void stopPressAndHold();

    QString typeToString(DecorationButtonType type);

    QPointer<Decoration> decoration;
    DecorationButtonType type;
    QRectF geometry;
    bool hovered;
    bool enabled;
    bool checkable;
    bool checked;
    bool visible;
    Qt::MouseButtons acceptedButtons;
    bool doubleClickEnabled;
    bool pressAndHold;

private:
    void init();
    DecorationButton *q;
    Qt::MouseButtons m_pressed;
    QScopedPointer<QElapsedTimer> m_doubleClickTimer;
    QScopedPointer<QTimer> m_pressAndHoldTimer;
};

}

#endif
