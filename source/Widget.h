/*
 * Widget.h
 *
 *  Created on: Nov 11, 2009
 *      Author: Chase
 */

#ifndef __WIDGET_H_
#define __WIDGET_H_

class Widget {
protected:
	bool isInvalidated;
	bool isActive;
	bool isVisible;
	Widget* parent;
	std::vector<Widget*> children;
	s16 _x, _y, _w, _h;
	s16 _cx, _cy;
	inline void setParent(Widget*p) { parent = p; }
public:
	Widget(int x, int y, int w, int h);
	virtual ~Widget();

	const s16 getX() const;
	const s16 getY() const;
	void setX(s16 x) { _x = x; };
	void setY(s16 y) { _y = y; }
	inline const s16 getWidth() const { return _w; }
	inline const s16 getHeight() const { return _h; }
	virtual inline void addChild(Widget*w) {
		w->setParent(this);
		children.push_back(w);
	}
	bool checkBounds(int x, int y);
	virtual bool checkInvalidated();
	virtual void onClick(s16 x, s16 y);
	virtual void onDrag(s16 x, s16 y);
	virtual void onDragEnd();
	virtual void onControl(u32 keyPress);
	virtual void draw(int x, int y);
	virtual void setActive(bool a) { isActive = a; }
	virtual void setVisible(bool v) { isVisible = v; }
	virtual void enable() { isActive = true; isVisible = true; }
	virtual void disable() { isActive = false; isVisible = false; isInvalidated = false; }
	virtual bool checkActive() { return isActive; }
	virtual bool checkVisible() { return isVisible; }
	virtual void doUpdate() {};
	void draw() {
		draw(getX(),getY());
	}
};

#endif /* __WIDGET_H_ */
