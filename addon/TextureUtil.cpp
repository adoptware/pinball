/***************************************************************************
                          TexUtil.cpp  -  description
                             -------------------
    begin                : Mon Nov 27 2000
    copyright            : (C) 2000 by Henrik Enqvist
    email                : henqvist@excite.com
 ***************************************************************************/


#include "TextureUtil.h"
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

TextureUtil* TextureUtil::p_TextureUtil = NULL;

TextureUtil::TextureUtil() {
}

TextureUtil::~TextureUtil() {
}

TextureUtil* TextureUtil::getInstance() {
	if (p_TextureUtil == NULL) {
		p_TextureUtil = new TextureUtil();
	}
	return p_TextureUtil;
}

EmImage* TextureUtil::loadImage(char* fileName) {
	struct_image * image = loadP(fileName);
	if (image == NULL) {
		cerr << "Unable to load "<< fileName << endl;
		return NULL;
	}
	return image;
}

EmTexture* TextureUtil::loadTexture(char* filename) {
	// Load Texture
	struct_image* image;

	// look if the texture is already loaded
	if (m_hEmTexture.count(string(filename)) != 0) {
		EM_COUT("found texture " << filename << " in cache", 1);
		map<string, EmTexture*>::iterator element = m_hEmTexture.find(string(filename));
		return (*element).second;
	}
    
	image = loadP(filename);
	if (image == NULL) {
		cerr << "error loading file " << filename << endl;
		return NULL;
	}

	EmTexture* texture = (EmTexture*) malloc(sizeof(EmTexture));

	// Create Texture
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	// insert the texture into the chache
	m_hEmTexture.insert(pair<string, EmTexture*>(string(filename), texture));

	// 2d texture, level of detail 0 (normal), 3 components (red, green, blue), 
	// x size from image, y size from image, 
	// border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.

	GLenum comp;
	switch (image->channels) {
	case 3:	comp = GL_RGB; break;
	case 4: comp = GL_RGBA; break;
	default: comp = GL_RGB; cerr << "unknown image format" << endl;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, comp, image->width, image->height, 0, comp, GL_UNSIGNED_BYTE, image->pixels);
	
	EM_COUT("loaded texture: " << filename, 1);
	EM_COUT("size " << image->width <<" "<< image->height, 1);
	//	EM_COUT("bytes per pixel " << (int)image->format->BytesPerPixel, 1);
	//	EM_COUT("bits per pixel " << (int)image->format->BitsPerPixel, 1);

	EM_GLERROR(" In TextureUtil::loadTexture ");

	return texture;
}

