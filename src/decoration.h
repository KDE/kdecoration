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

#include <QObject>
#include <QRect>

class QHoverEvent;
class QMouseEvent;
class QPainter;

namespace KDecoration2
{

class DecorationPrivate;
class DecoratedClient;
class DecorationButton;

class KDECORATIONS2_EXPORT Decoration : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int borderLeft    READ borderLeft    NOTIFY bordersChanged)
    Q_PROPERTY(int borderRight   READ borderRight   NOTIFY bordersChanged)
    Q_PROPERTY(int borderTop     READ borderTop     NOTIFY bordersChanged)
    Q_PROPERTY(int borderBottom  READ borderBottom  NOTIFY bordersChanged)
    Q_PROPERTY(Qt::WindowFrameSection windowFrameSection READ windowFrameSection NOTIFY windowFrameSectionChanged)
    Q_PROPERTY(QRect titleRect READ titleRect NOTIFY titleRectChanged)
    /**
     * Rendering whether the Decoration is fully opaque. By default a Decoration is considered to
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

    DecoratedClient *client();
    const DecoratedClient *client() const;

    int borderLeft() const;
    int borderRight() const;
    int borderTop() const;
    int borderBottom() const;
    Qt::WindowFrameSection windowFrameSection() const;
    QRect titleRect() const;
    bool isOpaque() const;

    DecorationShadow *shadow() const;

    /**
     * The decoration's geometry in local coordinates.
     *
     * Basically the size of the DecoratedClient combined with the borders.
     **/
    QRect rect() const;
    QSize size() const;

    virtual void paint(QPainter *painter) = 0;

    /**
     * internal
     **/
    void addButton(DecorationButton *button);

    virtual bool event(QEvent *event) override;

public Q_SLOTS:
    void requestClose();
    void requestMaximize(Qt::MouseButtons buttons);
    void requestMinimize();
    void requestContextHelp();
    void requestToggleOnAllDesktops();
    void requestToggleShade();
    void requestToggleKeepAbove();
    void requestToggleKeepBelow();
    void requestShowWindowMenu();

    void update(const QRect &rect = QRect());

Q_SIGNALS:
    void bordersChanged();
    void windowFrameSectionChanged(Qt::WindowFrameSection);
    void titleRectChanged(const QRect &);
    void opaqueChanged(bool);
    void shadowChanged(DecorationShadow *shadow);

protected:
    explicit Decoration(QObject *parent = nullptr);
    void setBorders(int left, int right, int top, int bottom);
    void setTitleRect(const QRect &rect);
    void setOpaque(bool opaque);
    void setShadow(DecorationShadow *shadow);
    DecorationShadow *shadow();

    virtual void hoverEnterEvent(QHoverEvent *event);
    virtual void hoverLeaveEvent(QHoverEvent *event);
    virtual void hoverMoveEvent(QHoverEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    QScopedPointer<DecorationPrivate> d;
};

} // namespace

Q_DECLARE_METATYPE(KDecoration2::Decoration*)

#endif
