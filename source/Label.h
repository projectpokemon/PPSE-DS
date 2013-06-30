/*
 * Label.h
 *
 *  Created on: Nov 11, 2009
 *      Author: Chase
 */

#ifndef LABEL_H_
#define LABEL_H_

enum Alignment {
	ALIGN_LEFT = 0,
	ALIGN_CENTER = 1,
	ALIGN_RIGHT = 2,
};

enum {
	labelBorderColor = RGB5(15,15,15),
	labelBackgroundColor = RGB5(31,31,31),
};

class Label : public Widget {
protected:
	int _align;
	int _slen;
	char * _str;
	rgb _color;
	rgb _border;
	rgb _background;
	bool _colorSet;
	bool _backgroundSet;
	bool _borderSet;
	bool _wordWrap;
public:
	Label(int x, int y, int w, int h, const char *str);
	Label(int x, int y, int w, int h, const char *str, bool wrap);
	virtual ~Label();
	void setColor(rgb color);
	void setBackgroundColor(rgb color);
	void setBorderColor(rgb color);
	void setText(const char *str);
	void setBackground(bool visible) { _backgroundSet = visible; }
	void setBorder(bool visible)  { _borderSet = visible; }
	void setWrap(bool wrap) { _wordWrap = wrap; }
	virtual void draw(int x, int y);
	void setAlignment(Alignment a);
};

#endif /* LABEL_H_ */
