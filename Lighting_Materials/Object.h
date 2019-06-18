#pragma once
#include <glm/glm.hpp>

class CGame;
class CSphere;
class CCube;
class CMan;

enum Type {
	SPHERE,
	CUBE,
	MAN
};

enum State {
	BUILDING,
	GROUND,
	CONTROL,
	ZOMBIE,
	HUMAN
};
struct Material {
	float specular[3];
	float shininess;
};

class CObject
{
public:
	Type m_type;
	State m_state;

	Material material;

	float m_fPosition[3];
	float m_fScale[3];
	float m_fColor[3];
	float m_fVelocity[3];
	float m_fAngle[4];

	bool m_gravity_on;
	float m_gravity_scale;

	CObject();
	CObject(float *fPosition, float *fScale, float *fColor, float *fvelocity);
	~CObject();

	void SetPosition(float fX, float fY, float fZ);
	void SetScale(float fX, float fY, float fZ);
	void SetRotation(float fAngle, float fX, float fY, float fZ);
	void SetColor(float fR, float fG, float fB);
	void SetVelocity(float fVx, float fVy, float fVz);

	void Translate(float fX, float fY, float fZ);

	float GetDistance(CObject *other);

	//Velocity ���� �̵�.
	void Move(int nElapsedTime);
	//���� ������ ���� �߷� ����.
	void Gravity(int nElapsedTime);
	//�� Object Ư���� ���� �浹������.
	bool Collide(CObject *other);
	//�� Object Ư���� ���� Render ��.
	virtual void RenderScene() = 0;
	virtual void RenderShadow() = 0;
	virtual void GetRealClass(CSphere*& sphere, CCube*& cube, CMan*& man) = 0;
};