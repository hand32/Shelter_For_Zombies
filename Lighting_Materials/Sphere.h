#pragma once
#include "Object.h"

class CGame;

class CSphere : public CObject
{
public:
	double m_dRadius;

	GLUquadric *m_Quad;
	int m_nSlice, m_nStack;

	CSphere();
	CSphere(float *fPosition, float *fScale, float *fColor, float *fVelocity, double dRadius);
	~CSphere();

	void SetRadius(double radius);

	virtual void GetRealClass(CSphere*& sphere, CCube*& cube, CMan*& man) override;
	virtual void Render() override;
};

