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

#include "cruise/cruise.h"
#include "cruise/cruise_main.h"
#include "cruise/staticres.h"

// MOD: #include "engines/metaengine.h"
// MOD: #include "gui/saveload.h"
// MOD: #include "common/system.h"

namespace apk { // MOD:
    extern bool s_RulesCanSaveLoad;
    extern char s_OverlayText[33];
    extern uint16 s_OverlayTime;
}

namespace Cruise {

extern int currentMouseButton;

menuStruct *menuTable[8];

menuStruct *createMenu(int X, int Y, const char *menuName) {
	menuStruct *entry;

	entry = (menuStruct *) MemAlloc(sizeof(menuStruct));
	assert(entry);

	entry->x = X - 160 / 2;
	entry->y = Y;
	entry->stringPtr = menuName;
	entry->numElements = 0;
	entry->ptrNextElement = nullptr;
	entry->gfx = renderText(160, menuName);

	return entry;
}

// TODO: rewrite to remove the goto
void addSelectableMenuEntry(int ovlIdx, int headerIdx, menuStruct *pMenu, int param2, int color, const char *menuText) {
	menuElementStruct *di;
	menuElementStruct *var_6;
	menuElementStruct *pNewElement;
	menuElementSubStruct *pSubStruct;
	menuElementSubStruct *pSubStructCurrent;

	if (pMenu->numElements <= 48) {
		var_6 = pMenu->ptrNextElement;

		if (var_6) {
			do {
				di = var_6;
				if (param2) {
					if (!strcmp(var_6->string, menuText)) {
						pNewElement = var_6;
						pSubStruct = (menuElementSubStruct *)allocAndZero(sizeof(menuElementSubStruct));
						assert(pSubStruct);

						pSubStruct->pNext = nullptr;
						pSubStruct->ovlIdx = ovlIdx;
						pSubStruct->header = headerIdx;

						pSubStructCurrent = pNewElement->ptrSub;

						if (!pSubStructCurrent) {
							pNewElement->ptrSub = pSubStruct;
							return;
						}

						if (pSubStructCurrent->pNext) {
							do {
								pSubStructCurrent = pSubStructCurrent->pNext;
							} while (pSubStructCurrent->pNext);
						}

						pSubStructCurrent->pNext = pSubStruct;
						return;
					}
				}
				var_6 = var_6->next;
			} while (var_6);

			var_6 = di;
		}

		pNewElement = (menuElementStruct *)allocAndZero(sizeof(menuElementStruct));
		assert(pNewElement);
		pSubStruct = (menuElementSubStruct *)allocAndZero(sizeof(menuElementSubStruct));
		assert(pSubStruct);

		pNewElement->string = menuText;
		pNewElement->next = nullptr;
		pNewElement->selected = false;
		pNewElement->color = color;
		pNewElement->gfx = renderText(160, menuText);

		if (var_6 == nullptr) {
			pMenu->ptrNextElement = pNewElement;
		} else {
			var_6->next = pNewElement;
		}

		pNewElement->ptrSub = pSubStruct;

		pSubStruct->pNext = nullptr;
		pSubStruct->ovlIdx = ovlIdx;
		pSubStruct->header = headerIdx;

		pMenu->numElements++;
	}
}

void updateMenuMouse(int mouseX, int mouseY, menuStruct *pMenu) {
	if (pMenu) {
		if (pMenu->gfx) {
			int height = pMenu->gfx->height;	// rustine
			int var_2 = 0;
			menuElementStruct *pCurrentEntry = pMenu->ptrNextElement;

			while (pCurrentEntry) {
				pCurrentEntry->selected = false;

				if (var_2 == 0) {
					if ((mouseX > pCurrentEntry->x) && ((pCurrentEntry->x + 160) >= mouseX)) {
						if ((mouseY > pCurrentEntry->y) && ((pCurrentEntry->y + height) >= mouseY)) {
							var_2 = 1;
							pCurrentEntry->selected = true;
						}
					}
				}

				pCurrentEntry = pCurrentEntry->next;
			}
		}
	}
}

bool manageEvents();

#if 0 // MOD:
int processMenu(menuStruct *pMenu) {
	int16 mouseX;
	int16 mouseY;
	int16 mouseButton;
	int di;
	int si;
	currentActiveMenu = 0;

	mainDraw(true);
	flipScreen();

	// MOD: di = 0;
	// MOD: si = 0;

	do {
		getMouseStatus(&main10, &mouseX, &mouseButton, &mouseY);

		updateMenuMouse(mouseX, mouseY, pMenu);

		if (mouseButton) {
			if (di) {
				si = 1;
			}
		} else {
			di = 1;
		}

		mainDraw(true);
		flipScreen();

		manageEvents();
		g_system->delayMillis(10);
		if (_vm->shouldQuit())
			return -1;

//    readKeyboard();
	} while (!si);

	currentActiveMenu = -1;

	mainDraw(true);
	flipScreen();

	if (mouseButton & 1) {
		menuElementSubStruct* pSelectedEntry = getSelectedEntryInMenu(pMenu);

		if (pSelectedEntry) {
			return pSelectedEntry->header;
		} else {
			return -1;
		}
	}

	return -1;
}
#endif

static void handleSaveLoad(bool saveFlag) {
	#if 0 // MOD:
	if (saveFlag)
		_vm->saveGameDialog();
	else
		_vm->loadGameDialog();
	#endif
}

static int playerMenu_Stop(menuStruct* pMenu, int mouseButton);

void playerMenu_EventCb(void*, apk::Event& evt) {
	ProcessEvents_NoReaction(evt);
}

static int playerMenu_di, playerMenu_si; // MOD:

void playerMenu_TimerCb(void* data) {
	int16 mouseX;
	int16 mouseY;
	int16 mouseButton;

	menuStruct* pMenu = (menuStruct*) data;
	assert(pMenu);


	getMouseStatus(&main10, &mouseX, &mouseButton, &mouseY);

	updateMenuMouse(mouseX, mouseY, pMenu);

	mainDraw(true);
	flipScreen();


    if (mouseButton) {
        if (playerMenu_di) {
            playerMenu_si = 1;
        }
    } else {
        playerMenu_di = 1;
    }

    if (playerMenu_si == 1) {
        playerMenu_Stop(pMenu, mouseButton);
    }

}

void playerMenu_LoadGame(const char* path) {
    if (apk::s_RulesCanSaveLoad == false)
        return;

    //apk::requester_okay("Load Game?", "Load Game!??");
	Common::Error rv = loadSavegameData(path);
	debug("load rv = %ld", rv);
    apk::s_OverlayTime = 0;
}

void playerMenu_SaveGame(const char* path) {
    if (apk::s_RulesCanSaveLoad == false)
        return;

    //apk::requester_okay("Save Game?", "Save Game!??");
    Common::Error rv = saveSavegameData(path);
    debug("save rv = %ld", rv);

    apk::strncpy(apk::s_OverlayText, "Saved...", sizeof(apk::s_OverlayText));
    apk::s_OverlayTime = 120;
}

void playerMenu_ResetGame() {
    _vm->sound().fadeOutMusic();
    Op_FadeOut();
    apk::video::clearChunkyPixels(0); // MOD: memset(globalScreen, 0, 320 * 200);
    gfxModuleData_clearAll();
    gfxModuleData_zeroPalette();
    initVars();
    _vm->initAllData();
    changeCursor(CURSOR_NORMAL);
    userEnabled = 0;
    apk::s_RulesCanSaveLoad = false;
}

void playerMenu_ExitGame() {
    apk::performQuit();
}

void playerMenu_PauseGame() {
	requester_okay("Paused", "Continue");
}

int playerMenu_Start(int menuX, int menuY) { // MOD:

    debug("start of player menu");

	if (playerMenuEnabled && displayOn) {
		if (remdo) {
			_vm->sound().stopMusic();
			freeStuff2();
		}

		freeDisk();

		menuTable[0] = createMenu(menuX, menuY, _vm->langString(ID_PLAYER_MENU));
		assert(menuTable[0]);

		//addSelectableMenuEntry(0, 3, menuTable[0], 1, -1, "Save game disk");
		if (userEnabled) {
			addSelectableMenuEntry(0, 4, menuTable[0], 1, -1, _vm->langString(ID_SAVE));
		}
		addSelectableMenuEntry(0, 5, menuTable[0], 1, -1, _vm->langString(ID_LOAD));
		addSelectableMenuEntry(0, 6, menuTable[0], 1, -1, _vm->langString(ID_RESTART));
		addSelectableMenuEntry(0, 7, menuTable[0], 1, -1, _vm->langString(ID_QUIT));

        playerMenu_di = 0;
        playerMenu_si = 0;
	    currentActiveMenu = 0;

		apk::video::pushWindowEventCallback(playerMenu_EventCb, menuTable[0]);
		apk::video::pushWindowTimerCallback(playerMenu_TimerCb, menuTable[0]);

	}

    return 0;
}

static int playerMenu_Stop(menuStruct* pMenu, int mouseButton) { // MOD:

    int rc = 0;
	apk::video::popWindowEventCallback();
	apk::video::popWindowTimerCallback();

	currentActiveMenu = -1;

	if (mouseButton & 1) {
		menuElementSubStruct* pSelectedEntry = getSelectedEntryInMenu(pMenu);

		if (pSelectedEntry) {
			rc = pSelectedEntry->header;
		} else {
			rc = -1;
		}
	}

	freeMenu(menuTable[0]);
	menuTable[0] = nullptr;
	currentMouseButton = 0;

	switch (rc) {
		case 3: // select save drive
			break;
		case 4: // save
		case 5: // load
			handleSaveLoad(rc == 4);
			break;
		case 6: // restart
			_vm->sound().fadeOutMusic();
			Op_FadeOut();
			apk::video::clearChunkyPixels(0); // MOD: memset(globalScreen, 0, 320 * 200);
			initVars();
			_vm->initAllData();
			changeCursor(CURSOR_NORMAL);
			userEnabled = 0;
			break;
		case 7: // exit
			break;
		default:
			break;
	}

	mainDraw(true);
	flipScreen();

    debug("End of player menu");

    return 0;
}

#if 0 // MOD:
int playerMenu(int menuX, int menuY) {
	//int restartGame = 0;

	if (playerMenuEnabled && displayOn) {
		if (remdo) {
			_vm->sound().stopMusic();
			freeStuff2();
		}
		/*
		    if (currentMenu) {
		      freeMenu(currentMenu);
		      currentMenu = 0;
		      selectDown = 0;
		      menuDown = 0;
		      main9 = -1;
		    }

		    if (inventoryMenu) {
		      freeMenu(inventoryMenu);
		      inventoryMenu = 0;
		      selectDown = 0;
		      menuDown = 0;
		      main9 = -1;
		    }*/

		/*    if (mouseVar2) {
		      free3(mouseVar2);
		    } */

		/*    mouseVar2 = 0;
		    linkedRelation = 0; */
		freeDisk();

		menuTable[0] = createMenu(menuX, menuY, _vm->langString(ID_PLAYER_MENU));
		assert(menuTable[0]);

		//addSelectableMenuEntry(0, 3, menuTable[0], 1, -1, "Save game disk");
		if (userEnabled) {
			addSelectableMenuEntry(0, 4, menuTable[0], 1, -1, _vm->langString(ID_SAVE));
		}
		addSelectableMenuEntry(0, 5, menuTable[0], 1, -1, _vm->langString(ID_LOAD));
		addSelectableMenuEntry(0, 6, menuTable[0], 1, -1, _vm->langString(ID_RESTART));
		addSelectableMenuEntry(0, 7, menuTable[0], 1, -1, _vm->langString(ID_QUIT));

		int retourMenu = processMenu(menuTable[0]);

		freeMenu(menuTable[0]);
		menuTable[0] = nullptr;
		currentMouseButton = 0;

		switch (retourMenu) {
		case 3: // select save drive
			break;
		case 4: // save
		case 5: // load
			handleSaveLoad(retourMenu == 4);
			break;
		case 6: // restart
			_vm->sound().fadeOutMusic();
			Op_FadeOut();
			apk::gfx::clearChunkyPixels(0); // MOD: memset(globalScreen, 0, 320 * 200);
			initVars();
			_vm->initAllData();
			changeCursor(CURSOR_NORMAL);
			userEnabled = 0;
			break;
		case 7: // exit
			return 1;
		default:
			break;
		}
	}

	return 0;
}
#endif

void freeMenu(menuStruct *pMenu) {
	menuElementStruct *pElement = pMenu->ptrNextElement;

	while (pElement) {
		menuElementStruct *next;
		menuElementSubStruct *pSub = pElement->ptrSub;

		next = pElement->next;

		while (pSub) {
			menuElementSubStruct *nextSub;

			nextSub = pSub->pNext;

			MemFree(pSub);

			pSub = nextSub;
		}

		if (pElement->gfx) {
			freeGfx(pElement->gfx);
		}

		MemFree(pElement);

		pElement = next;
	}

	freeGfx(pMenu->gfx);
	MemFree(pMenu);
}

} // End of namespace Cruise
