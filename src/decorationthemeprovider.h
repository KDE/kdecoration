/*
 * SPDX-FileCopyrightText: 2021 Alexander Lohnau <alexander.lohnau@gmx.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KDECORATION2_DECORATION_THEME_PROVIDER
#define KDECORATION2_DECORATION_THEME_PROVIDER

#include "decorationdefines.h"
#include <QObject>
#include <kdecoration2/kdecoration2_export.h>
#include <memory>

class KPluginMetaData;
class DecorationThemeMetaDataPrivate;

namespace KDecoration2
{
/**
 * Class providing type-safe access to data of themes
 *
 * @since 5.23
 * @author Alexander Lohnau
 */
class KDECORATIONS2_EXPORT DecorationThemeMetaData
{
public:
    explicit DecorationThemeMetaData();
    virtual ~DecorationThemeMetaData();

    /// User-visible name of the theme
    QString visibleName() const;
    void setVisibleName(const QString &name);

    /// Internal name of the theme
    QString themeName() const;
    void setThemeName(const QString &name);

    /// Indicates that the theme has KCMs
    bool hasConfiguration() const;
    void setHasConfiguration(bool hasConfig);

    /// Border size of the decoration, this gets set based on the "recommendedBorderSize" key in the json metadata
    /// @internal
    KDecoration2::BorderSize borderSize() const;
    void setBorderSize(KDecoration2::BorderSize size);

    /// @internal
    QString pluginId() const;
    void setPluginId(const QString &id);

private:
    std::shared_ptr<DecorationThemeMetaDataPrivate> d;
};
/**
 * Class to give the KWin decorationmodel access to the plugin's themes.
 *
 * @since 5.23
 * @author Alexander Lohnau
 */
class KDECORATIONS2_EXPORT DecorationThemeProvider : public QObject
{
    Q_OBJECT

public:
    explicit DecorationThemeProvider(QObject *parent, const KPluginMetaData &data, const QVariantList &args);

    /**
     * List containing information of supported themes
     */
    virtual QList<DecorationThemeMetaData> themes() const = 0;
};
}
#endif
