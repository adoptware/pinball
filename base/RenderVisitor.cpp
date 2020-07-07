// -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*-
// SPDX-License-Identifier: GPL-2.0+

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "Private.h"
#include "RenderVisitor.h"
#include "OpenGLVisitor.h"
#include "AllegroVisitor.h"


using namespace std;


RenderVisitor* RenderVisitor::getInstance()
{
  static RenderVisitor* pInstance = NULL;

  if (!pInstance) {
    char* text = getenv("PINBALL_RENDER_DRIVER");
    if (text) {
      cerr << "PINBALL_RENDER_DRIVER=\""<< text<<"\""<<endl;
      if ((0 == strcmp(text, "dummy"))) {
	pInstance =  new RenderVisitor;
      }
    } else {
#if EM_USE_SDL
      pInstance = OpenGLVisitor::getInstance();
#elif EM_USE_ALLEGRO
      pInstance = AllegroVisitor::getInstance();
#else
      pInstance = new RenderVisitor;
#endif    
    }
  }

  return pInstance;
}
