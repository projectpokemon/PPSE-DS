/*
 * MainMenu.h
 *
 *  Created on: Mar 7, 2010
 *      Author: Chase
 */

#ifndef MAINMENU_H_
#define MAINMENU_H_

class MainMenu: public Screen {
	friend class OptionMenu;
	friend class MainMenuFunct;
	friend class LoadFunct;
	friend class AboutMenuFunct;

	PokeLib::Save *save;
	//friend class LabelDirFunct;
	Button *b;
	FileList *list;
	Label *lab;
	Label *status;
	Button *lbtn;

	Image *sleepy;
	Button *about;
public:
	MainMenu(Image *main, Image *sub);
	virtual ~MainMenu();
	virtual void drawTop();
	virtual void drawBottom();
	void doload();

	virtual Screen *nextScreen();
	virtual void previous(Screen *n) { }
};

#endif /* MAINMENU_H_ */
