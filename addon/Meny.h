/***************************************************************************
                          Meny.h  -  description
                             -------------------
    begin                : Tue Feb 15 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef MENUITEM_H
#define MENUITEM_H

#include "Private.h"

class Engine;
class EmFont;

#define EM_MENU_NOP         0
#define EM_MENU_EXIT        1
#define EM_MENU_RESUME      2
#define EM_MENU_BACK        3

#define EM_MENU             0
#define EM_MENU_SUB         1
#define EM_MENU_CHOOSE      2
#define EM_MENU_INSERT      3
#define EM_MENU_FCT         4

/** */

class MenuItem {
 public:
	MenuItem(Engine* e, int type);
	virtual ~MenuItem();
	virtual char* getText() = 0;
	virtual int perform() = 0;
	int getType() { return m_iType; };
 protected:
	EmFont* p_EmFont;
	Engine* p_Engine;
	int m_iType;
};

/** A meny holding sub menys, can also be used for a back or exit item */
class MenuSub : public MenuItem {
 public:
	MenuSub(char* name, Engine* e);
	~MenuSub();
	void addMenuItem(MenuItem* m);
	char* getText() { return m_Name; };
	int perform();
	void draw();
	void setAction(int a) { m_iAction = a; };
 private:
	char* m_Name;
	int m_iCurrent;
	int m_iAction;
	vector<MenuItem*> m_vMenuItem;
};

class MenuChoose : public MenuItem {
 public:
	MenuChoose(Engine* e);
	~MenuChoose();
	void addText(char *);
	int getCurrent() { return m_iCurrent; };
	void setCurrent(int c) { m_iCurrent = c; };
	char* getText();
	int perform();
 private:
	int m_iCurrent;
	vector<char*> m_vText;
};

class MenuFct : public MenuItem {
 public:
	MenuFct(char* name, int (*fct)(void), Engine* e);
	~MenuFct();
	int perform();
	char* getText() { return m_Name; };
 private:
	char* m_Name;
	int (*m_Fct)(void);
};

#endif // MENUITEM_H
