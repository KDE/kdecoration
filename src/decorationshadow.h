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
#ifndef KDECORATION2_DECORATION_SHADOW_H
#define KDECORATION2_DECORATION_SHADOW_H

#include <kdecoration2/kdecoration2_export.h>

#include <QObject>
#include <QImage>

namespace KDecoration2
{

class DecorationShadowPrivate;

/**
 * @brief A wrapper to define the shadow around the Decoration.
 *
 * The shadow around the Decoration should not be rendered as part of the Decoration.
 * Instead a DecorationShadow should be used. That way a backend can optimize the
 * rendering of the shadow in a better way. If the shadow were part of the Decoration
 * directly it would need to be updated when the rendering changes. By using a dedicated
 * DecorationShadow the same shadow can be shared between multiple DecoratedClients.
 *
 * The DecorationShadow consists of a shadow QImage which is composed of multiple parts:
 * @li topLeft: rendered as it is
 * @li top: stretched in x direction
 * @li topRight: rendered as it is
 * @li right: stretched in y direction
 * @li bottomRight: rendered as it is
 * @li bottom: stretched in x direction
 * @li bottomLeft: rendered as it is
 * @li left: stretched in y direction
 *
 * The sizes of these parts is denoted in the properties of same name and the layout is the
 * following:
 * #######################################
 * # topLeft #     top        # topRight #
 * #######################################
 * # left #                      # right #
 * #######################################
 * # bottomLeft #  bottom  # bottomRight #
 * #######################################
 *
 * The position of the rendering depends on the values;
 * @li paddingTop
 * @li paddingRight
 * @li paddingBottom
 * @li paddingLeft
 *
 * The top left element is rendered with an offset of paddingLeft and paddingTop.
 * The non-stretched elements are rendered in the size as specified, the area
 * between two non-stretched elements (e.g. between topLeft and topRight) is filled
 * by the element with one direction stretched and the other direction fixed at the
 * corresponding padding value. E.g. the top element is stretched in x direction and
 * fixed at paddingTop value. If stretching the side elements is not wanted one needs
 * to provide a shadow image with those elements at a size that stretching is not
 * required.
 *
 * If the padding values are smaller than the sizes of the shadow elements the shadow
 * will overlap with the Decoration and be rendered behind the Decoration.
 *
 **/
class KDECORATIONS2_EXPORT DecorationShadow : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QImage shadow     READ shadow        WRITE setShadow        NOTIFY shadowChanged)
    Q_PROPERTY(QSize topLeft     READ topLeft       WRITE setTopLeft       NOTIFY topLeftChanged)
    Q_PROPERTY(QSize top         READ top           WRITE setTop           NOTIFY topChanged)
    Q_PROPERTY(QSize topRight    READ topRight      WRITE setTopRight      NOTIFY topRightChanged)
    Q_PROPERTY(QSize right       READ right         WRITE setRight         NOTIFY rightChanged)
    Q_PROPERTY(QSize bottomRight READ bottomRight   WRITE setBottomRight   NOTIFY bottomRightChanged)
    Q_PROPERTY(QSize bottom      READ bottom        WRITE setBottom        NOTIFY bottomChanged)
    Q_PROPERTY(QSize bottomLeft  READ bottomLeft    WRITE setBottomLeft    NOTIFY bottomLeftChanged)
    Q_PROPERTY(QSize left        READ left          WRITE setLeft          NOTIFY leftChanged)
    Q_PROPERTY(int paddingTop    READ paddingTop    WRITE setPaddingTop    NOTIFY paddingTopChanged)
    Q_PROPERTY(int paddingRight  READ paddingRight  WRITE setPaddingRight  NOTIFY paddingRightChanged)
    Q_PROPERTY(int paddingBottom READ paddingBottom WRITE setPaddingBottom NOTIFY paddingBottomChanged)
    Q_PROPERTY(int paddingLeft   READ paddingLeft   WRITE setPaddingLeft   NOTIFY paddingLeftChanged)
public:
    explicit DecorationShadow(QObject *parent = nullptr);
    virtual ~DecorationShadow();

    QImage shadow() const;
    QSize topLeft() const;
    QSize top() const;
    QSize topRight() const;
    QSize right() const;
    QSize bottomRight() const;
    QSize bottom() const;
    QSize bottomLeft() const;
    QSize left() const;
    int paddingTop() const;
    int paddingRight() const;
    int paddingBottom() const;
    int paddingLeft() const;

    void setShadow(const QImage &image);
    void setTopLeft(const QSize &size);
    void setTop(const QSize &size);
    void setTopRight(const QSize &size);
    void setRight(const QSize &size);
    void setBottomRight(const QSize &size);
    void setBottom(const QSize &size);
    void setBottomLeft(const QSize &size);
    void setLeft(const QSize &size);
    void setPaddingTop(int padding);
    void setPaddingRight(int padding);
    void setPaddingBottom(int padding);
    void setPaddingLeft(int padding);

Q_SIGNALS:
    void shadowChanged(const QImage&);
    void topLeftChanged(const QSize&);
    void topChanged(const QSize&);
    void topRightChanged(const QSize&);
    void rightChanged(const QSize&);
    void bottomRightChanged(const QSize&);
    void bottomChanged(const QSize&);
    void bottomLeftChanged(const QSize&);
    void leftChanged(const QSize&);
    void paddingTopChanged(int);
    void paddingRightChanged(int);
    void paddingBottomChanged(int);
    void paddingLeftChanged(int);

private:
    class Private;
    QScopedPointer<Private> d;
};

}

Q_DECLARE_METATYPE(KDecoration2::DecorationShadow*)

#endif
