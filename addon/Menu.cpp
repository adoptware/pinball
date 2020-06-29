//#ident "$Id: Menu.cpp,v 1.14 2003/06/18 10:43:45 henqvist Exp $"
/***************************************************************************
                          Menu.cpp  -  description
                             -------------------
    begin                : Tue Feb 15 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
***************************************************************************/

#include <cstring>
#include <iostream>

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
  strncpy(m_Name, name, MAX_MENU_NAME);
  m_iCurrent = 0;
  m_iAction = EM_MENU_NOP;
  strcpy(m_BottomText, "");
}

MenuSub::~MenuSub() {
}

void MenuSub::addMenuItem(MenuItem * menu) {
  m_vMenuItem.push_back(menu);
}

int MenuSub::perform() {
  EM_COUT("MenuSub::perform() " << this->getText(), 1);
  
  if (m_vMenuItem.size() == 0) {
    return m_iAction;
  }
  
  int ret = 0;
  
  EMKey key = SDL_SCANCODE_UNKNOWN;
  while(true) {
    if (key == SDLK_DOWN) m_iCurrent++;
    if (key == SDLK_UP) m_iCurrent--;
    
    if (m_iCurrent < 0) m_iCurrent = m_vMenuItem.size()-1;
    if (m_iCurrent >= (signed)m_vMenuItem.size()) m_iCurrent = 0;
    
    this->draw();
    
    key = Keyboard::waitForKey();
    // escape, exit current menu (go back to previous)
    if (key == SDLK_ESCAPE) {
      return EM_MENU_NOP;
    }
    if (key == SDLK_LALT) {
	return EM_MENU_EXIT;
    }

    // menu chossen, do something
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

void MenuSub::addInfoText(const char * text) {
  char * str = (char*) malloc((MAX_MENU_NAME+1)*sizeof(char));
  strncpy(str, text, MAX_MENU_NAME);
  m_vInfoText.push_back(str);
}

void MenuSub::setBottomText(const char * text) {
  strncpy(m_BottomText, text, MAX_MENU_NAME);
}

void MenuSub::draw() {
  EM_COUT("MenuSub::draw() " << this->getText(), 1);
  p_Engine->clearScreen();
  if (p_Texture != NULL) p_Engine->drawSplash(p_Texture);
  
  int size = this->size() + m_vInfoText.size() + 2;
  float yoffset = 10 - ((float)size)/2;
  
  p_EmFont->printRowCenter(this->getText(), 0 + yoffset);
  vector<char*>::iterator iter = m_vInfoText.begin();
  vector<char*>::iterator end = m_vInfoText.end();
  for (int a=1; iter != end; ++iter, ++a) {
    p_EmFont->printRowCenter((*iter), a + yoffset);
  }
  p_EmFont->printRowCenter(m_BottomText, -2);
  
  vector<MenuItem*>::iterator menuIter = m_vMenuItem.begin();
  vector<MenuItem*>::iterator menuEnd = m_vMenuItem.end();
  { for (int a=0; menuIter != menuEnd; menuIter++, a++) {
    if (a == m_iCurrent) {
      char str[256];
      strncpy(str, "> ", 16);
      strncat(str, (*menuIter)->getText(), MAX_MENU_NAME);
      strncat(str, " <", 16);
      p_EmFont->printRowCenter(str, a+2 + yoffset);
    } else {
      p_EmFont->printRowCenter((*menuIter)->getText(), a+2 + yoffset);
    }
  } }
  
  p_Engine->swap();
}

/**************************************************************************
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
  char * str = (char*) malloc((MAX_MENU_NAME+1)*sizeof(char));
  strncpy(str, text, MAX_MENU_NAME);
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


/***************************************************************************
 * A menu that performs a function when choosen */

MenuFct::MenuFct(const char * name, int (*fct)(void), Engine* e)
  : MenuItem(e, EM_MENU_FCT) {
  strncpy(m_Name, name, MAX_MENU_NAME);
  p_Fct = fct;
}

MenuFct::~MenuFct() {
}

int MenuFct::perform() {
  return p_Fct();
}

/**************************************************************************
 * A meny item to input text */

#define FILL_CHAR       '.'    // Char used to fill the input line so the
                               //  user can see the max input length
#define DEFAULT_STRING  "?"    // Default name for those who don't give name

MenuInput::MenuInput(const char * name, Engine* e) : MenuItem(e, EM_MENU_INPUT) {
  strncpy(m_Name, name, MAX_MENU_NAME);
  memset(m_Input, FILL_CHAR,  MAX_INPUT_STRING);
  m_Input[MAX_INPUT_STRING] = '\0';

  m_iAction = EM_MENU_BACK;
}

MenuInput::~MenuInput() {
}

const char* MenuInput::getInput() {
  return m_Input;
}

void MenuInput::draw() {
  EM_COUT("MenuSub::draw() " << this->getText(), 1);

  p_Engine->clearScreen();

  if (p_Texture != NULL)
    p_Engine->drawSplash(p_Texture);
  
  p_EmFont->printRowCenter(this->getText(),   8);
  p_EmFont->printRowCenter(this->getInput(), 10);
  
  p_Engine->swap();
}

int MenuInput::perform() {
  EM_COUT("MenuInput::perform() " << this->getText(), 1);
  this->draw();

  int iLetter = 0;
  while (iLetter < MAX_INPUT_STRING) {
    EMKey key = Keyboard::waitForKey();
    
    // If return or escape then end input
    if (key == SDLK_RETURN || key == SDLK_ESCAPE) {
      break;
    } else if (key == SDLK_BACKSPACE || key == SDLK_DELETE) {
      // Delete previous char, replacing it by the FILL_CHAR
      if (iLetter > 0) {
	iLetter--;
	m_Input[iLetter] = FILL_CHAR;
      }
    } else {
      m_Input[iLetter] = key;
      iLetter++;
    }
    EM_COUT("MenuInput::perform() input " << this->getInput(), 1);
    this->draw();
  }
  
  // Clear the FILL_CHAR at the end of the input string, if the result is
  //  empty replace it by an '?'
  int i = 0;
  if (strlen(m_Input) > 0) {
    // Stop at first FILL_CHAR
    for (i=0; i<strlen(m_Input); i++) {
      if (m_Input[i] == FILL_CHAR) break;
    }
    
    // Terminate string at first FILL_CHAR
    m_Input[i] = '\0';
  }
  if (strlen(m_Input) == 0) {
    strcpy(m_Input, DEFAULT_STRING);
  }

  return m_iAction;;
}
//EOF: $Id: Menu.cpp,v 1.14 2003/06/18 10:43:45 henqvist Exp $
