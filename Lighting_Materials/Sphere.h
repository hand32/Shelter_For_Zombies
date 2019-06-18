#pragma once
#include "Object.h"

class CGame;

class CSphere : public CObject
{
public:
	double m_dRadius;

	CSphere();
	CSphere(float *fPosition, float *fScale, float *fColor, float *fVelocity, double dRadius);
	~CSphere();

	void SetRadius(double radius);

	virtual void GetRealClass(CSphere*& sphere, CCube*& cube, CMan*& man) override;
	virtual void RenderScene() override;
	virtual void RenderShadow() override;
};

