//	OpenGL Shading Language Program (Implementation)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr

#include "GLSLProgram.h"
#include <cstdio>

#pragma warning(disable: 4996)

CGLSLProgram::CGLSLProgram()
{
	m_Handle = 0;
	m_bCreated = false;
}

CGLSLProgram::~CGLSLProgram()
{
	if (!m_Handle) return;

	GLint nNumShaders = 0;
	glGetProgramiv(m_Handle, GL_ATTACHED_SHADERS, &nNumShaders);

	GLuint *ShaderNames = new GLuint[nNumShaders];
	glGetAttachedShaders(m_Handle, nNumShaders, NULL, ShaderNames);

	for (int i = 0; i < nNumShaders; i++)
		glDeleteShader(ShaderNames[i]);

	glDeleteProgram(m_Handle);

	delete[] ShaderNames;
}

char *CGLSLProgram::LoadText(char *FileName)
{
	FILE *fp = fopen(FileName, "r");

	if(!fp)	return NULL;

	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if(size == 0) {
		fclose(fp);
		return NULL;
	}

	char *code = new char[size + 1];

	fread(code, 1, size, fp);

	fclose(fp);

	while (size-- >= 1) {
		if (code[size - 1] == '}')
		{
			code[size] = '\0';
			break;
		}
	}

	return code;
}

void CGLSLProgram::Compile(char *FileName, GLSLShaderType nType)
{
	char *code = LoadText(FileName);
	if(!code) return;

	if(m_Handle <= 0)
		m_Handle = glCreateProgram();

	GLuint ShaderHandle = glCreateShader(nType);

	glShaderSource(ShaderHandle, 1, (const char**)&code, NULL);
	glCompileShader(ShaderHandle);

	int nResult = GL_FALSE;
	glGetShaderiv(ShaderHandle, GL_COMPILE_STATUS, &nResult);

	if(nResult == GL_FALSE) {
		int nLength = 0;
		glGetShaderiv(ShaderHandle, GL_INFO_LOG_LENGTH, &nLength);

		if (nLength > 0) {
            char *msg = new char[nLength];
            int nReadLength = 0;
            glGetShaderInfoLog(ShaderHandle, nLength, &nReadLength, msg);
			printf("%s\n", msg);
            delete[] msg;
        }

		glDeleteShader(ShaderHandle);
		printf("Compile error.\n");
	}
	else {
		glAttachShader(m_Handle, ShaderHandle);
		glDeleteShader(ShaderHandle);
	}

	delete [] code;
}

void CGLSLProgram::Link()
{
	if(m_bCreated) return;
	if(m_Handle <= 0) return;

	glLinkProgram(m_Handle);

	int nResult = GL_FALSE;
	glGetProgramiv(m_Handle, GL_LINK_STATUS, &nResult);
	if(nResult == GL_FALSE) {
		int nLength = 0;
		glGetShaderiv(m_Handle, GL_INFO_LOG_LENGTH, &nLength);

		if (nLength > 0) {
            char *msg = new char[nLength];
            int nReadLength = 0;
            glGetShaderInfoLog(m_Handle, nLength, &nReadLength, msg);
			printf("%s\n", msg);
            delete[] msg;
        }

		printf("Link error.\n");
	}
	else {
		m_bCreated = true;
	}
}

void CGLSLProgram::Use()
{
	if(!m_bCreated) return;
	if(m_Handle <= 0) return;

	glUseProgram(m_Handle);
}

void CGLSLProgram::Stop()
{
	glUseProgram(0);
}

void CGLSLProgram::UniformMatrix3fv(char *Name, GLfloat *Mat)
{
	GLint Handle = glGetUniformLocation(m_Handle, Name);
	glUniformMatrix3fv(Handle, 1, GL_FALSE, Mat);
}

void CGLSLProgram::UniformMatrix4fv(char *Name, GLfloat *Mat)
{
	GLint Handle = glGetUniformLocation(m_Handle, Name);
	glUniformMatrix4fv(Handle, 1, GL_FALSE, Mat);
}

void CGLSLProgram::Uniform1f(char *Name, GLfloat f)
{
	GLint Handle = glGetUniformLocation(m_Handle, Name);
	glUniform1f(Handle, f);
}

void CGLSLProgram::Uniform2f(char *Name, GLfloat f1, GLfloat f2)
{
	GLint Handle = glGetUniformLocation(m_Handle, Name);
	glUniform2f(Handle, f1, f2);
}

void CGLSLProgram::Uniform3f(char *Name, GLfloat f1, GLfloat f2, GLfloat f3)
{
	GLint Handle = glGetUniformLocation(m_Handle, Name);
	glUniform3f(Handle, f1, f2, f3);
}

void CGLSLProgram::Uniform4f(char *Name, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4)
{
	GLint Handle = glGetUniformLocation(m_Handle, Name);
	glUniform4f(Handle, f1, f2, f3, f4);
}
