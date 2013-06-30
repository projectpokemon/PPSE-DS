/*
 * FileList.h
 *
 *  Created on: Mar 8, 2010
 *      Author: Chase
 */

#ifndef FILELIST_H_
#define FILELIST_H_

#include <unistd.h>

class FileList: public List {
private:
	//char *_cdir;'
	char _cdir[MAXPATHLEN];
	uint16 dir_num;
public:
	FileList(int x, int y, int w, int lines);
	virtual ~FileList();
	virtual void onControl(u32 keyPress);
	virtual void onClick(s16 x, s16 y);
	void changeDirectory(const char* dir);
	void setDirectory(const char* dir);
	void loadDirectory();
	inline bool isDirectory(int n) {
		if(n < dir_num) return true; return false;
	}
	inline void getDirectory(char *str) const {
		if(_cdir) strcpy(str, _cdir);
	}
	virtual void draw(int x, int y);
};

#endif /* FILELIST_H_ */
