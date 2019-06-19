#include "Man.h"
#include "Game.h"

CMan::CMan() : CObject()
{
	m_Collider = new CCube();
	m_Collider->m_type = MANCOLLIDER;
	m_Collider->m_gravity_on = false;
	m_type = MAN;
	hp = 100;
}

CMan::CMan(float *fPosition, float *fScale, float *fColor, float *fVelocity, State character)
	: CObject(fPosition, fScale, fColor, fVelocity)
{
	m_Collider = new CCube();
	m_Collider->m_type = MANCOLLIDER;
	m_Collider->m_gravity_on = false;
	m_state = character;
	m_type = MAN;
	hp = 100;
}

CMan::~CMan() {}

void CMan::SetPosition(float fX, float fY, float fZ)
{
	CObject::SetPosition(fX, fY, fZ);
	m_Collider->SetPosition(m_fPosition[0], m_fPosition[1] + m_fScale[1] * 5, m_fPosition[2]);
}
void CMan::SetScale(float fX, float fY, float fZ)
{
	CObject::SetScale(fX, fY, fZ);
	m_Collider->SetScale(m_fScale[0] * 5, m_fScale[1] * 12, m_fScale[2] * 3);
}
void CMan::SetRotation(float fAngle, float fX, float fY, float fZ)
{
	CObject::SetRotation(fAngle, fX, fY, fZ);
	m_Collider->SetRotation(m_fAngle[0], m_fAngle[1], m_fAngle[2], m_fAngle[3]);
}
void CMan::SetVelocity(float fVx, float fVy, float fVz)
{
	CObject::SetVelocity(fVx, fVy, fVz);
	m_Collider->SetVelocity(m_fVelocity[0], m_fVelocity[1], m_fVelocity[2]);
}

void CMan::GetRealClass(CSphere*& sphere, CCube*& cube, CMan*& man)
{
	man = this;
}

void CMan::RenderScene()
{
	glDisable(GL_CULL_FACE);
	glViewport(0, 0, CGame::pInstance->m_nW, CGame::pInstance->m_nH);
	CGLSLProgram* m_Glsl;
	m_Glsl = &CGame::pInstance->m_Glsl[0];
	double(*m_dLookAt)[3];
	m_dLookAt = CGame::pInstance->m_dLookAt;
	LightInfo worldlight = CGame::pInstance->m_worldLight;


	glm::mat4 m_ProjectionMatrix = CGame::pInstance->m_ProjectionMatrix;
	glm::mat4 m_View = CGame::pInstance->m_View;

	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(m_fScale[0], m_fScale[1], m_fScale[2]));
	glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), m_fAngle[0], glm::vec3(m_fAngle[1], m_fAngle[2], m_fAngle[3]));
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(m_fPosition[0], m_fPosition[1], m_fPosition[2]));
	Model = translate * rotate * scale * Model;

	glm::mat4 MV = m_View * Model;
	glm::mat4 m_Mvp;

	m_Mvp = m_ProjectionMatrix * m_View*Model;

	glm::mat3 m_Normal = glm::mat3(glm::vec3(MV[0]), glm::vec3(MV[1]), glm::vec3(MV[2]));

	glm::mat4 biasMatrix{
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	};
	glm::mat4 depthView =
		glm::lookAt(glm::vec3(worldlight.Position),
			glm::vec3(0., 0., 0.),
			glm::vec3(0., 1., 0.));
	glm::mat4 depthModel = glm::mat4(1.0f);
	depthModel = translate * rotate * scale * depthModel;
	glm::mat4 depthProjection = CGame::pInstance->m_DepthProjectionMatrix;
	glm::mat4 depthMVP = depthProjection * depthView * depthModel;
	glm::mat4 depthBiasMVP = biasMatrix * depthMVP;

	m_Glsl->UniformMatrix4fv("MVP", &m_Mvp[0][0]);
	m_Glsl->UniformMatrix4fv("ProjectionMatrix", &m_ProjectionMatrix[0][0]);
	m_Glsl->UniformMatrix4fv("ModelViewMatrix", &MV[0][0]);
	m_Glsl->UniformMatrix3fv("NormalMatrix", &m_Normal[0][0]);

	glm::vec4 LightPosition = m_View * worldlight.Position;
	m_Glsl->Uniform4f("Light.Position", LightPosition[0], LightPosition[1], LightPosition[2], LightPosition[3]);
	m_Glsl->Uniform3f("Material.Kd", m_fColor[0] * m_fColor[3], m_fColor[1] * m_fColor[3], m_fColor[2] * m_fColor[3]);
	m_Glsl->Uniform3f("Light.Ld", worldlight.Ld[0], worldlight.Ld[1], worldlight.Ld[2]);
	m_Glsl->Uniform3f("Material.Ka", m_fColor[0] * m_fColor[3], m_fColor[1] * m_fColor[3], m_fColor[2] * m_fColor[3]);
	m_Glsl->Uniform3f("Light.La", worldlight.La[0], worldlight.La[1], worldlight.La[2]);
	m_Glsl->Uniform3f("Material.Ks", material.specular[0], material.specular[1], material.specular[2]);
	m_Glsl->Uniform3f("Light.Ls", worldlight.Ls[0], worldlight.Ls[1], worldlight.Ls[2]);
	m_Glsl->Uniform1f("Material.Shininess", material.shininess);
	m_Glsl->UniformMatrix4fv("DepthBiasMVP", &depthBiasMVP[0][0]);
	GLuint ShadowMapID = glGetUniformLocation(m_Glsl->m_Handle, "shadowMap");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, CGame::pInstance->m_depthTexture);
	glUniform1i(ShadowMapID, 0);

	glBindVertexArray(CGame::pInstance->m_VaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, CGame::pInstance->m_nMan_VertexCnt);
	glBindVertexArray(0);
	
}

void CMan::RenderShadow()
{
	glDisable(GL_CULL_FACE);
	glViewport(0, 0, CGame::pInstance->m_shadowW, CGame::pInstance->m_shadowH);
	CGLSLProgram* m_Glsl;
	m_Glsl = &CGame::pInstance->m_Glsl[1];
	LightInfo worldlight = CGame::pInstance->m_worldLight;

	glm::mat4 biasMatrix{
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	};
	glm::mat4 depthView =
		glm::lookAt(glm::vec3(worldlight.Position),
			glm::vec3(0., 0., 0.),
			glm::vec3(0., 1., 0.));
	glm::mat4 depthModel = glm::mat4(1.0f);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(m_fScale[0], m_fScale[1], m_fScale[2]));
	glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), m_fAngle[0], glm::vec3(m_fAngle[1], m_fAngle[2], m_fAngle[3]));
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(m_fPosition[0], m_fPosition[1], m_fPosition[2]));
	depthModel = translate * rotate * scale * depthModel;

	glm::mat4 depthProjection = CGame::pInstance->m_DepthProjectionMatrix;
	glm::mat4 depthMVP = depthProjection * depthView * depthModel;
	m_Glsl->UniformMatrix4fv("depthMVP", &depthMVP[0][0]);

	glBindVertexArray(CGame::pInstance->m_VaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, CGame::pInstance->m_nMan_VertexCnt);
	glBindVertexArray(0);
}

void CMan::Move(int nElapsedTime)
{
	if (hp > 0 && !IsInShadow())
	{
		glm::vec3 velocity = normalize(glm::vec3(m_fVelocity[0] + (float(rand()) / RAND_MAX * 2.0f - 1.0f) * 0.2f, 0, m_fVelocity[2] + (float(rand()) / RAND_MAX * 2.0f - 1.0f) * 0.2f));
		float theta = acosf(dot(velocity, glm::vec3(0, 0, 1)));
		if (acosf(dot(velocity, glm::vec3(1, 0, 0))) > 3.141592f / 2)
			theta = -theta;
		SetRotation(theta, 0, 1, 0);
		SetVelocity(velocity.x, 0, velocity.z);
		CObject::Move(nElapsedTime);
		m_Collider->SetPosition(m_fPosition[0], m_fPosition[1] + m_fScale[1] * 5, m_fPosition[2]);
		hp -= 2 * float(nElapsedTime) / 1000;
		m_fColor[3] = hp / 100;
	}
	else if(hp > 0)
	{
		hp += 15 * float(nElapsedTime) / 1000;
		m_fColor[3] = hp / 100;
		CGame::pInstance->m_points += 2.0f * nElapsedTime / 1000;
	}
	m_fPosition[1] = 1.3f;
}

bool CMan::IsInShadow()
{
	CGame *cgame = CGame::pInstance;
	LightInfo light = cgame->m_worldLight;
	for (int i = 0; i < cgame->m_Objects_num; i++)
	{
		if (cgame->m_Objects[i]->m_state == BUILDING && 
			cgame->m_Objects[i]->RayCast(light.Position.x, light.Position.y, light.Position.z, m_fPosition[0], m_fPosition[1], m_fPosition[2]))
		{
  			return true;
		}
	}
	return false;
}