/*
 * Toolkit.h
 *
 *  Created on: Nov 11, 2009
 *      Author: Chase
 */

#ifndef __TOOLKIT_H_
#define __TOOLKIT_H_


#define APP_STRING	"PPSEDS"
#define APP_VERSION	"r11"
#define APP_ARTIFACT	"PPSEDS.nds"


#define VERSION_COLOR	RGB8(128,40,17)

#include <pokelib/pokelib.h>
#include <nds.h>
#include <vector>
#include <ulx.h>
#include "Widget.h"
#include "Image.h"
#include "Label.h"
#include "Button.h"
#include "SliderV.h"
#include "ButtonKey.h"
#include "ToggleButton.h"
#include "List.h"
#include "FileList.h"
#include "Screen.h"
#include "Numpad.h"
#include "Keypad.h"
#include "Kanapad.h"

#include "SpritePKM.h"
#include "MainMenu.h"
#include "OptionMenu.h"
#include "ItemEditMenu.h"
#include "PokeBrowseMenu.h"
#include "DexEditMenu.h"
#include "TrainerEditMenu.h"

extern bool NOCASH;
extern bool OLD_ARGV;
extern bool BAD_ARGV;
extern bool NO_ARGV;
extern size_t file_size;

/* Functions */
u8 * loadFile(const char* filename);
void loadFile(const char* filename, u8 * data, uint32 len);

void saveFile(const char* filename, u8 * data, uint32 len);

//u8 * loadFileX(const char* filename);
ulxImage *forceSpriteResize(ulxImage* tmp);
void setupArgV();

#endif /* __TOOLKIT_H_ */
