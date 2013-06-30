/*
 * TrainerEditMenu.cpp
 *
 *  Created on: Mar 13, 2010
 *      Author: Chase
 */

#include <cstdio>
#include "Toolkit.h"

class ReturnFunct : public Funct {
	TrainerEditMenu* menu;
public:
	ReturnFunct(TrainerEditMenu* m) {
		menu = m;
	}
	virtual void execute() {
		menu->setReturn();
	}
};

class BadgeEditFunct : public FunctNum {
public:
	PokeLib::Badge *badge;
	int slot;
	BadgeEditFunct(PokeLib::Badge*bdg, u8 slot) {
		this->badge = bdg;
		this->slot = slot;
	}
	virtual void execute(int32 n) {
		if(n == 1) {
			*((u8*)badge) |= (1 << slot);
		} else {
			*((u8*)badge) &= ~(1 << slot);
		}
	}

};

TrainerEditMenu::TrainerEditMenu(OptionMenu*m) : Screen() {
	menu = m;
	this->setTop(m->top);
	this->setBottom(m->top);

	train = new PokeLib::Trainer(m->save);

	Label *label = new Label(0,8,256,16,"Trainer Editor");
	label->setColor(0);
	label->setAlignment(ALIGN_CENTER);
	bottom->addChild(label);

	//

	if(menu->save->getSaveType() == PokeLib::HGSS) {
		label = new Label(128-8,64,128,12,"Kanto Badges");
		label->setAlignment(ALIGN_CENTER);
		bottom->addChild(label);
		//Add toggle buttons
		for(int i = 0; i<8; ++i) {
			char tmp[8]; memset(tmp,0,8);
			siprintf(tmp,"%i",i);
			ToggleButton *tb = new ToggleButton(128+16*i-8,64+12,16,24,tmp);
			if(*((u8*)(&train->badgeAlt)) & (1 << i)) tb->setPressed(true);
			tb->setNumFunctionoid(new BadgeEditFunct(&train->badgeAlt,i));
			bottom->addChild(tb);
		}

		label = new Label(128-8,24,128,12,"Johto Badges");
		label->setAlignment(ALIGN_CENTER);
		bottom->addChild(label);
		//Add toggle buttons
		for(int i = 0; i<8; ++i) {
			char tmp[8]; memset(tmp,0,8);
			siprintf(tmp,"%i",i);
			ToggleButton *tb = new ToggleButton(128+16*i-8,24+12,16,24,tmp);
			if(*((u8*)(&train->badgeMain)) & (1 << i)) tb->setPressed(true);
			tb->setNumFunctionoid(new BadgeEditFunct(&train->badgeMain,i));
			bottom->addChild(tb);
		}

	} else {
		label = new Label(128-8,24,128,12,"Sinnoh Badges");
		label->setAlignment(ALIGN_CENTER);
		bottom->addChild(label);
		//Add toggle buttons
		for(int i = 0; i<8; ++i) {
			char tmp[8]; memset(tmp,0,8);
			siprintf(tmp,"%i",i);
			ToggleButton *tb = new ToggleButton(128+16*i-8,24+12,16,24,tmp);
			if(*((u8*)(&train->badgeMain)) & (1 << i)) tb->setPressed(true);
			tb->setNumFunctionoid(new BadgeEditFunct(&train->badgeMain,i));
			bottom->addChild(tb);
		}
	}

	Button *b = new Button(8,192-16-8,256-16,16,"Back to Menu");
	b->setFunctionoid(new ReturnFunct(this));
	bottom->addChild(b);
}

TrainerEditMenu::~TrainerEditMenu() {
	delete train;
}

Screen *TrainerEditMenu::nextScreen() {
	train->saveInto(menu->save);
	return menu;
}

