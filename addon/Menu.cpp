/***************************************************************************
                          Menu.cpp  -  description
                             -------------------
    begin                : Tue Feb 15 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include <cstring>

#include "Private.h"
#include "Menu.h"
#include "EmFont.h"
#include "Keyboard.h"
#include "Engine.h"


MenuItem::MenuItem(Engine * e, int type) {
	EmAssert(e != NULL, "Engine not created");

	p_Texture = NULL;
	p_EmFont = EmFont::getInstance();
	p_Engine = e;
	m_iType = type;
}

MenuItem::~MenuItem() {
}

/*************************************************************************
 * A menu with sub menys */

MenuSub::MenuSub(const char * name, Engine* e) : MenuItem(e, EM_MENU_SUB) {
	strncpy(m_Name, name, 63);
	m_iCurrent = 0;
	m_iAction = EM_MENU_NOP;
}

MenuSub::~MenuSub() {
}

void MenuSub::addMenuItem(MenuItem * menu) {
	m_vMenuItem.push_back(menu);
}

int MenuSub::perform() {
  EM_COUT("MenuSub::perform() " << this->getText(), 1)
    
    if (m_vMenuItem.size() == 0) {
      return m_iAction;
    }
  
  int ret = 0;
  
  EMKey key = SDLK_a;
  while(true) {
    if (key == SDLK_DOWN) m_iCurrent++;
    if (key == SDLK_UP) m_iCurrent--;
    
    if (m_iCurrent < 0) m_iCurrent = m_vMenuItem.size()-1;
    if (m_iCurrent >= (signed)m_vMenuItem.size()) m_iCurrent = 0;
    
    this->draw();
    
    key = Keyboard::waitForKey();
    if (key == SDLK_RETURN) {
      ret = m_vMenuItem[m_iCurrent]->perform();
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
    if (key == SDLK_LEFT && m_vMenuItem[m_iCurrent]->getType() == EM_MENU_CHOOSE) {
      ((MenuChoose *)m_vMenuItem[m_iCurrent])->prev();
    }
    if (key == SDLK_RIGHT && m_vMenuItem[m_iCurrent]->getType() == EM_MENU_CHOOSE) {
      ((MenuChoose *)m_vMenuItem[m_iCurrent])->next();
    }
  }
}

void MenuSub::draw() {
  EM_COUT("MenuSub::draw() " << this->getText(), 1);
  p_Engine->clearScreen();

	if (p_Texture != NULL) p_Engine->drawSplash(p_Texture);
  
  float yoffset = 10 - ((float)this->size() + 2)/2;
  
  p_EmFont->printRowCenter(this->getText(), 0 + yoffset);
  
  vector<MenuItem*>::iterator menuIter = m_vMenuItem.begin();
  vector<MenuItem*>::iterator menuEnd = m_vMenuItem.end();
  for (int a=0; menuIter != menuEnd; menuIter++, a++) {
    if (a == m_iCurrent) {
      char str[256];
      strncpy(str, "> ", 16);
      strncat(str, (*menuIter)->getText(), 64);
      strncat(str, " <", 16);
      p_EmFont->printRowCenter(str, a+2 + yoffset);
    } else {
      //p_EmFont->printRowCenter((*menuIter)->getText(), a+2);
      p_EmFont->printRowCenter((*menuIter)->getText(), a+2 + yoffset);
    }
  }
  
  p_Engine->swap();
}

/*************************************************************************'
 * A meny where you can choose items */

MenuChoose::MenuChoose(Engine* e) : MenuItem(e, EM_MENU_CHOOSE) {
  m_iCurrent = 0;
  m_iPrevious = 0;
}

MenuChoose::~MenuChoose() {
}

const char* MenuChoose::getText() {
  if (m_vText.size() == 0) {
    return "";
  } else {
    // if not the active choice
    if (m_iCurrent != m_iPrevious) {
      strcpy(t_Str, "*");
      strncat(t_Str, m_vText[m_iCurrent], 61);
      strcat(t_Str, "*");
      return t_Str;
    } else {
      return m_vText[m_iCurrent];
    }
  }
}

void MenuChoose::addText(const char * text) {
  // TODO free these chars
  char * str = (char*) malloc(64*sizeof(char));
  strncpy(str, text, 63);
  m_vText.push_back(str);
}

int MenuChoose::perform() {
  EM_COUT("MenuSub::perform() " << this->getText(), 1);
  return this->next();
}

int MenuChoose::next() {
  EM_COUT("MenuSub::next() " << this->getText(), 1);
  if (m_vText.size() == 0) {
    return EM_MENU_NOP;
  }
  ++m_iCurrent;
  if (m_iCurrent >= (signed)m_vText.size()) m_iCurrent = 0;

  return EM_MENU_NOP;
}

int MenuChoose::prev() {
  EM_COUT("MenuSub::prev() " << this->getText(), 1);
  if (m_vText.size() == 0) {
    return EM_MENU_NOP;
  }
  --m_iCurrent;
  if (m_iCurrent < 0) m_iCurrent = (signed)m_vText.size()-1;

  return EM_MENU_NOP;
}


/*************************************************************************''
 * A menu that performs a function when choosen */

MenuFct::MenuFct(const char * name, int (*fct)(void), Engine* e) : MenuItem(e, EM_MENU_FCT) {
  strncpy(m_Name, name, 63);
  p_Fct = fct;
}

MenuFct::~MenuFct() {
}

int MenuFct::perform() {
  return p_Fct();
}
