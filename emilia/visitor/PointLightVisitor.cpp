/***************************************************************************
                          PointLightVisitor.cpp  -  description
                             -------------------
    begin                : Sun Jan 30 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#include "PointLightVisitor.h"
#include "Light.h"
#include "Shape3D.h"
#include "Group.h"
#include "EMath.h"
#include "Polygon.h"

PointLightVisitor::PointLightVisitor(int size = 4) {
	m_vLight.reserve(size);
}

PointLightVisitor::~PointLightVisitor() {
}

void PointLightVisitor::add(Light* l) {
	if (l == NULL) return;
	m_vLight.push_back(l);
}

void PointLightVisitor::visit(Group* g) {
	// Check properties before applying light
	if (g->m_iProperties & EM_GROUP_NO_LIGHT) return;
	
	if (g->m_iProperties & EM_GROUP_LIGHT_ONCE) {
		g->unsetProperty(EM_GROUP_LIGHT_ONCE);
		g->setProperty(EM_GROUP_NO_LIGHT);
	}

	vector<Shape3D*>::iterator iter = g->m_vShape3D.begin();
	vector<Shape3D*>::iterator end = g->m_vShape3D.end();
	for ( ; iter != end; iter++) {
		this->visit((*iter), g);
	}
}

void PointLightVisitor::visit(Shape3D* s, Group* g) {
	vector<Light*>::iterator lightIter = m_vLight.begin();
	vector<Light*>::iterator lightEnd = m_vLight.end();
	EM_COUT("PointLightVisitor::visit() lights " << m_vLight.size(), 0);
	for ( ; lightIter != lightEnd; lightIter++) {
		if (!(*lightIter)->m_bOn) continue;
		// TODO: ugly optimization: if statements moved outside for loop		
		vector<Vertex3D>::iterator vtxIter = s->m_vVtxTrans.begin();
		vector<Vertex3D>::iterator vtxEnd = s->m_vVtxTrans.end();
		vector<Vertex3D>::iterator nmlTransIter = s->m_vNmlTrans.begin();
		vector<Vertex3D>::iterator nmlTransEnd = s->m_vNmlTrans.end();
		vector<Vertex3D>::iterator nmlAlignIter = s->m_vNmlAlign.begin();
		vector<Vertex3D>::iterator nmlAlignEnd = s->m_vNmlAlign.end();
		vector<Color>::iterator diffuseIter = s->m_vLight.begin();
		vector<Color>::iterator diffuseEnd = s->m_vLight.end();
		vector<Color>::iterator specularIter = s->m_vSpecular.begin();
		vector<Color>::iterator specularEnd = s->m_vSpecular.end();

		EmAssert((s->m_vVtxTrans.size() == s->m_vNmlTrans.size()) &&
						 (s->m_vVtxTrans.size() == s->m_vNmlAlign.size()) &&
						 (s->m_vVtxTrans.size() == s->m_vLight.size()) &&
						 (s->m_vVtxTrans.size() == s->m_vSpecular.size()),
						 "size missmatch");

		for ( ; vtxIter != vtxEnd; 
					vtxIter++, nmlTransIter++, nmlAlignIter++, diffuseIter++, specularIter++) {
			Vertex3D vtxLight;
		
			// Get length from vertex to light
			vtxLight.x = (*lightIter)->m_vtxTrans.x - (*vtxIter).x;
			vtxLight.y = (*lightIter)->m_vtxTrans.y - (*vtxIter).y;
			vtxLight.z = (*lightIter)->m_vtxTrans.z - (*vtxIter).z;
			float length = EMath::vectorLength(vtxLight);
			// Check bounds
			if (((*lightIter)->m_iProperties & EM_USE_BOUNDS) && length > (*lightIter)->m_fBounds) {
				continue;
			}
			// Calculate angle between normal and light
			float light;
			if ((*lightIter)->m_iProperties & EM_IGNORE_ANGLE_FULL) {
				light = 1;
			} else if ((*lightIter)->m_iProperties & EM_IGNORE_ANGLE_HALF) {
				EMath::normalizeVector(vtxLight);
				float angle = EMath::dotProduct((*nmlTransIter), vtxLight);
				light = angle; // 1.0f - EMath::emAcos(angle)/EM_PI_DIV_2; // ??
				if (light > 0.0f) light = 1.0f;
			} else {
				EMath::normalizeVector(vtxLight);
				float angle = EMath::dotProduct((*nmlTransIter), vtxLight);
				light = angle;// 1.0f - EMath::emAcos(angle)/EM_PI_DIV_2;
				light = EM_MAX(0.0f, light);
			}
			// Calculate distance
			float k;
			if ((*lightIter)->m_iProperties & EM_IGNORE_DISTANCE) {
				k = light;
			} else {
				k = light / ((*lightIter)->m_fConstant + (*lightIter)->m_fLinear * length + 
										 (*lightIter)->m_fQuadratic * length * length);
			}
			// specular light
			float specular;
			if ((s->m_iProperties & EM_SPECULAR) == EM_SPECULAR || true) {
				Vertex3D vtxRef = {0,0,-1};
				Vertex3D vtxDir;
				vtxDir.x = -vtxLight.x;
				vtxDir.y = -vtxLight.y;
				vtxDir.z = -vtxLight.z;
				EMath::reflection(vtxDir, (*nmlAlignIter), vtxRef, false);
				EMath::normalizeVector(vtxRef); // TODO: optimize - remove normalize
				//float view = EMath::dotProduct((*nmlAlignIter), vtxView); // TODO: optimize
				float spe = EM_MAX(0.0f, vtxRef.z);
				float spe2 = spe * spe;
				float spe4 = spe2 * spe2;
				specular = spe4 * spe4;
			} else {
				specular = 0;
			}

			EM_COUT("PointLightVisitor::visit() factor " << k, 0);
			EM_COUT("PointLightVisitor::visit() specular " << specular, 0);
			(*diffuseIter).r += k * (*lightIter)->m_fR;
			(*diffuseIter).g += k * (*lightIter)->m_fG;
			(*diffuseIter).b += k * (*lightIter)->m_fB;

			(*specularIter).r += specular * (*lightIter)->m_fR;
			(*specularIter).g += specular * (*lightIter)->m_fG;
			(*specularIter).b += specular * (*lightIter)->m_fB;

			(*diffuseIter).r = EM_MIN((*diffuseIter).r, 1.0);
			(*diffuseIter).g = EM_MIN((*diffuseIter).g, 1.0);
			(*diffuseIter).b = EM_MIN((*diffuseIter).b, 1.0);

			(*specularIter).r = EM_MIN((*specularIter).r, 1.0);
			(*specularIter).g = EM_MIN((*specularIter).g, 1.0);
			(*specularIter).b = EM_MIN((*specularIter).b, 1.0);
		}
	}
}

