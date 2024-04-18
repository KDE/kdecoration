/*
 * SPDX-FileCopyrightText: 2024 Vlad Zahorodnii <vlad.zahorodnii@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */
#pragma once

#include <kdecoration2/kdecoration2_export.h>

#include <QFontMetrics>

namespace KDecoration2
{

class KDECORATIONS2_EXPORT DecorationMetrics
{
public:
    DecorationMetrics();

    int map(int value) const;
    float map(float value) const;
    double map(double value) const;

    QPoint map(const QPoint &point) const;
    QPointF map(const QPointF &point) const;

    QSize map(const QSize &size) const;
    QSizeF map(const QSizeF &size) const;

    QRect map(const QRect &rect) const;
    QRectF map(const QRectF &rect) const;

    int pixelUnit() const;
    void setPixelUnit(int unit);

    int gridUnit() const;
    void setGridUnit(int unit);

    int smallSpacing() const;
    void setSmallSpacing(int spacing);

    int largeSpacing() const;
    void setLargeSpacing(int spacing);

    static DecorationMetrics from(const QFontMetrics &fontMetrics, int pixelUnit = 1);

private:
    int m_pixelUnit = -1;
    int m_gridUnit = -1;
    int m_smallSpacing = -1;
    int m_largeSpacing = -1;
};

} // namespace KDecoration2
