//	Game Class Using OpenGL & Glut (Header), ver. 0.6
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr

#pragma once

#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/freeglut.h>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <windows.h>

#include "Hud.h"
#include "Object.h"
#include "Sphere.h"
#include "Cube.h"
#include "Man.h"

#include "GLSLProgram.h"
#include <glm/gtx/transform.hpp>

struct LightInfo {
	glm::vec4 Position; // Light position in eye coords.
	glm::vec3 La;       // Ambient light intensity
	glm::vec3 Ld;       // Diffuse light intensity
	glm::vec3 Ls;       // Specular light intensity
};
class CGame
{
public:
	static int nInstanceCnt;
	static CGame *pInstance;

	int m_nW, m_nH;
	int m_nPosX, m_nPosY;
	bool m_bCreated;

	float m_fBgColor[4];
	double m_dLookAt[3][3];
	
	int m_nFrame;
	int m_nCurrentTime, m_nBaseTime, m_nPreviousTime;
	bool m_bVSync, m_bFullscreen;

	bool m_bPause;

	CGLSLProgram m_Glsl[3];
	GLuint m_VaoHandle;
	glm::mat4 m_ProjectionMatrix;
	glm::vec4 m_projectionNums;
	glm::mat4 m_View;

	LightInfo m_worldLight;


	CHud m_Hud;
	static CObject *m_Objects[100];
	static int m_Objects_num;
	static CObject *m_select_Object;
	GLfloat *m_man_Vertex, *m_man_Normal;
	int m_nMan_VertexCnt, m_nSphere_VertexCnt, m_nCube_VertexCnt;

	CGame(int nW, int nH, int nPosX, int nPosY);
	~CGame();

	static void SetVSync(bool bSync);
	static void DrawBitmapText(char *string, GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, int nW, int nH, bool bFixed2D);

	static void DrawCoordinates();
	
	static void RenderSceneStatic();
	static void KeyDownStatic(unsigned char key, int x, int y);
	static void SpecialInputStatic(int key, int x, int y);
	static void ResizeStatic(int width, int height);
	static void MouseStatic(int button, int state, int x, int y);
	static void MouseWheelStatic(int button, int state, int x, int y);
	static void ShutdownAppStatic();

	void Create(int arg, char **argc, float *fBgColor, double (*dLookAt)[3], bool bVSync);

	void RenderScene();
	void KeyDown(unsigned char key, int x, int y);
	void SpecialInput(int key, int x, int y);
	void Resize(int width, int height);
	void Mouse(int button, int state, int x, int y);
	void MouseWheel(int button, int state, int x, int y);

	bool InitializeApp();
	void ShutdownApp();



	// Game ºÎºÐ.

	CObject* MakeBrick();
};

