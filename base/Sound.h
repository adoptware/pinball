/***************************************************************************
                          Sound.h  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef SOUND_H
#define	SOUND_H

#include "Private.h"
#include "EMath.h"

class Group;

/**
 * <p>Sound.</p>
 */
class Sound {
	public:
		/** <p>Sound( sample, distance ). Creates a sound object using the sample.
		 * The distance parameter defines the distance were sound is played at full volume.
	   * Create the sample using Allogros functions.</p> */
		Sound(EmSample* sample, float dist, bool b3D = true);
		~Sound(){};
		void play(bool loop = true);
		void adjust();
		/** <p>Group uses this mehtod when the Sound is added to the Group.</p> */
		void setParent(Group* g);
	private:
		Vertex3D m_vtxSrc;
		Vertex3D m_vtxTrans;
		Vertex3D m_vtxAlign;
		EmSample* m_Sample;
		bool m_bLoop;
		float m_fDistance;
		Group* p_Parent;
		bool m_b3D;
};

#endif // SOUND_H
