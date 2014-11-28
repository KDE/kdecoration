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
#ifndef KDECORATION2_DECORATION_H
#define KDECORATION2_DECORATION_H

#include <kdecoration2/kdecoration2_export.h>
#include "decorationshadow.h"

#include <QMargins>
#include <QObject>
#include <QPointer>
#include <QRect>

class QHoverEvent;
class QMouseEvent;
class QPainter;
class QWheelEvent;

namespace KDecoration2
{

class DecorationPrivate;
class DecoratedClient;
class DecorationButton;
class DecorationSettings;

class KDECORATIONS2_EXPORT Decoration : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QMargins borders READ borders NOTIFY bordersChanged)
    Q_PROPERTY(int borderLeft    READ borderLeft    NOTIFY bordersChanged)
    Q_PROPERTY(int borderRight   READ borderRight   NOTIFY bordersChanged)
    Q_PROPERTY(int borderTop     READ borderTop     NOTIFY bordersChanged)
    Q_PROPERTY(int borderBottom  READ borderBottom  NOTIFY bordersChanged)
    Q_PROPERTY(QMargins resizeOnlyBorders READ resizeOnlyBorders NOTIFY resizeOnlyBordersChanged)
    Q_PROPERTY(int resizeOnlyBorderLeft    READ resizeOnlyBorderLeft    NOTIFY resizeOnlyBordersChanged)
    Q_PROPERTY(int resizeOnlyBorderRight   READ resizeOnlyBorderRight   NOTIFY resizeOnlyBordersChanged)
    Q_PROPERTY(int resizeOnlyBorderTop     READ resizeOnlyBorderTop     NOTIFY resizeOnlyBordersChanged)
    Q_PROPERTY(int resizeOnlyBorderBottom  READ resizeOnlyBorderBottom  NOTIFY resizeOnlyBordersChanged)
    /**
     * This property denotes the part of the Decoration which is currently under the mouse pointer.
     * It gets automatically updated whenever a QMouseEvent or QHoverEvent gets processed.
     **/
    Q_PROPERTY(Qt::WindowFrameSection sectionUnderMouse READ sectionUnderMouse NOTIFY sectionUnderMouseChanged)
    /**
     * The titleBar is the area inside the Decoration containing all controls (e.g. Buttons)
     * and the caption. The titleBar is the main interaction area, while all other areas of the
     * Decoration are normally used as resize areas.
     **/
    Q_PROPERTY(QRect titleBar READ titleBar NOTIFY titleBarChanged)
    /**
     * Whether the Decoration is fully opaque. By default a Decoration is considered to
     * use the alpha channel and this property has the value @c false. But for e.g. a maximized
     * DecoratedClient it is possible that the Decoration is fully opaque. In this case the
     * Decoration should set this property to @c true.
     **/
    Q_PROPERTY(bool opaque READ isOpaque NOTIFY opaqueChanged)
    /**
     * DecorationShadow for this Decoration. It is recommended that multiple Decorations share
     * the same DecorationShadow. E.g one DecorationShadow for all inactive Decorations and one
     * for the active Decoration.
     **/
    Q_PROPERTY(KDecoration2::DecorationShadow *shadow READ shadow NOTIFY shadowChanged)
public:
    virtual ~Decoration();

    /**
     * The DecoratedClient for this Decoration.
     * As long as the Decoration is alive it is guaranteed that the object is not
     * deleted. Thus it is save to access using QWeakPointer::data in e.g. a sublcass
     * of Decoration without promoting to QSharedPointer.
     **/
    QWeakPointer<DecoratedClient> client() const;

    QMargins borders() const;
    int borderLeft() const;
    int borderRight() const;
    int borderTop() const;
    int borderBottom() const;
    QMargins resizeOnlyBorders() const;
    int resizeOnlyBorderLeft() const;
    int resizeOnlyBorderRight() const;
    int resizeOnlyBorderTop() const;
    int resizeOnlyBorderBottom() const;
    Qt::WindowFrameSection sectionUnderMouse() const;
    QRect titleBar() const;
    bool isOpaque() const;

    QPointer<DecorationShadow> shadow() const;

    /**
     * The decoration's geometry in local coordinates.
     *
     * Basically the size of the DecoratedClient combined with the borders.
     **/
    QRect rect() const;
    QSize size() const;

    /**
     * Invoked by the framework to set the Settings for this Decoration before
     * init is invoked.
     * @internal
     **/
    void setSettings(const QSharedPointer<DecorationSettings> &settings);
    /**
     * @returns The DecorationSettings used for this Decoration.
     **/
    QSharedPointer<DecorationSettings> settings() const;

    virtual void paint(QPainter *painter, const QRect &repaintArea) = 0;

    virtual bool event(QEvent *event) override;

public Q_SLOTS:
    void requestClose();
    void requestToggleMaximization(Qt::MouseButtons buttons);
    void requestMinimize();
    void requestContextHelp();
    void requestToggleOnAllDesktops();
    void requestToggleShade();
    void requestToggleKeepAbove();
    void requestToggleKeepBelow();
    void requestShowWindowMenu();

    void update(const QRect &rect);
    void update();

    /**
     * This method gets invoked from the framework once the Decoration is created and
     * completely setup.
     *
     * An inheriting class should override this method and perform all initialization in
     * this method instead of the constructor.
     **/
    virtual void init();

Q_SIGNALS:
    void bordersChanged();
    void resizeOnlyBordersChanged();
    void sectionUnderMouseChanged(Qt::WindowFrameSection);
    void titleBarChanged();
    void opaqueChanged(bool);
    void shadowChanged(QPointer<DecorationShadow> shadow);

protected:
    explicit Decoration(QObject *parent, const QVariantList &args);
    void setBorders(const QMargins &borders);
    void setResizeOnlyBorders(const QMargins &borders);
    /**
     * An implementation has to invoke this method whenever the area
     * containing the controls and caption changes.
     * @param rect The new geometry of the titleBar in Decoration coordinates
     **/
    void setTitleBar(const QRect &rect);
    void setOpaque(bool opaque);
    void setShadow(const QPointer<DecorationShadow> &shadow);

    virtual void hoverEnterEvent(QHoverEvent *event);
    virtual void hoverLeaveEvent(QHoverEvent *event);
    virtual void hoverMoveEvent(QHoverEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);

private:
    friend class DecorationButton;
    class Private;
    QScopedPointer<Private> d;
};

} // namespace

Q_DECLARE_METATYPE(KDecoration2::Decoration*)

#endif
