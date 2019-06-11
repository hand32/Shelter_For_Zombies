//	Bitmap(BMP) Open & Save (Header)
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr

unsigned char *ReadBmp(char *FileName, int *pW, int *pH);
bool SaveBmp(char *FileName, unsigned char  *Image1D, int nW, int nH);