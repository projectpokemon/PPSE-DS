/*
 * Kanapad.cpp
 *
 *  Created on: 2010/01/27
 *      Author: Chase
 */


#include <nds.h>
#include <vector>
#include <cctype>
#include "Toolkit.h"
#include <ulx.h>
#include "Kanapad.h"

const wchar_t ktenten = L'゛';
const wchar_t kmaru = L'゜';
const wchar_t kwhite = L'　';

//1 ゛ てんてん　tenten
//2 ゜ まる　maru
//3 まると点点　both

//まると点点　表　（ダイアクリティック）
const u8 kanaExt[65] = {
	0,1,1, 1,0,3, 0,0,0, 0, 0,0,0,
	0,1,1, 1,0,3, 0,0,0, 0, 0,0,0,
	0,1,1, 1,0,3, 0,0,0, 0, 0,0,0,
	0,1,1, 1,0,3, 0,0,0, 0, 0,0,0,
	0,1,1, 1,0,3, 0,0,0, 0, 0,0,0,
};

const wchar_t kanab[4][65] = { //皆13です
	{//Hiragana ひらがな
		L'あ',L'か',L'さ',L'た',L'な',L'は',L'ま',L'や',L'ら',L'わ',L'　',L'ぁ',L'ゃ',
		L'い',L'き',L'し',L'ち',L'に',L'ひ',L'み',L'　',L'り',L'　',L'　',L'ぃ',L'ゅ',
		L'う',L'く',L'す',L'つ',L'ぬ',L'ふ',L'む',L'ゆ',L'る',L'ん',L'　',L'ぅ',L'ょ',
		L'え',L'け',L'せ',L'て',L'ね',L'へ',L'め',L'　',L'れ',L'　',L'　',L'ぇ',L'っ',
		L'お',L'こ',L'そ',L'と',L'の',L'ほ',L'も',L'よ',L'ろ',L'を',L'　',L'ぉ',L'ー',
	},{//Katakana　かたかな
		L'ア',L'カ',L'サ',L'タ',L'ナ',L'ハ',L'マ',L'ヤ',L'ラ',L'ワ',L'　',L'ァ',L'ャ',
		L'イ',L'キ',L'シ',L'チ',L'ニ',L'ヒ',L'ミ',L'　',L'リ',L'　',L'　',L'ィ',L'ュ',
		L'ウ',L'ク',L'ス',L'ツ',L'ヌ',L'フ',L'ム',L'ユ',L'ル',L'ン',L'　',L'ゥ',L'ョ',
		L'エ',L'ケ',L'セ',L'テ',L'ネ',L'ヘ',L'メ',L'　',L'レ',L'　',L'　',L'ェ',L'ッ',
		L'オ',L'コ',L'ソ',L'ト',L'ノ',L'ホ',L'モ',L'ヨ',L'ロ',L'オ',L'　',L'ォ',L'ｰ',
	},{//Wide
		L'Ａ',L'Ｂ',L'Ｃ',L'Ｄ',L'Ｅ',L'Ｆ',L'Ｇ',L'Ｈ',L'Ｉ',L'Ｊ',L'Ｋ',L'Ｌ',L'Ｍ',
		L'Ｎ',L'Ｏ',L'Ｐ',L'Ｑ',L'Ｒ',L'Ｓ',L'Ｔ',L'Ｕ',L'Ｖ',L'Ｗ',L'Ｘ',L'Ｙ',L'Ｚ',
		L'ａ',L'ｂ',L'ｃ',L'ｄ',L'ｅ',L'ｆ',L'ｇ',L'ｈ',L'ｉ',L'ｊ',L'ｋ',L'ｌ',L'ｍ',
		L'ｎ',L'ｏ',L'ｐ',L'ｑ',L'ｒ',L'ｓ',L'ｔ',L'ｕ',L'ｖ',L'ｗ',L'ｘ',L'ｙ',L'ｚ',
		L'０',L'１',L'２',L'３',L'４',L'５',L'６',L'７',L'８',L'９',L'．',L'，',L'　',
	},{//Special
		L'0',L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8',L'9',L'　',L'！',L'？',
		L'、',L'。',L'，',L'．',L'…',L'・',L'～',L'：',L'；',L'／',L'　',L'♂',L'♀',
		L'「',L'」',L'『',L'』',L'（',L'）',L'＋',L'－',L'×',L'÷',L'＝',L'％',L'＠',
		L'◎',L'○',L'□',L'△',L'◇',L'♥',L'♠',L'♦',L'♣',L'★',L'♪',L'　',L'　',
		L'☀',L'☁',L'☂',L'☃',L'　',L'　',L'　',L'　',L'　',L'⤴',L'⤵',L'　',L'　',
	}
};

class KanaPadChangeFunct : public Funct {
	Kanapad *p;
	s8 target;
	public:
	KanaPadChangeFunct(Kanapad *n, s8 targ) { p = n; target = targ; }
	inline void execute() {
		//p->changeBoard = true;
		//p->_targetBoard = target;

		if(target == 4) {
			p->kpress("\1");
			return;
		}
		p->board[p->_cBoard]->disable();
		p->board[target]->enable();
		p->_cBoard = target;
	}
};

class KanaPadFunct : public FunctKey {
	Kanapad *p;
	public:
	KanaPadFunct(Kanapad *n) { p = n; }
	inline void execute(const char* str) {
		p->kpress(str);
	}
};

void Kanapad::kpress(const char * str) {
	if(retFunoid != NULL) retFunoid->execute(str);
	if(retFun != NULL) retFun(str);
}

//#define SIZE	18
//#define SPACE	1

Kanapad::Kanapad(int x, int y)  : Widget(x,y,256,140) {
	retFunoid = NULL;
	retFun = NULL;
	_cBoard = 0;

	int size = 18;
	int space = 1;

	u8 hTime = 1;

	for(int i = 0; i < 4; ++i) {
		board[i] = new Widget(0,19,256,140);
		board[i]->disable();
		addChild(board[i]);
	}

	char tmp[8];
	for(int i = 0; i < 65; ++i) {
		for(int j = 0; j < 4; ++j) {
			wchar_t wtmp = kanab[j][i];
			memset(tmp,0,8);
			wctomb(tmp, wtmp);
			ButtonKey *b = new ButtonKey((size+space)*(i%13),(size+space)*(i/13),size,size,tmp);
			if(wtmp == kwhite) {
				b->setBackground(false);
				b->setClickable(false);
			} else {
				b->setHighlightTime(hTime);
				b->setColor(0);
				b->setKeyFunctionoid(new KanaPadFunct(this));
			}
			board[j]->addChild(b);
		}
	}
	board[_cBoard]->enable();

	Button *b = new Button((size+space)*0, (size+space)*0, size*2+space, size, "Mode");
	b->setColor(0);
	b->setClickable(false);
	b->setBorder(false);
	b->setBackground(false);
	addChild(b);

	b = new Button((size+space)*2, (size+space)*0, size*2+space, size, "かな");
	b->setFunctionoid(new KanaPadChangeFunct(this,0));
	b->setHighlightTime(hTime);
	b->setColor(0);
	addChild(b);

	b = new Button((size+space)*4, (size+space)*0, size*2+space, size, "カナ");
	b->setFunctionoid(new KanaPadChangeFunct(this,1));
	b->setHighlightTime(hTime);
	b->setColor(0);
	addChild(b);

	b = new Button((size+space)*6, (size+space)*0, size*2+space, size, "ABC");
	b->setFunctionoid(new KanaPadChangeFunct(this,2));
	b->setHighlightTime(hTime);
	b->setColor(0);
	addChild(b);

	b = new Button((size+space)*8, (size+space)*0, size*2+space, size, "1/♪");
	b->setFunctionoid(new KanaPadChangeFunct(this,3));
	b->setHighlightTime(hTime);
	b->setColor(0);
	addChild(b);

//	b = new Button((size+space)*10, (size+space)*0, size*3+space*2, size, "Exit");
//	b->setFunctionoid(new KanaPadChangeFunct(this,4));
//	b->setHighlightTime(hTime);
//	b->setColor(0);
//	addChild(b);

	wctomb(tmp, kmaru);
	ButtonKey *bk = new ButtonKey((size+space)*11, (size+space)*6, size, size,tmp);
	bk->setKeyFunctionoid(new KanaPadFunct(this));
	bk->setHighlightTime(hTime);
	bk->setColor(0);
	addChild(bk);

	wctomb(tmp, ktenten);
	bk = new ButtonKey((size+space)*12, (size+space)*6, size, size,tmp);
	bk->setKeyFunctionoid(new KanaPadFunct(this));
	bk->setHighlightTime(hTime);
	bk->setColor(0);
	addChild(bk);

	bk = new ButtonKey((size+space)*3, (size+space)*6, size*7+space*6, size, " ");
	bk->setKeyFunctionoid(new KanaPadFunct(this));
	bk->setHighlightTime(hTime);
	bk->setColor(0);
	addChild(bk);
}

Kanapad::~Kanapad() {
//	for(int i = 0; i < 4; ++i) {
//		delete board[i];
//	}
}
void Kanapad::addMaru(char * str) {
	wchar_t tmp = 0;
	mbtowc(&tmp,str,5);
	for(int i=0;i<65;++i) {
		if( kanaExt[i] & 2 ) {
			if( tmp == kanab[0][i] || tmp == kanab[1][i] ) {
				tmp += 2;
				break;
			}
		}
	}
	wctomb(str, tmp);
}

void Kanapad::addTenten(char * str) {
	wchar_t tmp = 0;
	mbtowc(&tmp,str,5);
	for(int i=0;i<65;++i) {
		if( kanaExt[i] & 1 ) {
			if( tmp == kanab[0][i] || tmp == kanab[1][i] ) {
				tmp += 1;
				break;
			}
		}
	}
	wctomb(str, tmp);
}

//bool Kanapad::checkInvalidated() {
//	bool b = Widget::checkInvalidated();
//
//	//if(!b && ) {
//	if(_changeBoard) {
//		board[_cBoard]->disable();
//		board[_targetBoard]->enable();
//		_cBoard = _targetBoard;
//		_changeBoard = false;
//		b = true;
//	}
//
//	return b;
//}
