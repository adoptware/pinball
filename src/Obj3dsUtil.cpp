//#ident "$Id: Obj3dsUtil.cpp,v 1.2 2003/06/01 22:37:44 rzr Exp $" 
/** 
 * @author: www.Philippe.COVAL.online.fr - rev: $Author: rzr $
 * 3ds loader wrapper
 **/
/*


*/

#ifndef Obj3dsUtil_cpp_ 
#define Obj3dsUtil_cpp_ 
#include "Private.h"

#ifdef RZR_PATCHES_3DS //unstable
#include <math.h>
//----------------------------------------------------------------
extern "C" {
  /** 
   * @author: www.Philippe.COVAL.online.fr - rev: $Author: rzr $
   * C code excrapt from 3ds mini library  (GPL)
   * Modifying code here is not a good idea, 
   * please contact the author before, I may release something cleaner
   **/
  //#define RZR_LOCAL
#if 0 //def  RZR_LOCAL
#include "../../3dsview/src/include.h"
#include "../../3dsview/src/types3d.c"
#include "../../3dsview/src/parse3ds.c"
#include "../../3dsview/src/color3ds.c"
#else
  /*
    cd ../3dsview/src && \
    cat include.h types3d.h types3d.c \
    parse3ds.h parse3ds.c color3ds.h color3ds.c\
    | grep -v "#include \"" >| /tmp/inl-3ds.c
  */
  //#include "/tmp/inl-3ds.c" //C-x i
/* #ident "$Id: Obj3dsUtil.cpp,v 1.2 2003/06/01 22:37:44 rzr Exp $" */
/**
 * @author: www.Philippe.COVAL.online.fr - Rev: $Author: rzr $ 
 * Copyright: GPL @ http://rzr.online.fr/licence.htm
 **/

#ifndef include_h_
#define include_h_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>

#undef assert
#include <assert.h>


#define debugf  printf
#define debug(x) { debugf("%s\n",(x) ); }

#define assert1(x) { if ( x ) debugf("(%d)\n",y); assert( x);}
#define assert2eq(x,y) { if ( x != y) debugf("(%d<%d )\n",x,y); assert( x==y);}
#define assert2ne(x,y) { if ( x == y) debugf("(%d<%d )\n",x,y); assert( x!=y);}
#define assert2lt(x,y) { if ( x >= y) debugf("(%d<%d )\n",x,y); assert( x<y); }
#define assert2le(x,y) { if ( x > y) debugf("(%d<%d )\n",x,y); assert( x<=y); }
#define assert2gt(x,y) assert2lt(y,x)
#define assert2ge(x,y) assert2le(y,x)
#define assert3gl(x,y,z) { assert2ge(x,y); assert2le(y,z) };
#define assertpos(x) { assert2gt(x,0) }
#define assertneg(x) { assert2gt(0,x) }
#define assertz(x) { assert2eq(0,x) }

//#define abs(x) ( ( (x)>0) ? (x) : (-x) )// @stdlib
#define min2(x,y) ( ((y)>(x)) ? (x) : (y) )
#define max2(x,y) ( ((y)<(x)) ? (x) : (y) )

#define min3(x,y,z) min2( min2(x,z) , min2(y,z) )
#define max3(x,y,z) max2( max2(x,z) , max2(y,z) )

#define min6(a,b,c,d,e,f) min2( min3(a,b,c) , min3(d,e,f) )
#define max6(a,b,c,d,e,f) max2( max3(a,b,c) , max3(d,e,f) )

#define SQRT2 1.4142135f 
#ifndef Uint32
#define Uint32 unsigned int
#endif
#endif //_h_
/* #ident "$Id: */
/**
 * @author: www.Philippe.COVAL.online.fr - Rev: $Author: rzr $ 
 * Copyright: GPL @ http://rzr.online.fr/licence.htm
 **/

#ifndef types3d_h_
#define types3d_h_

#ifdef __cplusplus
extern "C" {
#endif



typedef struct
{
  float x, y, z;
  float u,v;
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
  unsigned int nv;
  unsigned int nf;
  char * name;
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
  unsigned int no;
} Scene;

void initObject(Object* a);

void freeObject(Object* o);

void freeScene(Scene* o);

void initScene(Scene* s);

void statsScene(Scene const * const s);

void reset(Vertex* v);

void printV( Vertex const v);

#ifdef __cplusplus
}
#endif

#endif //_h_
/**
 * @author: www.Philippe.COVAL.online.fr - Rev: $Author: rzr $ 
 * Copyright: GPL @ http://rzr.online.fr/licence.htm
 **/
#ifndef types3d_c_
#define types3d_c_

#include <stdlib.h>


void freeObject(Object* o)
{
  unsigned int i=0;
  for (i=0;i< o->nf;i++) {
    free ( o->vf[i].material );
  }
  free( o->vv); o->vv=0;
  free( o->vf); o->vv=0;
  initObject(o);
}

void initObject(Object* a)
{
  a->nv = 0; 
  a->nf = 0;  
  a->name = 0;
}

void initScene(Scene* s)
{
  //debug("+initScene");
  s->no = 0;
  s->vo = 0;
  //debug("-initScene");
}

void statsScene(Scene const * const s)
{
  debugf("%d",s->no);
  //debug("- statsScene");
}

void freeScene(Scene* s)
{
  unsigned int i=0;
  for(i=0;i< s->no; i++)
    freeObject( &(s->vo)[i]);
  free( s->vo );
  initScene(s);
}

void reset(Vertex* v)
{
  v->x=0; v->y=0; v->z=0;
}
void printV( Vertex const v)
{
  printf("{ { %f, %f, %f} , { %f, %f} }; \n",v.x,v.y,v.z,v.u,v.v);
}
void printPoint(Point v)
{
  printf("{ %d, %d }; \n",v.x,v.y);
}
#endif // inclusion
/* #ident "$Id: Obj3dsUtil.cpp,v 1.2 2003/06/01 22:37:44 rzr Exp $" */
/**
 * @author: www.Philippe.COVAL.online.fr - Rev: $Author: rzr $ 
 * Copyright: GPL @ http://rzr.online.fr/licence.htm
 **/
#ifndef parse3ds_h_
#define parse3ds_h_
#include <stdio.h>

int errorIO(FILE* f,int const t,char const * const msg);

char* skipafter(char* src, char* pat);

int parse3dsAscii(FILE* f, Object* o);

int load3dsAscii(char* filename,  Object* o);

int readScene3dsAscii(FILE* f, Scene* s) ;

int loadScene3dsAscii(char* filename, Scene* s) ;

int get3dsAscii(char* filename, Object* o) ;

int fprintObject(FILE*f, Object const * const o);

int saveObject(Object const * const in, char const * const filename);

int saveScene(Scene const *  const in, char const * const filename);

//int saveObject(Object* o, FILE* out = stdout);

int fgets_strcmp_token(FILE* f, char* buf) ;

int fgets_tag(FILE* f, char* buf, char** strarg);

#endif
/* #ident "$Id: Obj3dsUtil.cpp,v 1.2 2003/06/01 22:37:44 rzr Exp $" */
/**
 * @author: www.Philippe.COVAL.online.fr - Rev: $Author: rzr $ 
 * Copyright: GPL @ http://rzr.online.fr/licence.htm
 **/
#ifndef parse3ds_c_
#define parse3ds_c_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <assert.h>


#define BSIZE 256
#define BOUND 1000.


char* keyword[] = { "Tri-mesh,", "Vertex list", "Face list",
                    "Vertex", "Face", "Material" , "Smoothing"};



#define TRIMESH 0
#define VERTEXLIST 1
#define FACELIST 2
#define VERTEX 3
#define FACE 4
#define MATERIAL 5
#define SMOOTHING 6

int nkey= 7; //sizeof(keyword); 


char* sscanStrQuoted(char* src, char** dest)
{
  char* e;
  int n=0;
  //debugf("+ sscanStrQuotted (%s)\n",src);
  assert( src );
  assert( dest);
  e = strchr( src, '"');
  if ( e == 0) { *dest=0; return e; }
  src = e;
  //debugf("find 2d quote (%s)\n",e);
  
  do {
    e = strchr( e+1, '"');
  } while ( e && (*(e-1) == '\\') );
  if ( e ==0 ) { *dest=0; return e; }
  n = e - src;
  //debugf("alloc %d \n",n);  
  *dest = (char*)  malloc( n );
  strncpy( *dest , src + 1 , n );  
  *((*dest)+(n-1)) = 0;
  //debugf("- sscanStrQuotted \"%s\"\n",*dest);
  return e+1;
}


int loadScene3dsAscii(char* filename, Scene* s) 
{
  FILE *f = 0;
  int t=0;
  //debugf("+loadScene3dsAscii \"%s\"\n",filename);
  initScene(s);
  
  if( !(f=fopen(filename, "ra")) ) {
    return errorIO(f,-1,"Err/IO/ Need a 3DS ascii file \n");
  }
  t = readScene3dsAscii( f, s);
  assert(t>0);
  fclose( f );
  //debugf("-loadScene3dsAscii (%d)\n",t);
  return t;
  
}

int readScene3dsAscii(FILE* f, Scene* s)
{
  char buf[BSIZE];
  int a,b,c=0;
  float x,y,z,u,v=0;
  unsigned int i=0;
  int t=0;
  char tmpc=0;
  char * str= NULL;
  unsigned int nv = 0;
  unsigned int nf =0;
  Object *o = NULL;
  
  //debug("+ readScene3dsAscii" );  
  assert(f);
  assert(s);
  
  buf[0]= 0;
  str = & tmpc;
  initScene(s);
  t = fgets_tag(f,buf,&str);  
  while ( t >=0 ) {
    //debug(buf);
    switch(t) {
    case TRIMESH:
      //debugf(".read trimesh %d\n",s->no);
      s->vo =(Object*)  realloc( s->vo, ++(s->no) * sizeof(Object) );
      
      o = & (s->vo)[ (s->no) - 1];
      sscanf( str, "%s %d %s %d", buf, &nv, buf, &nf );
      //debugf( "Vertices=%d; Faces=%d;\n",  nv , nf );
      
      o->vv  = (Vertex *) calloc( nv ,sizeof(Vertex) );
      o->vf = (Face *) calloc( nf, sizeof(Face) );
      o->nv = nv; 
      o->nf = nf;
      //debugf("read vertexes (%d)\n", o->nv);
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
        //debug(".texture");
        u = v = 0;
        str = skipafter( str, "U:"); 
        if (str) sscanf(str,"%f",&u);
        str = skipafter( str, "V:"); 
        if (str) sscanf(str,"%f",&v);
                
        //assert3gl( x , -BOUND , +BOUND );
        //assert3gl( y , -BOUND , +BOUND );
        //assert3gl( z , -BOUND , +BOUND );
        //assert3gl( u , 0 , 1);
        //assert3gl( v , 0 , 1);
        o->vv[i].x = x; o->vv[i].y = y; o->vv[i].z = z;
        o->vv[i].u = u; o->vv[i].v = v;
        //printV( o->vv[i] );
      }
      t = fgets_tag(f,buf,&str);  
    case FACELIST:
      //debugf("read faces %d\n",nf);
      t = fgets_tag(f,buf,&str);
      for( i=0; i<nf; i++ ) { 
        //debugf("face[%d]=%s\n",i,buf);
         //str= skipafter( str, ":");
        //debug(str);
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
        //assert3gl( a , 0 , o->nf);
        //assert3gl( b , 0 , o->nf);
        //assert3gl( c , 0 , o->nf);
        //

        t = fgets_tag(f,buf,&str);
        //debug(str);
        
        switch ( t ) {
        case MATERIAL :
          str= skipafter( str, ":");
          sscanStrQuoted(str, & (o->vf[i].material) );
          //debugf(".-Material[%d]=\"%s\"\n",i, o->vf[i].material );
          t = fgets_tag(f,buf,&str);
          //debugf("%d=%s\n",t,buf);
        case SMOOTHING:
          //assert( t == SMOOTHING); // ??? I dont understand 
          
          //debugf("%d=%s\n",t,buf);
          switch ( t ) {
            case SMOOTHING:
              // debugf(".? %d Smoothing=\"%s\",\n",t,buf);
              str= skipafter( str, ":");
              sscanf(str,"%d", &t);
              o->vf[i].smoothing = t;
          //t = fgets_strcmp_token(f,buf); str = buf;
              t = fgets_tag(f,buf,&str);
              //debug(buf);
          }
          
        }
      }
    }
  }
  //debugf( "no=%d\n",s->no);
  //debugf("read v=(%d) f=(%d)\n", o->nv, o->nf);
  
  //printObject(o);
  //fprintObject(stdout, o);
  //debugf("- readScene3dsAscii (%d)\n",s->no);  
  return s->no;
}

int errorIO(FILE* f,int const t,char const * const msg)
{
  fclose( f );
  printf("%s\n", msg);
  perror("!!! Err/IO/");
  return( t );
  exit(t);
}

char* skipafter(char* src, char* pat) 
{
  char* str=0;
  //debugf("+ skipafter [%s]@[%s]\n",pat,src);
  //assert(src);
  if ( src == 0 ) return 0;
  str=strstr(src, pat);
  if (str) str+=strlen(pat);
  //debug(str);
  return str;
}
  

int fgets_strcmp_token(FILE* f, char* buf) 
{
  char* str=0;
  int i=0;
  *buf=0;
  assert(buf);
  assert(f);
  //debug("+ fgets_strcmp_token");
  do {
    str = fgets(buf, BSIZE,f);
    //debug(buf);
    //assert(str); 
    if ( str == 0 ) return -1;
    //buf [ strlen(buf) - 2 ] = 0;
    //debugf("+ [%s]\n",buf);
    for (i=0; (i<nkey) || (str !=0 ) ;i++) {
      //debugf("? k[%i]=%s\n",i,keyword[i]);
      str = strstr( buf, keyword[i] );
      if (str ) return i;
    }
  } while( str == 0 );
  //assert(str);
  //debug("- fgets_strcmp_token");
  return -2;
}

int fgets_tag(FILE* f, char* buf, char** strarg) 
{
  int t=0;
  char * str = 0;
  
  //debug("+fgets_tag");
  assert(f);
  assert(buf);
  assert(strarg);
  t = fgets_strcmp_token(f, buf);
  //assert(t >= 0);
  //debug(buf);
  //debug(keyword[t]);
  if (t >=0 ) str = skipafter( buf, keyword[t] );
  else { str = 0; }
  //debug(str);
  *strarg = str;
  //debug("-fgets_tag");
  return t;
}

int saveScene(Scene const * const in, char const * const filename)
{
  int t=0;
  unsigned int i=0; FILE* f=0;
  
  f=fopen( filename, "wt");
  if ( ! f ) return (int) f;
  for(i=0; i < in->no; i++) {
    fprintf(f,"Named object : \"RzR_%dof%d\"\r",i+1,in->no);
    t = fprintObject( f, & (in->vo[i]) );
  }
  fclose(f);
  return t;
}


int saveObject(Object const * const o, char const * const filename)
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

int fprintObject(FILE* f, Object const * const  o)
{
  unsigned int i=0;

  fprintf(f,"\r\nNamed object: \"rzr_%dv%df\"\r\n",o->nv, o->nf);
  fprintf(f,"\r\nTri-mesh, Vertices: %d Faces: %d\r\n",o->nv, o->nf);
  fprintf(f,"\r\nVertex list\r\n");
  
  for(i=0;i<o->nv;i++) {
    fprintf( f,"Vertex %d: X: %f Y: %f Z: %f ",i,
	     o->vv[i].x,  o->vv[i].y, o->vv[i].z);
    fprintf( f,"\t U: %f V: %f \r\n", o->vv[i].u,  o->vv[i].v);
  }
  fprintf(f,"\r\nFace list\r\n");
  for(i=0;i<o->nf;i++) {
    fprintf( f,"Face %d: A:%d B:%d C:%d",i,
	     o->vf[i].a,  o->vf[i].b, o->vf[i].c);
    fprintf(f,"\t AB:1 BC:1 CA:1 \r\n"); //!!!
    if( o->vf[i].material)
      fprintf(f, "Material: \"%s\"\r\n",o->vf[i].material);
    fprintf(f, "Smoothing:1\r\n");
    //fprintf( f, "Smoothing: \"%d\"\r\n",o->vf[i].smoothing);
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
  char tmpc;
  int nv = 0;
  int nf =0;
  char * str = NULL;
  //debug("+ parse3dsAscii");  
  exit(0);
  
  buf[0]= 0;
  str = & tmpc;
  //debug("skip to 1st object");
  t = fgets_tag(f,buf,&str);  
  while ( t >=0 ) {
    switch(t) {
    case TRIMESH:
      //debug("read trimesh");
      sscanf( str, "%s %d %s %d", buf, &nv, buf, &nf );
      //debugf( "Vertices=%d; Faces=%d;\n",  nv , nf );
      
      o->vv  = (Vertex *) calloc( nv ,sizeof(Vertex) );
      o->vf = (Face *) calloc( nf, sizeof(Face) );
      o->nv = nv; 
      o->nf = nf;
      //debugf("read vertexes (%d)\n", o->nv);
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
        // assert3gl( y , -BOUND , +BOUND ); assert3gl( z , -BOUND , +BOUND );
        o->vv[i].x = x; o->vv[i].y = y; o->vv[i].z = z;
        //exit(0);
      }
      t = fgets_tag(f,buf,&str);  
    case FACELIST:
      //debugf("read faces %d\n",nf);
      t = fgets_tag(f,buf,&str);
      for( i=0; i<nf; i++ ) { //Vertex 0:  X:-1.0	Y:1.0	Z:1.0
        //str= skipafter( str, ":");
        //debug(str);
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
        //assert3gl( a , 0 , o->nf);
        //assert3gl( b , 0 , o->nf);        assert3gl( c , 0 , o->nf);
        t = fgets_tag(f,buf,&str);
        //debug(str);
        switch ( t ) {
        case MATERIAL :
          str= skipafter( str, ":");
          sscanStrQuoted(str, & (o->vf[i].material) );
          //debugf(".-Material[%d]=\"%s\"\n",i, o->vf[i].material );
          t = fgets_tag(f,buf,&str);
          //debugf("%d=%s\n",t,buf);
        case SMOOTHING:
          //debug(str);
          //debug(pat);
          str= skipafter( str, ":");
          sscanf(str,"%d", &t);
          o->vf[i].smoothing = t;
          t = fgets_strcmp_token(f,buf); str = buf;
        }
      }
      
    }
  }
  //debugf("read v=(%d) f=(%d)\n", o->nv, o->nf);
  //debug("- parse3dsAscii");  
  return 0;
}

int load3dsAscii(char* filename, Object* o)
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

#endif // inclusion
/* #ident "$Id: Obj3dsUtil.cpp,v 1.2 2003/06/01 22:37:44 rzr Exp $" */
#ifndef color3ds_h_ 
#define color3ds_h_ 
/**
 * Unstable 
 * @author: www.Philippe.COVAL.online.fr - Rev: $Author: rzr $ 
 * Copyright: GPL @ http://rzr.online.fr/licence.htm
 **/

int convertColorUint32ToRGB(unsigned int const src, char* dest);

int convertColorRGBFloatToString(float const r, float const g, float const b,
                                 const float a, char* dest);

int convertColorRGBFloatToRGB8(float const r, float const g, float const b,
                               unsigned char * dr, 
                               unsigned char * dg,unsigned char * db);

int convertColorRGBFloatToAnsi(float const r, float const g, float const b,
                               int * res);

int convertColorToRGB8(unsigned int const src, 
                       unsigned  char* r, unsigned  char* g, unsigned  char* b,
                       unsigned  char* a);

int convertColorToRGBFloat(unsigned int const src,
                           float* r, float* g, float* b , float *a);

int convertColorStringRGBAToRGBA8(char const * const p,
                                  unsigned char *r, unsigned char* g, 
                                  unsigned char* b,unsigned char* a,
                                  unsigned int* dest);

int convertColorStringRGBAToRGBAFloat(char const * const p,
                                      float *r, float* g, float* b, float* a,
                                      unsigned int* dest);

int convertColorStringToRGBAFloat(char const * const p,
                                  float *r, float* g, float* b, float* a,
                                  unsigned int* dest);
///
unsigned int getColor(char const * const p);
///
unsigned int getColorRGB(unsigned char* r, unsigned char*g, unsigned char*b , 
                         char const * const p) ;
///
unsigned int getColorRGBFloat(float* r, float*g, float*b , float *a,
                              char const * const p) ;
/// r010g256b0a0
int getColorEncodedUint32(unsigned int const src, char* dest);

unsigned int getColorEncoded(unsigned char*r, unsigned char* g, 
                             unsigned char* b, unsigned char* a,
                             char const * const p);

int getColorTable(char const * const src, unsigned int * dest);

#endif //_h_ inclusion 
//$Id: Obj3dsUtil.cpp,v 1.2 2003/06/01 22:37:44 rzr Exp $
/* #ident "$Id: Obj3dsUtil.cpp,v 1.2 2003/06/01 22:37:44 rzr Exp $" */
/**
 * @author: www.Philippe.COVAL.online.fr - Rev: $Author: rzr $ 
 * Copyright: GPL @ http://rzr.online.fr/licence.htm
 **/

#ifndef color3ds_c_ 
#define color3ds_c_ 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define float2char(x) ( ( unsigned char) ( (x) * 255.) )
#define char2float(x) ( ( (unsigned char) (x)) / 255. )
#define toUint32(r,g,b) ( (r<<16) + (g<<8) + b )
#define blue(x) ( (x) & 0x0000FF )
#define green(x) ( (x>>8) & 0x0000FF )
#define red(x) ( (x>>16) & 0x0000FF )
#define alpha(x) ( (x>>24) & 0x0000FF )

int convertColorUint32ToRGB(unsigned int const src, char* dest)
{
  char r= src & 0xFF;
  char g= (src>>8) & 0xFF;
  char b= (src>>18) & 0xFF;
  char a= (src>>24) & 0xFF;
  
  //debugf("r%dg%db%da%d", r,g,b,a);
  sprintf(dest,"r%dg%db%da%d", r,g,b,a);
  return 0;
}

int convertColorRGBFloatToString(const float r, const float g, const float b,
                                 const float a, char* dest)
{
  unsigned char cr,cg,cb,ca=0;
  cr= float2char(r);
  cg= float2char(g);
  cb= float2char(b);
  ca= float2char(a);
  dest="r255g255b255a000";
  //debugf("r%dg%db%da%d\0",cr,cg,cb,ca);
  exit(0);
  return 0;
}


int convertColorToRGB(unsigned int const src,
                      unsigned  char* r, unsigned char* g,unsigned  char* b)
{
  //debug("+ convertColorToRGB");
  *b =  src & 0x0000FF;
  *g = (src>>8) & 0x0000FF;
  *r = (src>>16) & 0x0000FF;
  //debugf("%x={%x,%x,%x};\n",src,*r,*g,*b); //check here
  //  exit(0);
  return 0;
}

int convertColorToRGBFloat(unsigned int const src, 
                           float* r, float* g, float* b, float *a)
{
  *b = (float) ( src & 0x0000FF ) / 255.; 
  *g = (float) ( (src>>8) & 0x0000FF ) / 255.; 
  *r = (float) ( (src>>16) & 0x0000FF ) / 255.;
  *a = alpha( src);
  //debugf("%x={%f,%f,%f};\n",src,*r,*g,*b);
  return 0;
}

int convertColorRGBFloatToAnsi(float const r, float const g, float const b,
                               int * res) 
{
  //debugf("TODO");
  *res=1; 
  exit(0);
}



int convertColorStringRGBAToRGBA8(char const * const p,
                                  unsigned char *r, unsigned char* g,
                                  unsigned char* b,unsigned  char* a,
                                  unsigned int* dest)
{
  //debugf("+ getColorEncoded \"%s\"\n",p);
  char * pb=0, * pe =0;
  *dest=0xFFFFFF;
  *r = *g = *b = *a = 255;
  assert(p);
  //debug(".todo regexp on paterns like \"r27g88b176a0\"");
  //sscanf("r%dg%db%d", &r, &g, &b); // &tmpr, &r, &tmpg,g &tmpb, &b, p);

  if ( *p=='r' && strstr(p,"g") && strstr(p,"b") && strstr(p,"a") ) {
    pb = strstr(p,"r") +1;
    pe = strstr(pb,"g");
    if ( ! pe ) return -1;
    //assert(pe);
    *r = atoi(pb);
    pb = pe + 1;
    pe = strstr(pb,"b");
    if ( ! pe ) return -1;
    *g = atoi(pb);
    pb = pe + 1;
    pe = strstr(pb,"a");
    if ( ! pe ) return -1;
    *b = atoi(pb);
    pb = pe + 1;
    *a = atoi(pb);
  }
  *dest = toUint32(*r,*g,*b);
  //debug("- convertColorStringRGBAToRGBA8");
  return 0;
}

int convertColorStringRGBAToRGBAFloat(char const * const p,
                                      float *r, float* g, float* b, float* a,
                                      unsigned int* dest)
{
unsigned  char cr,cg,cb,ca;
  convertColorStringRGBAToRGBA8(p, &cr,&cg,&cb,&ca,dest);
  *r = char2float(cr);
  *g = char2float(cg);
  *b = char2float(cb);
  *a = char2float(ca);
  //debug("+ convertColorStringRGBAToRGBAFloat");
  return 0;
}



int getColorTable(char const * const p, unsigned int * dest)
{
  //debugf("+ getColorTable \"%s\"\n",p);
  *dest = 0xFFFFFF;
  if ( p == 0 ) return -1;
  if ( strstr( p, "BLACK") ) {  *dest = 0x000000;  }
  else if ( strstr( p, "WHITE") ) {  *dest = 0xFFFFFF;  }
  else if ( strstr( p, "BLUE") ) { *dest = 0x0000FF; }
  else if ( strstr( p, "GREEN") ) { *dest = 0x00FF00; }
  else if ( strstr( p, "RED") ) { *dest = 0xFF0000;} 
  else if ( strstr( p, "CYAN") ) {  *dest = 0x00FFFF;  }
  else if ( strstr( p, "MAGENTA") ) {*dest = 0xFF00FF;  } 
  else if ( strstr( p, "YELLOW") ) {    *dest = 0xFFFF00;  }
  else if ( strstr( p, "GRAY") ) { *dest = 0xFFFFFF; }
  else { return -1; }
  // GREENBUG, COW,  etc
  //debugf("- getColorTable (%x)\n",*dest);
  return 0;
}

int convertColorStringToRGBAFloat(char const * const p,
                                  float *r, float* g, float* b, float* a,
                                  unsigned int* dest)
{
 unsigned char cr=255,cg=255,cb=255,ca=0;
  unsigned int ret=0xFFFFFF;
  int t=-1;
  
  //debugf("+ convertColorStringToRGBAFloat \"%s\"\n",p);
  *r = *g = *b = 1.; *a=0.; 
  *dest = ret;
  if ( p == 0 ) return -1;
  t = getColorTable(p, &ret );
  
  if( t >= 0 ) {
    cr=red(ret);    cg=green(ret);    cb=blue(ret);    ca=alpha(ret);
  } else 
    if (  ( *p=='r' && strstr(p,"g") && strstr(p,"b") && strstr(p,"a") ) ) {
      t = convertColorStringRGBAToRGBA8(p,&cr,&cg,&cb,&ca,&ret);
    }
  if ( t < 0 ) { //debugf("!!! Err/ Not Found \"%s\"\n",p); 
    return t; 
  }
  //debugf("%s={%u,%u,%u,%u}=%x\n",p,cr,cg,cb,ca,ret);  
  // assert(t);

  *r = char2float( cr);
  *g = char2float( cg); 
  *b = char2float( cb);
  *a = char2float( ca);
  *dest = ret;
  
  //debugf("%s={%f,%f,%f,%f}=%x\n",p,*r,*g,*b,*a,*dest);  
  //debugf("- convertColorStringToRGBAFloat \"%s\"\n",p);
  return 0;
}
//----------------------------------------------------------------------------
unsigned int getColor(char const * const p)
{ 
  unsigned char r=0,g=0,b=0,a=0;
  unsigned int ret=0xFFFFFF;
  int t=0;
  
  //debugf("+getColor \"%s\"\n",p);
  if ( p == 0 ) return ret;
  r= g = b = 0;
  if( getColorTable(p, &ret ) >= 0 ) return ret;
  if ( *p != 'r' )  return ret;
  t = convertColorStringRGBAToRGBA8(p,&r,&g,&b,&a,&ret);
  if ( t < 0 ) { ; //debugf("!!! Err/ Not Found \"%s\"\n",p);  
  }
  
  // assert(t);
  convertColorToRGB(ret, &r, &g, &b );
  //debugf("%s={%u,%u,%u}=%x\n",pb,r,g,b,ret);  
  return ret;
  
}


unsigned int getColorRGBFloat(float* r, float*g, float*b, float*a,
                              char const * const p) 
{
  //debug("+getColorRGBFloat");
  unsigned int c=0;
  c = getColor(p);
  convertColorToRGBFloat(c,r,g,b,a);
  //debugf("={ %f,%f,%f};\n",*r,*g,*b);
  return c;
}

unsigned int getColorRGB(unsigned char* r,unsigned  char*g, unsigned  char*b ,
                         char const * const p) 
{ 
  //debug("+getColorRGBFloat");
  unsigned int c;
  c = getColor(p);
  convertColorToRGB(c,r,g,b);
  //debugf("={ %f,%f,%f};\n",*r,*g,*b);
  return c;
}

//----------------------------------------------------------------------------
#if 0
main()
{
  float r,g,b,a;
  unsigned int ui;
  char* pstr = "GREEN METAL";
  r = g = b = a = 0;  
  convertColorStringToRGBAFloat( pstr, &r,&g,&b,&a,&ui);
  //debugf("%s={%f,%f,%f,%f}=%u\n",pstr,r,g,b,a,ui);
}
#endif

#endif //_h_ 
//EOF $Id: Obj3dsUtil.cpp,v 1.2 2003/06/01 22:37:44 rzr Exp $


//-----------------------------------------------------------------------------
#endif // 3dsview include/inline
} // C lang

//-----------------------------------------------------------------------------
#include <cstdio>
#include <cstdlib>
#include "Obj3dsUtil.h"
#include "Shape3D.h"
#include "Polygon.h"
#include "Group.h"

///
static int convert(Scene const & s , Shape3D & shape);
///
static int convert( Object const & o , Shape3D & shape);
///
static int convert( Shape3D const & shape, Object& dest);

int convert(Scene const & s , Shape3D & shape)
{
  //debugf("+ convert structures (%d)\n",s.no);
  int off=0;
  Polygon3D* poly = 0;
  float r=1,g=1,b=1,a=0;
  unsigned int ui32=0xFFFFFFFF;
  for (unsigned int j=0; j < s.no ; j++) {
    Object o = s.vo[j];
    for(unsigned int i=0;i<o.nv;i++) {
      shape.add( o.vv[i].x ,o.vv[i].y, o.vv[i].z , 
                 1,1,1,0, o.vv[i].u , o.vv[i].v  );
    }
    //debugf(".add triangles (%d)\n",o.nf);
    for(unsigned int k=0; k<o.nf;k++) {
      poly = new Polygon3D(&shape);
      poly->add( o.vf[k].a + off);
      poly->add( o.vf[k].c + off); // not conventional order
      poly->add( o.vf[k].b + off);
      shape.add( poly );
      //debugf("..add color (or textures?) (%s)\n", o.vf[k].material );
#if 1
      convertColorStringToRGBAFloat(o.vf[k].material , &r, &g, &b, &a, &ui32);
      shape.setColor( o.vf[k].a + off , r,g,b,a);
      shape.setColor( o.vf[k].b + off , r,g,b,a); 
      shape.setColor( o.vf[k].c + off , r,g,b,a);
      if ( o.vf[k].material)
        shape.m_sMaterialName = string(o.vf[k].material);
#endif
    }
    //debugf(".offset, serveral trimesh in one shape (%d)\n",off);
    off += o.nv; 
  }
  return off;
  //debug("- convert ");
}

int convert( Object const & o , Shape3D & shape)
{
  //debugf("+ convert structures (%d)\n",o.nv);
  Polygon3D* poly = 0;
  float r=1,g=1,b=1,a=0;
  unsigned int ui32=0;
  for(unsigned int i=0;i<o.nv;i++) {
    shape.add( o.vv[i].x ,o.vv[i].y, o.vv[i].z ,
               1,1,1,0, o.vv[i].u , o.vv[i].v  );
  }
  //debugf(".add triangles (%d)\n",o.nf);
  for(unsigned int k=0; k<o.nf;k++) {
    poly = new Polygon3D(&shape);
    poly->add( o.vf[k].a );
    poly->add( o.vf[k].c ); // not conventional order
    poly->add( o.vf[k].b );
    shape.add( poly );
    //debugf(".add color (or textures?) (%s)\n", o.vf[k].material );
#if 1
    convertColorStringToRGBAFloat(o.vf[k].material , &r, &g, &b, &a, &ui32);
    shape.setColor( o.vf[k].a  , r,g,b,a);
    shape.setColor( o.vf[k].b  , r,g,b,a); 
    shape.setColor( o.vf[k].c  , r,g,b,a);
    //debug("extention to Shape for post text loading");
    if ( o.vf[k].material)
      shape.m_sMaterialName = string(o.vf[k].material);
#endif
  }
  //debug("- convert ");
  return 0;
}

/// WARNING : may be loosy (polygons , colors)
int convert(Shape3D const & shape, Object& dest)
{
  //debug("+ Obj3dsUtil::convert");
  initObject(&dest);
  dest.nv = shape.m_vVtxSrc.size();
  dest.vv = (Vertex*) calloc( dest.nv , sizeof( Vertex));
  vector<Vertex3D>::const_iterator i = shape.m_vVtxSrc.begin();
  vector<TexCoord>::const_iterator itex = shape.m_vTexCoord.begin();
  unsigned int j=0;
  for ( ; i != shape.m_vVtxSrc.end(); i++) {
    dest.vv[j].x = (*i).x;    dest.vv[j].y = (*i).y;    dest.vv[j].z = (*i).z;
    dest.vv[j].u = (*itex).u;    dest.vv[j].v = (*itex).v;
    j++; itex++;
    //printV( dest.vv[j] );
  }
  dest.nf = 0;
  
  vector<Polygon3D*>::const_iterator k;
  for ( k = shape.m_vPolygon.begin() ; k != shape.m_vPolygon.end(); k++)
    dest.nf  += (*k)->m_vIndex.size() - 2;

  dest.vf = (Face*) calloc ( dest.nf, sizeof(Face));
  
  j=0; int n=0;
  for ( k = shape.m_vPolygon.begin(); k != shape.m_vPolygon.end(); k++) {
    n = (*k)->m_vIndex.size();
    for(int l=0;l< n-2; l++) { //!!! check this
      dest.vf[j].a = (*k)->m_vIndex[0];
      dest.vf[j].b = (*k)->m_vIndex[l+2]; //order mixed!!
      dest.vf[j].c = (*k)->m_vIndex[l+1];      
      
      //debug("WARNING may be loosy on multicolor neighbour triangles");
      Color color =  shape.m_vColor[ (*k)->m_vIndex[0] ] ;
      dest.vf[j].material = (char*) calloc( 1024 , 1); // rXXXgXXXbXXXaXXX
      sprintf( dest.vf[j].material , "r%dg%db%da%d" 
               ,  float2char(color.r), float2char(color.g) 
               , float2char(color.b), float2char(color.a) );
      
      //debugf( ".TODO, check vert of tri %d=%s\n",j, dest.vf[j].material);
      /// 
      dest.vf[j].smoothing = 1 ;
      j++;
    }
  }
  //++j;  assert2eq(j,dest.nf);
  //debug("- Obj3dsUtil::convert ");
  //dest.vf[j]
  return (int) dest.nv;
}


int convert( Group const & in , Scene & dest)
{
  //int l = in.m_vShape3D.size();
  //in.m_vShape3D
  exit(0); //TODO:
  return 0;
}

//-----------------------------------------------------------------------------
Obj3dsUtil* Obj3dsUtil::p_Instance = 0;

Obj3dsUtil* Obj3dsUtil::getInstance()
{
  if (!  p_Instance ) p_Instance = new Obj3dsUtil;
  return p_Instance;
}

int Obj3dsUtil::read( Shape3D& shape,  char const * const filename )
{
  //EM_COUT("+ Obj3dsUtil::read",1);
  char name[BSIZE];
  strcpy(name, filename);
  //Object o; initObject(&object);
  Scene scene; initScene(&scene);
  //debug("some transformations for testing");
  int t = loadScene3dsAscii(name,&scene);
  //assert( t == 0);
  if ( t <= 0 ) return t;
  t = convert( scene, shape);
  freeScene( &scene);
  //EM_COUT("- Obj3dsUtil::read",1);
  return t;
}

int Obj3dsUtil::write( Shape3D const & shape,  char const * const filename )
{
  //debugf("+ Obj3dsUtil::write \"%s\"\n",filename);
  int n=0;
  Object o;
  convert(shape, o);
  assert( n >= 0);
  saveObject( &o, filename);
  //exit(-1);
  //debugf("+ Obj3dsUtil::write %d",o.nv);
  return o.nv;
}


int Obj3dsUtil::write( Shape3D const & shape, FILE* file)
{
  //debugf("+ Obj3dsUtil::write \n");
  int n=0;
  Object o;
  convert(shape, o);
  assert( n >= 0);
  n = fprintObject( file, &o);
  //debugf("+ Obj3dsUtil::write %d",o.nv);
  return o.nv;
}



int Obj3dsUtil::read( Group& in,  char const * const filename )
{
  //EM_COUT("+ Obj3dsUtil::read",1);
  char name[BSIZE];
  Shape3D*  shape = 0;
  strcpy(name, filename);
  //Object o;
  Scene s; initScene(&s);
  //debug("some transformations for testing");
  //get3dsAscii(name, &o);
  int t = loadScene3dsAscii(name,&s);
  //assert( t == 0);
  if ( t <= 0 ) return t;
  //debugf("Convert structures %d\n",s.no);

  for (unsigned int j=0; j< s.no ; j++) {
    Object o = s.vo[j];
    shape = new Shape3D;
    convert(o,*shape);
    in.addShape3D(shape);    
  }
  freeScene( &s);
  
  //EM_COUT("- Obj3dsUtil::read",1);
  return 0;
}

int Obj3dsUtil::write( Group & in, FILE* file)
{
  Shape3D *shape = 0;
  Shape3D s;
  for(int i=0; i < in.getShape3DSize() ; i++) {
    shape = in.getShape3D(i);
    s = *shape;
    write(  s, file);
  }
  return 0;
}
int Obj3dsUtil::write( Group  & in, char const* const filename  )
{
  //debugf("+ Obj3dsUtil::write");
  FILE* f = fopen( filename, "wt");
  if ( f == 0 ) return -1;
  int t = write( in, f);
  fclose(f);
  return t;
}

#endif // RZR_PATCHES_3DS 
#endif // inclusion 
//EOF $Id: Obj3dsUtil.cpp,v 1.2 2003/06/01 22:37:44 rzr Exp $
