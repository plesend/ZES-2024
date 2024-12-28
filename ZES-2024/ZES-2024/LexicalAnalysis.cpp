#include "stdafx.h"

using namespace std;

namespace Lex
{
	int Oct_to_dec(char* s)
	{
		int res = 0;
		for (int i = 2; s[i] != NULL; ++i)
		{
			res *= 8;
			res += (s[i] - '0'); // ����������� ��������, ���������������� ������� ����� � ������������ ������� (��������� ���� ������� '0')
		}
		return res;
	}
	int Bin_to_dec(char* s)
	{
		int res = 0;
		for (int i = 2; s[i] != NULL; ++i)
		{
			res *= 2;
			res += (s[i] - '0');
		}
		return res;
	}

	void ITLog(IT::IdTable it, Log::LOG log)
	{
		int i, numberOP = 0;
		*log.stream << "\n������� ���������������:\n\n";
		*log.stream << "�   " << " | " << "�������������" << " | " << "��� ������" << " | " << "��� ��������������" << " | " << "������ � ��" << " | " << " ��������   " << endl;
		*log.stream << setw(87) << endl;
		for (i = 0; i < it.size; i++)
		{
			*log.stream << setfill('0') << setw(4) << right << i << "    ";
			*log.stream << setfill(' ') << setw(13) << left << it.table[i].id;
			switch (it.table[i].iddatatype)
			{
			case IT::INT: *log.stream << setw(10) << left;
				if (it.table[i].idtype == IT::OP)*log.stream << "-";
				else *log.stream << "integer   "; break;
			case IT::STR: *log.stream << setw(10) << left;
				if (it.table[i].idtype == IT::OP) *log.stream << "-";
				else *log.stream << "string    "; break;
			default: *log.stream << setw(10) << std::left << "unknown"; break;
			}
			switch (it.table[i].idtype)
			{
			case IT::V: *log.stream << left << "����������     "; break;
			case IT::F: *log.stream << left << "�������        "; break;
			case IT::P: *log.stream << left << "��������       "; break;
			case IT::L: *log.stream << left << "�������        "; break;
			case IT::OP: *log.stream << left << "��������       "; break;
				break;
			default: *log.stream << left << "unknown        "; break;
			}
			*log.stream << setw(11) << left << it.table[i].idxfirstLE;
			if (it.table[i].iddatatype == IT::INT && (it.table[i].idtype == IT::V || it.table[i].idtype == IT::L))
				*log.stream << setw(18) << left << it.table[i].value.vint;
			else if (it.table[i].iddatatype == IT::STR && (it.table[i].idtype == IT::V || it.table[i].idtype == IT::L))
				*log.stream << "[" << it.table[i].value.vstr->len << "]" << " \"" << it.table[i].value.vstr->str << "\" " << "";
			else if (it.table[i].idtype == IT::OP)
				*log.stream << setw(18) << left << it.table[i].value.op;
			else
				*log.stream << "       -       ";
			*log.stream << endl;
		}

		*log.stream << "\n���������� ���������������:\n " << i << endl;

	}


	void LTLog(LT::LexTable lt, Log::LOG log)
	{

		int line = 1;
		*log.stream << "\n";
		*log.stream << "������� ������:\n\n";
		*log.stream << line << "  ";
		for (int i = 0; i < lt.size; i++)
		{
			if (lt.table[i].lexema[0] == ';' || lt.table[i].lexema[0] == '[')
			{
				*log.stream << lt.table[i].lexema[0];
				line++;
				*log.stream << '\n';
				*log.stream << line << "  ";
				continue;
			}

			*log.stream << lt.table[i].lexema[0];
		}
	}

	LEX LexicalAnalysis(Log::LOG log, In::IN in, vector<ffst::FST>& fsts)
	{
		LEX lex;
		LT::LexTable lextable = LT::Create(LT_MAXSIZE);
		IT::IdTable idtable = IT::Create(TI_MAXSIZE);
		IT::Entry entryIT;

		int cHead = 0;
		int line = 1;
		int position = 0;
		int indxLex = 0;
		int indexID = 0;
		int cLit = 1;
		int cOperators = 1;


		bool findFunc = false;
		bool findInt = false;
		bool findString = false;
		bool findParm = false;
		bool findHead = false;
		bool findSameID = false;
		bool findNew = false;
		bool findBlock = false;
		bool findStandart = false;
		bool findUntil = false;

		char emptystr[] = "";
		char operators[] = "=+-*/%{}"; //������ ��� ��������� � ����������������� �����������

		char* L = new  char[2] { "L" };              // ������� ��� ���������
		char* OP = new  char[3] { "OP" };            // ������� ��� ����������
		char* bufL = new  char[TI_STR_MAXSIZE];     // ����� ��� ������������ ����� ��������
		char* nameLiteral = new  char[10] { "" };    // ��� ��������
		char* charclit = new char[10] { "" };        // ����� ��� ����������� ����� � ������
		char* prefix = new  char[10] { "" };         // ������� ��� ���������������
		char* bufprefix = new  char[10] { "" };      // ����� ��� ������������ ����� ��������������
		char* oldprefix = new  char[10] { "" };      // ������ �������, ������������ ��� ��������� ����������� �������


		auto addLexeme = [&line, &lextable, &indxLex](char lexeme, int priority = 0) -> void // ���������� � ���� ���� � �����������
			{
				LT::Entry entryLT = WriteEntry(entryLT, lexeme, LT_TI_NULLIDX, line);
				entryLT.priority = priority;
				indxLex++;
				LT::Add(lextable, entryLT);
			};

		char* word[max_word];  // ���������� ������� ���������� �� ������ (������ ����) �������� max_word
		for (int i = 0; i < max_word; i++)
			word[i] = new char[size_word] {NULL};
		// ������������ ��������� ������ ��� ������ ����� � �������.
		// ������ ����� ����� ��������� �� size_word �������� (������� ����������� ����), ������������� ������.

		for (int i = 0; i < max_word; i++)
			std::strcpy(word[i], SplitText((char*)in.text, in.size)[i]);
		// ���������� ������� ���� ����������, ����������� ��� ������ ������� SplitText.

// ��������� � SplitText:
// SplitText((char*)in.text, in.size) ��������� ����� in.text �� ��������� ����� � ���������� ������ ����� (���������� ��������).
// [i] ���������� � i-���� �������� ����� ������� �����, ������� ����� ���������� � i-��� ������ ������� word.

		for (int i = 0; i < max_word; i++) // �������� �� ����� ������
		{
			if (word[i][0] == '|')
			{
				position = 0;
				line++;
				continue;
			}
			position++;


			// new
			int fst{};
			for (; fst < fsts.size(); fst++)
				if (ffst::execute(fsts[fst], word[i])) //������������� �� ������� ����� �������, �������� �������� ���������.
					break;

			if (strcmp(word[i], emptystr) == 0 and fst == fsts.size()) // �������� �� ������ �����
				break;

			if (fst == fsts.size())
				throw ERROR_THROW_IN(5, line, position);

			switch (fsts.at(fst).lexeme) {
			case LEX_INTEGER: // same as LEX_STRING
				if (fsts[fst].type == IT::INT)
					findInt = true;
				if (fsts[fst].type == IT::STR)
					findString = true;
				break;

			case LEX_ID: {
				if (strlen(word[i]) > 7)
				{
					for (int j = strlen(word[i]) - 1; j > 5; j--) // �������
					{
						word[i][j] = NULL;
						sizeof(word[i])--;
					}
				}
				indxLex++;

				int idx = IT::IsId(idtable, word[i]); //���������� �� ������������� � ��� ���
				if (findFunc)
				{
					std::strcpy(oldprefix, prefix);
					std::strcpy(prefix, word[i]);
					if (!findFunc)
						findStandart = true;

				}
				else {
					if ((findBlock || findParm) && (strcmp(word[i], STANDART1) != 0 && strcmp(word[i], STANDART2) != 0))
					{
						if (!(idx != TI_NULLIDX && word[i + 1][0] == '('))     //���� ��� �� ����� �������
						{

							std::strcpy(bufprefix, prefix);
							word[i] = strcat(bufprefix, word[i]); // ������������ �������� � ������� ��
						}
					}
				}

				idx = IT::IsId(idtable, word[i]);
				if (idx != TI_NULLIDX)
				{
					if (findNew == true)				//�������� �� ��������� ����������
						throw ERROR_THROW_IN(6, line, position);

					LT::Entry entryLT = WriteEntry(entryLT, LEX_ID, idx, line);
					LT::Add(lextable, entryLT); // ���� ������ � ���� ������
					continue;
				}
				else {

					if (findNew && !findInt && !findString) // ���� ������ � ���� ������ var
						throw ERROR_THROW_IN(4, line, position);

					if (strcmp(word[i], STANDART1) == 0 || strcmp(word[i], STANDART2) == 0 || findFunc) // ���� ��� ����������� �������� ��� ���� ����
					{
						entryIT.idxfirstLE = indxLex; // ������ ������ ������� ������
						std::strcpy(entryIT.id, word[i]); // �������� �� � ������
						if (findString || strcmp(word[i], STANDART1) == 0 || strcmp(word[i], STANDART2) == 0)
							entryIT.iddatatype = IT::STR; // ��������� ����
						else { entryIT.iddatatype = IT::INT; }
						entryIT.idtype = IT::F; // ��������������� ��� �� ��� ��-��
						entryIT.value.vstr->len = 0; //��������������� ����� ������ �������� ������
						std::strcpy(entryIT.value.vstr->str, emptystr); //������������� ������ �������

						IT::Add(idtable, entryIT); //���������� � ���� ��
						idx = IT::IsId(idtable, word[i]); //�������� ������ ������������ ��������������
						indexID++; //������������� ������ ��������������

						LT::Entry entryLT = WriteEntry(entryLT, LEX_ID, idx, line); //������ � ���� �����
						LT::Add(lextable, entryLT);

						findFunc = false;
						findInt = false; //�������� �����, �������������� ��� ���������� ���� ������������� �������� �������
						findString = false;
						continue;
					}

					if (findInt || findString)
					{
						if (findParm)
						{
							entryIT.idtype = IT::P; //��� �� ��������������� ��� ��������, ����� ����������
						}
						else { entryIT.idtype = IT::V; }

						entryIT.idxfirstLE = indxLex;
						std::strcpy(entryIT.id, word[i]);
						if (findInt)
						{
							entryIT.iddatatype = IT::INT;
							entryIT.value.vint = 0;
						}
						else {
							entryIT.iddatatype = IT::STR;
							entryIT.value.vstr->len = 0;
							std::strcpy(entryIT.value.vstr->str, emptystr);
						}

						IT::Add(idtable, entryIT);
						int idx = IT::IsId(idtable, word[i]);
						indexID++;

						LT::Entry entryLT = WriteEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);


						if (findInt)findInt = false;
						else findString = false;
						findNew = false;
						continue;
					}
				}
			}

			case LEX_LITERAL: {
				if (fsts[fst].type == IT::INT) {
					indxLex++;
					_itoa_s(cLit++, charclit, sizeof(char) * 10, 10);
					std::strcpy(bufL, L);
					nameLiteral = strcat(bufL, (char*)charclit);

					int value;
					if (word[i][0] == 'O')
						value = Oct_to_dec(word[i]);
					else if (word[i][0] == 'B')
						value = Bin_to_dec(word[i]);
					else
						value = atoi((char*)word[i]);


					if (word[i - 1][0] == '-' && lextable.table[lextable.size - 1].lexema[0] != 'l' && lextable.table[lextable.size - 1].lexema[0] != 'i' && lextable.table[lextable.size - 1].lexema[0] != 'o')
						value = -value;//����������� ���� ��������
					for (int k = 0; k < idtable.size; k++)//����� ����������� ��������
					{
						if (idtable.table[k].idtype == IT::L and idtable.table[k].value.vint == value)
						{
							findSameID = true;
							LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, k, line);
							LT::Add(lextable, entryLT);
							break;
						}
					}


					if (findSameID)
					{
						findSameID = false; continue;
					}

					entryIT.value.vint = value; //��������� �������� �������� � ������ � ������� ��
					std::strcpy(entryIT.id, nameLiteral);
					entryIT.idxfirstLE = indxLex;
					entryIT.iddatatype = IT::INT;
					entryIT.idtype = IT::L;

					IT::Add(idtable, entryIT);
					indexID++;
					LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, idtable.size - 1, line);
					LT::Add(lextable, entryLT);


					continue;
				}
				else {
					indxLex++;

					int length = strlen(word[i]);
					for (int k = 0; k < length; k++)
						word[i][k] = word[i][k + 1]; //��� ������ ������ L
					word[i][length - 2] = 0; //��������� �������


					for (int k = 0; k < idtable.size; k++) //������ �����������
					{
						if (strcmp(idtable.table[k].value.vstr->str, word[i]) == 0)
						{
							findSameID = true;
							LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, k, line);
							LT::Add(lextable, entryLT);
							break;
						}
					}
					if (findSameID)
					{
						findSameID = false; continue;
					}

					_itoa_s(cLit++, charclit, sizeof(char) * 10, 10);
					std::strcpy(bufL, L); //�������� ������� � �����
					nameLiteral = strcat(bufL, (char*)charclit);

					entryIT.idxfirstLE = indxLex;
					std::strcpy(entryIT.id, nameLiteral);
					entryIT.iddatatype = IT::STR;
					entryIT.idtype = IT::L;

					entryIT.value.vstr->len = length - 2;
					std::strcpy(entryIT.value.vstr->str, word[i]);
					IT::Add(idtable, entryIT);
					indexID++;

					int idx = IT::IsId(idtable, word[i]);
					LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, idtable.size - 1, line);
					LT::Add(lextable, entryLT);

					continue;
				}
			}

			case LEX_HEAD:
				findHead = true;
				cHead++;
				std::strcpy(oldprefix, prefix);
				std::strcpy(prefix, word[i]);
				break;

			case LEX_UNTIL:
				findUntil = true;
				break;

			case LEX_FUNCTION:
				findFunc = true;
				break;

			case LEX_NEW:
				findNew = true;
				break;

			case LEX_LEFTBRACE:
				findBlock = true;
				break;

			case LEX_BRACELET:
				if (!findUntil) findBlock = false;
				else findUntil = false;
				break;

			case LEX_LEFTTHESIS:
				if (idtable.table[indexID - 1].idtype == IT::F) //�������� ����������� ��, ��� ��� �������
					findParm = true;
				break;

			case LEX_RIGHTTHESIS:
				if (findStandart) {				// ���� ������� ����������� ������� � �������� ������, ������ ������� �� ����������
					std::strcpy(prefix, oldprefix);
					findStandart = false;
				}
				findNew = false;
				findParm = false;
				break;

			case LEX_OPERATOR: {
				_itoa_s(cOperators++, charclit, sizeof(char) * 10, 10);
				std::strcpy(bufL, OP);
				nameLiteral = strcat(bufL, (char*)charclit);

				if (word[i][0] == '-' && lextable.table[lextable.size - 1].lexema[0] != 'i' && lextable.table[lextable.size - 1].lexema[0] != 'l')
					continue;
				for (int k = 0; k < idtable.size; k++)
				{
					if (idtable.table[k].value.op == word[i][0]) //���� �� �� � ����� �� ����������
					{
						findSameID = true;
						LT::Entry entryLT = WriteEntry(entryLT, LEX_OPERATOR, k, line);
						LT::Add(lextable, entryLT);
						break;
					}
				}
				if (findSameID) 	// ���� ������ ����� �� ���. �������
				{
					findSameID = false; continue;
				}
				entryIT.value.op = word[i][0]; //�������� ���� ��������� � ������
				std::strcpy(entryIT.id, nameLiteral);
				entryIT.idxfirstLE = indxLex;
				entryIT.iddatatype = IT::STR;
				entryIT.idtype = IT::OP;

				IT::Add(idtable, entryIT);
				indexID++;
				LT::Entry entryLT = WriteEntry(entryLT, LEX_OPERATOR, idtable.size - 1, line);
				entryLT.priority = 2;
				if (word[i][0] == '*' or word[i][0] == '/' or word[i][0] == '%')
					entryLT.priority = 3;
				LT::Add(lextable, entryLT);
				continue;
			}

			case LEX_RETURN:
			case LEX_PRINT:
			case LEX_SEPARATOR:
			case LEX_EQUAL:
				break;

			case LEX_COMMA:
				addLexeme(fsts[fst].lexeme, 1);
				continue;

			case LEX_COMMENT:
				continue;

			default:
				if (strcmp(word[i], emptystr) != 0) //���� ��  ������ ������
					throw ERROR_THROW_IN(5, line, position);
				break;
			}
			addLexeme(fsts[fst].lexeme);
		}

		if (!findHead) throw ERROR_THROW(2);
		if (cHead > 1) throw ERROR_THROW(3);

		lex.lextable = lextable;
		lex.idtable = idtable;
		return lex;
	}
}