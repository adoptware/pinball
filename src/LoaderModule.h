//#ident "$Id: LoaderModule.h,v 1.1 2003/04/08 00:39:04 rzr Exp $" 
#ifndef LoaderModule_h_ 
#define LoaderModule_h_ 
#include <string> 
#include <map>
using namespace std;
class Beaviour;

///
struct ltstr
{
  bool operator()( char* const s1,  char* const s2)  const {
    return strcmp(s1, s2) < 0;
  }
};

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
class LoaderModule
{ 
public:
  ///
  LoaderModule();
  ///
  virtual ~LoaderModule();
  ///
  static LoaderModule* getInstance();
  ///
  Behavior* read(string & filename);
  ///
  Behavior* readLibStatic(string & filename) ;
  /// 
  Behavior* readLibDynamic(string & filename);

protected:
  ///
  map<char* const, Behavior* , ltstr> m_hMods;
  ///
  static LoaderModule * p_Instance;
};
#endif //_h_ inclusion 
//$Id: LoaderModule.h,v 1.1 2003/04/08 00:39:04 rzr Exp $
