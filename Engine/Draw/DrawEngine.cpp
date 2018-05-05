
#include "DrawEngine.h"
#include "../App/App.h"
#include "Shader.h"
#include "Camera.h"
#include "../Object/Map.h"
#include "../Object/Object.h"
#include "../Object/Glider.h"
#include "../Object/Model.h"
#include "../Object/Shape.h"
#include "../Object/Shell.h"

#include <GL/glew.h>

float DrawEngine::_backgroundColor[] = {0.3f, 0.6f, 0.9f, 1.0f};
unsigned int DrawEngine::_programBase;
unsigned int DrawEngine::_cuttrentBufer = 0;
unsigned int DrawEngine::_cuttrentTexture = 0;
Texture* DrawEngine::_textureTemp = nullptr;

unsigned int DrawEngine::_programLine = 0;

float DrawEngine::_lightDirection[] = {1.0f, 2.0f, -0.5f};
float DrawEngine::_lightColor[]     = {0.95f, 1.0f, 0.9f, 1.0f};
float DrawEngine::_ambientColor[]   = {0.1f, 0.1f, 0.1f, 1.0f};

void DrawEngine::setBackgroundColor(const float *color)
{
	_backgroundColor[0] = color[0];
	_backgroundColor[1] = color[1];
	_backgroundColor[2] = color[2];
	_backgroundColor[3] = color[3];
}

void DrawEngine::setLightDirect(const float& x, const float& y, const float& z)
{
	glm::vec3 vec = glm::normalize(glm::vec3(x, y, z));

	_lightDirection[0] = vec.x;
	_lightDirection[1] = vec.y;
	_lightDirection[2] = vec.z;
}

void DrawEngine::setLightColor(const float& red, const float& green, const float& blue, const float &alpha)
{
    _lightColor[0] = red;
    _lightColor[1] = green;
    _lightColor[2] = blue;
    _lightColor[3] = 1.0f;
}

void DrawEngine::setAmbientColor(const float& red, const float& green, const float& blue, const float &alpha)
{
    _ambientColor[0] = red;
    _ambientColor[1] = green;
    _ambientColor[2] = blue;
    _ambientColor[3] = 1.0f;
}

void DrawEngine::setBackgroundColor(const float &red, const float &green, const float &blue, const float &alpha)
{
	_backgroundColor[0] = red;
	_backgroundColor[1] = green;
	_backgroundColor[2] = blue;
	_backgroundColor[3] = alpha;
}

const float* DrawEngine::getBackgroundColor()
{
    return _backgroundColor;
}

const float* DrawEngine::getLightDirect()
{
	return _lightDirection;
}

const float* DrawEngine::getLightColor()
{
    return _lightColor;
}

const float* DrawEngine::getAmbientColor()
{
    return _ambientColor;
}

void DrawEngine::clearColor()
{
	clearColor(_backgroundColor[0], _backgroundColor[1], _backgroundColor[2], _backgroundColor[3]);
}

void DrawEngine::clearColor(const float &red, const float &green, const float &blue, const float &alpha)
{
	glClearColor(red, green, blue, alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DrawEngine::prepareDraw(bool clear)
{
	int widthScreen = App::width();
	int heightScreen = App::height();
	glViewport(0.0, 0.0, widthScreen, heightScreen);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (clear)
	{
		clearColor();
	}
}

// DrawMap
void DrawEngine::initDrawMap()
{
    Shader::getShaderProgram(_programBase, "Shaders/BaseLight.vert", "Shaders/BaseLight.frag");
}

void DrawEngine::drawMap(Map& map)
{
	if (!_programBase) return;

	prepareDraw(true);
	glUseProgram(_programBase);

	GLuint u_matProjectionView = glGetUniformLocation(_programBase, "u_matProjectionView");
	glUniformMatrix4fv(u_matProjectionView, 1, GL_FALSE, Camera::current.matPV());

    GLuint u_lightDirection = glGetUniformLocation(_programBase, "u_lightDirection");
    glUniform3fv(u_lightDirection, 1, _lightDirection);

    GLuint u_color = glGetUniformLocation(_programBase, "u_color");
    glUniform4fv(u_color, 1, _lightColor);
    
    GLuint u_ambientColor = glGetUniformLocation(_programBase, "u_ambientColor");
    glUniform4fv(u_ambientColor, 1, _ambientColor);

	for (auto object : map._objects)
	{
		drawObject(*object);
	}

	vector<Glider*>& gliders = map._gliders;

	for (auto glider : gliders)
	{
		drawObject(*glider);
	}

	for (auto shell : Shell::getShells())
	{
		drawObject(*shell);
	}
}

void DrawEngine::drawMapPhysic(Map& map)
{
	if (!_programBase) return;

	if (!_textureTemp)
	{
		_textureTemp = new Texture("Textures/Greed/Greed_00.png", true);
	}

	prepareDraw(true);
	glUseProgram(_programBase);

	GLuint u_matProjectionView = glGetUniformLocation(_programBase, "u_matProjectionView");
	glUniformMatrix4fv(u_matProjectionView, 1, GL_FALSE, Camera::current.matPV());

	GLuint u_matViewModel = glGetUniformLocation(_programBase, "u_matViewModel");
	
	vector<Object*>& objects = map._objects;

	for (auto object : objects)
	{
		MeshPhysic* meshPhysic = object->getModel().getShape().getPhysic();

		if (meshPhysic)
		{
			const float* matrix = object->matrixFloat();
			glUniformMatrix4fv(u_matViewModel, 1, GL_FALSE, matrix);

			for (int iP = 0; iP < meshPhysic->_count; ++iP)
			{
				drawMesh(meshPhysic->_meshes[iP], _textureTemp);
			}
		}
	}

	vector<Glider*>& gliders = map._gliders;

	for (auto glider : gliders)
	{
		MeshPhysic* meshPhysic = glider->getModel().getShape().getPhysic();

		if (meshPhysic)
		{
			const float* matrix = glider->matrixFloat();
			glUniformMatrix4fv(u_matViewModel, 1, GL_FALSE, matrix);

			for (int iP = 0; iP < meshPhysic->_count; ++iP)
			{
				drawMesh(meshPhysic->_meshes[iP], _textureTemp);
			}
		}
	}
}

void DrawEngine::drawObject(Object& object)
{
	drawModel(object.getModel(), object.matrixFloat());
}

void DrawEngine::drawModel(Model& model, const float* matrix)
{
	unsigned int textureId = model.textureId();
	Mesh& mesh = model.getMesh();
	if (!mesh._hasVBO) mesh.initVBO();

	if (mesh._buffer[3] != _cuttrentBufer)
	{
		GLuint a_position = glGetAttribLocation(_programBase, "a_position");
		GLuint a_texCoord = glGetAttribLocation(_programBase, "a_texCoord");
        GLuint a_normal = glGetAttribLocation(_programBase, "a_normal");
        
		glBindBuffer(GL_ARRAY_BUFFER, mesh._buffer[0]);
		glEnableVertexAttribArray(a_position);
		glVertexAttribPointer(a_position, SHAPE_VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ARRAY_BUFFER, mesh._buffer[1]);
		glEnableVertexAttribArray(a_texCoord);
		glVertexAttribPointer(a_texCoord, SHAPE_VERTEX_TEX_SIZE, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

        glBindBuffer(GL_ARRAY_BUFFER, mesh._buffer[2]);
        glEnableVertexAttribArray(a_normal);
        glVertexAttribPointer(a_normal, SHAPE_VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
        
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh._buffer[3]);

		_cuttrentBufer = mesh._buffer[3];
	}

	if (textureId != _cuttrentTexture)
	{
		GLuint s_baseMap = glGetUniformLocation(_programBase, "s_baseMap");
		glUniform1i(s_baseMap, 0);
		glBindTexture(GL_TEXTURE_2D, textureId);

		_cuttrentTexture = textureId;
	}

	GLuint u_matViewModel = glGetUniformLocation(_programBase, "u_matViewModel");
	glUniformMatrix4fv(u_matViewModel, 1, GL_FALSE, matrix);

	glDrawElements(GL_TRIANGLES, mesh._countIndex, GL_UNSIGNED_SHORT, 0);
}

void DrawEngine::drawMesh(Mesh& mesh, Texture* texture)
{
	if (!mesh._hasVBO) mesh.initVBO();

	if (mesh._buffer[3] != _cuttrentBufer)
	{
		GLuint a_position = glGetAttribLocation(_programBase, "a_position");
		GLuint a_texCoord = glGetAttribLocation(_programBase, "a_texCoord");

		glBindBuffer(GL_ARRAY_BUFFER, mesh._buffer[0]);
		glEnableVertexAttribArray(a_position);
		glVertexAttribPointer(a_position, SHAPE_VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ARRAY_BUFFER, mesh._buffer[1]);
		glEnableVertexAttribArray(a_texCoord);
		glVertexAttribPointer(a_texCoord, SHAPE_VERTEX_TEX_SIZE, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh._buffer[3]);

		_cuttrentBufer = mesh._buffer[3];
	}

	unsigned int textureId = 0;
	if (texture)
	{
		textureId = texture->id();
	}

	if (textureId != _cuttrentTexture)
	{
		GLuint u_color = glGetUniformLocation(_programBase, "u_color");
		GLuint s_baseMap = glGetUniformLocation(_programBase, "s_baseMap");

		float color[] = { 1.0, 1.0, 1.0, 1.0 };
		glUniform4fv(u_color, 1, color);

		glUniform1i(s_baseMap, 0);
		glBindTexture(GL_TEXTURE_2D, textureId);

		_cuttrentTexture = textureId;
	}

	glDrawElements(GL_TRIANGLES, mesh._countIndex, GL_UNSIGNED_SHORT, 0);
}

// DrawLines

void DrawEngine::initDrawLines()
{
	Shader::getShaderProgram(_programLine, "Shaders/LineMatrix.vert", "Shaders/LineMatrix.frag");
}

void DrawEngine::prepareDrawLine()
{
	if (!_programLine) return;

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(_programLine);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	int a_position = 0;
	glBindAttribLocation(_programLine, a_position, "a_position");

	glEnableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void DrawEngine::drawLine(float* point0, float* point1, float* color)
{
	unsigned int _u_matrix = glGetUniformLocation(_programLine, "u_matrix");
	unsigned int _u_pointSize = glGetUniformLocation(_programLine, "u_pointSize");
	unsigned int _u_color = glGetUniformLocation(_programLine, "u_color");

	
	if (color)
	{
		glUniform4fv(_u_color, 1, color);

		float width = color[0] * 3;
		glLineWidth(width);
	}
	else
	{
		float _colorRed[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		glUniform4fv(_u_color, 1, _colorRed);

		glLineWidth(2.0f);
	}

	glUniformMatrix4fv(_u_matrix, 1, GL_FALSE, Camera::current.matProjectViewFloat());
	
	GLfloat line[] = {point0[0], point0[1] , point0[2], point1[0], point1[1] , point1[2]};
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, line);
	glDrawArrays(GL_LINES, 0, 2);
}
