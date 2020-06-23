/***************************************************************************
                          Script.h  -  description
                             -------------------
    begin                : Sat Aug 24 2002
    copyright            : (C) 2002 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef SCRIPT_H
#define SCRIPT_H

#include <vector>
#include <map>

#include "Pinball.h"
#include "Behavior.h"
#include "StateMachine.h"

#define EM_SCRIPT_ONSIGNAL   1
#define EM_SCRIPT_IFVAR      2
#define EM_SCRIPT_SENDSIGNAL 3
#define EM_SCRIPT_SETVAR     4
#define EM_SCRIPT_PLAYSOUND  5

/** A query item for the scripting engine. */
class QueryItem {
 public:
  QueryItem() {
    m_iQuery = -1;
    m_iAction = -1;
  };
  ~QueryItem() {};
  void setQuery(int q) { m_iQuery = q; };
  void setAction(int a) { m_iAction = a; };
  void addQueryParm(int p) { m_vQueryParm.push_back(p); };
  void addActionParm(int p) { m_vActionParm.push_back(p); };
  
  int m_iQuery;
  int m_iAction;
  vector<int> m_vQueryParm;
  vector<int> m_vActionParm;
};

/** A scripting engine. */
class Script : public Behavior {
 public:
  Script() {
    this->setType(PBL_TYPE_SCRIPTBEH);
  };
  ~Script() {};
  void onTick() {};
  void onSignal(int signal, Group* group);
  void StdOnCollision() {};
  void addQueryItem(QueryItem * qi) { m_vQueryItem.push_back(qi); };
  /** Set variable with identifier 'i' to the value of 'v'. If no variable
   * exist a new will be created. */
  void setVariable(int i, int v);
  /** Returns the vaule of variable 'i'. If no variable exist it returns 0. */
  int getVariable(int i);
  void interpret(QueryItem * queryitem, int signal, Group * group);
  void execute(QueryItem * queryitem);
  
  vector<QueryItem*> m_vQueryItem;
  map<int, int> m_hVariable;
};

#endif // SCRIPT_H
