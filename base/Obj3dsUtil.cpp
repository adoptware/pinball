//#ident "$Id: Obj3dsUtil.cpp,v 1.1 2003/05/20 15:21:18 rzr Exp $" 
/** 
 * @author: www.Philippe.COVAL.online.fr - rev: $Author: rzr $
 **/
#ifndef Obj3dsUtil_cpp_ 
#define Obj3dsUtil_cpp_ 

//----------------------------------------------------------------
extern "C" {
  /** 
   * @author: www.Philippe.COVAL.online.fr - rev: $Author: rzr $
   * C code excrapt from 3ds library 
   * Modifying code here is not a good idead, 
   * please contact the author before, I may release something cleaner
   **/

  /*
#include "../../3dsview/src/types3d.c"
#include "../../3dsview/src/parse3ds.c"
#include "../../3dsview/src/color3ds.c"

 cat types3d.h types3d.c parse3ds.h parse3ds.c color3ds.h color3ds.c | grep -v "#include \"" > /tmp/inl-3ds.c

  */
#define debug(x)
#define assert3gl(a,b,c)

/* #ident "$Id: */
/**
 * @author: www.Philippe.COVAL.online.fr - Rev: $Author: rzr $ 
 **/

#include "stdio.h"
#ifndef types3d_h_
#define types3d_h_



typedef struct 
{
  float x, y, z;
} Vertex;

typedef struct 
{
  int x, y;
} Point;
/*
Face list
Face 0:  A:4 B:7 C:3 AB:1 BC:1 CA:1
Material:"BLUE"
Smoothing:  1
*/
typedef struct
{
  unsigned int a,b,c;
  unsigned int aa, ab, ac;
  unsigned char col;
  char smoothing;
  char *material;
} Face;

typedef struct
{
  Vertex* vv;
  Face*   vf;
  int nv;
  int nf;
} Object; //TriMesh

/*
typedef struct
{
  char* name;
} Object;

  //Light* vl;
  //TriMesh* vm;
*/

/*
Ambient light color: Red=0.2 Green=0.2 Blue=0.2
*/

typedef struct
{
  Vertex lightambiantcolor;
  Object* vo;
  int no;
} Scene;

void freeObject(Object* o);

#endif //_h_
/**
 * @author: www.Philippe.COVAL.online.fr - Rev: $Author: rzr $ 
 **/

#include <stdlib.h>


void freeObject(Object* o)
{
  int i=0;
  for (i=0;i< o->nf;i++) {
    free ( o->vf[i].material );
  }
  free( o->vv); o->vv=0;
  free( o->vf); o->vv=0;
  o->nv = 0; o->nf = 0;
  //free (o); 
  //o = 0;
}
#ifndef parse3ds_h_
#define parse3ds_h_
/**
 * @author: www.Philippe.COVAL.online.fr - Rev: $Author: rzr $ 
 **/
int errorIO(FILE* f,int t,char* msg);

char* skipafter(char* src, char* pat);

int parse3dsAscii(FILE* f, Object* o);

int read3dsAscii(char* filename,  Object* o);

int get3dsAscii(char* filename, Object* o) ;

int fprintObject(FILE*f, Object* o);

int saveObject(Object* o, char const * const filename);

int fgets_strcmp_token(FILE* f, char* buf) ;

int fgets_tag(FILE* f, char* buf, char** strarg);

#endif
/* #ident "$Id: Obj3dsUtil.cpp,v 1.1 2003/05/20 15:21:18 rzr Exp $" */
/**
 * @author: www.Philippe.COVAL.online.fr - Rev: $Author: rzr $ 
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>


#define BSIZE 256
#define BOUND 1000.


char* keyword[] = { "Tri-mesh,", 
                    "Vertex list", "Face list",
                    "Vertex", "Face", "Material" , "Smoothing"};



#define TRIMESH 0
#define VERTEXLIST 1
#define FACELIST 2
#define VERTEX 3
#define FACE 4
#define MATERIAL 5
#define SMOOTHING 6

char nkey=7; //sizeof(keyword);

int errorIO(FILE* f,int t,char* msg)
{
  fclose( f );
  ////debugf("%s", msg);
  perror("!!! Err/IO/");
  return( t );
  exit(t);
}

char* skipafter(char* src, char* pat) 
{
  char* str=0;
  ////debugf("+ skipafter [%s]@[%s]\n",pat,src);
  //assert(src);
  if ( src == 0 ) return 0;
  str=strstr(src, pat);
  if (str) str+=strlen(pat);
  //debug(str);
  return str;
}
  

int fgets_strcmp_token(FILE* f, char* buf) 
{
  int t=0;
  char* str=0;
  int i=0;
  //debug("+ fgets_strcmp_token");
  do {
    str = fgets(buf, BSIZE,f);
    if ( str == 0 ) return -1;
    //assert(str); 
    //buf [ strlen(buf) - 2 ] = 0;
    //debugf("+ [%s]\n",buf);
    for (i=0; (i<nkey) || (str !=0 ) ;i++) {
      //debugf("? [%s]\n",keyword[i]);
      str = strstr( buf, keyword[i] );
      if (str ) return i;
    }
  } while( str == 0 );
  //assert(str);
  ////debug("- fgets_strcmp_token");
  return -2;
}

int fgets_tag(FILE* f, char* buf, char** strarg) 
{
  int t=0;
  //debug("+ fgets_tag");
  char * str;
  t = fgets_strcmp_token(f, buf);
  //debug(buf);
  //debug(keyword[t]);
  if (t >=0 ) str = skipafter( buf, keyword[t] );
  else { str = 0; }
  //debug(str);
  *strarg = str;
  //debug("- fgets_tag");
  return t;
}

int saveObject(Object* o, char const * const filename)
{
  int t=0;
  FILE* f=0;
  //debugf("+saveObject %s\n",filename);
  f=fopen( filename, "wt");
  if ( ! f ) return (int) f;
  t = fprintObject( f, o);
  fclose(f);
  //debug("-saveObject");
  return t;
  
  
}

int fprintObject(FILE*f, Object* o)
{
  int i=0;

  fprintf(f,"Tri-mesh, Vertices: %d Faces: %d\n",o->nv, o->nf);
  fprintf(f,"Vertex list\n");
  
  for(i=0;i<o->nv;i++) 
    fprintf( f,"Vertex %d: X:%f Y:%f Z:%f \n",i,
	     o->vv[i].x,  o->vv[i].y, o->vv[i].z);

  fprintf(f,"Face list\n");
  for(i=0;i<o->nf;i++) {
    fprintf( f,"Face %d: A:%d B:%d C:%d",i,
	     o->vf[i].a,  o->vf[i].b, o->vf[i].c);
    fprintf(f," AB:1 BC:1 CA:1 \n"); //!!!
    if( o->vf[i].material)
      fprintf(f, "Material: \"%s\"\n",o->vf[i].material);
    fprintf(f, "Smoothing:1\n");
    //fprintf( f, "Smoothing: \"%d\"\n",o->vf[i].smoothing);
  }
  return 1;
}



int parse3dsAscii(FILE* f, Object* o) 
{
  char buf[BSIZE];
  int a,b,c=0;
  float x,y,z=0;
  int i=0;
  int t=0;
  char * str = (char*) malloc (sizeof(char));;
  char * pat = 0;
  int nv = 0;
  int nf =0;
  //debug("+parse3dsAscii");  
  buf[0]= 0;

  
  //debug("skip to 1st object");
  t=1;

  t = fgets_tag(f,buf,&str);  
  while ( t >=0 ) {

    //debugf("[%d]\n",t);
    
    //debug(buf);
    //debug(str);
    switch(t) {
    case TRIMESH:
      //debug("read trimesh");
      sscanf( str, "%s %d %s %d", buf, &nv, buf, &nf );
      //debugf( "Vertices=%d; Faces=%d;\n",  nv , nf );
      o->vv  = (Vertex *) calloc( nv ,sizeof(Vertex) );
      o->vf = (Face *) calloc( nf, sizeof(Face) );
      o->nv = nv; 
      o->nf = nf;
      ////debugf("read vertexes (%d)\n", o->nv);
      t = fgets_tag(f,buf,&str);
    case VERTEXLIST:
      //debugf("read vertext list %d\n",nv);
      //debug(str);
      for( i=0; i<nv; i++ ) { //Vertex 0:  X:-1.0	Y:1.0	Z:1.0
        t = fgets_tag(f,buf,&str);
        assert ( t == VERTEX);
        str = skipafter( str, "X:"); assert(str); 
        sscanf(str,"%f",&x);
        str = skipafter( str, "Y:"); assert(str); 
        sscanf(str,"%f",&y);
        str = skipafter( str, "Z:"); assert(str); 
        sscanf(str,"%f",&z);
        //debugf("vertex[%d]={ %f, %f, %f };\n",i,x,y,z);
        //assert3gl( x , -BOUND , +BOUND );
        //assert3gl( y , -BOUND , +BOUND );
        //assert3gl( z , -BOUND , +BOUND );
        o->vv[i].x = x; o->vv[i].y = y; o->vv[i].z = z;
        //exit(0);
      }
      t = fgets_tag(f,buf,&str);  
    case FACELIST:
      //debugf("read faces %d\n",nf);
      t = fgets_tag(f,buf,&str);
      for( i=0; i<nf; i++ ) { //Vertex 0:  X:-1.0	Y:1.0	Z:1.0
        //str= skipafter( str, ":");
        ////debug(str);
        //assert ( t == FACE);
        //assert(str);
        str = skipafter( str, "A:"); assert(str); 
        sscanf(str,"%d",&a);
        str = skipafter( str, "B:"); assert(str); 
        sscanf(str,"%d",&b);
        str = skipafter( str, "C:"); assert(str); 
        sscanf(str,"%d",&c);
        //debugf("face[%d]={ %d, %d, %d }; \n",i,a,b,c);
        o->vf[i].a = a; o->vf[i].b = b; o->vf[i].c = c;
        assert3gl( a , -1 , o->nf);
        assert3gl( b , -1 , o->nf);
        assert3gl( c , -1 , o->nf);
        //

        t = fgets_tag(f,buf,&str);
        //debug(str);
        
        switch ( t ) {
        case MATERIAL :
          str= skipafter( str, ":");
          str= skipafter( str, "\"");
          ////debug("mat:");
          ////debug(str);
          sscanf(str,"%s", buf);
          t = strlen(buf);
          if ( str[t-1] == '\"') t--;
          ////debug(buf);
          o->vf[i].material = (char*) malloc( t+1);
          strncpy( o->vf[i].material , buf , t);
          o->vf[i].material[t]=0;
          //debugf("Material[%d]=\"%s\"",i, o->vf[i].material[t] );

          t = fgets_tag(f,buf,&str);
        case SMOOTHING:
          ////debug(str);
          ////debug(pat);
          str= skipafter( str, ":");
          sscanf(str,"%d", &t);
          o->vf[i].smoothing = t;
          t = fgets_strcmp_token(f,buf); str = buf;
        }
      }
    }
  }
  ////debugf("read v=(%d) f=(%d)\n", o->nv, o->nf);
  ////debug("-parse3dsAscii");  
  //printObject(o);
  //fprintObject(stdout, o);
  return 0;
}

int read3dsAscii(char* filename, Object* o)
{ 
  FILE *f = 0;
  int t=0;
  //debugf("+read parse3ds %s\n",filename );
  if( !(f=fopen(filename, "ra")) ) {
    perror("Err/IO/ Need a 3DS ascii file \n");
    return(-1);
  }
  t = parse3dsAscii( f, o);
  fclose( f );
  //debugf("+read parse3ds %d\n",t);
  return t;
}
//#ident "$Id: Obj3dsUtil.cpp,v 1.1 2003/05/20 15:21:18 rzr Exp $" 
#ifndef color3ds_h_ 
#define color3ds_h_ 
/**
 * @author: www.Philippe.COVAL.online.fr - Rev: $Author: rzr $ 
 **/
///
unsigned int getColor(char const * const p);
///
unsigned int getColorRGB(char* r, char*g, char*b , char const * const p) ;
///
unsigned int getColorRGBFloat(float* r, float*g, float*b ,
                              char const * const p) ;

#endif //_h_ inclusion 
//$Id: Obj3dsUtil.cpp,v 1.1 2003/05/20 15:21:18 rzr Exp $
//#ident "$Id: Obj3dsUtil.cpp,v 1.1 2003/05/20 15:21:18 rzr Exp $" 
#ifndef color3ds_c_ 
#define color3ds_c_ 

/**
 * @author: www.Philippe.COVAL.online.fr - Rev: $Author: rzr $ 
 **/

unsigned int getColorRGBFloat(float* r, float*g, float*b , char const * const p) 
{
  unsigned int c=0; 
  //debug("+getColorRGBFloat");
  c = getColor(p);
  *b = (float)  ((c)     & 0x000000FF);
  *g = (float)  ((c>>8)  & 0x000000FF);
  *r = (float)  ((c>>16) & 0x000000FF);

  //debugf("%x={ %f,%f,%f};\n",c,*r,*g,*b);
  *r /= 255. ;
  *g /= 255. ;
  *b /= 255. ;
  //debugf("={ %f,%f,%f};\n",*r,*g,*b);
  //exit(0);
  return c;
}
unsigned int getColorRGB(char* r, char*g, char*b , char const * const p) 
{ 
  unsigned int c,ur,ug,ub;
  c = getColor(p);
  ub = (c)     & 0x000000FF;
  ug = (c>>8)  & 0x000000FF;
  ur = (c>>16) & 0x000000FF;
  //debugf("%x={ %x,%x,%x }\n",c,ur,ug,ub);
  //*g = (char) (&ug+1);
  //sprintf(g,"%u",(ug<<24));
  *b = (char) ub;
  *r = (char) ur;
  *g = *g & 0x000000FF;
  //debugf("%x={ %x,%u,%d }\n",ug,*g,*g,*g);
  exit(-1);
  return c;
}
unsigned int getColor(char const * const p)
{
  //debugf("+getColor \"%s\"\n",p);
  if ( p == 0 ) return 0xFFFFFF;
  if (! strcmp( p, "BLACK") )  return 0x000000;
  if (! strcmp( p, "WHITE") )  return 0xFFFFFF;
  if (! strcmp( p, "BLUE") )   return 0x0000FF;
  if (! strcmp( p, "GREEN") )  return 0x00FF00;
  if (! strcmp( p, "RED")  )   return 0xFF0000;  

  if (! strcmp( p, "CYAN") )   return 0x00FFFF;
  if (! strcmp( p, "YELLOW") ) return 0xFFFF00;
  if (! strcmp( p, "MAGENTA") )return 0xFF00FF;
  if (! strcmp( p, "GRAY") )   return 0x909090;
  // GREENBUG, COW,  etc
  if (! strcmp( p, "X")   )    return 0xFFFF00; 
  //debugf("!!! Err/ Not Found %s",p);
  return 0xFFFFFF;
}
#endif //_h_ 
//$Id: Obj3dsUtil.cpp,v 1.1 2003/05/20 15:21:18 rzr Exp $

} // C lang
//-----------------------------------------------------------------------------
#include "Private.h"
#include "Obj3dsUtil.h"
#include "Shape3D.h"
#include "Polygon.h"

Obj3dsUtil* Obj3dsUtil::p_Instance = 0;

Obj3dsUtil* Obj3dsUtil::getInstance()
{
  if (!  p_Instance ) p_Instance = new Obj3dsUtil;
  return p_Instance;
}

/*


Ambient light color: Red=0.2 Green=0.2 Blue=0.2

Named object: "Object_0"
Tri-mesh, Vertices: 3     Faces: 1
Vertex list
Vertex 0:  X:0 		Y:0	Z:0
Vertex 1:  X:0		Y:10	Z:0
Vertex 2:  X:10		Y:10	Z:0
Face list
Face 0:  A:3 B:2 C:1 AB:1 BC:1 CA:1
Material:"BLUE"
Smoothing:  1


*/

int Obj3dsUtil::read( Shape3D& shape,  char const * const filename )
{
  debug("+loadShape3dsAscii");
  char name[BSIZE];
  int i=0;
  strcpy(name, filename);

  Object o;
  //debug("some transformations for testing");
  //get3dsAscii(name, &o);
  int t = read3dsAscii(name,(Object*) &o);
  assert( t == 0);
  if ( t != 0 ) return t;
  //debug("Convert structures");
  for(i=0;i<o.nv;i++) {
    shape.add( o.vv[i].x ,o.vv[i].y, o.vv[i].z );
  }
  Polygon3D* poly = 0;
  float r=1,g=1,b=1;
  int j=0;
  for(i=0; i<o.nf;i++) {
    poly = new Polygon3D(&shape);
    poly->add( o.vf[i].a);
    poly->add( o.vf[i].c);
    poly->add( o.vf[i].b);
    shape.add( poly );
    //debug("replace vertex color r g b a u v");
#if 1
    j =  o.vf[i].a;
    getColorRGBFloat( &r, &g, &b, o.vf[i].material );
    shape.setColor( j, r,g,b,1);
    j =  o.vf[i].b;
    getColorRGBFloat( &r, &g, &b, o.vf[i].material );
    shape.setColor( j, r,g,b,1);
    j =  o.vf[i].c;
    getColorRGBFloat( &r, &g, &b, o.vf[i].material );
    shape.setColor( j, r,g,b,1);
#endif
  }
  freeObject( &o);
  shape.countNormals();
  //debug("-loadShape3dsAscii");
  return 0;
}
int Obj3dsUtil::write( Shape3D const & shape,  char const * const filename )
{
  exit(-1);
  Object o;
  
  return 0;
}

#endif // inclusion 
//EOF:$Id: Obj3dsUtil.cpp,v 1.1 2003/05/20 15:21:18 rzr Exp $
