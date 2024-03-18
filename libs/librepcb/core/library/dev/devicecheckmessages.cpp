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
#include "devicecheckmessages.h"

/*******************************************************************************
 *  Namespace
 ******************************************************************************/
namespace librepcb {

/*******************************************************************************
 *  MsgDeviceHasNoParts
 ******************************************************************************/

MsgDeviceHasNoParts::MsgDeviceHasNoParts() noexcept
  : RuleCheckMessage(
        Severity::Hint, tr("No part numbers added"),
        tr("There are no orderable parts added to the device. It's recommended "
           "(but not mandatory) to add the concrete manufacturer part numbers "
           "this device is valid for. These MPNs are used by the BOM export to "
           "make BOMs of projects much more complete and accurate.\n\n"
           "If this device doesn't represent an orderable part, just ignore "
           "this message."),
        "no_parts") {
}

/*******************************************************************************
 *  MsgNoPadsInDeviceConnected
 ******************************************************************************/

MsgNoPadsInDeviceConnected::MsgNoPadsInDeviceConnected() noexcept
  : RuleCheckMessage(
        Severity::Warning,  // Only warning because it could be a false-positive
        tr("No pads connected"),
        tr("The chosen package contains pads, but none of them are connected "
           "to component signals. So these pads have no electrical function "
           "and when adding the device to a PCB, no traces can be connected to "
           "them.\n\nTo fix this issue, connect the package pads to their "
           "corresponding component signals in the table widget.\n\nIf all "
           "pads have only a mechanical purpose and thus don't need to be "
           "connected to component signals, this message can be ignored."),
        "no_pads_connected") {
}

/*******************************************************************************
 *  End of File
 ******************************************************************************/

}  // namespace librepcb
