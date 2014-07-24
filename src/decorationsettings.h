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


namespace KDecoration2
{
class DecorationBridge;
class DecorationSettingsPrivate;

class KDECORATIONS2_EXPORT DecorationSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool onAllDesktopsAvailable READ isOnAllDesktopsAvailable NOTIFY onAllDesktopsAvailableChanged)
    Q_PROPERTY(bool alphaChannelSupported READ isAlphaChannelSupported NOTIFY alphaChannelSupportedChanged)
    Q_PROPERTY(QList<DecorationButtonType> decorationButtonsLeft READ decorationButtonsLeft NOTIFY decorationButtonsLeftChanged)
    Q_PROPERTY(QList<DecorationButtonType> decorationButtonsRight READ decorationButtonsRight NOTIFY decorationButtonsRightChanged)
    Q_PROPERTY(BorderSize borderSize READ borderSize NOTIFY borderSizeChanged)
    /**
     * The recommended font for the Decoration's caption.
     **/
    Q_PROPERTY(QFont font READ font NOTIFY fontChanged)
public:
    virtual ~DecorationSettings();
    bool isOnAllDesktopsAvailable() const;
    bool isAlphaChannelSupported() const;
    QList<DecorationButtonType> decorationButtonsLeft() const;
    QList<DecorationButtonType> decorationButtonsRight() const;
    BorderSize borderSize() const;

    QFont font() const;

    /**
     * @param parent Used as parent if static instance is not yet created, a Decoration plugin
     * doesn't have to pass the parent argument. The backend ensures that the DecorationSettings
     * is created before the Decoration is created.
     **/
    static DecorationSettings *self(QObject *parent = nullptr);

Q_SIGNALS:
    void onAllDesktopsAvailableChanged(bool);
    void alphaChannelSupportedChanged(bool);
    void decorationButtonsLeftChanged(const QList<DecorationButtonType>&);
    void decorationButtonsRightChanged(const QList<DecorationButtonType>&);
    void borderSizeChanged(BorderSize size);
    void fontChanged(const QFont &font);

private:
    explicit DecorationSettings(QObject *parent);
    static DecorationSettings *s_self;
    QScopedPointer<DecorationSettingsPrivate> d;
};

}

Q_DECLARE_METATYPE(KDecoration2::BorderSize)

#endif
