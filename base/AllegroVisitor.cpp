/***************************************************************************
                          AllegroVisitor.cpp  -  description
                             -------------------
    begin                : Sat Jan 6 2001
    copyright            : (C) 2001 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/

#include "Private.h"
#include "AllegroVisitor.h"
#include "Group.h"
#include "Shape3D.h"
#include "Polygon.h"
#include "BillBoard.h"
#include "Config.h"

AllegroVisitor * AllegroVisitor::p_AllegroVisitor = NULL;

AllegroVisitor::AllegroVisitor(){
#if EM_USE_ALLEGRO
	//
	for (int a=0; a<256; a++) {
		alleg_vtx_p[a] = &alleg_vtx[a];
		alleg_clip_p[a] = &alleg_clip[a];
		alleg_tmp_p[a] = &alleg_tmp[a];
	}
	m_iMode = EM_ALLEGRO_GCOL_TEX;
#endif // EM_USE_ALLEGRO
}

AllegroVisitor::~AllegroVisitor() {
	p_AllegroVisitor = NULL;
}

AllegroVisitor * AllegroVisitor::getInstance() {
	if (p_AllegroVisitor == NULL) {
		p_AllegroVisitor = new AllegroVisitor();
	}
	return p_AllegroVisitor;
}

void AllegroVisitor::empty() {
#if EM_USE_ALLEGRO
	switch (m_iMode) {
	case EM_ALLEGRO_GCOL_TEX: {
		drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
	} break;
	case EM_ALLEGRO_GCOL_TEX_TRANS: {
		drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
	} break;
	case EM_ALLEGRO_CLEAN: {
#if EM_DEBUG
		em_poly_m = em_poly_m*0.7 + em_poly*0.3;
		em_poly = 0;
#endif
	} break;
	}
#endif // EM_USE_ALLEGRO
}

#if EM_USE_ALLEGRO
void AllegroVisitor::visit(Group * g) {
	int filter = Config::getInstance()->getGLFilter();

	switch (m_iMode) {
	case EM_ALLEGRO_GCOL_TEX: {
		// shapes
		vector<Shape3D*>::iterator shapeIter = g->m_vShape3D.begin();
		vector<Shape3D*>::iterator shapeEnd = g->m_vShape3D.end();
		for ( ; shapeIter != shapeEnd; ++shapeIter) {
			if (EM_SHAPE3D_HIDDEN & (*shapeIter)->m_iProperties) continue;

			vector<Polygon*>::iterator polyIter = (*shapeIter)->m_vPolygon.begin();
			vector<Polygon*>::iterator polyEnd = (*shapeIter)->m_vPolygon.end();
			for ( ; polyIter != polyEnd; polyIter++) {
				// if ((*polyIter)->m_iProperties & EM_POLY_HIDDEN) continue;
				if ((*polyIter)->m_iProperties & EM_POLY_TRANS) continue;
				vector<int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
				vector<int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
				
				// 3.4 %
				int index=0;
				// TODO implement a fast float to int conversion and fast clamp;
				// see section 2.1 i Game Prog Gems 2
				if ((*shapeIter)->m_Texture == NULL || filter == -1) {
					// color polygon
					for (; indexIter != indexEnd; ++indexIter, ++index) {
#if 0
						alleg_vtx[index].x = (*shapeIter)->m_vVtxAlign[(*indexIter)].x;
						alleg_vtx[index].y = -(*shapeIter)->m_vVtxAlign[(*indexIter)].y;
						alleg_vtx[index].z = -(*shapeIter)->m_vVtxAlign[(*indexIter)].z;
						alleg_vtx[index].u = 0;
						alleg_vtx[index].v = 0;
						alleg_vtx[index].c = 0xFFFFFF;
#else
						// TODO Fix this slow copying !!!
						alleg_vtx[index].x = (*shapeIter)->m_vVtxAlign[(*indexIter)].x;
						alleg_vtx[index].y = -(*shapeIter)->m_vVtxAlign[(*indexIter)].y;
						alleg_vtx[index].z = -(*shapeIter)->m_vVtxAlign[(*indexIter)].z;
						float r = (*shapeIter)->m_vLitColor[(*indexIter)].r*255.0f;
						float g = (*shapeIter)->m_vLitColor[(*indexIter)].g*255.0f;
						float b = (*shapeIter)->m_vLitColor[(*indexIter)].b*255.0f;
						int ir = (int)r;
						int ig = (int)g;
						int ib = (int)b;
						alleg_vtx[index].c = 	makecol(EM_MIN(ir,255), EM_MIN(ig,255), EM_MIN(ib,255));
#endif
					}
				} else { 
					// textured polygon
					for (; indexIter != indexEnd; ++indexIter, ++index) {
						// TODO Fix this slow copying !!!
						alleg_vtx[index].x = (*shapeIter)->m_vVtxAlign[(*indexIter)].x;
						alleg_vtx[index].y = -(*shapeIter)->m_vVtxAlign[(*indexIter)].y;
						alleg_vtx[index].z = -(*shapeIter)->m_vVtxAlign[(*indexIter)].z;
						// TODO Not all textures are 256
						alleg_vtx[index].u = (int)((*shapeIter)->m_vTexCoord[(*indexIter)].u*255.0f);
						alleg_vtx[index].v = (int)((*shapeIter)->m_vTexCoord[(*indexIter)].v*255.0f);
						float r = (*shapeIter)->m_vLitColor[(*indexIter)].r*255.0f;
						float g = (*shapeIter)->m_vLitColor[(*indexIter)].g*255.0f;
						float b = (*shapeIter)->m_vLitColor[(*indexIter)].b*255.0f;
						int ir = (int)r;
						int ig = (int)g;
						int ib = (int)b;
						ib = EM_MAX(ir, EM_MAX(ig, ib));
						alleg_vtx[index].c = 	EM_MIN(ib, 255);
					}
				}
				// 52.9 %
				if (index < 3) continue;
				EmAssert(index < 128, "AllegorVisitor::visit polygonedge amout to large");
				// clip
				int size = 0;
				if ((*shapeIter)->m_Texture == NULL || filter == -1) {
					size = clip3d_f(POLYTYPE_GCOL, 0.1, 1000, index, 
													(AL_CONST V3D_f**)alleg_vtx_p,alleg_clip_p,	alleg_tmp_p, int_tmp);
				} else {
					size = clip3d_f(POLYTYPE_ATEX, 0.1, 1000, index, (AL_CONST V3D_f**)alleg_vtx_p, 
													alleg_clip_p, alleg_tmp_p, int_tmp);
				}
				if (size < 3) continue;
				// project, if clause moved outside for loop for perfomance
				// most likely scenario first
				if (!((*shapeIter)->m_iProperties & (EM_SHAPE3D_BEHIND + EM_SHAPE3D_BEHIND2))) {
					for (int a=0; a<size; ++a) {
						persp_project_f(alleg_clip[a].x, alleg_clip[a].y, alleg_clip[a].z, 
														&alleg_clip[a].x, &alleg_clip[a].y);
					}
				} else if ((*shapeIter)->m_iProperties & EM_SHAPE3D_BEHIND) {
					for (int a=0; a<size; ++a) {
						persp_project_f(alleg_clip[a].x, alleg_clip[a].y, alleg_clip[a].z,
														&alleg_clip[a].x, &alleg_clip[a].y);
						alleg_clip[a].z += 0.1f;
					}
				} else if ((*shapeIter)->m_iProperties & EM_SHAPE3D_BEHIND2) {
					for (int a=0; a<size; ++a) {
						persp_project_f(alleg_clip[a].x, alleg_clip[a].y, alleg_clip[a].z, 
														&alleg_clip[a].x, &alleg_clip[a].y);
						alleg_clip[a].z += 0.2f;
					}
				} 
				// cull
				if (polygon_z_normal_f(alleg_clip_p[0], alleg_clip_p[1], alleg_clip_p[2]) < 0) continue;
				//53.5 %
				// render
				if ((*shapeIter)->m_Texture == NULL || filter == -1) {
					polygon3d_f(backbuffer, POLYTYPE_GCOL | POLYTYPE_ZBUF, NULL, size, alleg_clip_p);
				} else {
					polygon3d_f(backbuffer, POLYTYPE_ATEX_LIT | POLYTYPE_ZBUF, (*shapeIter)->m_Texture, 
											size, alleg_clip_p);
				}
				// 72 % (79%)
#if EM_DEBUG
				++em_poly;
#endif
			}
		}
		// billboard TODO
		BillBoard * b = g->p_BillBoard;
		if (b != NULL && b->m_iProperties & EM_BILLBOARD_TRANS) {
			EmAssert(b->m_Texture != NULL, "all billboards must have a texture");

			
			EM_COUT_D("AllegroVisitor::visit() BillBoard at " << b->m_vtxAlign.x <<" "<<
								b->m_vtxAlign.y <<" "<< b->m_vtxAlign.z, 0);
		}
	} break;
	case EM_ALLEGRO_GCOL_TEX_TRANS: {
		// shapes
		vector<Shape3D*>::iterator shapeIter = g->m_vShape3D.begin();
		vector<Shape3D*>::iterator shapeEnd = g->m_vShape3D.end();
		for ( ; shapeIter != shapeEnd; ++shapeIter) {
			if (EM_SHAPE3D_HIDDEN & (*shapeIter)->m_iProperties) continue;
			if (!(EM_SHAPE3D_USE_TRANS & (*shapeIter)->m_iProperties)) continue;

			vector<Polygon*>::iterator polyIter = (*shapeIter)->m_vPolygon.begin();
			vector<Polygon*>::iterator polyEnd = (*shapeIter)->m_vPolygon.end();
			for ( ; polyIter != polyEnd; ++polyIter) {
				// if ((*polyIter)->m_iProperties & EM_POLY_HIDDEN) continue;
				if (!((*polyIter)->m_iProperties & EM_POLY_TRANS)) continue;
				vector<int>::iterator indexIter = (*polyIter)->m_vIndex.begin();
				vector<int>::iterator indexEnd = (*polyIter)->m_vIndex.end();
				
				int index=0;
				if ((*shapeIter)->m_Texture == NULL || filter == -1) {
					// color polygon
					for (; indexIter != indexEnd; ++indexIter, ++index) {
						// TODO Fix this slow copying !!!
						alleg_vtx[index].x = (*shapeIter)->m_vVtxAlign[(*indexIter)].x;
						alleg_vtx[index].y = -(*shapeIter)->m_vVtxAlign[(*indexIter)].y;
						alleg_vtx[index].z = -(*shapeIter)->m_vVtxAlign[(*indexIter)].z;
						// TODO implement a fast float to int conversion and fast clamp;
						// see section 2.1 i Game Prog Gems 2
						float r = (*shapeIter)->m_vLitColor[(*indexIter)].r*255.0f;
						float g = (*shapeIter)->m_vLitColor[(*indexIter)].g*255.0f;
						float b = (*shapeIter)->m_vLitColor[(*indexIter)].b*255.0f;
						int ir = (int)r;
						int ig = (int)g;
						int ib = (int)b;
						alleg_vtx[index].c = 	makecol(EM_MIN(ir,255), EM_MIN(ig,255), EM_MIN(ib,255));
					}
				} else {
					// textured polygon
					for (; indexIter != indexEnd; ++indexIter, ++index) {
						// TODO Fix this slow copying !!!
						alleg_vtx[index].x = (*shapeIter)->m_vVtxAlign[(*indexIter)].x;
						alleg_vtx[index].y = -(*shapeIter)->m_vVtxAlign[(*indexIter)].y;
						alleg_vtx[index].z = -(*shapeIter)->m_vVtxAlign[(*indexIter)].z;
						// TODO Not all textures are 255
						alleg_vtx[index].u = (int)((*shapeIter)->m_vTexCoord[(*indexIter)].u*255.0f);
						alleg_vtx[index].v = (int)((*shapeIter)->m_vTexCoord[(*indexIter)].v*255.0f);
						// TODO implement a fast float to int conversion and fast clamp;
						// see section 2.1 i Game Prog Gems 2
						float r = (*shapeIter)->m_vLitColor[(*indexIter)].r*255.0f;
						float g = (*shapeIter)->m_vLitColor[(*indexIter)].g*255.0f;
						float b = (*shapeIter)->m_vLitColor[(*indexIter)].b*255.0f;
						int ir = (int)r;
						int ig = (int)g;
						int ib = (int)b;
						ib = EM_MAX(ir, EM_MAX(ig, ib));
						alleg_vtx[index].c = EM_MIN(ib,255) ;
					}
				}
				if (index < 3) continue;
				EmAssert(index < 128, "AllegorVisitor::visit polygonedge amout to large");
				// clip
				int size = 0;
				if ((*shapeIter)->m_Texture == NULL || filter == -1) {
					size = clip3d_f(POLYTYPE_GCOL, 0.1, 1000, index, 
													(AL_CONST V3D_f**)alleg_vtx_p,alleg_clip_p, alleg_tmp_p, int_tmp);
				} else {
					size = clip3d_f(POLYTYPE_ATEX, 0.1, 1000, index, (AL_CONST V3D_f**)alleg_vtx_p, 
													alleg_clip_p, alleg_tmp_p, int_tmp);
				}
				if (size < 3) continue;
				// project, if clause moved outside for loop for perfomance
				if ((*shapeIter)->m_iProperties & EM_SHAPE3D_BEHIND) {
					for (int a=0; a<size; ++a) {
						persp_project_f(alleg_clip[a].x, alleg_clip[a].y, alleg_clip[a].z,
														&alleg_clip[a].x, &alleg_clip[a].y);
						alleg_clip[a].z += 0.1f;
					}
				} else if ((*shapeIter)->m_iProperties & EM_SHAPE3D_BEHIND2) {
					for (int a=0; a<size; ++a) {
						persp_project_f(alleg_clip[a].x, alleg_clip[a].y, alleg_clip[a].z, 
														&alleg_clip[a].x, &alleg_clip[a].y);
						alleg_clip[a].z += 0.2f;
					}
				} else {
					for (int a=0; a<size; ++a) {
						persp_project_f(alleg_clip[a].x, alleg_clip[a].y, alleg_clip[a].z, 
														&alleg_clip[a].x, &alleg_clip[a].y);
					}
				}
				// cull
				if (polygon_z_normal_f(alleg_clip_p[0], alleg_clip_p[1], alleg_clip_p[2]) < 0) continue;
				// render
				if ((*shapeIter)->m_Texture == NULL || filter == -1) {
					polygon3d_f(backbuffer, POLYTYPE_GCOL | POLYTYPE_ZBUF, NULL, size, alleg_clip_p);
				} else {
					polygon3d_f(backbuffer, POLYTYPE_ATEX_MASK_LIT | POLYTYPE_ZBUF, 
											(*shapeIter)->m_Texture, size, alleg_clip_p);
				}
#if EM_DEBUG
				++em_poly;
#endif
			}
		}
		// billboard TODO
		BillBoard * b = g->p_BillBoard;
		if (b != NULL && b->m_iProperties & EM_BILLBOARD_TRANS) {
			EmAssert(b->m_Texture != NULL, "all billboards must have a texture");

			
			EM_COUT_D("AllegroVisitor::visit() BillBoard at " << b->m_vtxAlign.x <<" "<<
								b->m_vtxAlign.y <<" "<< b->m_vtxAlign.z, 0);
		}
	} break;
	}
}
#else
void AllegroVisitor::visit(Group *) {
}
#endif // EM_USE_ALLEGRO
