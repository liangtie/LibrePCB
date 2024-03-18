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

/*******************************************************************************
 *  Includes
 ******************************************************************************/
#include "circle.h"

#include <QtCore>

/*******************************************************************************
 *  Namespace
 ******************************************************************************/
namespace librepcb {

/*******************************************************************************
 *  Constructors / Destructor
 ******************************************************************************/

Circle::Circle(const Circle& other) noexcept
  : onEdited(*this),
    mUuid(other.mUuid),
    mLayer(other.mLayer),
    mLineWidth(other.mLineWidth),
    mIsFilled(other.mIsFilled),
    mIsGrabArea(other.mIsGrabArea),
    mCenter(other.mCenter),
    mDiameter(other.mDiameter) {
}

Circle::Circle(const Uuid& uuid, const Circle& other) noexcept : Circle(other) {
  mUuid = uuid;
}

Circle::Circle(const Uuid& uuid, const Layer& layer,
               const UnsignedLength& lineWidth, bool fill, bool isGrabArea,
               const Point& center, const PositiveLength& diameter) noexcept
  : onEdited(*this),
    mUuid(uuid),
    mLayer(&layer),
    mLineWidth(lineWidth),
    mIsFilled(fill),
    mIsGrabArea(isGrabArea),
    mCenter(center),
    mDiameter(diameter) {
}

Circle::Circle(const SExpression& node)
  : onEdited(*this),
    mUuid(deserialize<Uuid>(node.getChild("@0"))),
    mLayer(deserialize<const Layer*>(node.getChild("layer/@0"))),
    mLineWidth(deserialize<UnsignedLength>(node.getChild("width/@0"))),
    mIsFilled(deserialize<bool>(node.getChild("fill/@0"))),
    mIsGrabArea(deserialize<bool>(node.getChild("grab_area/@0"))),
    mCenter(node.getChild("position")),
    mDiameter(deserialize<PositiveLength>(node.getChild("diameter/@0"))) {
}

Circle::~Circle() noexcept {
}

/*******************************************************************************
 *  Setters
 ******************************************************************************/

bool Circle::setLayer(const Layer& layer) noexcept {
  if (&layer == mLayer) {
    return false;
  }

  mLayer = &layer;
  onEdited.notify(Event::LayerChanged);
  return true;
}

bool Circle::setLineWidth(const UnsignedLength& width) noexcept {
  if (width == mLineWidth) {
    return false;
  }

  mLineWidth = width;
  onEdited.notify(Event::LineWidthChanged);
  return true;
}

bool Circle::setIsFilled(bool isFilled) noexcept {
  if (isFilled == mIsFilled) {
    return false;
  }

  mIsFilled = isFilled;
  onEdited.notify(Event::IsFilledChanged);
  return true;
}

bool Circle::setIsGrabArea(bool isGrabArea) noexcept {
  if (isGrabArea == mIsGrabArea) {
    return false;
  }

  mIsGrabArea = isGrabArea;
  onEdited.notify(Event::IsGrabAreaChanged);
  return true;
}

bool Circle::setCenter(const Point& center) noexcept {
  if (center == mCenter) {
    return false;
  }

  mCenter = center;
  onEdited.notify(Event::CenterChanged);
  return true;
}

bool Circle::setDiameter(const PositiveLength& dia) noexcept {
  if (dia == mDiameter) {
    return false;
  }

  mDiameter = dia;
  onEdited.notify(Event::DiameterChanged);
  return true;
}

/*******************************************************************************
 *  General Methods
 ******************************************************************************/

void Circle::serialize(SExpression& root) const {
  root.appendChild(mUuid);
  root.appendChild("layer", *mLayer);
  root.ensureLineBreak();
  root.appendChild("width", mLineWidth);
  root.appendChild("fill", mIsFilled);
  root.appendChild("grab_area", mIsGrabArea);
  root.appendChild("diameter", mDiameter);
  mCenter.serialize(root.appendList("position"));
  root.ensureLineBreak();
}

/*******************************************************************************
 *  Operator Overloadings
 ******************************************************************************/

bool Circle::operator==(const Circle& rhs) const noexcept {
  if (mUuid != rhs.mUuid) return false;
  if (mLayer != rhs.mLayer) return false;
  if (mLineWidth != rhs.mLineWidth) return false;
  if (mIsFilled != rhs.mIsFilled) return false;
  if (mIsGrabArea != rhs.mIsGrabArea) return false;
  if (mCenter != rhs.mCenter) return false;
  if (mDiameter != rhs.mDiameter) return false;
  return true;
}

Circle& Circle::operator=(const Circle& rhs) noexcept {
  if (mUuid != rhs.mUuid) {
    mUuid = rhs.mUuid;
    onEdited.notify(Event::UuidChanged);
  }
  setLayer(*rhs.mLayer);
  setLineWidth(rhs.mLineWidth);
  setIsFilled(rhs.mIsFilled);
  setIsGrabArea(rhs.mIsGrabArea);
  setCenter(rhs.mCenter);
  setDiameter(rhs.mDiameter);
  return *this;
}

/*******************************************************************************
 *  End of File
 ******************************************************************************/

}  // namespace librepcb
