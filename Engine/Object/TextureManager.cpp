
#include "TextureManager.h"

#ifdef BUILD_WIN_GLES
	#define GL_GLEXT_PROTOTYPES
	#include "GLES2/gl2.h"
#elif BUILD_WIN_GLFW
    #include <GL/glew.h>
#elif BUILD_OSX
    #include "glfw3.h"
#endif

#include "FreeImage.h"
#include "../Platform/Source/FilesManager.h"

unsigned int TextureManager::LoadTexture(const char* fileName)
{
	char *fileNameWithDir = FilesManager::getFullPath(fileName);

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP *dib(0);
	BYTE* bits(0);
	unsigned int width(0), height(0);
	GLuint _textureId;

	fif = FreeImage_GetFileType(fileNameWithDir, 0);
	if (fif == FIF_UNKNOWN)
	{
		fif = FreeImage_GetFIFFromFilename(fileNameWithDir);
	}

	if (fif == FIF_UNKNOWN)
	{
		delete[] fileNameWithDir;
		return 0;
	}

	if (FreeImage_FIFSupportsReading(fif))
	{
		dib = FreeImage_Load(fif, fileNameWithDir);
	}

	if (!dib)
	{
		delete[] fileNameWithDir;
		return 0;
	}

	bits = FreeImage_GetBits(dib);
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	
	if ((bits == 0) || (width == 0) || (height == 0))
	{
		delete[] fileNameWithDir;
		return 0;
	}
	GLenum format = GL_RGB;
	if (FreeImage_GetBPP(dib) == 32)format = GL_RGBA;
	if (FreeImage_GetBPP(dib) == 24)format = GL_RGB;
	if (FreeImage_GetBPP(dib) == 8)format = GL_LUMINANCE;

	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, bits);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	delete[] fileNameWithDir;
	return  _textureId;
}
