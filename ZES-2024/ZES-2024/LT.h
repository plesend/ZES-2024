#pragma once

#define LEXEMA_FIXSIZE	1			
#define LT_MAXSIZE		4096		
#define LT_TI_NULLIDX	0xfffffff	//��� �������� ������ ���������������
#define LEX_INTEGER		't'	
#define LEX_STRING		't'	
#define LEX_ID			'i'	
#define LEX_LITERAL		'l'	
#define LEX_FUNCTION	'f'	
#define LEX_NEW			'v' 	
#define LEX_RETURN		'r'	
#define LEX_PRINT		'w'	
#define LEX_HEAD		'm' 	
#define LEX_SEPARATOR   ';'	
#define LEX_COMMA		','	
#define LEX_LEFTBRACE	'['	
#define LEX_BRACELET	']'	
#define LEX_LEFTTHESIS	'('	
#define LEX_RIGHTTHESIS	')'	
#define LEX_PLUS		'o'	
#define LEX_MINUS		'o'	
#define LEX_STAR		'o'	
#define LEX_DIRSLASH	'o'	
#define LEX_PROC		'o'	
#define LEX_OPERATOR	'o'	
#define LEX_EQUAL		'='
#define LEX_UNTIL		'u'
#define LEX_COMMENT		'c'
#define LEX_RIGHT		'o' //����� ������
#define LEX_LEFT		'o' //����� �����

namespace LT {
	struct Entry {								//������ ������� ������
		char lexema[LEXEMA_FIXSIZE];			//�������
		int sn;									//����� ������ � ���. ������
		int idxTI;								//������ � ������� ��������������� ��� LT_TI_NULLIDX
		int priority;							//��������� ���������
	};
	struct LexTable								//��������� ������� ������
	{
		int maxsize;							//������� ������� ������ <LT_MAXSIZE
		int size = 0;								//������� ������ ������� ������ <maxsize
		Entry* table;							//������ ����� ������� ������
	};

	LexTable Create(int size);					//������� ������� ������ (������� ������� ������ <LT_MAXSIZE)
	void Add(LexTable& lextable, Entry entry);	//�������� ������ � ������� ������
	Entry GetEntry(LexTable& lextable, int n);	//�������� ������ ������� ������
	void Delete(LexTable& lextable);			//������� ������� ������
	Entry WriteEntry(Entry& entry, char lexema, int indx, int line); //����������� ������ ������� ������
}

