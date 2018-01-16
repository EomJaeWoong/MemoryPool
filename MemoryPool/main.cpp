// main.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	CMemoryPool<int> memPool(100);
	int *p[100];

	while (1)
	{
		for (int iCnt = 0; iCnt < 100; iCnt++)
		{
			p[iCnt] = memPool.Alloc();

			*p[iCnt] = iCnt;
			printf("%d \n", *p[iCnt]);
		}

		Sleep(100);

		for (int iCnt = 0; iCnt < 100; iCnt++)
		{
			memPool.Free(p[iCnt]);
		}
	}

	return 0;
}