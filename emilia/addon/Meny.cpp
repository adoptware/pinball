/***************************************************************************
                          Meny.cpp  -  description
                             -------------------
    begin                : Tue Feb 15 2000
    copyright            : (C) 2000 by 
    email                :
 ***************************************************************************/

#include "Meny.h"
#include "EmFont.h"
#include "Keyboard.h"
#include "Engine.h"

#include <string.h>

extern int em_width_div2_;
extern int em_height_div2_;

Meny::Meny(char * name, Engine * e) {
	EmAssert(e != NULL, "Engine not created");

	m_action = EM_MENU_EXIT;
	m_name = name;
	m_current = 0;
	p_EmFont = EmFont::getInstance();
	p_Engine = e;
	m_function = NULL;
}

Meny::~Meny() {
}

void Meny::addMeny(Meny * menu) {
	m_vMeny.push_back(menu);
}

void Meny::setName(char * name) {
	m_name = name;
}

void Meny::setAction(int act) {
	m_action = act;
}

void Meny::setFunction(void (*fct)(Meny *)) {
	m_function = fct;
}

int Meny::action() {
	if (m_function != NULL) {
		m_function(this);
	}
	return m_action;
}

int Meny::start() {
  EM_COUT("Meny::start() " << m_name, 1)

	if (m_vMeny.size() == 0) {
		return this->action();
	}
	
	bool done = false;
	int ret = 0;

	SDLKey key = SDLK_a;
	while(true) {
		int counter = 0;
		if (key == KEY_DOWN) m_current++;
		if (key == KEY_UP) m_current--;
																	
		if (m_current < 0) m_current = m_vMeny.size()-1;
		if (m_current >= m_vMeny.size()) m_current = 0;

		this->draw();

		key = Keyboard::waitForKey();
		if (key == SDLK_RETURN) {
			ret = m_vMeny[m_current]->start();
			switch (ret) {
			case EM_MENU_EXIT:
			case EM_MENU_RESUME:
				Keyboard::clear();
				return ret;
				break;
			case EM_MENU_BACK:
				Keyboard::clear();
				return EM_MENU_NOP;
			}
		}
	}
}

char * Meny::getName() {
	return m_name;
}

void Meny::draw() {
	EM_COUT("Meny::draw()", 1);
	p_Engine->clearScreen();

	p_EmFont->print(this->getName(), em_width_div2_ -
									strlen(this->getName())*EmFont::getInstance()->getSize()/2, 
									em_height_div2_ + 40);

	vector<Meny*>::iterator menyIter = m_vMeny.begin();
	vector<Meny*>::iterator menyEnd = m_vMeny.end();
	for (int a=0; menyIter != menyEnd; menyIter++, a++) {
		if (a == m_current) {
			char str[256];
			strncpy(str, "*", 16);
			strncat(str, (*menyIter)->getName(), 64);
			strncat(str, "*", 16);
			p_EmFont->print(str, em_width_div2_ - strlen(str)*EmFont::getInstance()->getSize()/2,
											em_height_div2_ - 20*a);
		} else {
			p_EmFont->print((*menyIter)->getName(), em_width_div2_ - 
											strlen((*menyIter)->getName())*EmFont::getInstance()->getSize()/2, 
											em_height_div2_ - 20*a);
		}
	}

	p_Engine->swap();
}
 
