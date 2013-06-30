/*
 * ItemEditMenu.cpp
 *
 *  Created on: Mar 11, 2010
 *      Author: Chase
 */

#include <cstdio>
#include "Toolkit.h"


class ReturnFunct : public Funct {
	ItemEditMenu* menu;
public:
	ReturnFunct(ItemEditMenu* m) {
		menu = m;
	}
	virtual void execute() {
		menu->setReturn();
	}
};

ItemEditMenu::ItemEditMenu(OptionMenu*m) : Screen() {
	menu = m;
	this->setTop(m->top);
	this->setBottom(m->top);


	Label *label = new Label(0,8,256,16,"Item Editor");
	label->setColor(0);
	label->setAlignment(ALIGN_CENTER);
	bottom->addChild(label);

	Button *b = new Button(8,192-16-8,256-16,16,"Back to Menu");
	b->setFunctionoid(new ReturnFunct(this));
	bottom->addChild(b);

}

ItemEditMenu::~ItemEditMenu() {
	// TODO Make destructor
}
