#pragma once
#include "Object.h"

class CGame;

class CMan : public CObject
{
public:
	CMan();
	CMan(float *fPosition, float *fScale, float *fColor, float *fVelocity, State character);
	~CMan();

	virtual void GetRealClass(CSphere*& sphere, CCube*& cube, CMan*& man) override;
	virtual void Render() override;
};

