/*
 * SPDX-FileCopyrightText: 2014 Martin Gräßlin <mgraesslin@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */
#include "mockbutton.h"

MockButton::MockButton(KDecoration3::DecorationButtonType type, KDecoration3::Decoration *decoration, QObject *parent)
    : DecorationButton(type, decoration, parent)
{
}

void MockButton::paint(QPainter *painter, const QRectF &repaintRegion)
{
    Q_UNUSED(painter)
    Q_UNUSED(repaintRegion)
}

#include "moc_mockbutton.cpp"
