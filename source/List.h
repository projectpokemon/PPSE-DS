/*
 * List.h
 *
 *  Created on: Mar 7, 2010
 *      Author: Chase
 */

#ifndef LIST_H_
#define LIST_H_

enum {
	listHighlightColor = RGB5(24,24,24),
	listBorderColor = RGB5(15,15,15),
	listBGColor = RGB5(31,31,31),
};

class Entry {
public:
	char* string;
	uint16 color;
	Entry(const char* str, uint16 color) {
		int len = strlen(str)+1;
		this->string = (char*)malloc(len);
		memset(this->string,0,len);
		strcpy(this->string,str);
		this->color = color;
	}
	~Entry() {
		free(string);
		string = NULL;
	}
};

class List: public Widget {
protected:
	void (*function)(int32);
	FunctNumPtr functionoid;
	uint32 cursor;
	uint32 size;
	SliderV *slide;
	std::vector<Entry*> list;
	void _click(uint32 n);
public:
	List(int x, int y, int w, int lines);
	virtual ~List();

	virtual inline void setNumFunction(void (*funPtr)(int32)) { function = funPtr; }
	virtual inline void setNumFunctionoid(FunctNumPtr foidPtr) { functionoid = foidPtr; }
	inline virtual void getElement(char* str, uint32 n) const { strcpy(str,list[n]->string); }
	virtual void getCurrentElement(char *str) const { getElement(str,getCurrentNum()); }
	inline virtual int getCurrentNum() const { return cursor + slide->getWindowPosition(); }
	virtual void onClick(s16 x, s16 y);
	virtual void onControl(u32 keyPress);
	void addEntry(const char *str, uint16 Color);
	void clear();

	virtual void draw(int x, int y);
};

#endif /* LIST_H_ */
