/*! ----------------------------------------

 \file 		NDS_PPSEDS.c
 \author	Chase
 \version 	1.0
 \copyright Copyleft (c) 2009, Chase
 
 \brief 	This is a simple NDS ROM file  

-------------------------------------------- */

#include <nds.h>
#include <stdio.h>
#include <filesystem.h>
#include <unistd.h>
#include <maxmod9.h>

#define DEBUG 1

#include "Toolkit.h"

void argv_warn() {
	if(!NO_ARGV && !BAD_ARGV && !OLD_ARGV) return; //we're set
	ulxFirstPaletteTransparent = 0;

	u8 *data = loadFile("img/error.gif");
	ulxImage *error = ulxLoadImageGIF(data, ULX_PAL8);
	free(data);
	ulxTexture *errorTex = ulxCreateTexture(error);

	ulxSetTextColor(0);
	for(int i=0; i<256; ++i) {
		ulxStartFrame();
		ulxDrawTexture(0, 0, errorTex);
		ulxSyncDualFrame();
	}

	ulxDropTexture(errorTex);
	ulxDeleteImage(error);
	free(errorTex);
	ulxFirstPaletteTransparent = 1;
}

void splash() {
	//enum { CRC0 = 0x32C5, CRC1 = 0x6320 };
	ulxFirstPaletteTransparent = 0;

	u8 *data = loadFile("img/splash0.gif");
	/*
	{
		u16 crcTMP = swiCRC16(0,data,0x100);
		sassert(crcTMP == CRC0, "CRC for splash0 does not match!\n\nEither you're trying to hack.\nPoorly I might add, or Chase\nforgot to update the CRC.");
	} // */
	ulxImage *top = ulxLoadImageGIF(data, ULX_PAL8);
	free(data);
	data = loadFile("img/splash1.gif");
	/*
	{
		u16 crcTMP = swiCRC16(0,data,0x100);
		sassert(crcTMP == CRC1, "CRC for splash1 does not match!\n\nEither you're trying to hack.\nPoorly I might add, or Chase\nforgot to update the CRC.");
	} // */
	ulxImage *bottom = ulxLoadImageGIF(data, ULX_PAL8);
	free(data);
	ulxTexture *tTop = ulxCreateTexture(top);
	ulxTexture *tBottom = ulxCreateTexture(bottom);
	for(int i=0; i<64; ++i) {
		ulxStartFrame();
		if(i<32) {
			glPolyFmt(POLY_ALPHA(((i*2)>>1)) | POLY_CULL_NONE);
		}

		if(64-i < 32) {
			glPolyFmt(POLY_ALPHA(((64-i*2)>>1)) | POLY_CULL_NONE);
		}
		if(ulxDrawMain()) {
			ulxDrawTexture(0, 0, tTop);
		} else {
			ulxDrawTexture(0, 0, tBottom);
		}
		ulxSyncDualFrame();

		if(i == 33) {
			while(true) {
				swiWaitForVBlank();
				scanKeys();
				if(keysHeld()) {
					break;
				}
			}
		}
	}
	ulxDropTexture(tBottom);
	ulxDropTexture(tTop);
	ulxDeleteImage(bottom);
	ulxDeleteImage(top);
	free(tBottom);
	free(tTop);

	ulxFirstPaletteTransparent = 1;
}

void fontSetup() {
		u8 *data = loadFile("msgothic.fnt");
		BMF *bFont = ulxLoadFont(data,file_size);
		free(data);
		data = loadFile(bFont->pageName[0]);
		ulxImage *img = ulxLoadImageGIF(data, ULX_PAL2);
		free(data);
		ulxTexture *bFontTex = ulxCreateTexture(img);
		ulxSetFont(bFont,bFontTex);
}

void initialize() {
	setupArgV();

	sassert(nitroFSInit() == 1,"NitroFS Init Failed!\n\n"
			"Please be sure DLDI is done.\n"
			"Place "APP_ARTIFACT" into root.\n\n"
			"Root is often C:\\ D:\\ E:\\ etc.\n\n\n"
			"For DLDI Drivers visit:\nhttp://dldi.drunkencoders.com\n\n"
			"PLEASE USE YOUR BRAIN");
	chdir("nitro:/");

	ulxInit();
	ulxInitDual();
#ifndef DEBUG
	argv_warn();
#endif
	splash();

	{ //Clear the VRAM, without this there is a blue band before the splash screens, due to the above stuff
		u32 tmp = vramSetMainBanks(VRAM_A_LCD,VRAM_B_LCD,VRAM_C_LCD,VRAM_D_LCD);
		dmaFillWords(0, VRAM_A, 512*1024);
		vramRestoreMainBanks(tmp);
	}

	fontSetup();

}

Image *top;
Image *bottom;

Label *sn = NULL;

bool isPlaying = true;
int current_play = 0;

//void play_next() {
//	mmStop();
//	mmUnload(current_play);
//	current_play++;
//	if(current_play > 4) current_play = 0;
//	mmLoad(current_play);
//	mmStart(current_play,MM_PLAY_LOOP);
//}
//
//void play_prev() {
//	mmStop();
//	mmUnload(current_play);
//	current_play--;
//	if(current_play < 0) current_play = 4;
//	mmLoad(current_play);
//	mmStart(current_play,MM_PLAY_LOOP);
//}

//#define S_BANK_CNT(x) (*(vu32*)(0x4000400+(x<<4))

int main( void ) {
	//read register
	initialize();
	chdir("nitro:/");

	{
		ulxFirstPaletteTransparent = 0;
		u8 *data = loadFile("img/bglogo.gif");
		ulxImage *itop = ulxLoadImageGIF(data, ULX_PAL8);
		free(data);
		data = loadFile("img/bgmain.gif");
		ulxImage *ibottom = ulxLoadImageGIF(data, ULX_PAL8);
		free(data);

		top = new Image(0,0,256,192,itop);
		bottom = new Image(0,0,256,192,ibottom);
	}

	//Fade in menu
	for(int i=0; i<33; ++i) {
		ulxStartFrame();
		if(i<32) {
			glPolyFmt(POLY_ALPHA(((i*2)>>1)) | POLY_CULL_NONE);
		}
		if(ulxDrawMain()) {
			top->draw(0,0);
		} else {
			bottom->draw(0,0);
		}
		ulxSyncDualFrame();
	}
	//End Fade in

	Screen *s = new MainMenu(top,bottom);

//	mmInitDefault((char*)"nitro:/soundbank.bin");
//	mmSelectMode(MM_MODE_A);
//
//	mmLoad(current_play);
//	mmStart(current_play,MM_PLAY_LOOP);

	//u32 totalFrame = 0;
	bool rendered = false;
	bool isDragging = false;

	//this strange arrangement allows the catch to be outside the execution loop
	//so it doesn't need to be called constantly, but it allows exceptions to be
	//caught without having to rewrite the loop or without having to constantly
	//enter a try block with every loop.
	while(true) {
		try {
			while(true) {
				if(ulxDrawMain()) {
					ulxStartFrame();

					//top->draw(0,0);
					s->drawTop();

					ulxSyncFrame();
					ulxDrawDual();
					rendered = true;
				} else {
					if( s->bottom->checkInvalidated() ) {

						ulxStartFrame();

						//bottom->draw(0,0);
						s->drawBottom();

						ulxSyncFrame();
						ulxDrawDual();
						rendered = false;
					}
				}

//				if( !s->bottom->checkInvalidated() ) {
//				//&& !s->top->checkInvalidated() ) {
//					s->bottom->doUpdate();
//					//s->top->doUpdate();
//				}

				if(rendered) {
					swiIntrWait(1,IRQ_KEYS|IRQ_FIFO_NOT_EMPTY);

					//DO SCREEN CHANGE IF NEEDED
					if(s->changeScreen()) {
						Screen * last = s;
						s = s->nextScreen();
						s->previous(last);
						last = NULL;
					}

					//Do event stuff
					scanKeys();

					u32 down = keysDown();

					s->top->onControl(down);
					s->bottom->onControl(down);

					u32 held = keysHeld();
					touchPosition touch;
					touchRead(&touch);
					if(down & KEY_TOUCH) {
						s->bottom->onClick(touch.px,touch.py);
					} else if(held & KEY_TOUCH) {
						s->bottom->onDrag(touch.px,touch.py);
						isDragging = true;
					} else {
						//run stop drag here
						if(isDragging == true) {
							s->bottom->onDragEnd();
							isDragging = false;
						}

					}
				}
			}
		} catch(...) {
			rendered = false;
		}
	}

	return 0;
}
