/*
 * Label.h
 *
 *  Created on: Nov 11, 2009
 *      Author: Chase
 */

#ifndef NUMPAD_H_
#define NUMPAD_H_

#include "Funct.h"

class Numpad : public Widget {
friend class NumPadFunct;
protected:
	FunctNumPtr retFunoid;
	void (*retFun)(s16 n);
	void kpress(const char * str);
public:
	Numpad(int x, int y, int w, int h);
	virtual ~Numpad();
	//These will supply the number pressed OR -1 if backspace
	void setCallbackFunction(void (*funPtr)(s16 n)) { retFun = funPtr; }
	inline void setCallbackFunctionoid(FunctNumPtr funPtr) { retFunoid = funPtr; }
};

#endif /* NUMPAD_H_ */
