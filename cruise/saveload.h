/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
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
 *
 */

#ifndef CRUISE_SAVELOAD_H
#define CRUISE_SAVELOAD_H

// MOD: #include "common/scummsys.h"
// MOD: #include "graphics/surface.h"
#include "engine/engine.h"

namespace Cruise {

#define CRUISE_SAVEGAME_VERSION 1

#if 0 // MOD:
struct CruiseSavegameHeader {
	uint8 version;
	char saveName[64]; // MOD: Common::String saveName;
	Graphics::Surface *thumbnail;
};
#endif

Common::Error saveSavegameData(const char* path); // MOD: Common::Error saveSavegameData(int saveGameIdx, const Common::String &saveName);
Common::Error loadSavegameData(const char* path); // MOD: Common::Error loadSavegameData(int saveGameIdx);
#if 0 // MOD:
WARN_UNUSED_RESULT bool readSavegameHeader(Common::InSaveFile *in, CruiseSavegameHeader &header, bool skipThumbnail = true);
#endif
void initVars();

} // End of namespace Cruise

#endif
