// main.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	CMemoryPool<int> memPool(0);
	int *p[100];

	while (1)
	{
		for (int iCnt = 0; iCnt < 10; iCnt++)
		{
			p[iCnt] = memPool.Alloc();

			*p[iCnt] = iCnt;
			printf("value : %d Alloc : %d  Block : %d\n", *p[iCnt], memPool.GetAllocCount(), memPool.GetBlockCount());
		}

		Sleep(100);

		for (int iCnt = 0; iCnt < 10; iCnt++)
		{
			memPool.Free(p[iCnt]);
		}
	}

	return 0;
}