//#ident "$Id: Obj3dsUtil.h,v 1.1 2003/05/20 15:21:18 rzr Exp $" 
#ifndef Obj3dsUtil_h_ 
#define Obj3dsUtil_h_ 
class Shape3D;
class Obj3dsUtil;
/**
 * @author: www.Philippe.COVAL.online.fr - Rev: $Author: rzr $ 
 * 3dStudio § 3ds max import / export (Assci)
 **/
class Obj3dsUtil
{ 
public:
  ///
  static Obj3dsUtil* getInstance();
  /// load 3ds objects (exported to ascii  .asc) ,  textures unspecified ?
  static int read( Shape3D& shape, 
                   char const * const filename  = "obj-3ds.asc" );
  /// save to 3ds object
  static int write(Shape3D const & shape, 
                   char const* const filename = "obj-3ds.asc" );
protected:
  /// Singleton design pattern
  static Obj3dsUtil* p_Instance;
};
#endif //_h_ inclusion 
//$Id: Obj3dsUtil.h,v 1.1 2003/05/20 15:21:18 rzr Exp $
