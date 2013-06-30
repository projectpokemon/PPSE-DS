/*
 * OptionMenu.h
 *
 *  Created on: Mar 10, 2010
 *      Author: Chase
 */

#ifndef OPTIONMENU_H_
#define OPTIONMENU_H_

class OptionMenu: public Screen {
	friend class SaveFunct;
	Screen *sub;
	char* filename;
	bool success;
	Widget *buttons;
	Widget *verify;
	Label *status;
	int next;
public:
	PokeLib::Save *save;
	FunctPtr update;
	OptionMenu(MainMenu *mm, const char *file);
	virtual ~OptionMenu();

	void submenu(int n);

	virtual Screen *nextScreen();
	virtual void previous(Screen *n);
};

#endif /* OPTIONMENU_H_ */
