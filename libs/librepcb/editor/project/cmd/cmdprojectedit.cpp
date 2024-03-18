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
#include "cmdprojectedit.h"

#include <librepcb/core/project/project.h>

#include <QtCore>

/*******************************************************************************
 *  Namespace
 ******************************************************************************/
namespace librepcb {
namespace editor {

/*******************************************************************************
 *  Constructors / Destructor
 ******************************************************************************/

CmdProjectEdit::CmdProjectEdit(Project& project) noexcept
  : UndoCommand(tr("Edit Project Metadata")),
    mProject(project),
    mOldName(mProject.getName()),
    mNewName(mProject.getName()),
    mOldAuthor(mProject.getAuthor()),
    mNewAuthor(mProject.getAuthor()),
    mOldVersion(mProject.getVersion()),
    mNewVersion(mProject.getVersion()),
    mOldAttributes(mProject.getAttributes()),
    mNewAttributes(mOldAttributes),
    mOldLocaleOrder(mProject.getLocaleOrder()),
    mNewLocaleOrder(mOldLocaleOrder),
    mOldNormOrder(mProject.getNormOrder()),
    mNewNormOrder(mOldNormOrder),
    mOldOutputJobs(project.getOutputJobs()),
    mNewOutputJobs(mOldOutputJobs) {
}

CmdProjectEdit::~CmdProjectEdit() noexcept {
}

/*******************************************************************************
 *  Setters
 ******************************************************************************/

void CmdProjectEdit::setName(const ElementName& newName) noexcept {
  Q_ASSERT(!wasEverExecuted());
  mNewName = newName;
}

void CmdProjectEdit::setAuthor(const QString& newAuthor) noexcept {
  Q_ASSERT(!wasEverExecuted());
  mNewAuthor = newAuthor;
}

void CmdProjectEdit::setVersion(const FileProofName& newVersion) noexcept {
  Q_ASSERT(!wasEverExecuted());
  mNewVersion = newVersion;
}

void CmdProjectEdit::setAttributes(const AttributeList& attributes) noexcept {
  Q_ASSERT(!wasEverExecuted());
  mNewAttributes = attributes;
}

void CmdProjectEdit::setLocaleOrder(const QStringList& order) noexcept {
  Q_ASSERT(!wasEverExecuted());
  mNewLocaleOrder = order;
}

void CmdProjectEdit::setNormOrder(const QStringList& order) noexcept {
  Q_ASSERT(!wasEverExecuted());
  mNewNormOrder = order;
}

void CmdProjectEdit::setOutputJobs(const OutputJobList& jobs) noexcept {
  Q_ASSERT(!wasEverExecuted());
  mNewOutputJobs = jobs;
}

/*******************************************************************************
 *  Inherited from UndoCommand
 ******************************************************************************/

bool CmdProjectEdit::performExecute() {
  performRedo();  // can throw

  if (mNewName != mOldName) return true;
  if (mNewAuthor != mOldAuthor) return true;
  if (mNewVersion != mOldVersion) return true;
  if (mNewAttributes != mOldAttributes) return true;
  if (mNewLocaleOrder != mOldLocaleOrder) return true;
  if (mNewNormOrder != mOldNormOrder) return true;
  if (mNewOutputJobs != mOldOutputJobs) return true;
  return false;
}

void CmdProjectEdit::performUndo() {
  mProject.setName(mOldName);
  mProject.setAuthor(mOldAuthor);
  mProject.setVersion(mOldVersion);
  mProject.setAttributes(mOldAttributes);
  mProject.setLocaleOrder(mOldLocaleOrder);
  mProject.setNormOrder(mOldNormOrder);
  mProject.getOutputJobs() = mOldOutputJobs;
}

void CmdProjectEdit::performRedo() {
  mProject.setName(mNewName);
  mProject.setAuthor(mNewAuthor);
  mProject.setVersion(mNewVersion);
  mProject.setAttributes(mNewAttributes);
  mProject.setLocaleOrder(mNewLocaleOrder);
  mProject.setNormOrder(mNewNormOrder);
  mProject.getOutputJobs() = mNewOutputJobs;
}

/*******************************************************************************
 *  End of File
 ******************************************************************************/

}  // namespace editor
}  // namespace librepcb
