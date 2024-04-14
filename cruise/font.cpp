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

// MOD: #include "common/endian.h"
// MOD: #include "common/file.h"
// MOD: #include "common/util.h"

#include "cruise/cruise_main.h"
#include "cruise/mouse.h"
#include "cruise/staticres.h"

namespace Cruise {

const int SPACE_WIDTH = 5;

/**
 * Determines the line size by finding the highest character in the given font set
 */
int32 getLineHeight(int16 charCount, const FontEntry *fontPtr) {
	int32 highestChar = 0;

	if (!charCount)
		return (0);

	for (int i = 0; i < charCount; ++i) {
		int charHeight = fontPtr[i].charHeight;
		if (charHeight > highestChar) highestChar = charHeight;
	}

	return highestChar;
}

/**
 * This function determines how many lines the text will have
 */
int32 getTextLineCount(int32 rightBorder_X, int16 wordSpacingWidth,
					   const FontEntry *fontData, const char *textString) {
    //return 100; // MOD:

	const char *localString = textString;
	const char *tempPtr = textString;
	uint8 ch;
	int32 total = 0, lineLength = 0;

	if (rightBorder_X == 0)
		error("getTextLineCount() - invalid parameter");

	if (!*textString)
		return (0);

	ch = *localString;

	do {
		int32 charData = fontCharacterTable[ch];

		if (ch == '|') {
			lineLength = rightBorder_X;
			localString = tempPtr;
		} else if (charData >= 0) {
			lineLength += wordSpacingWidth + (int16)fontData[charData].charWidth;
		} else if (ch == ' ') {
			lineLength += wordSpacingWidth + SPACE_WIDTH;
			localString = tempPtr;
		}

		if (lineLength >= rightBorder_X) {
			total += rightBorder_X;
			tempPtr = localString;
			lineLength = 0;
		}

		ch = *++tempPtr;
	} while (ch);

	if (lineLength > 0)
		total += rightBorder_X;


	return (total / rightBorder_X);
}

void loadFNT(const char *fileName) {
	uint8 header[4];

	_systemFNT = nullptr;

	Common::ReadFile fontFileHandle;

	// MOD: if (!fontFileHandle.exists(fileName))
	// MOD:	return;
    assert(fontFileHandle.exists(fileName));

	fontFileHandle.open((const char *)fileName);

	fontFileHandle.read(header, 4);


#if 0 // MOD:
    if (strcmp((char *)header, "FNT") == 0) {
#else
    assert(strcmp((char *)header, "FNT") == 0);
    {
#endif
		uint32 fontSize = fontFileHandle.readUint32BE();

		_systemFNT = (uint8 *)mallocAndZero(fontSize);

		if (_systemFNT != nullptr) {
			fontFileHandle.seek(4);
			fontFileHandle.read(_systemFNT, fontSize);

			// Flip structure values from BE to LE for font files - this is for consistency
			// with font resources, which are in LE formatt
			FontInfo *f = (FontInfo *)_systemFNT;
			bigEndianLongToNative(&f->offset);
			bigEndianLongToNative(&f->size);
			flipGen(&f->numChars, 6);	// numChars, hSpacing, and vSpacing

			FontEntry *fe = (FontEntry *)(_systemFNT + sizeof(FontInfo));

			for (int i = 0; i < f->numChars; ++i, ++fe) {
				bigEndianLongToNative(&fe->offset);	// Flip 32-bit offset field
				flipGen(&fe->v1, 8);	// Flip remaining 16-bit fields
			}
		}
	}

	fontFileHandle.close();
}

void initSystem() {
	int32 i;

	itemColor = 15;
	titleColor = 9;
	selectColor = 13;
	subColor = 10;

	for (i = 0; i < 64; i++) {
		preloadData[i].name[0] = '\0';
		preloadData[i].ptr = nullptr;
		preloadData[i].nofree = 0;
	}

	doFade = 0;
	fadeFlag = 0;
	scroll = 0;
	switchPal = 0;
	masterScreen = 0;

	changeCursor(CURSOR_NOMOUSE);
	changeCursor(CURSOR_NORMAL);
	mouseOn();

	cmdLine[0] = '\0';

	loadFNT("system.fnt");
}

void freeSystem() {
	MemFree(_systemFNT);
}

void bigEndianShortToNative(void *var) {
	endian::poke_inplace<uint16, endian::Big, endian::Native>(var); // MOD: WRITE_UINT16(var, READ_BE_UINT16(var));
}

void bigEndianLongToNative(void *var) {
	endian::poke_inplace<uint32, endian::Big, endian::Native>(var); // MOD: WRITE_UINT32(var, READ_BE_UINT32(var));
}

void flipGen(void *var, int32 length) {
#if 0 // MOD:
	short int *varPtr = (int16 *) var;

	for (int i = 0; i < (length / 2); i++) {
		bigEndianShortToNative(&varPtr[i]);
	}
#else
    int16* varPtr = (int16*) var;

	for (int i = 0; i < (length / sizeof(int16)); i++) {
        varPtr[i] = endian::pod<int16, endian::Big>(varPtr[i]);
	}
#endif
}

void renderWord(const uint8 *fontPtr_Data, uint8 *outBufferPtr, int xOffset, int yOffset,
				int32 height, int32 param4, int32 stringRenderBufferSize, int32 width, int32 charWidth) {
	const uint8 *fontPtr_Data2 = fontPtr_Data + height * 2;
	outBufferPtr += yOffset * width + xOffset;

	for (int i = 0; i < height; i++) {	// y++
		uint16 bitSet1 = READ_BE_UINT16(fontPtr_Data);
		uint16 bitSet2 = READ_BE_UINT16(fontPtr_Data2);

		fontPtr_Data += sizeof(uint16);
		fontPtr_Data2 += sizeof(uint16);

		for (int j = 0; j < charWidth; j++) {
			if (((bitSet1 >> 15) & 1)) {
				*outBufferPtr = ((bitSet2 >> 15) & 1) + 1;
			}
			outBufferPtr++;

			bitSet1 <<= 1;
			bitSet2 <<= 1;
		}
		outBufferPtr += width - charWidth;
	}
}

// returns character count and pixel size (via pointer) per line of the string (old: prepareWordRender(int32 param, int32 var1, int16* out2, uint8* ptr3, uint8* string))
int32 prepareWordRender(int32 inRightBorder_X, int16 wordSpacingWidth,
						int16 *strPixelLength, const FontEntry *fontData, const char *textString) {
	const char *localString = textString;

	int32 counter = 0;
	int32 finish = 0;
	int32 temp_pc = 0;	// var_A                // temporary pixel count save
	int32 temp_cc = 0;	// var_C                // temporary char  count save
	int32 pixelCount = 0;	// si

	do {
		uint8 character = *(localString++);
		int16 charData = fontCharacterTable[character];

		if (character == ' ') {
			temp_cc = counter;
			temp_pc = pixelCount;

			if (pixelCount + wordSpacingWidth + SPACE_WIDTH >=
			        inRightBorder_X) {
				finish = 1;
			} else {
				pixelCount += wordSpacingWidth + SPACE_WIDTH;
			}
		} else {
			if (character == '|' || !character) {
				finish = 1;
			} else {
				if (charData >= 0) {
					if (pixelCount + wordSpacingWidth +
							(int16)fontData[charData].charWidth >= inRightBorder_X) {
						finish = 1;
						if (temp_pc) {
							pixelCount = temp_pc;
							counter = temp_cc;
						}
					} else {
						pixelCount += wordSpacingWidth +
							(int16)fontData[charData].charWidth;
					}
				}
			}
		}
		counter++;
	} while (!finish);

	*strPixelLength = (int16) pixelCount;
	return counter;
}

void drawString(int32 x, int32 y, const char *string, uint8 *buffer, uint8 fontColor, int32 rightBorder_X) {

	// Get the rendered text to display
	gfxEntryStruct *s = renderText(rightBorder_X, string);

	// Draw the message
	drawMessage(s, x, y, rightBorder_X - x, fontColor, buffer);

	// Free the data
	// delete s->imagePtr; // MOD:
	// free(s); // MOD:
}

// calculates all necessary datas and renders text
gfxEntryStruct *renderText(int inRightBorder_X, const char *string) {

	const FontInfo *fontPtr;
	const FontEntry *fontPtr_Desc;
	const uint8 *fontPtr_Data;
	int16 wordSpacingWidth;	// 0 or -1
	int16 wordSpacingHeight;// 0 or -1
	int32 rightBorder_X;
	int32 lineHeight;
	int32 numLines;
	int32 stringHeight;
	int32 stringFinished;
	int32 stringWidth;	// var_1C
	int32 stringRenderBufferSize;
	//  int32 useDynamicBuffer;
	uint8 *currentStrRenderBuffer;
	//  int32 var_8;                          // don't need that one
	int32 heightOffset;	// var_12     // how much pixel-lines have already been drawn
	//  int32 var_1E;
	gfxEntryStruct *generatedGfxEntry;

	// check if string is empty
	if (!string) {
		return nullptr;
	}
	// check if font has been loaded, else get system font
	if (fontFileIndex != -1) {
		fontPtr = (const FontInfo *)filesDatabase[fontFileIndex].subData.ptr;

		if (!fontPtr) {
			fontPtr = (const FontInfo *)_systemFNT;
		}
	} else {
		fontPtr = (const FontInfo *)_systemFNT;
	}

    assert(fontPtr); // MOD:
	// MOD: if (!fontPtr) {
	// MOD: 	return nullptr;
	// MOD: }

	fontPtr_Desc = (const FontEntry *)((const uint8 *)fontPtr + sizeof(FontInfo));
	fontPtr_Data = (const uint8 *)fontPtr + fontPtr->offset;

	lineHeight = getLineHeight(fontPtr->numChars, fontPtr_Desc);

	wordSpacingWidth = fontPtr->hSpacing;
	wordSpacingHeight = fontPtr->vSpacing;

	// if right border is higher then screenwidth (+ spacing), adjust border
	if (inRightBorder_X > 310) {
		rightBorder_X = 310;
	} else {
		rightBorder_X = inRightBorder_X;
	}
	numLines = getTextLineCount(rightBorder_X, wordSpacingWidth, fontPtr_Desc, string);	// ok

	if (!numLines) {
		return nullptr;
	}

	stringHeight = ((wordSpacingHeight + lineHeight + 2) * numLines) + 1;
	stringFinished = 0;
	stringWidth = rightBorder_X + 2;	// max render width to the right
	stringRenderBufferSize = stringWidth * stringHeight * 4;
	inRightBorder_X = rightBorder_X;

	currentStrRenderBuffer =
	    (uint8 *) mallocAndZero(stringRenderBufferSize);
	resetBitmap(currentStrRenderBuffer, stringRenderBufferSize);

	generatedGfxEntry = (gfxEntryStruct *) MemAlloc(sizeof(gfxEntryStruct));
	generatedGfxEntry->imagePtr = currentStrRenderBuffer;
	generatedGfxEntry->imageSize = stringRenderBufferSize / 2;
	generatedGfxEntry->fontIndex = fontFileIndex;
	generatedGfxEntry->height = stringHeight;
	generatedGfxEntry->width = stringWidth;	// maximum render width to the right

	// var_8 = 0;
	heightOffset = 0;

	const int16* fontCharTable = fontCharacterTable; // MOD: Cache

	do {
		int spacesCount = 0;	// si
		unsigned char character = *string;
		short int strPixelLength;	// var_16
		const char *ptrStringEnd;	// var_4     //ok
		int drawPosPixel_X;	// di

		// find first letter in string, skip all spaces
		while (character == ' ') {
			spacesCount++;
			character = *(string + spacesCount);
		}

		string += spacesCount;

		// returns character count and pixel length (via pointer) per line of the text string
		ptrStringEnd = string + prepareWordRender(inRightBorder_X, wordSpacingWidth, &strPixelLength, fontPtr_Desc, string);	//ok

		// determine how much space is left to the right and left (center text)
		if (inRightBorder_X > strPixelLength) {
			//var_8 = (inRightBorder_X - strPixelLength) / 2;
			drawPosPixel_X =
			    (inRightBorder_X - strPixelLength) / 2;
		} else {
			drawPosPixel_X = 0;
		}
		//drawPosPixel_X = var_8;

		// draw textline, character wise
		do {
			character = *(string++);

			int16 charData = fontCharTable[character]; // MOD: short int charData = fontCharacterTable[character];	// get character position

			if (character) {
				if (character == ' ' || character == 0x7C) {
					drawPosPixel_X += wordSpacingWidth + SPACE_WIDTH;	// if char = "space" adjust word starting postion (don't render space though);
				} else {
					if (charData >= 0) {
						const FontEntry &fe = fontPtr_Desc[charData];
                        assert(fe.offset < 0x20000); // MOD:
						// should ist be stringRenderBufferSize/2 for the second last param?
						renderWord((const uint8 *)fontPtr_Data + fe.offset,
						           currentStrRenderBuffer,
						           drawPosPixel_X,
						           fe.height2 - fe.charHeight +
						           lineHeight + heightOffset,
						           fe.charHeight,
							   fe.v1,
						           stringRenderBufferSize,
						           stringWidth,
								   (int16)fe.charWidth);

						drawPosPixel_X +=
						    wordSpacingWidth + (int16)fe.charWidth;
					}
				}
			} else {
				stringFinished = 1;	// character = 0x00
			}
		} while ((string < ptrStringEnd) && !stringFinished);

		// var_8 = 0;
		heightOffset += wordSpacingHeight + lineHeight;
	} while (!stringFinished);

	return generatedGfxEntry;
}

void freeGfx(gfxEntryStruct *pGfx) {
	if (pGfx->imagePtr) {
		MemFree(pGfx->imagePtr);
	}

	MemFree(pGfx);
}

void renderTextQuick(const char* string, uint8* buffer, uint32 x, uint32 y, uint32 stride) { // MOD:

	if (string == NULL)
		return;

	const FontInfo* fontPtr = (const FontInfo *)_systemFNT;
	const FontEntry* fontPtr_Desc = (const FontEntry *)((const uint8 *)fontPtr + sizeof(FontInfo));
	const uint8* fontPtr_Data = (const uint8 *)fontPtr + fontPtr->offset;
	const int16* charTable = fontCharacterTable;
	
	int32 lineHeight = getLineHeight(fontPtr->numChars, fontPtr_Desc);

	uint8* dst = buffer + x + ((y * stride));

	while(true) {
		char character = *(string)++;
		
		if (character < 32 || character >= 127)
			break;

		if (character == ' ') {
			dst += fontPtr_Desc->charWidth  + fontPtr->hSpacing;
			continue;
		}
		
		int16 charData = charTable[character];
		const FontEntry &fe = fontPtr_Desc[charData];
		
		const uint8 *fontPtr_Data1 = (const uint8 *) fontPtr_Data + fe.offset;
		const uint8 *fontPtr_Data2 = fontPtr_Data1 + lineHeight * 2;
		
		renderWord((const uint8 *)fontPtr_Data + fe.offset, 
			dst, 
			0, 
			fe.height2 - fe.charHeight + lineHeight + 0, 
			fe.charHeight,
			fe.v1, 
			0, 
			stride, 
			(int16) fe.charWidth);

		dst += fe.charWidth + fontPtr->hSpacing;
	}

}


} // End of namespace Cruise
