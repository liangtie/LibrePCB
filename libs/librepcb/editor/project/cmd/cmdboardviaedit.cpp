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
#include "cmdboardviaedit.h"

#include <librepcb/core/project/board/items/bi_via.h>
#include <librepcb/core/types/layer.h>

#include <QtCore>

/*******************************************************************************
 *  Namespace
 ******************************************************************************/
namespace librepcb {
namespace editor {

/*******************************************************************************
 *  Constructors / Destructor
 ******************************************************************************/

CmdBoardViaEdit::CmdBoardViaEdit(BI_Via& via) noexcept
  : UndoCommand(tr("Edit via")),
    mVia(via),
    mOldStartLayer(&via.getVia().getStartLayer()),
    mNewStartLayer(mOldStartLayer),
    mOldEndLayer(&via.getVia().getEndLayer()),
    mNewEndLayer(mOldEndLayer),
    mOldPos(via.getPosition()),
    mNewPos(mOldPos),
    mOldSize(via.getSize()),
    mNewSize(mOldSize),
    mOldDrillDiameter(via.getDrillDiameter()),
    mNewDrillDiameter(mOldDrillDiameter),
    mOldExposureConfig(via.getVia().getExposureConfig()),
    mNewExposureConfig(mOldExposureConfig) {
}

CmdBoardViaEdit::~CmdBoardViaEdit() noexcept {
  if (!wasEverExecuted()) {
    mVia.setPosition(mOldPos);
    mVia.setSize(mOldSize);
    mVia.setDrillDiameter(mOldDrillDiameter);
  }
}

/*******************************************************************************
 *  Setters
 ******************************************************************************/

void CmdBoardViaEdit::setLayers(const Layer& startLayer,
                                const Layer& endLayer) noexcept {
  Q_ASSERT(!wasEverExecuted());
  mNewStartLayer = &startLayer;
  mNewEndLayer = &endLayer;
}

void CmdBoardViaEdit::setPosition(const Point& pos, bool immediate) noexcept {
  Q_ASSERT(!wasEverExecuted());
  mNewPos = pos;
  if (immediate) mVia.setPosition(mNewPos);
}

void CmdBoardViaEdit::translate(const Point& deltaPos,
                                bool immediate) noexcept {
  Q_ASSERT(!wasEverExecuted());
  mNewPos += deltaPos;
  if (immediate) mVia.setPosition(mNewPos);
}

void CmdBoardViaEdit::snapToGrid(const PositiveLength& gridInterval,
                                 bool immediate) noexcept {
  setPosition(mNewPos.mappedToGrid(gridInterval), immediate);
}

void CmdBoardViaEdit::rotate(const Angle& angle, const Point& center,
                             bool immediate) noexcept {
  Q_ASSERT(!wasEverExecuted());
  mNewPos.rotate(angle, center);
  if (immediate) mVia.setPosition(mNewPos);
}

void CmdBoardViaEdit::mirrorLayers(int innerLayers) noexcept {
  Q_ASSERT(!wasEverExecuted());
  const Layer* tmp = mNewStartLayer;
  mNewStartLayer = &mNewEndLayer->mirrored(innerLayers);
  mNewEndLayer = &tmp->mirrored(innerLayers);
}

void CmdBoardViaEdit::setSize(const PositiveLength& size,
                              bool immediate) noexcept {
  Q_ASSERT(!wasEverExecuted());
  mNewSize = size;
  if (immediate) mVia.setSize(mNewSize);
}

void CmdBoardViaEdit::setDrillDiameter(const PositiveLength& diameter,
                                       bool immediate) noexcept {
  Q_ASSERT(!wasEverExecuted());
  mNewDrillDiameter = diameter;
  if (immediate) mVia.setDrillDiameter(mNewDrillDiameter);
}

void CmdBoardViaEdit::setExposureConfig(const MaskConfig& config) noexcept {
  Q_ASSERT(!wasEverExecuted());
  mNewExposureConfig = config;
}

/*******************************************************************************
 *  Inherited from UndoCommand
 ******************************************************************************/

bool CmdBoardViaEdit::performExecute() {
  performRedo();  // can throw

  if (mNewStartLayer != mOldStartLayer) return true;
  if (mNewEndLayer != mOldEndLayer) return true;
  if (mNewPos != mOldPos) return true;
  if (mNewSize != mOldSize) return true;
  if (mNewDrillDiameter != mOldDrillDiameter) return true;
  if (mNewExposureConfig != mOldExposureConfig) return true;
  return false;
}

void CmdBoardViaEdit::performUndo() {
  mVia.setLayers(*mOldStartLayer, *mOldEndLayer);  // can throw
  mVia.setPosition(mOldPos);
  mVia.setSize(mOldSize);
  mVia.setDrillDiameter(mOldDrillDiameter);
  mVia.setExposureConfig(mOldExposureConfig);
}

void CmdBoardViaEdit::performRedo() {
  mVia.setLayers(*mNewStartLayer, *mNewEndLayer);  // can throw
  mVia.setPosition(mNewPos);
  mVia.setSize(mNewSize);
  mVia.setDrillDiameter(mNewDrillDiameter);
  mVia.setExposureConfig(mNewExposureConfig);
}

/*******************************************************************************
 *  End of File
 ******************************************************************************/

}  // namespace editor
}  // namespace librepcb
