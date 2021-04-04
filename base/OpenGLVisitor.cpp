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

/* Optimization observations:
 * * Voodoo 3 System
 * glVertex3f is a bit fatser than glVertex3fv ~3%.
 * glVertex3f is faster then glVertexArray ~10-20%, really strange.
 */

OpenGLVisitor * OpenGLVisitor::p_OpenGLVisitor = NULL;

OpenGLVisitor::OpenGLVisitor()
  : RenderVisitor()
{
  m_iMode = EM_GL_GCOL_TEX;
  m_bOffset = false;
}

OpenGLVisitor::~OpenGLVisitor()
{
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
    m_iPoly = 0;
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

      if ((*shapeIter)->m_Texture != NULL && filter != -1) { // textured polygons	
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, *((*shapeIter)->m_Texture));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        if ((*shapeIter)->m_iProperties & EM_SHAPE3D_ALWAYSLIT) { // EM_SHAPE3D_ALWAYSLIT
          vector<Polygon3D*>::iterator polyIter = (*shapeIter)->m_vPolygon.begin();
          vector<Polygon3D*>::iterator polyEnd = (*shapeIter)->m_vPolygon.end();
          for ( ; polyIter != polyEnd; ++polyIter) {
            if ((*polyIter)->m_iProperties & EM_POLY_TRANS) continue;
#if EM_DEBUG
            ++m_iPoly;
#endif
            vector<int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
            vector<int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
            #ifdef HAVE_OPENGLES
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            float *verts = new float[(*polyIter)->m_vIndex.size() * 5];
            glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(float), NULL);
            glVertexPointer(3, GL_FLOAT, 5 * sizeof(float), (const float *)NULL + 2);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnableClientState(GL_VERTEX_ARRAY);
            for ( ; indexIter != indexEnd; indexIter++) {
              int i = indexIter - (*polyIter)->m_vIndex.begin();
              verts[i * 5]     = (*shapeIter)->m_vTexCoord[(*indexIter)].u;
              verts[i * 5 + 1] = (*shapeIter)->m_vTexCoord[(*indexIter)].v;
              glColor4f((*shapeIter)->m_vColor[(*indexIter)].r,
                        (*shapeIter)->m_vColor[(*indexIter)].g,
                        (*shapeIter)->m_vColor[(*indexIter)].b,
                        1.0f);
              verts[i * 5 + 2] = (*shapeIter)->m_vVtxAlign[(*indexIter)].x;
              verts[i * 5 + 3] = (*shapeIter)->m_vVtxAlign[(*indexIter)].y;
              verts[i * 5 + 4] = (*shapeIter)->m_vVtxAlign[(*indexIter)].z;
            }
            glBufferData(GL_ARRAY_BUFFER, (*polyIter)->m_vIndex.size() * 5 * sizeof(float), verts, GL_STATIC_DRAW);
            glDrawArrays(GL_TRIANGLE_FAN, 0, (*polyIter)->m_vIndex.size());
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            delete verts;
            glDeleteBuffers(1, &vbo);
            #else
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
            #endif
          }
        } else { // ! EM_SHAPE3D_ALWAYSLIT
          vector<Polygon3D*>::iterator polyIter = (*shapeIter)->m_vPolygon.begin();
          vector<Polygon3D*>::iterator polyEnd = (*shapeIter)->m_vPolygon.end();
          for ( ; polyIter != polyEnd; ++polyIter) {
            if ((*polyIter)->m_iProperties & EM_POLY_TRANS) continue;
#if EM_DEBUG
            ++m_iPoly;
#endif
            vector<int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
            vector<int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
            #ifdef HAVE_OPENGLES
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            float *verts = new float[(*polyIter)->m_vIndex.size() * 5];
            glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(float), NULL);
            glVertexPointer(3, GL_FLOAT, 5 * sizeof(float), (const float *)NULL + 2);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnableClientState(GL_VERTEX_ARRAY);
            for ( ; indexIter != indexEnd; indexIter++) {
              int i = indexIter - (*polyIter)->m_vIndex.begin();
              verts[i * 5]     = (*shapeIter)->m_vTexCoord[(*indexIter)].u;
              verts[i * 5 + 1] = (*shapeIter)->m_vTexCoord[(*indexIter)].v;
              glColor4f((*shapeIter)->m_vLight[(*indexIter)].r,
                        (*shapeIter)->m_vLight[(*indexIter)].g,
                        (*shapeIter)->m_vLight[(*indexIter)].b,
                        1.0f);
              verts[i * 5 + 2] = (*shapeIter)->m_vVtxAlign[(*indexIter)].x;
              verts[i * 5 + 3] = (*shapeIter)->m_vVtxAlign[(*indexIter)].y;
              verts[i * 5 + 4] = (*shapeIter)->m_vVtxAlign[(*indexIter)].z;
            }
            glBufferData(GL_ARRAY_BUFFER, (*polyIter)->m_vIndex.size() * 5 * sizeof(float), verts, GL_STATIC_DRAW);
            glDrawArrays(GL_TRIANGLE_FAN, 0, (*polyIter)->m_vIndex.size());
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            delete verts;
            glDeleteBuffers(1, &vbo);
            #else
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
            #endif
          }
        }
      } else { // if ((*shapeIter)->m_Texture... // color polygons
        glDisable(GL_TEXTURE_2D);
        // if statement moved outside for loop for performance
        if ((*shapeIter)->m_iProperties & EM_SHAPE3D_ALWAYSLIT) { // EM_SHAPE3D_ALWAYSLIT
          vector<Polygon3D*>::iterator polyIter = (*shapeIter)->m_vPolygon.begin();
          vector<Polygon3D*>::iterator polyEnd = (*shapeIter)->m_vPolygon.end();
          for ( ; polyIter != polyEnd; ++polyIter) {
            if ((*polyIter)->m_iProperties & EM_POLY_TRANS) continue;
#if EM_DEBUG
            ++m_iPoly;
#endif
            vector<int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
            vector<int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
            #ifdef HAVE_OPENGLES
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            float *verts = new float[(*polyIter)->m_vIndex.size() * 3];
            glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), NULL);
            glEnableClientState(GL_VERTEX_ARRAY);
            for ( ; indexIter != indexEnd; indexIter++) {
              int i = indexIter - (*polyIter)->m_vIndex.begin();
              glColor4f((*shapeIter)->m_vColor[(*indexIter)].r,
                        (*shapeIter)->m_vColor[(*indexIter)].g,
                        (*shapeIter)->m_vColor[(*indexIter)].b,
                        1.0f);
              verts[i * 3]     = (*shapeIter)->m_vVtxAlign[(*indexIter)].x;
              verts[i * 3 + 1] = (*shapeIter)->m_vVtxAlign[(*indexIter)].y;
              verts[i * 3 + 2] = (*shapeIter)->m_vVtxAlign[(*indexIter)].z;
            }
            glBufferData(GL_ARRAY_BUFFER, (*polyIter)->m_vIndex.size() * 3 * sizeof(float), verts, GL_STATIC_DRAW);
            glDrawArrays(GL_TRIANGLE_FAN, 0, (*polyIter)->m_vIndex.size());
            glDisableClientState(GL_VERTEX_ARRAY);
            delete verts;
            glDeleteBuffers(1, &vbo);
            #else
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
            #endif
          }
        } else { // ! EM_SHAPE3D_ALWAYSLIT
          vector<Polygon3D*>::iterator polyIter = (*shapeIter)->m_vPolygon.begin();
          vector<Polygon3D*>::iterator polyEnd = (*shapeIter)->m_vPolygon.end();
          for ( ; polyIter != polyEnd; ++polyIter) {
            if ((*polyIter)->m_iProperties & EM_POLY_TRANS) continue;
#if EM_DEBUG
            ++m_iPoly;
#endif
            vector<int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
            vector<int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
            #ifdef HAVE_OPENGLES
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            float *verts = new float[(*polyIter)->m_vIndex.size() * 3];
            glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), NULL);
            glEnableClientState(GL_VERTEX_ARRAY);
            for ( ; indexIter != indexEnd; indexIter++) {
              int i = indexIter - (*polyIter)->m_vIndex.begin();
              glColor4f((*shapeIter)->m_vLitColor[(*indexIter)].r,
                        (*shapeIter)->m_vLitColor[(*indexIter)].g,
                        (*shapeIter)->m_vLitColor[(*indexIter)].b,
                        1.0f);
              verts[i * 3]     = (*shapeIter)->m_vVtxAlign[(*indexIter)].x;
              verts[i * 3 + 1] = (*shapeIter)->m_vVtxAlign[(*indexIter)].y;
              verts[i * 3 + 2] = (*shapeIter)->m_vVtxAlign[(*indexIter)].z;
            }
            glBufferData(GL_ARRAY_BUFFER, (*polyIter)->m_vIndex.size() * 3 * sizeof(float), verts, GL_STATIC_DRAW);
            glDrawArrays(GL_TRIANGLE_FAN, 0, (*polyIter)->m_vIndex.size());
            glDisableClientState(GL_VERTEX_ARRAY);
            delete verts;
            glDeleteBuffers(1, &vbo);
            #else
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
            #endif
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
      #ifdef HAVE_OPENGLES
      GLuint vbo;
      glGenBuffers(1, &vbo);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      float verts[20];
      glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(float), NULL);
      glVertexPointer(3, GL_FLOAT, 5 * sizeof(float), (const float *)NULL + 2);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glEnableClientState(GL_VERTEX_ARRAY);
      glColor4f(1, 1, 1, 1);
      verts[0]  = 0; verts[1] = 0;
      verts[2]  = b->m_vtxAlign.x - b->m_fSizexD2;
      verts[3]  = b->m_vtxAlign.y + b->m_fSizeyD2;
      verts[4]  = b->m_vtxAlign.z;
      verts[5]  = 1; verts[6] = 0;
      verts[7]  = b->m_vtxAlign.x + b->m_fSizexD2;
      verts[8]  = b->m_vtxAlign.y + b->m_fSizeyD2;
      verts[9]  = b->m_vtxAlign.z;
      verts[10] = 1; verts[11] = 1;
      verts[12] = b->m_vtxAlign.x + b->m_fSizexD2;
      verts[13] = b->m_vtxAlign.y - b->m_fSizeyD2;
      verts[14] = b->m_vtxAlign.z;
      verts[15] = 0; verts[16] = 1;
      verts[17] = b->m_vtxAlign.x - b->m_fSizexD2;
      verts[18] = b->m_vtxAlign.y - b->m_fSizeyD2;
      verts[19] = b->m_vtxAlign.z;
      glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
      glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      glDeleteBuffers(1, &vbo);
      #else
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
      #endif
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
#if 0 // I used wireframes when showing some demos in school
      {
        // wireframe
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0, -1.0);
        vector<Polygon3D*>::iterator polyIter = (*shapeIter)->m_vPolygon.begin();
        vector<Polygon3D*>::iterator polyEnd = (*shapeIter)->m_vPolygon.end();
        for ( ; polyIter != polyEnd; ++polyIter) {
          ++m_iPoly;
          glColor3f(0.0f, 0.0f, 0.0f);
          vector<int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
          vector<int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
          glBegin(GL_LINE_LOOP);
          for ( ; indexIter != indexEnd; ++indexIter) {
            glVertex3f((*shapeIter)->m_vVtxAlign[(*indexIter)].x,
                       (*shapeIter)->m_vVtxAlign[(*indexIter)].y,
                       (*shapeIter)->m_vVtxAlign[(*indexIter)].z);
          }
          glEnd();
        }
        glDisable(GL_POLYGON_OFFSET_FILL);
      }
#endif

      if ((*shapeIter)->m_iProperties & EM_SHAPE3D_BEHIND) {
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0, 1.0);
      } else if ((*shapeIter)->m_iProperties & EM_SHAPE3D_BEHIND2) {
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0, 2.0);
      } else {
        glDisable(GL_POLYGON_OFFSET_FILL);
      }

      if ((*shapeIter)->m_Texture != NULL && filter != -1) { // textured polygon
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, *((*shapeIter)->m_Texture));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				
        if ((*shapeIter)->m_iProperties & EM_SHAPE3D_ALWAYSLIT) { // EM_SHAPE3D_ALWAYSLIT
          vector<Polygon3D*>::iterator polyIter = (*shapeIter)->m_vPolygon.begin();
          vector<Polygon3D*>::iterator polyEnd = (*shapeIter)->m_vPolygon.end();
          for ( ; polyIter != polyEnd; polyIter++) {
            if (!((*polyIter)->m_iProperties & EM_POLY_TRANS)) continue;
#if EM_DEBUG
            ++m_iPoly;
#endif
            // textured polygon
            vector<int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
            vector<int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
            #ifdef HAVE_OPENGLES
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            float *verts = new float[(*polyIter)->m_vIndex.size() * 5];
            glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(float), NULL);
            glVertexPointer(3, GL_FLOAT, 5 * sizeof(float), (const float *)NULL + 2);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnableClientState(GL_VERTEX_ARRAY);
            for ( ; indexIter != indexEnd; indexIter++) {
              int i = indexIter - (*polyIter)->m_vIndex.begin();
              verts[i * 5]     = (*shapeIter)->m_vTexCoord[(*indexIter)].u;
              verts[i * 5 + 1] = (*shapeIter)->m_vTexCoord[(*indexIter)].v;
              glColor4f((*shapeIter)->m_vColor[(*indexIter)].r,
                        (*shapeIter)->m_vColor[(*indexIter)].g,
                        (*shapeIter)->m_vColor[(*indexIter)].b,
                        (*shapeIter)->m_vColor[(*indexIter)].a);
              verts[i * 5 + 2] = (*shapeIter)->m_vVtxAlign[(*indexIter)].x;
              verts[i * 5 + 3] = (*shapeIter)->m_vVtxAlign[(*indexIter)].y;
              verts[i * 5 + 4] = (*shapeIter)->m_vVtxAlign[(*indexIter)].z;
            }
            glBufferData(GL_ARRAY_BUFFER, (*polyIter)->m_vIndex.size() * 5 * sizeof(float), verts, GL_STATIC_DRAW);
            glDrawArrays(GL_TRIANGLE_FAN, 0, (*polyIter)->m_vIndex.size());
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            delete verts;
            glDeleteBuffers(1, &vbo);
            #else
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
            #endif
          }
        } else { // ! EM_SHAPE3D_ALWAYSLIT
          vector<Polygon3D*>::iterator polyIter = (*shapeIter)->m_vPolygon.begin();
          vector<Polygon3D*>::iterator polyEnd = (*shapeIter)->m_vPolygon.end();
          for ( ; polyIter != polyEnd; polyIter++) {
            if (!((*polyIter)->m_iProperties & EM_POLY_TRANS)) continue;
#if EM_DEBUG
            ++m_iPoly;
#endif
            // textured polygon
            vector<int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
            vector<int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
            #ifdef HAVE_OPENGLES
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            float *verts = new float[(*polyIter)->m_vIndex.size() * 5];
            glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(float), NULL);
            glVertexPointer(3, GL_FLOAT, 5 * sizeof(float), (const float *)NULL + 2);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnableClientState(GL_VERTEX_ARRAY);
            for ( ; indexIter != indexEnd; indexIter++) {
              int i = indexIter - (*polyIter)->m_vIndex.begin();
              verts[i * 5]     = (*shapeIter)->m_vTexCoord[(*indexIter)].u;
              verts[i * 5 + 1] = (*shapeIter)->m_vTexCoord[(*indexIter)].v;
              glColor4f((*shapeIter)->m_vLight[(*indexIter)].r,
                        (*shapeIter)->m_vLight[(*indexIter)].g,
                        (*shapeIter)->m_vLight[(*indexIter)].b,
                        (*shapeIter)->m_vLight[(*indexIter)].a);
              verts[i * 5 + 2] = (*shapeIter)->m_vVtxAlign[(*indexIter)].x;
              verts[i * 5 + 3] = (*shapeIter)->m_vVtxAlign[(*indexIter)].y;
              verts[i * 5 + 4] = (*shapeIter)->m_vVtxAlign[(*indexIter)].z;
            }
            glBufferData(GL_ARRAY_BUFFER, (*polyIter)->m_vIndex.size() * 5 * sizeof(float), verts, GL_STATIC_DRAW);
            glDrawArrays(GL_TRIANGLE_FAN, 0, (*polyIter)->m_vIndex.size());
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            delete verts;
            glDeleteBuffers(1, &vbo);
            #else
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
            #endif
          }
        }
      } else { // color polygon
        glDisable(GL_TEXTURE_2D);
        if ((*shapeIter)->m_iProperties & EM_SHAPE3D_ALWAYSLIT) {
          vector<Polygon3D*>::iterator polyIter = (*shapeIter)->m_vPolygon.begin();
          vector<Polygon3D*>::iterator polyEnd = (*shapeIter)->m_vPolygon.end();
          for ( ; polyIter != polyEnd; polyIter++) {
            if (!((*polyIter)->m_iProperties & EM_POLY_TRANS)) continue;
#if EM_DEBUG
            ++m_iPoly;
#endif
            vector<int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
            vector<int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
            #ifdef HAVE_OPENGLES
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            float *verts = new float[(*polyIter)->m_vIndex.size() * 3];
            glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), NULL);
            glEnableClientState(GL_VERTEX_ARRAY);
            for ( ; indexIter != indexEnd; indexIter++) {
              int i = indexIter - (*polyIter)->m_vIndex.begin();
              glColor4f((*shapeIter)->m_vColor[(*indexIter)].r,
                        (*shapeIter)->m_vColor[(*indexIter)].g,
                        (*shapeIter)->m_vColor[(*indexIter)].b,
                        (*shapeIter)->m_vColor[(*indexIter)].a);
              verts[i * 3]     = (*shapeIter)->m_vVtxAlign[(*indexIter)].x;
              verts[i * 3 + 1] = (*shapeIter)->m_vVtxAlign[(*indexIter)].y;
              verts[i * 3 + 2] = (*shapeIter)->m_vVtxAlign[(*indexIter)].z;
            }
            glBufferData(GL_ARRAY_BUFFER, (*polyIter)->m_vIndex.size() * 3 * sizeof(float), verts, GL_STATIC_DRAW);
            glDrawArrays(GL_TRIANGLE_FAN, 0, (*polyIter)->m_vIndex.size());
            glDisableClientState(GL_VERTEX_ARRAY);
            delete verts;
            glDeleteBuffers(1, &vbo);
            #else
            glBegin(GL_POLYGON);
            for ( ; indexIter != indexEnd; indexIter++) {
              // transparent polygons should not be lit, or ?
              glColor4f((*shapeIter)->m_vColor[(*indexIter)].r,
                        (*shapeIter)->m_vColor[(*indexIter)].g,
                        (*shapeIter)->m_vColor[(*indexIter)].b,
                        (*shapeIter)->m_vColor[(*indexIter)].a);
              glVertex3f((*shapeIter)->m_vVtxAlign[(*indexIter)].x,
                         (*shapeIter)->m_vVtxAlign[(*indexIter)].y,
                         (*shapeIter)->m_vVtxAlign[(*indexIter)].z);
            }
            glEnd();
            #endif
          }
        } else { // EM_SHAPE3D_ALWAYSLIT
          vector<Polygon3D*>::iterator polyIter = (*shapeIter)->m_vPolygon.begin();
          vector<Polygon3D*>::iterator polyEnd = (*shapeIter)->m_vPolygon.end();
          for ( ; polyIter != polyEnd; polyIter++) {
            if (!((*polyIter)->m_iProperties & EM_POLY_TRANS)) continue;
#if EM_DEBUG
            ++m_iPoly;
#endif
            vector<int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
            vector<int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
            #ifdef HAVE_OPENGLES
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            float *verts = new float[(*polyIter)->m_vIndex.size() * 3];
            glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), NULL);
            glEnableClientState(GL_VERTEX_ARRAY);
            for ( ; indexIter != indexEnd; indexIter++) {
              int i = indexIter - (*polyIter)->m_vIndex.begin();
              glColor4f((*shapeIter)->m_vLitColor[(*indexIter)].r,
                        (*shapeIter)->m_vLitColor[(*indexIter)].g,
                        (*shapeIter)->m_vLitColor[(*indexIter)].b,
                        (*shapeIter)->m_vLitColor[(*indexIter)].a);
              verts[i * 3]     = (*shapeIter)->m_vVtxAlign[(*indexIter)].x;
              verts[i * 3 + 1] = (*shapeIter)->m_vVtxAlign[(*indexIter)].y;
              verts[i * 3 + 2] = (*shapeIter)->m_vVtxAlign[(*indexIter)].z;
            }
            glBufferData(GL_ARRAY_BUFFER, (*polyIter)->m_vIndex.size() * 3 * sizeof(float), verts, GL_STATIC_DRAW);
            glDrawArrays(GL_TRIANGLE_FAN, 0, (*polyIter)->m_vIndex.size());
            glDisableClientState(GL_VERTEX_ARRAY);
            delete verts;
            glDeleteBuffers(1, &vbo);
            #else
            glBegin(GL_POLYGON);
            for ( ; indexIter != indexEnd; indexIter++) {
              // transparent polygons should not be lit, or ?
              glColor4f((*shapeIter)->m_vLitColor[(*indexIter)].r,
                        (*shapeIter)->m_vLitColor[(*indexIter)].g,
                        (*shapeIter)->m_vLitColor[(*indexIter)].b,
                        (*shapeIter)->m_vLitColor[(*indexIter)].a);
              glVertex3f((*shapeIter)->m_vVtxAlign[(*indexIter)].x,
                         (*shapeIter)->m_vVtxAlign[(*indexIter)].y,
                         (*shapeIter)->m_vVtxAlign[(*indexIter)].z);
            }
            glEnd();
            #endif
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
      #ifdef HAVE_OPENGLES
      GLuint vbo;
      glGenBuffers(1, &vbo);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      float verts[20];
      glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(float), NULL);
      glVertexPointer(3, GL_FLOAT, 5 * sizeof(float), (const float *)NULL + 2);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glEnableClientState(GL_VERTEX_ARRAY);
      glColor4f(1, 1, 1, 1);
      verts[0]  = 0; verts[1] = 0;
      verts[2]  = b->m_vtxAlign.x - b->m_fSizexD2;
      verts[3]  = b->m_vtxAlign.y + b->m_fSizeyD2;
      verts[4]  = b->m_vtxAlign.z;
      verts[5]  = 1; verts[6] = 0;
      verts[7]  = b->m_vtxAlign.x + b->m_fSizexD2;
      verts[8]  = b->m_vtxAlign.y + b->m_fSizeyD2;
      verts[9]  = b->m_vtxAlign.z;
      verts[10] = 1; verts[11] = 1;
      verts[12] = b->m_vtxAlign.x + b->m_fSizexD2;
      verts[13] = b->m_vtxAlign.y - b->m_fSizeyD2;
      verts[14] = b->m_vtxAlign.z;
      verts[15] = 0; verts[16] = 1;
      verts[17] = b->m_vtxAlign.x - b->m_fSizexD2;
      verts[18] = b->m_vtxAlign.y - b->m_fSizeyD2;
      verts[19] = b->m_vtxAlign.z;
      glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
      glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      glDeleteBuffers(1, &vbo);
      #else
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
      #endif
      EM_COUT_D("OpenGLVisitor::visit() BillBoard at " << b->m_vtxAlign.x <<" "<<
                b->m_vtxAlign.y <<" "<< b->m_vtxAlign.z, 0);
    }
  } break;

  }
#endif // EM_USE_SDL
}

