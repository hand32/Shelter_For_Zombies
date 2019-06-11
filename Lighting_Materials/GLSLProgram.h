//	OpenGL Shading Language Program (Header)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr

#pragma once

#include <gl/glew.h>
#include <gl/gl.h>
#include <gl/glut.h>

enum GLSLShaderType {
	GAME_GL_VERTEX = GL_VERTEX_SHADER,
	GAME_GL_FRAGMENT = GL_FRAGMENT_SHADER,
	GAME_GL_GEOMETRY = GL_GEOMETRY_SHADER,
	GAME_GL_TESS_CONTROL = GL_TESS_CONTROL_SHADER,
	GAME_GL_TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
	GAME_GL_COMPUTE = GL_COMPUTE_SHADER
};

class CGLSLProgram
{
public:
	CGLSLProgram();
	~CGLSLProgram();
	char *LoadText(char *FileName);
	void Compile(char *FileName, GLSLShaderType nType);
	void Link();
	void Use();
	void Stop();

	void UniformMatrix3fv(char *Name, GLfloat *Mat);
	void UniformMatrix4fv(char *Name, GLfloat *Mat);
	void Uniform1f(char *Name, GLfloat f);
	void Uniform2f(char *Name, GLfloat f1, GLfloat f2);
	void Uniform3f(char *Name, GLfloat f1, GLfloat f2, GLfloat f3);
	void Uniform4f(char *Name, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4);

	GLuint m_Handle;
	bool m_bCreated;
};

