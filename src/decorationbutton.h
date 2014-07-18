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

#include <QObject>
#include <QRect>

class QHoverEvent;
class QMouseEvent;
class QPainter;

namespace KDecoration2
{

class DecorationButtonPrivate;
class Decoration;

/**
 * The DecorationButtonType is a helper type for the DecorationButton.
 * A Decoration should provide a DecorationButton for each of the types,
 * if it wants to provide further buttons it should use the Custom type.
 * The DecorationButton gets configured depending on the type. E.g. the
 * Close button gets disabled if the DecoratedClient is not closeable.
 **/
enum class DecorationButtonType : uint
{
    /**
     * The Menu button requests showing the window menu on left or right click.
     **/
    Menu,
    /**
     * The ApplicationMenu button requests showing the application's menu on left or right click.
     */
    ApplicationMenu,
    /**
     * The OnAllDesktops button requests toggling the DecoratedClient's on all desktops state.
     * The DecoratedButton is only visible if multiple virtual desktops are available.
     **/
    OnAllDesktops,
    /**
     * The Minimize button requests minimizing the DecoratedClient. The DecorationButton is only
     * enabled if the DecoratedClient is minimizeable.
     **/
    Minimize,
    /**
     * The Maximize button requests maximizing the DecoratedClient. The DecorationButton is checkable
     * and if the DecoratedClient is maximized the DecorationButton is checked. The DecorationButton
     * supports multiple mouse buttons to change horizontal, vertical and overall maximized state.
     *
     * The DecorationButton is only enabled if the DecoratedClient is maximizeable.
     **/
    Maximize,
    /**
     * The Close button requests closing the DecoratedClient. The DecorationButton is only enabled
     * if the DecoratedClient is closeable.
     **/
    Close,
    /**
     * The QuickHelp button requests entering the context help mode. The DecorationButton is only
     * visible if the DecoratedClient provides context help.
     **/
    QuickHelp,
    /**
     * The Shade button requests toggling the DecoratedClient's shaded state. The DecoratedButton
     * is only enabled if the DecoratedClient is shadeable.
     **/
    Shade,
    /**
     * The KeepBelow button requests toggling the DecoratedClient's keep below state.
     **/
    KeepBelow,
    /**
     * The KeepAbove button requests toggling the DecoratedClient's keep above state.
     **/
    KeepAbove,
    /**
     * The Custom type allows a Decoration to provide custom DecorationButtons.
     **/
    Custom
};
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

    Decoration *decoration();
    const Decoration *decoration() const;

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
    void pointerLeaved();

    void pressedChanged(bool);
    void hoveredChanged(bool);
    void enabledChanged(bool);
    void checkableChanged(bool);
    void checkedChanged(bool);
    void geometryChanged(const QRect&);
    void acceptedButtonsChanged(Qt::MouseButtons);
    void visibilityChanged(bool);

protected:
    explicit DecorationButton(DecorationButtonType type, Decoration *decoration, QObject *parent = nullptr);

    virtual void hoverEnterEvent(QHoverEvent *event);
    virtual void hoverLeaveEvent(QHoverEvent *event);
    virtual void hoverMoveEvent(QHoverEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    QScopedPointer<DecorationButtonPrivate> d;
};

} // namespace

#endif
