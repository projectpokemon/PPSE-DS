/*
 * ItemEditMenu.h
 *
 *  Created on: Mar 11, 2010
 *      Author: Chase
 */

#ifndef DEXEDITMENU_H_
#define DEXEDITMENU_H_

class DexEditMenu: public Screen {
public:
	OptionMenu*menu;
	DexEditMenu(OptionMenu*m);
	virtual ~DexEditMenu();

	void inline setReturn() { nextScreenCheck = true; }
	virtual Screen *nextScreen() { return menu; }
};

#endif /* DEXEDITMENU_H_ */
