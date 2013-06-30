/*
 * PokeBrowseMenu.cpp
 *
 *  Created on: Mar 10, 2010
 *      Author: Chase
 */

#include <cstdio>
#include "Toolkit.h"


//6x5
//32x24

class ReturnFunct : public Funct {
	PokeBrowseMenu* menu;
public:
	ReturnFunct(PokeBrowseMenu* m) {
		menu = m;
	}
	virtual void execute() {
		menu->setReturn();
	}
};

class ChangeBoxFunct : public Funct {
public:
	PokeBrowseMenu* menu;
	s8 direction;
	ChangeBoxFunct(PokeBrowseMenu* m, s8 dir) {
		menu = m;
		direction = dir;
	}
	virtual void execute() {
		menu->box_num += direction;
		if(menu->box_num > 18) menu->box_num = 1;
		if(menu->box_num < 1) menu->box_num = 18;
		menu->loadBox();
	}
};

class Icon  {
public:
	int16_t num;
	ulxImage* img;
	ulxTexture* tex;

	void createTexture() {
		if(this->tex == NULL)
			this->tex = ulxCreateTexture(img);
	}
	void dropTexture() {
		ulxDropTexture(this->tex);
		free(this->tex);
		this->tex = NULL;
	}
	Icon(ulxImage*img, int num) {
		this->num = num;
		this->img = img;
		this->tex = ulxCreateTexture(img);
		//createTexture();
	}
	~Icon() {
		ulxDropTexture(this->tex);
		free(this->tex);
		this->tex = NULL;

		ulxDeleteImage(this->img);
		free(this->img);
		this->img = NULL;
	}
};


class DragIcon : public Image {
public:
	uint8_t pos;
	uint8_t box;
	Icon *ico;
	bool can_drag;

	PokeBrowseMenu* menu;
	DragIcon(PokeBrowseMenu *m, int x, int y) : Image(x,y,32,24,NULL) {
		menu = m;
		can_drag = false;
		pos = 100;
		box = 100;
	}
	void refreshIcon() {
		int16_t tmp = ico->num;
		setNum(tmp);
	}
	void setNum(int16_t num) {
		setIcon(menu->getIcon(num));
		if(num != 0) can_drag = true;
	}
	void setIcon(Icon* n) {
		ico = n;
		this->_texture = ico->tex;
	}
	void createTexture() { }
	void draw(int x, int y) {
		if(!can_drag) return;
		if(this != menu->isDrag
		&& isMatch(menu->isDrag)) return;
		if(this->box == 100) return;
		ulxDrawTexture(x+_x,y+_y,_texture);
	}
	bool checkBounds(int x, int y) {
		int tx = getX();
		int ty = getY()+8;
		if(x > tx && y > ty && x < tx+_w && y < ty+_h) {
			return true;
		}
		return false;
	}
	virtual void onClick(s16 x, s16 y) {
		if(!checkBounds(x,y) || !isActive) return;
		this->_cx = x; this->_cy = y;
		if(ico == NULL) return;
		if(ico->num == 0) return;
		if(this == menu->isDrag) return;

		//TODO: Do actual loading in a function call. Including everything from here down!
		if(!menu->sprite->checkActive())
			menu->sprite->enable();

		bool isShiny = false;
		if(box == 0)
			isShiny = menu->pkPty->getPokemon(pos+1).isShiny();
		else //For now, assume this box!
			isShiny = menu->pkBox->getPokemon(pos+1).isShiny();

		menu->sprite->setSpriteNumber(ico->num,isShiny);
	}
	virtual void onDrag(s16 x, s16 y) {
		if(!checkBounds(_cx,_cy) || !isActive) return;
		if(menu->isDrag->box == 100) {
			menu->isDrag->box = this->box;
			menu->isDrag->pos = this->pos;
			menu->isDrag->setIcon(this->ico);
			menu->isDrag->can_drag = true;
			menu->isDrag->_cx = x;
			menu->isDrag->_cy = y;
			menu->isDrag->setX(x-16);
			menu->isDrag->setY(y-16);
		} else if(this == menu->isDrag) {
			this->setX(x-16);
			this->setY(y-16);
		}
		this->_cx = x;
		this->_cy = y;
	}
	virtual void onDragEnd() {
		if(this == menu->isDrag) {
			//Determine the location we are over!
			DragIcon *stdic = NULL;
			for(int i=0; i<6; ++i) {
				DragIcon *tdic = menu->display_party[i];
				if(tdic->checkBounds(_cx,_cy)) {
					stdic = tdic;
					break;
				}
			}
			for(int i=0; i<30 && stdic == NULL; ++i) {
				DragIcon *tdic = menu->display_box[i];
				if(tdic->checkBounds(_cx,_cy)) {
					stdic = tdic;
					break;
				}
			}

			//If nowhere, snap back!
			if(stdic == NULL || isMatch(stdic)) {
				box = 100;
				pos = 100;
				can_drag = false;
				return;
			}

			//switch with it!
			menu->switchPokemon(box,pos,stdic->box,stdic->pos);

			Icon *tmp = stdic->ico;
			//Update the icons
			stdic->setIcon(this->ico);
			stdic->can_drag = true;
			this->ico = NULL;

			if(box == menu->box_num
			|| box == 0) {
				//We need to update the display! (original pokemon)
				if(box == 0) {
					menu->display_party[pos]->setIcon(tmp);
				} else {
					menu->display_box[pos]->setIcon(tmp);
				}
			}

			box = 100;
			pos = 100;
			can_drag = false;
		}
	}
	bool isMatch(DragIcon *icon) {
		if(icon->box == this->box
		&& icon->pos == this->pos)
			return true;
		return false;
	}
};

PokeBrowseMenu::PokeBrowseMenu(OptionMenu*m) : Screen() {
	menu = m;
	this->setTop(m->top);
	this->setBottom(m->top);

	//Draw the box
	Label *bg = new Label(8,32,192,120,NULL);
	bg->setBackground(true);
	bottom->addChild(bg);

	bg = new Label(216,8,32,24*6,NULL);
	bg->setBackground(true);
	bottom->addChild(bg);

	Button *b = new Button(8,8,32,20,"<<");
	b->setFunctionoid(new ChangeBoxFunct(this,-1));
	b->bindControl(KEY_LEFT);
	bottom->addChild(b);
	b = new Button(168,8,32,20,">>");
	b->setFunctionoid(new ChangeBoxFunct(this,1));
	b->bindControl(KEY_RIGHT);
	bottom->addChild(b);

	//boxName = new Label(8,8+4,128,20,"box");
	boxName = new Label(8,8+4,192,20,NULL);
	boxName->setAlignment(ALIGN_CENTER);
	bottom->addChild(boxName);

	bg = new Label(4,4,88,88,NULL);
	bg->setBackground(true);
	bg->setBorder(true);
	top->addChild(bg);

	sprite = new SpritePKM(8,8,1,false);
	sprite->disable();
	top->addChild(sprite);

	//isDrag = NULL;
	pkBox = NULL;
	pkPty = NULL;
	box_num = 1;

	icons.push_back(new Icon(NULL,0));

	for(int i = 0; i < 30; ++i) {
		DragIcon *tmp = new DragIcon(this, (i%6)*32+8,(i/6)*24+24);
		display_box.push_back(tmp);
		bottom->addChild(tmp);
	}
	//
	for(int i = 0; i < 6; ++i) {
		DragIcon *tmp = new DragIcon(this, 216, i*24);
		display_party.push_back(tmp);
		bottom->addChild(tmp);
	}

	isDrag = new DragIcon(this,0,0);
	bottom->addChild(isDrag);

	loadBox();
}

PokeBrowseMenu::~PokeBrowseMenu() {
	while(icons.size() > 0) {
		Icon *img = icons.back();
		icons.pop_back();
		ulxDeleteImage(img->img);
		img->img = NULL;
		img = NULL;
	}
}

void PokeBrowseMenu::loadBox() {
	//Drop old textures from texture memory
	//on box change (very important!)
	for(uint16 i=0; i<icons.size(); ++i) {
		icons[i]->dropTexture();
	}

//	ulxDropTexture(sprite->_texture);
//	for(uint16 i=0; i<icons.size(); ++i) {
//		ulxDropTexture(icons[i]->tex);
//	}
	if(isDrag->box != 100) {
		isDrag->refreshIcon();
	}

	if(pkPty != NULL) {
		//just reload the icons from the party
		for(int i=0; i<6; ++i) {
			display_party[i]->refreshIcon();
		}
	} else {
		pkPty = menu->save->getParty();
		for(int i=0; i<6; ++i) {
			PokeLib::Pokemon poke = pkPty->getPokemon(i+1);
			int num = poke.pkm->pkm.species;

			display_party[i]->box = 0;
			display_party[i]->pos = i;

			if(poke.pkm->pkm.isEgg) {
				display_party[i]->setNum(-1);
			} else {
				display_party[i]->setNum(num);
			}
		}
	}

	//Delete old box
	delete pkBox;
	pkBox = menu->save->getBox(box_num);

	//SET BOX NAME
	{
		char tmp[128];
		memset(tmp,0,128);
		using namespace PokeLib;
		widetext txt = pkBox->getName();
		//txt
		char tmp2[64];
		memset(tmp2,0,64);
		wcstombs (tmp2, txt.c_str(), 64);
		siprintf(tmp,"#%02i: %s", box_num, tmp2);

		boxName->setText(tmp);
	}

	if(pkBox != NULL) {
		for(int i=0; i<30; ++i) {
			PokeLib::Pokemon poke = pkBox->getPokemon(i+1);
			int num = poke.pkm->pkm.species;

			display_box[i]->can_drag = false;
			display_box[i]->box = box_num;
			display_box[i]->pos = i;

			if(poke.pkm->pkm.isEgg) {
				display_box[i]->setNum(-1);
			} else {
				display_box[i]->setNum(num);
			}
		}
	}
}

void PokeBrowseMenu::switchPokemon(s16 box1, s16 pos1, s16 box2, s16 pos2) {
	using namespace PokeLib;
	Pokemon pkm1;
	Pokemon pkm2;

	if(box1 == box2) {
		//Same Box (super optimized!)
		if(box1 == 0) {
			pkm1 = pkPty->getPokemon(pos1 + 1);
			pkm2 = pkPty->getPokemon(pos2 + 1);
			pkPty->setPokemon(pos1 + 1, pkm2);
			pkPty->setPokemon(pos2 + 1, pkm1);

			menu->save->setParty(pkPty);
		} else {
			//This means that it is also the CURRENT box, meaning no loading!
			pkm1 = pkBox->getPokemon(pos1 + 1);
			pkm2 = pkBox->getPokemon(pos2 + 1);
			pkBox->setPokemon(pos1 + 1, pkm2);
			pkBox->setPokemon(pos2 + 1, pkm1);

			menu->save->setBox(box_num,pkBox);
		}
	} else {
		//Otherwise: Pain in the ass!
		Box *fbox=NULL, *sbox=NULL;
		if(box1 == 0) {
			pkm1 = pkPty->getPokemon(pos1 + 1);
		} else {
			if(box1 == box_num) fbox = pkBox;
			else fbox = menu->save->getBox(box1);
			pkm1 = fbox->getPokemon(pos1 + 1);
		}
		if(box2 == 0) {
			pkm2 = pkPty->getPokemon(pos2 + 1);
			pkm1.updateBattleStats();
			pkPty->setPokemon(pos2 + 1, pkm1);
			menu->save->setParty(pkPty);
		} else {
			if(box2 == box_num) sbox = pkBox;
			else sbox = menu->save->getBox(box2);
			pkm2 = sbox->getPokemon(pos2 + 1);
			sbox->setPokemon(pos2 + 1, pkm1);

			menu->save->setBox(box2,sbox);
			if(sbox != pkBox)
				delete sbox;
		}
		if(box1 == 0) {
			pkm2.updateBattleStats();
			pkPty->setPokemon(pos1 + 1, pkm2);
			menu->save->setParty(pkPty);
		} else {
			if(fbox == NULL)
				sassert(false, "Box Error!");
			fbox->setPokemon(pos1 + 1, pkm2);
			menu->save->setBox(box1,fbox);
			if(fbox != pkBox)
				delete fbox;
		}
	}
}

Icon *PokeBrowseMenu::getIcon(int num) {
	for(uint16 i=0; i<icons.size(); ++i) {
		if(icons[i]->num == num) {
			icons[i]->createTexture();
			return icons[i];
		}
	}

	char tmp[32];
	memset(tmp,0,32);
	if(num == -1) {
		strcpy(tmp,"nitro:/icon/egg.gif");
	} else {
		siprintf(tmp,"nitro:/icon/%03i.gif",num);
	}

	u8 *data = loadFile(tmp);
	ulxImage *img = ulxLoadImageGIF(data,ULX_PAL4);
	free(data);

	Icon *ico = new Icon(img,num);
	icons.push_back(ico);
	return ico;
}

bool PokeBrowseMenu::changeScreen() {
//	ulxDropTexture(sprite->_texture);
//	for(uint16 i=0; i<icons.size(); ++i) {
//		ulxDropTexture(icons[i]->tex);
//	}
	return nextScreenCheck;
}
