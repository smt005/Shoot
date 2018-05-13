#pragma once

class Map;
class Object;
class Model;
class Shape;
class Mesh;
class Texture;

class DrawEngine
{
private:
	static float _backgroundColor[4];
	static unsigned int _programBase;
	static unsigned int _cuttrentBufer;
	static unsigned int _cuttrentTexture;
	static Texture* _textureTemp;

	static unsigned int _programLine;

	static float _lightDirection[3];
    static float _lightColor[4];
    static float _ambientColor[4];

public:
	static void setBackgroundColor(const float *color);
	static void setBackgroundColor(const float &red, const float &green, const float &blue, const float &alpha);
	static void setLightDirect(const float& x, const float& y, const float& z);
    static void setLightColor(const float& red, const float& green, const float& blue, const float &alpha = 1.0f);
    static void setAmbientColor(const float& red, const float& green, const float& blue, const float &alpha = 1.0f);
    
    static const float* getBackgroundColor();
	static const float* getLightDirect();
    static const float* getLightColor();
    static const float* getAmbientColor();
public:
	static void clearColor();
	static void clearColor(const float &red, const float &green, const float &blue, const float &alpha);
	static void prepareDraw(bool clear);

	// DrawMap
	static void initDrawMap();

	static void drawMap(Map& map);
	static void drawMapPhysic(Map& map);

	static void drawObject(Object& object, const float scale = 1.0f, const float alpha = 1.0f);
	static void drawModel(Model& model, const float* matrix, const float alpha = 1.0f);
	static void drawMesh(Mesh& mesh, Texture* texture);

	// DrawLines
	static void initDrawLines();
	static void prepareDrawLine();
	static void drawLine(float* point0, float* point1, float* color = nullptr);
};
