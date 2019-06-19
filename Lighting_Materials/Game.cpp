//	Game Class Using OpenGL & Glut (Implementation Main), ver. 0.6
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr

#pragma warning(disable: 4996)

#include "Game.h"
#include "LoadObj.h"
#include "Bmp.h"
#include <string>
#include <cstdlib>
#include <glm/glm.hpp>

enum
{
	MOUSE_LEFT_BUTTON = 0,
	MOUSE_MIDDLE_BUTTON = 1,
	MOUSE_RIGHT_BUTTON = 2,
	MOUSE_SCROLL_UP = 3,
	MOUSE_SCROLL_DOWN = 4
};
CGame::CGame(int nW, int nH, int nPosX, int nPosY)
{
	m_nW = nW;
	m_nH = nH;
	
	m_nPosX = nPosX;
	m_nPosY = nPosY;

	m_mouseX = -1;
	m_mouseY = -1;
	m_click = false;
	m_zoomCnt = 0;

	m_level = 1;
	LevelControl();

	m_bPause = false;

	m_bCreated = false;

	if(nInstanceCnt == 0) pInstance = this;

	nInstanceCnt++;
}

CGame::~CGame()
{
	if(m_bCreated)
		ShutdownApp();

	nInstanceCnt--;
}

void CGame::Create(int arg, char **argc, float *fBgColor, double (*dLookAt)[3], bool bVSync)
{
	glutInitWindowSize(m_nW, m_nH);
	glutInitWindowPosition(m_nPosX, m_nPosY);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInit(&arg, argc);
	glutCreateWindow("Shelter For Zombies");
	
	glewInit();

	m_nFrame = 0;
	m_nBaseTime = glutGet(GLUT_ELAPSED_TIME); 
	m_nPreviousTime = m_nBaseTime;
	m_nCurrentTime = 0;
	m_projectionNums = glm::vec4(-26.f, 26.f, -26.f * m_nH / m_nW, 26.f * m_nH / m_nW);
	m_hud_center = new CCube();
	m_hud_center->SetScale(0.0f, 0.0f, 0.0f);
	m_hud_center->m_fPosition[0] = -0.f;
	m_hud_center->m_fPosition[1] = 5.f;
	m_hud_center->m_fPosition[2] = 30.f;
	//m_hud_center->m_gravity_on = false;

	glutDisplayFunc(RenderSceneStatic);
	glutReshapeFunc(ResizeStatic);
	glutKeyboardFunc(KeyDownStatic);
	glutSpecialFunc(SpecialInputStatic);
	glutMouseFunc(MouseStatic);
	glutMouseWheelFunc(MouseWheelStatic);
	glutMotionFunc(PassiveMotionStatic);

	atexit(ShutdownAppStatic);

	m_bVSync = bVSync;
	SetVSync(m_bVSync);

	int i;
	int r, c;
	for(i = 0 ; i < 4 ; i++)
		m_fBgColor[i] = fBgColor[i];

	for(r = 0 ; r < 3 ; r++)
		for(c = 0 ; c < 3 ; c++)
			m_dLookAt[r][c] = dLookAt[r][c];

	
	m_select_Object = NULL;
	CCube *newCube = new CCube();
	newCube->SetPosition(2.0f, 0.0f, 2.0f);
	newCube->SetScale(45.0f, 2.0f, 45.0f);
	newCube->SetColor(1.0f, 1.0f, 1.0f);;
	newCube->m_gravity_on = false;
	newCube->material.specular[0] = 0.1f;
	newCube->material.specular[1] = 0.1f;
	newCube->material.specular[2] = 0.1f;
	newCube->material.shininess = 20000.0f;
	newCube->m_state = GROUND;

	/*
	CMan *newMan = new CMan();
	newMan->m_gravity_on = false;
	newMan->SetPosition(2, 1.f, 2);
	newMan->SetColor(float(rand()) / RAND_MAX, float(rand()) / RAND_MAX, float(rand()) / RAND_MAX);
	newMan->SetScale(0.2f, 0.2f, 0.2f);
	newMan->SetRotation(3.141592, 0.0f, 1.0f, 0.0f);
	newMan->m_state = ZOMBIE;
	*/

	/*
	newCube = new CCube();
	newCube->m_state = BUILDING;
	newCube->SetPosition(0.0f, 5.0f, 0.0f);
	newCube->SetColor(1.0f, 1.0f, 1.0f);
	newCube->SetScale(5.0f, 10.0f, 5.0f);
	newCube->m_gravity_on = false;
	newCube->material.specular[0] = 0.1f;
	newCube->material.specular[1] = 0.1f;
	newCube->material.specular[2] = 0.1f;
	newCube->material.shininess = 20000.0f;
	*/

	LevelControl();
	MakeBrick();

	m_bFullscreen = false;

	if (InitializeApp() == true)
	{
		m_bCreated = true;
		glutMainLoop();
	}
}

void CGame::RenderScene()
{
	m_nBaseTime = glutGet(GLUT_ELAPSED_TIME);
	int nElapsedTime = m_nBaseTime - m_nPreviousTime;
	if (m_bPause == true)
	{
		m_nPreviousTime = m_nBaseTime;
		glutPostRedisplay();
		return;
	}
	
	if(nElapsedTime < 16.67)
	{
		glutPostRedisplay();
		return;
	}

	m_nCurrentTime += nElapsedTime;
	m_nPreviousTime = m_nBaseTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(m_fBgColor[0], m_fBgColor[1], m_fBgColor[2], m_fBgColor[3]);

	m_worldLight.Position = glm::vec4(50.0 * cos(m_nCurrentTime / 4000.0), 30.0, 50.0 * sin(m_nCurrentTime / 4000.0), 1.0);
	
	//m_worldLight.Position = glm::vec4(50.0 * cos(100 / 800.0), 30.0, 50.0 * sin(100 / 800.0), 1.0);
	m_worldLight.La = glm::vec3(0.3f, 0.3f, 0.3f);
	m_worldLight.Ld = glm::vec3(1.0f, 1.0f, 1.0f);
	m_worldLight.Ls = glm::vec3(1.0f, 1.0f, 1.0f);

	m_View = glm::lookAt(glm::vec3(m_dLookAt[0][0], m_dLookAt[0][1], m_dLookAt[0][2]),
		glm::vec3(m_dLookAt[1][0], m_dLookAt[1][1], m_dLookAt[1][2]),
		glm::vec3(m_dLookAt[2][0], m_dLookAt[2][1], m_dLookAt[2][2]));
	m_ProjectionMatrix =
		glm::ortho(m_projectionNums.x, m_projectionNums.y, m_projectionNums.z, m_projectionNums.w,
			-10.0f, 100.0f);
	m_DepthProjectionMatrix =
		glm::ortho(-26.f, 26.f, -20.f, 20.f, -10.0f, 100.f);

	bool makeBrick = false;
	bool *collideCheck = new bool[m_Objects_num*m_Objects_num];
	for (int i = 0; i < m_Objects_num*m_Objects_num; i++)
		collideCheck[i] = false;
	for (int i = 0; i < m_Objects_num; i++)
	{
		for (int j = 0; j < m_Objects_num; j++)
		{
			if (i != j && collideCheck[i * m_Objects_num +j] == false &&
				m_Objects[i]->m_state != CONTROL && m_Objects[j]->m_state != CONTROL)
			{
				CSphere *sphere = NULL;
				CCube *cube = NULL;
				CMan *man = NULL;
				if (m_Objects[i]->Collide(m_Objects[j]) == true)
				{
					collideCheck[i*m_Objects_num + j] = true;
					//collideCheck[j*m_Objects_num + i] = true;
					if (m_Objects[i]->m_type != MAN && m_Objects[i]->m_type != MANCOLLIDER)
					{
						m_Objects[i]->m_gravity_on = false;
						m_Objects[i]->SetVelocity(0.0f, 0.0f, 0.0f);
						if (m_Objects[i] == m_select_Object)
							makeBrick = true;
					}
					if(m_Objects[j]->m_type != MAN && m_Objects[i]->m_type != MANCOLLIDER)
					{
						m_Objects[j]->m_gravity_on = false;
						m_Objects[j]->SetVelocity(0.0f, 0.0f, 0.0f);
						if (m_Objects[j] == m_select_Object)
							makeBrick = true;
					}
					if (m_Objects[i]->m_type == MAN)
					{
						m_Objects[i]->GetRealClass(sphere, cube, man);
						m_Objects[i]->m_fPosition[0] = man->m_Collider->m_fPosition[0];
						m_Objects[i]->m_fPosition[1] = man->m_Collider->m_fPosition[1] - man->m_fScale[1] * 5;
						m_Objects[i]->m_fPosition[2] = man->m_Collider->m_fPosition[2];
					}
					if (m_Objects[j]->m_type == MAN)
					{
						m_Objects[j]->GetRealClass(sphere, cube, man);
						m_Objects[j]->m_fPosition[0] = man->m_Collider->m_fPosition[0];
						m_Objects[j]->m_fPosition[1] = man->m_Collider->m_fPosition[1] - man->m_fScale[1] * 5;
						m_Objects[j]->m_fPosition[2] = man->m_Collider->m_fPosition[2];
					}
					break;
				}
			}
		}
		m_Objects[i]->Gravity(nElapsedTime);
		m_Objects[i]->Move(nElapsedTime);
	}
	delete[] collideCheck;
	glBindFramebuffer(GL_FRAMEBUFFER, CGame::pInstance->m_frameBuffer);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_Glsl[1].Use();
	for (int i = 0; i < m_Objects_num; i++)
		if(m_Objects[i]->m_type != MANCOLLIDER)
			m_Objects[i]->RenderShadow();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_Glsl[0].Use();
	for (int i = 0; i < m_Objects_num; i++)
		if (m_Objects[i]->m_type != MANCOLLIDER)
			m_Objects[i]->RenderScene();
	m_hud_center->RenderScene();
	if (makeBrick == true)
		MakeBrick();

	m_nBaseTime = glutGet(GLUT_ELAPSED_TIME);
	m_Glsl[0].Stop();

	HudDisplay();

	if (m_points >= m_clear_points)
	{
		m_level++;
		LevelControl();
	}

	glutSwapBuffers();
	glutPostRedisplay();
}

void CGame::Resize(int width, int height)
{
	m_nW = width;
	m_nH = height;
	glViewport(0, 0, width, height);
	m_projectionNums = glm::vec4(-26.f, 26.f, -26.f * m_nH / m_nW, 26.f * m_nH / m_nW);

	glm::vec2 normalized = glm::normalize(glm::vec2(m_nW, m_nH));
	m_projectionNums.x += normalized.x * m_zoomCnt;
	m_projectionNums.y -= normalized.x * m_zoomCnt;
	m_projectionNums.z += normalized.y * m_zoomCnt;
	m_projectionNums.w -= normalized.y * m_zoomCnt;
}

void CGame::KeyDown(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;

	case 32: //Space Bar
		if (m_select_Object != NULL)
		{
			m_select_Object->m_gravity_on = true;
			m_select_Object->m_state = BUILDING;
		}
		break;

	case 'v':
		m_bVSync = !m_bVSync;
		SetVSync(m_bVSync);
		break;

	case 'p':
		m_bPause = !m_bPause;
		break;
	}
}

void CGame::SpecialInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		if (m_select_Object != NULL)
			if (m_select_Object->m_gravity_on == false)
				m_select_Object->Translate(-0.1f, 0.0f, -0.1f);
		break;

	case GLUT_KEY_DOWN:
		if (m_select_Object != NULL)
			if (m_select_Object->m_gravity_on == false)
				m_select_Object->Translate(0.1f, 0.0f, 0.1f);
		break;

	case GLUT_KEY_RIGHT:
		if (m_select_Object != NULL)
			if (m_select_Object->m_gravity_on == false)
				m_select_Object->Translate(0.1f, 0.0f, -0.1f);
		break;

	case GLUT_KEY_LEFT:
		if (m_select_Object != NULL)
			if (m_select_Object->m_gravity_on == false)
				m_select_Object->Translate(-0.1f, 0.0f, 0.1f);
		break;

	case GLUT_KEY_F11:
		if (!m_bFullscreen){
			glutFullScreen();
			
			m_bFullscreen = true;
		}
		else if (m_bFullscreen){
			glutReshapeWindow(m_nW, m_nH);
			glutPositionWindow(m_nPosX, m_nPosY);

			m_bFullscreen = false;
		}

		break;
	}
}


void CGame::Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		m_click = true;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (m_select_Object != NULL)
		{
			m_select_Object->m_gravity_on = true;
			m_select_Object->m_state = BUILDING;
		}
		m_click = false;
	}
}

void CGame::MouseWheel(int button, int state, int x, int y)
{
	if (state == 1)
	{
		if (m_dLookAt[0][1] > 3.0f)
		{
			m_dLookAt[0][1] -= 0.7f;
			m_zoomCnt++;
			glm::vec2 normalized = glm::normalize(glm::vec2(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)));
			m_projectionNums.x += normalized.x;
			m_projectionNums.y -= normalized.x;
			m_projectionNums.z += normalized.y;
			m_projectionNums.w -= normalized.y;
		}
	}
	else if (state == -1)
	{
		if (m_dLookAt[0][1] < 30.f)
		{
			m_dLookAt[0][1] += 0.7f;
			m_zoomCnt--;
			glm::vec2 normalized = glm::normalize(glm::vec2(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)));
			m_projectionNums.x -= normalized.x;
			m_projectionNums.y += normalized.x;
			m_projectionNums.z -= normalized.y;
			m_projectionNums.w += normalized.y;
		}
	}
}

void CGame::PassiveMotion(int x, int y)
{
	if (m_click == true && m_mouseX != -1 && m_mouseY != -1 && !(m_mouseX == x && m_mouseY == y))
	{
		glm::vec3 vx = glm::vec3(x - m_mouseX, 0, -(x-m_mouseX));
		glm::vec3 vy = glm::vec3(y - m_mouseY, 0, y - m_mouseY);
		glm::vec3 move = normalize(vx+vy) * 0.2f;
		if (m_select_Object != NULL)
			if (m_select_Object->m_gravity_on == false)
				m_select_Object->Translate(move.x, 0.0f, move.z);
	}
	m_mouseX = x;
	m_mouseY = y;
}

void InputVertex(int* index, GLfloat* array, const GLfloat* vertex)
{
	array[*index] = vertex[0];
	array[*index + 1] = vertex[1];
	array[*index + 2] = vertex[2];
	*index += 3;
}

bool CGame::InitializeApp()
{
	GLfloat *positionData = new GLfloat[(1600 + 30 * 30 * 6) * 9];
	GLfloat *normalData = new GLfloat[(1600 + 30 * 30 * 6) * 9];
	GLfloat *cubeVertex = new GLfloat[900 * 9];
	GLfloat *cubeNormal = new GLfloat[900 * 9];

	glClearColor(m_fBgColor[0], m_fBgColor[1], m_fBgColor[2], m_fBgColor[3]);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	
	glEnable(GL_CULL_FACE);

	if (!LoadObj("man.obj", &positionData, &m_nMan_VertexCnt, &normalData))
		return false;
	if (!LoadObj("cube.obj", &cubeVertex, &m_nCube_VertexCnt, &cubeNormal))
		return false;

	m_Glsl[0].Compile("phong.vsl", GAME_GL_VERTEX);
	m_Glsl[0].Compile("phong.fsl", GAME_GL_FRAGMENT);
	m_Glsl[0].Link();

	m_Glsl[1].Compile("shadow.vsl", GAME_GL_VERTEX);
	m_Glsl[1].Compile("shadow.fsl", GAME_GL_FRAGMENT);
	m_Glsl[1].Link();

	m_Glsl[2].Compile("Hud.vsl", GAME_GL_VERTEX);
	m_Glsl[2].Compile("Hud.fsl", GAME_GL_FRAGMENT);
	m_Glsl[2].Link();

	const double PI = 3.14159;
	int slice, stack;
	double sliceAngle1, stackAngle1;
	double sliceAngle2, stackAngle2;
	int nStack = 30, nSlice = 30;
	double sliceStep = 2 * PI / nSlice;
	double stackStep = PI / nStack;
	float radius = 0.5f;

	int m_nFaceNum = nStack * nSlice * 2;


	int index = m_nMan_VertexCnt / 3;
	m_nSphere_VertexCnt = 0;
	for (stack = 0; stack < nStack; stack++)
	{
		stackAngle1 = PI / 2 - stack * stackStep;
		stackAngle2 = PI / 2 - (stack + 1) * stackStep;

		for (slice = 0; slice < nSlice; slice++)
		{
			sliceAngle1 = slice * sliceStep;
			sliceAngle2 = (slice + 1) * sliceStep;

			GLfloat x0, y0, z0;
			GLfloat x1, y1, z1;
			GLfloat x2, y2, z2;
			GLfloat x3, y3, z3;

			x0 = radius * cos(stackAngle1) * cos(sliceAngle1);
			y0 = radius * cos(stackAngle1) * sin(sliceAngle1);
			z0 = radius * sin(stackAngle1);

			x1 = radius * cos(stackAngle2) * cos(sliceAngle1);
			y1 = radius * cos(stackAngle2) * sin(sliceAngle1);
			z1 = radius * sin(stackAngle2);

			x2 = radius * cos(stackAngle1) * cos(sliceAngle2);
			y2 = radius * cos(stackAngle1) * sin(sliceAngle2);
			z2 = radius * sin(stackAngle1);

			x3 = radius * cos(stackAngle2) * cos(sliceAngle2);
			y3 = radius * cos(stackAngle2) * sin(sliceAngle2);
			z3 = radius * sin(stackAngle2);

			positionData[index * 9] = x0;
			positionData[index * 9 + 1] = y0;
			positionData[index * 9 + 2] = z0;

			positionData[index * 9 + 3] = x2;
			positionData[index * 9 + 4] = y2;
			positionData[index * 9 + 5] = z2;

			positionData[index * 9 + 6] = x1;
			positionData[index * 9 + 7] = y1;
			positionData[index * 9 + 8] = z1;

			normalData[index * 9] = x0 / radius;
			normalData[index * 9 + 1] = y0 / radius;
			normalData[index * 9 + 2] = z0 / radius;

			normalData[index * 9 + 3] = x2 / radius;
			normalData[index * 9 + 4] = y2 / radius;
			normalData[index * 9 + 5] = z2 / radius;

			normalData[index * 9 + 6] = x1 / radius;
			normalData[index * 9 + 7] = y1 / radius;
			normalData[index * 9 + 8] = z1 / radius;

			m_nSphere_VertexCnt += 3;
			index++;

			positionData[index * 9] = x3;
			positionData[index * 9 + 1] = y3;
			positionData[index * 9 + 2] = z3;

			positionData[index * 9 + 3] = x1;
			positionData[index * 9 + 4] = y1;
			positionData[index * 9 + 5] = z1;

			positionData[index * 9 + 6] = x2;
			positionData[index * 9 + 7] = y2;
			positionData[index * 9 + 8] = z2;

			normalData[index * 9] = x3 / radius;
			normalData[index * 9 + 1] = y3 / radius;
			normalData[index * 9 + 2] = z3 / radius;

			normalData[index * 9 + 3] = x1 / radius;
			normalData[index * 9 + 4] = y1 / radius;
			normalData[index * 9 + 5] = z1 / radius;

			normalData[index * 9 + 6] = x2 / radius;
			normalData[index * 9 + 7] = y2 / radius;
			normalData[index * 9 + 8] = z2 / radius;
			m_nSphere_VertexCnt += 3;

			index++;
		}
	}

	int cubeIndex = (m_nSphere_VertexCnt + m_nMan_VertexCnt) * 3;
	int cnt = 0;
	for (int i = cubeIndex; i < cubeIndex + (m_nCube_VertexCnt) * 3; i++)
	{
		positionData[i] = cubeVertex[cnt];
		normalData[i] = cubeNormal[cnt];
		cnt++;
	}

	GLuint VboHandles[2];
	glGenBuffers(2, VboHandles);
	GLuint positionBufferHandle = VboHandles[0];
	GLuint normalBufferHandle = VboHandles[1];

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, (m_nMan_VertexCnt + m_nSphere_VertexCnt + m_nCube_VertexCnt) * 3 * sizeof(GLfloat), positionData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, (m_nMan_VertexCnt + m_nSphere_VertexCnt + m_nCube_VertexCnt) * 3 * sizeof(GLfloat), normalData, GL_STATIC_DRAW);


	// Create and set-up the vertex array object
	glGenVertexArrays(1, &m_VaoHandle);
	glBindVertexArray(m_VaoHandle);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	delete[] positionData;
	delete[] normalData;
	delete[] m_man_Vertex;
	delete[] m_man_Normal;
	delete[] cubeVertex;
	delete[] cubeNormal;

	GLuint HudHandles[3];
	glGenBuffers(3, HudHandles);
	m_hud_blocknum.bufferID = HudHandles[0];
	m_hud_level.bufferID = HudHandles[1];
	m_hud_points.bufferID = HudHandles[2];
	

	//////////////////////////////////////////////////////////////////
	// For Shadow
	glGenFramebuffers(1, &m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

	m_shadowW = 2048;
	m_shadowH = 2048;
	glGenTextures(1, &m_depthTexture);
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, m_shadowW, m_shadowH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthTexture, 0);
	glDrawBuffer(GL_NONE);
	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;
	//////////////////////////////////////////////////////////////////

	return true;
}

void CGame::ShutdownApp()
{
	if(m_man_Vertex)
		delete [] m_man_Vertex;
	if (m_man_Normal)
		delete[] m_man_Normal;
}


CObject* CGame::MakeBrick()
{
	if (m_blockNum <= 0)
		return NULL;
	CObject* brick = NULL;
	CSphere* sphere;
	CCube* cube;
	CMan* man;
	int ran = rand() % 2;

	switch (ran)
	{
	case SPHERE:
		brick = new CSphere();
		brick->GetRealClass(sphere, cube, man);
		sphere->SetRadius((float(rand()) / RAND_MAX) * 0.6f + 0.7f);
		break;
	case CUBE:
		brick = new CCube();
		brick->SetScale(float(rand()) / RAND_MAX + 1.2f,
			float(rand()) / RAND_MAX + 1.2f, float(rand()) / RAND_MAX + 1.2f);
		break;
	}

	float high = 0.0f;
	for (int i = 0; i < m_Objects_num; i++)
	{
		if (high < m_Objects[i]->m_fPosition[1] && m_Objects[i]->m_fScale[0] >0)
			high = m_Objects[i]->m_fPosition[1];
	}
	brick->m_gravity_on = false;
	if (high < 8)
		brick->SetPosition(0.0f, 10.0f, 0.0f);
	else
		brick->SetPosition(0.0f, high + 2, 0.0f);
	brick->SetColor(float(rand()) / RAND_MAX, float(rand()) / RAND_MAX, float(rand()) / RAND_MAX);
	brick->m_state = CONTROL;
	m_select_Object = brick;

	if(float(rand())/ RAND_MAX <= (100.f - m_clear_points) / 100.f + 0.05f)
	{
		man = new CMan();
		man->m_gravity_on = false;
		man->SetPosition((float(rand()) / RAND_MAX * 2 - 1) * 10, 1.3f, (float(rand()) / RAND_MAX * 2 - 1) * 10);
		man->SetColor(float(rand()) / RAND_MAX / 2 + 0.5f, float(rand()) / RAND_MAX * 0.5f, float(rand()) / RAND_MAX * 0.5f);
		man->SetScale(0.2f, 0.2f, 0.2f);
		man->SetRotation(float(rand()) / RAND_MAX * 3.141592f, 0.0f, 1.0f, 0.0f);
		man->m_state = ZOMBIE;
	}
	
	m_blockNum--;

	return brick;
}

void CGame::LevelControl()
{
	m_points = 0;
	switch (m_level)
	{
	case 1:
		m_blockNum = 30;
		m_clear_points = 40;
		break;

	case 2:
		m_blockNum = 25;
		m_clear_points = 60;
		break;

	case 3:
		m_blockNum = 20;
		m_clear_points = 80;
		break;

	case 4:
		m_blockNum = 15;
		m_clear_points = 100;
		break;
	}
}

void CGame::HudDisplay()
{
	char text[100];

	m_hud_level.SetColor(0.f, 0.f, 0.f);
	m_hud_level.SetPosition(10.f, m_nH - 25);
	sprintf(text, "Level: %d", m_level);
	m_hud_level.SetText(text);
	m_hud_level.DisplayHud(this);

	m_hud_points.SetColor(0.f, 0.f, 0.f);
	m_hud_points.SetPosition(m_nW / 2 - 5, m_nH - 25);
	sprintf(text, "%.0f", m_points);
	m_hud_points.SetText(text);
	m_hud_points.DisplayHud(this);

	m_hud_blocknum.SetColor(0.f, 0.f, 0.f);
	m_hud_blocknum.SetPosition(m_nW - 100, m_nH - 25);
	sprintf(text, "Blocks: %d", m_blockNum);
	m_hud_blocknum.SetText(text);
	m_hud_blocknum.DisplayHud(this);
}