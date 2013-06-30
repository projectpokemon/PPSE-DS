/*
 * Kanapad.h
 *
 *  Created on: 2010/01/27
 *      Author: Chase
 */

#ifndef KANAPAD_H_
#define KANAPAD_H_

#include "Funct.h"

class Kanapad : public Widget {
friend class KanaPadFunct;
friend class KanaPadChangeFunct;
protected:
//	bool _changeBoard;
//	s8 _targetBoard;
	s8 _cBoard;
	Widget *board[4];
	FunctKeyPtr retFunoid;
	void (*retFun)(const char * str);
	void kpress(const char * str);
public:
	Kanapad(int x, int y);
	virtual ~Kanapad();
	void setCallbackFunction(void (*funPtr)(const char * str)) { retFun = funPtr; }
	inline void setCallbackFunctionoid(FunctKeyPtr funPtr) { retFunoid = funPtr; }
	static void addMaru(char * str);
	static void addTenten(char * str);
};

#endif /* KANAPAD_H_ */
