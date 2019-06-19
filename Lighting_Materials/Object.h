#pragma once
#include <glm/glm.hpp>

class CGame;
class CSphere;
class CCube;
class CMan;

enum Type {
	SPHERE,
	CUBE,
	MAN,
	MANCOLLIDER
};

enum State {
	BUILDING,
	GROUND,
	CONTROL,
	ZOMBIE
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
	float m_fColor[4];
	float m_fVelocity[3];
	float m_fAngle[4];

	bool m_gravity_on;
	float m_gravity_scale;

	CObject();
	CObject(float *fPosition, float *fScale, float *fColor, float *fvelocity);
	~CObject();

	virtual void SetPosition(float fX, float fY, float fZ);
	virtual void SetScale(float fX, float fY, float fZ);
	virtual void SetRotation(float fAngle, float fX, float fY, float fZ);
	virtual void SetColor(float fR, float fG, float fB);
	virtual void SetVelocity(float fVx, float fVy, float fVz);

	void Translate(float fX, float fY, float fZ);

	float GetDistance(CObject *other);

	//Velocity 따라 이동.
	virtual void Move(int nElapsedTime);

	//현재 설정에 따라 중력 적용.
	void Gravity(int nElapsedTime);

	//각 Object 특성에 따라 충돌판정함.
	bool Collide(CObject *other);

	bool CObject::RayCast(float x1, float y1, float z1, float x2, float y2, float z2);
	static bool RayCast(float x1, float y1, float z1, float x2, float y2, float z2, CObject* object);

	//각 Object 특성에 따라 Render 함.
	virtual void RenderScene() = 0;
	virtual void RenderShadow() = 0;
	virtual void GetRealClass(CSphere*& sphere, CCube*& cube, CMan*& man) = 0;
};