
#pragma warning(disable: 4996)

#include "Game.h"
#include "Hud.h"


CHud::CHud() 
{
	m_fPosition[0] = 0.f;
	m_fPosition[1] = 0.f;

	m_fTextColor[0] = 0.f;
	m_fTextColor[1] = 0.f;
	m_fTextColor[2] = 0.f;

	m_Text[0] = '\0';
}

CHud::CHud(float *fPosition, float *fTextColor)
{
	int i;
	for(i = 0 ; i < 2 ; i++)
		m_fPosition[i] = fPosition[i];

	for(i = 0 ; i < 3 ; i++)
		m_fTextColor[i] = fTextColor[i];

	m_Text[0] = '\0';
}

CHud::~CHud()
{
}

void CHud::SetPosition(float fX, float fY)
{
	m_fPosition[0] = fX;
	m_fPosition[1] = fY;
}

void CHud::SetColor(float fR, float fG, float fB)
{
	m_fTextColor[0] = fR;
	m_fTextColor[1] = fG;
	m_fTextColor[2] = fB;
}

void CHud::SetText(char *text)
{
	strcpy(m_Text, text);
}

void CHud::DisplayHud(CGame *pGame)
{
	CGame::DrawBitmapText(m_Text, m_fPosition[0], m_fPosition[1], 0, m_fTextColor[0], m_fTextColor[1], m_fTextColor[2], pGame->m_nW, pGame->m_nH, true);
}

