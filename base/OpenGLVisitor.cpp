/***************************************************************************
                          OpenGLVisitor.cpp  -  description
                             -------------------
    begin                : Sat Jan 6 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#include "Private.h"
#include "OpenGLVisitor.h"
#include "Group.h"
#include "Shape3D.h"
#include "Polygon.h"
#include "BillBoard.h"
#include "Config.h"

#ifndef EM_VERTEXARRAY
#define EM_VERTEXARRAY 0
#endif

#ifndef EM_OPENGL_LIGHTS
#define EM_OPENGL_LIGHTS 0
#endif

#if EM_DEBUG
volatile int em_poly = 0;
volatile float em_poly_m = 0;
#endif

/* Optimization observations:
 * * Voodoo 3 System
 * glVertex3f is a bit fatser than glVertex3fv ~3%.
 * glVertex3f is faster then glVertexArray ~10-20%, really strange.
 */

OpenGLVisitor * OpenGLVisitor::p_OpenGLVisitor = NULL;

OpenGLVisitor::OpenGLVisitor(){
	m_iMode = EM_GL_GCOL_TEX;
	m_bOffset = false;
}

OpenGLVisitor::~OpenGLVisitor(){
	p_OpenGLVisitor = NULL;
}

OpenGLVisitor * OpenGLVisitor::getInstance() {
	if (p_OpenGLVisitor == NULL) {
		p_OpenGLVisitor = new OpenGLVisitor();
	}
	return p_OpenGLVisitor;
}

void OpenGLVisitor::empty() {
#if EM_USE_SDL
#if EM_VERTEXARRAY
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
#endif
	
	switch (m_iMode) {
	case EM_GL_GCOL_TEX: {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		//glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
		glDepthMask(GL_TRUE);

	} break;
	case EM_GL_GCOL_TEX_TRANS: {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		//glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		// 		glEnable(GL_ALPHA_TEST);
		// 		glAlphaFunc(GL_GREATER, 0.05);
	} break;
	case EM_GL_CLEAN: {
#if EM_DEBUG
		em_poly_m = em_poly_m*0.0f + ((float)em_poly)*1.0f;
		em_poly = 0;
#endif
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	} break;
	}
#endif
}

void OpenGLVisitor::visit(Group* g) {
#if EM_USE_SDL
	int filter = Config::getInstance()->getGLFilter();
	//bool lights = Config::getInstance()->useLights();

	switch (m_iMode) {
	case EM_GL_GCOL_TEX: {
		// shapes
		vector<Shape3D*>::iterator shapeIter = g->m_vShape3D.begin();
		vector<Shape3D*>::iterator shapeEnd = g->m_vShape3D.end();
		for ( ; shapeIter != shapeEnd; shapeIter++) {
			if (EM_SHAPE3D_HIDDEN & (*shapeIter)->m_iProperties) continue;

			if ((*shapeIter)->m_iProperties & EM_SHAPE3D_BEHIND) {
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(1.0, 1.0);
			} else if ((*shapeIter)->m_iProperties & EM_SHAPE3D_BEHIND2) {
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(1.0, 2.0);
			} else {
				glDisable(GL_POLYGON_OFFSET_FILL);
			}

			vector<Polygon*>::iterator polyIter = (*shapeIter)->m_vPolygon.begin();
			vector<Polygon*>::iterator polyEnd = (*shapeIter)->m_vPolygon.end();

			if ((*shapeIter)->m_Texture != NULL && filter != -1) {
				// textured polygons	
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, *((*shapeIter)->m_Texture));
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

				if ((*shapeIter)->m_iProperties & EM_SHAPE3D_ALLWAYSLIT) {
					for ( ; polyIter != polyEnd; ++polyIter) {
						if ((*polyIter)->m_iProperties & EM_POLY_TRANS) continue;
#if EM_DEBUG
						++em_poly;
#endif
						vector<int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
						vector<int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
						glBegin(GL_POLYGON);
						for ( ; indexIter != indexEnd; ++indexIter) {
							glTexCoord2f((*shapeIter)->m_vTexCoord[(*indexIter)].u,
													 (*shapeIter)->m_vTexCoord[(*indexIter)].v);
							glColor3f((*shapeIter)->m_vColor[(*indexIter)].r,
												(*shapeIter)->m_vColor[(*indexIter)].g,
												(*shapeIter)->m_vColor[(*indexIter)].b);
							glVertex3f((*shapeIter)->m_vVtxAlign[(*indexIter)].x,
												 (*shapeIter)->m_vVtxAlign[(*indexIter)].y,
												 (*shapeIter)->m_vVtxAlign[(*indexIter)].z);
						}
						glEnd();
					}
				} else { // EM_SHAPE3D_ALLWAYSLIT
					for ( ; polyIter != polyEnd; ++polyIter) {
						if ((*polyIter)->m_iProperties & EM_POLY_TRANS) continue;
#if EM_DEBUG
						++em_poly;
#endif
						vector<int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
						vector<int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
						glBegin(GL_POLYGON);
						for ( ; indexIter != indexEnd; ++indexIter) {
							glTexCoord2f((*shapeIter)->m_vTexCoord[(*indexIter)].u,
													 (*shapeIter)->m_vTexCoord[(*indexIter)].v);
							// Question should textured polygons have specular?
							glColor3f((*shapeIter)->m_vLight[(*indexIter)].r,
												(*shapeIter)->m_vLight[(*indexIter)].g,
												(*shapeIter)->m_vLight[(*indexIter)].b);
							glVertex3f((*shapeIter)->m_vVtxAlign[(*indexIter)].x,
												 (*shapeIter)->m_vVtxAlign[(*indexIter)].y,
												 (*shapeIter)->m_vVtxAlign[(*indexIter)].z);
						}
						glEnd();
					}
				}
			} else { // if ((*shapeIter)->m_Texture...
				// color polygons
				glDisable(GL_TEXTURE_2D);
				// if statement moved outside for loop for performance
				if ((*shapeIter)->m_iProperties & EM_SHAPE3D_ALLWAYSLIT) {
					for ( ; polyIter != polyEnd; ++polyIter) {
						if ((*polyIter)->m_iProperties & EM_POLY_TRANS) continue;
#if EM_DEBUG
						em_poly++;
#endif
						vector<int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
						vector<int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
						glBegin(GL_POLYGON);
						for (; indexIter != indexEnd; ++indexIter) {
							glColor3f((*shapeIter)->m_vColor[(*indexIter)].r, 
												(*shapeIter)->m_vColor[(*indexIter)].g, 
												(*shapeIter)->m_vColor[(*indexIter)].b);
							glVertex3f((*shapeIter)->m_vVtxAlign[(*indexIter)].x,
												 (*shapeIter)->m_vVtxAlign[(*indexIter)].y,
												 (*shapeIter)->m_vVtxAlign[(*indexIter)].z);
						}
						glEnd();
					}
				} else { // EM_SHAPE3D_ALLWAYSLIT
					for ( ; polyIter != polyEnd; ++polyIter) {
						if ((*polyIter)->m_iProperties & EM_POLY_TRANS) continue;
#if EM_DEBUG
						em_poly++;
#endif
						vector<int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
						vector<int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
						glBegin(GL_POLYGON);
						for (; indexIter != indexEnd; ++indexIter) {
							glColor3f((*shapeIter)->m_vLitColor[(*indexIter)].r, 
												(*shapeIter)->m_vLitColor[(*indexIter)].g, 
												(*shapeIter)->m_vLitColor[(*indexIter)].b);
						glVertex3f((*shapeIter)->m_vVtxAlign[(*indexIter)].x,
											 (*shapeIter)->m_vVtxAlign[(*indexIter)].y,
											 (*shapeIter)->m_vVtxAlign[(*indexIter)].z);
						}
						glEnd();
					}
				}
			}
		}
		// billboard
		BillBoard * b = g->p_BillBoard;
		if (b != NULL && !(b->m_iProperties & EM_BILLBOARD_TRANS)) {
			EmAssert(b->m_Texture != NULL, "all billboards must have a texture");

			glBindTexture(GL_TEXTURE_2D, *(b->m_Texture));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

			// a textured polygon
			glColor3f(1, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(b->m_vtxAlign.x - b->m_fSizexD2, 
								 b->m_vtxAlign.y + b->m_fSizeyD2, 
								 b->m_vtxAlign.z);
			glTexCoord2f(1, 0);
			glVertex3f(b->m_vtxAlign.x + b->m_fSizexD2, 
								 b->m_vtxAlign.y + b->m_fSizeyD2, 
								 b->m_vtxAlign.z);
			glTexCoord2f(1, 1);
			glVertex3f(b->m_vtxAlign.x + b->m_fSizexD2, 
								 b->m_vtxAlign.y - b->m_fSizeyD2, 
								 b->m_vtxAlign.z);
			glTexCoord2f(0, 1);
			glVertex3f(b->m_vtxAlign.x - b->m_fSizexD2, 
								 b->m_vtxAlign.y - b->m_fSizeyD2, 
								 b->m_vtxAlign.z);
			glEnd();
			
			EM_COUT_D("OpenGLVisitor::visit() BillBoard at " << b->m_vtxAlign.x <<" "<<
								b->m_vtxAlign.y <<" "<< b->m_vtxAlign.z, 0);
		}
	} break;
	case EM_GL_GCOL_TEX_TRANS: {
		vector<Shape3D*>::iterator shapeIter = g->m_vShape3D.begin();
		vector<Shape3D*>::iterator shapeEnd = g->m_vShape3D.end();
		for ( ; shapeIter != shapeEnd; shapeIter++) {
			if (EM_SHAPE3D_HIDDEN & (*shapeIter)->m_iProperties) continue;
			if (!(EM_SHAPE3D_USE_TRANS & (*shapeIter)->m_iProperties)) continue;

			if ((*shapeIter)->m_iProperties & EM_SHAPE3D_BEHIND) {
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(1.0, 1.0);
			} else if ((*shapeIter)->m_iProperties & EM_SHAPE3D_BEHIND2) {
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(1.0, 2.0);
			} else {
				glDisable(GL_POLYGON_OFFSET_FILL);
			}

			vector<Polygon*>::iterator polyIter = (*shapeIter)->m_vPolygon.begin();
			vector<Polygon*>::iterator polyEnd = (*shapeIter)->m_vPolygon.end();
			if ((*shapeIter)->m_Texture != NULL && filter != -1) {
				// textured polygon
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, *((*shapeIter)->m_Texture));
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				
				vector<Polygon*>::iterator polyIter = (*shapeIter)->m_vPolygon.begin();
				vector<Polygon*>::iterator polyEnd = (*shapeIter)->m_vPolygon.end();
				if ((*shapeIter)->m_iProperties & EM_SHAPE3D_ALLWAYSLIT) {
					for ( ; polyIter != polyEnd; polyIter++) {
						if (!((*polyIter)->m_iProperties & EM_POLY_TRANS)) continue;
#if EM_DEBUG
						em_poly++;
#endif
						// textured polygon
						vector<int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
						vector<int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
						glBegin(GL_POLYGON);
						for ( ; indexIter != indexEnd; indexIter++) {
							glTexCoord2f((*shapeIter)->m_vTexCoord[(*indexIter)].u, 
													 (*shapeIter)->m_vTexCoord[(*indexIter)].v);
							glColor4f((*shapeIter)->m_vColor[(*indexIter)].r,
												(*shapeIter)->m_vColor[(*indexIter)].g,
												(*shapeIter)->m_vColor[(*indexIter)].b,
												(*shapeIter)->m_vColor[(*indexIter)].a);
							glVertex3f((*shapeIter)->m_vVtxAlign[(*indexIter)].x,
												 (*shapeIter)->m_vVtxAlign[(*indexIter)].y,
												 (*shapeIter)->m_vVtxAlign[(*indexIter)].z);
						}
						glEnd();
					}
				} else { // EM_SHAPE3D_ALLWAYSLIT
					for ( ; polyIter != polyEnd; polyIter++) {
						if (!((*polyIter)->m_iProperties & EM_POLY_TRANS)) continue;
#if EM_DEBUG
						em_poly++;
#endif
						// textured polygon
						vector<int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
						vector<int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
						glBegin(GL_POLYGON);
						for ( ; indexIter != indexEnd; indexIter++) {
							glTexCoord2f((*shapeIter)->m_vTexCoord[(*indexIter)].u, 
													 (*shapeIter)->m_vTexCoord[(*indexIter)].v);
							glColor4f((*shapeIter)->m_vLight[(*indexIter)].r,
												(*shapeIter)->m_vLight[(*indexIter)].g,
												(*shapeIter)->m_vLight[(*indexIter)].b,
												(*shapeIter)->m_vLight[(*indexIter)].a);
							glVertex3f((*shapeIter)->m_vVtxAlign[(*indexIter)].x,
												 (*shapeIter)->m_vVtxAlign[(*indexIter)].y,
												 (*shapeIter)->m_vVtxAlign[(*indexIter)].z);
						}
						glEnd();
					}
				}
			} else {
				// color polygon
				if ((*shapeIter)->m_iProperties & EM_SHAPE3D_ALLWAYSLIT) {
					for ( ; polyIter != polyEnd; polyIter++) {
						if (!((*polyIter)->m_iProperties & EM_POLY_TRANS)) continue;
#if EM_DEBUG
						em_poly++;
#endif
						vector<int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
						vector<int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
						glBegin(GL_POLYGON);
						for ( ; indexIter != indexEnd; indexIter++) {
							/* transparent polygons should not be lit, or ? */
							glColor4f((*shapeIter)->m_vColor[(*indexIter)].r,
												(*shapeIter)->m_vColor[(*indexIter)].g,
												(*shapeIter)->m_vColor[(*indexIter)].b,
												(*shapeIter)->m_vColor[(*indexIter)].a);
							glVertex3f((*shapeIter)->m_vVtxAlign[(*indexIter)].x,
												 (*shapeIter)->m_vVtxAlign[(*indexIter)].y,
												 (*shapeIter)->m_vVtxAlign[(*indexIter)].z);
						}
						glEnd();
					}
				} else { // EM_SHAPE3D_ALLWAYSLIT
					for ( ; polyIter != polyEnd; polyIter++) {
						if (!((*polyIter)->m_iProperties & EM_POLY_TRANS)) continue;
#if EM_DEBUG
						em_poly++;
#endif
						vector<int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
						vector<int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
						glBegin(GL_POLYGON);
						for ( ; indexIter != indexEnd; indexIter++) {
							/* transparent polygons should not be lit, or ? */
							glColor4f((*shapeIter)->m_vLitColor[(*indexIter)].r,
												(*shapeIter)->m_vLitColor[(*indexIter)].g,
												(*shapeIter)->m_vLitColor[(*indexIter)].b,
												(*shapeIter)->m_vLitColor[(*indexIter)].a);
							glVertex3f((*shapeIter)->m_vVtxAlign[(*indexIter)].x,
												 (*shapeIter)->m_vVtxAlign[(*indexIter)].y,
												 (*shapeIter)->m_vVtxAlign[(*indexIter)].z);
						}
						glEnd();
					}
				}
			}
		}
		// billboard
		BillBoard * b = g->p_BillBoard;
		if (b != NULL && b->m_iProperties & EM_BILLBOARD_TRANS) {
			EmAssert(b->m_Texture != NULL, "all billboards must have a texture");

			glBindTexture(GL_TEXTURE_2D, *(b->m_Texture));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

			// a textured polygon
			glColor3f(1, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(b->m_vtxAlign.x - b->m_fSizexD2, 
								 b->m_vtxAlign.y + b->m_fSizeyD2, 
								 b->m_vtxAlign.z);
			glTexCoord2f(1, 0);
			glVertex3f(b->m_vtxAlign.x + b->m_fSizexD2, 
								 b->m_vtxAlign.y + b->m_fSizeyD2, 
								 b->m_vtxAlign.z);
			glTexCoord2f(1, 1);
			glVertex3f(b->m_vtxAlign.x + b->m_fSizexD2, 
								 b->m_vtxAlign.y - b->m_fSizeyD2, 
								 b->m_vtxAlign.z);
			glTexCoord2f(0, 1);
			glVertex3f(b->m_vtxAlign.x - b->m_fSizexD2, 
								 b->m_vtxAlign.y - b->m_fSizeyD2, 
								 b->m_vtxAlign.z);
			glEnd();
			
			EM_COUT_D("OpenGLVisitor::visit() BillBoard at " << b->m_vtxAlign.x <<" "<<
								b->m_vtxAlign.y <<" "<< b->m_vtxAlign.z, 0);
		}
	} break;

	}
#endif // EM_USE_SDL
}

/*****************************************************************************************/

#if 0







	case EM_GL_TEX_TRANS: {
		if (filter == -1) break;

// 		glEnable(GL_BLEND);
// 		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// 		glDepthMask(GL_FALSE);

		vector<Shape3D*>::iterator shapeIter = g->m_vShape3D.begin();
		vector<Shape3D*>::iterator shapeEnd = g->m_vShape3D.end();
		for ( ; shapeIter != shapeEnd; shapeIter++) {
			if ((*shapeIter)->m_Texture == NULL) continue;
			if (EM_SHAPE3D_HIDDEN & (*shapeIter)->m_iProperties) continue;
			if (!(EM_SHAPE3D_USE_TRANS & (*shapeIter)->m_iProperties)) continue;

			if ((*shapeIter)->m_iProperties & EM_SHAPE3D_BEHIND) {
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(1.0, 1.0);
			} else if ((*shapeIter)->m_iProperties & EM_SHAPE3D_BEHIND2) {
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(1.0, 2.0);
			} else {
				glDisable(GL_POLYGON_OFFSET_FILL);
			}

 			glBindTexture(GL_TEXTURE_2D, *((*shapeIter)->m_Texture));
 			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
 			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
 			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

// 			glEnable(GL_ALPHA_TEST);
// 			glAlphaFunc(GL_GREATER, 0.05);

			// Clip, project and draw all polygons in Shape3D.
			vector<Polygon*>::iterator polyIter = (*shapeIter)->m_vPolygon.begin();
			vector<Polygon*>::iterator polyEnd = (*shapeIter)->m_vPolygon.end();
			for ( ; polyIter != polyEnd; polyIter++) {
				// if ((*polyIter)->m_iProperties & EM_POLY_HIDDEN) continue;
				if (!((*polyIter)->m_iProperties & EM_POLY_TRANS)) continue;
#if EM_DEBUG
				em_poly++;
#endif
				// textured polygon
				vector<unsigned int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
				vector<unsigned int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
				vector<Color>::iterator colorIter = (*polyIter)->m_vColor.begin();
				vector<Color>::iterator colorEnd = (*polyIter)->m_vColor.end();
				vector<TexCoord>::iterator texIter = (*polyIter)->m_vTexCoord.begin();
				vector<TexCoord>::iterator texEnd = (*polyIter)->m_vTexCoord.end();
				glBegin(GL_POLYGON);
				for ( ; indexIter != indexEnd && texIter != texEnd && colorIter != colorEnd; 
							indexIter++, texIter++, colorIter++) {
					glTexCoord2f((*texIter).u, (*texIter).v);
					glColor3f((*colorIter).r * (*shapeIter)->m_vLight[(*indexIter)].r,
										(*colorIter).g * (*shapeIter)->m_vLight[(*indexIter)].g,
										(*colorIter).b * (*shapeIter)->m_vLight[(*indexIter)].b);
					glVertex3f((*shapeIter)->m_vVtxAlign[(*indexIter)].x,
										 (*shapeIter)->m_vVtxAlign[(*indexIter)].y,
										 (*shapeIter)->m_vVtxAlign[(*indexIter)].z);
				}
				glEnd();
			}
		}
		
		// billboard
		BillBoard * b = g->p_BillBoard;
		if (b != NULL && b->m_iProperties & EM_BILLBOARD_TRANS) {
			EmAssert(b->m_Texture != NULL, "all billboards must have a texture");

			glBindTexture(GL_TEXTURE_2D, *(b->m_Texture));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

			// a textured polygon
			glColor3f(1, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(b->m_vtxAlign.x - b->m_fSizexD2, b->m_vtxAlign.y + b->m_fSizeyD2, b->m_vtxAlign.z);
			glTexCoord2f(1, 0);
			glVertex3f(b->m_vtxAlign.x + b->m_fSizexD2, b->m_vtxAlign.y + b->m_fSizeyD2, b->m_vtxAlign.z);
			glTexCoord2f(1, 1);
			glVertex3f(b->m_vtxAlign.x + b->m_fSizexD2, b->m_vtxAlign.y - b->m_fSizeyD2, b->m_vtxAlign.z);
			glTexCoord2f(0, 1);
			glVertex3f(b->m_vtxAlign.x - b->m_fSizexD2, b->m_vtxAlign.y - b->m_fSizeyD2, b->m_vtxAlign.z);
			glEnd();
			
			EM_COUT_D("OpenGLVisitor::visit() BillBoard at " << b->m_vtxAlign.x <<" "<<
								b->m_vtxAlign.y <<" "<< b->m_vtxAlign.z, 0);
		}
	} break;






	case EM_GL_TEX: {
		if (filter == -1) break;
		
		vector<Shape3D*>::iterator shapeIter = g->m_vShape3D.begin();
		vector<Shape3D*>::iterator shapeEnd = g->m_vShape3D.end();
		
		for ( ; shapeIter != shapeEnd; shapeIter++) {
			if ((*shapeIter)->m_Texture == NULL) continue;
			if ((*shapeIter)->m_iProperties & EM_SHAPE3D_HIDDEN) continue;

			if ((*shapeIter)->m_iProperties & EM_SHAPE3D_BEHIND) {
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(1.0, 1.0);
			} else if ((*shapeIter)->m_iProperties & EM_SHAPE3D_BEHIND2) {
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(1.0, 2.0);
			} else {
				glDisable(GL_POLYGON_OFFSET_FILL);
			}

			glBindTexture(GL_TEXTURE_2D, *((*shapeIter)->m_Texture));
 			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
 			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
 			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			
			vector<Polygon*>::iterator polyIter = (*shapeIter)->m_vPolygon.begin();
			vector<Polygon*>::iterator polyEnd = (*shapeIter)->m_vPolygon.end();
			for ( ; polyIter != polyEnd; ++polyIter) {
				// if ((*polyIter)->m_iProperties & EM_POLY_HIDDEN) continue;
				if ((*polyIter)->m_iProperties & EM_POLY_TRANS) continue;
#if EM_DEBUG
				em_poly++;
#endif
				vector<unsigned int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
				vector<unsigned int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
				vector<Color>::iterator colorIter = (*polyIter)->m_vColor.begin();
				vector<Color>::iterator colorEnd = (*polyIter)->m_vColor.end();
				vector<TexCoord>::iterator texIter = (*polyIter)->m_vTexCoord.begin();
				vector<TexCoord>::iterator texEnd = (*polyIter)->m_vTexCoord.end();
				
				glBegin(GL_POLYGON);
				for ( ; indexIter != indexEnd; 
							++indexIter, ++texIter, ++colorIter) {
					glTexCoord2f((*texIter).u, (*texIter).v);
					glColor3f((*colorIter).r * (*shapeIter)->m_vLight[(*indexIter)].r,
										(*colorIter).g * (*shapeIter)->m_vLight[(*indexIter)].g,
										(*colorIter).b * (*shapeIter)->m_vLight[(*indexIter)].b);
					glVertex3f((*shapeIter)->m_vVtxAlign[(*indexIter)].x,
										 (*shapeIter)->m_vVtxAlign[(*indexIter)].y,
										 (*shapeIter)->m_vVtxAlign[(*indexIter)].z);
				}
				glEnd();
			}
		}
		
		// billboard
		BillBoard * b = g->p_BillBoard;
		if (b != NULL && !(b->m_iProperties & EM_BILLBOARD_TRANS)) {
			EmAssert(b->m_Texture != NULL, "all billboards must have a texture");

			// no blending
			glBindTexture(GL_TEXTURE_2D, *(b->m_Texture));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			
			// should some pixels be masked
			//if (EM_BILLBOARD_ALPHATEST & b->m_iProperties) {
			//	glEnable(GL_ALPHA_TEST);
			//	glAlphaFunc(GL_GREATER, 0.05);
			//}
#if EM_DEBUG
				em_poly++;
#endif
			// a textured polygon
			glColor3f(1, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(b->m_vtxAlign.x - b->m_fSizexD2, b->m_vtxAlign.y + b->m_fSizeyD2, b->m_vtxAlign.z);
			glTexCoord2f(1, 0);
			glVertex3f(b->m_vtxAlign.x + b->m_fSizexD2, b->m_vtxAlign.y + b->m_fSizeyD2, b->m_vtxAlign.z);
			glTexCoord2f(1, 1);
			glVertex3f(b->m_vtxAlign.x + b->m_fSizexD2, b->m_vtxAlign.y - b->m_fSizeyD2, b->m_vtxAlign.z);
			glTexCoord2f(0, 1);
			glVertex3f(b->m_vtxAlign.x - b->m_fSizexD2, b->m_vtxAlign.y - b->m_fSizeyD2, b->m_vtxAlign.z);
			glEnd();
			
			EM_COUT_D("OpenGLVisitor::visit() BillBoard at " << b->m_vtxAlign.x <<" "<<
								b->m_vtxAlign.y <<" "<< b->m_vtxAlign.z, 0);
		}
	} break;












	// shapes
	vector<Shape3D*>::iterator shapeIter = g->m_vShape3D.begin();
	vector<Shape3D*>::iterator shapeEnd = g->m_vShape3D.end();
	for ( ; shapeIter != shapeEnd; shapeIter++) {
		if (EM_SHAPE3D_HIDDEN & (*shapeIter)->m_iProperties) continue;
		//if (EM_SHAPE3D_USE_TRANS & (*shapeIter)->m_iProperties) continue;

		// the shape has a texture
		int filter = Config::getInstance()->getGLFilter();
		if ((*shapeIter)->m_Texture != NULL && filter != -1) { 
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, *((*shapeIter)->m_Texture));

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			if (g->m_iProperties & EM_GROUP_NO_LIGHT) {
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			} else {
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			}
		}

#if EM_VERTEXARRAY
		glVertexPointer(3, GL_FLOAT, 0, (GLvoid*)(*shapeIter)->m_vVtxAlign.begin());
#endif

		// should some pixels be masked
		if (EM_SHAPE3D_ALPHATEST & (*shapeIter)->m_iProperties) {
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.05);
		}

		// TODO Flat polygons
// 		if ((*shapeIter)->m_iProperties & EM_SHAPE3D_FLAT) {
// 			glShadeModel(GL_FLAT);
// 		} else {
// 			glShadeModel(GL_SMOOTH);
// 		}

		EM_COUT_D("OpenGLVisitor::visit() "<< (*shapeIter)->m_vPolygon.size() <<" polygons", 0);
		// Clip, project and draw all polygons in Shape3D.
		vector<Polygon*>::iterator polyIter = (*shapeIter)->m_vPolygon.begin();
		vector<Polygon*>::iterator polyEnd = (*shapeIter)->m_vPolygon.end();
		for ( ; polyIter != polyEnd; polyIter++) {
			// if ((*polyIter)->m_iProperties & EM_POLY_HIDDEN) continue;
			if ((*polyIter)->m_iProperties & EM_POLY_TRANS) continue;

			/*
			if ((*polyIter)->m_iProperties & EM_POLY_CCW_VIEW) {
				glFrontFace(GL_CCW);
			} else {
				glFrontFace(GL_CW);
			}
			*/
#if EM_DEBUG
			em_poly++;
#endif

			if ((*shapeIter)->m_Texture != NULL && filter != -1) { 
				// textured polygon
				vector<unsigned int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
				vector<unsigned int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
				vector<Color>::iterator colorIter = (*polyIter)->m_vColor.begin();
				vector<Color>::iterator colorEnd = (*polyIter)->m_vColor.end();
				vector<TexCoord>::iterator texIter = (*polyIter)->m_vTexCoord.begin();
				vector<TexCoord>::iterator texEnd = (*polyIter)->m_vTexCoord.end();
				glBegin(GL_POLYGON);
				for ( ; indexIter != indexEnd && texIter != texEnd && colorIter != colorEnd; 
							indexIter++, texIter++, colorIter++) {
#if OPENGL_LIGHTS
					glNormal3f((*shapeIter)->m_vNmlTrans[(*indexIter)].x,
										 (*shapeIter)->m_vNmlTrans[(*indexIter)].y,
										 (*shapeIter)->m_vNmlTrans[(*indexIter)].z);
#endif
					glTexCoord2f((*texIter).u, (*texIter).v);
					glColor3f((*colorIter).r * (*shapeIter)->m_vLight[(*indexIter)].r,
										(*colorIter).g * (*shapeIter)->m_vLight[(*indexIter)].g,
										(*colorIter).b * (*shapeIter)->m_vLight[(*indexIter)].b);
					glVertex3f((*shapeIter)->m_vVtxAlign[(*indexIter)].x,
										 (*shapeIter)->m_vVtxAlign[(*indexIter)].y,
										 (*shapeIter)->m_vVtxAlign[(*indexIter)].z);
				}
				glEnd();
			} else if ((*polyIter)->m_iProperties & EM_WIREFRAME) { 
				//wireframe polygon
				vector<unsigned int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
				vector<unsigned int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
				vector<Color>::iterator colorIter = (*polyIter)->m_vColor.begin();
				vector<Color>::iterator colorEnd = (*polyIter)->m_vColor.end();

				EmAssert((*polyIter)->m_vIndex.size() == (*polyIter)->m_vColor.size(),
								 "size missmatch");
				glBegin(GL_LINE_LOOP);
				for ( ; indexIter != indexEnd; 	indexIter++, colorIter++) {
					glColor3f((*colorIter).r * (*shapeIter)->m_vLight[(*indexIter)].r,
										(*colorIter).g * (*shapeIter)->m_vLight[(*indexIter)].g,
										(*colorIter).b * (*shapeIter)->m_vLight[(*indexIter)].b);
					glVertex3f((*shapeIter)->m_vVtxAlign[(*indexIter)].x,
										 (*shapeIter)->m_vVtxAlign[(*indexIter)].y,
										 (*shapeIter)->m_vVtxAlign[(*indexIter)].z);
				}
				glEnd();
			} else { 
				// color polygon
				//glEnableClientState(GL_COLOR_ARRAY);

#if EM_VERTEXARRAY
				glDrawElements(GL_POLYGON, (*polyIter)->m_vIndex.size(), GL_UNSIGNED_INT, 
											 (GLvoid*)(*polyIter)->m_vIndex.begin());
#else
				vector<unsigned int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
				vector<unsigned int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
				vector<Color>::iterator colorIter = (*polyIter)->m_vColor.begin();
				vector<Color>::iterator colorEnd = (*polyIter)->m_vColor.end();

				glBegin(GL_POLYGON);
				for ( ; indexIter != indexEnd; indexIter++, colorIter++) {
#if EM_OPENGL_LIGHTS
					// stupid materials!
					glMaterialfv(GL_FRONT, GL_AMBIENT, (GLfloat*)&(*colorIter));
					glMaterialfv(GL_FRONT, GL_DIFFUSE, (GLfloat*)&(*colorIter));
					//					glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat*)&(*colorIter));
					//GLfloat mat[] = {1, 0, 0, 1}; 
					//glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
					glNormal3f((*shapeIter)->m_vNmlTrans[(*indexIter)].x,
										 (*shapeIter)->m_vNmlTrans[(*indexIter)].y,
										 (*shapeIter)->m_vNmlTrans[(*indexIter)].z);
#else
// 					if ((*shapeIter)->m_iProperties & EM_SHAPE3D_FLAT) {
// 						glColor3f((*colorIter).r * (*polyIter)->m_colFlatLight.r, 
// 											(*colorIter).g * (*polyIter)->m_colFlatLight.g,
// 											(*colorIter).b * (*polyIter)->m_colFlatLight.b);
// 					} else 
 					glColor3f((*colorIter).r * (*shapeIter)->m_vLight[(*indexIter)].r +	
 										(*shapeIter)->m_vSpecular[(*indexIter)].r,
 										(*colorIter).g * (*shapeIter)->m_vLight[(*indexIter)].g +
 										(*shapeIter)->m_vSpecular[(*indexIter)].g,
 										(*colorIter).b * (*shapeIter)->m_vLight[(*indexIter)].b + 
 										(*shapeIter)->m_vSpecular[(*indexIter)].b);

//					glColor3fv((GLfloat*)(colorIter));
//					glColor3f((*colorIter).r, (*colorIter).g, (*colorIter).b);
					
//					glVertex3fv((GLfloat*)&(*shapeIter)->m_vVtxAlign[(*indexIter)]);
					glVertex3f((*shapeIter)->m_vVtxAlign[(*indexIter)].x,
										 (*shapeIter)->m_vVtxAlign[(*indexIter)].y,
										 (*shapeIter)->m_vVtxAlign[(*indexIter)].z);
#endif

					EM_COUT_D("Specular " << (*shapeIter)->m_vSpecular[(*indexIter)].r << " "
									<< (*shapeIter)->m_vSpecular[(*indexIter)].g << " "
									<< (*shapeIter)->m_vSpecular[(*indexIter)].b << endl, 0);
					EM_COUT_D("Light " << (*shapeIter)->m_vLight[(*indexIter)].r << " "
									<< (*shapeIter)->m_vLight[(*indexIter)].g << " "
									<< (*shapeIter)->m_vLight[(*indexIter)].b << endl, 0);
					EM_COUT_D("Vertex " << (*shapeIter)->m_vVtxAlign[(*indexIter)].x << " " 
									<< (*shapeIter)->m_vVtxAlign[(*indexIter)].y << " " 
									<< (*shapeIter)->m_vVtxAlign[(*indexIter)].z << endl, 0);
				}
				glEnd();
#endif
			}
		}

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);
	}

	// billboard
	BillBoard * b = g->p_BillBoard;
	if (b != NULL && !(EM_BILLBOARD_TRANS & b->m_iProperties)) {
		EmAssert(b->m_Texture != NULL, "all billboards must have a texture");

		// the texture
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, *(b->m_Texture));
		
		int filter = Config::getInstance()->getGLFilter();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// no blending
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		// should some pixels be masked
		if (EM_BILLBOARD_ALPHATEST & b->m_iProperties) {
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.05);
		}

		// a textured polygon
		glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);
		glVertex3f(b->m_vtxAlign.x - b->m_fSizexD2, 
							 b->m_vtxAlign.y + b->m_fSizeyD2, 
							 b->m_vtxAlign.z);
		glTexCoord2f(1, 0);
		glVertex3f(b->m_vtxAlign.x + b->m_fSizexD2, 
							 b->m_vtxAlign.y + b->m_fSizeyD2, 
							 b->m_vtxAlign.z);
		glTexCoord2f(1, 1);
		glVertex3f(b->m_vtxAlign.x + b->m_fSizexD2, 
							 b->m_vtxAlign.y - b->m_fSizeyD2, 
							 b->m_vtxAlign.z);
		glTexCoord2f(0, 1);
		glVertex3f(b->m_vtxAlign.x - b->m_fSizexD2, 
							 b->m_vtxAlign.y - b->m_fSizeyD2, 
							 b->m_vtxAlign.z);
		glEnd();

		EM_COUT_D("OpenGLVisitor::visit() BillBoard at " << b->m_vtxAlign.x <<" "<<
						b->m_vtxAlign.y <<" "<< b->m_vtxAlign.z, 0);

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);
	}
#endif // 0


