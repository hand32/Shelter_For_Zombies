//	Game Class Using OpenGL & Glut (Implementation - Static), ver. 0.6
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr

#pragma warning(disable: 4996)

#include "Game.h"

int CGame::nInstanceCnt = 0;
CGame* CGame::pInstance = NULL;

CObject* CGame::m_Objects[100];
int CGame::m_Objects_num = 0;
CObject* CGame::m_select_Object;

void CGame::SetVSync(bool bSync) {
	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;

	const char *extensions =
		(char*)glGetString(GL_EXTENSIONS);
	if (strstr(extensions, "WGL_EXT_swap_control") == 0)
	{
		return;
	}
	else 
	{
		wglSwapIntervalEXT =
			(PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");
		if (wglSwapIntervalEXT) wglSwapIntervalEXT(bSync);
	}
}

void CGame::DrawBitmapText(char *string, GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, int nW, int nH, bool bFixed2D) 
{
	char *c;

	glColor3f(r, g, b);

	if(bFixed2D)
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0.0, 640, 0.0, 480);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glRasterPos2f(x, y);
	}
	else
		glRasterPos3f(x, y, z);

	for(c = string ; *c != '\0' ; c++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}

	if(bFixed2D)
	{
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}
}

void CGame::DrawCoordinates()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);

	glLineWidth(3);
	glBegin(GL_LINES);
	
	glColor3f(1.0f, 0.0f, 0.0f); 
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);

	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(0.4f, 0.1f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(0.4f, -0.1f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f); 
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.5f, 0.0f);

	glVertex3f(0.0f, 0.5f, 0.0f);
	glVertex3f(0.0f, 0.4f, 0.1f);
	glVertex3f(0.0f, 0.5f, 0.0f);
	glVertex3f(0.0f, 0.4f, -0.1f);

	glColor3f(0.0f, 0.0f, 1.0f); 
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.5f);

	glVertex3f(0.0f, 0.0f, 0.5);
	glVertex3f(0.1f, 0.0f, 0.4f);
	glVertex3f(0.0f, 0.0f, 0.5f);
	glVertex3f(-0.1f, 0.0f, 0.4f);
	
	glEnd();
	glLineWidth(1);
}

void CGame::RenderSceneStatic()
{
	if(pInstance)
		pInstance->RenderScene();
}

void CGame::KeyDownStatic(unsigned char key, int x, int y)
{
	if(pInstance)
		pInstance->KeyDown(key, x, y);
}
void CGame::SpecialInputStatic(int key, int x, int y)
{
	if(pInstance)
		pInstance->SpecialInput(key, x, y);
}
void CGame::ResizeStatic(int width, int height)
{
	if(pInstance)
		pInstance->Resize(width, height);
}

void CGame::MouseStatic(int button, int state, int x, int y)
{
	if(pInstance)
		pInstance->Mouse(button, state, x, y);
}

void CGame::ShutdownAppStatic()
{
	if(pInstance)
		pInstance->ShutdownApp();
}
