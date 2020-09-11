/*
 * SPDX-FileCopyrightText: 2014 Martin Gräßlin <mgraesslin@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */
#ifndef MOCK_DECORATION_H
#define MOCK_DECORATION_H

#include "../src/decoration.h"

class MockBridge;

class MockDecoration : public KDecoration2::Decoration
{
    Q_OBJECT
public:
    explicit MockDecoration(QObject *parent, const QVariantList &args);
    explicit MockDecoration(MockBridge *bridge);
    void paint(QPainter *painter, const QRect &repaintRegion) override;
    void setOpaque(bool set);
    using Decoration::setBorders;
    void setBorders(const QMargins &m);
    using Decoration::setTitleBar;
    void setTitleBar(const QRect &rect);
};

#endif
