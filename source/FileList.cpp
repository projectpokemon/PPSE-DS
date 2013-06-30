/*
 * FileList.cpp
 *
 *  Created on: Mar 8, 2010
 *      Author: Chase
 */

#include <dirent.h>
#include <cstdio>
#include <nds.h>
#include <vector>
#include "Widget.h"
#include <ulx.h>
#include "Funct.h"
#include "SliderV.h"
#include "List.h"
#include "FileList.h"

FileList::FileList(int x, int y, int w, int lines) : List(x,y,w,lines) {
	memset(_cdir,0,MAXPATHLEN);
	//_cdir = (char*)calloc(MAXPATHLEN,1);
}

FileList::~FileList() {
	//free(_cdir);
	//_cdir = NULL;
}

void FileList::changeDirectory(const char *dir) {
	chdir(dir);
	getcwd(_cdir, MAXPATHLEN);
	cursor = 0;
	loadDirectory();
	this->isInvalidated = true;
}

void FileList::setDirectory(const char*dir) {
	chdir(dir);
	strcpy(_cdir,dir);
}

int strcompare (const void * a, const void * b) {
  return strcmp((char*)a,(char*)b);
}

void FileList::onControl(u32 keyPress) {
	if(!isActive) return;
	if(keyPress & KEY_A) {
		uint32 pos = slide->getWindowPosition();
		if(pos + cursor < dir_num) {
			changeDirectory(list[pos + cursor]->string);
		}
		return;
	}
	List::onControl(keyPress);
}

void FileList::onClick(s16 x, s16 y) {
	if(!checkBounds(x,y) || !isActive) return;
	//Make sure it is only in our list window...
	if(x < _x+_w-32) {
		uint32 tmp = (y-_y)/12;
		if(tmp == cursor) {
			uint32 pos = slide->getWindowPosition();
			if(pos + cursor < dir_num) {
				changeDirectory(list[pos + cursor]->string);
			}
			return;
		}
		cursor = tmp;
		this->isInvalidated = true;
		return;
	}
	Widget::onClick(x,y);
}

void FileList::loadDirectory() {
	//struct stat st;
	//char filename[MAXPATHLEN];

	//DIR_ITER* dir = diropen (_cdir);
	DIR* dir = opendir(_cdir);

	if(dir == NULL) return;

	//in total, 32 KB (which is acceptable)
	char *directory = (char*)calloc(128,128);
	char *file = (char*)calloc(512,128);

	dir_num = 0;
	uint16 file_num = 0;
	
	while(true) {
			struct dirent* pent = readdir(dir);
			if(pent == NULL) break;
			
			if(pent->d_type == DT_DIR) {
				if(strcmp(pent->d_name,".") != 0)
					strncpy(&directory[128*dir_num++],pent->d_name,127);
			} else {
				strncpy(&file[128*file_num++],pent->d_name,127);
			}
	}
	
	/*
	while (nextdir(dir, filename, &st) == 0) {
		if(st.st_mode & S_IFDIR) {
			
		} else {
			strncpy(&file[128*file_num++],filename,127);
		}
	}
	*/

	//SORT THEM ALPHABETICALLY
	qsort(directory, dir_num, 128, strcompare);
	qsort(file, file_num, 128, strcompare);

	this->clear();
	for(uint16 i=0;i<dir_num;++i) {
		//siprintf(filename,"[%s]",&directory[128*i]);
		this->addEntry(&directory[128*i],0);
	}
	for(uint16 i=0;i<file_num;++i) {
		//siprintf(filename,"[%s]",directories[i]);
		this->addEntry(&file[128*i],0);
	}

	free(directory);
	free(file);
}

void FileList::draw(int x, int y) {
	if(!isVisible) return;

	//draw our window
	ulxDrawFillRect(x+_x, y+_y, x+_x+_w-32, y+_y+_h, listBGColor);
	ulxDrawRect(x+_x, y+_y, x+_x+_w-32, y+_y+_h, listBorderColor);

	//Draw the current list
	char tmp[128];
	uint32 pos = slide->getWindowPosition();
	for(uint32 i=0; i<list.size() && i < size; ++i) {
		if(i == cursor) {
			ulxDrawFillRect(x+_x+1, y+_y+1+i*12, x+_x+_w-33, y+_y+13+i*12, listHighlightColor);
		}
		ulxSetTextColor(list[i+pos]->color);
		if(i+pos < dir_num) {
			siprintf(tmp,"[%s]",list[i+pos]->string);
			ulxDrawString(x+_x+2,y+_y+2+i*12, tmp);
		} else {
			ulxDrawString(x+_x+2,y+_y+2+i*12, list[i+pos]->string);
		}
	}

	Widget::draw(x,y);
}
