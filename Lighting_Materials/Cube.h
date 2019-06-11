#pragma once
#include "Object.h"

class CGame;

class CCube : public CObject
{
public:
	CCube();
	CCube(float *fPosition, float *fScale, float *fColor, float *fVelocity);
	~CCube();

	virtual void Render() override;
	virtual void GetRealClass(CSphere*& sphere, CCube*& cube, CMan*& man) override;
};

