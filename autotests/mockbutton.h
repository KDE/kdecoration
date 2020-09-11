/*
 * SPDX-FileCopyrightText: 2014 Martin Gräßlin <mgraesslin@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */
#ifndef MOCK_BUTTON_H
#define MOCK_BUTTON_H

#include "../src/decorationbutton.h"

class MockButton : public KDecoration2::DecorationButton
{
    Q_OBJECT
public:
    MockButton(KDecoration2::DecorationButtonType type, const QPointer<KDecoration2::Decoration> &decoration, QObject *parent = nullptr);
    void paint(QPainter *painter, const QRect &repaintRegion) override;
};

#endif
