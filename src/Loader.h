/***************************************************************************
                            Loader.h -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef LOADER_H
#define LOADER_H

#include <fstream>
#include <sstream>
#include <string>
#include <map>

class Group;
class Engine;
class Shape3D;
class Polygon;
class Behavior;

#define LOADER_FIRSTSIGNAL 10000
#define LOADER_FIRSTVARIABLE 20000

class Loader {
 protected:
  Loader();
 public:
  ~Loader();
  static Loader *  getInstance();
  /** Loads the file and adds all groups to the engine. This function will not
   * clear groups or signals from engine. I.e. you must first call Loader::clearSignalVariable()
   * and Engine::clear(). */
  int loadFile(const char * fn, Engine * engine);
  /** Set to false if you do not want to load modules (plugins) */ 
  void useModules(bool m) { m_bModules = m; };
  Group * loadStdObject(ifstream & file, istringstream & str, Engine * engine);
  Shape3D* loadShape3DChunk(ifstream & file, istringstream & str);
  /** Returns the identifier for the signal with name 's'.
   * If no signal is found a new identifier will be allocated. */
  int getSignal(const char * s);
  /** Returns the identifier for the variable with name 'v'.
   * If no variable is found a new identifier will be allocated. */
  int getVariable(const char * v);
  /** Gets name for signal 's'. Return NULL in case of no signal. */
  const char * getSignal(int s);
  /** Gets name for variable 'v'. Returns NULL in case of no variable. */
  const char * getVariable(int v);
  void clearSignalVariable();
 private:
#if EM_UNIT_TEST
  friend class LoaderTest;
#endif
  void readNextToken(ifstream & file, istringstream & ist, string & str);
  void readNextToken(ifstream & file, istringstream & ist, int & i);
  void readNextToken(ifstream & file, istringstream & ist, float & f);

  void readPolygon(ifstream & file, istringstream & ist, Shape3D* shape);
  void readTexture(ifstream & file, istringstream & ist,Shape3D* shape);
  void readPolygonEdge(ifstream & file, istringstream & ist,Polygon* poly);
  void readVertex(ifstream & file, istringstream & ist,Shape3D* shape);
  void readUnknown(ifstream & file, istringstream & ist);

  void loadProperties(ifstream & file, istringstream & ist,Group * g);
  void loadArmBehavior(ifstream & file, istringstream & ist,Group * group);
  void loadShape(ifstream & file, istringstream & ist, 
		 Engine * engine, Group * group, Behavior * beh);
  void loadAnimation(ifstream & file, istringstream & ist, 
		     Engine * engine, Group * group, Behavior * beh);
  void loadBehaviorLight(ifstream & file, istringstream & ist, 
			 Engine * engine, Group * group, Behavior * beh);
  void loadCaveBehavior(ifstream & file, istringstream & ist, Engine * engine, Group* group);
  void loadBumperBehavior(ifstream & file, istringstream & ist, Engine * engine, Group * group);
  void loadPlungerBehavior(ifstream & file, istringstream & ist, Engine * engine, Group * group);
  void loadStateItem(ifstream & file, istringstream & ist, 
		     Engine * engine, Group * group, Behavior * beh);
  void loadStateBehavior(ifstream & file, istringstream & ist, Engine * engine, Group * group);
  void loadScript(ifstream & file, istringstream & ist, Engine * engine, Group * group);
  void loadModule(ifstream & file, istringstream & ist, Engine * engine, Group * group);
  void loadMisc(ifstream & file, istringstream & ist, Engine * engine, Group * group, Behavior * beh);

  static Loader * p_Loader;
  bool m_bModules;
  int m_iLineNumber;
  int m_iNextSignal;
  int m_iNextVariable;
  map<string, int> m_hSignalInt;
  map<int, string> m_hSignalString;
  map<string, int> m_hVariableInt;
  map<int, string> m_hVariableString;
};

#endif // LOADER_H

