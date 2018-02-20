// main.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

/*-------------------------------------------------------------------------------------------*/
//
// ������ �� �׽�Ʈ ���α׷� ����
//
// - MemoryPool, Lockfree Stack, Lockfree Queue
//
// �������� �����忡�� ���������� Alloc �� Free �� �ݺ������� ��
// ��� �����ʹ� 0x0000000055555555 ���� �ʱ�ȭ �Ǿ� ����.
//
// ������ 100,000 ���� st_TEST_DATA  �����͸� �޸�Ǯ�� �־��. 
//  ��� �����ʹ� Data - 0x0000000055555555 / Cound - 0 �ʱ�ȭ.
//
/*-------------------------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------------------------*/
// ������ �޸�Ǯ
/*-------------------------------------------------------------------------------------------*/
CMemoryPool<st_TEST_DATA>	g_MemoryPool;



/*-------------------------------------------------------------------------------------------*/
// �������ϸ� ������
/*-------------------------------------------------------------------------------------------*/
long						lInTPS = 0;
long						lOutTPS = 0;

long						lInCounter = 0;
long						lOutCounter = 0;



/*-------------------------------------------------------------------------------------------*/
// ������ �ڵ�
/*-------------------------------------------------------------------------------------------*/
HANDLE		hThread[dfTHREAD_MAX];
DWORD		dwThreadID;



/*===========================================================================================*/



/*-------------------------------------------------------------------------------------------*/
// ������ �޸�Ǯ �׽�Ʈ ������
/*-------------------------------------------------------------------------------------------*/
unsigned __stdcall MemoryPoolThread(void *pParam)
{
	st_TEST_DATA *pDataArray[dfTHREAD_ALLOC];

	while (1){
		///////////////////////////////////////////////////////////////////////////////////////
		// Alloc (10000��)
		///////////////////////////////////////////////////////////////////////////////////////
		for (int iCnt = 0; iCnt < dfTHREAD_ALLOC; iCnt++)
		{
			pDataArray[iCnt] = g_MemoryPool.Alloc();
			InterlockedIncrement((long *)&lOutCounter);
		}

		///////////////////////////////////////////////////////////////////////////////////////
		// 0x0000000055555555 �� �´��� Ȯ��
		///////////////////////////////////////////////////////////////////////////////////////
		for (int iCnt = 0; iCnt < dfTHREAD_ALLOC; iCnt++)
		{
			if ((0x0000000055555555 != pDataArray[iCnt]->lData) ||
				(0 != pDataArray[iCnt]->lCount))
				CCrashDump::Crash();
		}


		///////////////////////////////////////////////////////////////////////////////////////
		// Interlocked + 1 (Data + 1 / Count + 1)
		///////////////////////////////////////////////////////////////////////////////////////
		for (int iCnt = 0; iCnt < dfTHREAD_ALLOC; iCnt++)
		{
			InterlockedIncrement64((LONG64 *)&pDataArray[iCnt]->lData);
			InterlockedIncrement64((LONG64 *)&pDataArray[iCnt]->lCount);
		}


		///////////////////////////////////////////////////////////////////////////////////////
		// �ణ ���
		///////////////////////////////////////////////////////////////////////////////////////
		Sleep(dfSLEEP);


		///////////////////////////////////////////////////////////////////////////////////////
		// ������ 0x000000005555556, 1�� �´��� Ȯ��
		///////////////////////////////////////////////////////////////////////////////////////
		for (int iCnt = 0; iCnt < dfTHREAD_ALLOC; iCnt++)
		{
			if ((0x0000000055555556 != pDataArray[iCnt]->lData) ||
				(1 != pDataArray[iCnt]->lCount))
				CCrashDump::Crash();
		}


		///////////////////////////////////////////////////////////////////////////////////////
		// Interlocked - 1 (Data - 1 / Count - 1)
		///////////////////////////////////////////////////////////////////////////////////////
		for (int iCnt = 0; iCnt < dfTHREAD_ALLOC; iCnt++)
		{
			InterlockedDecrement64((LONG64 *)&pDataArray[iCnt]->lData);
			InterlockedDecrement64((LONG64 *)&pDataArray[iCnt]->lCount);
		}


		///////////////////////////////////////////////////////////////////////////////////////
		// �ణ ���
		///////////////////////////////////////////////////////////////////////////////////////
		Sleep(dfSLEEP);


		///////////////////////////////////////////////////////////////////////////////////////
		// 0x0000000055555555 / 0 �� �´��� Ȯ��
		///////////////////////////////////////////////////////////////////////////////////////
		for (int iCnt = 0; iCnt < dfTHREAD_ALLOC; iCnt++)
		{
			if ((0x0000000055555555 != pDataArray[iCnt]->lData) ||
				(0 != pDataArray[iCnt]->lCount))
				CCrashDump::Crash();
		}



		///////////////////////////////////////////////////////////////////////////////////////
		// Free
		///////////////////////////////////////////////////////////////////////////////////////
		for (int iCnt = 0; iCnt < dfTHREAD_ALLOC; iCnt++)
		{
			g_MemoryPool.Free(pDataArray[iCnt]);
			InterlockedIncrement((long *)&lInCounter);
		}
	}
}


void			InitMemoryPool()
{
	st_TEST_DATA *pDataArray[dfDATA_MAX];

	///////////////////////////////////////////////////////////////////////////////////////////
	// Alloc (10000��)
	// ������ ����(Ȯ��)
	///////////////////////////////////////////////////////////////////////////////////////////
	for (int iCnt = 0; iCnt < dfDATA_MAX; iCnt++)
		pDataArray[iCnt] = g_MemoryPool.Alloc();


	///////////////////////////////////////////////////////////////////////////////////////////
	// iData = 0x0000000055555555 ����
	// lCount = 0 ����
	///////////////////////////////////////////////////////////////////////////////////////////
	for (int iCnt = 0; iCnt < dfDATA_MAX; iCnt++)
	{
		pDataArray[iCnt]->lData = 0x0000000055555555;
		pDataArray[iCnt]->lCount = 0;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	// 2. �޸�Ǯ�� ����(��ȯ)
	///////////////////////////////////////////////////////////////////////////////////////////
	for (int iCnt = 0; iCnt < dfDATA_MAX; iCnt++)
		g_MemoryPool.Free(pDataArray[iCnt]);
}


void			TestMemoryPool()
{
	for (int iCnt = 0; iCnt < dfTHREAD_MAX; iCnt++)
	{
		hThread[iCnt] = (HANDLE)_beginthreadex(
			NULL,
			0,
			MemoryPoolThread,
			(LPVOID)0,
			0,
			(unsigned int *)&dwThreadID
			);
	}

	while (1)
	{
		lInTPS = lInCounter;
		lOutTPS = lOutCounter;

		lInCounter = 0;
		lOutCounter = 0;

		system("cls");
		wprintf(L"=====================================================================\n");
		wprintf(L"                        MemoryPool Testing...                        \n");
		wprintf(L"=====================================================================\n\n");

		wprintf(L"---------------------------------------------------------------------\n\n");
		wprintf(L"Alloc TPS		: %ld\n", lOutTPS);
		wprintf(L"Free  TPS		: %ld\n", lInTPS);
		wprintf(L"Alloc TPS		: %ld\n", g_MemoryPool.GetAllocCount());
		wprintf(L"---------------------------------------------------------------------\n\n\n");
		if (g_MemoryPool.GetAllocCount() > (dfTHREAD_MAX * dfTHREAD_ALLOC))
			CCrashDump::Crash();

		Sleep(999);
	}

}

void			TestLockfreeStack()
{

}

void			TestLockfreeQueue()
{

}

int				_tmain(int argc, _TCHAR* argv[])
{
	CCrashDump::CCrashDump();

	char		chSelectModel;

	do
	{
		system("cls");

		wprintf(L"=====================================================================\n");
		wprintf(L"                         Lockfree Model Test                         \n");
		wprintf(L"=====================================================================\n\n");

		wprintf(L"1. Lockfree MemoryPool\n\n");
		wprintf(L"2. Lockfree Stack\n\n");
		wprintf(L"3. Lockfree Queue\n\n");

		wprintf(L"=====================================================================\n");

		scanf_s("%c", &chSelectModel, sizeof(chSelectModel));
		fflush(stdin);

		switch (chSelectModel)
		{
		case '1':
			InitMemoryPool();
			TestMemoryPool();
			break;

		case '2':
			TestLockfreeStack();
			break;

		case '3':
			TestLockfreeQueue();
			break;

		default:
			continue;
			break;
		}

	} while (0);

	return 0;
}