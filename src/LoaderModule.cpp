//#ident "$Id: LoaderModule.cpp,v 1.3 2003/07/16 20:02:04 rzr Exp $" 
#ifndef LoaderModule_cpp_ 
#define LoaderModule_cpp_
/**
 * @author: Philippe.COVAL(a)IFrance.com - www.rzr.online.fr - Rev: $Author: rzr $ 
 * Load any beaviour from his filemane 
 * currently those plugins are C++ classes 
 * (build in a separate dyn lib or staticly with the main programm)
 * In the future it would be nice to load interpreted scripts as well 
 * (perl, pyton etc)
 * Toggle dynlib off / Workaround dynlib loading bugs  
 * (and keyboard bug that goes along on win32)
 **/
#include "Private.h"
#include "Config.h"
#include "Behavior.h"
#include "LoaderModule.h"
#include <map> 
#include <string> 
#include <iostream>
using namespace std;

#include "Behavior.h"
//-----------------------------------------------------------------------------
#ifndef RZR_LIBSTATIC
#include <ltdl.h>
#else
#define lt_dlhandle void*
#define lt_dlinit() 
#define lt_dlexit()
#define lt_dlerror()
#define lt_dlopen(a)
#define lt_dlsym(a,b)
#endif //!-rzr

#define new_object_fct new_object_fct_Tux
#include "../data/tux/ModuleTux.cpp"
#undef  new_object_fct 
#define new_object_fct new_object_fct_Professor
#include "../data/professor/ModuleProfessor.cpp"
#undef  new_object_fct
//-----------------------------------------------------------------------------
LoaderModule * LoaderModule::p_Instance = NULL;

LoaderModule::LoaderModule()
{
  lt_dlinit();
  m_hMods["libModuleTux.la"] = new TuxBehavior;
  m_hMods["libModuleProfessor.la"] = new ProfessorBehavior;
}
LoaderModule::~LoaderModule()
{
  lt_dlexit();
  EM_COUT("- LoaderModule::~LoaderModule",1);
}

LoaderModule* LoaderModule::getInstance()
{
  if ( p_Instance == NULL ) { p_Instance = new LoaderModule; }
  return p_Instance;
}
//-----------------------------------------------------------------------------

/// is it possible to do something 
/// like in java.lang.reflexion instead of protypes ?
Behavior* LoaderModule::readLibStatic(string & filename) 
{

  Behavior* beh = 0;
  //skip dir path
  string::size_type pos = filename.rfind('/', filename.length() ) ;
  if ( pos ) pos++;
  string name= filename.substr ( pos , filename.length() - pos );
  //cout<<"=="<<name<<pos<<endl; 
  beh = m_hMods[ (char* const) name.c_str() ];
  // TODO: if not found ?
  return beh;
}

///
Behavior* LoaderModule::readLibDynamic(string & filename) 
{
  EM_COUT("+ LoaderModule::readLibDynamic",0);
  Behavior * beh =0;
#ifndef RZR_LIBSTATIC
  lt_dlhandle handle = lt_dlopen(filename.c_str());
  if (handle == NULL) {
    throw (string("Could not open shared library: ") 
           + filename + " : " + string(lt_dlerror())); 
  } else {
    lt_ptr fct_ptr = lt_dlsym(handle, "new_object_fct");
    if (fct_ptr == NULL) {
      throw (string("Could not find symbol new_object_fct in library") 
             + string(lt_dlerror()));
    } else {
      beh = (Behavior*) ((void * (*)(void))fct_ptr)();
    }
  }
#endif
  EM_COUT("- LoaderModule::readLibDynamic",0);
  return beh;
}

Behavior* LoaderModule::read(string  & filename)
{
  EM_COUT("+ LoaderModule::read",0);
  //cerr<<filename<<endl;
#ifdef RZR_LIBSTATIC
  return readLibStatic(filename);
#else
  return readLibDynamic(filename);
#endif
  EM_COUT("- LoaderModule::read",0);
}
#endif // inclusion
//$Id: LoaderModule.cpp,v 1.3 2003/07/16 20:02:04 rzr Exp $
