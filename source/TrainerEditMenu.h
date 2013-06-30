/*
 * TrainerEditMenu.h
 *
 *  Created on: Mar 13, 2010
 *      Author: Chase
 */

#ifndef TRAINEREDITMENU_H_
#define TRAINEREDITMENU_H_

class TrainerEditMenu: public Screen {
public:
	PokeLib::Trainer *train;
	OptionMenu*menu;
	TrainerEditMenu(OptionMenu*m);
	virtual ~TrainerEditMenu();

	inline void setReturn() { nextScreenCheck = true; }
	virtual Screen *nextScreen(); // { return menu; }
};

#endif /* TRAINEREDITMENU_H_ */
