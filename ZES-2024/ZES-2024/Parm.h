#pragma once
#include "stdafx.h"
#define PARM_IN		L"-in:"
#define	PARM_OUT	L"-out:"
#define	PARM_LOG	L"-log:"
#define	PARM_MAX_SIZE	300 //максимальный размер строк
#define	PARM_OUT_DEFAULT_EXIT	L".out"
#define	PARM_LOG_DEFAULT_EXIT	L".log"

namespace Parm
{
	struct PARM // параметры коандной строки
	{
		wchar_t in[PARM_MAX_SIZE]; //путь к входному файлу
		wchar_t out[PARM_MAX_SIZE]; //путь к выходному
		wchar_t log[PARM_MAX_SIZE]; // путь к файлу лога
	};
	PARM getparm(int argc, wchar_t* argv[]); //получение параметров командной строки(принимает аргументы командной строки). ¬озврат-структура с извлечЄнными параметрами
}
