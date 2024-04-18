/*
 * SPDX-FileCopyrightText: 2024 Vlad Zahorodnii <vlad.zahorodnii@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "decorationmetrics.h"

namespace KDecoration2
{

DecorationMetrics::DecorationMetrics()
{
}

int DecorationMetrics::map(int value) const
{
    return value * m_pixelUnit;
}

float DecorationMetrics::map(float value) const
{
    return value * m_pixelUnit;
}

double DecorationMetrics::map(double value) const
{
    return value * m_pixelUnit;
}

QPoint DecorationMetrics::map(const QPoint &point) const
{
    return QPoint(map(point.x()), map(point.y()));
}

QPointF DecorationMetrics::map(const QPointF &point) const
{
    return QPointF(map(point.x()), map(point.y()));
}

QSize DecorationMetrics::map(const QSize &size) const
{
    return QSize(map(size.width()), map(size.height()));
}

QSizeF DecorationMetrics::map(const QSizeF &size) const
{
    return QSizeF(map(size.width()), map(size.height()));
}

QRect DecorationMetrics::map(const QRect &rect) const
{
    return QRect(map(rect.x()), map(rect.y()), map(rect.width()), map(rect.height()));
}

QRectF DecorationMetrics::map(const QRectF &rect) const
{
    return QRectF(map(rect.x()), map(rect.y()), map(rect.width()), map(rect.height()));
}

int DecorationMetrics::pixelUnit() const
{
    return m_pixelUnit;
}

void DecorationMetrics::setPixelUnit(int unit)
{
    m_pixelUnit = unit;
}

int DecorationMetrics::gridUnit() const
{
    return m_gridUnit;
}

void DecorationMetrics::setGridUnit(int unit)
{
    m_gridUnit = unit;
}

int DecorationMetrics::smallSpacing() const
{
    return m_smallSpacing;
}

void DecorationMetrics::setSmallSpacing(int spacing)
{
    m_smallSpacing = spacing;
}

int DecorationMetrics::largeSpacing() const
{
    return m_largeSpacing;
}

void DecorationMetrics::setLargeSpacing(int spacing)
{
    m_largeSpacing = spacing;
}

DecorationMetrics DecorationMetrics::from(const QFontMetrics &fontMetrics, int pixelUnit)
{
    int gridUnit = fontMetrics.boundingRect(QLatin1Char('M')).height();
    if (gridUnit % 2 != 0) {
        gridUnit++;
    }

    DecorationMetrics decorationMetrics;
    decorationMetrics.setPixelUnit(pixelUnit);
    decorationMetrics.setGridUnit(gridUnit * pixelUnit);
    decorationMetrics.setSmallSpacing(std::max(2, gridUnit / 4) * pixelUnit);
    decorationMetrics.setLargeSpacing(gridUnit * pixelUnit);

    return decorationMetrics;
}

} // namespace KDecoration2
