/***************************************************************************
                          Meny.h  -  description
                             -------------------
    begin                : Tue Feb 15 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef MENY_H
#define MENY_H

#define EM_MENU_NOP 0
#define EM_MENU_EXIT 1
#define EM_MENU_RESUME 2
#define EM_MENU_BACK 3
#define EM_MENU_SUB 4

#include "Private.h"

class Engine;
class EmFont;

/**
 */

class Meny {
 public:
	Meny(char * name, Engine * e);
	~Meny();
	virtual int start();
	void addMeny(Meny * meny);
	void setAction(int act);
	char * getName();
	void setName(char *);
	int action();
	void setFunction(void (*fct)(Meny *));
 private:
	int m_current;
	char * m_name;
	int m_action;
	vector<Meny*> m_vMeny;
	EmFont * p_EmFont;
	Engine * p_Engine;
	
	void (*m_function)(Meny *);

	void draw();
};

#endif // MENY_H
