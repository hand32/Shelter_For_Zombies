
#pragma warning(disable: 4996)

#include "Game.h"
#include "Sphere.h"


CSphere::CSphere() :CObject()
{
 	m_type = SPHERE;

	m_dRadius = 0.7;
}

CSphere::CSphere(float *fPosition, float *fScale, float *fColor, float *fVelocity, double dRadius)
	: CObject(fPosition, fScale, fColor, fVelocity)
{
	m_type = SPHERE;

	m_dRadius = dRadius;
}

CSphere::~CSphere()
{
}

void CSphere::SetRadius(double radius)
{
	m_dRadius = radius;
}

void CSphere::Render()
{
	CGLSLProgram* m_Glsl = &CGame::pInstance->m_Glsl[0];
	m_Glsl->Use();

	double(*m_dLookAt)[3];
	m_dLookAt = CGame::pInstance->m_dLookAt;

	glm::mat4 m_ProjectionMatrix = CGame::pInstance->m_ProjectionMatrix;
	glm::mat4 m_View = glm::lookAt(glm::vec3(m_dLookAt[0][0], m_dLookAt[0][1], m_dLookAt[0][2]),
		glm::vec3(m_dLookAt[1][0], m_dLookAt[1][1], m_dLookAt[1][2]),
		glm::vec3(m_dLookAt[2][0], m_dLookAt[2][1], m_dLookAt[2][2]));

	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(m_fScale[0] * m_dRadius * 2, m_fScale[1] * m_dRadius * 2, m_fScale[2] * m_dRadius * 2));
	glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), m_fAngle[0], glm::vec3(m_fAngle[1], m_fAngle[2], m_fAngle[3]));
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(m_fPosition[0], m_fPosition[1], m_fPosition[2]));
	Model = translate * rotate * scale * Model;

	glm::mat4 MV = m_View * Model;
	glm::mat4 m_Mvp;

	m_Mvp = m_ProjectionMatrix * m_View*Model;

	glm::mat3 m_Normal = glm::mat3(glm::vec3(MV[0]), glm::vec3(MV[1]), glm::vec3(MV[2]));

	m_Glsl->UniformMatrix4fv("MVP", &m_Mvp[0][0]);
	m_Glsl->UniformMatrix4fv("ProjectionMatrix", &m_ProjectionMatrix[0][0]);
	m_Glsl->UniformMatrix4fv("ModelViewMatrix", &MV[0][0]);
	m_Glsl->UniformMatrix3fv("NormalMatrix", &m_Normal[0][0]);

	LightInfo worldlight = CGame::pInstance->m_worldLight;
	glm::vec4 LightPosition = m_View * worldlight.Position;
	m_Glsl->Uniform4f("Light.Position", LightPosition[0], LightPosition[1], LightPosition[2], LightPosition[3]);
	m_Glsl->Uniform3f("Material.Kd", m_fColor[0], m_fColor[1], m_fColor[2]);
	m_Glsl->Uniform3f("Light.Ld", worldlight.Ld[0], worldlight.Ld[1], worldlight.Ld[2]);
	m_Glsl->Uniform3f("Material.Ka", m_fColor[0], m_fColor[1], m_fColor[2]);
	m_Glsl->Uniform3f("Light.La", worldlight.La[0], worldlight.La[1], worldlight.La[2]);
	m_Glsl->Uniform3f("Material.Ks", m_fColor[0], m_fColor[1], m_fColor[2]);
	m_Glsl->Uniform3f("Light.Ls", worldlight.Ls[0], worldlight.Ls[1], worldlight.Ls[2]);
	m_Glsl->Uniform1f("Material.Shininess", material.shininess);

	glBindVertexArray(CGame::pInstance->m_VaoHandle);
	glDrawArrays(GL_TRIANGLES, CGame::pInstance->m_nMan_VertexCnt, CGame::pInstance->m_nSphere_VertexCnt);
	glBindVertexArray(0);

	m_Glsl->Stop();
	
}

void CSphere::GetRealClass(CSphere*& sphere, CCube*& cube, CMan*& man)
{
	sphere = this;
}