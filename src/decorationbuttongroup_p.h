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
#ifndef KDECORATION2_DECORATIONBUTTONGROUP_P_H
#define KDECORATION2_DECORATIONBUTTONGROUP_P_H

#include <QRectF>

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
class DecorationButtonGroup;

class DecorationButtonGroupPrivate
{
public:
    explicit DecorationButtonGroupPrivate(Decoration *decoration, DecorationButtonGroup *parent);
    ~DecorationButtonGroupPrivate();

    Decoration *decoration() const {
        return m_decoration;
    }
    Decoration *decoration() {
        return m_decoration;
    }
    qreal spacing() const {
        return m_spacing;
    }
    void setSpacing(qreal spacing);
    QList<DecorationButton*> &buttons() {
        return m_buttons;
    }
    const QList<DecorationButton*> &buttons() const {
        return m_buttons;
    }
    const QRectF &geometry() const {
        return m_geometry;
    }
    void setGeometry(const QRectF &geometry);
    void updateLayout();
private:
    DecorationButtonGroup *q;
    Decoration *m_decoration;
    QRectF m_geometry;
    QList<DecorationButton*> m_buttons;
    qreal m_spacing;
};

} // namespace

#endif
