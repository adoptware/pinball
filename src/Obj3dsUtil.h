//#ident "$Id: Obj3dsUtil.h,v 1.2 2003/06/13 13:39:48 rzr Exp $"
#ifndef Obj3dsUtil_h_
#define Obj3dsUtil_h_
class Shape3D;
class Obj3dsUtil;
class Group;
/**
 * @author: www.Philippe.COVAL.online.fr - Rev: $Author: rzr $
 * 3dStudio : 3ds max import / export (Assci)
 * Wrap to a rzr's mini-lib (Under devel)
 * load 3ds objects (exported to ascii (dos "\r\n" ) .asc)
 * Limitations are : polygons are converted to triangles,
 * neighbour triangles can't have a different color
 *
 * Currenly it works this way :
 * - To use/test add #define RZR_PATCHES_3DS in ../pinconfig.h
 * - To use a full 3ds object with several object textured
 * it should possible when separating the .asc in several
 * - replace the material line by a texture file ( file.png)
 * or a color encoded like : "r255g128b000a128"
 * - export may be called when needed
 * @see Loader
 *
 * 3DS .ASC File export example:

 Named object: "Object_0"
 Tri-mesh, Vertices: 3     Faces: 1
 Vertex list
 Vertex 0:  X:0   Y:0	Z:0
 Vertex 1:  X:0	  Y:10	Z:0  U:.4 V:.6 (<= optional)
 Vertex 2:  X:10  Y:10	Z:0
 Face list
 Face 0:  A:3 B:2 C:1 AB:1 BC:1 CA:1
 Material:"BLUE (or whatever)"
 Smoothing:  1

**/
class Obj3dsUtil
{
public:
  ///
  static Obj3dsUtil* getInstance();
  /// load 3ds objects into one Shape (exported to ascii  .asc)
  /// multi Raw Colors or ONE SINGLE texture supported
  static int read( Shape3D& shape,
                   char const * const filename  = "obj-3ds.asc" );
  /// load objects (as much pinball's shapes as 3ds' trimesh)
  /// multi textures or Colors are supported
  static int read( Group& in,
                   char const * const filename  = "obj-3ds.asc" );
  /// export to 3ds object, loosy  //!!! check
  static int write(Shape3D const & shape, char const* const filename );
  /// export to 3ds object, loosy  //!!! check
  /// @param out : must be allready open as "wb"
  static int write(Shape3D const & shape, FILE* out = stdout );
  /// export to 3ds object, loosy  //!!! check
  static int write( Group & in,  char const* const filename );
  /// const
  /// @param out : must be allready open as "wb"
  static int write( Group & in, FILE* out = stdout);
protected:
  /// Singleton design pattern
  static Obj3dsUtil* p_Instance;
};
#endif //_h_ inclusion
//EOF $Id: Obj3dsUtil.h,v 1.2 2003/06/13 13:39:48 rzr Exp $
