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
	glDisable(GL_TEXTURE_2D);
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

