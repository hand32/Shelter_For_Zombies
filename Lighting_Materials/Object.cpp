#include "Game.h"
#include "Object.h"
#include "Sphere.h"
#include "Cube.h"
#include "Man.h"

float GetDistance(CObject *o1, CObject *o2);
float GetPointDistance(float p1[3], float p2[3]);
float GetPointDistance(glm::vec3 p1, glm::vec3 p2);
CObject::CObject()
{
	material.shininess = 50.f;

	m_gravity_on = true;
	m_gravity_scale = 9.8f;

	m_fPosition[0] = 0.0f;
	m_fPosition[1] = 0.0f;
	m_fPosition[2] = 0.0f;

	m_fColor[0] = 0.0f;
	m_fColor[1] = 0.0f;
	m_fColor[2] = 0.0f;

	m_fScale[0] = 1.0f;
	m_fScale[1] = 1.0f;
	m_fScale[2] = 1.0f;

	m_fVelocity[0] = 0.0f;
	m_fVelocity[1] = 0.0f;
	m_fVelocity[2] = 0.0f;

	m_fAngle[0] = 0.0f;
	m_fAngle[1] = 0.0f;
	m_fAngle[2] = 1.0f;
	m_fAngle[3] = 0.0f;

	CGame::m_Objects[CGame::m_Objects_num++] = this;
}

CObject::CObject(float *fPosition, float *fSclae, float *fColor, float *fVelocity)
{
	m_gravity_on = true;
	m_gravity_scale = 9.8f;

	int i;
	for (i = 0; i < 3; i++)
		m_fPosition[i] = fPosition[i];

	for (i = 0; i < 3; i++)
		m_fScale[i] = m_fScale[i];

	for (i = 0; i < 3; i++)
		m_fColor[i] = fColor[i];

	for (i = 0; i < 3; i++)
		m_fVelocity[i] = fVelocity[i];

	CGame::m_Objects[CGame::m_Objects_num++] = this;
}

CObject::~CObject() {}

void CObject::SetPosition(float fX, float fY, float fZ)
{
	m_fPosition[0] = fX;
	m_fPosition[1] = fY;
	m_fPosition[2] = fZ;
}

void CObject::SetScale(float fX, float fY, float fZ)
{
	m_fScale[0] = fX;
	m_fScale[1] = fY;
	m_fScale[2] = fZ;
}

void CObject::SetRotation(float fAngle, float fX, float fY, float fZ)
{
	m_fAngle[0] = fAngle;
	m_fAngle[1] = fX;
	m_fAngle[2] = fY;
	m_fAngle[3] = fZ;
}

void CObject::SetColor(float fR, float fG, float fB)
{
	m_fColor[0] = fR;
	m_fColor[1] = fG;
	m_fColor[2] = fB;
}

void CObject::SetVelocity(float fVx, float fVy, float fVz)
{
	m_fVelocity[0] = fVx;
	m_fVelocity[1] = fVy;
	m_fVelocity[2] = fVz;
}

void CObject::Translate(float fX, float fY, float fZ)
{
	m_fPosition[0] += fX;
	m_fPosition[1] += fY;
	m_fPosition[2] += fZ;
}

float CObject::GetDistance(CObject *other)
{
	return sqrt(pow((this->m_fPosition[0] - other->m_fPosition[0]), 2) +
		pow((this->m_fPosition[1] - other->m_fPosition[1]), 2) +
		pow((this->m_fPosition[2] - other->m_fPosition[2]), 2));
}

float GetDistance(CObject *o1, CObject *o2)
{
	return sqrt(pow((o1->m_fPosition[0] - o2->m_fPosition[0]), 2) +
		pow((o1->m_fPosition[1] - o2->m_fPosition[1]), 2) +
		pow((o1->m_fPosition[2] - o2->m_fPosition[2]), 2));
}

float GetPointDistance(float p1[3], float p2[3])
{
	return sqrt(pow((p1[0] - p2[0]), 2) +
		pow((p1[1] - p2[1]), 2) +
		pow((p1[2] - p2[2]), 2));
}
float GetPointDistance(glm::vec3 p1, glm::vec3 p2)
{
	return sqrt(pow((p1.x - p2.x), 2) +
		pow((p1.y - p2.y), 2) +
		pow((p1.z - p2.z), 2));
}


void CObject::Move(int nElapsedTime)
{
	m_fPosition[0] += nElapsedTime * m_fVelocity[0] / 1000;
	m_fPosition[1] += nElapsedTime * m_fVelocity[1] / 1000;
	m_fPosition[2] += nElapsedTime * m_fVelocity[2] / 1000;
}

void CObject::Gravity(int nElapsedTime)
{
	if (m_gravity_on)
		m_fVelocity[1] += -m_gravity_scale * nElapsedTime / 1000.f;
}
bool Sphere_Sphere_Collide(CSphere *sphere1, CSphere *sphere2);
bool Sphere_Cube_Collide(CSphere *sphere, CCube *cube);
bool Cube_Cube_Collide(CCube *cube1, CCube *cube2);

bool CObject::Collide(CObject *other)
{
	CSphere *s1, *s2;
	CCube *c1, *c2;
	CMan *m1, *m2;
	this->GetRealClass(s1, c1, m1);
	other->GetRealClass(s2, c2, m2);

	if (m_type == SPHERE && other->m_type == SPHERE)
		return Sphere_Sphere_Collide(s1, s2);
	else if (m_type == SPHERE && other->m_type == CUBE)
		return Sphere_Cube_Collide(s1, c2);
	else if (m_type == CUBE && other->m_type == SPHERE)
		return Sphere_Cube_Collide(s2, c1);
	else if (m_type == CUBE && other->m_type == CUBE)
		return Cube_Cube_Collide(c1, c2);

	return false;
}

bool Sphere_Sphere_Collide(CSphere *sphere1, CSphere *sphere2)
{
	float distance = GetDistance(sphere1, sphere2);

	if (distance <= sphere1->m_dRadius + sphere2->m_dRadius)
	{
		if (sphere1->m_fVelocity[0] != 0 || sphere1->m_fVelocity[1] != 0 || sphere1->m_fVelocity[2] != 0)
		{
			glm::vec3 normalized = glm::normalize(glm::vec3(sphere1->m_fVelocity[0], sphere1->m_fVelocity[1], sphere1->m_fVelocity[2]));
			glm::vec3 s1p = glm::vec3(sphere1->m_fPosition[0], sphere1->m_fPosition[1], sphere1->m_fPosition[2]);
			glm::vec3 s2p = glm::vec3(sphere2->m_fPosition[0], sphere2->m_fPosition[1], sphere2->m_fPosition[2]);
			while(sphere1->m_dRadius + sphere2->m_dRadius - distance > 0.01f)
			{
				s1p -= normalized * 0.01f;
				distance = GetPointDistance(s1p, s2p);
			}
			sphere1->SetPosition(s1p.x, s1p.y, s1p.z);
		}
		return true;
	}
	return false;
}

glm::vec3 GetBoxPoint(CSphere *sphere, CCube *cube)
{
	float box_point[3];
	if (sphere->m_fPosition[0] < cube->m_fPosition[0] - cube->m_fScale[0] * 0.5f)
		box_point[0] = cube->m_fPosition[0] - cube->m_fScale[0] * 0.5f;
	else if (sphere->m_fPosition[0] > cube->m_fPosition[0] + cube->m_fScale[0] * 0.5f)
		box_point[0] = cube->m_fPosition[0] + cube->m_fScale[0] * 0.5f;
	else
		box_point[0] = sphere->m_fPosition[0];

	if (sphere->m_fPosition[1] < cube->m_fPosition[1] - cube->m_fScale[1] * 0.5f)
		box_point[1] = cube->m_fPosition[1] - cube->m_fScale[1] * 0.5f;
	else if (sphere->m_fPosition[1] > cube->m_fPosition[1] + cube->m_fScale[1] * 0.5f)
		box_point[1] = cube->m_fPosition[1] + cube->m_fScale[1] * 0.5f;
	else
		box_point[1] = sphere->m_fPosition[1];

	if (sphere->m_fPosition[2] < cube->m_fPosition[2] - cube->m_fScale[2] * 0.5f)
		box_point[2] = cube->m_fPosition[2] - cube->m_fScale[2] * 0.5f;
	else if (sphere->m_fPosition[2] > cube->m_fPosition[2] + cube->m_fScale[2] * 0.5f)
		box_point[2] = cube->m_fPosition[2] + cube->m_fScale[2] * 0.5f;
	else
		box_point[2] = sphere->m_fPosition[2];

	return glm::vec3(box_point[0], box_point[1], box_point[2]);
}
bool Sphere_Cube_Collide(CSphere *sphere, CCube *cube)
{
	glm::vec3 box_point_v;
	float box_point[3];
	float distance;

	box_point_v = GetBoxPoint(sphere, cube);
	box_point[0] = box_point_v.x;
	box_point[1] = box_point_v.y;
	box_point[2] = box_point_v.z;

	distance = GetPointDistance(box_point, sphere->m_fPosition);
	if (distance <= sphere->m_dRadius)
	{
		if (sphere->m_fVelocity[0] != 0 || sphere->m_fVelocity[1] != 0 || sphere->m_fVelocity[2] != 0)
		{
			glm::vec3 normalized = glm::normalize(glm::vec3(sphere->m_fVelocity[0], sphere->m_fVelocity[1], sphere->m_fVelocity[2]));
			glm::vec3 sp = glm::vec3(sphere->m_fPosition[0], sphere->m_fPosition[1], sphere->m_fPosition[2]);
			glm::vec3 bp = glm::vec3(box_point[0], box_point[1], box_point[2]);
			while (sphere->m_dRadius - distance > 0.01f)
			{
				sp -= normalized * 0.01f;
				distance = GetPointDistance(sp, bp);
				sphere->SetPosition(sp.x, sp.y, sp.z);
				bp = GetBoxPoint(sphere, cube);
			}
		}
		else if (cube->m_fVelocity[0] != 0 || cube->m_fVelocity[1] != 0 || cube->m_fVelocity[2] != 0)
		{
			glm::vec3 normalized = glm::normalize(glm::vec3(cube->m_fVelocity[0], cube->m_fVelocity[1], cube->m_fVelocity[2]));
			glm::vec3 sp = glm::vec3(sphere->m_fPosition[0], sphere->m_fPosition[1], sphere->m_fPosition[2]);
			glm::vec3 cp = glm::vec3(box_point[0], box_point[1], box_point[2]);
			while (sphere->m_dRadius - distance > 0.01f)
			{
				distance = GetPointDistance(sp, cp);
				cube->Translate(-normalized.x * 0.01f, -normalized.y * 0.01f, -normalized.z * 0.01f);
				cp = GetBoxPoint(sphere, cube);
			}
		}
		return true;
	}
	return false;
}

bool Cube_Cube_Collide(CCube *cube1, CCube *cube2)
{
	float x_d = abs(cube1->m_fPosition[0] - cube2->m_fPosition[0]);
	float y_d = abs(cube1->m_fPosition[1] - cube2->m_fPosition[1]);
	float z_d = abs(cube1->m_fPosition[2] - cube2->m_fPosition[2]);

	if (x_d <= cube1->m_fScale[0] * 0.5f + cube2->m_fScale[0] * 0.5f &&
		y_d <= cube1->m_fScale[1] * 0.5f + cube2->m_fScale[1] * 0.5f &&
		z_d <= cube1->m_fScale[2] * 0.5f + cube2->m_fScale[2] * 0.5f)
		return true;
	return false;
}