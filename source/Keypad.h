/*
 * Keypad.h
 *
 *  Created on: 2010/01/27
 *      Author: Chase
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "Funct.h"

class Keypad : public Widget {
friend class KeyPadFunct;
friend class KeyPadShiftFunct;
friend class KeyPadSpecialFunct;
protected:
	bool _isShift;
	bool _isCapLock;
	bool _unsetShift;
	FunctKeyPtr retFunoid;
	void (*retFun)(const char * str);
	void kpress(const char * str);
	Widget *lower;
	Widget *upper;
public:
	Keypad(int x, int y);
	virtual ~Keypad();
	void setCallbackFunction(void (*funPtr)(const char * str)) { retFun = funPtr; }
	inline void setCallbackFunctionoid(FunctKeyPtr funPtr) { retFunoid = funPtr; }

	virtual bool checkInvalidated();
//	virtual void onClick(s16 x, s16 y);
	virtual void onDrag(s16 x, s16 y) { ; }
//	virtual void onControl(u32 keyPress);
//	virtual void draw(int x, int y);
};

#endif /* KEYPAD_H_ */
