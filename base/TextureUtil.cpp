/***************************************************************************
                          TextureUtil.cpp  -  description
                             -------------------
    begin                : Mon Nov 27 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


// TODO free textures and image memory on exit/clear function
#include "Private.h"
#include "TextureUtil.h"
#include "Config.h"

#if EM_USE_SDL
#include <GL/gl.h>
// TODO remove glu
#if EM_DEBUG
#include <GL/glu.h>
#endif
#include <SDL/SDL.h>
#include <SDL_image.h>

extern "C" {

struct_image* loadP(const char * filename) {
	SDL_Surface* surface = IMG_Load(filename);
	if (surface == NULL) {
		cerr << "::loadP could not load: " << filename << endl;
		return NULL;
	}
	struct_image* image = (struct_image*) malloc(sizeof(struct_image));
	image->width = surface->w;
	image->height = surface->h;
	// asume 24 bits are 3*8 and 32 are 4*8
	if (surface->format->BitsPerPixel == 24) {
		image->channels = 3;
	} else if (surface->format->BitsPerPixel == 32) {
		image->channels = 4;
	} else {
		cerr << "::loadP Only 32 bit RGBA and 24 bit RGB images supported" << endl;
		// TODO free surface struct
		free(image);
		return NULL;
	}
	image->pixels = (unsigned char*) surface->pixels;
	return image;
}

}
#endif // EM_USE_SDLIMAGE

#if EM_USE_ALLEGRO
#include <allegro.h>
BITMAP * backbuffer = NULL;
ZBUFFER * zbuffer = NULL;
#endif // EM_USE_ALLEGRO


TextureUtil* TextureUtil::p_TextureUtil = NULL;

TextureUtil::TextureUtil() {
	m_colClear.r = 0.0f;
	m_colClear.g = 0.0f;
	m_colClear.b = 0.0f;
	m_colClear.a = 0.0f;
}

TextureUtil::~TextureUtil() {
}

TextureUtil* TextureUtil::getInstance() {
	if (p_TextureUtil == NULL) {
		p_TextureUtil = new TextureUtil();
	}
	return p_TextureUtil;
}


void TextureUtil::initGrx() {
	Config * config = Config::getInstance();
	
#if EM_USE_SDL
	cerr << "Initing SDL" << endl << endl;
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
		cerr << "Couldn't initialize SDL video" << SDL_GetError() << endl;
		exit(1);
	}

	if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0) {
		cerr << "Couldn't initialize SDL joystick: " <<  SDL_GetError() << endl << endl;
	} else {
		int njoystick = SDL_NumJoysticks();
		cerr << njoystick << " joysticks were found." << endl;
		if (njoystick != 0) {
			cerr << "The names of the joysticks are:" << endl;
			for(int a=0; a<njoystick; a++ ) {
				cerr << "  " << SDL_JoystickName(a) << endl;
			}
			cerr << "Using " << SDL_JoystickName(0) << endl << endl;
			SDL_JoystickOpen(0);
			SDL_JoystickEventState(SDL_ENABLE);
		}
	}

	// See if we should detect the display depth 
	if ( SDL_GetVideoInfo()->vfmt->BitsPerPixel <= 8 ) {
		SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 2 );
		SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 3 );
		SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 3 );
	} else 	if ( SDL_GetVideoInfo()->vfmt->BitsPerPixel <= 16 ) {
		SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
		SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
		SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
	}	else {
		SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
		SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
		SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	}

	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	/* Initialize the display */
	SDL_Surface* screen = 
		SDL_SetVideoMode(config->getWidth(), config->getHeight(), config->getBpp(), 
										 SDL_OPENGL | (config->useFullScreen() ? SDL_FULLSCREEN : 0));

	//	if (config->useFullScreen()) {
	SDL_ShowCursor(SDL_DISABLE);
	//	}
	SDL_WM_SetCaption("Emilia Pinball", NULL);

	if (screen == NULL) {
		cerr << "Couldn't set video mode: " << SDL_GetError() << endl;
		exit(1);
	}

	cerr << "Vendor     : " << glGetString( GL_VENDOR ) << endl;
	cerr << "Renderer   : " << glGetString( GL_RENDERER ) << endl;
	cerr << "Version    : " << glGetString( GL_VERSION ) << endl;
	cerr << "Extensions : " << glGetString( GL_EXTENSIONS ) << endl << endl;

	int value;
	SDL_GL_GetAttribute( SDL_GL_RED_SIZE, &value );
	cerr << "SDL_GL_RED_SIZE: " << value << endl;
	SDL_GL_GetAttribute( SDL_GL_GREEN_SIZE, &value );
	cerr << "SDL_GL_GREEN_SIZE: " << value << endl;
	SDL_GL_GetAttribute( SDL_GL_BLUE_SIZE, &value );
	cerr << "SDL_GL_BLUE_SIZE: " << value << endl;
	SDL_GL_GetAttribute( SDL_GL_DEPTH_SIZE, &value );
	cerr << "SDL_GL_DEPTH_SIZE: " << value << endl;
	SDL_GL_GetAttribute( SDL_GL_DOUBLEBUFFER, &value );
	cerr << "SDL_GL_DOUBLEBUFFER: " << value << endl << endl;
	
	this->resizeView(config->getWidth(), config->getHeight());
#endif // EM_USE_SDL

#if EM_USE_ALLEGRO
	//config->setSize(320, 240);

	allegro_init();
  install_keyboard();
  install_timer();
  install_mouse();

	COLOR_MAP colorMap;
  RGB_MAP rgbMap;
	COLOR_MAP transMap;

  RGB* paPalette = (RGB*) calloc(256, sizeof(RGB));
  generate_332_palette(paPalette);
  // create rgb table
  create_rgb_table(&rgbMap, paPalette, NULL);
  rgb_map = &rgbMap;
  // create light table	and setup the truecolor blending functions.
  create_light_table(&colorMap, paPalette, 0, 0, 0, NULL);
  color_map = &colorMap;
	// texture and flat polygons are 50% transparent
	create_trans_table(&transMap, paPalette, 128, 128, 128, NULL);
  set_trans_blender(0, 0, 0, 128);
  // set the graphics mode
	int tc = GFX_AUTODETECT_WINDOWED, tw = config->getWidth();
	int th = config->getHeight(), tbpp = 16;
	/*
	set_gfx_mode(GFX_SAFE, 320, 200, 0, 0);
	set_palette(desktop_palette);
	
	if (!gfx_mode_select_ex(&tc, &tw, &th, &tbpp)) {
		allegro_exit();
		cerr << "Error setting safe graphics mode" << endl;
	}
	*/
	set_color_depth(tbpp);
	if (set_gfx_mode(tc, tw, th, 0, 0) != 0) {
		allegro_exit();
		cerr << "Error setting graphics mode " << endl << allegro_error << endl;
	}
  set_palette(paPalette);
	config->setSize(tw, th);
	set_projection_viewport(0, 0, tw, th);
	// Create back buffer.
  backbuffer = create_bitmap(tw, th);
  clear(backbuffer);
	zbuffer = create_zbuffer(backbuffer);
	set_zbuffer(zbuffer);
	clear_zbuffer(zbuffer, 0);
#endif // EM_USE_ALLEGRO
}

void TextureUtil::stopGrx() {
	cerr << "Stopping graphics...";
#if EM_USE_SDL
	SDL_Quit();
#endif
#if EM_USE_ALLEGRO
	allegro_exit();
#endif
	cerr << "ok." << endl;
}

void TextureUtil::setClearColor(float r, float g, float b, float a) { 
	m_colClear.r = r; 
	m_colClear.g = g; 
	m_colClear.b = b; 
	m_colClear.a = a;
#if EM_USE_SDL
	glClearColor(m_colClear.r, m_colClear.g, m_colClear.b, m_colClear.a);
#endif
}

void TextureUtil::resizeView(unsigned int w, unsigned int h) {
#if EM_USE_SDL
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);

	glClearColor(m_colClear.r, m_colClear.g, m_colClear.b, m_colClear.a);
	glClearDepth(1.0);

	glShadeModel(GL_SMOOTH);

 	glEnable(GL_CULL_FACE);
 	glCullFace(GL_BACK);
 	glFrontFace(GL_CW);

	//	glDisable(GL_DITHER);
	glDisable(GL_LIGHTING);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.5, 0.5, -0.5, 0.5, 0.5, 1000);
	glMatrixMode(GL_MODELVIEW);

#if OPENGL_LIGHTS 
	glEnable(GL_LIGHTING);
#endif

#endif // EM_USE_SDL
}


EmImage* TextureUtil::loadImage(const char* filename) {
#if EM_USE_SDL
	struct_image * image = loadP(filename);
	if (image == NULL) {
		cerr << "TextureUtil::loadImage unable to load " << filename << endl;
	}
	return image;
#endif // EM_USE_SDL
#if EM_USE_ALLEGRO
	RGB pal[256];
	BITMAP * bm = load_bitmap(filename, pal);
	if (bm == NULL) {
		cerr << "TextareUtil::loadImage unable to load " << filename << " : " << allegro_error << endl;
	}
	return bm;
#endif // EM_USE_ALLEGRO
}

EmTexture* TextureUtil::loadTexture(const char* filename) {
#if EM_USE_SDL
	// Load Texture
	struct_image* image;

	// look if the texture is already loaded
	if (m_hEmTexture.find(string(filename)) != m_hEmTexture.end()) {
		EM_COUT("TextureUtil::loadTexture found texture " << filename << " in cache", 1);
		map<string, EmTexture*>::iterator element = m_hEmTexture.find(string(filename));
		return (*element).second;
	}
  // load the texture
	image = loadP(filename);
	if (image == NULL) {
		cerr << "TextureUtil::loadTexture error loading file " << filename << endl;
		return NULL;
	}
	EmTexture* texture = (EmTexture*) malloc(sizeof(EmTexture));
	// Create Texture
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	// insert the texture into the chache
	m_hEmTexture.insert(pair<string, EmTexture*>(string(filename), texture));
	m_hImageName.insert(pair<EmTexture*, string>(texture, string(filename)));
	// 2d texture, level of detail 0 (normal), 3 components (red, green, blue), 
	// x size from image, y size from image, 
	// border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.

	GLenum comp;
	switch (image->channels) {
	case 3:	comp = GL_RGB; break;
	case 4: comp = GL_RGBA; break;
	default: comp = GL_RGB; 
		cerr << "TextureUtil::loadTexture unknown image format" << endl;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, comp, image->width, image->height, 0, comp, 
							 GL_UNSIGNED_BYTE, image->pixels);
	
	EM_COUT("loaded texture: " << filename, 1);
	EM_COUT("size " << image->width <<" "<< image->height, 1);
	//	EM_COUT("bytes per pixel " << (int)image->format->BytesPerPixel, 1);
	//	EM_COUT("bits per pixel " << (int)image->format->BitsPerPixel, 1);

	EM_GLERROR(" In TextureUtil::loadTexture ");

	return texture;
#endif // EM_USE_SDL
#if EM_USE_ALLEGRO
	RGB pal[256];
	BITMAP * bm = load_bitmap(filename, pal);
	if (bm == NULL) {
		cerr << "TextureUtil::loadTexture Unable to load " 
				 << filename << " : " << allegro_error << endl;
	}
	return bm;
#endif // EM_USE_ALLEGRO
}

const char * TextureUtil::getTextureName(EmTexture * tex) {
	map<EmTexture*, string>::iterator element = m_hImageName.find(tex);
	if (element != m_hImageName.end()) {
		return (*element).second.c_str();
	}
	cerr << "TextureUtil::getTextureName could not find image name" << endl;
	return NULL;
}

/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////

#if 0
#include "png.h"
#define EM_PNG_HEAD 4

// define as extern C, else the setjmp function won't work (I suppose...)
extern "C" {

struct_image* loadP(const char * filename) {
	png_byte buf[8];
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL, end_info = NULL;
	png_uint_32 width, height;
	int channels, depth, ctype, itype;
	//	SDL_Surface* surface = NULL;
	png_bytep* row_pointers = NULL;
	struct_image* image = NULL;
	// open file;
	FILE *fp = fopen(filename, "rb");
	if (!fp) {
		fprintf(stderr, "File not found\n") ;
		return NULL;
	}

	// check that it is a PNG file
	fread(buf, 1, EM_PNG_HEAD, fp);
	if (png_sig_cmp(buf, 0, EM_PNG_HEAD) != 0) {
		fprintf(stderr, "Not a PNG file\n");
		return NULL;
	}
	// initialize structs
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fprintf(stderr, "Error initializing PNG file\n");
		return NULL;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fprintf(stderr, "Error initializing PNG file\n");
		return NULL;
	}
	end_info = png_create_info_struct(png_ptr);
	if (end_info == NULL) {
		fprintf(stderr, "Error initializing PNG file\n");
		return NULL;
	}
	// some long jump compiler stuff
	if (setjmp(png_jmpbuf(png_ptr)) != 0) {
		fprintf(stderr, "Error reading PNG file\n");
		return NULL;
	}

	// init io
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, EM_PNG_HEAD);
	
	// set read mode
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING, NULL);
	// read info
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &depth, &ctype, &itype, NULL, NULL);
	channels = png_get_channels(png_ptr, info_ptr);
	if (depth != 8 || !(channels == 4 || channels == 3)) {
		fprintf(stderr, "Only 32 bit RGBA and 24 bit RGB images supported");
		return NULL;
	}
	//fprintf(stderr, "w %d, h %d, d %d c %d\n", width, height, depth, channels);

	image = (struct_image*)malloc(sizeof(struct_image));
	if (image == NULL) {
		fprintf(stderr, "Out of memory");
		return NULL;
	}
	image->width = width;
	image->height = height;
	image->channels = channels;
	image->pixels = (unsigned char*)calloc(width*height*channels, sizeof(unsigned char));

	// a bit unefficient to malloc twice
	row_pointers = png_get_rows(png_ptr, info_ptr);   
	for (unsigned int a=0; a<height; a++) {
		memcpy(image->pixels + a*width*sizeof(unsigned char)*channels, row_pointers[a], 
					 sizeof(unsigned char)*width*channels);
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	fclose(fp);
	
	return image;
}
}

#endif // 0
