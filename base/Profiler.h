/***************************************************************************
                          Profiler.h  -  description
                             -------------------
    begin                : Tue Jul 30 2001
    copyright            : (C) 2002 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#ifndef PROFILER_H
#define PROFILER_H


#define COUNT 1
#define ALIGN 2
#define BEH 3
#define GLIGHT 4
#define COLLISION 5
#define PLIGHT 6
#define PNORMAL 7
#define RENDER 8
#define SOUND 9
#define SIG 10
#define TRANS 11
#define DRAW 13
#define SWAP 14
#define KEY 15
#define RENDER_OUT 16
#define TICK_OUT 17
#define SWAP_OUT 18
#define WAIT 19


/** Singleton class for quick and dirty profiling */
class Profiler {
 protected:
	Profiler();
 public:
	~Profiler();
	static Profiler* getInstance();
	void startProfile(int i);
	void stopProfile();
	void printProfile();
 private:
	unsigned int m_aProfile[256];
	unsigned int m_iStart;
	unsigned int m_iCurrentProfile;
	unsigned int m_iCurrentTime;
	static Profiler* p_Profiler;
};

#endif
