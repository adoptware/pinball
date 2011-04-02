/***************************************************************************
                          Sound.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef SOUND_H
#define	SOUND_H

#include "EMath.h"
#include "SoundUtil.h"

class Group;

/** 3D Sound objects. */
class Sound {
	public:
		/** Sound( sample, distance ). Creates a sound object using the sample.
		 * The distance parameter defines the distance were sound is played at full volume.
	   * Create the sample using Allogros functions. */
		Sound(EmSample* sample, float dist, bool b3D = true);
		~Sound(){};
		void play(bool loop = true);
		void adjust();
		/** Group uses this mehtod when the Sound is added to the Group. */
		void setParent(Group* g);
	private:
		Vertex3D m_vtxSrc;
		Vertex3D m_vtxTrans;
		Vertex3D m_vtxAlign;
		EmSample* p_Sample;
		bool m_bLoop;
		float m_fDistance;
		Group* p_Parent;
		bool m_b3D;
};

#endif // SOUND_H
