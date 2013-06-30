/*
 * Slider.h
 *
 *  Created on: Nov 23, 2009
 *      Author: Chase
 */

#ifndef SLIDER_V_H_
#define SLIDER_V_H_

#include "Funct.h"

enum {
	sliderBorderColor = RGB5(15,15,15),
	sliderBGColor = RGB5(31,31,31),
	sliderInactiveGripColor = RGB5(18,18,18),
	sliderActiveGripColor = RGB5(22,22,22),

	sliderMinGripLength = 16,
};

class SliderV: public Widget {
protected:
	int _trackLength;
	int _gripLength;
	int _gripPosition;
	int _wLength;
	int _dLength;
	int _scrollSize;
	int windowPosition;

	FunctNumPtr sFunctionoid;
	void (*scrollFunPtr)(u16);

	void _calcGripLength();
	void _calcGripPosition();

	void _ret();
public:
	SliderV(int x, int y, int w, int h);
	virtual ~SliderV();

	void setWindowLength(int n);
	void setDocumentLength(int n);
	void setWindowPosition(int n);
	inline int getWindowPosition() { return windowPosition; }
	bool checkGripBounds(int x, int y);
	virtual void onDrag(s16 x, s16 y);
	virtual void onClick(s16 x, s16 y);
	virtual void onControl(u32 keyPress);
	virtual void draw(int x, int y);
	//called when the scroll is moved
	virtual void setFunction(void (*funPtr)(u16 position)) { scrollFunPtr = funPtr; }
	virtual void setFunctionoid(FunctNumPtr funPtr) { sFunctionoid = funPtr; }
	//no children
	inline void addChild(Widget*w) {}
};

#endif /* SLIDER_V_H_ */
