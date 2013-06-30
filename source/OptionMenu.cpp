/*
 * OptionMenu.cpp
 *
 *  Created on: Mar 10, 2010
 *      Author: Chase
 */

#include <cstdio>
#include "Toolkit.h"

class UpdateFunct : public Funct {
public:
	PokeLib::Save *save;
	Label *checksumA;
	Label *checksumB;
	Label *trainer;
	Label *rival;

	void execute() {
		using namespace PokeLib;
		u16 A = save->calcBlockAChecksum();
		u16 B = save->calcBlockBChecksum();
		char tmp[64];
		siprintf(tmp,"Small: 0x%04X",A);
		checksumA->setText(tmp);
		siprintf(tmp,"Big:   0x%04X",B);
		checksumB->setText(tmp);

		save->repairBlockAChecksum();
		save->repairBlockBChecksum();

		{

			Trainer t(save);
			widetext wide = t.getName();
			char tmp2[32];
			memset(tmp2,0,32);
			wcstombs (tmp2, wide.c_str(), 32);
			siprintf(tmp,"Trainer Name: %s", tmp2);
			trainer->setText(tmp);

			wide = t.getRivalName();
			wcstombs (tmp2, wide.c_str(), 32);
			siprintf(tmp,"Rival Name: %s", tmp2);
			rival->setText(tmp);
		}
	}
};

class SaveFunct : public Funct {
	OptionMenu *menu;
	u8 stage;
	//u32 update;
public:
	SaveFunct(OptionMenu *m, u8 stage) {
		this->menu = m;
		this->stage = stage;
	}
	virtual void execute() {
		if(stage == 1) {
			//this->menu->buttons->setActive(false);
			this->menu->buttons->disable();
			this->menu->verify->enable();
		} else if(stage == 2) {
			this->menu->buttons->enable();
			this->menu->verify->disable();
			//Save YES
			saveFile(menu->filename,menu->save->data,512*1024);

			if(file_size != 512*1024) {
				menu->status->setText("Warning: Save Failed!");
				menu->status->setColor(RGB5(31,0,0));
			} else {
				menu->status->setText("Save Succeed");
				menu->status->setColor(0);
			}

		} else if(stage == 3) {

			this->menu->buttons->enable();
			this->menu->verify->disable();
		}
	}
};


enum {
	SubMenuTrainer,
	SubMenuPokemon,
	SubMenuItem,
	SubMenuPokedex,
};

class SubMenu : public Funct {
	OptionMenu *menu;
	int number;
public:
	SubMenu(OptionMenu *m, int n) {
		menu = m;
		number = n;
	}
	virtual void execute() {
		menu->submenu(number);
	}
};

OptionMenu::OptionMenu(MainMenu *mm, const char* file) : Screen() {
	save = mm->save;
	mm->save = NULL;
	mm->top->dropTexture(); //this is no longer needed (never going to use it again)
	this->setTop(mm->bottom);
	this->setBottom(mm->bottom);
	delete mm;


	{
		int len = strlen(file)+1;
		filename = (char*)malloc(len);
		memset(filename,0,len);
		strcpy(filename,file);
	}


	using namespace PokeLib;
	//Design the top layout here
	//Save Statistics
	//
	//Option Menu
	Label *label = new Label(0,4,256,16,"Save Statistics");
	label->setAlignment(ALIGN_CENTER);
	label->setColor(0);
	top->addChild(label);

	UpdateFunct *funct = new UpdateFunct();
	funct->save = this->save;
	this->update = funct;

	label = new Label(8,16,192+32,30,NULL);
	label->setBackground(true);
	label->setBorder(true);
	top->addChild(label);

	label = new Label(16,16+8,256-32,16,NULL);
	SaveType type = save->getSaveType();
	char tmp[128];
	memset(tmp,0,128);
	strcpy(tmp,"Save Type: ");
	switch(type) {
	case DP:
		strcat(tmp,"Diamond / Pearl");
		break;
	case PLAT:
		strcat(tmp,"Platinum");
		break;
	case HGSS:
		strcat(tmp,"Heart Gold / Soul Silver");
		break;
	}
	label->setText(tmp);
	top->addChild(label);

	label = new Label(12,14+7+12+4,160,41,NULL);
	label->setBackground(true);
	label->setBorder(true);
	top->addChild(label);

	label = new Label(12,72+7,160,41,NULL);
	label->setBackground(true);
	label->setBorder(true);
	top->addChild(label);

	label = new Label(16,16+8+12+4,256-32,16,"Original Checksum");
	label->setColor(0);
	top->addChild(label);
	label = new Label(16,72+10,256-32,16,"Current Checksum");
	label->setColor(0);
	top->addChild(label);

	u16 A = save->calcBlockAChecksum();
	u16 B = save->calcBlockBChecksum();
	siprintf(tmp,"Small: 0x%04X",A);
	label = new Label(32,16+8+12+4+12,256-64,16,tmp);
	label->setColor(0);
	top->addChild(label);
	funct->checksumA = new Label(32,16+8+12*4+10+12,256-64,16,tmp);
	funct->checksumA->setColor(0);
	top->addChild(funct->checksumA);
	siprintf(tmp,"Big:   0x%04X",B);
	label = new Label(32,16+8+12+4+12*2,256-64,16,tmp);
	label->setColor(0);
	top->addChild(label);
	funct->checksumB = new Label(32,72+10+12*2,256-64,16,tmp);
	funct->checksumB->setColor(0);
	top->addChild(funct->checksumB);



	label = new Label(12,124+4,180,32,NULL);
	label->setBackground(true);
	label->setBorder(true);
	top->addChild(label);

	funct->trainer = new Label(16,124+8,256-64,16,NULL);
	funct->trainer->setColor(0);
	top->addChild(funct->trainer);

	funct->rival = new Label(16,124+20,256-64,16,NULL);
	funct->rival->setColor(0);
	top->addChild(funct->rival);

	update->execute();

	status = new Label(0,192-14,256,16,NULL);
	status->setAlignment(ALIGN_CENTER);
	status->setColor(0);
	top->addChild(status);

	//Design the menu here
	// Trainer     Pokemon
	// Items       Pokedex
	// ????        Save
	buttons = new Widget(0,0,256,192);

	Button *b = new Button(16,16,101,40,"Trainer");
	b->setFunctionoid(new SubMenu(this,SubMenuTrainer));
	b->setColor(0);
	buttons->addChild(b);

	b = new Button(16*2+101,16,101,40,"Pokémon");
	b->setFunctionoid(new SubMenu(this,SubMenuPokemon));
	b->setColor(0);
	buttons->addChild(b);

	b = new Button(16,16*2+40,101,40,"Items");
	b->setFunctionoid(new SubMenu(this,SubMenuItem));
	b->setColor(0);
	buttons->addChild(b);

	b = new Button(16*2+101,16*2+40,101,40,"Pokédex");
	b->setFunctionoid(new SubMenu(this,SubMenuPokedex));
	b->setColor(0);
	buttons->addChild(b);

	b = new Button(16*2+101,16*3+40*2,101,40,"Save");
	b->setFunctionoid(new SaveFunct(this,1));
	b->setColor(0);
	buttons->addChild(b);

	verify = new Widget(32,32,256-64,192-64);

	label = new Label(0,0,192,16,"Overwrite?");
	label->setAlignment(ALIGN_CENTER);
	label->setColor(0);
	verify->addChild(label);

	b = new Button(0,32,192,48,"Yes");
	b->setColor(0);
	b->setFunctionoid(new SaveFunct(this,2));
	verify->addChild(b);

	b = new Button(0,32+48+2,192,48,"No");
	b->setColor(0);
	b->setFunctionoid(new SaveFunct(this,3));
	verify->addChild(b);

	verify->disable();

	/*verify = new Button(32,32,256-64,192-64,"Overwrite?");
	verify->setColor(0);
	verify->disable();
	verify->setFunctionoid(new SaveFunct(this,2));
	*/

	bottom->addChild(buttons);

	bottom->addChild(new Widget(0,0,1,1));

	bottom->addChild(verify);
}

OptionMenu::~OptionMenu() {
	delete update;
}

void OptionMenu::submenu(int n) {
	nextScreenCheck = true;
	next = n;
}


Screen *OptionMenu::nextScreen() {
	switch(next) {
	case SubMenuTrainer:
		return new TrainerEditMenu(this);
	case SubMenuPokemon:
		return new PokeBrowseMenu(this);
	case SubMenuItem:
		return new ItemEditMenu(this);
	case SubMenuPokedex:
		return new DexEditMenu(this);
	}

	return NULL;
}

void OptionMenu::previous(Screen *n) {
	if(nextScreenCheck && n != NULL) {
		nextScreenCheck = false;
		delete n;
	}
	update->execute();
}
