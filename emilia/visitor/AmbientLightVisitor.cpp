/***************************************************************************
                          GVisitor.cpp  -  description
                             -------------------
    begin                : Wed Jan 26 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/


#include "AmbientLightVisitor.h"
#include "Group.h"
#include "Shape3D.h"
#include "Polygon.h"

AmbientLightVisitor::AmbientLightVisitor() {
	m_fStrength = (float)0.1;
	m_fBackground = (float)0.0;
}

AmbientLightVisitor::~AmbientLightVisitor() {
}

void AmbientLightVisitor::setLightning(float s, float bg) {
	m_fStrength = s;
	m_fBackground = bg;
#if OPENGL_LIGHTS
	GLfloat amb[] = {bg, bg, bg, 1};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,amb);

	glEnable(GL_LIGHT0);
	
	// create light
	GLfloat pos[] = {0.0f, 10000.0f, 0.0f, 0.0f};
	GLfloat light[] = {s, s, s, 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light);
#endif
}

void AmbientLightVisitor::visit(Group* g) {
	EM_COUT("AmbientLightVisitor::visit()" << endl, 0);
#if OPENGL_LIGHTS
#else
	// Check properties before applying light
	if (g->m_iProperties & EM_GROUP_NO_LIGHT) return;
	
	/* This one i done in PLightVisitor
	if (g->m_iProperties & EM_GROUP_LIGHT_ONCE) {
		g->unsetProperty(EM_GROUP_LIGHT_ONCE);
		g->setProperty(EM_GROUP_NO_LIGHT);
	}
	*/

	vector<Shape3D*>::iterator shapeIter = g->m_vShape3D.begin();
	vector<Shape3D*>::iterator shapeEnd = g->m_vShape3D.end();
	for ( ; shapeIter != shapeEnd; shapeIter++) {
		EM_COUT("AmbientLightVisitor::visit() shape lights " << 
						(*shapeIter)->m_vLight.size() << " normal " << 
						(*shapeIter)->m_vNmlTrans.size() << endl, 0);
		// Count light for all vertices.
		vector<Color>::iterator lightIter = (*shapeIter)->m_vLight.begin();
		vector<Color>::iterator lightEnd = (*shapeIter)->m_vLight.end();
		vector<Color>::iterator specularIter = (*shapeIter)->m_vSpecular.begin();
		vector<Color>::iterator speculartEnd = (*shapeIter)->m_vSpecular.end();
		vector<Vertex3D>::iterator nmlTransIter = (*shapeIter)->m_vNmlTrans.begin();
		vector<Vertex3D>::iterator nmlTransEnd = (*shapeIter)->m_vNmlTrans.end();
		vector<Vertex3D>::iterator nmlAlignIter = (*shapeIter)->m_vNmlAlign.begin();
		vector<Vertex3D>::iterator nmlAlignEnd = (*shapeIter)->m_vNmlAlign.end();

		EmAssert(((*shapeIter)->m_vLight.size() == (*shapeIter)->m_vSpecular.size()) &&
						 ((*shapeIter)->m_vLight.size() == (*shapeIter)->m_vNmlTrans.size()),
						 "size missmatch");
		for ( ; lightIter != lightEnd; 
					lightIter++, nmlTransIter++, nmlAlignIter++, specularIter++) {
			// Count angle between normal and y-axis.
			float fAngle = (*nmlTransIter).y;
			float fLight = fAngle/2 + 0.5;
			//			float fLight = 0.5;
			(*lightIter).r = fLight * m_fStrength + m_fBackground;
			(*lightIter).r = EM_MIN(1.0, (*lightIter).r);
			(*lightIter).g = (*lightIter).b = (*lightIter).r;
			EM_COUT("Light " << (*lightIter).r << " "	<< (*lightIter).g << " " << 
							(*lightIter).b << endl, 0);
			// specular light
			if (((*shapeIter)->m_iProperties & EM_SPECULAR) == EM_SPECULAR) {
				Vertex3D vtxRef = {0,0,-1};
				Vertex3D vtxDir = {0,-1,0};
				EMath::reflection(vtxDir, (*nmlAlignIter), vtxRef, false);
				EMath::normalizeVector(vtxRef); // TODO: optimize - remove normalize
				//float view = EMath::dotProduct((*nmlAlignIter), vtxView); // TODO: optimize
				float spe = EM_MAX(0.0f, vtxRef.z);
				float spe2 = spe * spe;
				float spe4 = spe2 * spe2;
				(*specularIter).r = (*specularIter).g = (*specularIter).b = spe4 * spe4 * 0.5;
			} else {
				(*specularIter).r = (*specularIter).g = (*specularIter).b = 0;
			}
		}
	}
#endif // OPENGL_LIGHTS
}	
