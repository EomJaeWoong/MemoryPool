// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>
#include <Windows.h>

#include <psapi.h>
#include <dbghelp.h>
#include <crtdbg.h>
#include <tlhelp32.h>
#include <strsafe.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

#pragma comment(lib, "DbgHelp.Lib")
#pragma comment(lib, "ImageHlp")
#pragma comment(lib, "psapi")

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "APIHook.h"
#include "CrashDump.h"
#include "MemoryPool.h"
#include "main.h"