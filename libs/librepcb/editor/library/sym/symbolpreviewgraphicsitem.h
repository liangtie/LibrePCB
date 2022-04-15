/*
 * LibrePCB - Professional EDA for everyone!
 * Copyright (C) 2013 LibrePCB Developers, see AUTHORS.md for contributors.
 * https://librepcb.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBREPCB_EDITOR_SYMBOLPREVIEWGRAPHICSITEM_H
#define LIBREPCB_EDITOR_SYMBOLPREVIEWGRAPHICSITEM_H

/*******************************************************************************
 *  Includes
 ******************************************************************************/
#include <librepcb/core/attribute/attributeprovider.h>
#include <librepcb/core/types/uuid.h>

#include <QtCore>
#include <QtWidgets>

/*******************************************************************************
 *  Namespace / Forward Declarations
 ******************************************************************************/
namespace librepcb {

class Component;
class ComponentSymbolVariantItem;
class GraphicsLayer;
class IF_GraphicsLayerProvider;
class Symbol;
class Text;

namespace editor {

/*******************************************************************************
 *  Class SymbolPreviewGraphicsItem
 ******************************************************************************/

/**
 * @brief The SymbolPreviewGraphicsItem class
 */
class SymbolPreviewGraphicsItem final : public QGraphicsItem,
                                        public AttributeProvider {
public:
  // Constructors / Destructor
  SymbolPreviewGraphicsItem() = delete;
  SymbolPreviewGraphicsItem(const SymbolPreviewGraphicsItem& other) = delete;
  explicit SymbolPreviewGraphicsItem(
      const IF_GraphicsLayerProvider& layerProvider,
      const QStringList& localeOrder, const Symbol& symbol,
      const Component* cmp = nullptr,
      const tl::optional<Uuid>& symbVarUuid = tl::nullopt,
      const tl::optional<Uuid>& symbVarItemUuid = tl::nullopt) noexcept;
  ~SymbolPreviewGraphicsItem() noexcept;

  // Setters
  void setDrawBoundingRect(bool enable) noexcept;

  // General Methods
  void updateCacheAndRepaint() noexcept;

  // Inherited from QGraphicsItem
  QRectF boundingRect() const noexcept override { return mBoundingRect; }
  QPainterPath shape() const noexcept override { return mShape; }
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget = 0) noexcept override;

  // Operator Overloadings
  SymbolPreviewGraphicsItem& operator=(const SymbolPreviewGraphicsItem& rhs) =
      delete;

signals:

  /// @copydoc AttributeProvider::attributesChanged()
  void attributesChanged() override {}

private:
  // Inherited from AttributeProvider
  /// @copydoc ::librepcb::AttributeProvider::getBuiltInAttributeValue()
  QString getBuiltInAttributeValue(const QString& key) const noexcept override;

  // Types

  struct CachedTextProperties_t {
    QString text;
    qreal fontSize;
    bool rotate180;
    Qt::Alignment align;
    QRectF textRect;
  };

  // General Attributes
  const IF_GraphicsLayerProvider& mLayerProvider;
  const Symbol& mSymbol;
  const Component* mComponent;
  const ComponentSymbolVariantItem* mSymbVarItem;
  QFont mFont;
  bool mDrawBoundingRect;
  QStringList mLocaleOrder;

  // Cached Attributes
  QRectF mBoundingRect;
  QPainterPath mShape;
  QHash<const Text*, CachedTextProperties_t> mCachedTextProperties;
};

/*******************************************************************************
 *  End of File
 ******************************************************************************/

}  // namespace editor
}  // namespace librepcb

#endif