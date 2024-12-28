#pragma once
#include "Parm.h"
#include "Error.h"
#include "In.h"

namespace Log
{	// ��������� LOG ��� �������� ���������� � ���-�����
	struct LOG
	{
		wchar_t logfile[PARM_MAX_SIZE]; // ���� � ���-�����
		std::ofstream* stream;  // ��������� �� ����� ������
	};

	static const LOG INITLOG = { L"", NULL };// ��������� INITLOG - ������������������ ��������� LOG
	LOG getlog(wchar_t logfile[]);// ������� ��� ��������� ���������� ��������� LOG � ��������� ����� � ���-�����(������������� ��������� INITLOG ���������� LOG � ������ ����� � ���-����� � ������� ���������� �� �����.)
	// ������� ��� ������ ����� � ��� - ���� � ���������� ������ ����������
	void WriteLine(LOG log, const char* c, ...); //���������� ������ � ���������� ������ ���������� � ���-����.
	void WriteLine(LOG log, const wchar_t* c, ...);
	// ������� ��� ������ ���������� � ���-����(�����������, ���������, ������� ������, ������).
	void WriteLog(LOG log);
	void WriteParm(LOG log, Parm::PARM parm);
	void WriteIn(LOG log, In::IN in);
	void WriteError(LOG log, Error::ERROR error);

	void Close(LOG log);
};

