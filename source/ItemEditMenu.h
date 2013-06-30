/*
 * ItemEditMenu.h
 *
 *  Created on: Mar 11, 2010
 *      Author: Chase
 */

#ifndef ITEMEDITMENU_H_
#define ITEMEDITMENU_H_

class ItemEditMenu: public Screen {
public:
	OptionMenu*menu;
	ItemEditMenu(OptionMenu*m);
	virtual ~ItemEditMenu();

	void inline setReturn() { nextScreenCheck = true; }
	virtual Screen *nextScreen() { return menu; }
};

#endif /* ITEMEDITMENU_H_ */
