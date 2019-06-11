//	Load Obj Files (Implementation)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr

#include "Game.h"
#include "LinkedList.h"
#include <istream>

#pragma warning(disable: 4996)

bool LoadObj(char *FileName, GLfloat **Vertex, int *pVertexCnt, GLfloat **Normal)
{
	GLfloat* VertexList = new GLfloat[3000];
	GLfloat* NormalVertexList = new GLfloat[3000];
	//*Normal = new GLfloat[10000];
	//*Vertex = new GLfloat[10000];

	FILE *fp;
	int read, cnt = 0;
	int normalnum = 0;
	int vertexnum = 0;
	GLfloat x, y, z;
	char lineHeader[128];

	fp = fopen(FileName, "r");
	if (!fp) return false;

	while (!(feof(fp)))
	{
		fscanf(fp, "%s", lineHeader);
		if (strcmp(lineHeader, "v") == 0)
		{
			fscanf(fp, "%f %f %f\n", &x, &y, &z);
			VertexList[vertexnum * 3] = x;
			VertexList[vertexnum * 3 + 1] = y;
			VertexList[vertexnum * 3 + 2] = z;
			vertexnum++;
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			fscanf(fp, " %f %f %f\n", &x, &y, &z);
			NormalVertexList[cnt * 3] = x;
			NormalVertexList[cnt * 3 + 1] = y;
			NormalVertexList[cnt * 3 + 2] = z;
			cnt++;
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			unsigned int vertex[3], texture[3], normal[3];
			
			read = fscanf(fp, " %d/%d/%d %d/%d/%d %d/%d/%d",
				&vertex[0], &texture[0], &normal[0],
				&vertex[1], &texture[1], &normal[1],
				&vertex[2], &texture[2], &normal[2]);
			(*Vertex)[normalnum * 9] = VertexList[(vertex[0] - 1) * 3];
			(*Vertex)[normalnum * 9 + 1] = VertexList[(vertex[0] - 1) * 3 + 1];
			(*Vertex)[normalnum * 9 + 2] = VertexList[(vertex[0] - 1) * 3 + 2];
			(*Vertex)[normalnum * 9 + 3] = VertexList[(vertex[1] - 1) * 3];
			(*Vertex)[normalnum * 9 + 4] = VertexList[(vertex[1] - 1) * 3 + 1];
			(*Vertex)[normalnum * 9 + 5] = VertexList[(vertex[1] - 1) * 3 + 2];
			(*Vertex)[normalnum * 9 + 6] = VertexList[(vertex[2] - 1) * 3];
			(*Vertex)[normalnum * 9 + 7] = VertexList[(vertex[2] - 1) * 3 + 1];
			(*Vertex)[normalnum * 9 + 8] = VertexList[(vertex[2] - 1) * 3 + 2];

			(*Normal)[normalnum * 9] = NormalVertexList[(normal[0] - 1) * 3];
			(*Normal)[normalnum * 9 + 1] = NormalVertexList[(normal[0] - 1) * 3 + 1];
			(*Normal)[normalnum * 9 + 2] = NormalVertexList[(normal[0] - 1) * 3 + 2];
			(*Normal)[normalnum * 9 + 3] = NormalVertexList[(normal[1] - 1) * 3];
			(*Normal)[normalnum * 9 + 4] = NormalVertexList[(normal[1] - 1) * 3 + 1];
			(*Normal)[normalnum * 9 + 5] = NormalVertexList[(normal[1] - 1) * 3 + 2];
			(*Normal)[normalnum * 9 + 6] = NormalVertexList[(normal[2] - 1) * 3];
			(*Normal)[normalnum * 9 + 7] = NormalVertexList[(normal[2] - 1) * 3 + 1];
			(*Normal)[normalnum * 9 + 8] = NormalVertexList[(normal[2] - 1) * 3 + 2];
			normalnum++;
			for (int i = 0; ; i++)
			{
				read = fscanf(fp, " %d/%d/%d", &vertex[i % 2 + 1], &texture[i % 2 + 1], &normal[i % 2 + 1]);
				if (read == 3)
				{
					(*Vertex)[normalnum * 9] = VertexList[(vertex[0] - 1) * 3];
					(*Vertex)[normalnum * 9 + 1] = VertexList[(vertex[0] - 1) * 3 + 1];
					(*Vertex)[normalnum * 9 + 2] = VertexList[(vertex[0] - 1) * 3 + 2];
					(*Vertex)[normalnum * 9 + 3] = VertexList[(vertex[1] - 1) * 3];
					(*Vertex)[normalnum * 9 + 4] = VertexList[(vertex[1] - 1) * 3 + 1];
					(*Vertex)[normalnum * 9 + 5] = VertexList[(vertex[1] - 1) * 3 + 2];
					(*Vertex)[normalnum * 9 + 6] = VertexList[(vertex[2] - 1) * 3];
					(*Vertex)[normalnum * 9 + 7] = VertexList[(vertex[2] - 1) * 3 + 1];
					(*Vertex)[normalnum * 9 + 8] = VertexList[(vertex[2] - 1) * 3 + 2];

					(*Normal)[normalnum * 9] = NormalVertexList[(normal[0] - 1) * 3];
					(*Normal)[normalnum * 9 + 1] = NormalVertexList[(normal[0] - 1) * 3 + 1];
					(*Normal)[normalnum * 9 + 2] = NormalVertexList[(normal[0] - 1) * 3 + 2];
					(*Normal)[normalnum * 9 + 3] = NormalVertexList[(normal[1] - 1) * 3];
					(*Normal)[normalnum * 9 + 4] = NormalVertexList[(normal[1] - 1) * 3 + 1];
					(*Normal)[normalnum * 9 + 5] = NormalVertexList[(normal[1] - 1) * 3 + 2];
					(*Normal)[normalnum * 9 + 6] = NormalVertexList[(normal[2] - 1) * 3];
					(*Normal)[normalnum * 9 + 7] = NormalVertexList[(normal[2] - 1) * 3 + 1];
					(*Normal)[normalnum * 9 + 8] = NormalVertexList[(normal[2] - 1) * 3 + 2];
					normalnum++;
				}
				else
				{
					fscanf(fp, "\n");
					break;
				}
			}
		}
	}

	fclose(fp);


	*pVertexCnt = normalnum * 3;


	delete [] NormalVertexList;
	delete [] VertexList;

	return true;
}