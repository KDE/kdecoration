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
#ifndef KDECORATION2_DECORATIONBUTTON_H
#define KDECORATION2_DECORATIONBUTTON_H

#include <kdecoration2/kdecoration2_export.h>
#include "decorationdefines.h"

#include <QObject>
#include <QPointer>
#include <QRect>

class QHoverEvent;
class QMouseEvent;
class QPainter;

namespace KDecoration2
{

class DecorationButtonPrivate;
class Decoration;
uint KDECORATIONS2_EXPORT qHash(const DecorationButtonType &type);

/**
 * @brief A button to be used in a Decoration.
 *
 * The DecorationButton is a simple Button which can be used (but doesn't have to) in a Decoration.
 * It takes care of the input handling and triggers the correct state change methods on the
 * Decoration.
 *
 * This simplifies the handling of DecorationButtons. A Decoration implementation just needs to
 * subclass DecorationButton and implement the paint method. Everything else is handled by the
 * DecorationButton.
 *
 * For positioning the DecorationButtons it's recommended to use a DecorationButtonGroup.
 *
 * @see Decoration
 * @see DecorationButtonGroup
 **/
class KDECORATIONS2_EXPORT DecorationButton : public QObject
{
    Q_OBJECT
    /**
     * Whether the DecorationButton is visible. By default this is @c true, OnAllDesktops and
     * QuickHelp depend on the DecoratedClient's state.
     **/
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibilityChanged)
    /**
     * Whether the DecorationButton is currently pressed.
     **/
    Q_PROPERTY(bool pressed READ isPressed NOTIFY pressedChanged)
    /**
     * Whether the DecorationButton is currently hovered.
     **/
    Q_PROPERTY(bool hovered READ isHovered NOTIFY hoveredChanged)
    /**
     * Whether the DecorationButton is enabled. Only an enabled button accepts hover and mouse
     * press events.
     **/
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    /**
     * Whether the DecorationButton can be checked. This is used for state aware DecorationButtons
     * like Maximize, Shade, KeepAbove, KeepBelow and OnAllDesktops.
     **/
    Q_PROPERTY(bool checkable READ isCheckable WRITE setCheckable NOTIFY checkableChanged)
    /**
     * Whether the DecorationButton is checked. A DecorationButton can only be checked if the
     * DecorationButton is checkable. Note: the checked state is not changed by clicking the
     * DecorationButton. It gets changed if the DecoratedClient changes it's state, though.
     **/
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked NOTIFY checkedChanged)
    /**
     * The geometry of the DecorationButton in Decoration-local coordinates.
     **/
    Q_PROPERTY(QRect geometry READ geometry NOTIFY geometryChanged)
    /**
     * The mouse buttons the DecorationButton accepts. By default the Qt::LeftButton gets accepted,
     * for some types more buttons are accepted.
     **/
    Q_PROPERTY(Qt::MouseButtons acceptedButtons READ acceptedButtons WRITE setAcceptedButtons NOTIFY acceptedButtonsChanged)
public:
    virtual ~DecorationButton();

    QRect geometry() const;
    QSize size() const;
    void setGeometry(const QRect &geometry);

    bool isVisible() const;
    bool isPressed() const;
    bool isHovered() const;
    bool isEnabled() const;
    bool isChecked() const;
    bool isCheckable() const;
    DecorationButtonType type() const;

    Qt::MouseButtons acceptedButtons() const;
    void setAcceptedButtons(Qt::MouseButtons buttons);

    /**
     * Invoked for painting this DecorationButtons. Implementing sub-classes need to implement
     * this method. The coordinate system of the QPainter is set to Decoration coordinates.
     *
     * This method will be invoked from the rendering thread.
     *
     * @param painter The QPainter to paint this DecorationButton.
     **/
    virtual void paint(QPainter *painter) = 0;

    QPointer<Decoration> decoration() const;

    virtual bool event(QEvent *event) override;

public Q_SLOTS:
    void setEnabled(bool enabled);
    void setCheckable(bool checkable);
    void setChecked(bool checked);
    void setVisible(bool visible);

    /**
     * Schedules a repaint of the DecorationButton.
     * Calling update will eventually result in paint being invoked.
     *
     * @param rect The area to repaint in Decoration local coordinates, a null QRect updates the complete geometry
     * @see paint
     **/
    void update(const QRect &rect = QRect());

Q_SIGNALS:
    void clicked(Qt::MouseButton);
    void pressed();
    void released();
    void pointerEntered();
    void pointerLeft();
    void doubleClicked();

    void pressedChanged(bool);
    void hoveredChanged(bool);
    void enabledChanged(bool);
    void checkableChanged(bool);
    void checkedChanged(bool);
    void geometryChanged(const QRect&);
    void acceptedButtonsChanged(Qt::MouseButtons);
    void visibilityChanged(bool);

protected:
    explicit DecorationButton(DecorationButtonType type, QPointer<Decoration> decoration, QObject *parent = nullptr);

    virtual void hoverEnterEvent(QHoverEvent *event);
    virtual void hoverLeaveEvent(QHoverEvent *event);
    virtual void hoverMoveEvent(QHoverEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    class Private;
    QScopedPointer<Private> d;
};

} // namespace
Q_DECLARE_METATYPE(KDecoration2::DecorationButtonType)

#endif
