#include "Cube.h"
#include "Game.h"
CCube::CCube() : CObject()
{
	m_type = CUBE;
}

CCube::CCube(float *fPosition, float *fScale, float *fColor, float *fVelocity)
	: CObject(fPosition, fScale, fColor, fVelocity)
{
	m_type = CUBE;
}

CCube::~CCube() {}

void CCube::RenderScene()
{
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
	m_Glsl->Uniform3f("Material.Kd", m_fColor[0], m_fColor[1], m_fColor[2]);
	m_Glsl->Uniform3f("Light.Ld", worldlight.Ld[0], worldlight.Ld[1], worldlight.Ld[2]);
	m_Glsl->Uniform3f("Material.Ka", m_fColor[0], m_fColor[1], m_fColor[2]);
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
	glDrawArrays(GL_TRIANGLES, (CGame::pInstance->m_nMan_VertexCnt + CGame::pInstance->m_nSphere_VertexCnt), CGame::pInstance->m_nCube_VertexCnt);
	glBindVertexArray(0);
}

void CCube::GetRealClass(CSphere*& sphere, CCube*& cube, CMan*& man)
{
	cube = this;
}


void CCube::RenderShadow()
{
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
	depthModel = rotate * translate * scale * depthModel;

	glm::mat4 depthProjection = CGame::pInstance->m_DepthProjectionMatrix;
		glm::mat4 depthMVP = depthProjection * depthView * depthModel;
	m_Glsl->UniformMatrix4fv("depthMVP", &depthMVP[0][0]);


	//glViewport(0, 0, (CGame::pInstance->m_nW), CGame::pInstance->m_nH);
	glBindVertexArray(CGame::pInstance->m_VaoHandle);
	glDrawArrays(GL_TRIANGLES, (CGame::pInstance->m_nMan_VertexCnt + CGame::pInstance->m_nSphere_VertexCnt), CGame::pInstance->m_nCube_VertexCnt);
	glBindVertexArray(0);

}