/*
 * MainMenu.cpp
 *
 *  Created on: Mar 7, 2010
 *      Author: Chase
 */

#include <cstdio>
#include "Toolkit.h"

class MainMenuFunct : public Funct {
	MainMenu *m;
public:
	MainMenuFunct(MainMenu *m) {
		this->m = m;
	}
	virtual void execute() {
		m->b->setVisible(false);
		m->b->disable();
		m->list->enable();
		m->about->disable();
		m->lab->enable();
		m->lbtn->enable();
	}
};

class AboutMenuFunct : public Funct {
	MainMenu *m;
public:
	AboutMenuFunct(MainMenu *m) {
		this->m = m;
	}
	virtual void execute() {
		if(!m->sleepy->checkActive()) {
			m->b->disable();
			m->sleepy->enable();
			m->about->setText("Done");
			m->about->setY(192-32);
		} else {
			m->b->enable();
			m->sleepy->disable();
			m->about->setText("About");
			m->about->setY((192>>1)+12+4);
		}
	}
};

class LoadFunct : public Funct {
	MainMenu *m;
public:
	LoadFunct(MainMenu *m) {
		this->m = m;
	}
	virtual void execute() {
		if( m->list->isDirectory(m->list->getCurrentNum()) ) {
			char tmp[256];
			m->list->getCurrentElement(tmp);
			m->list->changeDirectory(tmp);
			return;
		}
		m->doload();
	}
};

MainMenu::MainMenu(Image *main, Image *sub) : Screen() {
	setTop(main);
	setBottom(sub);

	Label *n = new Label(160,15,100,10,"Version "APP_VERSION);
	n->setColor(VERSION_COLOR);
	top->addChild(n);
	n = new Label(161,15,100,10,"Version "APP_VERSION);
	n->setColor(VERSION_COLOR);
	top->addChild(n);

	status = new Label(16,2,256-32,16,NULL);
	status->setAlignment(ALIGN_CENTER);

	b = new Button(128-64,(192>>1)-12-4,128,24,"Load");
	b->setFunctionoid(new MainMenuFunct(this));
	b->setColor(0);
	bottom->addChild(b);

	about = new Button(128-32,(192>>1)+12+4,64,24,"About");
	about->setFunctionoid(new AboutMenuFunct(this));
	about->setColor(0);
	bottom->addChild(about);

	u8 *data = loadFile("img/sleepy.gif");
	ulxImage *lazy = ulxLoadImageGIF(data, ULX_PAL8);
	free(data);
	sleepy = new Image(0,16,256,192-16,lazy);
	ulxFirstPaletteTransparent = 1;
	n = new Label(96+8,0,256-96,16,"Programmer: Chase-san");
	n->setColor(0);
	sleepy->addChild(n);
	n = new Label(96+16,12,256-96,16,"The Lazy Programmer");
	n->setColor(0);
	sleepy->addChild(n);
	n = new Label(16,48,256-32,16,"Made for Project Pokémon, as it should be. They"
			" supplied virtually all the information for this application. Special thanks to"
			" Alpha, Sabresite, codemonkey85, and WinterMute. All you"
			" researchers are great too. :)");
	n->setColor(0);
	n->setWrap(true);
	sleepy->addChild(n);
	bottom->addChild(sleepy);
	sleepy->disable();

	lab = new Label(16, 32-12, 256-32, 12, NULL);
	list = new FileList(16,32,256-32, 11);
	if(NOCASH) {
		list->setDirectory((const char*)"nitro:/");
	} else {
		list->setDirectory((const char*)"fat:/");
	}
	list->loadDirectory();
	//list->setDirChangeFunctionoid(new LabelDirFunct(this,l));


	lbtn = new Button(16,32+list->getHeight()+4,256-64, 16, "Load");
	lbtn->setFunctionoid(new LoadFunct(this));

	list->disable();
	lab->disable();
	lbtn->disable();

	bottom->addChild(status);

	bottom->addChild(list);
	bottom->addChild(lab);
	bottom->addChild(lbtn);
}

MainMenu::~MainMenu() {
	//delete sleepy->_img;
	ulxDeleteImage(sleepy->_img);
}

void MainMenu::drawTop() {
	Screen::drawTop();
}
void MainMenu::drawBottom() {
	char tmp[1024];
	list->getDirectory(tmp);
	lab->setText(tmp);

	Screen::drawBottom();
}

void MainMenu::doload() {
	using namespace PokeLib;

	char filename[256];
	memset(filename,0,256);
	list->getCurrentElement(filename);

	save = new Save(512*1024);
	loadFile(filename,save->data,512*1024);
	if(!save->parseRawSave()) {
		delete save;

		status->setText("Error: Unknown format.");

		return;
	} else {
		status->setText("Accepted!");

		list->disable();
		lab->disable();
		lbtn->disable();
	}

	//Wait while we load up the next screen
	nextScreenCheck = true;
}

Screen *MainMenu::nextScreen() {
	char filename[512];
	memset(filename,0,512);

	{
		char tmp[256];
		memset(tmp,0,256);;
		list->getDirectory(filename);
		list->getCurrentElement(tmp);
		strcat(filename,tmp);//CAT!
		//char tmp2[256];
		//memset(tmp2,0,256);;
		//list->getDirectory(tmp2);
		//siprintf(filename,"%s%s",tmp2,tmp);
	}

	return new OptionMenu(this, filename);
}
