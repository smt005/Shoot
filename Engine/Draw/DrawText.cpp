
#include "DrawText.h"
#include "Shader.h"
#include "Camera.h"
#include "Common/IncludesMatem.h"
#include "../Platform/Source/FilesManager.h"

#include<fstream>
#include<iostream>

using namespace glm;

using namespace std;

bool TextDrawManager::_state = false;
FT_Library TextDrawManager::_lib;
FT_Face TextDrawManager::_face;
char* TextDrawManager::_fd = 0;
unsigned int TextDrawManager::_program = 0;

void TextDrawManager::init(TextDrawContainer& textDraw)
{
    if (!_state)
    {
        _state = Shader::getShaderProgram(_program, "Shaders/Text/Text.vert", "Shaders/Text/Text.frag");
        _state = initFont("Fonts/Futured.ttf");
    }
    
    if (!_state) return;
    
    int dip = 100;
    int posX = 0;
    int posY = 0;

    initText(textDraw._textStr, textDraw._size, dip, posX, posY, textDraw._textureId, textDraw._width, textDraw._height);
}

void TextDrawManager::clean()
{
    if (_fd) delete[] _fd;
    FT_Done_FreeType(_lib);
    _state = false;
}

bool TextDrawManager::initFont(char* filePath)
{
    char* fileNameWithDir = FilesManager::getFullPath(filePath);
    FILE* fp = fopen(fileNameWithDir, "r");
    delete [] fileNameWithDir;
    
    if (!fp) return false;
    
    fseek(fp, 0, SEEK_END);
    long fs = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    _fd = new char[fs];
    fread(_fd, fs, 1, fp);
    fclose(fp);
    
    FT_Error err = FT_Init_FreeType(&_lib);
    if (err)
    {
        printf("TextDrawManager::initFont FT_Init_FreeType ERROR: %d", err);
        return false;
    }
    
    err = FT_New_Memory_Face(_lib, (const FT_Byte*)_fd, fs, 0, &_face);
    if (err)
    {
        printf("TextDrawManager::initFont FT_New_Memory_Face ERROR: %d", err);
        return false;
    }
    
    return true;
}

bool TextDrawManager::initText(const string &text2, int fontSize, int dpi, int x, int y, unsigned int& textureID, int& width, int& height)
{
	const wchar_t* text = L"XXX";
    //if (text.empty()) return false;
	int textLength = 3;// text.length();
    if (textLength == 0) return false;
    
    FT_Error err = FT_Set_Char_Size(_face, 0, fontSize * 64, dpi, dpi);
    if (err) return false;
    
    FT_GlyphSlot slot = _face->glyph;
    FT_UInt prevGlyph = 0;
    
    //-----------------------------------------
    int maxWidth = 0;
    width = 0;
    
    float space = fontSize * 0.5;
    height = fontSize + space;
    
    for (int i = 0; i < textLength; ++i)
    {
        if (text[i] == '\n')
        {
            height += (fontSize + space);
            width = 0;
        }
        
        FT_UInt glyphIndex = FT_Get_Char_Index(_face, text[i]);
        
        if (glyphIndex)
        {
            err = FT_Load_Glyph(_face, glyphIndex, FT_LOAD_FORCE_AUTOHINT);
            if (err) continue;
            
            err = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
            if (err) continue;
            
            width += (slot->advance.x >> 6);
            
            if (width > maxWidth)
            {
                maxWidth = width;
            }
        }
    }
    
    height -= space;
    width = maxWidth;
    //------------------------------------------
    
    int cointTexel = width * height;
    unsigned char* imageFont = new unsigned char[cointTexel];
    for (int i = 0; i < cointTexel; ++i) imageFont[i] = 0;
    
    float offset = 0;
    
    for (int i = 0; i < textLength; ++i)
    {
        if (text[i] == '\n')
        {
            offset += (fontSize + space);
            x = 0;
        }
        
        FT_UInt glyphIndex = FT_Get_Char_Index(_face, text[i]);
        
        if (glyphIndex)
        {
            err = FT_Load_Glyph(_face, glyphIndex, FT_LOAD_FORCE_AUTOHINT);
            if (err) continue;
            
            err = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
            if (err) continue;
            
            float y_ = y + fontSize - slot->bitmap_top;
            y_ += offset;
            
            drawBitmap(&slot->bitmap, x, y_, imageFont, width, height);
            x += (slot->advance.x >> 6);
        }
    }
    
    textureID = getTexture(imageFont, width, height);
    
    delete[] imageFont;
    return true;
}


int TextDrawManager::getTexture(unsigned char* imageFont, int height, int width)
{
	//viewTexture(imageFont, width, height);
	//viewTexture(imageFont, height, width);

    GLuint texture;
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
#ifdef BUILD_OSX
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, imageFont);
#elif BUILD_WIN_GLES
	unsigned char* imageFontRGB = new unsigned char[height * width * 4];

	int indexNew = 0;

	for (int col = 0; col < width; ++col)
	{
		for (int row = 0; row < height; ++row)
		{
			int index = col * height + row;

			indexNew = index * 4;
			imageFontRGB[indexNew] = imageFont[index];
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageFontRGB);

	delete[] imageFontRGB;
#endif

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    return texture;
}

void TextDrawManager::viewTexture(unsigned char* imageFont, int height, int width)
{
	if (!imageFont || !width || !height)
		return;

	static std::ofstream _WRITE_LOG("Log.txt", std::ios::app);

	_WRITE_LOG << "\n----------------------------------" << endl;

	for (int col = 0; col < width; ++col)
	{
		_WRITE_LOG << endl;

		for (int row = 0; row < height; ++row)
		{
			int index = col * height + row;

			_WRITE_LOG << imageFont[index];
		}
	}

	_WRITE_LOG << "\n----------------------------------" << endl;
	_WRITE_LOG.close();
	_WRITE_LOG.clear();
}

void TextDrawManager::drawBitmap(FT_Bitmap*  bitmap, FT_Int x, FT_Int y, unsigned char* imageFont, int width, int height)
{
	FT_Int  i, j, p, q;
	FT_Int  x_max = x + bitmap->width;
	FT_Int  y_max = y + bitmap->rows;

	for (i = x, p = 0; i < x_max; i++, p++)
	{
		for (j = y, q = 0; j < y_max; j++, q++)
		{
			if (i < 0 || j < 0 || i >= width || j >= height) continue;

			unsigned char point = bitmap->buffer[q * bitmap->width + p];
			int index = j * width + i;
			imageFont[index] = point;
		}
	}
}

//---------------

TextDrawContainer::TextDrawContainer()
{
    _textureId = 0;
    _width = 0;
    _height = 0;
    
    _size = 12;
    _alignment;
}

TextDrawContainer::TextDrawContainer(const string &text, int size, int posX, int posY, bool alignment)
{
    set(text, size, posX, posY, _alignment);
}

TextDrawContainer::~TextDrawContainer()
{
    glDeleteTextures(1, &_textureId);
}

void TextDrawContainer::set(const string &text, int size, int posX, int posY, bool alignment)
{
    _textStr = text;
    _size = size;
    _alignment = alignment;
    TextDrawManager::init(*this);
}

void TextDrawContainer::draw()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glUseProgram(TextDrawManager::_program);
    
    float width = _width * 0.0005;
    float height = _height * 0.0005;
    
    GLfloat vertexAttributes[] = {
        -width,    -height,    0.0f,
        -width,    +height,    0.0f,
        +width,    -height,    0.0f,
        
        -width,    +height,    0.0f,
        +width,    +height,    0.0f,
        +width,    -height,    0.0f
    };
    
    GLfloat texAttributes[] = {
        0.0f,    0.0f,
        0.0f,    1.0f,
        1.0f,    0.0f,
        
        0.0f,    1.0f,
        1.0f,    1.0f,
        1.0f,    0.0f
    };
    
    unsigned int a_position = glGetAttribLocation(TextDrawManager::_program, "a_position");
    unsigned int a_texCoord = glGetAttribLocation(TextDrawManager::_program, "a_texCoord");
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    int s_baseMap = glGetUniformLocation(TextDrawManager::_program, "s_baseMap");
    glUniform1i(s_baseMap, 0);
    
    float color[] = { 0.999, 0.999, 0.999, 0.999 };
    int vColor = glGetUniformLocation(TextDrawManager::_program, "vColor");
    glUniform4fv(vColor, 1, color);
    
    int uMatrix = glGetUniformLocation(TextDrawManager::_program, "uMatrix");
    const float* mat = Camera::current.matPV();
    glUniformMatrix4fv(uMatrix, 1, GL_FALSE, mat);
    
    glVertexAttribPointer(a_position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), vertexAttributes);
    glEnableVertexAttribArray(a_position);
    
    glVertexAttribPointer(a_texCoord, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), texAttributes);
    glEnableVertexAttribArray(a_texCoord);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void TextDrawContainer::drawOnScreen()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glUseProgram(TextDrawManager::_program);
    float scale = 0.25;
	//if (_width <= 0) _width = 1.0f;
    float k = (float)_height / (float)_width;
    float width = 1.0f * scale;
    float height = 1.0f * scale* k;
    float posX = -1.0f + height * 0.1;
    float posY = 1.0f - height - height * 0.1;
    
    GLfloat vertexAttributes[] = {
        posX,        posY,    0.0f,
        posX,        posY +height,    0.0f,
        posX+width,    posY,    0.0f,
        
        posX,        posY +height,    0.0f,
        posX+width,    posY +height,    0.0f,
        posX+width,    posY,    0.0f
    };
    
    GLfloat texAttributes[] = {
        0.0f,    0.0f,
        0.0f,    1.0f,
        1.0f,    0.0f,
        
        0.0f,    1.0f,
        1.0f,    1.0f,
        1.0f,    0.0f
    };
    
    mat4x4 matrix(1.0f);
    matrix = glm::scale(matrix, vec3(0.975, 0.975, 0.975));
    
    unsigned int a_position = glGetAttribLocation(TextDrawManager::_program, "a_position");
    unsigned int a_texCoord = glGetAttribLocation(TextDrawManager::_program, "a_texCoord");
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    int s_baseMap = glGetUniformLocation(TextDrawManager::_program, "s_baseMap");
    glUniform1i(s_baseMap, 0);
    
    float color[] = { 0.999, 0.999, 0.999, 0.999 };
    int vColor = glGetUniformLocation(TextDrawManager::_program, "vColor");
    glUniform4fv(vColor, 1, color);
    
    int uMatrix = glGetUniformLocation(TextDrawManager::_program, "uMatrix");
    glUniformMatrix4fv(uMatrix, 1, GL_FALSE, (float*)value_ptr(matrix));
    
    glVertexAttribPointer(a_position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), vertexAttributes);
    glEnableVertexAttribArray(a_position);
    
    glVertexAttribPointer(a_texCoord, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), texAttributes);
    glEnableVertexAttribArray(a_texCoord);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
