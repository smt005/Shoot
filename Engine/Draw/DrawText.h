#pragma once

#include <string>

#ifdef BUILD_WIN_GLES
    #define GL_GLEXT_PROTOTYPES
	#include "GLES2/gl2.h"
#elif BUILD_WIN_GLFW
    #include <GL/glew.h>
#elif BUILD_OSX
    #include "glfw3.h"
#endif

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/freetype.h>

class TextDrawContainer
{
private:
public:
    unsigned int _textureId;
    int _width;
    int _height;
    
    std::string _textStr;
    int _size;
    bool _alignment;
    
public:
    TextDrawContainer();
    TextDrawContainer(const std::string& text, int size, int posX = 0, int posY = 0, bool alignment = false);
    
    ~TextDrawContainer();
    
    void set(const std::string& text, int size, int posX = 0, int posY = 0, bool alignment = false);
    void draw();
    void drawOnScreen();
};

class TextDrawManager
{
private:
public:
    static bool _state;
    static FT_Library _lib;
    static FT_Face _face;
    static char* _fd;
    static unsigned int _program;
    
    static bool initFont(char* filePath);
    static bool initText(const std::string &text, int fontSize, int dpi, int x, int y, unsigned int& textureID, int& width, int& height);
    static void drawBitmap(FT_Bitmap*  bitmap, FT_Int x, FT_Int y, unsigned char* imageFont, int width, int height);
    static int getTexture(unsigned char* imageFont, int width, int height);
    static void drawPlane(float width, float height);
    static void viewTexture(unsigned char* imageFont, int width, int height);

public:
    static void init(TextDrawContainer& textDraw);
    static void clean();
};
