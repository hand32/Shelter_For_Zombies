#pragma once
#include "Object.h"

class CGame;

class CMan : public CObject
{
public:
	CMan();
	CMan(float *fPosition, float *fScale, float *fColor, float *fVelocity, State character);
	~CMan();
	CCube* m_Collider;
	float hp;

	virtual void SetPosition(float fX, float fY, float fZ) override;
	virtual void SetScale(float fX, float fY, float fZ) override;
	virtual void SetRotation(float fAngle, float fX, float fY, float fZ) override;
	virtual void SetVelocity(float fVx, float fVy, float fVz) override;

	virtual void GetRealClass(CSphere*& sphere, CCube*& cube, CMan*& man) override;
	virtual void RenderScene() override;
	virtual void RenderShadow() override;
	virtual void Move(int nElapsedTime) override;

	bool IsInShadow();
};

