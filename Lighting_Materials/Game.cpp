//	Game Class Using OpenGL & Glut (Implementation Main), ver. 0.6
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr

#pragma warning(disable: 4996)

#include "Game.h"
#include "LoadObj.h"
#include "Bmp.h"

CGame::CGame(int nW, int nH, int nPosX, int nPosY)
{
	m_nW = nW;
	m_nH = nH;
	
	m_nPosX = nPosX;
	m_nPosY = nPosY;

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

	glutDisplayFunc(RenderSceneStatic);
	glutReshapeFunc(ResizeStatic);
	glutKeyboardFunc(KeyDownStatic);
	glutSpecialFunc(SpecialInputStatic);
	glutMouseFunc(MouseStatic);

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
	CSphere *newSphere = new CSphere();
	newSphere->SetPosition(10.0f, 5.0f, 0.0f);
	newSphere->SetScale(1.0f, 1.0f, 1.0f);
	newSphere->SetColor(1.0f, 1.0f, 1.0f);
	newSphere->SetRadius(1.0f);
	newSphere->SetVelocity(1.0f, -1.0f, 0.0f);
	newSphere->m_gravity_on = false;

	CCube *newCube = new CCube();
	newCube->SetPosition(-10.0f, 10.0f, 0.0f);
	newCube->SetScale(1.0f, 1.0f, 1.0f);
	newCube->SetColor(1.0f, 0.0f, 0.0f);

	newCube = new CCube();
	newCube->SetPosition(0.0f, 0.0f, 0.0f);
	newCube->SetScale(50.0f, 2.0f, 50.0f);
	newCube->SetColor(0.8f, 0.2f, 1.0f);;
	newCube->m_gravity_on = false;
	newCube->m_state = GROUND;

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
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(m_dLookAt[0][0], m_dLookAt[0][1], m_dLookAt[0][2],
		m_dLookAt[1][0], m_dLookAt[1][1], m_dLookAt[1][2],
		m_dLookAt[2][0], m_dLookAt[2][1], m_dLookAt[2][2]);
	
	
	m_worldLight.Position = glm::vec4(50.0 * cos(m_nCurrentTime / 800.0), 30.0, 50.0 * sin(m_nCurrentTime / 800.0), 1.0);
	m_worldLight.La = glm::vec3(0.3f, 0.3f, 0.3f);
	m_worldLight.Ld = glm::vec3(1.0f, 1.0f, 1.0f);
	m_worldLight.Ls = glm::vec3(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < m_Objects_num; i++)
	{
		for (int j = 0; j < m_Objects_num; j++)
		{
			if (i != j && m_Objects[i]->Collide(m_Objects[j]) == true)
			{
				if (m_Objects[i]->m_type != MAN)
				{
					m_Objects[i]->m_gravity_on = false;
					m_Objects[i]->SetVelocity(0.0f, 0.0f, 0.0f);
					if (m_Objects[i] == m_select_Object)
						MakeBrick();
				}
				break;
			}
		}
		m_Objects[i]->Gravity(nElapsedTime);
		m_Objects[i]->Move(nElapsedTime);
		m_Objects[i]->Render();
	}


	m_nBaseTime = glutGet(GLUT_ELAPSED_TIME);

	glutSwapBuffers();
	glutPostRedisplay();
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

void CGame::Resize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-24, 24, -18, 18, 0.1f, 60.0f);
	//gluPerspective(60, (double)width / (double)height, 0.1, 80.0);
	m_ProjectionMatrix = glm::ortho(-24.f, 24.f, -18.f, 18.f, 0.1f, 60.0f);
}

void CGame::Mouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		
	}
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
	glEnable(GL_DEPTH_TEST);

	if (!LoadObj("man.obj", &positionData, &m_nMan_VertexCnt, &normalData))
		return false;
	if (!LoadObj("cube.obj", &cubeVertex, &m_nCube_VertexCnt, &cubeNormal))
		return false;

	m_Glsl[0].Compile("phong.vsl", GAME_GL_VERTEX);
	m_Glsl[0].Compile("phong.fsl", GAME_GL_FRAGMENT);
	m_Glsl[0].Link();

	m_Glsl[1].Compile("flat.vsl", GAME_GL_VERTEX);
	m_Glsl[1].Compile("flat.fsl", GAME_GL_FRAGMENT);
	m_Glsl[1].Link();

	const double PI = 3.14159;
	int slice, stack;
	double sliceAngle1, stackAngle1;
	double sliceAngle2, stackAngle2;
	int nStack = 30, nSlice = 30;
	double sliceStep = 2 * PI / nSlice;
	double stackStep = PI / nStack;
	double radius = 1;

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

	glBindVertexArray(0);
	
	delete[] positionData;
	delete[] normalData;
	delete[] m_man_Vertex;
	delete[] m_man_Normal;
	delete[] cubeVertex;
	delete[] cubeNormal;
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
		sphere->SetRadius((float(rand()) / RAND_MAX) * 0.5f + 0.5f);
		break;
	case CUBE:
		brick = new CCube();
		brick->SetScale(float(rand()) / RAND_MAX + 0.5f,
			float(rand()) / RAND_MAX + 0.5f, float(rand()) / RAND_MAX + 0.5f);
		break;
	}

	brick->m_gravity_on = false;
	brick->SetPosition(0.0f, 10.0f, 0.0f);
	brick->SetColor(float(rand()) / RAND_MAX, float(rand()) / RAND_MAX, float(rand()) / RAND_MAX);
	brick->m_state = BUILDING;
	m_select_Object = brick;

	man = new CMan();
	man->m_gravity_on = false;
	man->SetPosition((float(rand()) / RAND_MAX * 2 - 1) * 10, 5.f, (float(rand()) / RAND_MAX * 2 - 1) * 10);
	man->SetColor(float(rand()) / RAND_MAX, float(rand()) / RAND_MAX, float(rand()) / RAND_MAX);
	man->SetScale(0.2f, 0.2f, 0.2f);
	man->SetRotation(float(rand()) / RAND_MAX * 360.f, 0.0f, 1.0f, 0.0f);
	
	return brick;
}