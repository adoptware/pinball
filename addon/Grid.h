/***************************************************************************
                          Grid.h  -  description
                             -------------------
    begin                : Tue Feb 8 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#ifndef GRID_H
#define GRID_H

#include "Shape3D.h"

/** A simple grid. @see Shape3D */
class Grid : public Shape3D {
	public:
	  /**
	   * <p>Grid(Texture, SizeForEachStepX, SizeForEachStepY, Steps, SizeForEachBitmapStep, 
		 * r, g, b, a)</p>
	   * <pre>
	   *
	   *  v-sizex-v            Steps=3
	   *  ----------------------
	   *  |      |      |      |
	   *  |      |      |      |
	   *  ---------------------- <
	   *  |      |      |      | | sizey
	   *  |      |      |      | |
	   *  ---------------------- <
	   *  |      |      |      |
	   *  |      |      |      |
	   *  ---------------------- Steps=3
	   * </pre>
	   */
		Grid(EmTexture*, float sizex, float sizey, int steps, float bmStep,  
				 float fR=1.0, float fG=1.0, float fB=1.0, float fA=1.0);
};

#endif // GRID_H
