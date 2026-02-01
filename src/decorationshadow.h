/*
 * SPDX-FileCopyrightText: 2014 Martin Gräßlin <mgraesslin@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */
#pragma once

#include <kdecoration2/kdecoration2_export.h>

#include <QImage>
#include <QMargins>
#include <QObject>

namespace KDecoration2
{
class DecorationShadowPrivate;

/*!
 * \class KDecoration2::DecorationShadow
 * \inheaderfile KDecoration3/DecorationShadow
 * \brief A wrapper to define the shadow around the Decoration.
 *
 * The shadow around the Decoration should not be rendered as part of the Decoration.
 * Instead a DecorationShadow should be used. That way a backend can optimize the
 * rendering of the shadow in a better way. If the shadow were part of the Decoration
 * directly it would need to be updated when the rendering changes. By using a dedicated
 * DecorationShadow the same shadow can be shared between multiple DecoratedClients.
 *
 * The DecorationShadow consists of a shadow QImage which is composed of multiple parts:
 * \list
 *   \li topLeft: rendered as it is
 *   \li top: stretched in x direction
 *   \li topRight: rendered as it is
 *   \li right: stretched in y direction
 *   \li bottomRight: rendered as it is
 *   \li bottom: stretched in x direction
 *   \li bottomLeft: rendered as it is
 *   \li left: stretched in y direction
 * \endlist
 *
 * The sizes of these parts is denoted in the property innerShadowRect and the layout is the
 * following:
 *
 * \badcode
 * #######################################
 * # topLeft #     top        # topRight #
 * #######################################
 * # left #                      # right #
 * #######################################
 * # bottomLeft #  bottom  # bottomRight #
 * #######################################
 * \endcode
 *
 * The innerShadowRect property is a QRect of the geometry of the areas not covered by any of the
 * elements. This means that:
 * \list
 *   \li x/y of the rect is the same as the size of the topLeft element
 *   \li width of the rect is the same as the width of the top and bottom element
 *   \li height of the rect is the same as the height of the left and the right element
 * \endlist
 * By that the actual sizes of all elements can be derived out of the size of the shadow image
 * and the innerShadowRect.
 *
 * The position of the rendering depends on the values;
 * \list
 *   \li paddingTop
 *   \li paddingRight
 *   \li paddingBottom
 *   \li paddingLeft
 * \endlist
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
    /*!
     * \property KDecoration2::DecorationShadow::shadow
     */
    Q_PROPERTY(QImage shadow READ shadow WRITE setShadow NOTIFY shadowChanged)
    /*!
     * \property KDecoration2::DecorationShadow::innerShadowRect
     */
    Q_PROPERTY(QRect innerShadowRect READ innerShadowRect WRITE setInnerShadowRect NOTIFY innerShadowRectChanged)
    /*!
     * \property KDecoration2::DecorationShadow::topLeftGeometry
     */
    Q_PROPERTY(QRect topLeftGeometry READ topLeftGeometry NOTIFY innerShadowRectChanged)
    /*!
     * \property KDecoration2::DecorationShadow::topGeometry
     */
    Q_PROPERTY(QRect topGeometry READ topGeometry NOTIFY innerShadowRectChanged)
    /*!
     * \property KDecoration2::DecorationShadow::topRightGeometry
     */
    Q_PROPERTY(QRect topRightGeometry READ topRightGeometry NOTIFY innerShadowRectChanged)
    /*!
     * \property KDecoration2::DecorationShadow::rightGeometry
     */
    Q_PROPERTY(QRect rightGeometry READ rightGeometry NOTIFY innerShadowRectChanged)
    /*!
     * \property KDecoration2::DecorationShadow::bottomRightGeometry
     */
    Q_PROPERTY(QRect bottomRightGeometry READ bottomRightGeometry NOTIFY innerShadowRectChanged)
    /*!
     * \property KDecoration2::DecorationShadow::bottomGeometry
     */
    Q_PROPERTY(QRect bottomGeometry READ bottomGeometry NOTIFY innerShadowRectChanged)
    /*!
     * \property KDecoration2::DecorationShadow::bottomLeftGeometry
     */
    Q_PROPERTY(QRect bottomLeftGeometry READ bottomLeftGeometry NOTIFY innerShadowRectChanged)
    /*!
     * \property KDecoration2::DecorationShadow::leftGeometry
     */
    Q_PROPERTY(QRect leftGeometry READ leftGeometry NOTIFY innerShadowRectChanged)
    /*!
     * \property KDecoration2::DecorationShadow::paddingTop
     */
    Q_PROPERTY(int paddingTop READ paddingTop NOTIFY paddingChanged)
    /*!
     * \property KDecoration2::DecorationShadow::paddingRight
     */
    Q_PROPERTY(int paddingRight READ paddingRight NOTIFY paddingChanged)
    /*!
     * \property KDecoration2::DecorationShadow::paddingBottom
     */
    Q_PROPERTY(int paddingBottom READ paddingBottom NOTIFY paddingChanged)
    /*!
     * \property KDecoration2::DecorationShadow::paddingLeft
     */
    Q_PROPERTY(int paddingLeft READ paddingLeft NOTIFY paddingChanged)
    /*!
     * \property KDecoration2::DecorationShadow::padding
     */
    Q_PROPERTY(QMargins padding READ padding WRITE setPadding NOTIFY paddingChanged)
public:
    explicit DecorationShadow();
    ~DecorationShadow() override;

    /*!
     */
    QImage shadow() const;
    /*!
     */
    QRect innerShadowRect() const;
    /*!
     */
    QRect topLeftGeometry() const;
    /*!
     */
    QRect topGeometry() const;
    /*!
     */
    QRect topRightGeometry() const;
    /*!
     */
    QRect rightGeometry() const;
    /*!
     */
    QRect bottomRightGeometry() const;
    /*!
     */
    QRect bottomGeometry() const;
    /*!
     */
    QRect bottomLeftGeometry() const;
    /*!
     */
    QRect leftGeometry() const;
    /*!
     */
    int paddingTop() const;
    /*!
     */
    int paddingRight() const;
    /*!
     */
    int paddingBottom() const;
    /*!
     */
    int paddingLeft() const;
    /*!
     */
    QMargins padding() const;

    /*!
     */
    void setShadow(const QImage &image);
    /*!
     */
    void setInnerShadowRect(const QRect &rect);
    /*!
     */
    void setPadding(const QMargins &margins);

Q_SIGNALS:
    void shadowChanged(const QImage &);
    void innerShadowRectChanged();
    void paddingChanged();

private:
    class Private;
    std::unique_ptr<Private> d;
};

}

Q_DECLARE_METATYPE(KDecoration2::DecorationShadow *)
