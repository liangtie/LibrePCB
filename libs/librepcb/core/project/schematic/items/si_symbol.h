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

#ifndef LIBREPCB_CORE_SI_SYMBOL_H
#define LIBREPCB_CORE_SI_SYMBOL_H

/*******************************************************************************
 *  Includes
 ******************************************************************************/
#include "../../../geometry/text.h"
#include "../../../types/angle.h"
#include "../../../types/point.h"
#include "../../../types/uuid.h"
#include "../../../utils/signalslot.h"
#include "si_base.h"

#include <QtCore>

/*******************************************************************************
 *  Namespace / Forward Declarations
 ******************************************************************************/
namespace librepcb {

class ComponentInstance;
class ComponentSymbolVariantItem;
class SI_SymbolPin;
class SI_Text;
class Schematic;
class Symbol;

/*******************************************************************************
 *  Class SI_Symbol
 ******************************************************************************/

/**
 * @brief The SI_Symbol class
 */
class SI_Symbol final : public SI_Base {
  Q_OBJECT

public:
  // Signals
  enum class Event {
    PositionChanged,
    RotationChanged,
    MirroredChanged,
  };
  Signal<SI_Symbol, Event> onEdited;
  typedef Slot<SI_Symbol, Event> OnEditedSlot;

  // Constructors / Destructor
  SI_Symbol() = delete;
  SI_Symbol(const SI_Symbol& other) = delete;
  SI_Symbol(Schematic& schematic, const Uuid& uuid,
            ComponentInstance& cmpInstance, const Uuid& symbolItem,
            const Point& position, const Angle& rotation, bool mirrored,
            bool loadInitialTexts);
  ~SI_Symbol() noexcept;

  // Getters
  const Uuid& getUuid() const noexcept { return mUuid; }
  const Point& getPosition() const noexcept { return mPosition; }
  const Angle& getRotation() const noexcept { return mRotation; }
  bool getMirrored() const noexcept { return mMirrored; }
  QString getName() const noexcept;
  SI_SymbolPin* getPin(const Uuid& pinUuid) const noexcept {
    return mPins.value(pinUuid);
  }
  const QHash<Uuid, SI_SymbolPin*>& getPins() const noexcept { return mPins; }
  ComponentInstance& getComponentInstance() const noexcept {
    return mComponentInstance;
  }
  const Symbol& getLibSymbol() const noexcept { return *mSymbol; }
  const ComponentSymbolVariantItem& getCompSymbVarItem() const noexcept {
    return *mSymbVarItem;
  }

  // Setters
  void setPosition(const Point& newPos) noexcept;
  void setRotation(const Angle& newRotation) noexcept;
  void setMirrored(bool newMirrored) noexcept;

  // Text Methods
  TextList getDefaultTexts() const noexcept;
  const QMap<Uuid, SI_Text*>& getTexts() const noexcept { return mTexts; }
  void addText(SI_Text& text);
  void removeText(SI_Text& text);

  // General Methods
  void addToSchematic() override;
  void removeFromSchematic() override;

  /**
   * @brief Serialize into ::librepcb::SExpression node
   *
   * @param root    Root node to serialize into.
   */
  void serialize(SExpression& root) const;

  // Operator Overloadings
  SI_Symbol& operator=(const SI_Symbol& rhs) = delete;

signals:
  void textAdded(SI_Text& text);
  void textRemoved(SI_Text& text);
  void attributesChanged();

private:
  bool checkAttributesValidity() const noexcept;

  // General
  ComponentInstance& mComponentInstance;
  const ComponentSymbolVariantItem* mSymbVarItem;
  const Symbol* mSymbol;
  QHash<Uuid, SI_SymbolPin*> mPins;  ///< key: symbol pin UUID
  QMap<Uuid, SI_Text*> mTexts;  ///< key: text UUID

  // Attributes
  Uuid mUuid;
  Point mPosition;
  Angle mRotation;
  bool mMirrored;
};

/*******************************************************************************
 *  End of File
 ******************************************************************************/

}  // namespace librepcb

#endif
