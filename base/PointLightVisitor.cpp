/***************************************************************************
                          PointLightVisitor.cpp  -  description
                             -------------------
    begin                : Sun Jan 30 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "PointLightVisitor.h"
#include "Light.h"
#include "Shape3D.h"
#include "Group.h"
#include "EMath.h"
#include "Polygon.h"
#include "Config.h"

PointLightVisitor * PointLightVisitor::p_PointLightVisitor = NULL;

PointLightVisitor::PointLightVisitor(int size) {
  m_vLight.reserve(size);
}

PointLightVisitor::~PointLightVisitor() {
  p_PointLightVisitor = NULL;
}

PointLightVisitor * PointLightVisitor::getInstance() {
  if (p_PointLightVisitor == NULL) {
    p_PointLightVisitor = new PointLightVisitor();
  }
  return p_PointLightVisitor;
}

void PointLightVisitor::add(Light* l) {
  if (l == NULL) return;
  m_vLight.push_back(l);
}

void PointLightVisitor::clear() {
  m_vLight.clear();
}

void PointLightVisitor::visit(Group * g) {
  EM_COUT("PointLightVisitor deprecated", 1);
  exit(1);
  // Check properties before applying light
  if (g->m_iProperties & EM_GROUP_NO_LIGHT) return;

  if (g->m_iProperties & EM_GROUP_LIGHT_ONCE) {
    g->unsetProperty(EM_GROUP_LIGHT_ONCE);
    g->setProperty(EM_GROUP_NO_LIGHT);
  } 

  vector<Shape3D*>::iterator iter = g->m_vShape3D.begin();
  vector<Shape3D*>::iterator end = g->m_vShape3D.end();
  for ( ; iter != end; iter++) {
    // if ((*iter)->m_iProperties & EM_SHAPE3D_TRANS) continue;
    if ((*iter)->m_iProperties & EM_SHAPE3D_HIDDEN) continue;
    this->visit((*iter), g);
  }
}

void PointLightVisitor::visit(Shape3D * s, Group *) {
#if 0
  if (Config::getInstance()->useLights()) {
    vector<Light*>::iterator lightIter = m_vLight.begin();
    vector<Light*>::iterator lightEnd = m_vLight.end();
    EM_COUT("PointLightVisitor::visit() lights " << m_vLight.size(), 0);
    for ( ; lightIter != lightEnd; ++lightIter) {
      if (!(*lightIter)->m_bOn) continue;
      // TODO check bounds with collision bounds so that we don't
      // need to check every vertex.
      // TODO ugly optimization: if statements moved outside for loop		
      vector<Vertex3D>::iterator vtxTransIter = s->m_vVtxTrans.begin();
      vector<Vertex3D>::iterator vtxTransEnd = s->m_vVtxTrans.end();
      vector<Vertex3D>::iterator vtxAlignIter = s->m_vVtxAlign.begin();
      //vector<Vertex3D>::iterator vtxAlignEnd = s->m_vVtxAlign.end();
      vector<Vertex3D>::iterator nmlTransIter = s->m_vNmlTrans.begin();
      //vector<Vertex3D>::iterator nmlTransEnd = s->m_vNmlTrans.end();
      vector<Vertex3D>::iterator nmlAlignIter = s->m_vNmlAlign.begin();
      //vector<Vertex3D>::iterator nmlAlignEnd = s->m_vNmlAlign.end();
      vector<Color>::iterator diffuseIter = s->m_vLight.begin();
      //vector<Color>::iterator diffuseEnd = s->m_vLight.end();
      vector<Color>::iterator specularIter = s->m_vSpecular.begin();
      //vector<Color>::iterator specularEnd = s->m_vSpecular.end();
      
      for ( ; vtxTransIter != vtxTransEnd; 
	    ++vtxTransIter, ++vtxAlignIter, ++nmlTransIter, 
	      ++nmlAlignIter, ++diffuseIter, ++specularIter) {
	Vertex3D vtxLight;
	// Get length from vertex to light
	vtxLight.x = (*lightIter)->m_vtxTrans.x - (*vtxTransIter).x;
	vtxLight.y = (*lightIter)->m_vtxTrans.y - (*vtxTransIter).y;
	vtxLight.z = (*lightIter)->m_vtxTrans.z - (*vtxTransIter).z;
	float lengthsqr = EMath::vectorLengthSqr(vtxLight);
	// Check bounds
	if (((*lightIter)->m_iProperties & EM_USE_BOUNDS) && 
	    lengthsqr > ((*lightIter)->m_fBounds*(*lightIter)->m_fBounds)) {
	  continue;
	}
	// TODO Move if statement outside for loop for performance
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
	  k = light / ((*lightIter)->m_fConstant + 
		       (*lightIter)->m_fLinear * EMath::emSqrt(lengthsqr) + 
		       (*lightIter)->m_fQuadratic * lengthsqr);
	}
	// specular light
#if EM_USE_SOURCE_SPECULAR
	if (s->m_iProperties & EM_SHAPE3D_SPECULAR) {
	  float specular;
	  Vertex3D vtxRef = {0.0f ,0.0f , -1.0f};
	  Vertex3D vtxDir;
	  vtxDir.x = (*vtxAlignIter).x - (*lightIter)->m_vtxAlign.x;
	  vtxDir.y = (*vtxAlignIter).y - (*lightIter)->m_vtxAlign.y;
	  vtxDir.z = (*vtxAlignIter).z - (*lightIter)->m_vtxAlign.z;
	  EMath::reflection(vtxDir, (*nmlAlignIter), vtxRef, false);
	  EMath::normalizeVector(vtxRef); // TODO: optimize - remove normalize
	  //float view = EMath::dotProduct((*nmlAlignIter), vtxView); // TODO: optimize
	  float spe = EM_MAX(0.0f, vtxRef.z);
	  float spe2 = spe * spe;
	  float spe4 = spe2 * spe2;
	  specular = spe4 * spe4;
	  
	  (*specularIter).r += specular * (*lightIter)->m_fR;
	  (*specularIter).g += specular * (*lightIter)->m_fG;
	  (*specularIter).b += specular * (*lightIter)->m_fB;
	} 
#endif
	
	EM_COUT_D("PointLightVisitor::visit() factor " << k, 0);
	EM_COUT_D("PointLightVisitor::visit() specular " << specular, 0);
	(*diffuseIter).r += k * (*lightIter)->m_fR;
	(*diffuseIter).g += k * (*lightIter)->m_fG;
	(*diffuseIter).b += k * (*lightIter)->m_fB;
      }
    }
  } // if config->useLights
  // apply the lights to the color
  // TODO textured polygons don't need color if filter != -1
  vector<Color>::iterator diffuseIter = s->m_vLight.begin();
  vector<Color>::iterator diffuseEnd = s->m_vLight.end();
  vector<Color>::iterator specularIter = s->m_vSpecular.begin();
  vector<Color>::iterator colorIter = s->m_vColor.begin();
  vector<Color>::iterator litColorIter = s->m_vLitColor.begin();
  // move if statement outside loop for performance
  if (s->m_iProperties & EM_SHAPE3D_SPECULAR) {
    for (; diffuseIter != diffuseEnd;
	 ++diffuseIter, ++specularIter, ++colorIter, ++litColorIter) {
      (*litColorIter).r = (*colorIter).r * (*diffuseIter).r + (*specularIter).r;
      (*litColorIter).g = (*colorIter).g * (*diffuseIter).g + (*specularIter).g;
      (*litColorIter).b = (*colorIter).b * (*diffuseIter).b + (*specularIter).b;
      (*litColorIter).a = (*colorIter).a;
    }
  } else {
    for (; diffuseIter != diffuseEnd;
	 ++diffuseIter, ++specularIter, ++colorIter, ++litColorIter) {
      (*litColorIter).r = (*colorIter).r * (*diffuseIter).r;
      (*litColorIter).g = (*colorIter).g * (*diffuseIter).g;
      (*litColorIter).b = (*colorIter).b * (*diffuseIter).b;
      (*litColorIter).a = (*colorIter).a;
    }
    /*
      (*litColorIter).r = EM_MAX((*litColorIter).r, 1.0f);
      (*litColorIter).g = EM_MAX((*litColorIter).g, 1.0f);
      (*litColorIter).b = EM_MAX((*litColorIter).b, 1.0f);
    */
  }
#endif
}

