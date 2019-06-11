#pragma once

#define MAX_TEXT	500

class CGame;

class CHud
{
public:
	float m_fPosition[2];
	float m_fTextColor[3];
	char m_Text[MAX_TEXT];

	CHud();
	CHud(float *fPosition, float *fTextColor);
	~CHud();

	void SetPosition(float fX, float fY);
	void SetColor(float fR, float fG, float fB);
	void SetText(char *text);
	void DisplayHud(CGame *pGame);
};

