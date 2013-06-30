/*
 * PokeBrowseMenu.h
 *
 *  Created on: Mar 10, 2010
 *      Author: Chase
 */

#ifndef POKEBROWSEMENU_H_
#define POKEBROWSEMENU_H_

class Icon;
class DragIcon;

class PokeBrowseMenu: public Screen {
public:
	OptionMenu*menu;
	std::vector<Icon*> icons;
	std::vector<DragIcon*> display_box;
	std::vector<DragIcon*> display_party;
	PokeLib::Box *pkBox;
	PokeLib::Party *pkPty;
	DragIcon *isDrag;
	Label *boxName;
	SpritePKM *sprite;
	s8 box_num;

	PokeBrowseMenu(OptionMenu*m);
	virtual ~PokeBrowseMenu();

	Icon *getIcon(int num);
	void loadBox();
	void switchPokemon(s16 box1, s16 pos1, s16 box2, s16 pos2);

	virtual bool changeScreen();
	void inline setReturn() { nextScreenCheck = true; }
	virtual Screen *nextScreen() { return menu; }
};

#endif /* POKEBROWSEMENU_H_ */
