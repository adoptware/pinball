/***************************************************************************
                          OpenGLTransVisitor.cpp  -  description
                             -------------------
    begin                : Sat Jan 6 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#include "OpenGLTransVisitor.h"

#include "Group.h"
#include "Shape3D.h"
#include "Polygon.h"
#include "BillBoard.h"

OpenGLTransVisitor::OpenGLTransVisitor(){
}

OpenGLTransVisitor::~OpenGLTransVisitor(){
}


void OpenGLTransVisitor::visit(Group* g) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(0);

	vector<Shape3D*>::iterator shapeIter = g->m_vShape3D.begin();
	vector<Shape3D*>::iterator shapeEnd = g->m_vShape3D.end();
	for ( ; shapeIter != shapeEnd; shapeIter++) {
		if (EM_SHAPE3D_HIDDEN & (*shapeIter)->m_iProperties) continue;
		if (!(EM_SHAPE3D_TRANS & (*shapeIter)->m_iProperties)) continue;

		// the shape has a texture
		if ((*shapeIter)->m_Texture != NULL) { 
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, *((*shapeIter)->m_Texture));

			if (g->m_iProperties & EM_GROUP_NO_LIGHT) {
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			} else {
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			}
		}

		// should some pixels be masked
		if (EM_SHAPE3D_ALPHATEST & (*shapeIter)->m_iProperties) {
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.05);
		}

		EM_COUT("OpenGLTransVisitor::visit() "<< (*shapeIter)->m_vPolygon.size()
						<<" polygons"<< endl, 0);
		// Clip, project and draw all polygons in Shape3D.
		vector<Polygon*>::iterator polyIter = (*shapeIter)->m_vPolygon.begin();
		vector<Polygon*>::iterator polyEnd = (*shapeIter)->m_vPolygon.end();
		for ( ; polyIter != polyEnd; polyIter++) {
			// if ((*polyIter)->m_iProperties & EM_POLY_HIDDEN) continue;
			// if (!((*polyIter)->m_iProperties & EM_POLY_TRANS)) continue;
			
			if ((*shapeIter)->m_Texture != NULL) { // textured polygon
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
			} else if ((*polyIter)->m_iProperties & EM_WIREFRAME) { // wireframe polygon
			} else { // color polygon
				glBegin(GL_POLYGON);
				vector<unsigned int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
				vector<unsigned int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
				vector<Color>::iterator colorIter = (*polyIter)->m_vColor.begin();
				vector<Color>::iterator colorEnd = (*polyIter)->m_vColor.end();

				EmAssert((*polyIter)->m_vIndex.size() == (*polyIter)->m_vColor.size(),
								 "size miss match");
				for ( ; indexIter != indexEnd; 
							indexIter++, colorIter++) {
#if OPENGL_LIGHTS
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
					glColor4f((*colorIter).r * (*shapeIter)->m_vLight[(*indexIter)].r,
										(*colorIter).g * (*shapeIter)->m_vLight[(*indexIter)].g,
										(*colorIter).b * (*shapeIter)->m_vLight[(*indexIter)].b,
										(*colorIter).a);
#endif
					glVertex3f((*shapeIter)->m_vVtxAlign[(*indexIter)].x,
										 (*shapeIter)->m_vVtxAlign[(*indexIter)].y,
										 (*shapeIter)->m_vVtxAlign[(*indexIter)].z);
				}
				glEnd();
			}
		}
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);
	}

	// billboard
	BillBoard * b = g->p_BillBoard;
	if (b != NULL && EM_BILLBOARD_TRANS & b->m_iProperties) {
		EmAssert(b->m_Texture != NULL, "all billboards must have a texture");

		// the texture
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, *(b->m_Texture));
		
		// blending not done yet
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		// should some pixels be masked
		if (EM_BILLBOARD_ALPHATEST & b->m_iProperties) {
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.05);
		}

		// a textured polygon
		glBegin(GL_POLYGON);
		glTexCoord2f(b->m_aTexCoord[0].u, b->m_aTexCoord[0].v);
		glVertex3f(b->m_vtxAlign.x - b->m_fSizexD2, 
							 b->m_vtxAlign.y + b->m_fSizeyD2, 
							 b->m_vtxAlign.z + b->m_fZOffset);
		glTexCoord2f(b->m_aTexCoord[1].u, b->m_aTexCoord[1].v);
		glVertex3f(b->m_vtxAlign.x + b->m_fSizexD2, 
							 b->m_vtxAlign.y + b->m_fSizeyD2, 
							 b->m_vtxAlign.z + b->m_fZOffset);
		glTexCoord2f(b->m_aTexCoord[2].u, b->m_aTexCoord[2].v);
		glVertex3f(b->m_vtxAlign.x + b->m_fSizexD2, 
							 b->m_vtxAlign.y - b->m_fSizeyD2, 
							 b->m_vtxAlign.z + b->m_fZOffset);
		glTexCoord2f(b->m_aTexCoord[3].u, b->m_aTexCoord[3].v);
		glVertex3f(b->m_vtxAlign.x - b->m_fSizexD2, 
							 b->m_vtxAlign.y - b->m_fSizeyD2, 
							 b->m_vtxAlign.z + b->m_fZOffset);
		glEnd();

		EM_COUT("OpenGLVisitor::visit() BillBoard at " << b->m_vtxAlign.x <<" "<<
						b->m_vtxAlign.y <<" "<< b->m_vtxAlign.z, 0);

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);
	}

	glDepthMask(1);
	glDisable(GL_BLEND);
}

