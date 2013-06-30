/*
 * Toolkit.cpp
 *
 *  Created on: 2010/02/04
 *      Author: Chase
 */

#include <nds.h>
#include <stdio.h>
#include <unistd.h>
#include <fat.h>
#include <ulx.h>
#include <errno.h>
#include "Toolkit.h"
#include <sys/stat.h>

bool NOCASH = false;


size_t file_size;

u8 * loadFile(const char* filename) {
//	size_t filesize = 0;
	{
		struct stat results;
		stat(filename, &results);// == 0
		file_size = results.st_size;

		if(file_size >= 1024*1024*2) {
			char tmp[128];
			siprintf(tmp,"Error!\n File is too large to load.\n Please use segment loader.\n"
					"File: %s",filename);
			sassert(file_size < 1024*1024*2,tmp);
		}
	}

	FILE * fp = fopen(filename,"rb");

	if(fp == NULL) {
		//Try again!
		//fp = fopen(filename,"rb");

		char tmp[128];
		siprintf(tmp,"File failed to load!\nFile: %s",filename);
		sassert(fp != NULL,tmp);
	}

	u8 *data = (u8*)malloc(file_size*sizeof(u8));
	sassert(data != NULL,"Out of memory!");

	//read data to memory
	size_t sz = fread(data,1,file_size,fp);
	sassert(sz == file_size,"Reading Failed!");
	fclose(fp);

	return data;
//	return loadFileX(filename);
}

void loadFile(const char* filename, u8 * data, uint32 len) {
	{
		struct stat results;
		stat(filename, &results);// == 0
		file_size = results.st_size;

		if(file_size >= 1024*1024*2) {
			char tmp[128];
			siprintf(tmp,"Error!\n File is too large to load.\n Please don't be stupid.\n"
					"File: %s",filename);
			sassert(file_size < 1024*1024*2,"Error!\n File is too large to load.\n Please use segment loader.\n");
		}
	}

	FILE * fp = fopen(filename,"rb");

	if(fp == NULL) {
		char tmp[128];
		siprintf(tmp,"File failed to load!\nFile: %s",filename);
		sassert(fp != NULL,tmp);
	}

	uint32 toRead = file_size;
	if(len < file_size) toRead = len;
	size_t sz = fread(data,1,toRead,fp);
	sassert(sz == toRead,"Reading Failed!");
	fclose(fp);
}


void saveFile(const char* filename, u8*data, uint32 len) {
	FILE * fp = fopen(filename,"wb");
	if(fp == NULL) {
		char tmp[128];
		siprintf(tmp,"File failed to load!\nFile: %s",filename);
		sassert(fp != NULL,tmp);
	}

	file_size = fwrite (data, 1, len, fp);

	fclose(fp);
}

//According to WinterMute, a bunch of ugly code that shouldn't have to exist, and I do agree with
//him, but its needed to work around how fail cart makers are with argv.
bool OLD_ARGV = false;
bool BAD_ARGV = false;
bool NO_ARGV = false;
void setupArgV() {
	//Return if we are on an emulator!
	if (memcmp(&__NDSHeader->filenameOffset, &((tNDSHeader *)0x08000000)->filenameOffset, 16) == 0 ) {
		NOCASH = true;
		//Kill argv for desmume compatibility
		__system_argv->argvMagic = 0;
		return;
	}

	//Let use make an updated version of this now shall we?
	struct __argv *newArg = ((struct __argv*)0x02FFFE70);
	struct __argv *oldArg = ((struct __argv*)0x027FFF70);

	char nCommandline[MAXPATHLEN];
	memset(nCommandline,0,MAXPATHLEN);

	if(newArg->argvMagic == ARGV_MAGIC) {
		//It supports the new argv, good!
		if(strncmp(newArg->argv[0],"fat",3) == 0) {
			//Everything is fine here, move along, nothing to see here.
			return;
		}

		BAD_ARGV = true;
		if(newArg->commandLine[0] != '/') {
			siprintf(nCommandline,"fat:%s",newArg->commandLine);
		} else {
			siprintf(nCommandline,"fat:/%s",newArg->commandLine);
		}
		newArg->length = strlen(nCommandline);
		newArg->commandLine = (char*)malloc(newArg->length);
		strcpy(newArg->commandLine,nCommandline);
		newArg->argc = 1;
		newArg->argv = &newArg->commandLine;

		return;
	} else if(oldArg->argvMagic == ARGV_MAGIC) {
		OLD_ARGV = true;
		//It uses the old argv, booo!
		newArg->argvMagic = ARGV_MAGIC;
		if(strncmp(oldArg->argv[0],"fat",3) == 0) {
			//Copy everything over and get on with life.
			newArg->argc = oldArg->argc;
			newArg->length = oldArg->length;
			newArg->commandLine = oldArg->commandLine;
			newArg->argv = oldArg->argv;
			return;
		}

		BAD_ARGV = true;
		//Otherwise... ugh
		if(oldArg->commandLine[0] != '/') {
			siprintf(nCommandline,"fat:%s",oldArg->commandLine);
		} else {
			siprintf(nCommandline,"fat:/%s",oldArg->commandLine);
		}
		newArg->length = strlen(nCommandline);
		newArg->commandLine = (char*)malloc(newArg->length);
		strcpy(newArg->commandLine,nCommandline);
		newArg->argc = 1;
		newArg->argv = &newArg->commandLine;

		return;
	}

	NO_ARGV = true;
	//Otherwise... oi, try compatibility
	//this is DANGEROUS (be sure to add extra checks!)
	siprintf(nCommandline, "fat:/"APP_ARTIFACT);
	newArg->argvMagic = ARGV_MAGIC;
	newArg->length = strlen(nCommandline);
	newArg->commandLine = (char*)malloc(newArg->length);
	strcpy(newArg->commandLine,nCommandline);
	newArg->argc = 1;
	newArg->argv = &newArg->commandLine;
}

int resize(int k) {
	int i = 0;
	for(i=0;i<32;++i) if(k<(1<<i)) return (1<<i);
	return 0;
}

ulxImage *forceSpriteResize(ulxImage* tmp) {
	/* Create the image for the bitmap */
	ulxImage *img = (ulxImage*)malloc(sizeof(ulxImage));
	if(img == NULL) {
		return NULL;
	}
	memset(img,0,sizeof(ulxImage));
	img->width = resize(tmp->width);
	//img->height = resize(tmp->height);
	img->height = tmp->height;
	img->format = tmp->format;
	u8 tmpPixelWidth[]    = {0, 8, 2, 4, 8, 0, 8,16,16};
	u16 len = (tmpPixelWidth[img->format]*img->width*img->height)>>3;
	u16 nLine = (tmpPixelWidth[img->format]*img->width)>>3;
	u16 oLine = (tmpPixelWidth[img->format]*tmp->width)>>3;
	img->image = malloc(len);
	if(img->image == NULL) {
		free(img); return NULL;
	}
	img->paletteSize = tmp->paletteSize;
	//this way we don't need to do any copying of palette data
	img->palette = tmp->palette;
	tmp->palette = NULL;

	//fill the image with zero
	memset(img->image, 0, len);
	//now overlay the old image
	for(int i = 0; i < tmp->height; ++i) {
		memcpy(((u8*)img->image) + i*nLine, ((u8*)tmp->image) + i*oLine, oLine);
	}
	//delete the old image
	ulxDeleteImage(tmp);

	return img;
}
