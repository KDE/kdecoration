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
#ifndef KDECORATION2_DECORATION_SETTINGS_H
#define KDECORATION2_DECORATION_SETTINGS_H

#include <kdecoration2/kdecoration2_export.h>
#include "decorationbutton.h"

#include <QObject>

#include <memory>

namespace KDecoration2
{
class DecorationBridge;
class DecorationSettingsPrivate;

class KDECORATIONS2_EXPORT DecorationSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool onAllDesktopsAvailable READ isOnAllDesktopsAvailable NOTIFY onAllDesktopsAvailableChanged)
    Q_PROPERTY(bool alphaChannelSupported READ isAlphaChannelSupported NOTIFY alphaChannelSupportedChanged)
    Q_PROPERTY(bool closeOnDoubleClickOnMenu READ isCloseOnDoubleClickOnMenu NOTIFY closeOnDoubleClickOnMenuChanged)
    Q_PROPERTY(QList<DecorationButtonType> decorationButtonsLeft READ decorationButtonsLeft NOTIFY decorationButtonsLeftChanged)
    Q_PROPERTY(QList<DecorationButtonType> decorationButtonsRight READ decorationButtonsRight NOTIFY decorationButtonsRightChanged)
    Q_PROPERTY(BorderSize borderSize READ borderSize NOTIFY borderSizeChanged)
    /**
     * The fundamental unit of space that should be used for sizes, expressed in pixels.
     * Given the screen has an accurate DPI settings, it corresponds to a millimeter
     */
    Q_PROPERTY(int gridUnit READ gridUnit NOTIFY gridUnitChanged)
    /**
     * The recommended font for the Decoration's caption.
     **/
    Q_PROPERTY(QFont font READ font NOTIFY fontChanged)
    /**
     * smallSpacing is the amount of spacing that should be used around smaller UI elements,
     * for example as spacing in Columns. Internally, this size depends on the size of
     * the default font as rendered on the screen, so it takes user-configured font size and DPI
     * into account.
     */
    Q_PROPERTY(int smallSpacing READ smallSpacing NOTIFY spacingChanged)

    /**
     * largeSpacing is the amount of spacing that should be used inside bigger UI elements,
     * for example between an icon and the corresponding text. Internally, this size depends on
     * the size of the default font as rendered on the screen, so it takes user-configured font
     * size and DPI into account.
     */
    Q_PROPERTY(int largeSpacing READ largeSpacing NOTIFY spacingChanged)
public:
    explicit DecorationSettings(QObject *parent = nullptr);
    virtual ~DecorationSettings();
    bool isOnAllDesktopsAvailable() const;
    bool isAlphaChannelSupported() const;
    bool isCloseOnDoubleClickOnMenu() const;
    QList<DecorationButtonType> decorationButtonsLeft() const;
    QList<DecorationButtonType> decorationButtonsRight() const;
    BorderSize borderSize() const;

    QFont font() const;

    int gridUnit() const;
    int smallSpacing() const;
    int largeSpacing() const;

Q_SIGNALS:
    void onAllDesktopsAvailableChanged(bool);
    void alphaChannelSupportedChanged(bool);
    void closeOnDoubleClickOnMenuChanged(bool);
    void decorationButtonsLeftChanged(const QList<DecorationButtonType>&);
    void decorationButtonsRightChanged(const QList<DecorationButtonType>&);
    void borderSizeChanged(BorderSize size);
    void fontChanged(const QFont &font);
    void gridUnitChanged(int);
    void spacingChanged();

private:
    std::unique_ptr<DecorationSettingsPrivate> d;
};

}

Q_DECLARE_METATYPE(KDecoration2::BorderSize)

#endif
